/**
 **************************************************************************
 * File Name    : at32f4xx_it.c
 * Description  : at32f4xx interrupt service routines.
 * Date         : 2018-01-10
 * Version      : V1.0.4
 **************************************************************************
 */


/* Includes ------------------------------------------------------------------*/
#include "at32f4xx_it.h"

/** @addtogroup AT32F407_StdPeriph_Examples
  * @{
  */

/** @addtogroup DAC_DualModeDMA_SineWave
  * @{
  */ 
 
u32 gTimerUS=0;
u32 gTimerMS=0;
static u32 mSCounter=1000;

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}


void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
	u32 temp;
	temp = SysTick->CTRL;
	mSCounter--;
	gTimerUS++;
	if(mSCounter==0)
	{
		gTimerMS++;
		mSCounter=1000;
	}
}

/******************************************************************************/
/*                 at32f4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_at32f403_xx.s).                                            */
/******************************************************************************/
/**
  * @}
  */ 

/**
  * @}
  */ 
  
