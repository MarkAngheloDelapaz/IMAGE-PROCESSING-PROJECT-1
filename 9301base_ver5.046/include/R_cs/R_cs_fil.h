/******************************************************************************
	
	R_CS_FIL.H

	�t�@�C������֐��w�b�_
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
	2004.04.07.000		�I���W�i��
	2004.04.21.001		DrawPlane�`��֐�(R_draw�n)��g�ݍ���
	2004.08.30.002		�f�B���N�g���R�s�[��ǉ�
	2007.11.20.003		�l�b�g���[�N�ڑ��֘A�֐��ǉ�
	2007.11.22.004		�f�B���N�g���폜���j���[�A�֘A�֐��ǉ�
	1.07b1.4	2008.08.07	�ȉ��̊֐���ǉ�
								R_CS_SaveGmd()
								R_CS_SaveGmdSetComment()
				2008.10.03	�f�B���N�g���폜���j���[�ɑI���폜�ǉ�
	1.07b1.5	2008.12.09	R_CS_LoadGmdWithComment�ǉ�
	1.07b2.3	2009.11.05	�ȉ��̊֐����ύX(�����̂̓}�N����)
								R_CS_FileBitmapSave
								R_CS_FileBitmapLoad
								R_CS_FileGmdSave
								R_CS_FileGmdSetComment
								R_CS_FileGmdLoad
								R_CS_FileGmdLoadWithComment
							�ȉ��̊֐���ǉ�
								R_CS_FileGmdSetAddress()
								R_CS_FileGmdSaveWithComment()
	1.07��5.7	2014.09.04	komatsu
							�摜�ۑ�/�Ǎ��̃p�X���w��ł���悤�ɕύX
								R_CS_FileBitmapLoadWithPath
								R_CS_FileGmdLoadWithCommentAndPath
								R_CS_FileSetWorkDir
							�f�B���N�g�����폜�ł���悤�ɕύX
								R_CS_FileDeleteDir
	1.07��7.5	2016.02.29	�f�B���N�g���폜�����w��Ή�	oshima
								R_CS_SetImageDirAutoDeleteKeepDay
******************************************************************************/

#ifndef __R_CS_FIL_H__
#define __R_CS_FIL_H__

#include "R_bitmap.h"

#ifdef __cplusplus
extern "C"
{
#endif


// --- define ---
typedef enum deltype{
	WITHOUT_DELETE = 0,
	WITHOUT_FILE,
	WITHOUT_GMD,
	WITHOUT_GMD_BMP,
	WITHOUT_GMD_BMP_CSV,
	SPECIFY_KEEPING_DAY,		//<1.07b7.5> oshima
	SPECIFY_KEEPING_DAY_WITH_TIMESTAMP		//<Ver.5.044> miyazaki
}FILE_DELTYPE;

// --- variable ---
extern struct winb_t R_CS_WinbFileMenu;

// --- prototype ---
//int R_CS_SaveBitmap(void);
//int R_CS_LoadBitmap(void);
//int R_CS_SaveGmd(void);													//<1.07b1.4>
//int R_CS_LoadGmd(void);
//int R_CS_LoadGmdWithComment(char *comment,int size);						//2008.12.9 oshima
//<1.07b2.3> �֐����ύX
int R_CS_FileBitmapSave(void);												// �摜�Z�[�u(BMP)
int R_CS_FileBitmapLoad(void);												// �摜���[�h(BMP)
int R_CS_FileBitmapLoadWithPath(char *path);								// �摜���[�h(BMP)						<1.07b5.7> �p�X���w��ł���悤�ɕύX	2014.09.04	komatsu
int R_CS_FileGmdSetAddress(unsigned char *pImage, int nType);				//<1.07b2.3> �摜(GMD)�A�N�Z�X���̉摜�ݒ�(�A�h���X�w��)
int R_CS_FileGmdSetComment(char *pComment);									// �摜�Z�[�u(GMD)���̃R�����g�ݒ�
int R_CS_FileGmdSave(void);													// �摜�Z�[�u(GMD)
int R_CS_FileGmdSaveWithComment(char *pComment);							//<1.07b2.3> �摜�Z�[�u(GMD�R�����g�t��)
int R_CS_FileGmdLoad(void);													// �摜���[�h(GMD)
int R_CS_FileGmdLoadWithComment(char *comment,int size);					// �摜���[�h(GMD�R�����g�t��)
int R_CS_FileGmdLoadWithCommentAndPath(char *path, char *comment, int size);// �摜���[�h(GMD�R�����g�t��,�p�X�w�肠��)	<1.07b5.7> �p�X���w��ł���悤�ɕύX	2014.09.04	komatsu
void	R_CS_FileGmdLoad_SetPostLoadFunc(void (*func)(int*, const char*, const char*));

int R_CS_FileSetWorkDir(char *path);										//<1.07b5.7> �p�X���w��ł���悤�ɕύX	2014.09.04	komatsu
int R_CS_FileDeleteDir(char *path, FILE_DELTYPE type, int msglevel);		//<1.07b5.7> �f�B���N�g���폜�֐�	2014.09.04	komatsu

//<1.07b2.3> ���֐����݊��p
#define R_CS_SaveBitmap				R_CS_FileBitmapSave
#define R_CS_LoadBitmap				R_CS_FileBitmapLoad
#define R_CS_SaveGmd				R_CS_FileGmdSave
#define R_CS_SaveGmdSetComment		R_CS_FileGmdSetComment
#define R_CS_LoadGmd				R_CS_FileGmdLoad
#define R_CS_LoadGmdWithComment		R_CS_FileGmdLoadWithComment

int R_CS_FileCopy(int src, int dst);
int R_CS_DirCopy(int src, int dst);

int R_CS_FileNetworkSetNetCommandDir(char *dir);		// NET�R�}���h�f�B���N�g���̐ݒ�  2007.11.20  by matsu
char* R_CS_FileNetworkGetNetCommandDir(void);			// NET�R�}���h�f�B���N�g���̎Q��  2007.11.20  by matsu
int R_CS_FileNetworkSetComputerName(char *name);		// �ڑ���R���s���[�^���̐ݒ�  2007.11.20  by matsu
char* R_CS_FileNetworkGetComputerName(void);			// �ڑ���R���s���[�^���̎Q��  2007.11.20  by matsu
int R_CS_FileNetworkSetDirectoryName(char *name);		// �ڑ���f�B���N�g�����̐ݒ�  2007.11.20  by matsu
char* R_CS_FileNetworkGetDirectoryName(void);			// �ڑ���f�B���N�g�����̎Q��  2007.11.20  by matsu
int R_CS_FileDeleteDirSetPath(char *path);				// �f�B���N�g���폜���s���p�X�̐ݒ�  2007.11.22
char* R_CS_FileDeleteDirGetPath(void);					// �f�B���N�g���폜���s���p�X�̎Q��  2007.11.22

int R_CS_SetFileLoadIdNumber(int nId);					//2008.12.15	��������}�����AFileLoad����ID�ݒ�
int R_CS_GetFileLoadIdNumber(void);						//2008.12.15	��������}�����AFileLoad����ID�Q��
char *R_CS_GetFileNameBuff(void);
int R_CS_SetFileNameBuff(char *filename);

int R_CS_SetImageDirAutoDeleteKeepDay(int nDay);		// �摜�f�B���N�g�����c�������w��@2016.02.29 oshima

#ifdef __cplusplus
}
#endif

#endif
