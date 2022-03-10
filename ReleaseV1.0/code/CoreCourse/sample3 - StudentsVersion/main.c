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

/* Extern variables ---------------------------------------------------------*/
extern GPIO_Type *BUTTON_GPIO_PORT[BUTTON_NUM];
extern uint16_t BUTTON_GPIO_PIN[BUTTON_NUM];
extern uint32_t BUTTON_GPIO_RCC_CLK [BUTTON_NUM];

/* Private variables ---------------------------------------------------------*/
uint16_t BUTTON_EXTI_LINE[BUTTON_NUM] = {EXTI_Line0, EXTI_Line13};
uint16_t BUTTON_EXTI_IRQ[BUTTON_NUM] = {EXTI0_IRQn, EXTI15_10_IRQn};
uint8_t BUTTON_EXTI_SOURCE_PORT[BUTTON_NUM] = {GPIO_PortSourceGPIOA, GPIO_PortSourceGPIOC};
uint8_t BUTTON_EXTI_SOURCE_PIN[BUTTON_NUM] = {GPIO_PinsSource0, GPIO_PinsSource13};

BUTTON_Type gButtonType = BUTTON_WAKEUP;
uint8_t gSpeed = FAST;

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

  /*PA8输出PLL/4*/
  RCC_CLKOUTConfig(RCC_CLKOUT_PLL_Div4, RCC_MCOPRE_1);
}

/**
  * @brief  Configure Button EXTI   
  * @param  Button: Specifies the Button to be configured.
  * @retval None
  */
void BUTTON_EXTI_Init(BUTTON_Type button)
{
  EXTI_InitType EXTI_InitStructure;   //声明一个专用的结构体，用于配置中断线
  NVIC_InitType NVIC_InitStructure;   //声明一个专用的结构体，用于配置中断控制器

  GPIO_EXTILineConfig(BUTTON_EXTI_SOURCE_PORT[button], BUTTON_EXTI_SOURCE_PIN[button]); //调用一个专用函数来将内部中断线和外部的管脚连接起来
  EXTI_StructInit(&EXTI_InitStructure);  //将前面声明的结构体中的内容用默认值做初始化
  //---------根据实际情况初始化按键中断结构体
	EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[button];
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_LineEnable = ENABLE;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_Init(&EXTI_InitStructure); // 用结构体中的内容来初始化中断线
  //---------根据实际情况初始化中断控制器结构体
  NVIC_InitStructure.NVIC_IRQChannel = BUTTON_EXTI_IRQ[button];
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_Init(&NVIC_InitStructure); // 用结构体中的内容来初始化中断控制器
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
  gButtonType = BUTTON_WAKEUP; // 定义按键，赋初值，在这里没有实际作用，后面会初始化。

  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO, ENABLE);  // 使能APB2总线上的I/O复用模块的时钟
																											// IO复用模块可以将芯片内的不同功能映射到IO上

  AT32_Board_Init();  // 在这个函数里会对按键、LED等功能做初始化。
											// 如果进入函数体内，可以看到，按键被定义在USER标号的按键上

  MCO_Config();  // 配置时钟输出管脚

  BUTTON_EXTI_Init(gButtonType);  // 配置中断源，把按键的事件联系到中断源上
	
	UART_Print_Init(115200);  //调用初始化程序，并设置波特率为115200 

  while(1)
  {
    AT32_LEDn_Toggle(LED2);  // 反转LED2的状态
    Delay_ms(gSpeed * DELAY); // delay一段时间
    AT32_LEDn_Toggle(LED3);   // 反转LED3的状态
    Delay_ms(gSpeed * DELAY); // delay一段时间
    AT32_LEDn_Toggle(LED4);  //反转LED4的状态
    Delay_ms(gSpeed * DELAY);  // delay一段时间
		//printf("Hello World~! \n"); //从串口输出字符串
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



