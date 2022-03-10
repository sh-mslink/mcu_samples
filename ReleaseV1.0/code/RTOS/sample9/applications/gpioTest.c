
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_gpio.h"

#define BUTTON_PIN GET_PIN(A,0)
#define BUTTON_PRESS_EVENT 0x0001
#define BUTTON_RELEASE_EVENT 0x0002


static uint16_t gCmdRuningFlag=0;
static rt_event_t buttonEvent = RT_NULL;
	
//GPIO的中断回调函数
void gpioHandle(void)
{
	int pinValue=rt_pin_read(BUTTON_PIN);
	if(pinValue == PIN_HIGH)
	{
		rt_event_send(buttonEvent,BUTTON_PRESS_EVENT);
	}
	else if(pinValue == PIN_LOW)
	{
		rt_event_send(buttonEvent,BUTTON_RELEASE_EVENT);
	}
	else
	{
		rt_kprintf("what?! %d\n",pinValue);
	}
}

//处理按键状态的线程
void gpioThread(void)
{
	rt_uint32_t e = 0;
	rt_uint16_t i;
	
	while(1)
	{
		if(RT_EOK == rt_event_recv(buttonEvent, 
															(BUTTON_PRESS_EVENT|BUTTON_RELEASE_EVENT), 
															RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, 
															RT_WAITING_FOREVER, 
															&e))
		{
			switch(e) //根据事件的值分散处理不同的状况
			{
				case BUTTON_PRESS_EVENT:
					//多次判断按键状态，确定是长按还是短按
					for(i=0;i<100;i++)
					{
						rt_thread_mdelay(5);
						if(rt_pin_read(BUTTON_PIN) != PIN_HIGH)
						{
							break;
						}
					}
					if((i>=5)&&(i<=50))
						rt_kprintf("button pressed!\n");
					else if((i>50)&&(i<=100))
						rt_kprintf("button long pressed!\n");
				break;
				case BUTTON_RELEASE_EVENT:
					for(i=0;i<5;i++)
					{
						rt_thread_mdelay(5);
						if(rt_pin_read(BUTTON_PIN) != PIN_LOW)
						{
							break;
						}
					}
					if(i==5)
						rt_kprintf("button released!\n");
				break;
				case (BUTTON_PRESS_EVENT|BUTTON_RELEASE_EVENT):
					rt_kprintf("what?!\n");
				break;
			}
		}
	}
}

//按键测试函数
static int gpioTest(int argc, char *argv[])
{
	rt_err_t ret = RT_EOK;
	if(gCmdRuningFlag == 0)
	{
		//创建一个事件用于给中断和按键处理线程使用
		buttonEvent= rt_event_create("button",RT_IPC_FLAG_FIFO);
		if(buttonEvent == RT_NULL)
		{
			rt_kprintf("button event create fail~!\n");
			return RT_ERROR;
		}
		
		//加载中断回调函数，同时指定中断模式是双边沿触发
		rt_pin_attach_irq(BUTTON_PIN, PIN_IRQ_MODE_RISING_FALLING, gpioHandle, NULL);
		//使能GPIO的中断
		rt_pin_irq_enable(BUTTON_PIN, PIN_IRQ_ENABLE);

		//创建一个线程专门用来处理按键状态
		rt_thread_t pThread = rt_thread_create("GPIOA0", gpioThread, RT_NULL, 1024,10,10);
		if(pThread != RT_NULL)
		{
			rt_thread_startup(pThread);
			gCmdRuningFlag = 1;
			rt_kprintf("GPIO thread created~!\n");
		}
		else
		{
			ret = RT_ERROR;
		}
	}
	else
		rt_kprintf("GPIO thread existed~!\n");
	
	return ret;
}

//添加命令
MSH_CMD_EXPORT(gpioTest, PIN device test);
