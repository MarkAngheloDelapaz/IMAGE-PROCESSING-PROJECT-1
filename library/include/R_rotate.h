/******************************************************************************
	R_ROTATE.H

	�ύX����
	Ver 3.42	2004/04/23	�V�K�쐬
	Ver 3.45	2005/03/15	R_RotateGrayMemoryRect�s��C��
							(�E�[�A���[�̃��C������������Ă��Ȃ�����)
	Ver 3.47	2005/12/28	R_RotateGrayMemory(Rect)������
	Ver 3.50	2008/11/13	��]�p���O�̂Ƃ��̃o�O�A���`��ԃo�O�C���A������
	Ver 3.50	2009/04/02	DOS & LINUX �\�[�X����
******************************************************************************/

#ifndef	R_ROTATE_H_
#define	R_ROTATE_H_

#include "R_stdlib.h"	// R_RECT,R_POINT

#ifdef __cplusplus
extern "C" {
#endif

int	R_RotateGrayMemory(int sp, int dp, int xc, int yc, int bs, int bb);
int	R_RotateGrayMemoryRect(int sp, int dp, R_RECT area, R_POINT center, int numerator, int denominator);
int	R_rotate_gray_memory(int nSrcPage, int nDstPage, R_RECT tArea, R_POINT tCenter, double dRadian);

#ifdef __GNUC__
	void	RotateGrayMemoryRect_sub_(unsigned char* pSrcC, unsigned char* pDst, int xe, int ye, int xl, int yl, int dxx, int dyy, int ddp, int sin_th, int cos_th);
#endif

// �����֐�
void	RotateGrayMemoryRect_sub(unsigned char *pSrcC, unsigned char *pDst, int xe, int ye, int xl, int yl, int dxx, int dyy, int ddp, int sin_th, int cos_th);	// R_rot_s.asm

#ifdef __cplusplus
}
#endif

#endif	// R_ROTATE_H_
