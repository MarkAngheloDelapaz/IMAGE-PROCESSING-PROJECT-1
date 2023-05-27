/******************************************************************************
	R_ROTATE.H

	変更履歴
	Ver 3.42	2004/04/23	新規作成
	Ver 3.45	2005/03/15	R_RotateGrayMemoryRect不具合修正
							(右端、下端のラインが処理されていなかった)
	Ver 3.47	2005/12/28	R_RotateGrayMemory(Rect)高速化
	Ver 3.50	2008/11/13	回転角＝０のときのバグ、線形補間バグ修正、高速化
	Ver 3.50	2009/04/02	DOS & LINUX ソース統合
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

// 内部関数
void	RotateGrayMemoryRect_sub(unsigned char *pSrcC, unsigned char *pDst, int xe, int ye, int xl, int yl, int dxx, int dyy, int ddp, int sin_th, int cos_th);	// R_rot_s.asm

#ifdef __cplusplus
}
#endif

#endif	// R_ROTATE_H_
