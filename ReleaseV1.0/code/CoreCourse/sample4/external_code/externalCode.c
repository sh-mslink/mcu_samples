/**
  ******************************************************************************
  * @file    externalCode.c
  * @author  mSlink Technology 
  * @version V1.1.2
  * @date    2020-08-25
  * @brief   This file contains the function SPIM_run used to check the LED toggle in SPIM
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
#include "at32f4xx.h"  
#include "at32_board.h"

/** @addtogroup AT32F407_StdPeriph_Examples
  * @{
  */

/** @addtogroup FLASH_Run_In_SPIM
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Check the LED toggle in SPIM
  * @param  None
  * @retval None
  */
void externalCodeRun(void)
{
	/* Toggle LED2 */
	AT32_LEDn_Toggle(LED2);
	Delay_ms(200);
	printf("I am here: %s @ 0x%x ~!\r\n", __func__,&(externalCodeRun)); //打印该函数所处的地址
}


/**
  * @}
  */ 

/**
  * @}
  */ 
  
/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/
