#include <rtthread.h>
#include <rtdevice.h>

#define BUFFER_SIZE 64
static struct rt_semaphore gRxSem;
static rt_device_t serial;
static char recevBuffer[BUFFER_SIZE]={0};
static uint16_t counter=0, len=0;
static uint16_t gCmdRuningFlag=0;

//接收中断回调函数，负责把数据保存到一个乒乓buffer中
static rt_err_t uartRX(rt_device_t dev, rt_size_t size)
{
	char temp;
	
	//按字节将数据从串口读出
	rt_device_read(serial, -1, &temp, 1);
	//如果接收到了换行或者回车符号，就记录字符串长度
	if(temp == '\n' || temp == '\r' || temp == 0x04)
	{
		len=counter;
		//释放信号量
		rt_sem_release(&gRxSem);	
	}
	else	
		recevBuffer[counter] = temp;
	
	//增加计数器
	counter++;
	//防止接收的数据过多而越界
	if(counter >= BUFFER_SIZE)
	{
		counter = 0;
	}
	
	return RT_EOK;
}

//用来把接收到的数据发送出去
void serialThread(void)
{	
	while(1)
	{	
		//等待信号量
		rt_sem_take(&gRxSem, RT_WAITING_FOREVER);

		//将接收缓冲区中的数据全部发送出去
		if(len)
		{
			rt_device_write(serial,0,recevBuffer,len);
			len=0;
			counter = 0;
		}
	}
}


//被导出的命令函数
//在该函数内创建一个线程
static int uart2_test(int argc, char *argv[])
{
	rt_err_t ret = RT_EOK;
	char str[] = "uart2 test string ~!\n";

	if(gCmdRuningFlag == 0)
	{
		//以名字来查找设备，并获取设备的控制句柄
		serial = rt_device_find("uart2");
		if(!serial)
		{
			rt_kprintf("No this device~!\n");
			return RT_ERROR;
		}
		
		//初始化一个信号量用来等待接收中断，注意初始化成0
		rt_sem_init(&gRxSem,"rx_sem",0,RT_IPC_FLAG_FIFO);
		//用控制句柄来打开设备，设备访问方式是读写访问，并且当设备有数据接收时产生中断
		rt_device_open(serial, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
		//配置当接收到数据时的中断回调函数
		rt_device_set_rx_indicate(serial, uartRX);
		//发送数据
		rt_device_write(serial,0,str,(sizeof(str)-1));
		
		//创建一个线程专门用来接收数据
		rt_thread_t pThread = rt_thread_create("serial2", serialThread, RT_NULL, 1024,25,10);
		if(pThread != RT_NULL)
		{
			rt_thread_startup(pThread);
			gCmdRuningFlag = 1;
			rt_kprintf("uart2 opened~!\n");	
		}
		else
		{

			ret = RT_ERROR;
		}
	}
	else
	{
		//发送数据
		rt_device_write(serial,0,str,(sizeof(str)-1));
		rt_kprintf("uart2 opened~!\n");
	}
	return ret;
}
//添加命令
MSH_CMD_EXPORT(uart2_test, uart2 device sample);