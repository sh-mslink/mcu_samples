/**
  ******************************************************************************
  * @file    run_in_SysMem/main.c
  * @author  Artery Technology 
  * @version V1.0.0
  * @date    2019-07-25
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
#include "run_in_SysMem.h"
#include "at32f4xx.h"
#include "at32_board.h"
#include <stdio.h>

/** @addtogroup AT32F415_StdPeriph_Examples
  * @{
  */

/** @addtogroup SysMem_AP_EXAMPLE
  * @{
  */

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{ 
  /* USART1 configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  UART_Print_Init(115200);
  printf("This message is printing in main block.\r\n");
  
  /* Check the LED toggle in System Memory */
  printf("Check the LED toggle in System Memory.\r\n");
  SysMem_run();
}

/**
  * @}
  */ 

/**
  * @}
  */ 

