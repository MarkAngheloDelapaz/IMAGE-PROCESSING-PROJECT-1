/******************************************************************************
	�{�^�����j���[�v���O�������C�u�����[
	 < R_dbutto.h / R_dbutto.c >
	
	�ύX����
	3.42		2004.4.21	�x�[�X�v���O�����쐬
******************************************************************************/

#ifndef	R_DBUTTO_H_
#define	R_DBUTTO_H_

#include "R_button.h"	// struct button_t

#ifdef __cplusplus
extern "C" {
#endif

int R_draw_button_set( int color, int mode, struct button_t *p );
int R_draw_button_set_and_get( int color, int mode, struct button_t *p,int flag );
int R_draw_button_get( int color, int mode, int flag );
int R_draw_button_close( int color, int mode );

#ifdef __cplusplus
}
#endif

#endif	// R_DBUTTO_H_
