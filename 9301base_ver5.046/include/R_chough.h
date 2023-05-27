/***************************************************************************
	R_chough.h
	
履歴
Ver 4.01	2009.08.07 R_chough_max_point_detection_x256追加	by nakahara

***************************************************************************/

#ifndef R_CHOUGH_H_
#define R_CHOUGH_H_

#include "R_stdlib.h"

#ifdef __x86_64__
#include "R_plane.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define R_CHOUGH_DRAW_NORMAL	0
#define R_CHOUGH_DRAW_MAX		1
#define R_CHOUGH_DRAW_XY	0
#define R_CHOUGH_DRAW_XR	1
#define R_CHOUGH_DRAW_YR	2
// --- variable ---
#ifndef __x86_64__
extern int nCHoughErrorCode;
#endif

typedef struct{
	int nRMin;								// 最大検出半径最大値
	int nRMax;								// 最大検出半径最小値
	int nXMin;								// 最大検出領域左上X座標
	int nYMin;								// 最大検出領域左上Y座標
	int nXMax;								// 最大検出領域右下X座標
	int nYMax;								// 最大検出領域右下Y座標
	int nRSearchMin;						// 探索する半径の最小値
	int nRSearchMax;						// 探索する半径の最大値
	int nXSearchMin;						// 探索するＸ座標の最小値
	int nXSearchMax;						// 探索するＸ座標の最大値
	int nYSearchMin;						// 探索するＹ座標の最小値
	int nYSearchMax;						// 探索するＹ座標の最大値
	int nMemorySize;						// ライブラリで使用しているメモリサイズ[byte]
	int nAreaSize;							// 投票領域(XYR全て)のサイズ[byte]
	int nPlaneSize;							// 投票領域(XYのみ、R固定)のサイズ[byte]
	unsigned short *pChoughVotingArea;		// 投票領域へのポインタ
	int nVotingNumber;						// 投票数(R_chough_votingで投票した数)	2008.07.29 追加
}R_CHOUGH_DATA;
// --- prototype ---
#ifdef __x86_64__
typedef	R_CHOUGH_DATA*	R_CHOUGH_HANDLE;
R_CHOUGH_HANDLE	R_chough_open(R_RECT* tWindow, int nRMin, int nRMax, int width, int height);	// 円検出ハフ変換ライブラリのオープン
int	R_chough_disp_voting_area(R_CHOUGH_HANDLE nHandle, int d, int cut_mode, int mode, R_PLANE* plane);
#else
typedef	int				R_CHOUGH_HANDLE;
R_CHOUGH_HANDLE	R_chough_open(R_RECT* tWindow, int nRMin, int nRMax);	// 円検出ハフ変換ライブラリのオープン
int	R_chough_disp_voting_area(R_CHOUGH_HANDLE nHandle, int d, int cut_mode, int mode);
#endif
int	R_chough_close(R_CHOUGH_HANDLE nHandle);							// 円検出ハフ変換ライブラリのクローズ
int	R_chough_clear(R_CHOUGH_HANDLE nHandle);							// 円検出ハフ空間のクリア(投票のクリア)
int	R_chough_get_error(void);
int	R_chough_voting(R_CHOUGH_HANDLE nHandle, int number, R_POINT* point);
int	R_chough_set_search_radius(R_CHOUGH_HANDLE nHandle, int rmin, int rmax);
int	R_chough_set_search_area(R_CHOUGH_HANDLE nHandle, int xmin, int ymin, int xmax, int ymax);
int	R_chough_max_point_detection(R_CHOUGH_HANDLE nHandle, R_POINT *res_point, int *res_r , int *res_val);
int	R_chough_max_point_detection_x256(R_CHOUGH_HANDLE nHandle, R_POINT* res_point, int* res_r, int* res_val);		//2009.08.07 by nakahara added
int	R_chough_get_voted_value(R_CHOUGH_HANDLE nHandle, int x, int y, int r);
int	R_chough_get_memsize(R_CHOUGH_HANDLE nHandle);
int	R_chough_point_voting(R_CHOUGH_HANDLE nHandle, int r, R_POINT* point);

int R_chough_detect_consentric_circle(R_CHOUGH_HANDLE nHandle,int *width,int linenum, R_POINT* res_point, int* res_r, int* res_val);
int R_chough_detect_consentric_circle_x256(R_CHOUGH_HANDLE nHandle,int *width,int linenum, R_POINT* res_point, int* res_r, int* res_val);

R_POINT	R_chough_set_vote_area(R_CHOUGH_HANDLE nHandle, int xmin, int ymin);										//2016.11.21	komatsu
int		R_chough_set_vote_radius(R_CHOUGH_HANDLE nHandle, int r);												//2016.11.21	komatsu

/*
int R_chough_open( R_RECT *tWindow , int nRMin, int nRMax);							// 円検出ハフ変換ライブラリのオープン
int R_chough_close( int nHandle );													// 円検出ハフ変換ライブラリのクローズ
int R_chough_clear( int nHandle );													// 円検出ハフ空間のクリア(投票のクリア)
int R_chough_get_error( void );
int R_chough_voting( int nHandle, int number, R_POINT *point );
int R_chough_set_search_radius( int nHandle, int rmin, int rmax );
int R_chough_set_search_area( int nHandle, int xmin, int ymin, int xmax, int ymax );
int R_chough_max_point_detection( int nHandle, R_POINT *res_point, int *res_r , int *res_val);
int R_chough_max_point_detection_x256( int nHandle, R_POINT *res_point,int *res_r , int *res_val);		//2009.08.07 by nakahara added
int R_chough_get_voted_value( int nHandle, int x, int y, int r );
int R_chough_disp_voting_area( int nHandle, int d, int cut_mode, int mode );
int R_chough_get_memsize( int nHandle );
int R_chough_point_voting( int nHandle, int r, R_POINT *point );	// 2009/07/21
*/
//int R_chough_voting( int nHandle, R_POINT tPoint );								// 円検出ハフ空間への投票
//int R_chough_detection( int nHandle, int nNumber, R_CIRCLE pCoeff[] );			// ハフ変換による円の検出
//int R_chough_set_threshold( int nHandle, int nThreshold );						// 円検出ハフ　投票数しきい値の設定
//int R_chough_set_region( int nHandle, int nRegionX, int nRegionY, int nRegionR );	// 円検出ハフ　局所極大点を求める領域の設定
//int R_chough_set_start_point( int nHandle, R_POINT tStart );						// 円検出ハフ　処理ウィンドウ始点の設定
//int R_chough_set_vote_range_x( int nHandle, int nRangeMin, int nRangeMax );		// 円検出ハフ　中心X投票範囲の設定
//int R_chough_set_vote_range_y( int nHandle, int nRangeMin, int nRangeMax );		// 円検出ハフ　中心Y投票範囲の設定
//int R_chough_set_vote_range_r( int nHandle, int nRangeMin, int nRangeMax );		// 円検出ハフ　半径投票範囲の設定

#ifdef __cplusplus
}
#endif

#endif	// R_CHOUGH_H_
