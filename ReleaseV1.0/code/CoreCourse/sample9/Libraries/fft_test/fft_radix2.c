/************************************************************************/
// FILE NAME :		fft.cpp
// VERSION :			1.0
// DATE :			2011. 03. 22
// COMPANY :		thorlab
// DESCRIPTION :   
//		v 0.1 : 2011.03.22 start project
//		v 0.2 : 2011.04.01 reference code and radix2(decimation-in-frequency)
//		v 0.3 : 2011.04.05 scott radix2(decimation-in-time)
//		v 1.0 : 2011.04.25 split radix(decimation-in-frequency)
// Author :		eason dai
//                                                                 
/************************************************************************/

#include "at32f4xx.h"
#include <stdint.h>
#include <string.h>
//#include <stdlib.h>
#include <math.h>
#include "fft_radix2.h"

#define M_PI 3.14159265358979323846f
#define M_SQRT1_2  0.707106781186547524401f
//#define fft_size 1024
#define fft_size TEST_LENGTH_SAMPLES/2

//构建一个结构体来描述复数
typedef struct {
	float  re;
	float  im;
} cmplx_32fc;

//构建一个结构体来管理蝶形运算
typedef struct tag_radix2_msg
{
	uint32_t n_sample;
	uint16_t n_order;
	uint8_t inv;
	uint8_t en_radix2;
	cmplx_32fc exptab[fft_size];
	cmplx_32fc exptab2[fft_size];
	uint32_t pRedirect[fft_size];
}radix2_msg;

//构建一个宏来完成实部乘法运算
#define MUL(c,a,b) {\
    (c)=(a)*(b);\
}

//构建一个宏来完成实部蝶形运算
#define BF(x,y,a,b) {\
    (y) = (a) - (b);\
    (x) = (a) + (b);\
}
//构建一个宏来完成替换操作
#define BF2(a,b) {\
    t0 = (a) - (b); (a) += (b); (b)=t0; \
}
//构建一个宏来完成一个复数的蝶形运算，并替换原始复数
#define ACMUL(pre, pim, are, aim, bre, bim) \
{\
	pre += ((are*bre) - (aim*bim));\
	pim += ((are*bim) + (bre*aim));\
}
//构建一个宏来完成一个复数的蝶形运算
#define CMUL(pre, pim, are, aim, bre, bim) \
{\
	pre = ((are*bre) - (aim*bim));\
	pim = ((are*bim) + (bre*aim));\
}

static radix2_msg local_msg;

void fft_radix2_init(uint16_t nbits, uint8_t en)
{
	float alpha, c1, s1;
	local_msg.n_order = nbits;
	local_msg.n_sample = 1 << nbits;
	
//	local_msg.exptab = (cmplx_32fc*)malloc(local_msg.n_sample *sizeof(cmplx_32fc));
//	local_msg.pRedirect = (uint32_t*)malloc(local_msg.n_sample *sizeof(uint32_t));

	{ // build array of e^-j2πkn/N
		int32_t m = local_msg.n_sample >>2;
		for (int32_t i=0;i < m; i++)
		{
			alpha = 2.0f * M_PI * i / local_msg.n_sample;
			c1 = cosf(alpha);
			s1 = sinf(alpha);
			//if (!inverse)	s1 = -s1;
			local_msg.exptab[i].re = c1;
			local_msg.exptab[i].im = -s1;
			
			local_msg.exptab[i+m].re = -s1;
			local_msg.exptab[i+m].im = -c1;
			
			local_msg.exptab[i+2*m].re = -c1;
			local_msg.exptab[i+2*m].im = s1;
			
			local_msg.exptab[i+3*m].re = s1;
			local_msg.exptab[i+3*m].im = c1;
		}
	}

	fft_radix2_permute();

}

void fft_radix2_inplace(float* pData)
{
	double AR,AI,TR,TI;
	uint32_t fftLen = local_msg.n_sample, loop = 0;
	uint32_t i,x,k,n2 = local_msg.n_sample * sizeof(cmplx_32fc);
	cmplx_32fc* src = (cmplx_32fc*)pData;

	cmplx_32fc* temp = local_msg.exptab2;
	for (x = 0; x < local_msg.n_sample; x++){
		i = local_msg.pRedirect[x]>>1;
		temp[x].re = src[i].re;
		temp[x].im = src[i].im;
	}
	memcpy(pData, temp, n2);

	uint32_t BlockEnd = 1;
	x = fftLen >>1;
	for (uint32_t BlockSize = 2; BlockSize <= fftLen; BlockSize <<= 1)
	{
		for (uint32_t i = 0; i < fftLen; i += BlockSize)
		{
			loop = 0;
			for (uint32_t j = i; j < i + BlockEnd; j++){
				k = j + BlockEnd;
				AR = local_msg.exptab[loop].re;
				AI = local_msg.exptab[loop].im;
				loop += x;
				//TR = AR*pReal[k] + AI*pImag[k];
				//TI = AR*pImag[k] - AI*pReal[k]; //(tr,ti)=x(n+N/2)*exp(-jkw)
				TR = AR * src[k].re - AI * src[k].im;
				TI = AR * src[k].im + AI * src[k].re;

				src[k].re = (src[j].re - (float)TR);
				src[j].re += (float)(TR);
				src[k].im = (src[j].im - (float)TI);
				src[j].im += (float)(TI);
			}
		}
		x >>=1;
		BlockEnd = BlockSize;
	}
}

void fft_radix2_permute(void)
{
	int32_t x,i=0, ri, j;
	do {
		  j = 0;
		  ri = i;
		  x = local_msg.n_order; 
		  do {
				j = (j << 1) | (ri&0x01);
				ri >>=1;
		  } while (--x);
		  local_msg.pRedirect[j] = (short)i<<1;
	} while (++i < (int32_t)local_msg.n_sample);
}

//X(e^jω)= ∑x(n) e^j-ωn
void DFT_ref_inplace(float *pData)
{
	int32_t i, j, k, n2 = local_msg.n_sample * sizeof(cmplx_32fc);
	double tmp_re, tmp_im, s, c;
	cmplx_32fc* q;
	int32_t n_sample= local_msg.n_sample;

	cmplx_32fc *temp = local_msg.exptab2;
	//n2 = n_sample >> 1;
	for (i = 0; i < n_sample; i++) 
	{
		tmp_re = 0;
		tmp_im = 0;
		q = (cmplx_32fc*)pData;
		for (j = 0; j < n_sample; j++) 
		{
			k = (i * j) & (n_sample - 1);
			c = local_msg.exptab[k].re;
			s = local_msg.exptab[k].im;
			ACMUL(tmp_re, tmp_im, c, s, q->re, q->im);
			q++;
		}
		temp[i].re = (float)tmp_re;
		temp[i].im = (float)tmp_im;
	}
	memcpy(pData, temp, n2);
}

void fft_radix2_dispose(void)
{
//	free(local_msg.exptab);
//	free(local_msg.exptab2);
//	free(local_msg.pRedirect);
}
