 /**
  ******************************************************************************
  * @file    usbd_core.h
  * @author  Artery Technology
  * @version V1.0.4
  * @date    2019-05-27
  * @brief   Header file for usbd_core.c
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
#ifndef __USBD_CORE_H
#define __USBD_CORE_H

/* Includes ------------------------------------------------------------------*/
#include "usb_dcd.h"
#include "usbd_def.h"
#include "usbd_conf.h"

/** @addtogroup AT32_USB_OTG_DEVICE_LIBRARY
  * @{
  */
  
/** @defgroup USBD_CORE
  * @brief This file is the Header file for usbd_core.c file
  * @{
  */ 


/** @defgroup USBD_CORE_Exported_Defines
  * @{
  */ 

typedef enum {
  USB_DEVICE_OK   = 0,
  USB_DEVICE_BUSY,
  USB_DEVICE_FAIL,
}USB_Device_Status;
/**
  * @}
  */ 


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */
 

/**
  * @}
  */ 



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup USBD_CORE_Exported_FunctionsPrototype
  * @{
  */ 
void USB_Device_Init(USB_OTG_CORE_HANDLE *pusbdev,
               USB_OTG_CORE_ID_TypeDef coreid, 
               USB_DEVICE_USER *pDevice,                  
               USBD_Class_cb_Type *class_cb, 
               USB_Device_User_cb_Type *user_cb);

USB_Device_Status USB_Device_Reset(USB_OTG_CORE_HANDLE *pusbdev);

USB_Device_Status USB_Device_ClrCfg(USB_OTG_CORE_HANDLE  *pusbdev, uint8_t cfgidx);

USB_Device_Status USB_Device_SetCfg(USB_OTG_CORE_HANDLE  *pusbdev, uint8_t cfgidx);

/**
  * @}
  */ 

#endif /* __USBD_CORE_H */

/**
  * @}
  */ 

/**
* @}
*/ 

/************************ (C) COPYRIGHT Artery Technology *****END OF FILE****/



