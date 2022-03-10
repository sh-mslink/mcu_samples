#include <rtthread.h>
#include <rtdevice.h>

#define FLASH_DEVICE_NAME "exFlash"

//SPI测试函数
static int spiFlashTest(int argc, char *argv[])
{
	rt_err_t ret = RT_EOK;
	struct rt_spi_device *spiDevice;
	struct rt_spi_configuration cfg;
	struct rt_spi_message sendMsg, recvMsg;
	rt_uint8_t readIDcmd[4] = {0x90, 0x00, 0x00, 0x00};
	rt_uint8_t id[2] = {0};
	
	spiDevice=(struct rt_spi_device *)rt_device_find(FLASH_DEVICE_NAME);
	if(spiDevice)
	{
		cfg.mode = RT_SPI_MASTER|RT_SPI_MODE_0|RT_SPI_MSB; //设置SPI的模式
		cfg.data_width = 8;  //设置spi数据的位宽
		cfg.max_hz = 20 * 1000*1000;  //设置SPI的时钟速度
		rt_spi_configure(spiDevice, &cfg); //配置SPI设备
		
		sendMsg.send_buf = &readIDcmd;//发送数据的buffer
		sendMsg.recv_buf = RT_NULL;		//接收数据的buffer为空
		sendMsg.length = 4;					//发送或者接收数据的长度
		sendMsg.cs_take = 1; 		//片选信号设置为有效状态
		sendMsg.cs_release = 0;  //传输结束后不需要释放片选信号线
		sendMsg.next = &recvMsg;  //本次数据传输结束后，下一个消息结构体的指针
		
		recvMsg.send_buf = RT_NULL; //发送数据的buffer为空
		recvMsg.recv_buf = id;     //接收数据的buffer
		recvMsg.length = 2;				//传输数据的长度
		recvMsg.cs_take = 0;			//不需要控制片选信号
		recvMsg.cs_release = 1;		//传输结束后释放片选信号线
		recvMsg.next = RT_NULL;  //本次传输结束后，结束传输

		rt_spi_transfer_message(spiDevice, &sendMsg);
		rt_kprintf("read id from EN25QH128A is 0x%x%x\n",id[0],id[1]);
		}
	else
	{
		rt_kprintf("Can not find the external flash device~!\n");
		ret = RT_ERROR;
	}
	
	return ret;
}
//添加命令
MSH_CMD_EXPORT(spiFlashTest, external SPI flash test);
