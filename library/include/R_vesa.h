/******************************************************************************
	R_VESA.H / R_VESA.C
	
	�ύX����
	Ver 3.49	2006/09/08	�b��\�[�X
	Ver 3.49	2006/11/18	DrawText�ǉ�
	Ver 3.50	2008/03/07	R_plane�w�b�_�t�@�C������
******************************************************************************/

#ifndef R_VESA_H_
#define R_VESA_H_

#include "R_plane.h"

int		R_vesa_set_vbe_mode(int nVesaMode);

extern R_PLANE*	pVesaPlane;

void	R_vesa_debug_linear_mode(void);	// for debug only

#endif	// R_VESA_H_
