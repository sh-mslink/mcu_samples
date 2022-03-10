/************************************************************************/
// FILE NAME :		fft.h
// VERSION :			1.0
// DATE :			2011. 03. 22
// COMPANY :		thorlab
// DESCRIPTION :   see .cpp file 
// Author :		eason dai
//                                                                 
/************************************************************************/

#ifndef FFT_RADIX2_H
#define FFT_RADIX2_H

//#define TEST_LENGTH_SAMPLES 2048  //1024���㣬������Ҫ���Ǹ�����ʽ
#define TEST_LENGTH_SAMPLES 4096  //2048���㣬������Ҫ���Ǹ�����ʽ
//#define TEST_LENGTH_SAMPLES 8192  //4096���㣬������Ҫ���Ǹ�����ʽ
extern void enableSysTickInt(void);
extern u32 stopSysTick(u8 mS_or_uS);

void fft_test(void);

#endif // FFT_RADIX2_H
