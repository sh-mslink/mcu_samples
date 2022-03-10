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

//定义LED2的互斥量指针
static rt_mutex_t led2Mutex = RT_NULL;
//定义LED3的互斥量指针
static rt_mutex_t led3Mutex = RT_NULL;

//该线程只针对LED2，控制快闪烁
static void led2MutexThread(void)
{
	uint16_t count;
	while(1)
	{
		//获取LED2的互斥量后让LED2闪烁几次
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

//该线程只针对LED3，控制快闪烁
static void led3MutexThread(void)
{
	uint16_t count;
	while(1)
	{
		//获取LED3的互斥量后让LED3闪烁几次
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

//创建用测试的互斥量和线程
int mutexThreadCreate(void)
{
	rt_thread_t tid;
	
	//创建一个互斥量来保护LED2的操作
	led2Mutex=rt_mutex_create("LED2",RT_IPC_FLAG_FIFO);
	if(led2Mutex == RT_NULL)
	{
		rt_kprintf("create led2Mutex failed~!\n");
		return -1;
	}
	//创建一个互斥量来保护LED3的操作
	led3Mutex=rt_mutex_create("LED3",RT_IPC_FLAG_FIFO);
	if(led3Mutex == RT_NULL)
	{
		rt_kprintf("create led3Mutex failed~!\n");
		return -1;
	}
	
	//创建一个线程，用来操作LED2
	tid = rt_thread_create("LED2Thread", led2MutexThread, RT_NULL, LED2_STACK_SIZE, LED2_PRIO, LED2_TICKS);
	RT_ASSERT(tid != RT_NULL);
	rt_thread_startup(tid);
	//创建一个线程，用来操作LED3
	tid = rt_thread_create("LED3Thread", led3MutexThread, RT_NULL, LED3_STACK_SIZE, LED3_PRIO, LED3_TICKS);
	RT_ASSERT(tid != RT_NULL);
	rt_thread_startup(tid);
	
	return RT_EOK;
}

//该函数被主线程调用，先控制LED2慢闪烁，在控制LED3慢闪烁
void mainMutexTest(void)
{
	uint16_t count;

	//获取LED2的互斥量后让LED2闪烁几次
	rt_mutex_take(led2Mutex,RT_WAITING_FOREVER);
	for(count=0;count<10;count++)
	{
		rt_pin_write(LED2_PIN, PIN_LOW);
		rt_thread_mdelay(500);
		rt_pin_write(LED2_PIN, PIN_HIGH);
		rt_thread_mdelay(500);
	}
	rt_mutex_release(led2Mutex);

	//获取LED3的互斥量后让LED3闪烁几次
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
