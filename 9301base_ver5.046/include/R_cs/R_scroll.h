/******************************************************************************

	R_scroll.h
	
	�X�N���[�������\�[�X

									Authered. by	Kazuya Matsunaga
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	����
	<version>	<date>		<name>		<comment>
	0.01		2008.03.xx	matsu		�V�K�쐬
	1.07b1.9	2009.07.15	matsu		�e��֐��v���g�^�C�v�錾�ǉ�

******************************************************************************/

#ifndef __R_CS_SCROLL_H__
#define __R_CS_SCROLL_H__


// --- program version ---

// --- include header file ---

// TSC-9301 library


// --- define ---

// --- variable ---

// --- prototype ---
int r_scroll_open(R_PLANE *pSrcImage, int nWindowX, int nWindowY);
int r_scroll_close(int nHandle);

int r_scroll_set_offset(int nHandle, int nOffsetX, int nOffsetY);
int r_scroll_get_offset(int nHandle, int *pOffsetX, int *pOffsetY);
int r_scroll_set_scale(int nHandle, float fScale);
int r_scroll_get_scale(int nHandle, float *pScale);
int r_scroll_get_destination(int nHandle, R_PLANE *pDstPlane);
int r_scroll_execute(int nHandle);
int r_scroll_execute2(int nHandle, int nOffsetX, int nOffsetY, float fScale, R_PLANE *pDstPlane);

int r_scroll_menu(R_PLANE *pSrcPlane, int nMode, int *pOffsetX, int *pOffsetY, float *pScale, R_PLANE *pDstPlane);
int r_scroll_menu_disp(int nHandle, int mode);			//<1.07b1.9> �v���g�^�C�v�ǉ�  2009.07.15 matsu

#endif
