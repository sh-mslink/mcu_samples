/**
  ******************************************************************************
  * @file    usbh_ioreq.h
  * @author  Artery Technology
  * @version V1.0.4
  * @date    2019-05-27
  * @brief   Header file for usbh_ioreq.c
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

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_IOREQ_H
#define __USBH_IOREQ_H

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"
#include "usbh_core.h"
#include "usbh_def.h"

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_LIB_CORE
* @{
*/
  
/** @defgroup USBH_IOREQ
  * @brief This file is the header file for usbh_ioreq.c
  * @{
  */ 


/** @defgroup USBH_IOREQ_Exported_Defines
  * @{
  */
#define USB_HOST_SETUP_PKT_SIZE   8
#define USB_HOST_EPT0_EP_NUM       0
#define USB_HOST_MAX_PACKET_SIZE  0x40
/**
  * @}
  */ 


/** @defgroup USBH_IOREQ_Exported_Types
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBH_IOREQ_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_IOREQ_Exported_Variables
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_IOREQ_Exported_FunctionsPrototype
  * @{
  */
USB_HOST_Status USB_HOST_CtrlSendSetup ( USB_OTG_CORE_HANDLE *pusbdev, 
                                uint8_t *buff, 
                                uint8_t hc_num);

USB_HOST_Status USB_HOST_CtrlSendData ( USB_OTG_CORE_HANDLE *pusbdev, 
                                uint8_t *buff, 
                                uint16_t length,
                                uint8_t hc_num);

USB_HOST_Status USB_HOST_CtrlReceiveData( USB_OTG_CORE_HANDLE *pusbdev, 
                                uint8_t *buff, 
                                uint16_t length,
                                uint8_t hc_num);

USB_HOST_Status USB_HOST_BulkReceiveData( USB_OTG_CORE_HANDLE *pusbdev, 
                                uint8_t *buff, 
                                uint16_t length,
                                uint8_t hc_num);

USB_HOST_Status USB_HOST_BulkSendData ( USB_OTG_CORE_HANDLE *pusbdev, 
                                uint8_t *buff, 
                                uint16_t length,
                                uint8_t hc_num);

USB_HOST_Status USB_HOST_InterruptReceiveData( USB_OTG_CORE_HANDLE *pusbdev, 
                                       uint8_t             *buff, 
                                       uint8_t             length,
                                       uint8_t             hc_num);

USB_HOST_Status USB_HOST_InterruptSendData( USB_OTG_CORE_HANDLE *pusbdev, 
                                    uint8_t *buff, 
                                    uint8_t length,
                                    uint8_t hc_num);

USB_HOST_Status USB_HOST_CtrlReq (USB_OTG_CORE_HANDLE *pusbdev,
                         USB_HOST *phost, 
                         uint8_t             *buff,
                         uint16_t            length);

USB_HOST_Status USB_HOST_IsocReceiveData( USB_OTG_CORE_HANDLE *pusbdev, 
                                uint8_t *buff, 
                                uint32_t length,
                                uint8_t hc_num);


USB_HOST_Status USB_HOST_IsocSendData( USB_OTG_CORE_HANDLE *pusbdev, 
                                uint8_t *buff, 
                                uint32_t length,
                                uint8_t hc_num);
/**
  * @}
  */ 

#endif /* __USBH_IOREQ_H */

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


