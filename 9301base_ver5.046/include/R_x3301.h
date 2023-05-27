/******************************************************************************
	R_X3301.H / R_X3301.C
	
	�ύX����
	Ver 3.10	1999/04/14	release
	Ver 3.24	2000/04/06	���_�Z���T�_�����]�o�O�C��
	Ver 3.27	2000/08/07	P5022�\���̕ύX
	Ver 3.28	2000/10/18	set_acc�֐��s��C��
	Ver 3.28	2000/10/18	���̓p�����[�^���\���̂Ɋi�[
	Ver 3.28	2000/10/31	set�֐������ԈႢERROR3�x����ǉ�
	Ver 3.28	2000/10/31	Initial���move�֐��s����C��
	Ver 3.50	2008/04/08	RX3301On�`�̖߂�l�̃R�����g���C��
	Ver 3.50	2009/02/12	DOS & LINUX �\�[�X����
******************************************************************************/

#ifndef	R_X3301_H_
#define	R_X3301_H_

int		RX3301IsAvailable(int nChannel);
int		RX3301Open(AXIS* lpAxis);

// �p�����[�^�ݒ�֐�
int		RX3301SetRange(AXIS* lpAxis, int nRange);
int		RX3301SetFreqMin(AXIS* lpAxis, int nFreq);
int		RX3301SetFreqMax1(AXIS* lpAxis, int nFreq);
int		RX3301SetFreqMax2(AXIS* lpAxis, int nFreq);
int		RX3301SetAcc(AXIS* lpAxis, int nTg, int nAlpha);

// �\�t�g�E�F�A���~�b�g�ݒ�֐�
int		RX3301SetLimitCw(AXIS* lpAxis, int nLimit);
int		RX3301SetLimitCcw(AXIS* lpAxis, int nLimit);

// ���_���A����֐�
int		RX3301Initial(AXIS* lpAxis, int mode);

int		RX3301Reset(AXIS* lpAxis);
int		RX3301Stop(AXIS* lpAxis);
int		RX3301Down(AXIS* lpAxis);

// �ʒu���ߓ���֐�
int		RX3301MoveAbsolute(AXIS* lpAxis, int nMode, int nPoint);
int		RX3301MoveRelative(AXIS* lpAxis, int nMode, int nOffset);
//int	RX3301MoveIncrease(AXIS* lpAxis);
//int	RX3301MoveDecrease(AXIS* lpAxis);

// ����~���A�֐�
int		RX3301EmergencyMove(AXIS* lpAxis, int nMode);
int		RX3301EmergencyBack(AXIS* lpAxis, int nMode);

// �����p���X���U�֐�
int		RX3301MoveInfinity(AXIS* lpAxis, int nMode, int nDirection);

// ���݈ʒu�J�E���^�֐�
int		RX3301GetPosition(AXIS* lpAxis);
int		RX3301SetPosition(AXIS* lpAxis, int nCounter);
int		RX3301ResetPosition(AXIS* lpAxis);

// �_�E���J�E���^�֐�
int		RX3301GetDownCounter(AXIS* lpAxis);

// �ėp�o�͊֐�
int		RX3301ClrOneshot(AXIS* lpAxis);
int		RX3301ClrOn(AXIS* lpAxis);
int		RX3301ClrOff(AXIS* lpAxis);
int		RX3301ResOn(AXIS* lpAxis);
int		RX3301ResOff(AXIS* lpAxis);

// �X�e�[�^�X�֐�
int		RX3301GetStatus(AXIS* lpAxis);
int		RX3301OnMove(AXIS* lpAxis);
int		RX3301OnInp(AXIS* lpAxis);
int		RX3301OnOrgi(AXIS* lpAxis);
int		RX3301OnEz(AXIS* lpAxis);
int		RX3301OnElCw(AXIS* lpAxis);
int		RX3301OnElCcw(AXIS* lpAxis);
int		RX3301OnAlm(AXIS* lpAxis);

// �Q��������Ԋ֐�
int		RX3301x2Reset(AXIS* lpAxis1, AXIS* lpAxis2);
int		RX3301x2MoveAbsolute(AXIS* lpAxis1, AXIS* lpAxis2, int nMode, int nPoint1, int nPoint2);
int		RX3301x2MoveRelative(AXIS* lpAxis1, AXIS* lpAxis2, int nMode, int nOffset1, int nOffset2);
int		RX3301x2EmergencyBack(AXIS* lpAxis1, AXIS* lpAxis2, int nMode);
int		RX3301x2EmergencyMove(AXIS* lpAxis1, AXIS* lpAxis2, int nMode);

#endif	// R_X3301_H_
