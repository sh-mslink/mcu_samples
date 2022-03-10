/**
 ******************************************************************************
 * @file        ETH/iperf/eth_config.h
 * @brief       This file contains all the functions prototypes for the ethernet
                configuration related.
 * @author      Artery Technology
 * @date        2019-01-04
 * @version     V1.1.2
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
 * <h2><center>&copy; COPYRIGHT 2019 ArteryTek</center></h2>
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EHT_CONFIG_H
#define __ETH_CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif

void System_Setup(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void Ethernet_Configuration(void);
#endif
