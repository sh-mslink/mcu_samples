/**
  ******************************************************************************
  * @file    usbd_usr.h
  * @author  Artery Technology
  * @version V1.0.4
  * @date    2019-05-27
  * @brief   Header file for usbd_usr.c
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
#ifndef __USBD_USR_H__
#define __USBD_USR_H__

/* Includes ------------------------------------------------------------------*/
#include "usbd_ioreq.h"
/** @addtogroup USBD_USER
  * @{
  */

/** @addtogroup USBD_MSC_DEMO_USER_CALLBACKS
  * @{
  */

/** @defgroup USBD_USR
  * @brief This file is the Header file for usbd_usr.c
  * @{
  */ 


/** @defgroup USBD_USR_Exported_Types
  * @{
  */ 

extern  USB_Device_User_cb_Type USER_cb;
extern  USB_Device_User_cb_Type USR_FS_cb;
extern  USB_Device_User_cb_Type USR_HS_cb;



/**
  * @}
  */ 



/** @defgroup USBD_USR_Exported_Defines
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup USBD_USR_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBD_USR_Exported_Variables
  * @{
  */ 

void     USB_DEVICE_USER_Init(void);
void     USB_DEVICE_USER_DeviceReset (uint8_t speed);
void     USB_DEVICE_USER_DeviceConfigured (void);
void     USB_DEVICE_USER_DeviceSuspended(void);
void     USB_DEVICE_USER_DeviceResumed(void);

void     USB_DEVICE_USER_DeviceConnected(void);
void     USB_DEVICE_USER_DeviceDisconnected(void); 

void     USB_DEVICE_USER_FS_Init(void);
void     USB_DEVICE_USER_FS_DeviceReset (uint8_t speed);
void     USB_DEVICE_USER_FS_DeviceConfigured (void);
void     USB_DEVICE_USER_FS_DeviceSuspended(void);
void     USB_DEVICE_USER_FS_DeviceResumed(void);

void     USB_DEVICE_USER_FS_DeviceConnected(void);
void     USB_DEVICE_USER_FS_DeviceDisconnected(void);  

/**
  * @}
  */ 

/** @defgroup USBD_USR_Exported_FunctionsPrototype
  * @{
  */ 
/**
  * @}
  */ 

#endif /*__USBD_USR_H__*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT Artery Technology *****END OF FILE****/
