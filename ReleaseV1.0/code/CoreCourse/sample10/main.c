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
#include "fft_radix2.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define ADC_BUFFER_SIZE  (TEST_LENGTH_SAMPLES/2)

/* Private variables ---------------------------------------------------------*/

__IO uint16_t ADCConvertedValue[ADC_BUFFER_SIZE]={0};
/* Private function prototypes -----------------------------------------------*/

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
  

  /* GPIOA Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA, ENABLE);
  
	/* Enable ADC1 and GPIOC clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_ADC1 | RCC_APB2PERIPH_GPIOC, ENABLE);
  
	/* TIM3 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR3, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitType GPIO_InitStructure;

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
  * @brief  Configures timer for ADC trigger.
  * @param  None
  * @retval None
  */
void timer3Init(void)
{
	TMR_TimerBaseInitType    TMR_TimeBaseStructure;

  /* Time base configuration */
  TMR_TimeBaseStructInit(&TMR_TimeBaseStructure); 
  TMR_TimeBaseStructure.TMR_Period = 15000-1;    //设置Timer的自动加载值,实现16KHz采样率    
  TMR_TimeBaseStructure.TMR_DIV = 0x0;       
  TMR_TimeBaseStructure.TMR_ClockDivision = 0x0;    
  TMR_TimeBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;  
  TMR_TimeBaseInit(TMR3, &TMR_TimeBaseStructure); 

  /* TIM3 TRGO selection */
	TMR_SelectOutputTrigger(TMR3, TMR_TRGOSource_Update);
}

/**
  * @brief  Configures ADC.
  * @param  None
  * @retval None
  */
void initADC(void)
{
	DMA_InitType   DMA_InitStructure;
	ADC_InitType   ADC_InitStructure;
	
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
	
	
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_Mode              = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanMode          = DISABLE;
	ADC_InitStructure.ADC_ContinuousMode    = DISABLE;//ENABLE;
	ADC_InitStructure.ADC_ExternalTrig      = ADC_ExternalTrig_TMR3_TRGO_ADC12; //ADC_ExternalTrig_None;
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
  * @brief  start DMA and Timer.
  * @param  None
  * @retval None
  */
void letsGo(void)
{
	/* Enable DMA1 channel1 for ADC*/
	DMA_ChannelEnable(DMA1_Channel1, ENABLE);

  /* TIM3 enable counter */
  TMR_Cmd(TMR3, ENABLE);
	
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCtrl(ADC1, ENABLE);
	//ADC_ExternalTrigConvCtrl(ADC1, ENABLE);
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
		printf("%d,",(ADCConvertedValue[i]));
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
	
	GPIO_Configuration();
	
	/* TIM3 Configuration */
	timer3Init();
	
	UART_Print_Init(921600);

	/* ADC Configuration */
	initADC();
	printf("ADC initialized ~!\n");
	//start 
	letsGo();
	
	while (1)
  {
	  while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==0);
		AT32_LEDn_Toggle(LED2); 
	  DMA_ChannelEnable(DMA1_Channel1, DISABLE);
		DMA_ClearFlag(DMA1_FLAG_TC1);
		//printADCRawData();
		fft_test();
		Delay_ms(5000); //留出些时间来手动处理数据
	  DMA_ChannelEnable(DMA1_Channel1, ENABLE);
	}	

}
