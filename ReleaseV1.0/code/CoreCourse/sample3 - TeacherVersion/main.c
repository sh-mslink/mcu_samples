/**
  ******************************************************************************
  * @file    Templates/main.c 
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Main program body
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
  * <h2><center>&copy; COPYRIGHT 2018 ArteryTek</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "at32f4xx.h"
#include "at32_board.h"
/** @addtogroup AT32F407_StdPeriph_Templates
  * @{
  */

/** @addtogroup Template
  * @{
  */

/* Private define ------------------------------------------------------------*/
#define DELAY          100
#define FAST           1
#define SLOW           4

#define RxDATASIZE   4  //I2C�������ݻ���Ĵ�С

/* Extern variables ---------------------------------------------------------*/
extern GPIO_Type *BUTTON_GPIO_PORT[BUTTON_NUM];
extern uint16_t BUTTON_GPIO_PIN[BUTTON_NUM];
extern uint32_t BUTTON_GPIO_RCC_CLK [BUTTON_NUM];

/* Private variables ---------------------------------------------------------*/
uint16_t BUTTON_EXTI_LINE[BUTTON_NUM] = {EXTI_Line0, EXTI_Line13};
uint16_t BUTTON_EXTI_IRQ[BUTTON_NUM] = {EXTI0_IRQn, EXTI15_10_IRQn};
uint8_t BUTTON_EXTI_SOURCE_PORT[BUTTON_NUM] = {GPIO_PortSourceGPIOA, GPIO_PortSourceGPIOC};
uint8_t BUTTON_EXTI_SOURCE_PIN[BUTTON_NUM] = {GPIO_PinsSource0, GPIO_PinsSource13};

u8 receiveBuffer[RxDATASIZE]={0}; //I2C�������ݻ���

BUTTON_Type gButtonType = BUTTON_WAKEUP;
uint8_t gSpeed = FAST;

/* Extern function ---------------------------------------------------------*/
extern void SW_I2C_Init(void);
extern u8 HDC2080_Init(void);
extern u8 readHDC2080Data(u8 *pBuffer, u8 size);
extern u8 readHDC2080ID(u8 *pBuffer, u8 size);
extern void calTempAndHumi(u8 *pBuffer, float *pTemp, float *pHumi);
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Config Clock Out Function.
  * @param  None
  * @retval None
  */
void MCO_Config(void)
{
  GPIO_InitType GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_8;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*PA8���PLL/4*/
  RCC_CLKOUTConfig(RCC_CLKOUT_PLL_Div4, RCC_MCOPRE_1);
}

/**
  * @brief  Configure Button EXTI   
  * @param  Button: Specifies the Button to be configured.
  * @retval None
  */
void BUTTON_EXTI_Init(BUTTON_Type button)
{
  EXTI_InitType EXTI_InitStructure;   //����һ��ר�õĽṹ�壬���������ж���
  NVIC_InitType NVIC_InitStructure;   //����һ��ר�õĽṹ�壬���������жϿ�����

  GPIO_EXTILineConfig(BUTTON_EXTI_SOURCE_PORT[button], BUTTON_EXTI_SOURCE_PIN[button]); //����һ��ר�ú��������ڲ��ж��ߺ��ⲿ�Ĺܽ���������
  EXTI_StructInit(&EXTI_InitStructure);  //��ǰ�������Ľṹ���е�������Ĭ��ֵ����ʼ��
  //---------����ʵ�������ʼ�������жϽṹ��
	EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[button];
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_LineEnable = ENABLE;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_Init(&EXTI_InitStructure); // �ýṹ���е���������ʼ���ж���
  //---------����ʵ�������ʼ���жϿ������ṹ��
  NVIC_InitStructure.NVIC_IRQChannel = BUTTON_EXTI_IRQ[button];
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_Init(&NVIC_InitStructure); // �ýṹ���е���������ʼ���жϿ�����
}

/**
  * @brief  Button EXTI0 Interrupt Handler
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  Delay_ms(5);
  EXTI_ClearIntPendingBit(BUTTON_EXTI_LINE[gButtonType]);
  if ((GPIO_ReadInputData(BUTTON_GPIO_PORT[gButtonType]) & BUTTON_GPIO_PIN[gButtonType]) == \
      BUTTON_GPIO_PIN[gButtonType])
  {
    if(gSpeed == SLOW)
      gSpeed = FAST;
    else
      gSpeed = SLOW;
  }
	printf("I am here : %s \n", __func__);
}


/**
  * @brief  Button EXTI13 Interrupt Handler
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  Delay_ms(5);
  EXTI_ClearIntPendingBit(BUTTON_EXTI_LINE[gButtonType]);
  if ((GPIO_ReadInputData(BUTTON_GPIO_PORT[gButtonType]) & BUTTON_GPIO_PIN[gButtonType]) == \
      BUTTON_GPIO_PIN[gButtonType])
  {
    if(gSpeed == SLOW)
      gSpeed = FAST;
    else
      gSpeed = SLOW;
  }
	printf("I am here : %s \n", __func__);
}


/**
  * @brief  Main Function.
  * @param  None
  * @retval None
  */
int main(void)
{
  u8 ret=0;
  float temp,humi;
  gButtonType = BUTTON_WAKEUP; // ���尴��������ֵ��������û��ʵ�����ã�������ʼ����

  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO, ENABLE);  // ʹ��APB2�����ϵ�I/O����ģ���ʱ��
																											// IO����ģ����Խ�оƬ�ڵĲ�ͬ����ӳ�䵽IO��

  AT32_Board_Init();  // ������������԰�����LED�ȹ�������ʼ����
											// ������뺯�����ڣ����Կ�����������������USER��ŵİ�����
  MCO_Config();  // ����ʱ������ܽ�

  BUTTON_EXTI_Init(gButtonType);  // �����ж�Դ���Ѱ������¼���ϵ���ж�Դ��
	
  UART_Print_Init(115200);  //���ó�ʼ�����򣬲����ò�����Ϊ115200 

  SW_I2C_Init(); //��ʼ�����ڷ���HDC2080��I2C����
  HDC2080_Init();  //��ʼ��HDC2080
	ret=readHDC2080ID(receiveBuffer, 2);
	printf("ID [%x,%x]\n",receiveBuffer[0],receiveBuffer[1]);
	
  while(1)
  {
    AT32_LEDn_Toggle(LED2);  // ��תLED2��״̬
    Delay_ms(gSpeed * DELAY); // delayһ��ʱ��
    AT32_LEDn_Toggle(LED3);   // ��תLED3��״̬
    Delay_ms(gSpeed * DELAY); // delayһ��ʱ��
    AT32_LEDn_Toggle(LED4);  //��תLED4��״̬
    Delay_ms(gSpeed * DELAY);  // delayһ��ʱ��

    ret=readHDC2080Data(receiveBuffer, RxDATASIZE);
		if(ret == 0)
		{
			printf("data read error~! \n");
		}
		else
		{
			calTempAndHumi(receiveBuffer, &temp, &humi);
			printf("temperature=%f,\t humidity=%f\n",temp,humi);
		}
		
		Delay_ms(500);
  }
	
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @}
  */

/**
  * @}
  */


