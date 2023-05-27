/******************************************************************************

	R_CS_SAV.C	

	�����摜�Z�[�u�֐��w�b�_
	�o�[�W�����A�b�v���́A�K�������̒ǉ����s���A
	�O���[�v���ŕύX���e���A�i�E���X���邱�ƁI�I

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	����
	Version
	2004.03.15.000
	2007.11.16.001	matsu		R_CS_SaveImgAutoSetDriveFreeSpaceCheck�ǉ�
	1.07b1.4	2008.08.08		R_CS_SaveImgAutoSetAddress()�ǉ�
	1.07b3.1	2011.05.17		R_CS_SaveImgAutoGetLast�n�ǉ�(�摜�����Z�[�u�ŏI��Ԏ擾)
	1.07b6.6	2015.06.09		R_CS_SaveImgAutoSetDirectoryName()�ǉ�
	1.07b8.0	2016.04.25		R_CS_SaveImgGetLastFileName()�ǉ�
	5.004		2018.05.21		ImagePath.csv�̃w�b�_�[���}�N����`
	5.004		2018.05.29		R_CS_SaveImgAutoGetRootDirName�֐��ǉ�
	5.019		2019.10.30		R_CS_SaveDataAutoGetRootDirName�֐��ǉ�

******************************************************************************/

#ifndef R_CS_SAV_H
#define R_CS_SAV_H

#ifdef __cplusplus
extern "C"{
#endif


// --- define ---
#include "R_cs.h"
#include <stdarg.h>

//�������t���O(HDD�ۑ�or�������ۑ�)
#define SAVE_IMG_OPEN_HDD    0x0000
#define SAVE_IMG_OPEN_MEMORY 0x0001
#define SAVE_IMG_MEM_64      0x0010
#define SAVE_IMG_MEM_128     0x0020
#define SAVE_IMG_MEM_256     0x0030

//�ۑ����[�h
#define SAVE_IMG_MAX			0x0000		//�ő喇���܂ŃZ�[�u������A�ۑ����Ȃ�
#define SAVE_IMG_RING			0x0001		//�ő喇���܂ŃZ�[�u������A�J�E���^��0�ɂ��ăZ�[�u���s
#define SAVE_IMG_RANDOM			0x0002		//<1.07b3.0> �ő喇���܂ŃZ�[�u������A�����_�����[�h�ŃZ�[�u���s

#define SAVE_IMG_FAST			0x0010		//�f�B�X�N�A�N�Z�X���x���Ȃ�O�Ƀf�B���N�g���؂�ւ����s��

#define SAVE_IMG_METHOD			0xFF00		//<1.07b3.0> �ۑ����@�}�X�N
#define SAVE_IMG_OFF			0x0000		//<1.07b3.0> �ۑ����Ȃ�
#define SAVE_IMG_ALL			0x0100		//<1.07b3.0> �S�摜�ۑ�
#define SAVE_IMG_LIST			0x0200		//<1.07b3.0> �w��ԍ��ۑ�
#define SAVE_IMG_NO_LIST		0x0300		//<1.07b3.0> �w��ԍ��ȊO�ۑ�
#define SAVE_IMG_DIR_ALL		0x0400		//<1.07b3.0> �S�摜���ޕۑ�
#define SAVE_IMG_DIR_LIST		0x0500		//<1.07b3.0> �w��ԍ����ޕۑ�
#define SAVE_IMG_DIR_NO_LIST	0x0600		//<1.07b3.0> �w��ԍ��ȊO���ޕۑ�


#define SAVE_IMG_INIMODE_DEFAULT		0x00000000
#define SAVE_IMG_INIMODE_NOMKDIR		0x00000001
#define SAVE_IMG_INIMODE_MKDIR_DATE_LOT	0x00000010		//�f�B���N�g�����ɓ�����LotNo�����郂�[�h 2018.03.29 by miyazaki

#define	SAVE_IMG_DIRECTORY_NAME_FORMAT	"%Y%m%d_%H%M%S"	// YYYYmmdd_HHMMSS
#define	SAVE_IMG_STRLEN_OF_DIRECTORY_NAME_FORMAT	15

#define SAVE_IMG_IMAGE_PATH_CSV_HEADER "Total,CapNo,NgCode,ImageName\n" //ImagePath.csv�̃w�b�_�[ 20180521 miyazaki

/* memo
	type						method					list
	�ۑ����Ȃ�					SAVE_IMG_OFF			-
	�S�摜�ۑ�					SAVE_IMG_ALL			-
	NG�摜�ۑ�					SAVE_IMG_NO_LIST		0
	OK�摜�ۑ�					SAVE_IMG_LIST			0
	NG�摜�ۑ�(���[�N��������)	SAVE_IMG_NO_LIST		0,1
	�S�摜�ۑ�(���[�N��������)	SAVE_IMG_NO_LIST		1
	�S�摜���ޕۑ�				SAVE_IMG_DIR_ALL,		-
	NG�摜���ޕۑ�				SAVE_IMG_DIR_NO_LIST	0
	�w��ԍ����ޕۑ�			SAVE_IMG_DIR_LIST		n1,n2,n3,...
*/

// --- prototype (public)---
int R_CS_SaveImgAutoExecute(int number, char *format, ...);
int R_CS_SaveImgAutoExecuteCode(int number, int code, const char *format, ...);	//<1.07b3.0> �摜�����Z�[�u���s(�m�f�R�[�h����L��)
int R_CS_SaveImgAutoExecuteCodeV(int number, int code, const char *format, va_list arglist);
int R_CS_SaveImgAutoExecuteCode_withList(const R_CS_COUNTER_DATA *monitor, const R_CS_RESULT_DATA *result, int number, int code, const char *format, ...);
int R_CS_SaveImgAutoExecuteCode_withListV(const R_CS_COUNTER_DATA *monitor, const R_CS_RESULT_DATA *result, int number, int code, const char *format, va_list arglist);
																				//<1.07b9.6> �摜�����Z�[�u���s(�m�f�R�[�h����L��,���X�g�ۑ�����)
int R_CS_SaveImgAutoInitial(void);
int R_CS_SaveImgAutoInitial_withMode(int mode);
int R_CS_SaveImgAutoMemoryExport(void);
int R_CS_SaveImgAutoSetDirName(char *dir_name);
int R_CS_SaveImgAutoSetImgDirName(char *dir_name);
int R_CS_SaveImgAutoSetDataDirName(char *dir_name);
int R_CS_SaveImgAutoSetFileName(int number, char *name, int page);
int R_CS_SaveImgAutoSetAddress(int number, unsigned char *pAddress);	//<1.07b1.4> �摜�Z�[�u����摜�A�h���X�̐ݒ�  2008.08.08
int R_CS_SaveImgAutoSetMaxNum(int max);
int R_CS_SaveImgAutoSetMode(int mode);
int R_CS_SaveImgAutoSetCodeList(int num, int *list);					//<1.07b3.0> �m�f�R�[�h���X�g�̐ݒ�
int R_CS_SaveImgAutoSetSkipNumber(int num);								//<1.07b3.0> �摜�ۑ��X�L�b�v��
int R_CS_SaveImgAutoSetNgCodeMax(int number);							//<1.07b3.8> �m�f�R�[�h�ő�ԍ��̐ݒ�	2011.10.27
int R_CS_SaveImgAutoGetNgCodeMax(void);									//<1.07b3.8> �m�f�R�[�h�ő�ԍ��̎擾	2011.10.27

int R_CS_SaveImgAutoOpen(int open_flag);
int R_CS_SaveImgAutoClose(void);

const char* R_CS_SaveImgAutoGetRootDirName(void);
char* R_CS_SaveImgAutoGetWorkDirName(void);
const char* R_CS_SaveDataAutoGetRootDirName(void);				// ver5.019
char* R_CS_SaveDataAutoGetWorkDirName(void);
int R_CS_SaveImgAutoGetSaveCount(int nNum);
int R_CS_SaveImgAutoSetDriveFreeSpaceCheck(int mode);			// �󂫗e�ʃ`�F�b�N(�f�t�H���g�F����)  2007.11.16

int R_CS_SaveImgAutoGetLastCode(void);									//<1.07b3.1> �摜�����Z�[�u�ŏI��Ԏ擾(�m�f�R�[�h)
int R_CS_SaveImgAutoGetLastNumber(void);								//<1.07b3.1> �摜�����Z�[�u�ŏI��Ԏ擾(�摜�t�@�C���ԍ�)
char* R_CS_SaveImgAutoGetLastDirName(void);								//<1.07b3.1> �摜�����Z�[�u�ŏI��Ԏ擾(�摜�ۑ��f�B���N�g��)

void	R_CS_SaveImgSetOverWriteEnable(int enable);						// 2015.05.29

int R_CS_SaveImgAutoSetDirectoryName(char *name);						//<1.07b6.6>�ۑ��f�B���N�g�����̐ݒ�
int R_CS_SaveImgAutoChangeDirectoryName(char *name, int digit);
const char* R_CS_SaveImgGetLastFileName(void);							//<1.07b8.0> �ŐV�̉摜�ۑ��t�@�C�����̎擾
void R_CS_SaveImgClearLastFileName(void);								//<1.07b8.0> �ŐV�̉摜�ۑ��t�@�C�����̃N���A

void R_CS_SaveImgResumeON(void);

int R_CS_SaveImgAutoCheckCode(int code);

int R_CS_MakeImagePathEmptyFile(void);

#ifdef __cplusplus
}
#endif

#endif
