
#include "at32f4xx.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "fft_radix2.h"
#include "at32_board.h"
#include <stdio.h>

/* -------------------------------------------------------------------
* External Input and Output buffer Declarations for FFT Bin Example
* ------------------------------------------------------------------- */
extern float32_t testInput_f32_10khz[TEST_LENGTH_SAMPLES];
static float32_t testInput[TEST_LENGTH_SAMPLES]={0};
static float32_t testOutput[TEST_LENGTH_SAMPLES/2]={0};

extern void enableSysTickInt(void);
extern u32 stopSysTick(u8 mS_or_uS);
/* ------------------------------------------------------------------
* Global variables for FFT Bin Example
* ------------------------------------------------------------------- */
uint32_t fftSize = TEST_LENGTH_SAMPLES/2;  //因为是复数操作，所以分为虚部和实部，所以实际的buffer大小是采样数据的一倍
#define N_ORDER 10  //这个数据是采样点数的以2为底的对数，我们采样1024个点，那就是2的10次方，所以如果采样点数变化，这个数据必须变化
/* ----------------------------------------------------------------------
* Max magnitude FFT Bin test
* ------------------------------------------------------------------- */
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

void check_result(float* in, float *out, u32 refIndex, char *n, float time)
{
  float maxValue;
  char s[128] = {0};
	uint32_t testIndex;
  arm_cmplx_mag_f32(in, out, fftSize);
  arm_max_f32(out, fftSize/2, &maxValue, &testIndex);
  if (testIndex != refIndex)
  {
    sprintf(s,"%s ERR(test index=%d) time:%.3f mS\n",n,testIndex,time);
  }
  else 
  {
    sprintf(s,"%s OK time:%.3f mS\n",n,time);
  }
  printf("%s",s);
}

void fft_arm_dsp(float *pData)
{
  uint32_t ifftFlag = 0;
  uint32_t doBitReverse = 1;
  /* Process the data through the CFFT/CIFFT module */
  arm_cfft_f32(&arm_cfft_sR_f32_len1024, pData, ifftFlag, doBitReverse);
}

void fft_test(void)
{
  int32_t size = TEST_LENGTH_SAMPLES * sizeof(float32_t);
  uint32_t ticks=0;
	
	fft_radix2_init(N_ORDER,0);
#if 0
  memcpy(testInput, testInput_f32_10khz, size);
	//printData(testInput,TEST_LENGTH_SAMPLES);
  enableSysTickInt();//  timer_start();
  DFT_ref_inplace(testInput);
	ticks=stopSysTick(1);//  t = timer_stop_us();
  check_result(testInput, testOutput, 256, "DFT_ref_inplace", ticks/1000.0f);
	//printData(testOutput,fftSize);
#endif
#if 0
  memcpy(testInput, testInput_f32_10khz, size);
	//printData(testInput,TEST_LENGTH_SAMPLES);
	enableSysTickInt();//  timer_start();
  fft_radix2_inplace(testInput);
	ticks=stopSysTick(1);//  t = timer_stop_us();
  check_result(testInput, testOutput, 256, "fft_radix2_inplace", ticks/1000.0f);
	//printData(testOutput,fftSize);
#endif
  fft_radix2_dispose();
	
#if 1
  memcpy(testInput, testInput_f32_10khz, size);
	enableSysTickInt();//  timer_start();
  fft_arm_dsp(testInput);
	ticks=stopSysTick(1);//  t = timer_stop_us();
  check_result(testInput, testOutput, 256, "fft_arm_dsp", ticks/1000.0f);
	printData(testOutput,fftSize);
#endif	

}

 /** \endlink */
