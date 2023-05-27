/******************************************************************************
	�c�q�|�b�x�|�T�V
	�S���p���X�W�F�l���[�^�@�w�R�R�O�P���S����
	
	compiler : watcom C/C++32 compiler
		wcc386p /W4 /oneatx /zp4 /4r r_drcy57.c
		WLIB /l=9301base.lst 9301BASE.lib-+r_drcy57.obj
	
	Ver 1.00				���������[�X
	Ver 1.01				�w�x��Ԉړ��֐��ǉ��iR_linear_�`�j
	Ver 1.02				�A�҃p���X�̂Q���{�A�S���{�A�Q�N���b�N�Ή��iR_open_drcy57�g���j
							�ėp�o�͊֐��ǉ��iR_X3301_clr_on/off/oneshot�AR_X3301_res_on/off�j
	Ver 3.10				DR-CY-72�Ή�
	Ver 3.50	2009/02/12	DOS & LINUX �\�[�X����
******************************************************************************/

#ifndef	R_DRCY57_H_
#define	R_DRCY57_H_

#include "R_axis.h"

//	port

#define	DRCY57_channel_0	0x00000
#define	DRCY57_channel_1	0x01000
#define	DRCY57_channel_2	0x02000
#define	DRCY57_channel_3	0x03000
#define	DRCY57_channel_4	0x04000
#define	DRCY57_channel_5	0x05000
#define	DRCY57_channel_6	0x06000
#define	DRCY57_channel_7	0x07000
#define	DRCY57_channel_8	0x08000
#define	DRCY57_channel_9	0x09000
#define	DRCY57_channel_A	0x0A000
#define	DRCY57_channel_B	0x0B000
#define	DRCY57_channel_C	0x0C000
#define	DRCY57_channel_D	0x0D000
#define	DRCY57_channel_E	0x0E000
#define	DRCY57_channel_F	0x0F000

#define	DRCY57_device_1		0x0104
#define	DRCY57_device_2		0x0504
#define	DRCY57_device_3		0x0904
#define	DRCY57_device_4		0x0d04




// x3301_open
#define	ALLOCATED			(-1)		// ���ɂw�R�R�O�P�\���̂��A�̈�m�ۂ���Ă���B
#define	X3301_ERR			(-2)		// �c�q�|�b�x�|�T�V�����݂��Ȃ��B
#define	ALLOC_ERR			(-3)		// �̈�m�ۂł��Ȃ��B

#define	WITHOUT_FEEDBACK	0x00000000	// �A�҃p���X�𗘗p���Ȃ��B
#define	WITH_FEEDBACK		0x00000001	// �A�҃p���X�𗘗p����B
#define	PULSE_2CLOCK		0x00000000	// �p���X���U���Q�N���b�N�����ōs���B
#define	PULSE_GATE			0x00000010	// �p���X���U���Q�[�g�����ōs���B
#define	ACTIVE_LOW			0x00000000	// ���_�Z���T���_��
#define	ACTIVE_HIGH			0x00000100	// ���_�Z���T���_��

#define	PHASEX1				0x0000		// �Q���N���b�N�P���{�i�`���A�a���j
#define	PHASEX2				0x1000		// �Q���N���b�N�Q���{�i�`���A�a���j
#define	PHASEX4				0x2000		// �Q���N���b�N�S���{�i�`���A�a���j
#define	BICLOCK				0x3000		// �Q�N���b�N���_���i�b�v�A�b�b�v�j

#define	DIR_CW				0x00000000	// �{�������U�łb�v
#define	DIR_CCW				0x00010000	// �{�������U�łb�b�v

// x3301_initial
#define	X3301_CW			0x00000000
#define	X3301_CCW			0x00000001
#define	X3301_DIR			0x00000001
#define	INIT_ORGI			0x00000000	// �n�q�f�h�̂n�m�Œ�~�B
#define	INIT_EZ				0x00000010	// �n�q�f�h���n�m������̂d�y�̂n�m�Œ�~�B
#define	INIT_MODE			0x000000f0

#define	RANGE_DEFAULT		15			// ���g���{���f�t�H���g�l
#define	FL_DEFAULT			1000		// 
#define	FH1_DEFAULT			30000		// 
#define	FH2_DEFAULT			10000		// 
#define	ACC_DEFAULT			10			// ���������ԃf�t�H���g�l
#define	ALPHA_DEFAULT		100			// �r���������̈�f�t�H���g�l

// move mode
#define	FH1_SSSD			0x15		// �e�g�P�����X�^�[�g
#define	FH2_SSSD			0x17		// �e�g�Q�����X�^�[�g
#define	FL_CONST			0x10		// �e�k�葬�X�^�[�g
#define	FH1_CONST			0x11		// �e�g�P�葬�X�^�[�g
#define	FH2_CONST			0x13		// �e�g�Q�葬�X�^�[�g
#define	MODE_MASK			0xff		// �X�^�[�g���[�h�̃}�X�N

#define	WITH_LIMMIT_CHECK	(int)0x0000	// ���~�b�g�`�F�b�N���s��
#define	WITH_LIMIT_CHECK	(int)0x0000	// ���~�b�g�`�F�b�N���s��
#define	WITHOUT_LIMIT_CHECK	(int)0x0100	// ���~�b�g�`�F�b�N���s��Ȃ�




//	�w�R�R�O�P�T�[�{�\����
typedef	AXIS	X3301;

#define	R_open_drcy57(xpp, nChannel, nMode)		R_axis_open(xpp, nChannel, nMode)
#define	R_close_drcy57(lpAxis)					R_axis_close(lpAxis)

#define	R_X3301_set_range(lpAxis, nRange)		R_axis_set_range(lpAxis, nRange)
#define	R_X3301_set_freq_min(lpAxis, nFreq)		R_axis_set_freq_min(lpAxis, nFreq)
#define	R_X3301_set_freq_max1(lpAxis, nFreq)	R_axis_set_freq_max1(lpAxis, nFreq)
#define	R_X3301_set_freq_max2(lpAxis, nFreq)	R_axis_set_freq_max2(lpAxis, nFreq)
#define	R_X3301_set_acc(lpAxis, nTg, nAlpha)	R_axis_set_acc(lpAxis, nTg, nAlpha)
#define	R_X3301_set_limit_cw(lpAxis, nLimit)	R_axis_set_limit_cw(lpAxis, nLimit)
#define	R_X3301_set_limit_ccw(lpAxis, nLimit)	R_axis_set_limit_ccw(lpAxis, nLimit)

#define	R_X3301_initial(lpAxis, nMode)			R_axis_initial(lpAxis, nMode)

#define	R_X3301_reset(lpAxis)					R_axis_reset(lpAxis)
#define	R_X3301_stop(lpAxis)					R_axis_stop(lpAxis)
#define	R_X3301_down(lpAxis)					R_axis_down(lpAxis)

// �ʒu���ߓ���֐�
#define	R_X3301_move_absolute(lpAxis, nMode, nPosition)	R_axis_move_absolute(lpAxis, nMode, nPosition)
#define	R_X3301_move_relative(lpAxis, nMode, nOffset)	R_axis_move_relative(lpAxis, nMode, nOffset)
#define	R_X3301_move_increase(lpAxis)					R_axis_move_increase(lpAxis)
#define	R_X3301_move_decrease(lpAxis)					R_axis_move_decrease(lpAxis)

#define	R_X3301_emergency_move(lpAxis, nMode)			R_axis_emergency_move(lpAxis, nMode)
#define	R_X3301_emergency_back(lpAxis, nMode)			R_axis_emergency_back(lpAxis, nMode)

// �����p���X���U�֐�
#define	R_X3301_move_infinity(lpAxis, nMode, nDir)		R_axis_move_infinity(lpAxis, nMode, nDir)

// ���݈ʒu�J�E���^�֐�
#define	R_X3301_get_position(lpAxis)					R_axis_get_position(lpAxis)
#define	R_X3301_set_position(lpAxis, nPosition)			R_axis_set_position(lpAxis, nPosition)
#define	R_X3301_reset_position(lpAxis)					R_axis_reset_position(lpAxis)

#define	R_X3301_get_down_counter(lpAxis)				R_axis_get_down_counter(lpAxis)

// �X�e�[�^�X�֐�
#define	R_X3301_get_status(lpAxis)						R_axis_get_status(lpAxis)
#define	R_X3301_on_move(lpAxis)							R_axis_on_move(lpAxis)
#define	R_X3301_on_inp(lpAxis)							R_axis_on_inp(lpAxis)
#define	R_X3301_on_orgi(lpAxis)							R_axis_on_orgi(lpAxis)
#define	R_X3301_on_ez(lpAxis)							R_axis_on_ez(lpAxis)
#define	R_X3301_on_el_cw(lpAxis)						R_axis_on_el_cw(lpAxis)
#define	R_X3301_on_el_ccw(lpAxis)						R_axis_on_el_ccw(lpAxis)
#define	R_X3301_on_alm(lpAxis)							R_axis_on_alm(lpAxis)

// ������Ԋ֐�
#define	R_Xlinear_reset(lpAxis1, lpAxis2)											R_axis2_reset(lpAxis1, lpAxis2)
#define	R_Xlinear_move_absolute(lpAxis1, lpAxis2, nMode, nPosition1, nPosition2)	R_axis2_move_absolute(lpAxis1, lpAxis2, nMode, nPosition1, nPosition2)
#define	R_Xlinear_move_relative(lpAxis1, lpAxis2, nMode, nOffset1, nOffset2)		R_axis2_move_relative(lpAxis1, lpAxis2, nMode, nOffset1, nOffset2)
#define	R_Xlinear_emergency_back(lpAxis1, lpAxis2, nMode)							R_axis2_emergency_back(lpAxis1, lpAxis2, nMode)
#define	R_Xlinear_emergency_move(lpAxis1, lpAxis2, nMode)							R_axis2_emergency_move(lpAxis1, lpAxis2, nMode)




/*
�֐��Ή��\

SERAMOV		R_X3301_move_absolute
SERRMOV		R_X3301_move_relative
SEROMOV		�Ȃ��i�A�҃p���X���g�p���Ă��Ȃ��ꍇ��R_X3301_emergency_move���g�p�j
SERRES		R_X3301_reset
SERINT		R_X3301_initial
SERIMOV		R_X3301_move_increase
SERDMOV		R_X3301_move_decrease

// emergency move (feedback pulse mode only)
SEREMOV		R_X3301_emergency_move
SERBMOV		R_X3301_emergency_back

// manual move
SER_MCW		R_X3301_manual_cw
SER_MCCW	R_X3301_manual_ccw
SER_MEND	�Ȃ��iR_X3301_reset���́AR_X3301_stop���g�p�j
SER_MLO		R_X3301_manual_low
SER_MHI		R_X3301_manual_high

// current position write
READR10		R_X3301_get_position
SERPOSC		R_X3301_set_position
SER0		R_X3301_reset_position

SERCONST	R_X3301_const
SERDOWN		R_X3301_down
SERSTOP		R_X3301_stop

SERPMOV		R_X3301_pulse_move

// read register
READR0		R_X3301_get_down_counter
WRITER10	�Ȃ��iR_X3301_set_position���g�p�j
SERSTAT		R_X3301_get_status
*/

#endif	// R_DRCY57_H_
