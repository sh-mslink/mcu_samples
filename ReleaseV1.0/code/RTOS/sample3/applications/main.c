/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-01-08     shelton      first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "drv_gpio.h"

#define HARD_DELAY 0
#define SPEED 200
#define LED2_PRIO 8
#define LED3_PRIO 9
#define LED2_STACK_SIZE 256
#define LED3_STACK_SIZE 256
#define LED2_TICKS 1000
#define LED3_TICKS 1000

/* defined the LED2 pin: PD13 */
#define LED2_PIN    GET_PIN(D, 13)
/* defined the LED3 pin: PD14 */
#define LED3_PIN    GET_PIN(D, 14)
/* defined the LED4 pin: PD15 */
#define LED4_PIN    GET_PIN(D, 15)

#if HARD_DELAY==1
static void _delay_us(uint32_t us)
{
    volatile uint32_t len;
    for (; us > 0; us --)
        for (len = 0; len < 20; len++)
					__NOP;
}
#endif

void LED2Thread(void)
{
	//uint16_t i;
	uint16_t pinStatus=0;
	//for(i=0;i<10;i++)
	while(1)
	{
		pinStatus=rt_pin_read(LED2_PIN);
		if(pinStatus)
			rt_pin_write(LED2_PIN, PIN_LOW);
		else
			rt_pin_write(LED2_PIN, PIN_HIGH);
#if HARD_DELAY==1 
		_delay_us(SPEED*1000);
#else
		rt_thread_mdelay(SPEED);
#endif
	}  
	rt_kprintf("Goodby~!\n");
}

void LED3Thread(void)
{
	//uint16_t i;
	uint16_t pinStatus=0;
	//for(i=0;i<10;i++)
	while(1)
	{
		pinStatus=rt_pin_read(LED3_PIN);
		if(pinStatus)
			rt_pin_write(LED3_PIN, PIN_LOW);
		else
			rt_pin_write(LED3_PIN, PIN_HIGH);
#if HARD_DELAY==1
		_delay_us(SPEED*1000);
#else
		rt_thread_mdelay(SPEED);
#endif

	}  
	rt_kprintf("Goodby~!\n");
}

/*
void startLED3(void)
{
	rt_thread_t tid;
	tid = rt_thread_create("LED3Thread", LED3Thread, RT_NULL, LED3_STACK_SIZE, LED3_PRIO, LED3_TICKS);
	RT_ASSERT(tid != RT_NULL);
	rt_thread_startup(tid);
}
MSH_CMD_EXPORT(startLED3, startLED3);
*/

void printSchdule(struct rt_thread *from, struct rt_thread *to)
{
	rt_kprintf("from: %s --> to: %s \n", from->name, to->name);
}

int main(void)
{
	rt_thread_t tid;
	uint16_t pinStatus=0;

	/* set LED2 pin mode to output */
	rt_pin_mode(LED2_PIN, PIN_MODE_OUTPUT);
	/* set LED3 pin mode to output */
	rt_pin_mode(LED3_PIN, PIN_MODE_OUTPUT);
	/* set LED4 pin mode to output */
	rt_pin_mode(LED4_PIN, PIN_MODE_OUTPUT);

	rt_scheduler_sethook(printSchdule);

	tid = rt_thread_create("LED2Thread", LED2Thread, RT_NULL, LED2_STACK_SIZE, LED2_PRIO, LED2_TICKS);
	RT_ASSERT(tid != RT_NULL);
	rt_thread_startup(tid);
	
	tid = rt_thread_create("LED3Thread", LED3Thread, RT_NULL, LED3_STACK_SIZE, LED3_PRIO, LED3_TICKS);
	RT_ASSERT(tid != RT_NULL);
	rt_thread_startup(tid);

	while (1)
	{
		pinStatus=rt_pin_read(LED4_PIN);
		if(pinStatus)
			rt_pin_write(LED4_PIN, PIN_LOW);
		else
			rt_pin_write(LED4_PIN, PIN_HIGH);
#if HARD_DELAY==1
		_delay_us(SPEED*1000);
#else
		rt_thread_mdelay(SPEED);
#endif

	}
}
