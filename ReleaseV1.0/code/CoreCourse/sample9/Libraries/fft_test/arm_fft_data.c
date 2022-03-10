
/* ----------------------------------------------------------------------
Test Input signal contains 10KHz signal + Uniformly distributed white noise
** ------------------------------------------------------------------- */
#include "at32f4xx.h"
#include "arm_math.h"
#include "fft_radix2.h"

float32_t testInput_f32_10khz[TEST_LENGTH_SAMPLES] = {0};

#if 1
void genTestData(void)
{
	uint16_t i;

	//printf("sine talbe = {\n");
	for(i=0;i<TEST_LENGTH_SAMPLES;i+=2)
	{
		//testInput_f32_10khz[i] = (float32_t)(3.0*sinf((PI/4)*i)+3.0); //�ٶ�ÿ�������ܲ���8����
		testInput_f32_10khz[i] = (float32_t)(3.0*sinf((PI/4)*i))+(float32_t)(cosf((PI/3+0.0)*i)); //�ٶ�ÿ�������ܲ���8����,������һ��Ƶ�ʱ�ԭ�������źŸ�һ���Ĳ���
		testInput_f32_10khz[i+1] = (float32_t)(0.0);
		//printf("\t[%d]=%d\n",i,testInput_f32_10khz[i]);
	}
	//printf("}\n");
}
#else
void genTestData(void)
{
	uint16_t i;

	//printf("sine talbe = {\n");
	for(i=0;i<TEST_LENGTH_SAMPLES;i+=2)
	{
		testInput_f32_10khz[i] = 0;
		testInput_f32_10khz[i+1] = (float32_t)(0.0);
		//printf("\t[%d]=%d\n",i,testInput_f32_10khz[i]);
	}
	testInput_f32_10khz[100] = 3.0;
	testInput_f32_10khz[101] = 3.0;
	testInput_f32_10khz[102] = 3.0;
	testInput_f32_10khz[103] = 3.0;
	//testInput_f32_10khz[104] = 3.0;
	//testInput_f32_10khz[105] = 3.0;
	//testInput_f32_10khz[106] = 3.0;
	//testInput_f32_10khz[107] = 3.0;
	//testInput_f32_10khz[108] = 3.0;
	//testInput_f32_10khz[109] = 3.0;
	//testInput_f32_10khz[110] = 3.0;
	//testInput_f32_10khz[111] = 3.0;
	//printf("}\n");
}
#endif