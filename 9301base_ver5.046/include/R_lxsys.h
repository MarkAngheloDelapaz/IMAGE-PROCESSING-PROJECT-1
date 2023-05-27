/*******************************************************************
	Linux用システム関数群

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

//__Linux関連
#define R_LXSYS_SHM_MASTER	1
#define R_LXSYS_SHM_SLAVE	2
typedef struct{
	int		nMode;				//Master Or Slave
	char	cID;				//識別コード			例-(char)'x'
	char	cKeyFileName[100];	//識別コード用ファイル	
	int		nMemSize;
	int		nWaitTimer;			//待機時間
	void	*pAddressTop;		//共有化されたメモリの先頭アドレス	R_lxsys_shm_get_address( *pStruct)
	//private
	key_t	tKey;				//アクセス禁止　上記cCommonNameとcIDから生成される
	int		nFlag;				//アクセス禁止	shmget時に使用する
	int		hShmId;
}R_LXSYS_SHM_STRUCT;

int R_lxsys_shm_open( R_LXSYS_SHM_STRUCT *pPara, int nMode, int nMemSize, char cID, char *cKeyFileName, int nWaitTimer );
int R_lxsys_shm_close( R_LXSYS_SHM_STRUCT *pPara );
#define R_lxsys_shm_get_address( pPara )  ((pPara)->pAddressTop) 

#ifdef __cplusplus
}
#endif

#endif
