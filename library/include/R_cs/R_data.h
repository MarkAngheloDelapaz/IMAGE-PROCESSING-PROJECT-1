/******************************************************************************

	�f�[�^�t�@�C���@�Z�[�u�E���[�h�֐��w�b�_
	�o�[�W�����A�b�v���́A�K�������̒ǉ����s���A
	�O���[�v���ŕύX���e���A�i�E���X���邱�ƁI�I

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	����
	Version		2004.03.15.000
				2004.04.23.001		DrawPlane�`��֐�(R_draw�n)��g�ݍ���

				2006.09.12.002		R_DataLoad/Savefile�����C�u�����֐���
									R_datafile_load/save�ɕύX
				2007.11.05.003		r_cs_DataFileDisp�ǉ�(R_DataFileDisp��legacy����)
******************************************************************************/
#ifndef __R_DATA_H__
#define __R_DATA_H__

#include "R_dfile.h"
// --- define ---
// --- �p�����[�^�t�@�C���\���� ---
// TSC-9301���C�u������
/*
#define ITEM_LENGTH_MAX		128		// 1�s�̍ő啶����

#define ERR_FILE_OPEN	-1			// �t�@�C���I�[�v���G���[
#define ERR_FILE_READ	-2			// �f�[�^�ǂݍ��݃G���[
#define ERR_FILE_LABEL	-3			// ���x���s��v�G���[
#define ERR_FILE_DATA	-4			// �f�[�^�ϑւ����[

#define TYPE_INT	0				// int�^
#define TYPE_CHAR	1				// char�^
#define TYPE_DOUBLE	2				// double�^
#define TYPE_HEX	3				// int(16�i)�^
#define TYPE_STRING	4				// ������^

typedef struct {
	char *label;					// �p�����[�^���x��
	void *para;						// �p�����[�^�v�f
	int type;						// �p�����[�^�^���
} R_PARA_FILE_ITEM;					// �p�����[�^�t�@�C���v�f�v���g�^�C�v
*/
// --- variable ---

// --- prototype ---
//int R_DataLoadfile(R_PARA_FILE_ITEM *item, char *file_name);
//int R_DataSavefile(R_PARA_FILE_ITEM *item, char *file_name);
void R_DataFileDisp(R_DATAFILE_ITEM *item, char *tytle);				// legacy
int r_cs_DataFileDisp(char *fname);										// 2007.11.05  by matsu

#endif
