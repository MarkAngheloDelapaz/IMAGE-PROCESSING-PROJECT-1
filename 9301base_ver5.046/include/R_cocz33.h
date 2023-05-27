/******************************************************************************
	R_COCZ33.H / R_COCZ33.C
	
	�ύX����
	Ver 3.04	1999/01/13	CO-CZ-33�������Ή�
	Ver 3.20	1999/12/12	���W���[�����iCO-CZ-35�Ή��j
				2000/02/15	�o�O�t�B�N�X
	Ver 3.27	2000/09/06	R_copy.com�ɉe������o�O���C��
				2000/10/31	TI124A�Ƽ����i����Ѽ������j�o�O�C��
				2000/11/01	XC-ST50����Ѽ���(�ݸؾ��)�Ή�
				2000/11/14	�����֐�RCocz33DisplayControl�C��
	Ver 3.28	2001/02/08	�I�[�g�V�[�P���X�捞�Ή�
	Ver 3.30	2001/10/30	CaptureMulti�o�O�C��
	Ver 3.30	2002/01/10	�I�[�g�V�[�P���X�m�C�Y�Ή�
				2002/01/11	XC-ST50����Ѽ����\�����P
				2002/02/14	XC-ST50�C�j�V�G�[�^�捞���P
				2002/03/01	XC-ST50��ļ��ݽ�Ƽ����捞�Ή�
				2002/03/04	lpGrayImage�AlpBinImage�ǉ�
	Ver 3.31	2002/04/24	ID1�ȍ~��InitAdc�o�O�C��
				2002/07/25	XC-ST50�g���K���I���Ή�
	Ver 3.34	2002/12/20	cocz33_t�\���̂�nRevision��ǉ�
	Ver 3.34	2003/03/11	CaptureMulti�o�O�C��
	Ver 3.39	2003/06/04	XC-ST50����Ѽ����ANORMAL���s��C��
	Ver 3.39	2003/06/04	lpImageBuffer�AnStatus�ǉ�
	Ver 3.39	2003/06/05	R_get_video��ANY_LINE�捞���^�C�}�l�C��
	Ver 3.39	2003/06/20	CaptureMulti�֐�TI124A�Ƽ����Ή�
	Ver 3.39	2003/06/23	R_get_video_auto XC-ST50�s��C��
	Ver 3.39	2003/06/26	RCocz33SetCameraChannel��NO_CHECK��ǉ�
	Ver 3.40	2003/09/27	CaptureMulti�֐���XCST50�Ή�
	Ver 3.41	2003/10/17	CO-CZ-39�p�p�����[�^�ǉ�
	Ver 3.41	2004/01/15	RCocz33Set/GetBinLevel�ǉ�
	Ver 3.42	2004/03/31	cocz33_t�\���̂�nChannel��ǉ�
	Ver 3.42	2004/04/23	RCocz33Set/GetInitiatorPage��ǉ�
	Ver 3.43	2004/09/30	lpCurrentCocz33�̏����l��NULL�ɏC��
	Ver 3.44	2004/10/06	cocz33_t�\���̂�nAutoVideoCaptureTime��ǉ�
	Ver 3.44	2004/10/08	RCocz33CheckVS�ARCocz33CheckDispt��ǉ�
	Ver 3.44	2004/10/12	�����Ԋ����ݑ΍�
	Ver 3.44	2004/10/13	RTimerSet�ARTimerIsUp�p�~
	Ver 3.45	2004/10/22	RCocz33Open�̖߂�l��ID�ԍ��ɕύX
	Ver 3.45	2005/02/16	cocz33_t�\���̂�nMux�`��ǉ�
	Ver 3.46	2005/04/08	RCocz33SelectCard�C���BMxSize���ؑւ�炸�B
	Ver 3.48	2006/03/09	cocz33_t�\���̂�nAdcReference[]��ǉ�
	Ver 3.48	2006/03/27	RCocz33ChangeVideoMode����
	Ver 3.49	2006/08/08	BOARD_ID_COCZ41�ABOARD_ID_COCZ43��ǉ�
	Ver 3.49	2006/09/11	RDrawPlaneResource��ǉ�
	Ver 3.49	2006/09/14	RCocz33InitResource�ASelectResource�ǉ�
	Ver 3.49	2006/10/10	GetVideoAuto�֐��ATI-124A/Initiator�Ή�
	Ver 3.49	2006/10/18	BOARD_ID_COCZxx��`��R_video.h�ֈړ�
	Ver 3.49	2006/11/29	cocz33_t�\���̂�nStatus1�`3�ǉ�
	Ver 3.49	2006/12/27	pPlaneDraw,pPlaneUser,pPlaneTitle�ǉ�
	Ver 3.49	2007/01/06	cocz33_t�\���̂�pBinPage[2]�ǉ�
	Ver 3.49	2007/02/07	cocz33_t�\���̂�nCaptureTimeLimit�ǉ�
	Ver 3.49	2007/02/15	cocz33_t�\���̂�R_video.h�ֈړ�
	Ver 3.50	2008/11/04	�L���v�`�����X���[�\�������ؑ։��P
******************************************************************************/

#ifndef __R_COCZ33_H__
#define __R_COCZ33_H__

#ifdef __cplusplus
extern "C" {
#endif

extern int	COCZ33;

void	RCocz33InitResource(void);
void	RCocz33SelectResource(void);
void	RCocz33InitFuncTable(struct cocz33_t* cocz33);

int		RCocz33Open(int i, int j);
int		RCocz33SelectCard(int nID);
int		RCocz33PutwPROM(int nAdr, int nData);
int		RCocz33GetwPROM(int nAdr);

int		RCocz33CaptureMulti(int nBoard, int nMode, int nFlag);

int		RCocz33SetCameraChannel(struct cocz33_t*, int nFlag, int nChannel, int nBinLevel);
int		RCocz33DisplayControl(struct cocz33_t*, int);
int		RCocz33ChangeVideoMode(struct cocz33_t*, int nMode);
int		RCocz33ChangeCameraMode(struct cocz33_t*, int nMode);
void	RCocz33SetLinePage(struct cocz33_t*, int);
int		RCocz33GetVideo(struct cocz33_t*, int nMode, int nFlag);
int		RCocz33GetVideoAuto(struct cocz33_t*, int nMode, int nFlag);
int		RCocz33GetVideoAutoEndCheck(struct cocz33_t*);
int		RCocz33GetVideoAutoShutterCheck(struct cocz33_t* lpCocz33,int camera);
int		RCocz33GetVideoAutoSiWait(struct cocz33_t* lpCocz33,int camera);

int		RCocz33GetwEEPROM(struct cocz33_t*, int);
int		RCocz33PutwEEPROM(struct cocz33_t*, int, int);
int		RCocz33InitAdcReference(struct cocz33_t*);
int		RCocz33RegulateAdcReference(struct cocz33_t*, int, int, int);
int		RCocz33SetDacM62352(struct cocz33_t* lpCocz33, int ch, int data);
int		RCocz33SetShutterSpeed(struct cocz33_t* lpCocz33, int nShutterSpeed);
int		RCocz33GetShutterSpeed(struct cocz33_t* lpCocz33);
int		RCocz33SetBinLevel(struct cocz33_t* lpCocz33, int nLevel);
int		RCocz33GetBinLevel(struct cocz33_t* lpCocz33);

int		RCocz33GetVideoInitiator(struct cocz33_t* lpCocz33, int mode, int nPlane);
int		RCocz33SetInitiatorPage(struct cocz33_t* lpCocz33, int nPage);
int		RCocz33GetInitiatorPage(struct cocz33_t* lpCocz33);

int		RCocz33CheckVS(struct cocz33_t* lpCocz33);
int		RCocz33CheckDispt(struct cocz33_t* lpCocz33);
void	RCocz33WaitVS(struct cocz33_t* lpCocz33);
void	RCocz33WaitDispt(struct cocz33_t* lpCocz33);
void	RCocz33WaitDisptWait(struct cocz33_t* lpCocz33);

#define	RCocz33SetTriggerHigh(lpCocz33)		outp((lpCocz33)->nOutPort6, (lpCocz33)->nOutData6 &= ~0x04)	// Trigger high
#define	RCocz33SetTriggerLow(lpCocz33)		outp((lpCocz33)->nOutPort6, (lpCocz33)->nOutData6 |=  0x04)	// Trigger Low

#ifdef __cplusplus
}
#endif

#endif	// R_COCZ33_H_

/*
extern int	Cocz33RegisterPort;
extern int	S5933Port;
extern int	Cocz33IRQ;

#define	COCZ33MModeSelect					(lpCurrentCocz33->nOutData0)
#define	COCZ33MDisplayVideoMode				(lpCurrentCocz33->nOutData2)
#define	COCZ33MExternalOutput				(lpCurrentCocz33->nOutData2)
#define	COCZ33MVideoOutMode					(lpCurrentCocz33->nOutData3)
#define	COCZ33MAutoVstartCommand			(lpCurrentCocz33->nOutData4)
#define	COCZ33MCameraRegulation				(lpCurrentCocz33->nOutData5)
#define	COCZ33MCameraCommand				(lpCurrentCocz33->nOutData6)

#define	COCZ33MonitorStatus1()				inp(Cocz33RegisterPort + 0x00)
#define	COCZ33ExternalInput1()				inp(Cocz33RegisterPort + 0x00)
#define	COCZ33ExternalInput2()				inp(Cocz33RegisterPort + 0x01)
#define	COCZ33AutoVstartStatus()			inp(Cocz33RegisterPort + 0x02)
#define	COCZ33MonitorStatus2()				inp(Cocz33RegisterPort + 0x04)
#define	COCZ33CameraStatus()				inp(Cocz33RegisterPort + 0x04)

#define	COCZ33ModeSelect(data)				outp(lpCurrentCocz33->nOutPort0, data)
#define	COCZ33BinaryThresholdLevel(data)	outp(lpCurrentCocz33->nOutPort1, data)
#define	COCZ33DisplayVideoMode(data)		outp(lpCurrentCocz33->nOutPort2, data)
#define	COCZ33ExternalOutput(data)			outp(lpCurrentCocz33->nOutPort2, data)
#define	COCZ33VideoOutMode(data)			outp(lpCurrentCocz33->nOutPort3, data)
#define	COCZ33AutoVstartCommand(data)		outp(lpCurrentCocz33->nOutPort4, data)
#define	COCZ33CameraRegulation(data)		outp(lpCurrentCocz33->nOutPort5, data)
#define	COCZ33CameraCommand(data)			outp(lpCurrentCocz33->nOutPort6, data)

#define	COCZ33MRAR		(S5933Port + 0x2C)
#define	COCZ33MRTC		(S5933Port + 0x30)
#define	COCZ33MCSR		(S5933Port + 0x3C)
*/
