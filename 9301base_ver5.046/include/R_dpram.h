/******************************************************************************
	R_DPRAM.H / R_DPRAM.C
	
	変更履歴
		Ver 3.40	2003/10/08	正式リリース
		Ver 3.50	2008/08/22	R_dpram_get_addressプロトタイプ宣言追加
		Ver 4.01	2013/04/08	Linux対応
		Ver 4.01	2016/12/27	CP-CZ-38(PLD TYPE-B) D000セグメント対応
******************************************************************************/

#ifndef	R_DPRAM_H_
#define	R_DPRAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#define		_BOARD_JP_16_SHORT	0x00000
#define		_BOARD_JP_16_OPEN	0x80000

#define		_BOARD_PLD_TYPE_B	0x80000000

// プロトタイプ宣言
#ifdef __DOS__
int		RCpcz47Open(int i, int j);
#elif __GNUC__
int		RCpcz47OpenAll(int num, int bMultiProcess);
#endif

int		R_dpram_open(int nRDSW);
int		R_dpram_write(int nChannel, int nData);
int		R_dpram_read(int nChannel);
int		R_dpram_bst(int nChannel, int nBit);
int		R_dpram_bcl(int nChannel, int nBit);
int		R_dpram_get_address(int nChannel);
void*	R_dpram_address(int nRDSW);

#ifdef __cplusplus
}
#endif

#endif	// R_DPRAM_H_
