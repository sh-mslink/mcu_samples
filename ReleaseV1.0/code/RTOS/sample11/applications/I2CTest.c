#include <rtthread.h>
#include <rtdevice.h>

#define BUS_NAME		"i2c1"
#define SLAVE_ADDRESS  0x40 //定义HDC2080的从设备地址 7bit 模式

static struct rt_i2c_bus_device *i2c_bus = RT_NULL;

static void calTempAndHumi(rt_uint8_t *pBuffer, float *pTemp, float *pHumi)
{
	rt_uint16_t temp,humi;
	
	temp = ((rt_uint16_t)pBuffer[0]&0x00ff) | ((rt_uint16_t)pBuffer[1]<<8)&0xff00 ;
	humi = ((rt_uint16_t)pBuffer[2]&0x00ff) | ((rt_uint16_t)pBuffer[3]<<8)&0xff00 ;

	*pTemp = (float)(((double)temp*165)/65536 -40);
	*pHumi = (float)(((double)humi*100)/65536);
}

static rt_err_t write_reg(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t *value)
{
	struct rt_i2c_msg msg;
	rt_uint8_t buf[2]={0};
	
	buf[0] = reg; //寄存器地址
	if(value == RT_NULL)
	{
		msg.addr = SLAVE_ADDRESS;
		msg.flags = RT_I2C_WR;
		msg.buf = buf;
		msg.len = 1;
	}
	else
	{
		buf[1] = value[0]; //寄存器值 
		msg.addr = SLAVE_ADDRESS;
		msg.flags = RT_I2C_WR;
		msg.buf = buf;
		msg.len = 2;		
	}
	
	if(rt_i2c_transfer(bus, &msg, 1) == 1)
		return RT_EOK;
	else
	{
		rt_kprintf("I2C write failed~!\n");
		return RT_ERROR;
	}
}

static rt_err_t read_regs(struct rt_i2c_bus_device *bus, rt_uint8_t len, rt_uint8_t *buf)
{
	struct rt_i2c_msg msg;
	
	msg.addr = SLAVE_ADDRESS;
	msg.flags = RT_I2C_RD;
	msg.buf = buf;
	msg.len = len;
	
	//发送读数据的寄存器地址
	if(rt_i2c_transfer(bus, &msg, 1) != 1)
	{
		rt_kprintf("I2C write failed~!\n");
		return RT_ERROR;
	}
	return RT_EOK;
}

//通过命令方式得到温湿度数据
static void getData(void)
{
  rt_uint8_t buff[4]={0};
	float temp,humi=80.5;
	
	write_reg(i2c_bus, 0x00, RT_NULL);
	read_regs(i2c_bus, 4, buff);

	calTempAndHumi(buff, &temp, &humi);
	printf("temperature=%f,\t humidity=%f\n",temp,humi);
}
//添加命令
MSH_CMD_EXPORT(getData, read HDC2080 temp&humi data);

//初始化HDC2080
static int HDC2080Init(void)
{
	rt_err_t ret = RT_EOK;
  rt_uint8_t buff[3]={0};
	
	//根据名字来查找I2C总线设备
	i2c_bus = rt_i2c_bus_device_find(BUS_NAME);
	if(i2c_bus == RT_NULL)
	{
		rt_kprintf("can not find bus~!\n");
		return RT_ERROR;
	}
	
	//初始化HDC2080
	buff[0]=0x50;
	write_reg(i2c_bus, 0x0E, buff);
	buff[0]=0x01;
	write_reg(i2c_bus, 0x0F, buff);

	//读取设备ID
	write_reg(i2c_bus, 0xFE, RT_NULL);
	read_regs(i2c_bus, 2, buff);
	rt_kprintf("I2C device ID=0x%x%x\n", buff[0],buff[1]);
	return ret;
}
//将初始化函数添加到设备初始化阶段
INIT_DEVICE_EXPORT(HDC2080Init);
