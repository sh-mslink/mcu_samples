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

//����������Ϣ����ָ��
static rt_mq_t mqForLed2 = RT_NULL;
static rt_mq_t mqForLed3 = RT_NULL;
//������������ָ��
static rt_mailbox_t mbForLed2Ack = RT_NULL;
static rt_mailbox_t mbForLed3Ack = RT_NULL;

ALIGN(RT_ALIGN_SIZE)
//���忪��LED���ַ���ָ��
static char LedOnCmd[] = "On"; //!= {'O','n'};��һ��'\0'
static char LedOffCmd[] = "Off";

//����һ��ö���ͱ���������������Ϣ���׷���˭
static enum ThreadID {MainThread=1, Led2Thread, Led3Thread, Unknown};

//������Ϣ�ṹ��
static struct ledMsg
{
	enum ThreadID forWitchthread; //������Ϣ���ĸ��̴߳���
	char info[4];  //��Ϣ����
	rt_mailbox_t ack; //�����������Ϊ��Ϣ��Ӧ��
};

static void led2MsgThread(void)
{
	struct ledMsg msg;
	
	while(1)
	{
		//�ȴ������̷߳��͸�Led2�̵߳���Ϣ
		rt_mq_recv(mqForLed2, &msg, sizeof(struct ledMsg), RT_WAITING_FOREVER);
		//�ж���Ϣ�еı���Ƿ���ȷ
		if(msg.forWitchthread == Led2Thread)
		{
			//�Ƚ���Ϣ�����е��ַ�������
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
		//�ȴ������̷߳��͸�Led2�̵߳���Ϣ		
		rt_mq_recv(mqForLed3, &msg, sizeof(struct ledMsg), RT_WAITING_FOREVER);
		if(msg.forWitchthread == Led3Thread)
		{
			//�Ƚ���Ϣ�����е��ַ�������			
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
	
	//����һ����Ϣ���и�Led2�߳�ʹ��
	mqForLed2 = rt_mq_create("mqLed2", sizeof(struct ledMsg), MAX_MSG_NUM, RT_IPC_FLAG_FIFO);
	if(mqForLed2 == RT_NULL)
	{
		rt_kprintf("Led2 msg create failed~!\n");
		return -1;
	}
	
	//����һ����Ϣ���и�Led3�߳�ʹ��
	mqForLed3 = rt_mq_create("mqLed3", sizeof(struct ledMsg), MAX_MSG_NUM, RT_IPC_FLAG_FIFO);
	if(mqForLed3 == RT_NULL)
	{
		rt_kprintf("Led3 msg create failed~!\n");
		return -1;
	}
	
	//����һ��������и�Led2�߳�Ӧ��ʹ��
	mbForLed2Ack=rt_mb_create("mbLed2", MAX_MSG_NUM*4, RT_IPC_FLAG_FIFO);
	if(mbForLed2Ack == RT_NULL)
	{
		rt_kprintf("mbForLed2 mailbox create failed~!\n");
		return -1;
	}
	
	//����һ��������и�Led3�߳�Ӧ��ʹ��
	mbForLed3Ack=rt_mb_create("mbLed3", MAX_MSG_NUM*4, RT_IPC_FLAG_FIFO);
	if(mbForLed3Ack == RT_NULL)
	{
		rt_kprintf("mbForLed3 mailbox create failed~!\n");
		return -1;
	}
	
	//����Led2�߳�
	tid = rt_thread_create("led2MsgThread", (void *)led2MsgThread, (void *)RT_NULL, LED2_STACK_SIZE, LED2_PRIO, LED2_TICKS);
	RT_ASSERT(tid != RT_NULL);
	rt_thread_startup(tid);
	//����Led3�߳�
	tid = rt_thread_create("led3MsgThread", (void *)led3MsgThread, (void *)RT_NULL, LED3_STACK_SIZE, LED3_PRIO, LED3_TICKS);
	RT_ASSERT(tid != RT_NULL);
	rt_thread_startup(tid);
	
	return RT_EOK;
}

void mainMsgTest(void)
{
	uint16_t count;
	struct ledMsg msg={0};
	
	//������Ϣ���ƣ���Led2Thread��˸10��
	for(count=0;count<10;count++)
	{
		uint32_t value;
		
		//��ʼ����Ϣ����
		msg.forWitchthread=Led2Thread;
		strcpy(msg.info, LedOnCmd);
		msg.ack=mbForLed2Ack;
		//������Ϣ��Led2�߳�
		rt_mq_send(mqForLed2, &msg, sizeof(struct ledMsg));
		//�ȴ�������Led2�̵߳�Ӧ��
		rt_mb_recv(mbForLed2Ack, &value, RT_WAITING_FOREVER);
		//����յ�Ӧ���ж�Ӧ��ֵ�Ƿ���ȷ
		if(value == ACK_FROM_LED2)
		{
			//��ʼ����Ϣ����
			msg.forWitchthread=Led2Thread;
			strcpy(msg.info, LedOffCmd);
			msg.ack=mbForLed2Ack;
			//������Ϣ��Led2�߳�
			rt_mq_send(mqForLed2, &msg, sizeof(struct ledMsg));
			//�ȴ�������Led2�̵߳�Ӧ��
			rt_mb_recv(mbForLed2Ack, &value, RT_WAITING_FOREVER);
		}
		else
			rt_kprintf("get ACK error from LED2 thread~!\n");
	}
	
	//������Ϣ���ƣ���Led3Thread��˸10��
	for(count=0;count<10;count++)
	{
		uint32_t value;
		//��ʼ����Ϣ����		
		msg.forWitchthread=Led3Thread;
		strcpy(msg.info, LedOnCmd);
		msg.ack=mbForLed3Ack;
		//������Ϣ��Led3�߳�		
		rt_mq_send(mqForLed3, &msg, sizeof(struct ledMsg));
		rt_mb_recv(mbForLed3Ack, &value, RT_WAITING_FOREVER);
		//����յ�Ӧ���ж�Ӧ��ֵ�Ƿ���ȷ		
		if(value == ACK_FROM_LED3)
		{
			//��ʼ����Ϣ����			
			msg.forWitchthread=Led3Thread;
			strcpy(msg.info, LedOffCmd);
			msg.ack=mbForLed3Ack;
			//������Ϣ��Led3�߳�			
			rt_mq_send(mqForLed3, &msg, sizeof(struct ledMsg));
			//�ȴ�������Led2�̵߳�Ӧ��			
			rt_mb_recv(mbForLed3Ack, &value, RT_WAITING_FOREVER);
		}
		else
			rt_kprintf("get ACK error from LED3 thread~!\n");
	}

}