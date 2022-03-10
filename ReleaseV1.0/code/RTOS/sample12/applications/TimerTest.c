#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "drv_gpio.h"

/* defined the LED4 pin: PD15 */
#define LED4_PIN    GET_PIN(D, 15)
#define HWTIMER_NAME  "timer3"

rt_device_t timerDev=RT_NULL;

rt_uint8_t flip=0;

//Timer��ʱ�жϻص�����
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
//������ģʽ����Timer
static void timerStart(void)
{
	rt_hwtimer_mode_t mode = HWTIMER_MODE_PERIOD;
	rt_hwtimerval_t timeInterval;
	//Ѱ��Timer�豸
	timerDev = rt_device_find(HWTIMER_NAME);
	if(timerDev == RT_NULL)
	{
		rt_kprintf("can not find the timer~!\n");
		return;
	}
	//�Զ�д��ʽ��Timer
	rt_device_open(timerDev, RT_DEVICE_OFLAG_RDWR);
	//���ó�ʱ�жϻص�����
	rt_device_set_rx_indicate(timerDev, timeout_cb);
	//����timer�Ĺ���ģʽΪѭ����ʱ
	rt_device_control(timerDev,HWTIMER_CTRL_MODE_SET, &mode);
	//���ö�ʱʱ��
	timeInterval.sec=3; //��
	timeInterval.usec=0; //΢��
	rt_device_write(timerDev, 0, &timeInterval, sizeof(timeInterval));
	
	rt_kprintf("Timer started~!\n");
	
}
MSH_CMD_EXPORT(timerStart, hardware timer to control led);
//��ȡTimer��ǰ�ļ�ʱֵ
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
//ֹͣTimer
static void timerStop(void)
{
	if(timerDev)
		rt_device_close(timerDev);
	timerDev=RT_NULL;
	rt_kprintf("Timer Stopped~!\n");
}
MSH_CMD_EXPORT(timerStop, stop hardware timer);
