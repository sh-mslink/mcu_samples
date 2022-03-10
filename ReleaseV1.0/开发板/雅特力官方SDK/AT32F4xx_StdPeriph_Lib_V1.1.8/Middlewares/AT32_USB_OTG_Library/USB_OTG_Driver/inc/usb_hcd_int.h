/**
  ******************************************************************************
  * @file    usb_hcd_int.h
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
#ifndef __HCD_INT_H__
#define __HCD_INT_H__

/* Includes ------------------------------------------------------------------*/
#include "usb_hcd.h"


/** @addtogroup USB_OTG_DRIVER
  * @{
  */
  
/** @defgroup USB_HCD_INT
  * @brief This file is the 
  * @{
  */ 


/** @defgroup USB_HCD_INT_Exported_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USB_HCD_INT_Exported_Types
  * @{
  */ 

typedef struct _USBH_HCD_INT
{
  uint8_t (* SOF) (USB_OTG_CORE_HANDLE *pusbdev);
  uint8_t (* DevConnected) (USB_OTG_CORE_HANDLE *pusbdev);
  uint8_t (* DevDisconnected) (USB_OTG_CORE_HANDLE *pusbdev);
  uint8_t (* DevPortEnabled) (USB_OTG_CORE_HANDLE *pusbdev);  
  uint8_t (* DevPortDisabled) (USB_OTG_CORE_HANDLE *pusbdev); 
  
}USB_HOST_HCD_INT_cb_Type;

extern USB_HOST_HCD_INT_cb_Type *USB_HOST_HCD_INT_fops;
/**
  * @}
  */ 


/** @defgroup USB_HCD_INT_Exported_Macros
  * @{
  */ 

#define CLEAR_HCH_INT(HC_REGS, intr) \
  {\
  USB_OTG_HCINTn_Type  hcint_clear; \
  hcint_clear.u32val = 0; \
  hcint_clear.b.intr = 1; \
  USB_OTG_WRITE_R32(&((HC_REGS)->HCINT), hcint_clear.u32val);\
  }\

#define MASK_HOST_INT_CHH(hc_num) { USB_OTG_HCINTMSK_Type  INTMSK; \
    INTMSK.u32val = USB_OTG_READ_R32(&pusbdev->regs.HC_REGS[hc_num]->HCINTMASK); \
    INTMSK.b.chhltm = 0; \
    USB_OTG_WRITE_R32(&pusbdev->regs.HC_REGS[hc_num]->HCINTMASK, INTMSK.u32val);}

#define UNMASK_HOST_INT_CHH(hc_num) { USB_OTG_HCINTMSK_Type  INTMSK; \
    INTMSK.u32val = USB_OTG_READ_R32(&pusbdev->regs.HC_REGS[hc_num]->HCINTMASK); \
    INTMSK.b.chhltm = 1; \
    USB_OTG_WRITE_R32(&pusbdev->regs.HC_REGS[hc_num]->HCINTMASK, INTMSK.u32val);}

#define MASK_HOST_INT_ACK(hc_num) { USB_OTG_HCINTMSK_Type  INTMSK; \
    INTMSK.u32val = USB_OTG_READ_R32(&pusbdev->regs.HC_REGS[hc_num]->HCINTMASK); \
    INTMSK.b.ackm = 0; \
    USB_OTG_WRITE_R32(&pusbdev->regs.HC_REGS[hc_num]->HCINTMASK, GINTMASK.u32val);}

#define UNMASK_HOST_INT_ACK(hc_num) { USB_OTG_HCGINTMSK_TypeDef  INTMSK; \
    INTMSK.u32val = USB_OTG_READ_R32(&pusbdev->regs.HC_REGS[hc_num]->HCINTMASK); \
    INTMSK.b.ackm = 1; \
    USB_OTG_WRITE_R32(&pusbdev->regs.HC_REGS[hc_num]->HCINTMASK, INTMSK.u32val);}

/**
  * @}
  */ 

/** @defgroup USB_HCD_INT_Exported_Variables
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_HCD_INT_Exported_FunctionsPrototype
  * @{
  */ 
/* Callbacks handler */
void ConnectCallback_Handler(USB_OTG_CORE_HANDLE *pusbdev);
void Disconnect_Callback_Handler(USB_OTG_CORE_HANDLE *pusbdev);
void Overcurrent_Callback_Handler(USB_OTG_CORE_HANDLE *pusbdev);
uint32_t USB_HOST_OTG_INTSTS_Handler (USB_OTG_CORE_HANDLE *pusbdev);

/**
  * @}
  */ 



#endif //__HCD_INT_H__


/**
  * @}
  */ 

/**
  * @}
  */ 
/************************ (C) COPYRIGHT Artery Technology *****END OF FILE****/

