/******************************************************************************
	R_P5022.H / R_P5022.C
	
	�ύX����
	Ver 3.10	1999/04/14	release
	Ver 3.20	2000/01/18	�O�H�i�Q�Ή�
	Ver 3.24	2000/04/05	DR-CY-72A�Ή�
	Ver 3.24	2000/04/06	�Q��������ԃo�O�C��
	Ver 3.27	2000/08/07	�r���������쓮�Ή�
	Ver 3.27	2000/08/07	�����A�����ʐݒ�Ή�
	Ver 3.27	2000/08/08	set_acc�֐��o�O�C��
	Ver 3.27	2000/08/24	Start�֐��o�O�C��
	Ver 3.27	2000/08/24	�������̕Е����������쓮�ɑΉ�
	Ver 3.273	2000/09/28	�������̌ʐݒ�s�ɏC���i�s��΍�j
	Ver 3.273	2000/09/28	move��set_acc�֐������v�Z�Ή�
	Ver 3.273	2000/09/28	on�֐�io�擾���@�ύX
	Ver 3.273	2000/09/28	move�֐����s��busy�M��on�҂������C��
	Ver 3.274	2000/10/10	�������̌ʐݒ�ɏC��
	Ver 3.274	2000/10/10	move��set_acc�֐������v�Z�C��
	Ver 3.275	2000/10/25	R_axis_move_infinity�s��C��
	Ver 3.31b	2002/01/25	FL,FH����l���͎��Ή�
	Ver 3.33	2002/12/26	R_PCL5022_get_position�֐��o�O�t�B�N�X
	Ver 3.49	2006/12/18	R_PCL5022_move�n�֐��̖߂�l�ǉ�
	Ver 3.49	2006/12/20	R_PCL5022_initial�s��C��
	Ver 3.50	2008/04/08	R_PCL5022_on_�`�̖߂�l�R�����g�C��
	Ver 3.50	2009/02/12	DOS & LINUX �\�[�X����
******************************************************************************/

#ifndef	R_P5022_H_
#define	R_P5022_H_

int		R_PCL5022_open(AXIS* lpAxis);

// �p�����[�^�ݒ�֐�
int		R_PCL5022_set_range(AXIS* lpAxis, int nRange);
int		R_PCL5022_set_freq_min(AXIS* lpAxis, int nFreq);
int		R_PCL5022_set_freq_max1(AXIS* lpAxis, int nFreq);
int		R_PCL5022_set_freq_max2(AXIS* lpAxis, int nFreq);
int		R_PCL5022_set_acc(AXIS* lpAxis, int nTg, int nAlpha);
/*	�ǉ���������	*/
int		R_PCL5022_set_acc2(AXIS* lpAxis, int nTga, int ntgb, int nAlpha1, int nAlpha2);
/*	�ǉ������܂�	2000/08/07 by sugimoto */

// �\�t�g�E�F�A���~�b�g�ݒ�֐�
int		R_PCL5022_set_limit_cw(AXIS* lpAxis, int nLimit);
int		R_PCL5022_set_limit_ccw(AXIS* lpAxis, int nLimit);

// ���_���A����֐�
int		R_PCL5022_initial(AXIS* lpAxis, int mode);

int		R_PCL5022_reset(AXIS* lpAxis);
int		R_PCL5022_stop(AXIS* lpAxis);
int		R_PCL5022_down(AXIS* lpAxis);

// �ʒu���ߓ���֐�
int		R_PCL5022_move_absolute(AXIS* lpAxis, int nMode, int nPoint);
int		R_PCL5022_move_relative(AXIS* lpAxis, int nMode, int nOffset);
//int	R_PCL5022_move_increase(AXIS* lpAxis);
//int	R_PCL5022_move_decrease(AXIS* lpAxis);

// ����~���A�֐�
int		R_PCL5022_emergency_move(AXIS* lpAxis, int nMode);
int		R_PCL5022_emergency_back(AXIS* lpAxis, int nMode);

// �����p���X���U�֐�
int		R_PCL5022_move_infinity(AXIS* lpAxis, int nMode, int nDirection);

// ���݈ʒu�J�E���^�֐�
int		R_PCL5022_get_position(AXIS* lpAxis);
int		R_PCL5022_set_position(AXIS* lpAxis, int nCounter);
int		R_PCL5022_reset_position(AXIS* lpAxis);

// �_�E���J�E���^�֐�
int		R_PCL5022_get_down_counter(AXIS* lpAxis);

// �ėp�h�^�n�֐�
int		R_PCL5022_clr_on(AXIS* lpAxis);
int		R_PCL5022_clr_off(AXIS* lpAxis);
int		R_PCL5022_res_on(AXIS* lpAxis);
int		R_PCL5022_res_off(AXIS* lpAxis);

// �X�e�[�^�X�֐�
int		R_PCL5022_get_status(AXIS* lpAxis);
int		R_PCL5022_on_move(AXIS* lpAxis);
int		R_PCL5022_on_inp(AXIS* lpAxis);
int		R_PCL5022_on_orgi(AXIS* lpAxis);
int		R_PCL5022_on_ez(AXIS* lpAxis);
int		R_PCL5022_on_el_cw(AXIS* lpAxis);
int		R_PCL5022_on_el_ccw(AXIS* lpAxis);
int		R_PCL5022_on_alm(AXIS* lpAxis);

// �Q��������Ԋ֐�
int		R_PCL5022x2_reset(AXIS* lpAxis1, AXIS *lpAxis2);
int		R_PCL5022x2_move_absolute(AXIS* lpAxis1, AXIS* lpAxis2, int nMode, int nPoint1, int nPoint2);
int		R_PCL5022x2_move_relative(AXIS* lpAxis1, AXIS* lpAxis2, int nMode, int nOffset1, int nOffset2);
int		R_PCL5022x2_emergency_back(AXIS* lpAxis1, AXIS* lpAxis2, int nMode);
int		R_PCL5022x2_emergency_move(AXIS* lpAxis1, AXIS* lpAxis2, int nMode);

#endif	// R_P5022_H_
