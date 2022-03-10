#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "drv_gpio.h"

#define LED2_PRIO 8
#define LED3_PRIO 9
#define LED2_STACK_SIZE 256
#define LED3_STACK_SIZE 256
#define LED2_TICKS 2
#define LED3_TICKS 2

//����LED2�Ļ�����ָ��
static rt_mutex_t led2Mutex = RT_NULL;
//����LED3�Ļ�����ָ��
static rt_mutex_t led3Mutex = RT_NULL;

//���߳�ֻ���LED2�����ƿ���˸
static void led2MutexThread(void)
{
	uint16_t count;
	while(1)
	{
		//��ȡLED2�Ļ���������LED2��˸����
		rt_mutex_take(led2Mutex,RT_WAITING_FOREVER);
		for(count=0;count<10;count++)
		{
			rt_pin_write(LED2_PIN, PIN_LOW);
			rt_thread_mdelay(50);
			rt_pin_write(LED2_PIN, PIN_HIGH);
			rt_thread_mdelay(50);
		}
		rt_mutex_release(led2Mutex);
	}  
	rt_kprintf("Goodby~!\n");
}

//���߳�ֻ���LED3�����ƿ���˸
static void led3MutexThread(void)
{
	uint16_t count;
	while(1)
	{
		//��ȡLED3�Ļ���������LED3��˸����
		rt_mutex_take(led3Mutex,RT_WAITING_FOREVER);
		for(count=0;count<10;count++)
		{
			rt_pin_write(LED3_PIN, PIN_LOW);
			rt_thread_mdelay(50);
			rt_pin_write(LED3_PIN, PIN_HIGH);
			rt_thread_mdelay(50);
		}
		rt_mutex_release(led3Mutex);
	}  
	rt_kprintf("Goodby~!\n");
}

//�����ò��ԵĻ��������߳�
int mutexThreadCreate(void)
{
	rt_thread_t tid;
	
	//����һ��������������LED2�Ĳ���
	led2Mutex=rt_mutex_create("LED2",RT_IPC_FLAG_FIFO);
	if(led2Mutex == RT_NULL)
	{
		rt_kprintf("create led2Mutex failed~!\n");
		return -1;
	}
	//����һ��������������LED3�Ĳ���
	led3Mutex=rt_mutex_create("LED3",RT_IPC_FLAG_FIFO);
	if(led3Mutex == RT_NULL)
	{
		rt_kprintf("create led3Mutex failed~!\n");
		return -1;
	}
	
	//����һ���̣߳���������LED2
	tid = rt_thread_create("LED2Thread", led2MutexThread, RT_NULL, LED2_STACK_SIZE, LED2_PRIO, LED2_TICKS);
	RT_ASSERT(tid != RT_NULL);
	rt_thread_startup(tid);
	//����һ���̣߳���������LED3
	tid = rt_thread_create("LED3Thread", led3MutexThread, RT_NULL, LED3_STACK_SIZE, LED3_PRIO, LED3_TICKS);
	RT_ASSERT(tid != RT_NULL);
	rt_thread_startup(tid);
	
	return RT_EOK;
}

//�ú��������̵߳��ã��ȿ���LED2����˸���ڿ���LED3����˸
void mainMutexTest(void)
{
	uint16_t count;

	//��ȡLED2�Ļ���������LED2��˸����
	rt_mutex_take(led2Mutex,RT_WAITING_FOREVER);
	for(count=0;count<10;count++)
	{
		rt_pin_write(LED2_PIN, PIN_LOW);
		rt_thread_mdelay(500);
		rt_pin_write(LED2_PIN, PIN_HIGH);
		rt_thread_mdelay(500);
	}
	rt_mutex_release(led2Mutex);

	//��ȡLED3�Ļ���������LED3��˸����
	rt_mutex_take(led3Mutex,RT_WAITING_FOREVER);
	for(count=0;count<10;count++)
	{
		rt_pin_write(LED3_PIN, PIN_LOW);
		rt_thread_mdelay(500);
		rt_pin_write(LED3_PIN, PIN_HIGH);
		rt_thread_mdelay(500);
	}
	rt_mutex_release(led3Mutex);
}
