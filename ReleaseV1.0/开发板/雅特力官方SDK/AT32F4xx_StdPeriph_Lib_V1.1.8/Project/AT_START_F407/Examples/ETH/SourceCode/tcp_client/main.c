/**
 ******************************************************************************
 * @file        ETH/tcp_client/main.c
 * @brief       TCP server function test
 * @author      Artery Technology
 * @date        2019-01-04
 * @version     V1.1.2
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
 * <h2><center>&copy; COPYRIGHT 2019 ArteryTek</center></h2>
 ******************************************************************************
 */
/* Includes -----------------------------------------------------------------*/
#include "at32_board.h"
#include "main.h"
#include "eth_config.h"

/** @addtogroup AT32F407_StdPeriph_Examples
  * @{
  */

/** @addtogroup ETH_TCP_Client
  * @{
  */

/* Gloable variables ---------------------------------------------------------*/
__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
uint32_t timingdelay;
unsigned char tcp_rec_flag = 0;

/* Gloable functions ---------------------------------------------------------*/
/**
  * @brief  Main Function.
  * @param  None
  * @retval None
  */
int main(void)
{
    unsigned char tcp_data[] = "TCP client experiment!\n";
    struct tcp_pcb *pcb;
    /* --------BSP Init ------------------------------------------------*/
    AT32_Board_Init();
    UART_Print_Init(115200); 
    /* Setup AT32 system (clocks, Ethernet, GPIO, NVIC) */
    System_Setup();
    /* Initilaize the LwIP satck */
    LwIP_Init();
    /* Initialize TCP client module */
    TCP_Client_Init(TCP_LOCAL_PORT,TCP_SERVER_PORT,TCP_SERVER_IP);  //Initialize TCP client
    while(1)
    { 
        pcb = Check_TCP_Connect();
        if(pcb != 0)
        {	
          TCP_Client_Send_Data(pcb,tcp_data,sizeof(tcp_data));      //Send data to TCP server actively
        }
        Delay_s(0xfffff);											                      //necessary delay
        System_Periodic_Handle();									                  //Update TCP timers
    }
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: number of 10ms periods to wait for.
  * @retval None
  */
void Delay(uint32_t nCount)
{
  /* Capture the current local time */
  timingdelay = LocalTime + nCount;  

  /* wait until the desired delay finish */  
  while(timingdelay > LocalTime)
  {     
  }
}

/**
  * @brief  Updates the system local time
  * @param  None
  * @retval None
  */
void Time_Update(void)
{
  LocalTime += SYSTEMTICK_PERIOD_MS;
}

/**
  * @brief  Handles the periodic tasks of the system
  * @param  None
  * @retval None
  */
void System_Periodic_Handle(void)
{
  /* Manage the IP address setting */
  Display_Periodic_Handle(LocalTime);
  
  /* LwIP periodic services are done here */
  LwIP_Periodic_Handle(LocalTime);
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
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

/******************* (C) COPYRIGHT 2019 ArteryTek *****END OF FILE****/

