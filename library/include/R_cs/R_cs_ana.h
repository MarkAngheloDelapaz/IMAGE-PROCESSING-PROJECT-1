/******************************************************************************

	R_CS_ANA.H

	�摜��͊֐��w�b�_
	�o�[�W�����A�b�v���́A�K�������̒ǉ����s���A
	�O���[�v���ŕύX���e���A�i�E���X���邱�ƁI�I

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	����
	Version		2004.04.07.000		�I���W�i��
				2004.04.23.001		DrawPlane�`��֐�(R_draw�n)��g�ݍ���
				2004.10.06.002		�q�X�g�O�����\�����j���[�ǉ�
				2006.05.01.003		�摜�����f�����j���[�ǉ�
				2010.02.19			R_winb.h��include�ɒǉ�

******************************************************************************/

// --- define ---
#include "R_winb.h"			//<1.07b2.7>  2010.02.19  by mastu

// --- variable ---
extern struct winb_t R_CS_WinbAnalysisMenu;

// --- prototype ---
int R_CS_analysis_open(void);
int R_CS_analysis_close(void);
