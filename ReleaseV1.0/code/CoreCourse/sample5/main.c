/**
  ******************************************************************************
  * @file    main.c 
  * @author  mSlink Technology
  * @version V1.0.0
  * @date    2020-08-25
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
#include "spi_flash.h"

/** @addtogroup AT32F407_StdPeriph_Examples
  * @{
  */

/** @addtogroup SPI_EN25QH128A_Flash
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define FLASH_TEST_ADDR   0x1000  //设定测试程序要访问的Flash中的地址，这个地址需要是Flash Sector大小的的整数倍

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Buffer_Tx[BUF_SIZE];
uint8_t Buffer_Rx[BUF_SIZE];

volatile TestStatus TransferStatus = FAILED;

/* Private function prototypes -----------------------------------------------*/
void TxDataInit(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/
void TxDataInit(void)
{
  uint32_t i;
  for(i=0; i<BUF_SIZE; i++)
  {
    Buffer_Tx[i] = i;
  }
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_at32f403_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_at32f4xx.c file
     */     
  uint32_t i, Id;

  TxDataInit();  //初始化发送数据用的buffer

  UART_Print_Init(115200);

  AT32_Board_Init(); 

  SpiFlash_Init();

  Id = SpiFlash_ReadID();		//读取EN25QH128A的device ID并判断是否正确
	if(Id != EN25QH128A)
  {
    printf("Spi flash init error!\r\n");
    for(i=0;i<50;i++)
    {
     AT32_LEDn_Toggle(LED2);
     AT32_LEDn_Toggle(LED3);
     Delay_ms(100);
    }
    return 1;
  }
  else
  {
    printf("Check ID success! ID: %x\r\n", Id);
  }

  /* Erase sector */
  SpiFlash_Erase_Sector(FLASH_TEST_ADDR / SPIF_SECTOR_SIZE); //Flash在写以前必须先擦除
  /* Write Data */
  SpiFlash_Write(Buffer_Tx, FLASH_TEST_ADDR, BUF_SIZE);  //写Flash
  /* Read Data */
  SpiFlash_Read(Buffer_Rx, FLASH_TEST_ADDR, BUF_SIZE); //读Flash

  /* Printf read data */
  printf("Read Data: ");
  for(i=0; i<BUF_SIZE; i++)
  {
    printf("%x ", Buffer_Rx[i]);
  }
  printf("\r\n");

  /* Check the correctness of written data */
  TransferStatus = Buffercmp(Buffer_Tx, Buffer_Rx, BUF_SIZE);

  /* TransferStatus = PASSED, if the transmitted and received data
     are equal */
  /* TransferStatus = FAILED, if the transmitted and received data
     are different */
  /* if passed ,LED2 lights */
  if(TransferStatus==PASSED)
  {
    AT32_LEDn_ON(LED2);
  }    
  else
  {
    AT32_LEDn_OFF(LED2);
  }

  while (1)
  {}
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
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
