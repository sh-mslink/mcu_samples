#include <rtthread.h>
#include <rtdevice.h>

#define FLASH_DEVICE_NAME "exFlash"

//SPI���Ժ���
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
		cfg.mode = RT_SPI_MASTER|RT_SPI_MODE_0|RT_SPI_MSB; //����SPI��ģʽ
		cfg.data_width = 8;  //����spi���ݵ�λ��
		cfg.max_hz = 20 * 1000*1000;  //����SPI��ʱ���ٶ�
		rt_spi_configure(spiDevice, &cfg); //����SPI�豸
		
		sendMsg.send_buf = &readIDcmd;//�������ݵ�buffer
		sendMsg.recv_buf = RT_NULL;		//�������ݵ�bufferΪ��
		sendMsg.length = 4;					//���ͻ��߽������ݵĳ���
		sendMsg.cs_take = 1; 		//Ƭѡ�ź�����Ϊ��Ч״̬
		sendMsg.cs_release = 0;  //�����������Ҫ�ͷ�Ƭѡ�ź���
		sendMsg.next = &recvMsg;  //�������ݴ����������һ����Ϣ�ṹ���ָ��
		
		recvMsg.send_buf = RT_NULL; //�������ݵ�bufferΪ��
		recvMsg.recv_buf = id;     //�������ݵ�buffer
		recvMsg.length = 2;				//�������ݵĳ���
		recvMsg.cs_take = 0;			//����Ҫ����Ƭѡ�ź�
		recvMsg.cs_release = 1;		//����������ͷ�Ƭѡ�ź���
		recvMsg.next = RT_NULL;  //���δ�������󣬽�������

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
//�������
MSH_CMD_EXPORT(spiFlashTest, external SPI flash test);
