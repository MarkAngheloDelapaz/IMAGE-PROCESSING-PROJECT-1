/******************************************************************************
	R_difsbv.h / R_difsbv.c
	
	変更履歴
	Ver 1.12	1994/07/07	オリジナル
	Ver 1.12	1994/07/14	相関値最大値が２つある時にエラーにしていたものを
							隣接している場合については、ＯＫとした。
	Ver 3.50	2009/2/10	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_DIFSBV_H_
#define	R_DIFSBV_H_

int R_gs_search_bv_with_fixlayer( int n, GS_SEARCH_RESULT *result, int layer);
int R_dgs_search_bv_with_fixlayer( int n, GS_SEARCH_RESULT *result, int layer);
int R_gs_search_bv( int pn, GS_SEARCH_RESULT *result );

#endif	// R_DIFSBV_H_
