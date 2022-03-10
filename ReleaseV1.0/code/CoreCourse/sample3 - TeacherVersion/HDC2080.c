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
#define SLAVE_ADDRESS  0x80 //����HDC2080�Ĵ��豸��ַ 7bit ģʽ


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
	GPIO_SetBits(GPIOB,I2C_SDA_PIN); //���ͷ�SDA��	
	GPIO_SetBits(GPIOB,I2C_SCL_PIN); //���ͷ�SCL��

	Delay_us(4);  //����һ��ʱ��
	GPIO_ResetBits(GPIOB,I2C_SDA_PIN); //��SCL��Ϊ�ߵ�״̬�£�����SDA�ߣ���ʾһ����ʼ���
	Delay_us(4);  //����һ��ʱ��
	GPIO_ResetBits(GPIOB,I2C_SCL_PIN); //����SCL��
	Delay_us(8);  //����һ��ʱ��
	//GPIO_SetBits(GPIOx,I2C_SDA_PIN); //����Ҫ�ͷ�SDA�ߣ���Ϊ��������ݿ��ܻ�仯
	//GPIO_SetBits(GPIOx,I2C_SCL_PIN); //�����ͷ�SCL�ߣ���Ϊֻ����SCLΪ�͵�����±仯����
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
	u8 shiftbit=0x80; //����һ���Ƚϱ����������������ÿһλ��ֵ��I2C�������������λ
	u8 ret=0;
	
	for(i=0;i<8;i++)
	{
		if(shiftbit & data)
		{
			GPIO_SetBits(GPIOB,I2C_SDA_PIN); //�ͷ�SDA�ߣ���ζ�����1
		}
		else
		{
			GPIO_ResetBits(GPIOB,I2C_SDA_PIN); //����SDA�ߣ���ζ�����0
		}
		Delay_us(4);  //����һ��ʱ��
		GPIO_SetBits(GPIOB,I2C_SCL_PIN); //�ͷ�SCL��
		Delay_us(8);  //����һ��ʱ��
		GPIO_ResetBits(GPIOB,I2C_SCL_PIN); //����SCL�ߣ�������һ���������
		shiftbit=shiftbit>>1; //��λ�Ƚϱ���
		Delay_us(4);  //����һ��ʱ��
	}
	
	GPIO_SetBits(GPIOB,I2C_SDA_PIN); //�ͷ�SDA�ߣ��Ա����豸����ACK�ź�
	Delay_us(8);  //����һ��ʱ�䣬�Ա����豸����ACK�ź�
	GPIO_SetBits(GPIOB,I2C_SCL_PIN); //�ͷ�SCL��
	Delay_us(4);  //����һ��ʱ��
	ret=GPIO_ReadInputDataBit(GPIOB, I2C_SDA_PIN); //��ȡSDA�ߵ�״̬���Ա��ж��Ƿ���Ӧ��
	GPIO_ResetBits(GPIOB,I2C_SCL_PIN); //����SCL�ߣ�֪ͨ�豸�����Ѿ�������Ӧ�𣬵���Ҫ�ͷţ��Ա��ں������stop�����������������
	Delay_us(4);  //����һ��ʱ��
	
	if(ret) return 0; //���SDA��Ϊ�ߣ���ζ��û��Ӧ��
	else return 1;    //���SDA��Ϊ�ͣ���ζ����Ӧ��
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
	u16 shiftbyte=0; //����һ����λ����
	u8 ret=0;
	for(i=0;i<8;i++)
	{
		GPIO_SetBits(GPIOB,I2C_SDA_PIN); //�ͷ�SDA�ߣ����豸������SDA��
		Delay_us(4);  //����һ��ʱ��
		GPIO_SetBits(GPIOB,I2C_SCL_PIN); //�ͷ�SCL��
		Delay_us(4);  //����һ��ʱ��
		ret=GPIO_ReadInputDataBit(GPIOB, I2C_SDA_PIN); //��ȡSDA�ߵ�״̬
		shiftbyte |= ret;
		shiftbyte = shiftbyte << 1;
		Delay_us(4);  //����һ��ʱ��
		GPIO_ResetBits(GPIOB,I2C_SCL_PIN); //����SCL�ߣ�֪ͨ�豸���Ը�������
		Delay_us(4);  //����һ��ʱ��
	}
	
	shiftbyte = shiftbyte >> 1; //��ѭ������Ƶ�1λ���Ҳ�����
	
	if(needACKorNot)
	{
		GPIO_ResetBits(GPIOB,I2C_SDA_PIN); //����SDA�ߣ�׼������Ӧ��

	}
	else
	{
		GPIO_SetBits(GPIOB,I2C_SDA_PIN); //�ͷ�SDA�ߣ�������Ӧ���ź�

	}
	Delay_us(4);  //����һ��ʱ��
	GPIO_SetBits(GPIOB,I2C_SCL_PIN); //�ͷ�SCL��
	Delay_us(4);  //����һ��ʱ��
	GPIO_ResetBits(GPIOB,I2C_SCL_PIN); //����SCL��
	Delay_us(4);  //����һ��ʱ��
	
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

	ret=GPIO_ReadInputDataBit(GPIOB, I2C_SDA_PIN); //��ȡSDA�ߵ�״̬���ж��Ƿ��ͷ�
	if(ret == 0) 
	{
		GPIO_SetBits(GPIOB,I2C_SDA_PIN); //�ͷ�SDA��
		GPIO_SetBits(GPIOB,I2C_SCL_PIN); //�ͷ�SCL��
		return 0; //���SDA��û�б��ͷţ������⣡���ء�
	}
	
	GPIO_ResetBits(GPIOB,I2C_SDA_PIN); //����SDA�ߣ��Բ���stop�ź�
	Delay_us(4);  //����һ��ʱ��
	GPIO_SetBits(GPIOB,I2C_SCL_PIN); //�ͷ�SCL�ߣ��Բ���stop�ź�
	Delay_us(4);  //����һ��ʱ��
	GPIO_SetBits(GPIOB,I2C_SDA_PIN); //�ͷ�SDA�ߣ�����stop�ź�
	Delay_us(12);  //����һ��ʱ��
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
	GPIO_InitType  GPIO_InitStructure; // ����GPIO ���ò����ṹ��
	/*Enable the I2C GPIO Clock*/
	RCC_APB2PeriphClockCmd(I2C_GPIO_RCC_CLK, ENABLE);

	/** GPIO configuration*/
	GPIO_InitStructure.GPIO_Pins =	I2C_SCL_PIN | I2C_SDA_PIN;  // ����I2C�Ĺܽ�
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;  // ����GPIO�˿ڵ�ʱ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_OD;		// ����GPIO Ϊ��©���
	GPIO_Init(I2C_PORT, &GPIO_InitStructure);  //����GPIO���ú���

	GPIO_SetBits(I2C_PORT,I2C_SDA_PIN); //OD���1��ζ���ͷ� SDA �� 
	GPIO_SetBits(I2C_PORT,I2C_SCL_PIN); //OD���1��ζ���ͷ� SCL ��	

	Delay_us(20); //�ȴ�GPIO�����ȶ�
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

	Delay_ms(5); //����HDC2080�����ֲ���ָ�����豸�ϵ��������Ҫ3mS��ʱ����׼������

	//--------------��ʼ�����üĴ�����������ģʽ�趨Ϊ�Զ�������ÿ��һ��--------------
	genStart(); //����Start�ź�
	ret=shiftOutData8(SLAVE_ADDRESS);//���ʹ��豸��ַ��дģʽ���λ��0
	if(ret == 0)
	{
		printf("ERROR: send slave address no ACK !\n");
		return 0;
	}

	ret=shiftOutData8(0x0E); //�������üĴ�����ַ
	if(ret == 0)
	{
		printf("ERROR: send register address no ACK !\n");
		return 0;
	}

	ret=shiftOutData8(0x50); //�������üĴ���ֵ������Ϊ�Զ�����ģʽ��ÿ�����һ��
	if(ret == 0)
	{
		printf("ERROR: send register data no ACK !\n");
		return 0;
	}
	
	ret=genStop(); //����Stop�ź�

	//--------------���ô����Ĵ�����ʼ����--------------
	genStart(); //����Start�ź�
	ret=shiftOutData8(SLAVE_ADDRESS);//���ʹ��豸��ַ��дģʽ���λ��0
	if(ret == 0)
	{
		printf("ERROR: send slave address no ACK !\n");
		return 0;
	}

	ret=shiftOutData8(0x0F); //���Ͳ������üĴ�����ַ
	if(ret == 0)
	{
		printf("ERROR: send register address no ACK !\n");
		return 0;
	}

	ret=shiftOutData8(0x01); //���Ͳ������üĴ���ֵ����ʼ����
	if(ret == 0)
	{
		printf("ERROR: send register data no ACK !\n");
		return 0;
	}
	
	ret=genStop(); //����Stop�ź�


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

	genStart(); //����Start�ź�
	ret=shiftOutData8(SLAVE_ADDRESS);//���ʹ��豸��ַ��дģʽ���λ��0
	if(ret == 0)
	{
		printf("ERROR: send slave address no ACK !\n");
		return 0;
	}

	ret=shiftOutData8(0x00); //�����¶ȼĴ�����ʼ��ַ
	if(ret == 0)
	{
		printf("ERROR: send register address no ACK !\n");
		return 0;
	}
	
	genStart(); //����Start�ź�
	ret=shiftOutData8(SLAVE_ADDRESS|0x01);//���ʹ��豸��ַ����ģʽ���λ��1��������ȡ����
	for(i=0;i<size-1;i++)
	{
		pBuffer[i]=shiftInData8(1); //ǰ���������ҪACK
	}
	
	pBuffer[i]=shiftInData8(0); //���һ�����ݲ���ҪACK

	ret=genStop(); //����Stop�ź�
	
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

	genStart(); //����Start�ź�
	ret=shiftOutData8(SLAVE_ADDRESS);//���ʹ��豸��ַ��дģʽ���λ��0
	if(ret == 0)
	{
		printf("ERROR: send slave address no ACK !\n");
		return 0;
	}

	ret=shiftOutData8(0xFE); //����ID�Ĵ�����ʼ��ַ
	if(ret == 0)
	{
		printf("ERROR: send register address no ACK !\n");
		return 0;
	}
	
	genStart(); //�ٴη���Start�ź�
	ret=shiftOutData8(SLAVE_ADDRESS|0x01);//���ʹ��豸��ַ����ģʽ���λ��1��������ȡ����
	for(i=0;i<size-1;i++)
	{
		pBuffer[i]=shiftInData8(1); //ǰ���������ҪACK
	}
	
	pBuffer[i]=shiftInData8(0); //���һ�����ݲ���ҪACK

	ret=genStop(); //����Stop�ź�
	
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
