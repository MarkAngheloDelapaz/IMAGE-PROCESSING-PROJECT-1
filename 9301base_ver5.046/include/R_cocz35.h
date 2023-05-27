/******************************************************************************
	R_COCZ35.H / R_COCZ35.C
	
	�ύX����
	Ver 3.20	1999/12/12	���������[�X
				2000/02/15	RCocz35Shutter�ǉ�
				2000/09/27	CS8530D-01�Ή�
				2000/11/17	�����֐�RCocz35SetCameraChannel�ǉ�
	Ver 3.30	2001/08/24	CS8531�����_���V���b�^���P
	Ver 3.33	2002/11/15	CS8550Di/CS8541D�Ή�
	Ver 3.34	2003/01/20	CO-CZ-35 Revision.2 �Ή�
	Ver 3.35	2003/03/27	XC-HR50 STANBY�֕ύX����̓�������΍�
	Ver 3.38	2003/04/18	XC-HR50 NORMAL�֕ύX����̓�������΍�
	Ver 3.38	2003/05/15	�����������捞�֐�CaptureMulti�ǉ�
	Ver 3.37	2003/05/23	XC-HR50�m�[�}����荞�ݕs��C��
	Ver 3.38	2003/05/28	XC-HR50 �I�[�g�V�[�P���X�捞�Ή�
	Ver 3.39	2003/06/19	ANY_FIELD �I�[�g�V�[�P���X�捞�Ή�
	Ver 3.39	2003/06/23	GetVideo PROGRESSIVE-ANY_FIELD�C��
	Ver 3.39	2003/06/24	ChangeCameraMode�Đݒ莞�^�C�}�ȗ�
	Ver 3.39	2003/06/24	RCocz35SetCameraChannel��NO_CHECK��ǉ�
	Ver 3.41	2003/12/03	CaptureMulti�ɵ�ļ��ݽӰ�ޒǉ�
	Ver 3.43	2004/09/30	�����_���V���b�^����_disable�ύX
	Ver 3.44	2004/10/07	�����Ԋ����ݑ΍�
	Ver 3.45	2004/10/21	RCocz35CaptureMulti�o�X���׌y��
	Ver 3.45	2004/10/22	RCocz35Open�̖߂�l��ID�ԍ��ɕύX
	Ver 3.45	2005/04/01	RCocz35GetVideoAutoEndCheck�C��
							Ver3.44�ŏ��ERROR���^�[�����Ă����B
	Ver 3.46	2005/04/01	RCocz35ChangeVideoMode�֐����P
							VM_BOARD_SYNC�Ή�
	Ver 3.46	2005/04/07	RCocz35GetVideoAutoEndCheck�C��
							Ver3.45��TimeUp�G���[���������Ă����B
	Ver 3.47	2005/10/27	XCHR50��CS8550D��ANY_FIELD�捞�C��
							Ver3.44��XCHR50��CS8550D��ANY_FIELD�捞�s�\�ɂȂ��Ă����B
	Ver 3.47	2005/11/14	XCST50�����_���V���b�^�C��
	Ver 3.48	2006/03/02	GetVideoAuto�֐��ACS8530D��BINNING�Ή�
	Ver 3.49	2007/01/06	���������[�`���ύX
	Ver 3.49	2007/03/29	R_copy�p���[�`���֐���
	Ver 3.50	2008/11/04	�L���v�`�����X���[�\�������ؑ։��P
******************************************************************************/

#ifndef __R_COCZ35_H__
#define __R_COCZ35_H__

#include "R_cocz33.h"

extern int	COCZ35;

int	RCocz35Open(int i, int j);
int	RCocz35SetPLL(struct cocz33_t*, int, int);

int	RCocz35SetCameraHorizontalArea(struct cocz33_t*, int);
int	RCocz35GetCameraHorizontalArea(struct cocz33_t*);
int	RCocz35SetCameraVerticalArea(struct cocz33_t*, int);
int	RCocz35GetCameraVerticalArea(struct cocz33_t*);
int	RCocz35SetDisplayHorizontalArea(struct cocz33_t*, int);
int	RCocz35GetDisplayHorizontalArea(struct cocz33_t*);
int	RCocz35SetDisplayVerticalArea(struct cocz33_t*, int);
int	RCocz35GetDisplayVerticalArea(struct cocz33_t*);
int	RCocz35SetShutterSpeed(struct cocz33_t*, int);
int	RCocz35Shutter(struct cocz33_t*, int);

int	RCocz35ChangeVideoMode(struct cocz33_t*, int);
int	RCocz35ChangeCameraMode(struct cocz33_t*, int);
int	RCocz35GetVideo(struct cocz33_t*, int nMode, int nFlag);
int	RCocz35SetCameraChannel(struct cocz33_t*, int nFlag, int nChannel, int nBinLevel);

int	RCocz35GetVideoAuto(struct cocz33_t*, int, int);
int	RCocz35GetVideoAutoEndCheck(struct cocz33_t*);

int	RCocz35CaptureMulti(int nBoard, int nMode, int nFlag);

int	RCocz35PrintScreen(struct cocz33_t* lpCocz35, void* pDataBuffer);

int	RCocz35InitAdcReference(struct cocz33_t* lpCocz35);

#define	RCocz35DisplayControl				RCocz33DisplayControl
#define	RCocz35SetTriggerHigh(lpCocz35)		outp((lpCocz35)->nOutPort5, (lpCocz35)->nOutData5 &= ~0x08)	// Trigger high
#define	RCocz35SetTriggerLow(lpCocz35)		outp((lpCocz35)->nOutPort5, (lpCocz35)->nOutData5 |=  0x08)	// Trigger Low
#define	RCocz35GetShutterSpeed(lpCocz35)	(lpCocz35)->nShutterSpeed

#endif
