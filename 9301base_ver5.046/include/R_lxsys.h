/*******************************************************************
	Linux�p�V�X�e���֐��Q

	Ver		Date			contents		name
	4.01	2010.02.25		release			by nakahara
	
********************************************************************/

#ifndef __R_LXSYS_H__
#define __R_LXSYS_H__

#ifdef __GNUC__
#include </usr/include/sys/types.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

//__Linux�֘A
#define R_LXSYS_SHM_MASTER	1
#define R_LXSYS_SHM_SLAVE	2
typedef struct{
	int		nMode;				//Master Or Slave
	char	cID;				//���ʃR�[�h			��-(char)'x'
	char	cKeyFileName[100];	//���ʃR�[�h�p�t�@�C��	
	int		nMemSize;
	int		nWaitTimer;			//�ҋ@����
	void	*pAddressTop;		//���L�����ꂽ�������̐擪�A�h���X	R_lxsys_shm_get_address( *pStruct)
	//private
	key_t	tKey;				//�A�N�Z�X�֎~�@��LcCommonName��cID���琶�������
	int		nFlag;				//�A�N�Z�X�֎~	shmget���Ɏg�p����
	int		hShmId;
}R_LXSYS_SHM_STRUCT;

int R_lxsys_shm_open( R_LXSYS_SHM_STRUCT *pPara, int nMode, int nMemSize, char cID, char *cKeyFileName, int nWaitTimer );
int R_lxsys_shm_close( R_LXSYS_SHM_STRUCT *pPara );
#define R_lxsys_shm_get_address( pPara )  ((pPara)->pAddressTop) 

#ifdef __cplusplus
}
#endif

#endif
