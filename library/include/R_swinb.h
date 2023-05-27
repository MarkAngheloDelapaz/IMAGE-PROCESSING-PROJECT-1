/******************************************************************************
	R_SWINB.H / R_SWINB.C
	
	�ύX����
		Ver 3.42	2004.02.26	�����[�X							by matsu
******************************************************************************/

#ifndef	R_SWINB_H_
#define	R_SWINB_H_

#include "R_winb.h"

#ifdef __cplusplus
extern "C" {
#endif

// �p�����[�^�ύX�֐�
int	R_SVGA_winb_parameter( struct winb_para_t [], int, char *, char * );
int	R_SVGA_winb_parameter_with_mess( struct winb_para_tm [], char *, char * );

// �|�b�v�A�b�v������������
int R_SVGA_winb_menu( int, int, int, struct winb_t * );

#ifdef __cplusplus
}
#endif

#endif	// R_SWINB_H_
