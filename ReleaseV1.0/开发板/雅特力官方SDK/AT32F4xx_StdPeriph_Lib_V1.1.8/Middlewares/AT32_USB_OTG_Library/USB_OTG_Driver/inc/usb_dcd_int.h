/**
  ******************************************************************************
  * @file    usb_defines.h
  * @author  Artery Technology
  * @version V1.0.4
  * @date    2019-05-27
  * @brief   Peripheral Device Interface Layer
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USB_DCD_INT_H__
#define USB_DCD_INT_H__

/* Includes ------------------------------------------------------------------*/
#include "usb_dcd.h"



/** @addtogroup USB_OTG_DRIVER
  * @{
  */
  
/** @defgroup USB_DCD_INT
  * @brief This file is the 
  * @{
  */ 


/** @defgroup USB_DCD_INT_Exported_Defines
  * @{
  */ 

typedef struct _USBD_DCD_INT
{
  uint8_t (* DataOutStage) (USB_OTG_CORE_HANDLE *pusbdev , uint8_t bepnum);
  uint8_t (* DataInStage)  (USB_OTG_CORE_HANDLE *pusbdev , uint8_t bepnum);
  uint8_t (* SetupStage) (USB_OTG_CORE_HANDLE *pusbdev);
  uint8_t (* SOF) (USB_OTG_CORE_HANDLE *pusbdev);
  uint8_t (* Reset) (USB_OTG_CORE_HANDLE *pusbdev);
  uint8_t (* Suspend) (USB_OTG_CORE_HANDLE *pusbdev);
  uint8_t (* Resume) (USB_OTG_CORE_HANDLE *pusbdev);
  uint8_t (* IsoINIncomplete) (USB_OTG_CORE_HANDLE *pusbdev);
  uint8_t (* IsoOUTIncomplete) (USB_OTG_CORE_HANDLE *pusbdev);  
  
  uint8_t (* DevConnected) (USB_OTG_CORE_HANDLE *pusbdev);
  uint8_t (* DevDisconnected) (USB_OTG_CORE_HANDLE *pusbdev);   
  
}USB_DEVICE_DCD_INT_cb_Type;

extern USB_DEVICE_DCD_INT_cb_Type *USB_DEVICE_DCD_INT_fops;
/**
  * @}
  */ 


/** @defgroup USB_DCD_INT_Exported_Types
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_DCD_INT_Exported_Macros
  * @{
  */ 

#define CLEAR_IN_EP_INTR(bepnum,intr) \
  diepint.u32val=0; \
  diepint.b.intr = 1; \
  USB_OTG_WRITE_R32(&pusbdev->regs.INEP_REGS[bepnum]->DINEPTINT,diepint.u32val);

#define CLEAR_OUT_EP_INTR(bepnum,intr) \
  doepint.u32val=0; \
  doepint.b.intr = 1; \
  USB_OTG_WRITE_R32(&pusbdev->regs.OUTEP_REGS[(bepnum)]->DOUTEPTINT,doepint.u32val);

/**
  * @}
  */ 

/** @defgroup USB_DCD_INT_Exported_Variables
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_DCD_INT_Exported_FunctionsPrototype
  * @{
  */ 

uint32_t USB_DEVICE_OTG_INTSTS_Handler (USB_OTG_CORE_HANDLE *pusbdev);
uint32_t USBD_OTG_EP1OUT_ISR_Handler (USB_OTG_CORE_HANDLE *pusbdev);
uint32_t USBD_OTG_EP1IN_ISR_Handler (USB_OTG_CORE_HANDLE *pusbdev);
/**
  * @}
  */ 


#endif /* USB_DCD_INT_H__ */

/**
  * @}
  */ 

/**
  * @}
  */ 
/************************ (C) COPYRIGHT Artery Technology *****END OF FILE****/

