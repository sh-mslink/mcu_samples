/**
  ******************************************************************************
  * @file    usb_req.h
  * @author  Artery Technology
  * @version V1.0.4
  * @date    2019-05-27
  * @brief   Header file for the usbd_req.c file
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

#ifndef __USB_REQUEST_H_
#define __USB_REQUEST_H_

/* Includes ------------------------------------------------------------------*/
#include  "usbd_def.h"
#include  "usbd_core.h"
#include  "usbd_conf.h"


/** @addtogroup AT32_USB_OTG_DEVICE_LIBRARY
  * @{
  */
  
/** @defgroup USBD_REQ
  * @brief header file for the usbd_ioreq.c file
  * @{
  */ 

/** @defgroup USBD_REQ_Exported_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBD_REQ_Exported_Types
  * @{
  */
/**
  * @}
  */ 



/** @defgroup USBD_REQ_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBD_REQ_Exported_Variables
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBD_REQ_Exported_FunctionsPrototype
  * @{
  */ 

USB_Device_Status  USB_Device_StdDevReq (USB_OTG_CORE_HANDLE  *pusbdev, USB_SETUP_REQEST  *req);
USB_Device_Status  USB_Device_StdItfReq (USB_OTG_CORE_HANDLE  *pusbdev, USB_SETUP_REQEST  *req);
USB_Device_Status  USB_Device_StdEPReq (USB_OTG_CORE_HANDLE  *pusbdev, USB_SETUP_REQEST  *req);
void USB_Device_ParseSetupRequest( USB_OTG_CORE_HANDLE  *pusbdev,
                                    USB_SETUP_REQEST *req);

void USB_Device_CtlError( USB_OTG_CORE_HANDLE  *pusbdev,
                            USB_SETUP_REQEST *req);

void USB_Device_GetString(uint8_t *desc, uint8_t *unicode, uint16_t *len);
/**
  * @}
  */ 

#endif /* __USB_REQUEST_H_ */

/**
  * @}
  */ 

/**
* @}
*/ 


/************************ (C) COPYRIGHT Artery Technology *****END OF FILE****/
