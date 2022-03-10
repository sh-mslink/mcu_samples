#include <rtthread.h>
#include <rtdevice.h>
#include <string.h>
#include "board.h"
#include "drv_gpio.h"

#define LED2_PRIO 8
#define LED3_PRIO 9
#define LED2_STACK_SIZE 256
#define LED3_STACK_SIZE 256
#define LED2_TICKS 2
#define LED3_TICKS 2

#define MAX_MSG_NUM 32

#define ACK_FROM_LED2 0x1
#define ACK_FROM_LED3 0x2

//定义两个消息队列指针
static rt_mq_t mqForLed2 = RT_NULL;
static rt_mq_t mqForLed3 = RT_NULL;
//定义两个邮箱指针
static rt_mailbox_t mbForLed2Ack = RT_NULL;
static rt_mailbox_t mbForLed3Ack = RT_NULL;

ALIGN(RT_ALIGN_SIZE)
//定义开关LED的字符串指令
static char LedOnCmd[] = "On"; //!= {'O','n'};多一个'\0'
static char LedOffCmd[] = "Off";

//定义一个枚举型变量，用来区分消息到底发给谁
static enum ThreadID {MainThread=1, Led2Thread, Led3Thread, Unknown};

//定义消息结构体
static struct ledMsg
{
	enum ThreadID forWitchthread; //区分消息由哪个线程处理
	char info[4];  //消息内容
	rt_mailbox_t ack; //用邮箱机制作为消息的应答
};

static void led2MsgThread(void)
{
	struct ledMsg msg;
	
	while(1)
	{
		//等待其他线程发送给Led2线程的消息
		rt_mq_recv(mqForLed2, &msg, sizeof(struct ledMsg), RT_WAITING_FOREVER);
		//判断消息中的标记是否正确
		if(msg.forWitchthread == Led2Thread)
		{
			//比较消息内容中的字符串命令
			if(strcmp(msg.info, LedOnCmd)==0)
			{
				rt_pin_write(LED2_PIN, PIN_LOW);
				rt_thread_mdelay(500);
				rt_mb_send(msg.ack, ACK_FROM_LED2);
			}
			else if(strcmp(msg.info, LedOffCmd) == 0)
			{
				rt_pin_write(LED2_PIN, PIN_HIGH);
				rt_thread_mdelay(500);
				rt_mb_send(msg.ack, ACK_FROM_LED2);
			}
		}
	}
}

static void led3MsgThread(void)
{
	struct ledMsg msg;
	
	while(1)
	{
		//等待其他线程发送给Led2线程的消息		
		rt_mq_recv(mqForLed3, &msg, sizeof(struct ledMsg), RT_WAITING_FOREVER);
		if(msg.forWitchthread == Led3Thread)
		{
			//比较消息内容中的字符串命令			
			if(strcmp(msg.info, LedOnCmd)==0)
			{
				rt_pin_write(LED3_PIN, PIN_LOW);
				rt_thread_mdelay(500);
				rt_mb_send(msg.ack, ACK_FROM_LED3);
			}
			else if(strcmp(msg.info, LedOffCmd) == 0)
			{
				rt_pin_write(LED3_PIN, PIN_HIGH);
				rt_thread_mdelay(500);
				rt_mb_send(msg.ack, ACK_FROM_LED3);
			}
		}
	}
}

int msgThreadCreate(void)
{
	rt_thread_t tid = RT_NULL;
	
	//创建一个消息队列给Led2线程使用
	mqForLed2 = rt_mq_create("mqLed2", sizeof(struct ledMsg), MAX_MSG_NUM, RT_IPC_FLAG_FIFO);
	if(mqForLed2 == RT_NULL)
	{
		rt_kprintf("Led2 msg create failed~!\n");
		return -1;
	}
	
	//创建一个消息队列给Led3线程使用
	mqForLed3 = rt_mq_create("mqLed3", sizeof(struct ledMsg), MAX_MSG_NUM, RT_IPC_FLAG_FIFO);
	if(mqForLed3 == RT_NULL)
	{
		rt_kprintf("Led3 msg create failed~!\n");
		return -1;
	}
	
	//创建一个邮箱队列给Led2线程应答使用
	mbForLed2Ack=rt_mb_create("mbLed2", MAX_MSG_NUM*4, RT_IPC_FLAG_FIFO);
	if(mbForLed2Ack == RT_NULL)
	{
		rt_kprintf("mbForLed2 mailbox create failed~!\n");
		return -1;
	}
	
	//创建一个邮箱队列给Led3线程应答使用
	mbForLed3Ack=rt_mb_create("mbLed3", MAX_MSG_NUM*4, RT_IPC_FLAG_FIFO);
	if(mbForLed3Ack == RT_NULL)
	{
		rt_kprintf("mbForLed3 mailbox create failed~!\n");
		return -1;
	}
	
	//创建Led2线程
	tid = rt_thread_create("led2MsgThread", (void *)led2MsgThread, (void *)RT_NULL, LED2_STACK_SIZE, LED2_PRIO, LED2_TICKS);
	RT_ASSERT(tid != RT_NULL);
	rt_thread_startup(tid);
	//创建Led3线程
	tid = rt_thread_create("led3MsgThread", (void *)led3MsgThread, (void *)RT_NULL, LED3_STACK_SIZE, LED3_PRIO, LED3_TICKS);
	RT_ASSERT(tid != RT_NULL);
	rt_thread_startup(tid);
	
	return RT_EOK;
}

void mainMsgTest(void)
{
	uint16_t count;
	struct ledMsg msg={0};
	
	//利用消息机制，让Led2Thread闪烁10次
	for(count=0;count<10;count++)
	{
		uint32_t value;
		
		//初始化消息内容
		msg.forWitchthread=Led2Thread;
		strcpy(msg.info, LedOnCmd);
		msg.ack=mbForLed2Ack;
		//发送消息给Led2线程
		rt_mq_send(mqForLed2, &msg, sizeof(struct ledMsg));
		//等待来自于Led2线程的应答
		rt_mb_recv(mbForLed2Ack, &value, RT_WAITING_FOREVER);
		//如果收到应答判断应答值是否正确
		if(value == ACK_FROM_LED2)
		{
			//初始化消息内容
			msg.forWitchthread=Led2Thread;
			strcpy(msg.info, LedOffCmd);
			msg.ack=mbForLed2Ack;
			//发送消息给Led2线程
			rt_mq_send(mqForLed2, &msg, sizeof(struct ledMsg));
			//等待来自于Led2线程的应答
			rt_mb_recv(mbForLed2Ack, &value, RT_WAITING_FOREVER);
		}
		else
			rt_kprintf("get ACK error from LED2 thread~!\n");
	}
	
	//利用消息机制，让Led3Thread闪烁10次
	for(count=0;count<10;count++)
	{
		uint32_t value;
		//初始化消息内容		
		msg.forWitchthread=Led3Thread;
		strcpy(msg.info, LedOnCmd);
		msg.ack=mbForLed3Ack;
		//发送消息给Led3线程		
		rt_mq_send(mqForLed3, &msg, sizeof(struct ledMsg));
		rt_mb_recv(mbForLed3Ack, &value, RT_WAITING_FOREVER);
		//如果收到应答判断应答值是否正确		
		if(value == ACK_FROM_LED3)
		{
			//初始化消息内容			
			msg.forWitchthread=Led3Thread;
			strcpy(msg.info, LedOffCmd);
			msg.ack=mbForLed3Ack;
			//发送消息给Led3线程			
			rt_mq_send(mqForLed3, &msg, sizeof(struct ledMsg));
			//等待来自于Led2线程的应答			
			rt_mb_recv(mbForLed3Ack, &value, RT_WAITING_FOREVER);
		}
		else
			rt_kprintf("get ACK error from LED3 thread~!\n");
	}

}