/******************************************************************************
	R_hex.h / R_hex.c
	
	変更履歴
	Ver 1.04	1994/02/16	正式リリース
	Ver 3.50	2009/06/09	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_HEX_H_
#define	R_HEX_H_

// ヘキサフォーマットの変換
unsigned int r_sethex(unsigned char *buff,unsigned char bin,unsigned int sum);
int R_bin2intelhex(unsigned char *buff,unsigned int addres,unsigned char *src, int byte);

#endif	// R_HEX_H_
