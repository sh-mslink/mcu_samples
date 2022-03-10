/**
  ******************************************************************************
  * @file    IAP_APP_Demo/SYSMEM_IAP_Bootloader/main.c
  * @author  Artery Technology
  * @version V1.0.0
  * @date    2019-07-30
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

#include <stdio.h>
#include "at32f4xx.h"
#include "at32_board.h"

/** @addtogroup AT32F415_StdPeriph_Examples
  * @{
  */

/** @addtogroup SYSMEM_IAP_Bootloader
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define APP_ADDR  0x08000000
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

typedef void (*iapfun)(void);				//定义一个函数类型的参数.
iapfun jump2app; 
//跳转到应用程序段
//appxaddr:用户代码起始地址.
void IAP_Load_APP(uint32_t appxaddr)
{
	if(((*(uint32_t*)appxaddr)&0x2FFF8000)==0x20000000)	//检查栈顶地址是否合法.
	{
		jump2app=(iapfun)*(uint32_t*)(appxaddr+4);				//用户代码区第二个字为程序开始地址(复位地址)		
		__set_MSP(*(uint32_t*)appxaddr);									//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();																		//跳转到APP.
	}
}		

/**
  * @brief  Main Function.
  * @param  None
  * @retval None
  */
int main(void)
{
  AT32_Board_Init();
  UART_Print_Init(115200);
  printf("System memory as user-defined IAP if running.\r\n");
  printf("Jump to APP.\r\n");
  IAP_Load_APP(APP_ADDR);
  for(;;)
  {

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

/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/ 
