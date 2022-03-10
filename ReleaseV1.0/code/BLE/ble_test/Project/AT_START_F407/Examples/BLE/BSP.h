#ifndef _BSP_H_
#define _BSP_H_

#include <stdint.h>
#include <stdio.h>
#include "at32f4xx.h"
#include "at32_board.h"


typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;


void BSP_Init(void);

unsigned int GetSysTickCount(void);
void IrqMcuGotoSleepAndWakeup(void);

void LED_ONOFF(unsigned char OnOff);//for ui use

#endif

