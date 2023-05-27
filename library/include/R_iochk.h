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
				2006.06.07.006		�摜��Ή�(CO-CZ-33/35/39)		by matsu
				2010.04.12.007		Linux�R���p�C�������[�j���O�΍�		by matsu
	1.07b3.0	2010.12.10.008		IN-CY-84 OC/PC I/O�ǉ�				by matsu
				2011.04.27.009		IOCheckMain()���� Intel Compiler remark����̂��߂Q�s�ɕ���	by matsu
	1.07b4.1	2012.05.10.010		Linux�}���`�v���Z�X����R_dlio_open()�̖߂�l����ɂO�Ȃ̂�DLIO�n�͎����X�L�������Ȃ��悤�ɕύX	by matsunaga
									(�V���O���v���Z�X���͏]�����l�����X�L�������܂�)
				2013.03.08.006		9301base���֐����l�[��				by tanaka
				2014.06.17.011		DR-CY-140�Ή�
	1.07b6.4	2015.04.30.012		IN-CY-120(PCI-2724C)��IO�m�F�Ή�
	1.07b7.1	2015.12.28.013		IN-CY-122�Ή�
	5.030		2020.07.01			�S�Ă�R_Draw�`�֐���R_Draw�Ƀ��l�[��
	5.031		2020.07.06			R_cs/R_cs_io�Ƃ̋����p�~�A���S����
	5.031		2020.07.06			CO-CZ-47�Ή�
******************************************************************************/

#ifndef	R_IOCHK_H_
#define	R_IOCHK_H_

#ifdef __cplusplus
extern "C" {
#endif

void R_IOCheck( void );
int R_IOCheckRegisterBoard( int nId, int nHandle, char *name );
void R_IOCheck39( void );

#ifdef __cplusplus
}
#endif

#endif	// R_IOCHK_H_
