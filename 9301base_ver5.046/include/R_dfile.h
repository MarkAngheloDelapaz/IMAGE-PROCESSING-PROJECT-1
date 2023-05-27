/******************************************************************************

	�f�[�^�t�@�C���������C�u�����w�b�_�t�@�C��

	Ver.3.47	2006.02.10	�o�^
							<R_datafile_load(),R_save_datafile()�̂�>
	Ver.3.48	2006.03.22	R_TYPE_POINT,R_TYPE_DPOINT,R_TYPE_RECT,R_TYPE_BOX,R_TYPE_LINE�Ή�
							�z��̕ۑ��ɑΉ�
	Ver.3.49	2006.09.27	���x���̌��ɃC���f���g�p�̃X�y�[�X������ꍇ�̓ǂݏ����Ή�
							R_datafile_save()�̔z�񎞂̏o�͏����ɊԈႢ���������̂ŏC��
	Ver.3.50	2007.05.25	R_datafile_save ���x���̋󔒏�����������malloc��傫������
				2007.10.10	R_datafile_save malloc�T�C�Y���Ԉ���Ă����̂ŏC��
	Ver.4.01	2009.09.29	R_datafile_save_history()�ǉ�
	Ver.4.01	2012.09.04	R_datafile_save�o�O�t�B�N�X
	Ver.4.02	2014.01.08	INT,POINT,RECT,BOX�ɂ����ăf�[�^�t�@�C���̖����ɏ��������ǋL�ł���悤�ɕύX	by komatsu

******************************************************************************/

#ifndef	R_DFILE_H_
#define	R_DFILE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
// -> R_stdlib.h �ɓo�^
#define R_TYPE_INT			0	// an integral number (decimal number)
#define R_TYPE_CHAR			1	// character type
#define R_TYPE_DOUBLE		2	// double (decimal fraction format)
#define R_TYPE_HEX			3	// an integral number (hexadecimal number)
#define R_TYPE_STRING		4	// character string 
#define R_TYPE_SHORT		5	// short int (decimal number)
#define R_TYPE_SHORT_HEX	6	// short int (hexadecimal number)
#define R_TYPE_POINT		7	// R_POINT struct		2006.03.22 by matsu
#define R_TYPE_DPOINT		8	// R_DPOINT struct		"
#define R_TYPE_RECT			9	// R_RECT struct		"
#define R_TYPE_BOX			10	// R_BOX struct			"
#define R_TYPE_LINE			11	// R_LINE struct		"
*/


// R_datafile_save_history()
#define DATAFILE_DATA		0x0000		// �o�̓f�[�^	�f�[�^�o��
#define DATAFILE_LABEL		0x0001		// 				���x���o��
#define DATAFILE_NEW_FILE	0x0010		// �t�@�C�����[�h	�t�@�C���V�K�쐬
#define DATAFILE_NO_TIME	0x0100		// ���O�f�[�^	���ԏo�͂Ȃ�
#define DATAFILE_ALL_LOG	0x0200		// 				�ύX�������̃��O���c��

//		2015.07.16	komatsu
typedef struct {
	int		*nMax;						// �f�[�^�ő�l(INT:0�CPOINT�F0,1�CRECT�F0-3)
	int		*nMin;						// �f�[�^�ŏ��l(INT:0�CPOINT�F0,1�CRECT�F0-3)
} R_DATAFILE_ITEM_LIMIT;

// �p�����[�^�t�@�C���v�f�v���g�^�C�v
typedef struct {
	char	*pLabel;					// �p�����[�^���x��
	void	*pData;						// �p�����[�^�v�f
	int		nType;						// �p�����[�^�^���
	int		nLength;					// �o�b�t�@�[�T�C�Y
	int		nArray;						// �f�[�^�z��		2006.03.22 by mastu
	R_DATAFILE_ITEM_LIMIT	*pLimit;
//	int		nMax[4];					// �f�[�^�ő�l(INT:0�̂݁CPOINT�F0,1�CRECT�F0-3�g�p)		2014.01.08	komatsu
//	int		nMin[4];					// �f�[�^�ŏ��l(INT:0�̂݁CPOINT�F0,1�CRECT�F0-3�g�p)		2014.01.08	komatsu
} R_DATAFILE_ITEM;


int R_datafile_load(R_DATAFILE_ITEM *pList, const char* pFileName);
int R_datafile_save(R_DATAFILE_ITEM *pList, const char* pFileName);
int R_datafile_save_withMode(R_DATAFILE_ITEM *pList, const char* pFileName, char *sMode);
int R_datafile_save_history(R_DATAFILE_ITEM *pList, char *pFileName, int nMode);		// ver.4.01 2009.09.29

int R_datafile_chklimit(R_DATAFILE_ITEM *pList);
int R_datafile_openlimit(R_DATAFILE_ITEM *pList);
int R_datafile_closelimit(R_DATAFILE_ITEM *pList);

int R_datafile_count(R_DATAFILE_ITEM *pList);
int R_datafile_append(R_DATAFILE_ITEM *pTarget, R_DATAFILE_ITEM *pItem);
//R_DATAFILE_ITEM *R_datafile_connect(R_DATAFILE_ITEM *pItem0, R_DATAFILE_ITEM *pItem1);
int R_file_save_history(char *pLoadFileName, char *pSaveFileName, int nMode);		// 2019.11.29 kitamura

#ifdef __cplusplus
}
#endif

#endif	// R_DFILE_H_
