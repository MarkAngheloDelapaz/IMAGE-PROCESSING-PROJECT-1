/******************************************************************************

	R_CS.H

	���ʂu�f�`���j���[�V�X�e��
	���C���w�b�_

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	����
	Version		Date		Name		Comment
				2004.07.11				�I���W�i��
	1.07		2008.01.07				�o�[�W�����\�����@�V�K�ǉ�
				2008.01.29				���j���[����ݒ�ǉ�
				2008.02.08				�ʐMBIT�A�R�}���h���w�b�_�Ɉړ�
	1.07b1.4	2008.08.08				R_CS_SetImageAutoSaveAddress()�ǉ�
				2008.09.26				�������[�h�� INSPECT_PFILE �ǉ�
				2008.10.03				R_CS_SetResultMonitorDispTotalNum()�AR_CS_SetResultMonitorDispArea�ǉ�
	1.07b1.5	2008.12.9				R_CS_SetFunctionManualInspectFileAllReady(),
										R_CS_SetFunctionManualInspectFileAllClosing(),
										R_CS_SetFunctionCurrentCameraChange(),
										R_CS_SetPluralInspectionMode(),
										R_CS_SetPluralInspectionCount(),
										R_CS_GetPluralInspectionCount(),
										R_CS_SetFunctionThroughImageReady(),
										R_CS_SetFunctionThroughImageClosing()�̒ǉ�
										�S�摜�ۑ�(ܰ������ȊO)�Ή�
				2009.01.07				R_CS_VER_SOFTNAME(���s�t�@�C�����\��)�ǉ�
				2009.05.08				R_CS_SetFunctionThroughUser(),
										R_CS_SetMenuColor(),R_CS_GetMenuColor(),
										R_CS_DispCounterSub(),
										R_CS_SetDirectlyInspectionName(),
										R_CS_SetDirectlyImageName�̒ǉ�
	1.07b1.6	2009.05.27	saito		�ʐM���@�iR_CS_SetCommunicationMode()�Őݒ�j�̒ǉ�
	1.07b2.0	2009.07.27	oshima		R_CS_GetPfileResultMonitorNumber,R_CS_SetPfileResultMonitorNumber�̒ǉ�
										R_CS_GetResultSaveMode,R_CS_SetCounterFileItem�̒ǉ�
	1.07b2.2	2009.08.12	matsu		R_CS_SetMenuUserAnalysis�ǉ�
				2009.09.15	matsu		�������[�h�ǉ�(�}�j���A��������)�@INSPECT_CAPTURE�AINSPECT_GMD�AINSPECT_BMP
				2009.09.30	matsu		�ȉ��̊֐��ǉ�
											R_CS_ChangeLogMode()
											R_CS_CheckChangeParameter()
											R_CS_SetChangeLogFileSystem()
											R_CS_SetChangeLogFilePackage()
											R_CS_SetChangeLogFileType()
	1.07b2.4	2009.11.05	oshima		�摜��͑O�テ�[�U�ݒ�֐��ǉ�
											R_CS_SetFunctionAnalysisReady()
											R_CS_SetFunctionAnalysisClose()
	1.07b2.7	2010.02.19	matsu		R_winb.h��include�ɒǉ�
	1.07b2.9	2010.06.04	oshima		R_CS_SetCommentBufferForInspectManual()�̒ǉ�
				2010.08.26	oshima		R_CS_SetFunctionUserClearLast()�̒ǉ�
				2010.09.03	matsu		R_CS_RESULT_DATA�̃R�����g�T�C�Y�ύX(R_pfile�Ɠ��T�C�Y)
	1.07b3.7	2011.10.21	sassa		NG���ʕ\���R�����g��16->64�����ɕύX
	1.07b4.2	2012.05.16	oshima		R_CS_GetCounterDispMode()�֐��ǉ�
	1.07b4.4	2012.08.23	araki		R_CS_SetFunctionUserCommandTransmit�֐��ǉ�
	1.07b4.4	2012.08.28	araki		R_CS_AutoModeCommandRecheckDisable�֐��ǉ�
	1.07b4.8	2013.04.09	araki		�ʐM���@��R_CS_COMMUNICATION_COMMAND_DPRAM��ǉ�
	1.07b5.3	2013.11.03	oshima		INSPECT_OTHER,INSPECT_CMD�̒ǉ�
	1.07b6.1	2014.12.11	oshima		R_CS_GetResultMonitorHandle,R_CS_GetPfileResult�̒ǉ�
	5.019		2019.10.30	kitamura	R_CS_Get�n�֐������ǉ�
******************************************************************************/
#ifndef __R_CS_H__
#define __R_CS_H__

#include "R_winb.h"			//<1.07b2.7>  2010.02.19  by mastu

#include "R_cs_cam.h"
#include "R_cs_res.h"
#include "R_cs_cnt.h"
#include "R_cs_io.h"		// @m
#include "R_data.h"
#include "R_cs_cap.h"		// 2007.09.11 @m
#include "R_cs_lan.h"

//#include "RCsInspect/RCsInspect.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- define ---
//�O���[�v���[���ԍ�
#define GPLANE_BASE1  4
#define GPLANE_BASE2  5
#define GPLANE_BASE3  6

// �o�[�W�����\�����@  �V�K�ǉ� Ver.1.07 2008.01.07  by matsu
#define R_CS_VER_TIME1		0					// �^�C���X�^���v�\��("[yyyy.mm.dd HH:MM:SS]")
#define R_CS_VER_TIME2		1					// �^�C���X�^���v�\���Q("VersionHH.MM")<�f�t�H���g>
#define R_CS_VER_USER		2					// ���[�U�w��
#define R_CS_VER_SOFTNAME	3					//<1.07b1.5> ���s�t�@�C����(�g���q������)


//�ʐM���@		//1.07b1.6 2009.5.25 saito  10�ȍ~�ǉ�			//�ʐM���@		�{�[�h
#define R_CS_COMMUNICATION_CAPTURE_BOARD_IO				0x00	//BIT			CAPTURE_BOARD
#define R_CS_COMMUNICATION_COMMAND						0x01	//COMMAND		DLIO
#define R_CS_COMMUNICATION_USER							0x02	//���[�U�[��`	���[�U�[��`
#define R_CS_COMMUNICATION_LED_BOARD_IO					0x03	//BIT			GPIO

#define R_CS_COMMUNICATION_BIT_CAPTURE					0x10	//BIT			CAPTURE_BOARD	R_CS_COMMUNICATION_CAPTURE_BOARD_IO	�Ɠ���
#define R_CS_COMMUNICATION_BIT_GPIO						0x11	//BIT			GPIO			R_CS_COMMUNICATION_LED_BOARD_IO		�Ɠ���
#define R_CS_COMMUNICATION_BIT_DLIO						0x12	//BIT			DLIO			Reserved(���Ή��j
#define R_CS_COMMUNICATION_COMMAND_GPIO					0x13	//COMMAND		GPIO			Reserved(���Ή��j
#define R_CS_COMMUNICATION_COMMAND_DLIO					0x14	//COMMAND		DLIO			R_CS_COMMUNICATION_COMMAND			�Ɠ���
#define R_CS_COMMUNICATION_COMMAND_FAST_DLIO_MASTER		0x15	//COMMAND_FAST	DLIO			R_dlio_com()�g�p�@MASTER�ݒ�
#define R_CS_COMMUNICATION_COMMAND_FAST_DLIO_SLAVE		0x16	//COMMAND_FAST	DLIO			R_dlio_com()�g�p�@SLAVE�ݒ�
#define R_CS_COMMUNICATION_COMMAND_DPRAM				0x17	//COMMAND		DPRAM			CP-CZ-47 DP-RAM�ʐM



// LED�{�[�h�pInputBit���t(R_CS_COMMUNICATION_LED_BOARD_IO�ݒ莞)
#define	R_CS_INPUT_START			0x01
#define	R_CS_INPUT_COUNTER_CLEAR	0x02
#define	R_CS_INPUT_LOT_INITIAL		0x10

// �R�}���h�ʐM���R�}���h���t(R_CS_COMMUNICATION_COMMAND�ݒ莞)
#define R_CS_CMD_INSPECT_START	'S'
#define R_CS_CMD_COUNTER_CLEAR	'C'
#define R_CS_CMD_LOT_INITIAL	'L'
#define R_CS_CMD_RESULT_SEND	'R'


//�J�E���^�\�����[�h
#define COUNTER_DISP_TOTAL 0
#define COUNTER_DISP_ALL   1
#define COUNTER_DISP_NODISP 2

//���ʕ\�����[�h
#define RES_DISP_NOTHING 0
#define RES_DISP_DETAIL  1
#define RES_DISP_GRAPH   2

//NG�R�[�h
#define NG_NO_READY			-1		//���������s
#define NG_NO_WORK			 1		//���[�N�Ȃ�
#define NG_INSP				99		//�F���G���[
#define NG_CAPTURE			98		//�摜��荞�݃G���[
#define NG_LIGHTING			97		//�Ɩ��G���[

//�J�E���^�[�ő吔
#define COUNTER_MAX_NUMBER  8

//�J�����ő吔
#define CAMERA_MAX_NUMBER   4

//�������[�h(�r�b�g��`)
#define INSPECT_AUTO			0x00000001		// �I�[�g�ғ�
#define INSPECT_MANUAL			0x00000002		// �}�j���A���ғ�
#define INSPECT_STEP			0x00000004		// �X�e�b�v����
#define INSPECT_PFILE			0x00000008		//<1.07b1.4> �t�@�C���A���F��

//(�����摜���ǂ��ɂ��邩������)
#define INSPECT_TARGET_MASK		0x000F0000		//<1.07b2.2> �Ώۉ摜�}�X�N
#define INSPECT_CAPTURE			0x00010000		//<1.07b2.2> ��荞�݉摜	(�}�j���A���F�����ɐݒ�)
#define INSPECT_GMD				0x00020000		//<1.07b2.2> GMD�摜		(�}�j���A���F�����ɐݒ�)
#define INSPECT_BMP				0x00040000		//<1.07b2.2> BMP�摜		(�}�j���A���F�����ɐݒ�)
#define INSPECT_OTHER			0x00080000		//<1.07b5.3> OTHER�t�@�C��	(�}�j���A���F�����ɐݒ�)
#define INSPECT_CMD				0x00100000		//<1.07b5.4> CMD�摜		(�}�j���A���F�����ɐݒ�)


//�f�o�b�O���[�h
#define DEBUG_NO_COMMUNICATION	0x0001		// I/O�ʐM�Ȃ�
#define DEBUG_NO_VERSION_CHECK	0x1000		// �o�[�W�����������`�F�b�N�����Ȃ�  2008.01.29


//�摜�Z�[�u���[�h
#define IMAGE_SAVE_OFF					0
#define IMAGE_SAVE_ALL					1
#define IMAGE_SAVE_NG					2
#define IMAGE_SAVE_GO					3
#define IMAGE_SAVE_NG_WITHOUT_NOWORK	4
#define IMAGE_SAVE_ALL_WITHOUT_NOWORK	5	//�S�摜�ۑ�(ܰ������ȊO) 2008.12.9

//�I�[�g���[�h�����R�[�h
#define R_CS_CODE_NO_SIGNAL			0
#define R_CS_CODE_INSPECT_START		1
#define R_CS_CODE_COUNTER_CLEAR		2
#define R_CS_CODE_LOT_INITIAL		3
#define R_CS_CODE_RESULT_SEND		4
#define R_CS_CODE_ELSE				255

//�J�E���^�e�[�u���ԍ�
#define RESULT_TOTAL_NUMBER 		6
#define RESULT_MANU_NUMBER  		7

//���[�U�ݒ胁�j���[��
#define R_CS_USER_MENU_NB   		8

//���j���[����ݒ�  2008.01.29
#define R_CS_MENU_NORMAL		0			// �f�t�H���g���[�h
#define R_CS_MENU_ERROR_EXIT	1			// ���j���[�֐��ŃG���[������R_CS�I��
#define R_CS_MENU_EXIT_DISABLE	2			// ���C�����j���[�ŏI���\����OFF�ɂ���

//4�����\���摜������
#define R_CS_GPLANE_DIVIDE			3
#define R_CS_BINPLANE_DIVIDE		3

// ��ʕ\���ʒu
#define R_CS_WINDOW_POSITION0		0		// ����
#define R_CS_WINDOW_POSITION1		1		// �E��
#define R_CS_WINDOW_POSITION2		2		// ����
#define R_CS_WINDOW_POSITION3		3		// �E��

//R_CS_AfterInspection�p��`
#define NO_COUNT_UP_DETAIL			0x010
#define NO_COUNT_UP_TOTAL			0x020
#define NO_COUNT_UP_PRODUCT			0x040
#define NO_COUNT_UP_RESULT_MONITOR	0x080
#define NO_IMAGE_SAVE				0x100
#define NO_DISP_COUNTER				0x200
#define NO_DISP_GRAPH				0x400
#define NO_DISP_RESULT				0x800
#define	OVER_WRITE_IMAGE_FILE		0x1000

//�I�[�g�E�}�j���A�����[�h�ؑ֎��p��`
#define R_CS_CANNOT_AUTO_MODE_CLOSING	1
#define R_CS_CANNOT_AUTO_MODE_READY		1

//�ۑ��f�B���N�g���ǔԐݒ�p��`
#define R_CS_IMGDIR_ADDNO_NOADD		-1							//!< <1.07b9.6>	�ǔԂȂ�
#define R_CS_IMGDIR_ADDNO_DEFAULT	2							//!< <1.07b9.6>	�f�t�H���g�͒ǔ�2��


typedef struct {
	int nCounterColumn;				//�J�E���^�\����
	int nCounterLine;				//�J�E���^�\���s
	int nInfoColumn;				//�V�X�e�����\����
	int nInfoLine;					//�V�X�e�����\���s
	int nJudgeColumn;				//���茋�ʕ\����
	int nJudgeLine;					//���茋�ʕ\���s
	int nSizeColumn;				//�ő�\����
	int nSizeLine;					//�ő�\���s��
	int nLotInfoColumn;				//���b�g���\����
	int nLotInfoLine;				//���b�g���\���s
	int nPfileInfoLine;				//PFILE���\���s
	int nPfileInfoColumn;			//PFILE���\����
	int nLanInfoColumn;				//Lan���\����
	int nLanInfoLine;				//Lan���\���s
} DRAW_ITEM;

#include "R_pfile.h"
typedef struct {
	int		nCode;										//���ʃt���O(0��GO�A1�ȏオNG�R�[�h)
	char	sNgComment[64];								//NG���ʕ\���R�����g		16->64�����ɕύX	2011.10.21 by sassa
	char	sImageComment[PFILE_SEQ_PATH_NAME_MAX];		//�摜�t�@�C���ɖ��ߍ��ރR�����g
														//<1.07b2.9> �R�����g�T�C�Y�ύX 128 -> PFILE_SEQ_PATH_NAME_MAX(=256)  2010.09.03  by matsu
}R_CS_RESULT_DATA;


// --- variable ---
//R_CS.C�̒��Ɏ��̂�����
//extern R_CS_CAMERA 	R_CS_CameraPara[];
//extern R_CS_CAMERA 	*R_CS_CameraPara;
extern R_CS_RESULT_MONITOR_ITEM *R_CS_ResultMonitorItem[];		//�������ʊǗ��A�C�e��
extern R_CS_RESULT_DATA R_CS_Result[];							//�������ʃf�[�^
extern R_CS_COUNTER_DATA R_CS_Counter[];						//�����J�E���^
extern DRAW_ITEM R_CS_DrawItem;									//��ʕ`��A�C�e���\����
extern int R_CS_ResultDispMode;									//�������ʕ\�����[�h
extern int R_CS_ResultMonitorNumMax;							//�������ʊǗ��f�[�^�ۑ��ő吔
extern int R_CS_FlagDispVersion;								//�o�[�W�����\��ON/OFF�t���O(ON�Ȃ�o�[�W�������AOFF�Ȃ�^�C���X�^���v��\��)
																// ��R_CS_SetDispVersionMode()�Őݒ肷�邱�� @m 2007.09.04
extern int R_CS_FlagInitialTime;								//���ԏ�����ON/OFF�t���O(ON�Ȃ�\�t�g�N�����Ɏ��Ԑݒ���s��)
																// ��R_CS_InitialTime()�Őݒ肷�邱��  2006.09.08
extern int R_CS_FlagInitialCamera;								//�J����������ON/OFF�t���O(ON�Ȃ�\�t�g�N�����Ɏ��Ԑݒ���s��)

extern R_DATAFILE_ITEM *_R_CS_User_ParaFileItemSys;				// �@��f�[�^�t�@�C��  �����ϐ���  2006.09.12
extern R_DATAFILE_ITEM *_R_CS_User_ParaFileItemPkg;				// �@��f�[�^�t�@�C��
extern R_DATAFILE_ITEM *_R_CS_User_ParaFileItemTyp;				// �@��f�[�^�t�@�C���i�`�b�v�Ȃǁj2007.08.30 saito
extern char *_R_CS_User_ParaFileNameSys;						// �@��f�[�^�t�@�C����
extern char *_R_CS_User_ParaFileNamePkg;						// �@��f�[�^�t�@�C����
extern char *_R_CS_User_ParaFileNameTyp;						// �@��f�[�^�t�@�C�����i�`�b�v�Ȃǁj2007.08.30 saito


//���ʃt�@�C���������[��
#define RESULT_FILENAME_TYPE_SERIAL	1
#define RESULT_FILENAME_TYPE_PLANE	0
int R_CS_GetResultFileNameType();								//!<	<1.07b9.6>	<br> ���ʃt�@�C�����̖������[��	<br> 1�̂Ƃ��̓��U���g�t�@�C�����Ƀ��b�gNO��t������ <br> 0�̂Ƃ��̓��[�U�ݒ薼�̂܂ܕۑ�����

//���[�U�[�֐��Ɏ��̂�����
//extern R_CS_COUNTER_CLASS R_CS_User_NgClass[];				//NG�N���X
//extern R_CS_COUNTER_NG_CODE R_CS_User_NgCode[];				//NG�R�[�h
//extern R_DATAFILE_ITEM R_CS_User_ParaFileItemSys[];			//�@��f�[�^�t�@�C��
//extern R_DATAFILE_ITEM R_CS_User_ParaFileItemPkg[];			//�@��f�[�^�t�@�C��
//extern char R_CS_User_ParaFileNameSys[];						// �@��f�[�^�t�@�C����
//extern char R_CS_User_ParaFileNamePkg[];						// �@��f�[�^�t�@�C����
//extern char R_CS_User_ResultMonitorFileName[8][16];				//�������ʕۑ��t�@�C����

// --- prototype ---
int R_CS( int argc, char *argv[] );

int R_CS_ImageSave(int number);
int R_CS_ImageSavePluralInspection(int number, int total_number);
int R_CS_ImageSaveLastStatus(void);										//<1.07b3.6> �摜�ۑ���Ԃ̎擾

int R_CS_SetInspectionInfo(int camera_nb,int posi_nb,int inspect_nb,int capture_nb,int result_nb,int cycle_nb,char *name,char *p_name);
int R_CS_SetPackageName(char *pName);
int R_CS_SetTypeName(char *pName);

//int R_CS_Capture(int camera_nb, int capture_nb, int mode);			R_CS_CAP.H�Ɉړ�
//int R_CS_CaptureWait(int camera_nb);									R_CS_CAP.H�Ɉړ�
//int R_CS_CaptureMulti( int *camera_nb, int *capture_nb, int mode);	R_CS_CAP.H�Ɉړ�
//int R_CS_CaptureMultiWait(int *camera_nb);							R_CS_CAP.H�Ɉړ�

int R_CS_GrayMemoryDivideDisp( int src, int dst, int number );
int R_CS_SetInspectionName(int num, char* j_name, char* e_name);

int R_CS_SetShutterSpeed(int camera_nb,int capture_nb,int speed);
int R_CS_GetShutterSpeed(int camera_nb,int capture_nb);

void R_CS_CounterAllClear(void);
int  R_CS_SetImagePara(int camera_nb,int posi_nb,int capture_nb,int addresss,char *fname);
int  R_CS_SetImagePara_MultiInsp(int camera_nb, int posi_nb, int inspect_nb, int capture_nb, int address, char *fname);
int  R_CS_SetImageName(int camera_nb, int posi_nb, int capture_nb, char *name_j, char *name_e);				// �捞�݉摜���̂̐ݒ�  2008.01.08  by matsu
int  R_CS_SetImageAutoSaveAddress(int camera_nb, int posi_nb, int capture_nb, unsigned char *pAddress);		//<1.07b1.4> �����摜�ۑ��A�h���X�̐ݒ�
int  R_CS_GetCounterDispMode(void);
void R_CS_DispCounter(void);
//int  R_CS_CaptureCameraChange(int camera_nb);							R_CS_CAP.H�Ɉړ�
void R_CS_DispInspectTime(int t1, int t2, int t3, int t4 );
void R_CS_DispMenuUserSetting();
int  R_CS_SetMenuUserSetting(struct winb_item_t* item);
int  R_CS_SetMenuUserAnalysis(struct winb_item_t* item);					//<1.07b2.2> ���[�U�摜��̓��j���[�̐ݒ�
int  R_CS_SetMenuUserFile(struct winb_item_t* item);
void R_CS_DispTitle(int mode);
int  R_CS_AfterInspection(int number,int mode);
int  R_CS_AutoModeEndCheck(int code);
int  R_CS_GetCommunicationMode(void);
int  R_CS_SetCommunicationMode(int mode,int board_id);
void R_CS_SetVideoMemory(int gray_number,int bin_number);
void R_CS_DispResult(int number, int mode);
void R_CS_DispGraph(int number);
int  R_CS_BinMemoryDivideDisp( int src, int dst, int number );
int  R_CS_GetImageSaveMode(void);
int  R_CS_SetCameraPara(int camera_type, int video_size);

int R_CS_InformationMode(int mode);											// @m 2006.09.08
#define R_CS_InitialTime( val )		(R_CS_FlagInitialTime = val)			// @m ���ԏ��������邩��ݒ�  2006.09.08
#define R_CS_InitialCamera( val )	(R_CS_FlagInitialCamera = val)			// �J�������������邩��ݒ�  2008.12.9 oshima

int R_CS_SetParameterFileSystem(R_DATAFILE_ITEM *pList, char *pFileName);	// @m 2006.09.12
int R_CS_SetParameterFilePackage(R_DATAFILE_ITEM *pList, char *pFileName);	// @m 2006.09.12
int R_CS_SetParameterFileType(R_DATAFILE_ITEM *pList, char *pFileName);		// @sai 2007.08.22
int R_CS_SetLimitFileName(char *pFileName, int nNum);						// 2019.10.31 kitamura

int R_CS_SetToolConfigFileName(char *pFileName);							// @m 2006.09.13
//int R_CS_SetResultMonitorFileName(char **pFileNameList);					// @m 2006.09.13
int R_CS_SetResultMonitorItem(int nNum, char *sFileName, R_CS_RESULT_MONITOR_ITEM *pList);	// @m 2007.01.17
int R_CS_SetResultMonitorDispTotalNum(int nTotalNumber);					// @s 2008.10.03
int R_CS_SetResultMonitorDispArea(int nGraphNum, R_RECT tDispArea);			// @s 2008.10.03

int R_CS_GetParameterFileSystem(R_DATAFILE_ITEM **pList, char **pFileName);	//<1.07b2.8> �`���ύX  2010.04.12  by matsu
int R_CS_GetParameterFilePackage(R_DATAFILE_ITEM **pList, char **pFileName);
int R_CS_GetParameterFileType(R_DATAFILE_ITEM **pList, char **pFileName);	// @sai 2007.08.30
char* R_CS_GetToolConfigFileName(void);										// 2007.11.16 �`���ύX

int R_CS_SetFunctionUserInitial(int (*pFunc)(void));						// @m 2006.11.16
int R_CS_SetFunctionUserInitialLast(int (*pFunc)(void));					// @m 2007.02.13
int R_CS_SetFunctionManualCapture(int (*pFunc)(void));						// @m 2006.09.23
int R_CS_SetFunctionManualInspectCapture(void (*pFunc)(void));				// @m "
int R_CS_SetFunctionManualInspectRepeat(void (*pFunc)(void));				// @m "
int R_CS_SetFunctionCaptureThrough(void (*pFunc)(int));						// @m "
int R_CS_SetFunctionUserClear(void (*pFunc)(void));							// @m "
int R_CS_SetFunctionUserClearLast(void (*pFunc)(void));						// <1.07b2.9> 2010.08.26 oshima
int R_CS_SetFunctionAutoModeReady(int (*pFunc)(void));						// @m 2006.11.16
int R_CS_SetFunctionAutoModeClosing(int (*pFunc)(void));					// @m "
int R_CS_SetFunctionAutoModeExecute(int (*pFunc)(int));						// @m "
int R_CS_SetFunctionUserCommunication(int (*pFunc)(void));					// @m "
int R_CS_SetFunctionUserCommunicationEndCheck(int (*pFunc)(void));			// @m "
int R_CS_SetFunctionInspect(int (*pFunc)(int mode,int number,R_CS_RESULT_DATA *Result));
int R_CS_SetFunctionUserCommandTransmit(int (*pFunc)(char*));
int R_CS_SetFunctionUserLanExecute(int (*pFunc)(int));

int R_CS_SetCounterNgCode(R_CS_COUNTER_NG_CODE *pNgCodeList);
int R_CS_SetCounterNgClass(R_CS_COUNTER_CLASS *pNgClassList);
R_CS_COUNTER_DATA *R_CS_GetCounterDataStruct(int nNumber);
int R_CS_SetFunctionDisplayCounter(int (*pFunc)(int));

int R_CS_SetDispJudgePosition(int nColumn, int nLine);						// @m 2006.10.26
int R_CS_SetDispCounterPosition(int nColumn, int nLine);					// @m 2006.11.07
int R_CS_SetDispLotInfoPosition(int nColumn, int nLine);					// @m 2006.11.30
int R_CS_SetDispInfoPosition(int nColumn, int nLine);						// @m 2006.10.07
int R_CS_SetDispPfilePosition(int nColumn, int nLine);						//<1.07b1.4>  2008.09.26

int R_CS_SetDispCounterCategoryMode(int nMode);								// @m 2006.11.07

int R_CS_SetInspectMode(int nMode);											// @m 2007.02.07
int R_CS_GetInspectMode(void);												// @m 2007.02.07
int R_CS_GetInspectManualMode(void);										//<1.07b2.2> �}�j���A���������[�h�̎擾
int R_CS_SetDebugMode(int nMode);											// @m 2007.02.07
int R_CS_GetDebugMode(void);												// @m 2007.02.07

int R_CS_SetDispVersionMode(int nMode);										// �o�[�W�����\�����[�h��ݒ�  @m 2007.09.04
int R_CS_GetDispVersionMode(void);											// �o�[�W�����\�����[�h���擾  @m 2007.09.04
int R_CS_SetDispVersionString(char *pString);								// �o�[�W�����\��������̐ݒ�  2008.01.07  by matsu

void R_CS_CallInspectSequentialFileSetting(void);

int R_CS_SetDispDefaultZoom(int nZoom);
int R_CS_GetDispDefaultZoom(void);
int R_CS_SetDispThroughZoom(int nZoom);
int R_CS_GetDispThroughZoom(void);
int R_CS_SetCameraCheckMode(int nMode);
int R_CS_GetCameraCheckMode(void);

int R_CS_SetDisplayWindow(int nWindowNo, int nPosition, int nCameraNumber, int nZoomMode);
int R_CS_SetMenuMode(int nMode);
int R_CS_GetMenuMode(void);

int R_CS_GetCurrentCameraNumber(void);
int R_CS_SetCurrentCameraNumber(int nCameraNumber);				//<1.07b5.7>	2014/09/04	komatsu

int R_CS_GetCurrentInspectionNumber(void);
int R_CS_SetCurrentInspectionNumber(int nInspNumber);			//2017.04.17	komatsu

int R_CS_GetCurrentOverallNumber(void);							//2020.01.16	komatsu
int R_CS_SetCurrentOverallNumber(int nInspNumber);							//2020.01.16	komatsu

int R_CS_SetTimerCaptureThrough(int time);						// �ǉ�2007.11.05  by matsu
int R_CS_GetTimerCaptureThrough(void);						// "

int R_CS_SetNetworkPcName(char *pPcName);						// 2007.11.16�ǉ�
int R_CS_SetNetworkDirName(char *pDirName);						// 2007.11.16�ǉ�
char* R_CS_GetNetworkPcName(void);								// 2007.11.16�ǉ�
char* R_CS_GetNetworkDirName(void);								// 2007.11.16�ǉ�

int R_CS_SetFunctionManualInspectFileAllReady(int (*pFunc)(int));		//2008.12.9
int R_CS_SetFunctionManualInspectFileAllClosing(int (*pFunc)(int));		//2008.12.9
int R_CS_SetFunctionCurrentCameraChange(int (*pFunc)(void));
int R_CS_SetPluralInspectionMode(int nMode);
void R_CS_SetPluralInspectionCount(int nCount);
int R_CS_GetPluralInspectionCount(void);
int R_CS_PositionChange(int nNum);
int R_CS_SetFunctionThroughImageReady(int (*pFunc)(void));
int R_CS_SetFunctionThroughImageClosing(int (*pFunc)(void));

int R_CS_SetFunctionThroughImageUser(int (*pFunc)(void));					//2009.5.8
int R_CS_SetMenuColor(int color);											//2009.5.8
int R_CS_GetMenuColor(void);												//2009.5.8
void R_CS_DispCounterSub(int nPosiX, int nPosiY, int nNumber);				//2009.5.8
int R_CS_SetDirectlyInspectionName(int num, char* j_name, char* e_name);	//2009.5.8
int R_CS_SetDirectlyImageName(int image_nb, int address, char *fname);		//2009.5.8
int R_CS_SetPfileResultMonitorNumber(int num);								//2009.7.27
int R_CS_GetPfileResultMonitorNumber(void);									//2009.7.27
int R_CS_GetResultSaveMode(void);											//2009.7.27
int R_CS_SetCounterFileItem(char *pStrName,char *pStrVal);					//2009.7.27

int R_CS_GetImgDirAddNoDigit(void);

int R_CS_ChangeLogMode(int nMode);											//<1.07b2.2> �p�����[�^�t�@�C���ύX�����o�̓��[�h�ݒ�
int R_CS_CheckChangeParameter(void);										//<1.07b2.2> �p�����[�^�t�@�C���ύX�`�F�b�N
int R_CS_SetChangeLogFileSystem(char *pFileName);							//<1.07b2.2> �V�X�e���p�����[�^���O�t�@�C���̐ݒ�(�f�t�H���g:"log_sys.csv")
int R_CS_SetChangeLogFilePackage(char *pFileName);							//<1.07b2.2> �@��f�[�^(�p�b�P�[�W�f�[�^)���O�t�@�C���̐ݒ�(�f�t�H���g:"log_pkg.csv")
int R_CS_SetChangeLogFileType(char *pFileName);								//<1.07b2.2> �@��f�[�^(�`�b�v�f�[�^)���O�t�@�C���̐ݒ�(�f�t�H���g:"log_typ.csv")
int R_CS_SetChangeLogFileLim(char *pFileName, int nNum);						// 2019.12.4 kitamura lim�t�@�C�����O�t�@�C���̐ݒ�(�f�t�H���g:"log_lim0.csv�`log_lim3.csv")

int R_CS_SetFunctionAnalysisReady(int (*pFunc)(void));						//<1.07b2.4> 2009.11.5
int R_CS_SetFunctionAnalysisClose(int (*pFunc)(void));						//<1.07b2.4> 2009.11.5

int R_CS_SetCommentBufferForInspectManual(char *buff,int size);				//<1.07b2.9> 2010.06.04

int R_CS_WriteLog(char *format, ... );										//<1.07b3.5> ���O�f�[�^�t�@�C���o��				2011.07.11
int R_CS_WriteLogMode(int mode);											//<1.07b3.5> ���O�f�[�^�t�@�C���o�̓��[�h�ݒ�	2011.07.11

int R_CS_SetNgCodeMax(int number);											//<1.07b3.8> �m�f�R�[�h�ő�ԍ��̐ݒ�	2011.10.24
int R_CS_GetNgCodeMax(void);												//<1.07b3.8> �m�f�R�[�h�ő�ԍ��̎擾	2011.10.24

void	R_CS_AutoModeCommandRecheckDisable(void);							//<1.07b4.4>

// �V�X�e���֐�
//int r_cs_change_zoom(int mode);

//���[�U�[�֐��̒��Ɏ��̂�����
//void R_CS_User_ManualCaptureInspect(void);				@m 2006.09.23�폜
//int R_CS_User_Communication(void);
//void R_CS_User_Clear(void);
//int R_CS_User_Inspect(int mode,int number,R_CS_RESULT_DATA *Result);
//int R_CS_User_AutoModeReady(void);
//int R_CS_User_AutoModeClosing(void);
//int R_CS_User_AutoModeExecute(int code);
//int R_CS_User_CommunicationEndCheck(void);
//void R_CS_User_ManualInspectRepeat(void);					@m 2006.09.23�폜
//void R_CS_User_CaptureImage(int capture_num);				@m 2006.09.23�폜
//int R_CS_User_Initial(void);

int r_CS_Check9301libVersion(int nMajor, int nMinor, int nBeta);		//<1.07b1.5>
int r_CS_isInformationMode(void);										//<1.07b2.7>  2010.02.20  by matsu

int r_CS_UserCommandTransmit(char* command);

int R_CS_GetPFileFileNubmer(void);										//<1.07b5.7>	2014/09/04	komatsu
int	R_CS_SetPFileFileType(int type);									//<1.07b8.3>	2016.05.23	araki

int R_CS_GetResultMonitorHandle(int number);							//<1.073.4>		2014.12.11	oshima
int R_CS_GetPfileResult(void);											//<1.073.4>		2014.12.11	oshima

int R_CS_SetParaInspectFileUser(struct winb_para_tm *user);
void R_CS_DelParaInspectFileUser();

int R_CS_Lan_SetLanInfo(const R_CS_LANINFO *info);						// ���[�U�v���O��������̃��b�g�ʐM�ݒ苭����������
R_CS_LAN_HANDLE R_CS_Lan_GetHandle();									// Lan�ʐMSocekt�n���h�����擾

int R_CS_ProductSetLotNo(char *LotNo);

int R_CS_SetSoftVer(char *ver);

int R_CS_GetLotSerial(char *target);

int R_CS_SetImageSaveDirName(char *path);
int R_CS_SetImageLoadDirName(char *path);
int R_CS_SetImageGoChkDirName(char *path);
int R_CS_SetImageNGChkDirName(char *path);
int R_CS_SetImageManualDirName(char *path);
int R_CS_SetResultSaveDirName(char *path);

//ver5.019
const char* R_CS_GetImageSaveDirName(void);
char* R_CS_GetImageLoadDirName(void);
char* R_CS_GetImageGoChkDirName(void);
char* R_CS_GetImageNGChkDirName(void);
char* R_CS_GetImageManualDirName(void);
const char* R_CS_GetResultSaveDirName(void);
char* R_CS_GetImageSaveDirNameFull(void);
char* R_CS_GetResultSaveDirNameFull(void);

int	R_CS_ImageSaveDirName_Is_DateAndLotNumber(void);

void R_CS_GetDisplaySize(int *disp_x, int *disp_y);

//void R_CS_InspectSetHandle(R_CS_INSPECT_HANDLE handle, int cam_no);

#ifdef __cplusplus
}
#endif

#endif
