/**
  ******************************************************************************
  * @file    usb_hcd.h
  * @author  Artery Technology
  * @version V1.0.4
  * @date    2019-05-27
  * @brief   Host layer Header file
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
#ifndef __USB_HCD_H__
#define __USB_HCD_H__

/* Includes ------------------------------------------------------------------*/
#include "usb_regs.h"
#include "usb_core.h"


/** @addtogroup USB_OTG_DRIVER
  * @{
  */
  
/** @defgroup USB_HCD
  * @brief This file is the 
  * @{
  */ 


/** @defgroup USB_HCD_Exported_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USB_HCD_Exported_Types
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USB_HCD_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_HCD_Exported_Variables
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_HCD_Exported_FunctionsPrototype
  * @{
  */ 
uint32_t  USB_HCD_Init                 (USB_OTG_CORE_HANDLE *pusbdev ,
                                    USB_OTG_CORE_ID_TypeDef coreid);
uint32_t  USB_HCD_HCH_Init              (USB_OTG_CORE_HANDLE *pusbdev , 
                                    uint8_t hc_num); 
uint32_t  USB_HCD_SubmitRequest        (USB_OTG_CORE_HANDLE *pusbdev , 
                                    uint8_t hc_num) ;
uint32_t  USB_HCD_GetCurrentSpeed      (USB_OTG_CORE_HANDLE *pusbdev);
uint32_t  USB_HCD_ResetPort            (USB_OTG_CORE_HANDLE *pusbdev);
uint32_t  USB_HCD_IsDeviceConnected    (USB_OTG_CORE_HANDLE *pusbdev);
uint32_t  USB_HCD_IsPortEnabled         (USB_OTG_CORE_HANDLE *pusbdev);

uint32_t  USB_HCD_GetCurrentFrame      (USB_OTG_CORE_HANDLE *pusbdev) ;
URB_STATE USB_HCD_GetURB_State         (USB_OTG_CORE_HANDLE *pusbdev,  uint8_t ch_num); 
uint32_t  USB_HCD_GetXferCnt           (USB_OTG_CORE_HANDLE *pusbdev,  uint8_t ch_num); 
HCH_STS USB_HCD_GetHCState           (USB_OTG_CORE_HANDLE *pusbdev,  uint8_t ch_num) ;
/**
  * @}
  */ 

#endif //__USB_HCD_H__


/**
  * @}
  */ 

/**
  * @}
  */ 
/************************ (C) COPYRIGHT Artery Technology *****END OF FILE****/

