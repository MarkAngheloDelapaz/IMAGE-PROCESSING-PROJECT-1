/******************************************************************************
	R_COCZ39.H / R_COCZ39.C
	
	�ύX����
	Ver 3.41	2003/12/12	�b��\�[�X
	Ver 3.42	2004/04/21	�b��\�[�X
	Ver 3.45	2004/11/04	RCocz39GetVideoAutoShutterEndCheck�ǉ�
	Ver 3.45	2005/03/25	RCocz39ChangeCameraMode�o�O�t�B�N�X
	Ver 3.46	2005/04/01	RCocz39GetVideoAuto,WEN�`�F�b�N�ǉ�
	Ver 3.46	2005/04/28	RCocz39Set/GetCamera�`Area�ǉ�
	Ver 3.47	2005/12/01	CO-CZ-39 Ver.3 / CO-CZ-39A�b��Ή�
	Ver 3.47	2005/12/01	VGA�o�̓��[�h�ǉ��i39Ver.3�ȍ~�A39A�j
	Ver 3.47	2005/12/22	CS8541D�Ή��i39Ver.3�A39A�j
	Ver 3.47	2005/12/23	TI-124A�Ή��i39Ver.3�A39A�j
	Ver 3.47	2005/12/26	XC-ST50�Ή��i39Ver.3�A39A�j
	Ver 3.47	2006/01/27	BIN_PLANE�EInitiator�Ή��i39Ver.3�A39A�j
	Ver 3.47	2006/01/31	XC-HR70�b��Ή��i39Ver.3�A39A�j
	Ver 3.48	2006/03/09	RCocz39LoadAdcReference�ǉ�
	Ver 3.48	2006/06/02	�N������CO-CZ-39H��\��
	Ver 3.48	2006/07/25	���[�J�R�[�h�A�V���A���i���o�[�\��
	Ver 3.48	2006/07/25	������ݒ�C��
	Ver 3.48	2006/07/27	CO-CZ-39H�Ή�
	Ver 3.48	2006/08/07	CO-CZ-39A/H HSync Edge Select
	Ver 3.49	2006/08/17	CO-CZ-39A/H XC-HR50 High Rate Scan
	Ver 3.49	2006/09/11	DrawPlane->R_PLANE
	Ver 3.49	2006/10/05	RCocz39SetDisplayPage offset control
	Ver 3.49	2006/10/12	Over Sampling & Digital Filter
	Ver 3.49	2006/10/13	XC-HR70 Partial Scan (39H/Ver1.04)
	Ver 3.49	2006/10/16	Display Page Select by External Signal
	Ver 3.49	2006/10/20	Save booting massage
	Ver 3.49	2006/10/23	Half Resolution��Digital Filter�ύX
	Ver 3.49	2007/01/06	R_set_line_page�\�t�g�E�F�A�G�~�����[�g
	Ver 3.49	2007/01/06	���t�@�����X�d���ύX�֐�
	Ver 3.49	2007/03/19	Half Resolution��Digital Filter�o�O�C��
	Ver 3.49	2007/04/03	XGA�J����R_copy�Ή�
	Ver 3.50	2007/12/13	39Ver.2�g�p���ASetDisplayPage�o�O�C��
	Ver 3.50	2008/03/07	R_plane�w�b�_�t�@�C������
	Ver 3.50	2008/07/29	RCocz39CameraIsPresent�֐��ǉ�
	Ver 4.01	2011/01/26	CH3VideoOut�Ή�
	Ver 4.01	2011/01/27	R_copy�Ή����[�`���E512/256���[�h�Ή�
	Ver 4.01	2011/01/28	R_copy�Ή����[�`���Ecard1���Ή�
	Ver 4.01	2011/02/03	Linux�Ή�
	Ver 4.01	2015/04/02	�摜�捞���^�C���A�b�v�`�F�b�N���P
******************************************************************************/

#ifndef R_COCZ39_H_
#define R_COCZ39_H_

#include "R_cocz33.h"

#ifdef __GNUC__
#include "R_memmgr.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern int	COCZ39;

#define	COCZ39_INIT_NATIVE		0x01
#define	COCZ39_INIT_COMPATIBLE	0x00

#ifdef __GNUC__
int		RCocz39OpenAll(int bMultiProcess, int nBoardID);
#endif

int		RCocz39Open(int i, int j);
int		RCocz39Init(struct cocz33_t* lpCocz39, int nCameraType, int nFrameSize, int nCameraMode, int nMode);

int		RCocz39SetPLL(struct cocz33_t* lpCocz39, int nAdr, int nData);
int		RCocz39GetPLL(struct cocz33_t* lpCocz39, int nAdr);

int		RCocz39GetCameraVerticalArea(struct cocz33_t*, int*, int*);
int		RCocz39SetCameraVerticalArea(struct cocz33_t*, int, int);
int		RCocz39GetCameraHorizontalArea(struct cocz33_t*, int*, int*);
int		RCocz39SetCameraHorizontalArea(struct cocz33_t*, int, int);
int		RCocz39SetShutterSpeed(struct cocz33_t*, int);
int		RCocz39Shutter(struct cocz33_t*, int);

int		RCocz39DisplayControl(struct cocz33_t*, int);
int		RCocz39ChangeVideoMode(struct cocz33_t*, int);
int		RCocz39ChangeCameraMode(struct cocz33_t*, int);
int		RCocz39GetVideo(struct cocz33_t*, int nMode, int nFlag);
int		RCocz39SetCameraChannel(struct cocz33_t*, int nFlag, int nChannel, int nBinLevel);

int		RCocz39GetwEEPROM(struct cocz33_t*, int);
int		RCocz39PutwEEPROM(struct cocz33_t*, int, int);
int		RCocz39SetDacBU2507(struct cocz33_t*, int, int);

int		RCocz39GetVideoAuto(struct cocz33_t*, int, int);
int		RCocz39GetVideoAutoEndCheck(struct cocz33_t*);
int		RCocz39GetVideoAutoShutterEndCheck(struct cocz33_t*);

void	RCocz39SetLinePage(struct cocz33_t*, int);

int		RCocz39SetDisplayPage(struct cocz33_t*, int);
int		RCocz39GetDisplayPage(struct cocz33_t*);

int		RCocz39LoadAdcReference(struct cocz33_t*, int);

int		RCocz39PrintScreen(struct cocz33_t* lpCocz39, void* pDataBuffer);

int		RCocz39CameraIsPresent(struct cocz33_t*);

#ifdef __GNUC__
int		RCocz39GrabStart(struct cocz33_t* lpCocz43, int nMode, struct r_memmgr* pMemmgrNode, unsigned long nOffset);
#endif	// __GNUC__

#ifdef __cplusplus
}
#endif

#endif	// R_COCZ39_H_
