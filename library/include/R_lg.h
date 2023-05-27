/******************************************************************************
	R_lg.h / R_lg.c
	
	変更履歴
	Ver 3.28	2001/02/05	新規作成
	Ver 3.50	2009/02/09	DOS & LINUX ソース統合
******************************************************************************/

// エッジファインダー（ガウシアン・ラプラシアン）ライブラリ

#ifndef	R_LG_H_
#define	R_LG_H_

#ifdef LG_FILE
	signed long *lg_conv = NULL;
	int			 lg_size = 0;
#else
	extern signed long *lg_conv;
	extern int			lg_size;
#endif

int R_lg_get_convolver( int size, double sigma ,int mult);
int R_lg_filter( int src_no,int dst_no,int xs,int ys,int xsize,int ysize);
int R_lg_zero_cross( int src_no, int dst_no, int xs,int ys,int xsize,int ysize);
int R_lg_edge_find( int src_no_lg,int src_no_df,int xs,int ys,int xsize,int ysize ,int th);
void R_lg_disp_conv( void );

#endif	// R_LG_H_
