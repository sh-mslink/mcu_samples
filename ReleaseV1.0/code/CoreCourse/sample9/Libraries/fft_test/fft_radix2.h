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

#define TEST_LENGTH_SAMPLES 2048  //¿¼ÂÇ¸´ÊýÐÎÊ½

void fft_radix2_init(uint16_t nbits, uint8_t inverse);
void DFT_ref_inplace(float* pData);
void fft_radix2_inplace(float* pData);
void fft_radix2_permute(void);
void fft_radix2_dispose(void);

#endif // FFT_RADIX2_H
