
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_gpio.h"

#define BUTTON_PIN GET_PIN(A,0)
#define BUTTON_PRESS_EVENT 0x0001
#define BUTTON_RELEASE_EVENT 0x0002


static uint16_t gCmdRuningFlag=0;
static rt_event_t buttonEvent = RT_NULL;
	
//GPIO���жϻص�����
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

//������״̬���߳�
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
			switch(e) //�����¼���ֵ��ɢ����ͬ��״��
			{
				case BUTTON_PRESS_EVENT:
					//����жϰ���״̬��ȷ���ǳ������Ƕ̰�
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

//�������Ժ���
static int gpioTest(int argc, char *argv[])
{
	rt_err_t ret = RT_EOK;
	if(gCmdRuningFlag == 0)
	{
		//����һ���¼����ڸ��жϺͰ��������߳�ʹ��
		buttonEvent= rt_event_create("button",RT_IPC_FLAG_FIFO);
		if(buttonEvent == RT_NULL)
		{
			rt_kprintf("button event create fail~!\n");
			return RT_ERROR;
		}
		
		//�����жϻص�������ͬʱָ���ж�ģʽ��˫���ش���
		rt_pin_attach_irq(BUTTON_PIN, PIN_IRQ_MODE_RISING_FALLING, gpioHandle, NULL);
		//ʹ��GPIO���ж�
		rt_pin_irq_enable(BUTTON_PIN, PIN_IRQ_ENABLE);

		//����һ���߳�ר������������״̬
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

//�������
MSH_CMD_EXPORT(gpioTest, PIN device test);
