/******************************************************************************
	R_intpo.h / R_intpo.c
	
	�ύX����
	Ver 3.50	2009/01/30	DOS & LINUX �\�[�X����
******************************************************************************/

#ifndef	R_INTPO_H_
#define	R_INTPO_H_

#ifdef __cplusplus
extern "C" {
#endif

int R_interpolation(float *xs,float *fs,float *coeff,int N,int M);

#ifdef __cplusplus
}
#endif

#endif	// R_INTPO_H_
