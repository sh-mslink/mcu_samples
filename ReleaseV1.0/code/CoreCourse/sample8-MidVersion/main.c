/**
 **************************************************************************
 * File Name    : main.c
 * Description  : Main program body
 * Date         : 2018-01-24
 * Version      : V1.0.3
 **************************************************************************
 */

#include <math.h>
#include "at32f4xx.h"
#include "at32_board.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DAC_DHR12RD_Address      0x40007420  //右对齐寄存器地址
#define PI 3.14159266
#define TABLE_SIZE 32
#define ADC_BUFFER_SIZE 512

/* Private variables ---------------------------------------------------------*/
uint16_t gSine12bitTable[TABLE_SIZE] = {0};
uint32_t gDualSine12bit[TABLE_SIZE] = {0};
__IO uint16_t ADCConvertedValue[ADC_BUFFER_SIZE]={0};
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  initialize sine data table
  * @param  None
  * @retval None
  */
void initSineData(void)
{
	uint16_t i;

	printf("sine talbe = {\n");
	for(i=0;i<TABLE_SIZE;i++)
	{
		gSine12bitTable[i] = (UINT16)(2047*sin((2*PI/TABLE_SIZE)*i)+2047);
		printf("\t[%d]=%d\n",i,gSine12bitTable[i]);
	}
	printf("}\n");
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{   
  /* Enable peripheral clocks ------------------------------------------------*/
	
  /* ADCCLK = PCLK2/16 */
	RCC_ADCCLKConfig(RCC_APB2CLK_Div16);  
	
	/* Enable DMA1 clocks for ADC*/
	RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_DMA1, ENABLE);
  
	/* DMA2 clock enable for DAC*/
  RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_DMA2, ENABLE);

  /* GPIOA Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA, ENABLE);
  
	/* Enable ADC1 and GPIOC clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_ADC1 | RCC_APB2PERIPH_GPIOC, ENABLE);
  
	/* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_DAC, ENABLE);
  
	/* TIM2 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR2, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitType GPIO_InitStructure;

  /* Once the DAC channel is enabled, the corresponding GPIO pin is automatically 
     connected to the DAC converter. In order to avoid parasitic consumption, 
     the GPIO pin should be configured in analog */
  GPIO_InitStructure.GPIO_Pins =  GPIO_Pins_4 | GPIO_Pins_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_ANALOG;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure PC.04 (ADC Channel14) as analog input -------------------------*/
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_ANALOG;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/**
  * @brief  Config Clock Out Function.
  * @param  None
  * @retval None
  */
void MCO_Config(void)
{
  GPIO_InitType GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_8;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*PA8输出ADC CLOCK*/
  RCC_CLKOUTConfig(RCC_CLKOUT_ADC, RCC_MCOPRE_1);
}
/**
  * @brief  Configures timer for DAC trigger.
  * @param  None
  * @retval None
  */
void timer2Init(void)
{
	TMR_TimerBaseInitType    TMR_TimeBaseStructure;
	
  /* Time base configuration */
  TMR_TimeBaseStructInit(&TMR_TimeBaseStructure); 
  TMR_TimeBaseStructure.TMR_Period = 0x3A9;    //设置Timer的自动加载值,实现一个8KHz的正弦波 
  TMR_TimeBaseStructure.TMR_DIV = 0x0;       
  TMR_TimeBaseStructure.TMR_ClockDivision = 0x0;    
  TMR_TimeBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;  
  TMR_TimeBaseInit(TMR2, &TMR_TimeBaseStructure); 

  /* TIM2 TRGO selection */
  TMR_SelectOutputTrigger(TMR2, TMR_TRGOSource_Update);

}

/**
  * @brief  Configures DAC.
  * @param  None
  * @retval None
  */
void initDAC(void)
{
	DAC_InitType            DAC_InitStructure;
	
	  /* DAC channel1 Configuration */
  DAC_StructInit(&DAC_InitStructure);
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_TMR2_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  /* DAC channel2 Configuration */
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);
}
/**
  * @brief  Configures DMA for DAC output data, and ADC input data.
  * @param  None
  * @retval None
  */
void initDMA(void)
{
	DMA_InitType   DMA_InitStructure;
	uint32_t Idx = 0;  
	
	/* Fill Sine32bit table */
  for (Idx = 0; Idx < TABLE_SIZE; Idx++)
  { 
    gDualSine12bit[Idx] = (gSine12bitTable[Idx] << 16) + (gSine12bitTable[Idx]);
  }

	
	/* DMA1 channel1 configuration for ADC*/
	DMA_Reset(DMA1_Channel1);
	DMA_DefaultInitParaConfig(&DMA_InitStructure);
	DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)&ADC1->RDOR;
	DMA_InitStructure.DMA_MemoryBaseAddr        = (uint32_t)&ADCConvertedValue;
	DMA_InitStructure.DMA_Direction             = DMA_DIR_PERIPHERALSRC;
	DMA_InitStructure.DMA_BufferSize            = ADC_BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc         = DMA_PERIPHERALINC_DISABLE;
	DMA_InitStructure.DMA_MemoryInc             = DMA_MEMORYINC_ENABLE;
	DMA_InitStructure.DMA_PeripheralDataWidth   = DMA_PERIPHERALDATAWIDTH_HALFWORD;
	DMA_InitStructure.DMA_MemoryDataWidth       = DMA_MEMORYDATAWIDTH_HALFWORD;
	DMA_InitStructure.DMA_Mode                  = DMA_MODE_CIRCULAR;
	DMA_InitStructure.DMA_Priority              = DMA_PRIORITY_HIGH;
	DMA_InitStructure.DMA_MTOM                  = DMA_MEMTOMEM_DISABLE;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
  /* DMA2 channel4 configuration for DAC*/
  DMA_Reset(DMA2_Channel4);
  DMA_DefaultInitParaConfig(&DMA_InitStructure);
  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12RD_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)& gDualSine12bit;
  DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALDST;
  DMA_InitStructure.DMA_BufferSize = TABLE_SIZE;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
  DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
  DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_WORD;
  DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_WORD;
  DMA_InitStructure.DMA_Mode = DMA_MODE_CIRCULAR;
  DMA_InitStructure.DMA_Priority = DMA_PRIORITY_HIGH;
  DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
  DMA_Init(DMA2_Channel4, &DMA_InitStructure);
	
}
/**
  * @brief  Configures ADC.
  * @param  None
  * @retval None
  */
void initADC(void)
{
	ADC_InitType ADC_InitStructure;
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_Mode              = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanMode          = DISABLE;
	ADC_InitStructure.ADC_ContinuousMode    = ENABLE;
	ADC_InitStructure.ADC_ExternalTrig      = ADC_ExternalTrig_None;
	ADC_InitStructure.ADC_DataAlign         = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NumOfChannel      = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 regular channels configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_28_5);    
	
	/* Enable ADC1 DMA */
	ADC_DMACtrl(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Ctrl(ADC1, ENABLE);

	/* Enable ADC1 reset calibration register */   
	ADC_RstCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));

}
/**
  * @brief  start DMA, DAC and Timer.
  * @param  None
  * @retval None
  */
void letsGo(void)
{
	/* Enable DMA1 channel1 for ADC*/
	DMA_ChannelEnable(DMA1_Channel1, ENABLE);
	/* Enable DMA2 Channel4 for DAC*/
  DMA_ChannelEnable(DMA2_Channel4, ENABLE);

  /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is 
     automatically connected to the DAC converter. */
  DAC_Ctrl(DAC_Channel_1, ENABLE);
  /* Enable DAC Channel2: Once the DAC channel2 is enabled, PA.05 is 
     automatically connected to the DAC converter. */
  DAC_Ctrl(DAC_Channel_2, ENABLE);

  /* Enable DMA for DAC Channel2 */
  DAC_DMACtrl(DAC_Channel_2, ENABLE);

  /* TIM2 enable counter */
  TMR_Cmd(TMR2, ENABLE);
	
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCtrl(ADC1, ENABLE);
}
/**
  * @brief   print ADC Raw Data In Buffer to UART.
  * @param  None
  * @retval None
  */
void printADCRawData(void)
{
	uint32_t i;
	//printf("===Start of ADC raw data:===\n");
	for(i=0;i<ADC_BUFFER_SIZE;i++)
	{
		printf("%f,",(float)(ADCConvertedValue[i]));//*(3.3/4096)));
	}
	//printf("\n===End of ADC raw data===\n");
}
/**
  * @brief   Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	AT32_Board_Init(); 
	
	/* System Clocks Configuration */
  RCC_Configuration();   
	/* config clock output pin */
	MCO_Config();
	
	/* Once the DAC channel is enabled, the corresponding GPIO pin is automatically 
	 connected to the DAC converter. In order to avoid parasitic consumption, 
	 the GPIO pin should be configured in analog */
	GPIO_Configuration();
	/* TIM2 Configuration */
	timer2Init();
	/* DAC Configuration */
	initDAC();
	
	UART_Print_Init(115200);
	
	initSineData();
	
	/* DMA Configuration */
	initDMA();
	/* ADC Configuration */
	initADC();
	//start 
	letsGo();
	
  while (1)
  {
	  while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==0);
		AT32_LEDn_Toggle(LED2); 
	  DMA_ChannelEnable(DMA1_Channel1, DISABLE);
		DMA_ClearFlag(DMA1_FLAG_TC1);
		printADCRawData();
		Delay_ms(5000);
	  DMA_ChannelEnable(DMA1_Channel1, ENABLE);
	}	

}
