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

#include "test.h"

/* Private define ------------------------------------------------------------*/
#define DELAY          200


/* Extern variables ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/



/**
  * @brief  Main Function.
  * @param  None
  * @retval None
  */
int main(void)
{
	int c=3;
	int *p=0;

  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO, ENABLE);
  AT32_Board_Init(); 

	UART_Print_Init(115200);
	
	//tableTest();
	LinkedListTest();
	
	
  while(1)
  {
    AT32_LEDn_Toggle(LED2);  // 反转LED2的状态
    Delay_ms(DELAY); // delay一段时间
    AT32_LEDn_Toggle(LED3);   // 反转LED3的状态
    Delay_ms(DELAY); // delay一段时间
    AT32_LEDn_Toggle(LED4);  //反转LED4的状态
    Delay_ms(DELAY);  // delay一段时间
		//printf("Hello World~! \n\r");
		
		//p=&c;

		//printf("c address is 0x%x, c=%d\n", (uint32_t)&c,c);
		//printf("p=0x%x, *p=%d\n",(uint32_t)p,*p);
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



