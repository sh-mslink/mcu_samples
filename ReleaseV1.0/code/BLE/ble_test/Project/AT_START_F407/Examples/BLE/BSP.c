#include "BSP.h"
#include "mg_api.h"


/*
GPIOE14 - IRQ 
GPIOB12 - SPI1_CSN
GPIOB13 - SPI1_SCK
GPIOB15 - SPI1_MOSI
GPIOB14 - SPI1_MISO

GPIOD5 - UART2_TX
GPIOD6 - UART2_RX

GPIOD14 - LED3
*/

SPI_InitType   SPI_InitStructure;
USART_InitType USART_InitStructure;
GPIO_InitType GPIO_InitStructure;
EXTI_InitType EXTI_InitStructure;
NVIC_InitType NVIC_InitStructure = {0};

__IO uint32_t LsiFreq = 40000;

void SPI_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_13 | GPIO_Pins_15;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOB,GPIO_Pins_12);
    
    SPI_DefaultInitParaConfig(&SPI_InitStructure);
    SPI_InitStructure.SPI_TransMode = SPI_TRANSMODE_FULLDUPLEX;
    SPI_InitStructure.SPI_Mode = SPI_MODE_MASTER;
    SPI_InitStructure.SPI_FrameSize = SPI_FRAMESIZE_8BIT;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_LOW;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1EDGE;
    SPI_InitStructure.SPI_NSSSEL = SPI_NSSSEL_SOFT;
    SPI_InitStructure.SPI_MCLKP = SPI_MCLKP_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
    SPI_InitStructure.SPI_CPOLY = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
    
    SPI_Enable(SPI2, ENABLE);
}

void Uart_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOD | RCC_APB2PERIPH_AFIO, ENABLE);
    
    /* Enable USART2 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_USART2, ENABLE);
    
    /* Enable the USART2 Pins Software Remapping */
    GPIO_PinsRemapConfig(GPIO_Remap_USART2, ENABLE);  
    
    
    /* Configure USART2 Rx as input floating */
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
        
    /* Configure USART2 Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_5;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    /* Configure USART2 */
    USART_Init(USART2, &USART_InitStructure);
    
    USART_INTConfig(USART2, USART_INT_RDNE, ENABLE);
    USART_INTConfig(USART2, USART_INT_TDE, ENABLE);
    
    USART_Cmd(USART2, ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void BSP_Init(void)
{
    AT32_Board_Init();
    
    SPI_Config();
    Uart_Init();
    
    SysTick_Config(SystemCoreClock / 1000);//1ms
    
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOE, ENABLE);        ///<Enable GPIOA clock
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_PU;
    GPIO_Init(GPIOE, &GPIO_InitStructure);                       ///<Configure PA.00 pin as input floating
	
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO, ENABLE);         ///<Enable AFIO clock
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinsSource14); ///<Connect EXTI0 Line to PA0 pin
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineEnable = ENABLE;
	EXTI_Init(&EXTI_InitStructure);                              ///<Configure EXTI0 line
    
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
   
    IWDG_KeyRegWrite(IWDG_KeyRegWrite_Enable);
    IWDG_SetPrescaler(IWDG_Psc_32);
    IWDG_SetReload(LsiFreq / 32);//1S
    IWDG_ReloadCounter();
//    IWDG_Enable();
}

void LED_ONOFF(unsigned char OnOff)
{
    if(OnOff)
        AT32_LEDn_ON(LED3);
    else
        AT32_LEDn_OFF(LED3);
}

unsigned char SPI_WriteRead(unsigned char SendData,unsigned char WriteFlag) 
{
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TE) == RESET);
    SPI_I2S_TxData(SPI2, SendData);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RNE) == RESET);
    return SPI_I2S_RxData(SPI2);
}

void SPI_CS_Enable_(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pins_12);
}

void SPI_CS_Disable_(void)
{
    GPIO_SetBits(GPIOB,GPIO_Pins_12);
}

unsigned char SPI_WriteBuf(unsigned char reg, unsigned char const *pBuf, unsigned char len)
{
    unsigned char result = 0;
    unsigned char i;
    
    SPI_CS_Enable_();
    
    SPI_WriteRead(reg, 1);
    
    for (i=0;i<len;i++)
    {
        SPI_WriteRead(*pBuf++, 1);
    }
    
    SPI_CS_Disable_();
    
    return result;
}

unsigned char SPI_ReadBuf(unsigned char reg, unsigned char *pBuf, unsigned char len)
{
    unsigned char result = 0;
    unsigned char i;
    
    SPI_CS_Enable_();
    
    result = SPI_WriteRead(reg, 1);
    
    for (i=0;i<len;i++)
    {
        *(pBuf+i) = SPI_WriteRead(0xff, 0);
    }
    
    SPI_CS_Disable_();
    
    return result;
}

char IsIrqEnabled(void) //porting api
{
    return (!(GPIO_ReadInputDataBit(GPIOE,GPIO_Pins_14)));
}

void IrqMcuGotoSleepAndWakeup(void) // auto goto sleep AND wakeup, porting api
{
}


void McuGotoSleepAndWakeup(void)
{
}
void SysClk8to48(void)
{
}
void SysClk48to8(void)
{
}
