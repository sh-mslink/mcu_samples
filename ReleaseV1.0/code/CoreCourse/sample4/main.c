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
#include "externalCode.h"
#include "at32f4xx.h"
#include "at32_board.h"
/** @addtogroup AT32F407_StdPeriph_Templates
  * @{
  */

/** @addtogroup Template
  * @{
  */

/* Private define ------------------------------------------------------------*/

/* Extern variables ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Extern function ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Configures the ext.flash
  * @param  None
  * @retval None
  */
void FLASH_InitExtFlash(void)
{
  GPIO_InitType GPIO_InitStructure;  //声明一个用来配置GPIO的结构体
  
  /* Enable ext.flash GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO|RCC_APB2PERIPH_GPIOA|RCC_APB2PERIPH_GPIOB, ENABLE);  //使能对应的GPIO的时钟
  
  /* Configure ext.flash pin */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_8; //定义外部flash接口用到的端口A中的GPIO
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //打开GPIO复用功能
  GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化GPIO端口A
  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_1|GPIO_Pins_6|GPIO_Pins_7|GPIO_Pins_10|GPIO_Pins_11;//定义外部flash接口用到的端口B中的GPIO
  GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化GPIO端口B
  
  /* Select PB10, PB11 as SPIF_TX and SPIF_RX */
  GPIO_PinsRemapConfig(GPIO_Remap_EXT_FLASH, ENABLE); 
  GPIO_PinsRemapConfig(AFIO_MAP7_SPIF_1001, ENABLE);
  
  /* In this example, use on-board EN25QH128A as ext.flash */
  FLASH->B3SEL = FLASH_BANK3_TYPE2;  //外部Flash被映射到BANK3
  
  /* Unlock the ext.flash program erase controller */  
  while (BIT_READ(FLASH->STS3, FLASH_STS_BSY)); //等待Flash空闲
  FLASH->FCKEY3 = FLASH_KEY1; //解锁Flash需要的Key值，根据不同的Flash其值可能不同
  FLASH->FCKEY3 = FLASH_KEY2; //解锁Flash需要的Key值，根据不同的Flash其值可能不同
  while (BIT_READ(FLASH->CTRL3, FLASH_CTRL_LCK)); //等待解锁完毕
  
  /* If the data written to ext.flash need to be scrambled, please specify the scrambled range */
  FLASH->DA = 0;  
  
  return;
}

/**
  * @brief  Main Function.
  * @param  None
  * @retval None
  */
int main(void)
{
	/* Init LED */
  AT32_Board_Init(); 
  
	UART_Print_Init(115200);
  
	printf("this message is running in bank1.\r\n");
  
  /* Configures the ext.flash */
  FLASH_InitExtFlash();
  
	while(1)
	{
		printf("main loop @ 0x%x\r\n", &(main));
		/* Toggle LED3 */
		AT32_LEDn_Toggle(LED3);
		Delay_ms(200);
		/* Toggle LED4 */
		AT32_LEDn_Toggle(LED4);
		Delay_ms(200);

		externalCodeRun();    
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



