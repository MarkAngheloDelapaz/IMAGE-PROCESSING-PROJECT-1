/******************************************************************************

	R_CS_RES.H

	�������ʊǗ�(�g�����h���j�^�H)�֐��w�b�_

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	����
	Version		2004.07.06.000		�I���W�i��
				2006.05.09.001		���ρA�Ђ̒l���Ԉ���Ă����̂��C��
				2008.09.09			R_CS_ResultMonitorDisp()�ŕ`�悷�鍀�ڔԍ��̃`�F�b�N��ǉ�

******************************************************************************/
#ifndef __R_CS_RES_H__
#define __R_CS_RES_H__

#include "R_stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- define ---
#define RESULT_MONITOR_MAX        8
#define RESULT_MONITOR_ITEM_MAX   1000
#define RESULT_MONITOR_SELECT_MAX 4

// --- define ---
//�������ʃA�C�e���\����
typedef struct{
	int		*pData;						//�������ʂ̒l�ւ̃|�C���^
	int		*pDataBase;					//�������ʂ̊�l�ւ̃|�C���^
	int		*pDataBaseMin;				//�������ʂ̉^�p�ŏ��l�ւ̃|�C���^
	int		*pDataBaseMax;				//�������ʂ̉^�p�ő�l�ւ̃|�C���^
	int		*pDataLimitMin;				//�������ʂ̃��~�b�g�ŏ��l�ւ̃|�C���^
	int		*pDataLimitMax;				//�������ʂ̃��~�b�g�ő�l�ւ̃|�C���^
	int		nDataError;					//�������ʂ��G���[�̂Ƃ��̒l
	char	*sNameJp;					//�������ʂ̍��ږ��i���{��j
	char	*sNameEn;					//�������ʂ̍��ږ��i�p��)
	int		nDataMin;					//�O���t�\���p Y���ŏ��l
	int		nDataMax;					//�O���t�\���p Y���ő�l
	int		nDataDiv;					//�O���t�\���p X���������i�_�O���t�j
}R_CS_RESULT_MONITOR_ITEM;

//�������ʃO���t�\���\����
typedef struct{
	R_RECT	tArea;						//�`��G���A
	int		nTotalNum;					//�`�悷��f�[�^�z��̌�
	int		nItemNum[4];				//�`�悷��f�[�^�̍��ڔԍ�
	int		nDataMax;
	int		nDataMin;
}R_CS_RESULT_MONITOR_DISP;

//�������ʓ��v�l�\����
typedef struct{
	int		*pBuffer;			//�������ʊi�[�o�b�t�@
	int		nMax;				//�ő�l
	int		nMin;				//�ŏ��l
	int		nRange;				//�����W(�ő�l-�ŏ��l)
	double	dAverage;			//���ϒl
	double	dStdEv;				//�W���΍�
}RESULT_MONITOR_RES;

//�������ʊǗ��\����
typedef struct{
	int					nDataMaxNum;	//�������ʂ��i�[����ő�l
	int					nItemNum;		//�������ʂ̌�(RESULT_MONITOR_ITEM�̌�)
	int					nCurrentNum;	//�i�[�o�b�t�@�̌��݂̔ԍ�
	int					nRingBuffFlag;	//�i�[�o�b�t�@���ꏄ�����������߂��t���O
	R_CS_RESULT_MONITOR_ITEM	*pItem;			//�������ʃA�C�e���\����
	RESULT_MONITOR_RES	*pRes;			//�������ʓ��v�l�\����

	int					nLastSaved;		//�����ۑ��̃C���f�b�N�X
}RESULT_MONITOR;


// --- variable ---

// --- prototype ---
int R_CS_ResultMonitorOpen( R_CS_RESULT_MONITOR_ITEM tItem[], int nMax );
int R_CS_ResultMonitorClose( int nHandle );
int R_CS_ResultMonitorCounterUp( int nHandle );
int R_CS_ResultMonitorCounterClear( int nHandle );
int R_CS_ResultMonitorDisp( int nHandle, R_CS_RESULT_MONITOR_DISP *pDispPara );
int R_CS_ResultMonitorSelectItem( int nHandle );
int R_CS_ResultMonitorFileSave( int nHandle, int nFileHandle, int nCounter );
int R_CS_ResultMonitorFileSaveSequential(int nHandle, char *filename, int nCounter );
int R_CS_ResultMonitorFileLoad(int nHandle, int nFileHandle, int nMax);

int R_CS_ResultMonitorGetCount(R_CS_RESULT_MONITOR_ITEM *tItem);
int R_CS_ResultMonitorAppend(R_CS_RESULT_MONITOR_ITEM *pTarget, R_CS_RESULT_MONITOR_ITEM *pItem);
int R_CS_SaveResultMonitorLimitValue(R_CS_RESULT_MONITOR_ITEM *pTarget, char *pFileName);
int R_CS_LoadResultMonitorLimitValue(R_CS_RESULT_MONITOR_ITEM *pTarget, char *pFileName);

RESULT_MONITOR **R_CS_GetResultMonitor();
#ifdef __cplusplus
}
#endif

#endif
