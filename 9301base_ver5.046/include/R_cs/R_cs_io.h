/******************************************************************************

	I/O�`�F�b�N�֐��w�b�_
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
				2004.06.11.002		�o�C�i���̃X�e�[�^�X�\���s��C��
				2005.xx.xx.003		VESA�n�ڐA							by matsu
				2005.10.12.004		IN-CY-84�Ή�(DLIO�g�p)				by matsu
				2005.10.17.005		DLIO�p�Ɋ�o�^�֐��ǉ�			by matsu
				2020.07.06.006		R_cs/R_cs_io�Ƃ̋����p�~�A���S����

******************************************************************************/

// --- define ---
#define BaseIOCheck		IOCheck

#define	R_CS_IOCheck()	R_IOCheck()
#define	R_CS_IOCheckRegisterBoard(nId,nHandle,name)	R_IOCheckRegisterBoard(nId,nHandle,name)
#define	R_CS_IOCheck39()	R_CS_IOCheck39()

#include "R_iochk.h"
