#include <rtthread.h>
#include <rtdevice.h>

#define BUFFER_SIZE 64
static struct rt_semaphore gRxSem;
static rt_device_t serial;
static char recevBuffer[BUFFER_SIZE]={0};
static uint16_t counter=0, len=0;
static uint16_t gCmdRuningFlag=0;

//�����жϻص���������������ݱ��浽һ��ƹ��buffer��
static rt_err_t uartRX(rt_device_t dev, rt_size_t size)
{
	char temp;
	
	//���ֽڽ����ݴӴ��ڶ���
	rt_device_read(serial, -1, &temp, 1);
	//������յ��˻��л��߻س����ţ��ͼ�¼�ַ�������
	if(temp == '\n' || temp == '\r' || temp == 0x04)
	{
		len=counter;
		//�ͷ��ź���
		rt_sem_release(&gRxSem);	
	}
	else	
		recevBuffer[counter] = temp;
	
	//���Ӽ�����
	counter++;
	//��ֹ���յ����ݹ����Խ��
	if(counter >= BUFFER_SIZE)
	{
		counter = 0;
	}
	
	return RT_EOK;
}

//�����ѽ��յ������ݷ��ͳ�ȥ
void serialThread(void)
{	
	while(1)
	{	
		//�ȴ��ź���
		rt_sem_take(&gRxSem, RT_WAITING_FOREVER);

		//�����ջ������е�����ȫ�����ͳ�ȥ
		if(len)
		{
			rt_device_write(serial,0,recevBuffer,len);
			len=0;
			counter = 0;
		}
	}
}


//�������������
//�ڸú����ڴ���һ���߳�
static int uart2_test(int argc, char *argv[])
{
	rt_err_t ret = RT_EOK;
	char str[] = "uart2 test string ~!\n";

	if(gCmdRuningFlag == 0)
	{
		//�������������豸������ȡ�豸�Ŀ��ƾ��
		serial = rt_device_find("uart2");
		if(!serial)
		{
			rt_kprintf("No this device~!\n");
			return RT_ERROR;
		}
		
		//��ʼ��һ���ź��������ȴ������жϣ�ע���ʼ����0
		rt_sem_init(&gRxSem,"rx_sem",0,RT_IPC_FLAG_FIFO);
		//�ÿ��ƾ�������豸���豸���ʷ�ʽ�Ƕ�д���ʣ����ҵ��豸�����ݽ���ʱ�����ж�
		rt_device_open(serial, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
		//���õ����յ�����ʱ���жϻص�����
		rt_device_set_rx_indicate(serial, uartRX);
		//��������
		rt_device_write(serial,0,str,(sizeof(str)-1));
		
		//����һ���߳�ר��������������
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
		//��������
		rt_device_write(serial,0,str,(sizeof(str)-1));
		rt_kprintf("uart2 opened~!\n");
	}
	return ret;
}
//�������
MSH_CMD_EXPORT(uart2_test, uart2 device sample);