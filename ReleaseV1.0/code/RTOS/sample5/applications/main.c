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


//#define MUTEXT_TEST
#define MSG_TEST

#ifdef MUTEXT_TEST	
extern int mutexThreadCreate(void);
extern void mainMutexTest(void);
#endif

#ifdef MSG_TEST
extern int msgThreadCreate(void);
extern void mainMsgTest(void);
#endif

int main(void)
{
	/* set LED2 pin mode to output */
	rt_pin_mode(LED2_PIN, PIN_MODE_OUTPUT);
	/* set LED3 pin mode to output */
	rt_pin_mode(LED3_PIN, PIN_MODE_OUTPUT);
	/* set LED4 pin mode to output */
	rt_pin_mode(LED4_PIN, PIN_MODE_OUTPUT);
	
#ifdef MUTEXT_TEST	
	mutexThreadCreate();
#endif
	
#ifdef MSG_TEST	
	msgThreadCreate();
#endif

	while (1)
	{
#ifdef MUTEXT_TEST			
		mainMutexTest();
#endif
		
#ifdef MSG_TEST			
		mainMsgTest();
#endif
	}
}
