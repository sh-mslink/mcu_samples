
#include "at32f4xx.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "fft_radix2.h"
#include "at32_board.h"
#include <stdio.h>

/* -------------------------------------------------------------------
* External Input and Output buffer Declarations for FFT Bin Example
* ------------------------------------------------------------------- */
extern __IO uint16_t ADCConvertedValue[];

static float32_t testInput_f32[TEST_LENGTH_SAMPLES]={0};
static float32_t testOutput[TEST_LENGTH_SAMPLES/2]={0};

extern void enableSysTickInt(void);
extern u32 stopSysTick(u8 mS_or_uS);
/* ------------------------------------------------------------------
* Global variables for FFT Bin Example
* ------------------------------------------------------------------- */
uint32_t fftSize = TEST_LENGTH_SAMPLES/2;  //因为是复数操作，所以分为虚部和实部，所以实际的buffer大小是采样数据的一倍

/* ----------------------------------------------------------------------
* Max magnitude FFT Bin test
* ------------------------------------------------------------------- */
float check_result(float* in, float *out, float time)
{
  float maxValue;
  char s[128] = {0};
	uint32_t testIndex;
  arm_cmplx_mag_f32(in, out, fftSize);
  arm_max_f32(out, fftSize/2, &maxValue, &testIndex);

  printf("max value@[%d] = %.3f, time:%.3f mS\n", testIndex, maxValue, time);
	return maxValue;
}

void printData(float *pData, uint16_t len)
{
	uint16_t i;
	//printf("======start of data=====\n,");
	printf("value\n");
	for(i=0;i<len;i++)
	{
		printf("%f\n",pData[i]);
	}
	//printf("\n======end of data=====\n");
}

void fft_arm_dsp(float *pData)
{
  uint32_t ifftFlag = 0;
  uint32_t doBitReverse = 1;
  /* Process the data through the CFFT/CIFFT module */
  //arm_cfft_f32(&arm_cfft_sR_f32_len1024, pData, ifftFlag, doBitReverse);
	arm_cfft_f32(&arm_cfft_sR_f32_len2048, pData, ifftFlag, doBitReverse);
	//arm_cfft_f32(&arm_cfft_sR_f32_len4096, pData, ifftFlag, doBitReverse);
}

void fft_test(void)
{
	uint16_t i;		
  int32_t size = TEST_LENGTH_SAMPLES * sizeof(float32_t);
  uint32_t ticks=0;
	float v=0;
	enableSysTickInt();//  timer_start();
	for(i=0;i<TEST_LENGTH_SAMPLES;i=i+2)
	{
		testInput_f32[i]=ADCConvertedValue[i];
		testInput_f32[i+1]=0;
	}
	
	//enableSysTickInt();//  timer_start();
  fft_arm_dsp(testInput_f32);
	ticks=stopSysTick(1);//  t = timer_stop_us();
	v=check_result(testInput_f32, testOutput, ticks/1000.0f);
	if(v>0)
		printData(testOutput,fftSize);

}

 /** \endlink */
