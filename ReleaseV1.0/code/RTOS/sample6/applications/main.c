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

#include "test.h"

int main(void)
{
	/* set LED2 pin mode to output */
	rt_pin_mode(LED2_PIN, PIN_MODE_OUTPUT);
	/* set LED3 pin mode to output */
	rt_pin_mode(LED3_PIN, PIN_MODE_OUTPUT);
	/* set LED4 pin mode to output */
	rt_pin_mode(LED4_PIN, PIN_MODE_OUTPUT);
	
	LinkedListTest();
	
	while (1)
	{
		rt_pin_write(LED3_PIN, PIN_LOW);
		rt_thread_mdelay(50);
		rt_pin_write(LED3_PIN, PIN_HIGH);
		rt_thread_mdelay(50);
	}
}
