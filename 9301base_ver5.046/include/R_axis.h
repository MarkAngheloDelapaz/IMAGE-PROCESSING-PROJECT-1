/******************************************************************************
	R_AXIS.H / R_AXIS.C
	
	�ύX����
	Ver 3.10	1999/04/14	release
	Ver 3.22	2000/04/05	DR-CY-72A�Ή�
	Ver 3.27	2000/08/07	DR-CY-72A �r���쓮�Ή�
	Ver 3.27	2000/08/07	DR-CY-72A ���������ʐݒ�Ή�
	Ver 3.275b	2000/10/31	set�֐��������۔���t���O�ǉ�
	Ver 3.28	2000/10/31	DR-CY-57B Initial��move�֐��s��C��
	Ver 3.34	2002/12/26	DR-CY-90�Ή�
	Ver 3.34	2003/03/25	R_axis_close�o�O�C���AAXIS���ނ�id�ǉ�
	Ver 3.35	2003/02/20	DR-CY-88�Ή�
	Ver 3.50	2009/02/13	DOS & LINUX �\�[�X����
	Ver.3.50	2009/07/15	�����֐�static���ƃv���g�^�C�v�錾�ǉ�
	Ver 4.01	2011/07/05	DR-CY-132/134�Ή�
	Ver 4.01	2012/04/05	�}���`�v���Z�X�Ή�
	Ver 4.01	2012/09/11	AXIS�\���̂�base�As_flag�ǉ�
	Ver 4.01	2013/09/05	AXIS�\���̂�start_mode�ǉ�
	Ver 4.01	2016/06/07	AXIS�\���̂̃k���|�C���^�`�F�b�N�ǉ�
	Ver 4.01	2017/04/25	DR-CY-132/134��FPGA�o�[�W�����`�F�b�N�ǉ�
	Ver 5.010	2019/01/08	DR-CY-144�Ή�
******************************************************************************/

#ifndef	R_AXIS_H_
#define	R_AXIS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define	DR_CY_57	1
#define	DR_CZ_65	2
#define	DR_CY_68	3
#define	DR_CY_72	4
#define	DR_CY_88	5
#define	DR_CY_132	7

#define	DEVICE_X3301		1
#define	DEVICE_PCL240MS		2
#define	DEVICE_PCL5022		3
#define	DEVICE_FPGA_TL		4

// �I�[�v���֐��G���[�v��
#define	AXIS_ALLOCATED		(-1)		// ���ɂ`�w�h�r�\���̂��A�̈�m�ۂ���Ă���B
#define	AXIS_NOT_EXIST		(-2)		// �Y�����������݂��Ȃ��B
#define	AXIS_ALLOC_ERR		(-3)		// �̈�m�ۂł��Ȃ��B
#define	AXIS_NOT_AVAILABLE	(-4)		// ����g�p�s�\�B
										// �@DR-CY-132/134��FPGA�o�[�W�������Â��ȂǁB

// ���݈ʒu�Ǘ����@�ݒ�
#define	WITHOUT_FEEDBACK	0x00000000	// �A�҃p���X�𗘗p���Ȃ��B
#define	WITH_FEEDBACK		0x00000001	// �A�҃p���X�𗘗p����B

// �p���X�o�͕����ݒ�
#define	PULSE_2CLOCK		0x00000000	// �p���X���U���Q�N���b�N�����ōs���B
#define	PULSE_GATE			0x00000010	// �p���X���U���Q�[�g�����ōs���B

// ���_�Z���T�_���ݒ�
#define	ORG_ACTIVE_LOW		0x00000000	// ���_�Z���T���_��
#define	ORG_ACTIVE_HIGH		0x00000100	// ���_�Z���T���_��

// �t�B�[�h�o�b�N�p���X�J�E���g�����ݒ�
#define	PHASEX1				0x0000		// �Q���N���b�N�P���{�i�`���A�a���j
#define	PHASEX2				0x1000		// �Q���N���b�N�Q���{�i�`���A�a���j
#define	PHASEX4				0x2000		// �Q���N���b�N�S���{�i�`���A�a���j
#define	BICLOCK				0x3000		// �Q�N���b�N���_���i�b�v�A�b�b�v�j

// ���U�p���X���������ݒ�
#define	FORWARD_CW			0x00000		// �{�������U���b�v
#define	FORWARD_CCW			0x10000		// �{�������U���b�b�v
#define	ForwardCW(nMode)	!((nMode) & FORWARD_CCW)
#define	ForwardCCW(nMode)	((nMode) & FORWARD_CCW)

// �t�B�[�h�o�b�N�p���X�����ݒ�
#define	ENCODER_NORMAL		0x00000		// �G���R�[�_�J�E���g�������]
#define	ENCODER_REVERSE		0x20000		// �G���R�[�_�J�E���g�����t�]
#define	EncoderNormal(nMode)	!((nMode) & ENCODER_REVERSE)
#define	EncoderReverse(nMode)	((nMode) & ENCODER_REVERSE)

// ���_���A����ݒ�
#define	AXIS_INIT_FORWARD	0x0			// +�����Ɍ��_���A
#define	AXIS_INIT_BACK		0x1			// -�����Ɍ��_���A
#define	AXIS_INIT_ORG		0x00		// �n�q�f�̂n�m�Œ�~�B
#define	AXIS_INIT_EZ		0x10		// �n�q�f���n�m������̂d�y�̂n�m�Œ�~�B

// 
#define	AXIS_INFINITY_FORWARD	0x0
#define	AXIS_INFINITY_BACK		0x1

// ��h�c�ݒ�
#define	AXIS_CHANNEL_0	0x00000
#define	AXIS_CHANNEL_1	0x01000
#define	AXIS_CHANNEL_2	0x02000
#define	AXIS_CHANNEL_3	0x03000
#define	AXIS_CHANNEL_4	0x04000
#define	AXIS_CHANNEL_5	0x05000
#define	AXIS_CHANNEL_6	0x06000
#define	AXIS_CHANNEL_7	0x07000
#define	AXIS_CHANNEL_8	0x08000
#define	AXIS_CHANNEL_9	0x09000
#define	AXIS_CHANNEL_A	0x0A000
#define	AXIS_CHANNEL_B	0x0B000
#define	AXIS_CHANNEL_C	0x0C000
#define	AXIS_CHANNEL_D	0x0D000
#define	AXIS_CHANNEL_E	0x0E000
#define	AXIS_CHANNEL_F	0x0F000
#define	AXIS_CHANNEL(n)	(n<<12)

// �f�o�C�X�w��
#define	AXIS_DEVICE_1	0x0104
#define	AXIS_DEVICE_2	0x0504
#define	AXIS_DEVICE_3	0x0904
#define	AXIS_DEVICE_4	0x0D04

#define	AXIS_PORT(rdsw,num)	(0x104|(num<<10)|(rdsw<<12))

#define	FH1_SSSD		0x15			// �e�g�P�����X�^�[�g
#define	FH2_SSSD		0x17			// �e�g�Q�����X�^�[�g
#define	FL_CONST		0x10			// �e�k�葬�X�^�[�g
#define	FH1_CONST		0x11			// �e�g�P�葬�X�^�[�g
#define	FH2_CONST		0x13			// �e�g�Q�葬�X�^�[�g

#define	WITH_LIMIT_CHECK	(int)0x0000	// ���~�b�g�`�F�b�N���s��
#define	WITHOUT_LIMIT_CHECK	(int)0x0100	// ���~�b�g�`�F�b�N���s��Ȃ�
#define	WithLimitCheck(nMode)	!((nMode) & WITHOUT_LIMIT_CHECK)

#define	RANGE_DEFAULT		15			// ���g���{���f�t�H���g�l
#define	FL_DEFAULT			1000		// 
#define	FH1_DEFAULT			30000		// 
#define	FH2_DEFAULT			10000		// 
#define	ACC_DEFAULT			10			// ���������ԃf�t�H���g�l
#define	ALPHA_DEFAULT		100			// �r���������̈�f�t�H���g�l

typedef struct {
	int		id;							// �h�c�i�q�c�r�v�j
	int		board;						// ���
	int		axis;						// ��
	int		device;						// �f�o�C�X��
	int		fd;
	unsigned long	port;				// �|�[�g�A�h���X
	double	range;						// ���g���{��

	int		nFreqFL;					// ���N�����g��	added by sugimoto 2000/09/28
	int		nFreqFH1;					// �ō����g��	added by sugimoto 2000/09/28
	int		nFreqFH2;					// �ō����g��	added by sugimoto 2000/09/28

	int		reg_range;					// ���g���{�����W�X�^�l added by sugimoto 2000/08/07
	int		freq_min;					// ���N�����g�����W�X�^�l
	int		freq_max1;					// �ō����g�����W�X�^�l
	int		freq_max2;					// �ō����g�����W�X�^�l
	int		nTgUp;						// �������� added by sugimoto 2000/08/07
	int		nTgDown;					// �������� added by sugimoto 2000/08/07
	int		nRegAccUp;					// �������W�X�^�l	acc->nRegAccUp revised by sugimoto 2000/08/07
	int		nRegAccDown;				// �������W�X�^�l	added by sugimoto 2000/08/07
	int		nAlphaUp;					// �r�������̈�		alpha->nAlphaUp revised by sugimoto 2000/08/07
	int		nAlphaDown;					// �r�������̈�		added by sugimoto 2000/08/07
	int		limit_cw;					// �ړ��ő�_�i���~�b�g�j��������
	int		limit_ccw;					// �ړ��ő�_�i���~�b�g�j����������
	int		mode;
	
	int		manual;						// �}�j���A�����쎞���샂�[�h
	int		destination_point;			// �ړ��ڕW�_
	int		back_point;					// �ړ��J�n�_

//added by sugimoto 00/08/07 ->
	int		nRegAlphaUp;				// P5023 R16	
	int		nRegAlphaDown;				// P5023 R17	
	int		nFH1TriangleBorder1;		// FH1�O�p�쓮�������E�_1	
	int		nFH1TriangleBorder2;		// FH1�O�p�쓮�������E�_2	
	int		nFH1TriangleBorder3;		// FH1�O�p�쓮�������E�_3	
	double	lfFH1AutoBorder;			// FH1�e�g�I�[�g�␳���E�_	added by sugimoto 2000/09/28
	int		nFH2TriangleBorder1;		// FH2�O�p�쓮�������E�_1	
	int		nFH2TriangleBorder2;		// FH2�O�p�쓮�������E�_2	
	int		nFH2TriangleBorder3;		// FH2�O�p�쓮�������E�_3	
	double	lfFH2AutoBorder;			// FH2�e�g�I�[�g�␳���E�_	added by sugimoto 2000/09/28
//added by sugimoto 00/08/07 -<

	int		nSet_Flag;					// set�֐����s�t���O		added by sugimoto 2000/09/28

	int		nRMDReg;					// PCL5022���샂�[�h���W�X�^	added by sugimoto 2000/12/07
	int		nCMD2;						// X3301���쐧��R�}���h	added by sugimoto 2000/10/31
	
	const char*	board_name;
	unsigned long	base;
	int		s_flag;
	int		r_delay;
	
	int		start_mode;
} AXIS;




int		RDrcy72Open(int i, int j);
int		RDrcy88Open(int i, int j);
int		RDrcy90Open(int i, int j);

int		RDrcy72DriverOpen(int fd, uintptr_t header);
int		RDrcy90DriverOpen(int fd, uintptr_t header);

int		r_axis_regist_id(int id, int type);
#define	PCI_AXIS_DRCY72		1
#define	PCI_AXIS_DRCY90		2
#define	PCI_AXIS_DRCY132	3
#define	PCI_AXIS_DRCY134	4
#define	PCI_AXIS_DRCY144	5

// �v���g�^�C�v�錾

int		R_axis_board_is_present(int rdsw);

/*!
	@brief	�p���X������̃I�[�v��
	
	@param	axis_p	�T�[�{�\���̃|�C���^�̃A�h���X
	@param	channel	�p���X�������ID�ԍ��i����RDSW�Őݒ�j�Ǝ��ԍ���OR
				�� ��					�� ��
				AXIS_CHANNEL_0			ID = 0
				AXIS_CHANNEL_1			ID = 1
				AXIS_CHANNEL_2			ID = 2
				AXIS_CHANNEL_3			ID = 3
				AXIS_CHANNEL_4			ID = 4
				AXIS_CHANNEL_5			ID = 5
				AXIS_CHANNEL_6			ID = 6
				AXIS_CHANNEL_7			ID = 7
				AXIS_CHANNEL_8			ID = 8
				AXIS_CHANNEL_9			ID = 9
				AXIS_CHANNEL_A			ID = A
				AXIS_CHANNEL_B			ID = B
				AXIS_CHANNEL_C			ID = C
				AXIS_CHANNEL_D			ID = D
				AXIS_CHANNEL_E			ID = E
				AXIS_CHANNEL_F			ID = F
				AXIS_CHANNEL(n)			ID = n
				�� ��					�� ��
				AXIS_DEVICE_1			�P����
				AXIS_DEVICE_2			�Q����
				AXIS_DEVICE_3			�R����
				AXIS_DEVICE_4			�S����
	@param		mode
				�� ��					�� ��
				WITH_LIMIT_CHECK		���~�b�g�`�F�b�N���s��
				WITHOUT_LIMIT_CHECK		���~�b�g�`�F�b�N���s��Ȃ�
				�� ��					�� ��
				PULSE_2CLOCK			�p���X���U���Q�N���b�N�����ōs��
				PULSE_GATE				�p���X���U���Q�[�g�����ōs��
				�� ��					�� ��
				PHASEX1					�Q���N���b�N�P���{�i�`���A�a���j
				PHASEX2					�Q���N���b�N�Q���{�i�`���A�a���j
				PHASEX4					�Q���N���b�N�S���{�i�`���A�a���j
				BICLOCK					�Q�N���b�N���_���i�b�v�A�b�b�v�j
				�� ��					�� ��
				FORWARD_CW				�{�����w�߂łb�v���U
				FORWARD_CCW				�{�����w�߂łb�b�v���U
				�� ��					�� ��
				ORG_ACTIVE_LOW			���_�Z���T���_��
				ORG_ACTIVE_HIGH			���_�Z���T���_��
				�� ��					�� ��
	@return
			OK						����I��
			AXIS_ALLOCATED			���ɂ`�w�h�r�\���̂��A�̈�m�ۂ���Ă���
			AXIS_NOT_EXIST			�Y�����������݂��Ȃ�
			AXIS_ALLOC_ERR			�̈�m�ۂł��Ȃ�
*/
int		R_axis_open(AXIS** axis_p, int channel, int mode);
int		R_axis_close(AXIS* lpAxis);

// �p�����[�^�ݒ�֐�
int		R_axis_set_range(AXIS* lpAxis, int nRange);
int		R_axis_set_freq_min(AXIS* lpAxis, int nFreq);
int		R_axis_set_freq_max1(AXIS* lpAxis, int nFreq);
int		R_axis_set_freq_max2(AXIS* lpAxis, int nFreq);
int		R_axis_set_acc(AXIS* lpAxis, int nTg, int nAlpha);
/*�ȉ��̍s��ǉ�*/
int		R_axis_set_acc2(AXIS* lpAxis, int nTga, int nTgb, int nAlpha1, int nAlpha2);

// �\�t�g�E�F�A���~�b�g�ݒ�֐�
int		R_axis_set_limit_cw(AXIS* lpAxis, int nLimit);
int		R_axis_set_limit_ccw(AXIS* lpAxis, int nLimit);

// ���_���A����֐�
int		R_axis_initial(AXIS* lpAxis, int mode);

int		R_axis_reset(AXIS* lpAxis);
int		R_axis_stop(AXIS* lpAxis);
int		R_axis_down(AXIS* lpAxis);

// �ʒu���ߓ���֐�
int		R_axis_move_absolute(AXIS* lpAxis, int nMode, int nPoint);
int		R_axis_move_relative(AXIS* lpAxis, int nMode, int nOffset);
int		R_axis_move_increase(AXIS* lpAxis);
int		R_axis_move_decrease(AXIS* lpAxis);

// ����~���A�֐�
int		R_axis_emergency_move(AXIS* lpAxis, int nMode);
int		R_axis_emergency_back(AXIS* lpAxis, int nMode);

// �����p���X���U�֐�
int		R_axis_move_infinity(AXIS* lpAxis, int nMode, int nDirection);

// ���݈ʒu�J�E���^�֐�
int		R_axis_get_position(AXIS* lpAxis);
int		R_axis_set_position(AXIS* lpAxis, int nCounter);
int		R_axis_reset_position(AXIS* lpAxis);

// �_�E���J�E���^�֐�
int		R_axis_get_down_counter(AXIS* lpAxis);

// �ėp�o�͊֐�
int		R_axis_clr_oneshot(AXIS* lpAxis);
int		R_axis_clr_on(AXIS* lpAxis);
int		R_axis_clr_off(AXIS* lpAxis);
int		R_axis_res_on(AXIS* lpAxis);
int		R_axis_res_off(AXIS* lpAxis);

// �X�e�[�^�X�֐�
int		R_axis_get_status(AXIS* lpAxis);
int		R_axis_on_move(AXIS* lpAxis);
int		R_axis_on_inp(AXIS* lpAxis);
int		R_axis_on_orgi(AXIS* lpAxis);
int		R_axis_on_ez(AXIS* lpAxis);
int		R_axis_on_el_cw(AXIS* lpAxis);
int		R_axis_on_el_ccw(AXIS* lpAxis);
int		R_axis_on_alm(AXIS* lpAxis);

// �Q��������Ԋ֐�
int		R_axis2_reset(AXIS* lpAxis1, AXIS *lpAxis2);
int		R_axis2_move_absolute(AXIS* lpAxis1, AXIS* lpAxis2, int nMode, int nPoint1, int nPoint2);
int		R_axis2_move_relative(AXIS* lpAxis1, AXIS* lpAxis2, int nMode, int nOffset1, int nOffset2);
int		R_axis2_emergency_back(AXIS* lpAxis1, AXIS* lpAxis2, int nMode);
int		R_axis2_emergency_move(AXIS* lpAxis1, AXIS* lpAxis2, int nMode);

int		R_axis_count_of_drcy132(void);
int		R_axis_count_of_drcy144(void);

#ifdef __GNUC__
void	r_axis_init_channel(void);
int	r_axis_regist_channel(int virtual_channel, int physical_channel);
int	r_axis_virtual_channel(int physical_channel);
int	r_axis_physical_channel(int virtual_channel);
#endif

#ifdef __cplusplus
}
#endif

#endif	// R_AXIS_H_
