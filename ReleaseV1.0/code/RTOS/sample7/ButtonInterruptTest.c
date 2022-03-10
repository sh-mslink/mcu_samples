#include "drv_gpio.h"


#define MAX_MB_NUM 32
#define LED_ON 1
#define LED_OFF 0

#define BUTTON_PIN GET_PIN(A,0)

static rt_mailbox_t mbForButton = RT_NULL;

static void buttonISR(void)
{
	int value=rt_pin_read(BUTTON_PIN);
	
	if(value == 1)
	{
		//Press
		rt_mb_send(mbForButton, LED_ON);
	}
	else
	{
		//Release
		rt_mb_send(mbForButton, LED_OFF);
	}  
}

uint32_t waitingCMD(void)
{
	uint32_t value;
	rt_mb_recv(mbForButton, &value, RT_WAITING_FOREVER);
	return value;
}

int initButtonInterrupt(void)
{
	//创建一个邮箱队列
	mbForButton=rt_mb_create("mbBtn", MAX_MB_NUM, RT_IPC_FLAG_FIFO);
	if(mbForButton == RT_NULL)
	{
		rt_kprintf("mbForButton mailbox create failed~!\n");
		return -1;
	}
	
	//配置中断函数和中断模式
	rt_pin_attach_irq(BUTTON_PIN, PIN_IRQ_MODE_RISING_FALLING, buttonISR, NULL);
	rt_pin_irq_enable(BUTTON_PIN, PIN_IRQ_ENABLE);
	
	return 0;
}