/**
  ******************************************************************************
  * @file    HDC2080.c 
  * @author  mSlink Technology
  * @version V0.0.1
  * @date    2020-08-12
  * @brief   HDC2080 access program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, ARTERYTEK SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2020 mSlink </center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "at32_board.h"

/* Macros ------------------------------------------------------------------*/
#define SLAVE_ADDRESS  0x80 //定义HDC2080的从设备地址 7bit 模式


/* Private datatype ------------------------------------------------------------------*/


/* Global variable ------------------------------------------------------------------*/

/* Private variable ------------------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  GPIO SW I2C generate start condition
  * @param  None
  * @retval None
  * note: after calling, SDA & SCL line will keep low
  */
static void genStart(void)
{
	GPIO_SetBits(GPIOB,I2C_SDA_PIN); //先释放SDA线	
	GPIO_SetBits(GPIOB,I2C_SCL_PIN); //再释放SCL线

	Delay_us(4);  //保持一段时间
	GPIO_ResetBits(GPIOB,I2C_SDA_PIN); //在SCL线为高的状态下，拉低SDA线，表示一个开始标记
	Delay_us(4);  //保持一段时间
	GPIO_ResetBits(GPIOB,I2C_SCL_PIN); //拉低SCL线
	Delay_us(8);  //保持一段时间
	//GPIO_SetBits(GPIOx,I2C_SDA_PIN); //不需要释放SDA线，因为后面的数据可能会变化
	//GPIO_SetBits(GPIOx,I2C_SCL_PIN); //不能释放SCL线，因为只能在SCL为低的情况下变化数据
}

/**
  * @brief  GPIO SW I2C shift data to device,8bit mode
  * @param  data: send data to device 
  * @retval 0: means no ACK, 1: means detected ACK
  * note: after calling, normally SDA should be high, SCL line will keep low
  */
static u8 shiftOutData8(u8 data)
{
	u8 i;
	u8 shiftbit=0x80; //定义一个比较变量用来检测数据中每一位的值，I2C总线是先输出高位
	u8 ret=0;
	
	for(i=0;i<8;i++)
	{
		if(shiftbit & data)
		{
			GPIO_SetBits(GPIOB,I2C_SDA_PIN); //释放SDA线，意味着输出1
		}
		else
		{
			GPIO_ResetBits(GPIOB,I2C_SDA_PIN); //拉低SDA线，意味着输出0
		}
		Delay_us(4);  //保持一段时间
		GPIO_SetBits(GPIOB,I2C_SCL_PIN); //释放SCL线
		Delay_us(8);  //保持一段时间
		GPIO_ResetBits(GPIOB,I2C_SCL_PIN); //拉低SCL线，便于下一次数据输出
		shiftbit=shiftbit>>1; //移位比较变量
		Delay_us(4);  //保持一段时间
	}
	
	GPIO_SetBits(GPIOB,I2C_SDA_PIN); //释放SDA线，以便于设备产生ACK信号
	Delay_us(8);  //保持一段时间，以便于设备产生ACK信号
	GPIO_SetBits(GPIOB,I2C_SCL_PIN); //释放SCL线
	Delay_us(4);  //保持一段时间
	ret=GPIO_ReadInputDataBit(GPIOB, I2C_SDA_PIN); //读取SDA线的状态，以便判断是否有应答
	GPIO_ResetBits(GPIOB,I2C_SCL_PIN); //拉低SCL线，通知设备主机已经接收了应答，但不要释放，以便于后面产生stop条件或继续发送数据
	Delay_us(4);  //保持一段时间
	
	if(ret) return 0; //如果SDA线为高，意味着没有应答
	else return 1;    //如果SDA线为低，意味着有应答
}

/**
  * @brief  GPIO SW I2C shift data from device,8bit mode
  * @param  needACKorNot: 1, means generate ACK after received data; 0, means no need generate ACK 
  * @retval received data 
  * note: after calling, SDA not define, SCL line will keep low
  */
static u8 shiftInData8(u8 needACKorNot)
{
	u8 i;
	u16 shiftbyte=0; //定义一个移位变量
	u8 ret=0;
	for(i=0;i<8;i++)
	{
		GPIO_SetBits(GPIOB,I2C_SDA_PIN); //释放SDA线，让设备来控制SDA线
		Delay_us(4);  //保持一段时间
		GPIO_SetBits(GPIOB,I2C_SCL_PIN); //释放SCL线
		Delay_us(4);  //保持一段时间
		ret=GPIO_ReadInputDataBit(GPIOB, I2C_SDA_PIN); //读取SDA线的状态
		shiftbyte |= ret;
		shiftbyte = shiftbyte << 1;
		Delay_us(4);  //保持一段时间
		GPIO_ResetBits(GPIOB,I2C_SCL_PIN); //拉低SCL线，通知设备可以更新数据
		Delay_us(4);  //保持一段时间
	}
	
	shiftbyte = shiftbyte >> 1; //把循环里多移的1位给找补回来
	
	if(needACKorNot)
	{
		GPIO_ResetBits(GPIOB,I2C_SDA_PIN); //拉低SDA线，准备产生应答

	}
	else
	{
		GPIO_SetBits(GPIOB,I2C_SDA_PIN); //释放SDA线，产生无应答信号

	}
	Delay_us(4);  //保持一段时间
	GPIO_SetBits(GPIOB,I2C_SCL_PIN); //释放SCL线
	Delay_us(4);  //保持一段时间
	GPIO_ResetBits(GPIOB,I2C_SCL_PIN); //拉低SCL线
	Delay_us(4);  //保持一段时间
	
	return (u8)shiftbyte;
}

/**
  * @brief  GPIO SW I2C generate stop condition
  * @param  None
  * @retval 0:means bus error, device not release SDA line. 1: means success
   * note: after calling, SDA & SCL line should be high
  */
static u8 genStop(void)
{
	u8 ret=0;

	ret=GPIO_ReadInputDataBit(GPIOB, I2C_SDA_PIN); //读取SDA线的状态，判断是否被释放
	if(ret == 0) 
	{
		GPIO_SetBits(GPIOB,I2C_SDA_PIN); //释放SDA线
		GPIO_SetBits(GPIOB,I2C_SCL_PIN); //释放SCL线
		return 0; //如果SDA线没有被释放，有问题！返回。
	}
	
	GPIO_ResetBits(GPIOB,I2C_SDA_PIN); //拉低SDA线，以产生stop信号
	Delay_us(4);  //保持一段时间
	GPIO_SetBits(GPIOB,I2C_SCL_PIN); //释放SCL线，以产生stop信号
	Delay_us(4);  //保持一段时间
	GPIO_SetBits(GPIOB,I2C_SDA_PIN); //释放SDA线，发送stop信号
	Delay_us(12);  //保持一段时间
	return ret;
}

/* Global function ------------------------------------------------------------------*/

/**
  * @brief  Initializes peripherals used by the I2C HDC1080 driver, use interrupt mode
  * @param  None
  * @retval None
  */
void SW_I2C_Init(void)
{
	GPIO_InitType  GPIO_InitStructure; // 定义GPIO 配置参数结构体
	/*Enable the I2C GPIO Clock*/
	RCC_APB2PeriphClockCmd(I2C_GPIO_RCC_CLK, ENABLE);

	/** GPIO configuration*/
	GPIO_InitStructure.GPIO_Pins =	I2C_SCL_PIN | I2C_SDA_PIN;  // 配置I2C的管脚
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;  // 配置GPIO端口的时钟
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_OD;		// 配置GPIO 为开漏输出
	GPIO_Init(I2C_PORT, &GPIO_InitStructure);  //调用GPIO配置函数

	GPIO_SetBits(I2C_PORT,I2C_SDA_PIN); //OD输出1意味着释放 SDA 线 
	GPIO_SetBits(I2C_PORT,I2C_SCL_PIN); //OD输出1意味着释放 SCL 线	

	Delay_us(20); //等待GPIO配置稳定
	printf("HDC2080 I2C init done\n");
}

/**
  * @brief  config HDC2080 on auto measurement mode for every second
  * @param  None
  * @retval 0: ERROT reture; 1:success reture
  */
u8 HDC2080_Init(void)
{
	u8 ret=0;

	Delay_ms(5); //根据HDC2080数据手册中指导，设备上电后至少需要3mS的时间来准备数据

	//--------------初始化配置寄存器，将采样模式设定为自动采样，每秒一次--------------
	genStart(); //发送Start信号
	ret=shiftOutData8(SLAVE_ADDRESS);//发送从设备地址，写模式最低位是0
	if(ret == 0)
	{
		printf("ERROR: send slave address no ACK !\n");
		return 0;
	}

	ret=shiftOutData8(0x0E); //发送配置寄存器地址
	if(ret == 0)
	{
		printf("ERROR: send register address no ACK !\n");
		return 0;
	}

	ret=shiftOutData8(0x50); //发送配置寄存器值，设置为自动采样模式，每秒采样一次
	if(ret == 0)
	{
		printf("ERROR: send register data no ACK !\n");
		return 0;
	}
	
	ret=genStop(); //发送Stop信号

	//--------------设置触发寄存器开始采样--------------
	genStart(); //发送Start信号
	ret=shiftOutData8(SLAVE_ADDRESS);//发送从设备地址，写模式最低位是0
	if(ret == 0)
	{
		printf("ERROR: send slave address no ACK !\n");
		return 0;
	}

	ret=shiftOutData8(0x0F); //发送测量配置寄存器地址
	if(ret == 0)
	{
		printf("ERROR: send register address no ACK !\n");
		return 0;
	}

	ret=shiftOutData8(0x01); //发送测量配置寄存器值，开始采样
	if(ret == 0)
	{
		printf("ERROR: send register data no ACK !\n");
		return 0;
	}
	
	ret=genStop(); //发送Stop信号


	if(ret) printf("HDC2080 init sucess\n");
	else printf("HDC2080 init fail\n");

	return ret;
}

/**
  * @brief  Read temperature and humidiyt data from HDC1080 by the I2C
  * @param  Buffer address, buffer size
  * @retval erro code
  */
u8 readHDC2080Data(u8 *pBuffer, u8 size)
{
	u8 ret=0;
	u16 i;

	genStart(); //发送Start信号
	ret=shiftOutData8(SLAVE_ADDRESS);//发送从设备地址，写模式最低位是0
	if(ret == 0)
	{
		printf("ERROR: send slave address no ACK !\n");
		return 0;
	}

	ret=shiftOutData8(0x00); //发送温度寄存器起始地址
	if(ret == 0)
	{
		printf("ERROR: send register address no ACK !\n");
		return 0;
	}
	
	genStart(); //发送Start信号
	ret=shiftOutData8(SLAVE_ADDRESS|0x01);//发送从设备地址，读模式最低位是1，触发读取操作
	for(i=0;i<size-1;i++)
	{
		pBuffer[i]=shiftInData8(1); //前面的数据需要ACK
	}
	
	pBuffer[i]=shiftInData8(0); //最后一个数据不需要ACK

	ret=genStop(); //发送Stop信号
	
	return ret;
}

/**
  * @brief  Read HDC2080 ID NUMBER
  * @param  Buffer address, buffer size
  * @retval erro code
  */
u8 readHDC2080ID(u8 *pBuffer, u8 size)
{
	u8 ret=0;
	u16 i;

	genStart(); //发送Start信号
	ret=shiftOutData8(SLAVE_ADDRESS);//发送从设备地址，写模式最低位是0
	if(ret == 0)
	{
		printf("ERROR: send slave address no ACK !\n");
		return 0;
	}

	ret=shiftOutData8(0xFE); //发送ID寄存器起始地址
	if(ret == 0)
	{
		printf("ERROR: send register address no ACK !\n");
		return 0;
	}
	
	genStart(); //再次发送Start信号
	ret=shiftOutData8(SLAVE_ADDRESS|0x01);//发送从设备地址，读模式最低位是1，触发读取操作
	for(i=0;i<size-1;i++)
	{
		pBuffer[i]=shiftInData8(1); //前面的数据需要ACK
	}
	
	pBuffer[i]=shiftInData8(0); //最后一个数据不需要ACK

	ret=genStop(); //发送Stop信号
	
	return ret;
}

/**
  * @brief  convert raw data to Temp and Humi number
  * @param  buffer:Buffer address; pTemp: temperature value; pHumi: humidity value
  * @retval None
  */
void calTempAndHumi(u8 *pBuffer, float *pTemp, float *pHumi)
{
	u16 temp,humi;
	
	temp = ((u16)pBuffer[0]&0x00ff) | ((u16)pBuffer[1]<<8)&0xff00 ;
	humi = ((u16)pBuffer[2]&0x00ff) | ((u16)pBuffer[3]<<8)&0xff00 ;

	*pTemp = (float)(((double)temp*165)/65536 -40);
	*pHumi = (float)(((double)humi*100)/65536);
}
