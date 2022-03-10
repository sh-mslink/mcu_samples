/**
  ******************************************************************************
  * @file    run_in_SysMem/SysMem/run_in_SysMem.c
  * @author  Artery Technology 
  * @version V1.0.0
  * @date    2019-07-25
  * @brief   This file contains the function SysMem_run used to check the LED toggle in SysMem
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

/** @addtogroup AT32F415_StdPeriph_Examples
  * @{
  */

/** @addtogroup SysMem_AP_EXAMPLE
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Check the LED toggle in SysMem
  * @param  None
  * @retval None
  */
void SysMem_run(void)
{
  /* Init LED */
  AT32_Board_Init();    
  while(1)
  {
    /* Toggle LED2 */
    AT32_LEDn_Toggle(LED2);
    Delay_ms(100);
  }
}


/**
  * @}
  */ 

/**
  * @}
  */ 
  
/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/
