/**
  ******************************************************************************
  * @file    usb_dcd.h
  * @author  Artery Technology
  * @version V1.0.4
  * @date    2019-05-27
  * @brief   Peripheral Driver Header file
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
#ifndef __DCD_H__
#define __DCD_H__

/* Includes ------------------------------------------------------------------*/
#include "usb_core.h"


/** @addtogroup USB_OTG_DRIVER
* @{
*/

/** @defgroup USB_DCD
* @brief This file is the 
* @{
*/ 


/** @defgroup USB_DCD_Exported_Defines
* @{
*/ 
#define USB_OTG_EPT_CONTROL                       0
#define USB_OTG_EPT_ISOC                          1
#define USB_OTG_EPT_BULK                          2
#define USB_OTG_EPT_INT                           3
#define USB_OTG_EPT_MASK                          3

/*  Device Status */
#define USB_OTG_DEFAULT                          1
#define USB_OTG_ADDRESSED                        2
#define USB_OTG_CONFIGURED                       3
#define USB_OTG_SUSPENDED                        4

/**
* @}
*/ 


/** @defgroup USB_DCD_Exported_Types
* @{
*/ 
/********************************************************************************
Data structure ept_type
********************************************************************************/
typedef struct
{
  uint8_t  bLength;
  uint8_t  bDescriptorType;
  uint8_t  bEndpointAddress;
  uint8_t  bmAttributes;
  uint16_t wMaxPacketSize;
  uint8_t  bInterval;
}
EPT_DESCRIPTOR , *PEPT_DESCRIPTOR;

/**
* @}
*/ 


/** @defgroup USB_DCD_Exported_Macros
* @{
*/ 
/**
* @}
*/ 

/** @defgroup USB_DCD_Exported_Variables
* @{
*/ 
/**
* @}
*/ 

/** @defgroup USB_DCD_Exported_FunctionsPrototype
* @{
*/ 
/********************************************************************************
EXPORTED FUNCTION FROM THE USB-OTG LAYER
********************************************************************************/
void       USB_DCD_Init(USB_OTG_CORE_HANDLE *pusbdev ,
                    USB_OTG_CORE_ID_TypeDef coreid);

void        USB_DCD_DevConnect (USB_OTG_CORE_HANDLE *pusbdev);
void        USB_DCD_DevDisconnect (USB_OTG_CORE_HANDLE *pusbdev);
void        USB_DCD_EPT_SetAddress (USB_OTG_CORE_HANDLE *pusbdev,
                               uint8_t address);
uint32_t    USB_DCD_EPT_Open(USB_OTG_CORE_HANDLE *pusbdev , 
                     uint8_t ep_addr,
                     uint16_t ep_mps,
                     uint8_t ept_type);

uint32_t    USB_DCD_EPT_Close  (USB_OTG_CORE_HANDLE *pusbdev,
                                uint8_t  ep_addr);


uint32_t   USB_DCD_EPT_PrepareRx ( USB_OTG_CORE_HANDLE *pusbdev,
                        uint8_t   ep_addr,                                  
                        uint8_t *pbuf,                                  
                        uint16_t  buf_len);
  
uint32_t    USB_DCD_EPT_Tx (USB_OTG_CORE_HANDLE *pusbdev,
                               uint8_t  ep_addr,
                               uint8_t  *pbuf,
                               uint32_t   buf_len);
uint32_t    USB_DCD_EPT_Stall (USB_OTG_CORE_HANDLE *pusbdev,
                              uint8_t   bepnum);
uint32_t    USB_DCD_EPT_ClrStall (USB_OTG_CORE_HANDLE *pusbdev,
                                  uint8_t bepnum);
uint32_t    USB_DCD_EPT_Flush (USB_OTG_CORE_HANDLE *pusbdev,
                               uint8_t bepnum);
uint32_t    USB_DCD_Handle_ISTS(USB_OTG_CORE_HANDLE *pusbdev);

uint32_t USB_DCD_GetEPTStatus(USB_OTG_CORE_HANDLE *pusbdev ,
                         uint8_t bepnum);

void USB_DCD_SetEPTStatus (USB_OTG_CORE_HANDLE *pusbdev , 
                      uint8_t bepnum , 
                      uint32_t Status);

/**
* @}
*/ 


#endif /* __DCD_H__ */


/**
* @}
*/ 

/**
* @}
*/ 
/************************ (C) COPYRIGHT Artery Technology *****END OF FILE****/

