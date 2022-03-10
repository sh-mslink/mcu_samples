#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "drv_gpio.h"

/* defined the LED4 pin: PD15 */
#define LED4_PIN    GET_PIN(D, 15)
#define HWTIMER_NAME  "timer3"

rt_device_t timerDev=RT_NULL;

rt_uint8_t flip=0;

//Timer计时中断回调函数
static void timeout_cb(void)
{
	if(flip)
	{
		rt_pin_write(LED4_PIN, PIN_LOW);
	}
	else
	{
		rt_pin_write(LED4_PIN, PIN_HIGH);
	}
	
	flip ^= 1;

}
//以周期模式启动Timer
static void timerStart(void)
{
	rt_hwtimer_mode_t mode = HWTIMER_MODE_PERIOD;
	rt_hwtimerval_t timeInterval;
	//寻找Timer设备
	timerDev = rt_device_find(HWTIMER_NAME);
	if(timerDev == RT_NULL)
	{
		rt_kprintf("can not find the timer~!\n");
		return;
	}
	//以读写方式打开Timer
	rt_device_open(timerDev, RT_DEVICE_OFLAG_RDWR);
	//设置超时中断回调函数
	rt_device_set_rx_indicate(timerDev, timeout_cb);
	//设置timer的工作模式为循环定时
	rt_device_control(timerDev,HWTIMER_CTRL_MODE_SET, &mode);
	//设置定时时间
	timeInterval.sec=3; //秒
	timeInterval.usec=0; //微秒
	rt_device_write(timerDev, 0, &timeInterval, sizeof(timeInterval));
	
	rt_kprintf("Timer started~!\n");
	
}
MSH_CMD_EXPORT(timerStart, hardware timer to control led);
//读取Timer当前的计时值
static void timerRead(void)
{
	rt_hwtimerval_t timeVal = {0};
	if(timerDev)
	{
		rt_device_read(timerDev, 0, &timeVal, sizeof(rt_hwtimerval_t));
	}
	rt_kprintf("Sec=%d, uS=%d\n", timeVal.sec, timeVal.usec);
}
MSH_CMD_EXPORT(timerRead, current hardware timer value);
//停止Timer
static void timerStop(void)
{
	if(timerDev)
		rt_device_close(timerDev);
	timerDev=RT_NULL;
	rt_kprintf("Timer Stopped~!\n");
}
MSH_CMD_EXPORT(timerStop, stop hardware timer);
