/**
  ******************************************************************************
  * @file    xmc_lcd.h
  * @author  Artery
  * @version V4.5.0
  * @date    07-March-2011
  * @brief   This file contains all the functions prototypes for the 
  *          xmc_lcd firmware driver.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, ARTERY SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 Artery</center></h2>
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __XMC_LCD_H
#define __XMC_LCD_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
   
#include "at32f4xx.h"
#include "at32_board.h"
/** @addtogroup XMC_LCD_8BIT
  * @{
  */ 
  
/** @addtogroup XMC_Utilities
  * @{
  */
 

/** @defgroup AT32F407_XMC_LCD_Exported_Functions
  * @{
  */ 
   
/* The address of write data & command (XMC_A0) */
#define  XMC_LCD_COMMAND    0x6c000000
#define  XMC_LCD_DATA       0x6c000001
/* The LCD Reset line */
#define LCD_RESET_HIGH  GPIO_SetBits(GPIOB,GPIO_Pins_10);
#define LCD_RESET_LOW   GPIO_ResetBits(GPIOB,GPIO_Pins_10);

#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	   0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40
#define BRRED 			     0XFC07
#define GRAY  			     0X8430

void XMC_init(void);
void LCD_init(void);
void LCD_WR_COMMAND(u8 command);
void LCD_WR_DATA(u8 data);
void LCD_SetBlock(u16 xstart,u16 ystart,u16 xend,u16 yend);
void LCD_WriteOnePoint(u16 color);
void LCD_DrawPoint(u16 x,u16 y,u16 color);
void LCD_Clear(u16 color);
void LCD_Display_Number(void);

#ifdef __cplusplus
}
#endif

#endif /* __AT_START_XMC_LCD_8BIT_H */
/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 Artery *****END OF FILE****/
