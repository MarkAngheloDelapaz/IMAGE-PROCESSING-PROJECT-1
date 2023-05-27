/******************************************************************************

	R_CS_CAM.H
	
	�摜��荞�݊֐��w�b�_
	�o�[�W�����A�b�v���́A�K�������̒ǉ����s���A
	�O���[�v���ŕύX���e���A�i�E���X���邱�ƁI�I

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	����
	<version>	<date>				<name>		<comment>
	0.01		2004.03.15						�쐬
	0.02		2006.09.08						VM_1024_768�ǉ�
	0.03		2007.02.02						VM_1280_1024�AVM_1600_1200�ǉ�
	0.04		2007.09.27			sassa		�ȉ��̊֐���ǉ�(�f�o�b�O��)
													R_CS_CameraShutterWait()
													R_CS_CameraDisplayImage()
													R_CS_CameraChangeDisplayId()
				2007.09.28			matsu		�ȉ��̊֐���ǉ�
													R_CS_CameraSetGain()
													R_CS_CameraGetGainLimit()
	1.07b1.4	2008.06.21			matsu		�ȉ��̊֐���ǉ�
													R_CS_CameraSetShutterSpeed()
													R_CS_CameraGetShutterSpeed()
													R_CS_CameraSetInitiatorPage()
													R_CS_CameraGetInitiatorPage()
													R_CS_CameraSetInitiatorAddress()
													R_CS_CameraGetInitiatorAddress()
				2008.08.25			matsu		R_CS_CameraSetInitiatorPage()�}�N�����֐���
	1.07b1.6	2009.05.27			saito		KPFR31PCL(VGA RAW�o�� COLOR CAMERA�j�Ή�
	1.07b1.9	2009.07.16			matsu		R_CS_CameraGetType()�ǉ�
	1.07b2.1	2009.08.10			araki		�t�@�C����R_CS_CAM.H��R_cs_cam.h�ɕύX
				2009.08.10			sassa		R_CS_CameraDisplayImage()�AR_CS_CameraChangeDisplayId()�C��
												(R_cs_cap.c/h��R_CS_CaptureCameraChange()�C���ɔ���)
	1.07b2.7	2010.02.03			matsu		CAM_XCL5005�Ή�
				2010.02.20			matsu		Linux�Ή�
	1.07b2.8	2010.04.24			oshima		IK-TF5P/7P�Ή�
												DisplaySize Wide�Ή�
												CAM_KPFR80PCL�Ή�
												(�b���CAM_KPFR80PCL��R_cs_cam.h�Œ�`�A���C�u�����Ή�����폜)
	1.07b2.9	2010.11.02			oshima		R_video_init��Mode�ݒ�Ή�
	1.07b3.0	2010.11.15			sassa		CAPTURE_MAX_NB��3��4�ɕύX
	1.07b3.0	2010.12.16			tanaka		VM_2432_2048���`

	1.07b3.0	2011.03.25			saito		9301lib:ver4.01beta44�ȍ~ R_video.h �� CAM_KPFR80PCL�Ή�
												����ȑO�̃o�[�W�����̂��߂ɁA�b��R�[�h�͎c��
	1.07b5.2	2013.11.03			oshima		VM_2048_2048���`
	1.07b7.7	2016.04.15			oshima		CAPTURE_MAX_NB��4��10�ɕύX

	
												
******************************************************************************/
#ifndef __R_CS_CAM_H__
#define __R_CS_CAM_H__

//�b��
#ifndef CAM_KPFR80PCL
	#define	CAM_KPFR80PCL		0xA2000000		// HITACHI KOKUSAI DENKI KP-FR80PCL	(Bayer XGA  36fps)
	#define	__R_CS_CAM_TEMPORARY_MODE__	1
#endif


// �摜�T�C�Y
#define VM_256_240		0
#define VM_320_240		1
#define VM_512_240		2
#define VM_640_240		3
#define VM_512_480		4
#define VM_640_480		5
#define VM_1024_768		6		// 2006.09.08
#define VM_1280_1024	7		// 2007.02.02
#define VM_1600_1200	8		// 2007.02.02
//#define VM			9		// CAM_CSB4000CL�p
#define VM_2432_2048	10		// 2010.12.16
#define VM_2048_1088	11		// 2012.03.26 
#define VM_AOI			12		// 2012.11.28	kitamura

#define VM_2032_1080	13		// 2013.05.01	saito
#define VM_2048_2048	14		// 2013.11.03	oshima
#define	VM_XXX_YYY		15		// 2016.02.12	araki



// �J�����^�C�v
#define R_CS_CAMERA_GRAY	0		//<1.07b1.5>
#define R_CS_CAMERA_3CCD	1		//<1.07b1.5>
#define R_CS_CAMERA_BAYER	2		//<1.07b1.5>


#define RANDOM	STANDBY
#define CAPTURE_AUTO 0
#define CAPTURE_WAIT 1

//#define CAPTURE_MAX_NB 4		// 3��4�ɕύX	2010.11.15 by sassa
#define CAPTURE_MAX_NB 10		// 4��10�ɕύX	2016.4.15 by oshima

// �����o�ϐ��ւ͌����I�ɒ��ڃA�N�Z�X���Ȃ�����
// �����I�ɖ��̕ύX��B�����s���\��������܂�
typedef struct {
	int nInitialized;									//<1.07b2.8> �������t���O�ǉ�  2010.05.06  by matsu
	int nBoardId;										// ���ID
	int nChannel;										// �J����channel		0..3
	int nName;											// �J��������			TI-124A/XCHR50/..
	int nSize;											// ��荞�݃T�C�Y		640x480/512x480/256x240/..
	int nMode;											// �J�������[�h			NORMAL/RANDOM
	int nSequence;										// ��荞�݃��[�h		CAPTURE_WAIT/CAPTURE_AUTO
	int nInitiator;										// �C�j�V�G�[�^���[�h	ON/OFF
	int nPlane;											// ��荞�݃v���[��		GRAY_PLANE/BIN_PLANE
	int nBinLevel;										// ��荞�݂Q�l���x��	0..255
	int nCaptureFlag;									// GetVideo���̃��[�h	F_SHUTTER|XCHR50|INITIATOR...
	int nShutter[CAPTURE_MAX_NB];						// �V���b�^�[�X�s�[�h
	int nInitiatorPage[CAPTURE_MAX_NB];					// �C�j�V�G�[�^�y�[�W	1..
	unsigned char *pInitiatorAddress[CAPTURE_MAX_NB];	//<1.07b1.4> �C�j�V�G�[�^�A�h���X(nInitiatorPage�Ɣr���g�p)		2008.06.21  by matsu
	int nGain;											// �J�����Q�C��(10�{�l)									2007.09.28  by matsu
	int nType;											//<1.07b1.9> �J�����^�C�v	R_CS_CAMERA_GRAY / R_CS_CAMERA_COLOR / R_CS_CAMERA_BAYER
	int nInitMode;										// �J�������������[�h	�J���������̏ꍇ�FVI_BOARD_INIT_ONLY	2010.11.01 oshima
	int	nVideoInitShutterMode;							// R_video_init
	int	nVideoInitOtherMode;							// R_video_init
} R_CS_CAMERA;

// --- prototype (public)---

int R_CS_CameraInitial(R_CS_CAMERA *camera);
int R_CS_CameraChange(R_CS_CAMERA *camera);
int R_CS_CameraCapture(R_CS_CAMERA *camera, int capture_nb, int mode);
int R_CS_CameraCaptureWait(R_CS_CAMERA *camera);
int R_CS_CameraCaptureEndCheck(R_CS_CAMERA *camera);							// �摜��荞�݊����I���m�F  �ǉ� 2008.01.08  by matsu
int R_CS_CameraShutterWait(R_CS_CAMERA *camera);

int R_CS_CameraDisplayImage(R_CS_CAMERA *camera, int capture_nb);
int R_CS_CameraChangeDisplayId(void);

int R_CS_CameraGetColorMode(void);
int R_CS_CameraSetColorMode(int mode);

int R_CS_CameraGetSize(R_CS_CAMERA *pCamera, int *pSizeX, int *pSizeY);

int R_CS_CameraSetGain(R_CS_CAMERA *camera, int nGain);									// �ǉ�  2007.09.28
int R_CS_CameraGetGainLimit(R_CS_CAMERA *camera, int *pGainMin, int *pGainMax);			// "

int R_CS_CameraSetInitiatorPage(R_CS_CAMERA *pCamera, int nCaptureNumber, int nPage);
int R_CS_CameraGetType(R_CS_CAMERA *pCamera);													//<1.07b1.9> �J�����^�C�v�̎擾

#define R_CS_CameraSetShutterSpeed(pCamera, nCaptureNumber, nShutterSpeed)		((pCamera)->nShutter[(nCaptureNumber)] = nShutterSpeed)		//<1.07b1.4> �ǉ�  2008.06.21
#define R_CS_CameraGetShutterSpeed(pCamera, nCaptureNumber)						((pCamera)->nShutter[(nCaptureNumber)])						//<1.07b1.4> �ǉ�  2008.06.21
//#define R_CS_CameraSetInitiatorPage(pCamera, nCaptureNumber, nPage)				((pCamera)->nInitiatorPage[(nCaptureNumber)] = nPage)		//<1.07b1.4> �ǉ�  2008.06.21
#define R_CS_CameraGetInitiatorPage(pCamera, nCaptureNumber)					((pCamera)->nInitiatorPage[(nCaptureNumber)])				//<1.07b1.4> �ǉ�  2008.06.21
#define R_CS_CameraSetInitiatorAddress(pCamera, nCaptureNumber, pAddress)		((pCamera)->pInitiatorAddress[(nCaptureNumber)] = pAddress)		//<1.07b1.4> �ǉ�  2008.06.21
#define R_CS_CameraGetInitiatorAddress(pCamera, nCaptureNumber)					((pCamera)->pInitiatorAddress[(nCaptureNumber)])				//<1.07b1.4> �ǉ�  2008.06.21
#define R_CS_CameraSetCamlinkSioInitMode( val )									(SetCamlinkSioInitMode=val)

#endif
