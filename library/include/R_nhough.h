/******************************************************************************
	R_nHough.H / R_nHough.C
	
	変更履歴
	Ver 3.39	2003/06/10	R_nhough_clear_tbl(),R_nhough_voting_point()追加  by matsu
	Ver 4.01	2009/08/07	R_nhough_hp****	追加	by nakahara
	Ver 4.01	2012/04/09	64bitマルチスレッド対応
******************************************************************************/

#ifndef R_NHOUGH_H_
#define R_NHOUGH_H_

#include <R_stdlib.h>

#ifdef __x86_64__
#include "R_plane.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __x86_64__
	typedef struct {
		unsigned long	ulNhough_max_r;
		unsigned long	ulNhough_org_x;
		unsigned long	ulNhough_org_y;
		unsigned long	ulNhough_xo;
		unsigned long	ulNhough_yo;
		unsigned long	ulNhough_del_rw;
		unsigned long	ulNhough_del_aw;
		unsigned short 	*pNhough_tbl;
	} R_NHOUGH_DATA;
#else
	extern unsigned short	*pNhough_tbl;
	extern unsigned long	ulNhough_max_r;
	extern unsigned long	ulNhough_org_x;
	extern unsigned long	ulNhough_org_y;
	extern unsigned long	ulNhough_xo;
	extern unsigned long	ulNhough_yo;
	extern unsigned long	ulNhough_del_rw;
	extern unsigned long	ulNhough_del_aw;
#endif

#define R_NHOUGH_DIMENSION_MAX	10		//2009.08.07 by nakahara added

//2009.08.07 by nakahara added
typedef struct{			//汎用ハフ変換テーブル構造体
	int				nDimension;							//次元
	int				nSize[R_NHOUGH_DIMENSION_MAX];			//各次元の投票サイズ
	int				nMemSize;							//構造体全体のメモリサイズ
	int				nDepth;								//テーブルの投票空間サイズ(2次元の場合 nSize[0]*nSize[1]
	unsigned long	*pTable;							//テーブルの先頭アドレス
}R_NHOUGH_HPSTRUCT;

//2009.08.07 by nakahara added
typedef struct{			//直線ハフ変換用テーブル構造体
	int				nResoAngle;								//角度分解
	int				nResoPixel;								//画素分解
	int				nOrgAngle;								//角度軸テーブル原点の角度
	int				nOrgPixel;								//画素軸テーブル原点の距離
#ifdef __x86_64__
	int				nWidth;
	int				nHeight;
#endif
	R_NHOUGH_HPSTRUCT	*pVoteArea;
}R_NHOUGH_HPLINE;

//2009.08.07 by nakahara added
typedef struct{			//長方形ハフ変換用テーブル
	R_NHOUGH_HPLINE	tLineHough[4];
}R_NHOUGH_HPRECT;

typedef struct{
		int start;
		int end;
}R_NHOUGH_HPPOLY_RANGE;


#define R_NHOUGH_HPPOLY_LINE_MAXNUM	16

typedef struct{
		int nLineNum;									//	図形に使用されている直線数
		int nBaseAngleMin;								//	図形の許容角度(最小)
		int nBaseAngleMax;								//	図形の許容角度(最大)
		int nDist[R_NHOUGH_HPPOLY_LINE_MAXNUM];			//	各直線の原点からの距離
		int nAngle[R_NHOUGH_HPPOLY_LINE_MAXNUM];		//	各直線の法線の角度(右回り正)
}R_NHOUGH_HPPOLY_SHAPE;


#define R_NHOUGH_HPPOLY_RANGE_MAXNUM	64
typedef struct{
		R_NHOUGH_HPPOLY_RANGE tRange[R_NHOUGH_HPPOLY_RANGE_MAXNUM];
		int nRangeNum;
		R_NHOUGH_HPLINE tHpStruct;
}R_NHOUGH_HPPOLY;

#ifdef __x86_64__
R_NHOUGH_DATA*	R_nhough_open(int widht, int height);
int		R_nhough_voting(R_NHOUGH_DATA*, R_PLANE* plane, R_RECT* rect, int th);
int		R_nhough_disp_tbl(R_NHOUGH_DATA*, R_PLANE* plane);
void	R_nhough_clear_tbl(R_NHOUGH_DATA*);
void	R_nhough_voting_point(R_NHOUGH_DATA*, R_POINT* point, int num);
int		R_nhough_detection(R_NHOUGH_DATA*, int* a, int* b, int* c, int line_num);
int		R_nhough_set_mask(R_NHOUGH_DATA*, int width, int angle);
int		R_nhough_get_mask(R_NHOUGH_DATA*, int* width, int* angle);
#else
int R_nhough_open( void );
int R_nhough_voting( int xs,int ys,int xw,int yw,unsigned char *sp, int th);
int R_nhough_disp_tbl( void );
void R_nhough_clear_tbl();
void R_nhough_voting_point(R_POINT *point, int num);
int R_nhough_detection( int *a, int *b , int *c, int line_num );
int R_nhough_set_mask( int width, int angle );
int R_nhough_get_mask(int *width, int *angle);
#endif
int R_nhough_disp_line( int a[],int b[],int c[], int line_num ,int mode,int xs,int ys,int xw,int yw);

//高信頼直線ハフ変換関数群 //2009.08.07 by nakahara added
#ifdef __x86_64__
int	R_nhough_hpline_create(R_NHOUGH_HPLINE* pLineHough, int nResoAngle, int nResoPixel, int width, int height);
int	R_nhough_hprect_create(R_NHOUGH_HPRECT* pRectHough, int widht, int height);
#else
int R_nhough_hpline_create( R_NHOUGH_HPLINE *pLineHough, int nResoAngle, int nResoPixel );
int R_nhough_hprect_create( R_NHOUGH_HPRECT *pRectHough );
#endif
int R_nhough_hpline_destroy( R_NHOUGH_HPLINE *pLineHough );
int R_nhough_hpline_table_all_clear( R_NHOUGH_HPLINE *pLineHough );
int R_nhough_hpline_table_angle_clear( R_NHOUGH_HPLINE *pLineHough, int nMinAngle, int nMaxAngle );
int R_nhough_hpline_vote_with_mode( int nSrcNumber, const R_POINT *pSrcPoint, R_NHOUGH_HPLINE *pLineHough, int nAngleMin, int nAngleMax , int nMode);
int R_nhough_hpline_search( R_NHOUGH_HPLINE *pLineHough, int nNumber, int nAngleMask, int nPixelMask, int nMinAngle, int nMaxAngle, R_LINE *pLineCoeff, int *pLineAngle, unsigned long *pLineVoteValue);
int R_nhough_hprect_search( int nSrcNumber, const R_POINT* pSrcPoint, R_NHOUGH_HPRECT *pRectHough, int nAngleMin, int nAngleMax, int nLengthMin_a, int nLengthMax_a, int nLengthMin_b, int nLengthMax_b, R_ARBRECT *pResult );

// 内部関数(他ライブラリ関数で使う可能性あり)
#ifdef __x86_64__
R_NHOUGH_HPSTRUCT* r_nhough_hptrans_table_create(int nDimension, int* pSize);
#else
int		r_nhough_hptrans_table_create( int nDimension, int *pSize );
#endif
void	r_nhough_hptrans_table_destroy( R_NHOUGH_HPSTRUCT *pHough );
int		r_nhough_hpline_table_clear( unsigned long *pTable, int xs, int ys, int xe, int ye, int xsize );
int		r_nhough_hpline_get_maxvote( unsigned long *pTable, int xs, int ys, int xsize, int ysize, int nMaxXSize, int *pResX, int *pResY );
int		r_nhough_hpcalc_line_equation( R_LINE *pLineCoeff, int xs, int ys, int nOrgAngle, int nResoAngle, int nOrgPixel, int nResoPixel );
int		r_nhough_hpline_projection_angle( int nFilterSize, R_NHOUGH_HPLINE *pLineHough, int xs, int ys, int xe,  int ye, unsigned long *bright );
int		r_nhough_hprect_get_maxangle( R_NHOUGH_HPLINE *pLineHough, unsigned long *pProjection, int nAngleMin, int nAngleMax, int *pResultAngle );
int		r_nhough_hprect_get_maxlength( unsigned long *pTable, int xs, int ys, int xe, int ye, int xsize, int lmin, int lmax, R_POINT *pResPos, int *pResLength );
int		r_nhough_hptrans_table_all_clear( R_NHOUGH_HPSTRUCT *pHough );
int		R_nhough_hpline_table_disp(R_NHOUGH_HPLINE *pLineHough, int rate);


//----------------------------------------------------------------------------------------------------
//	多角形ハフ検出ライブラリ関数群
//	R_nhough_hppoly_***
//----------------------------------------------------------------------------------------------------

///
/// \brief R_nhough_hppoly_create
/// \param pHough		多角形ハフ変換構造体
/// \param nResoAngle	角度方向分解能	hpline構造体に準じる
/// \param nResoPixel	距離方向分解能	hpline構造体に準じる
/// \return
///
///	ハフ投票空間を作成する
///	直線検出と同じく2次元の空間を利用する
int		R_nhough_hppoly_create(R_NHOUGH_HPPOLY *pHough, int nResoAngle, int nResoPixel );


///
/// \brief R_nhough_hppoly_setrange
/// \param pHough		多角形ハフ変換構造体
/// \param nAngleMin	最小角度配列
/// \param nAngleMax	最大角度配列
/// \param num			配列数
/// \return
///
///	ハフ変換にて投票対象の角度を指定する
///	内部で角度範囲の重複チェックを行うため重複したエリアを指定可
int		R_nhough_hppoly_setrange(R_NHOUGH_HPPOLY *pHough, int *nAngleMin, int *nAngleMax, int num);

///
/// \brief R_nhough_hppoly_setshape
/// \param pHough		多角形ハフ変換構造体
/// \param pShape		多角形形状データ構造体
/// \return
///
///	ハフ変換にて投票対象の角度を多角形形状データから指定する
/// 内部で重複チェックを実施するため複数形状の登録が可
///	R_nhough_hppoly_searchにて検索する可能性がある形状は予め投票前に形状登録を実施すること
int		R_nhough_hppoly_setshape(R_NHOUGH_HPPOLY *pHough, R_NHOUGH_HPPOLY_SHAPE *pShape);

///
/// \brief R_nhough_hppoly_vote
/// \param pHough		多角形ハフ変換構造体
/// \param pPoint		投票する点群
/// \param num			点群の総数
/// \return
///
///	ハフ空間上に点群の投票を行う
int		R_nhough_hppoly_vote(R_NHOUGH_HPPOLY *pHough, R_POINT *pPoint, int num);

///
/// \brief R_nhough_hppoly_search
/// \param pHough		多角形ハフ変換構造体
/// \param pShape		検出する多角形形状データ
/// \param tArea		形状検出を行うエリア(図形の原点が含まれる範囲)
/// \param tPos			検出位置(result)
/// \param nAngle		検出角度(result)
/// \return
///
///	直線の重ね合わせマッチングを行い検出した位置結果を返す
///	注意すべき点は、本ライブラリーは直線Hough空間を使用した”多直線検出”であるため、一般的図形に適用する”一般化Hough変換”とは以下の点で異なるということである。
///		1.	検出するのは直線のみで曲線は扱わない
///		2.	検出するのは”直線”であり”線分”ではないこと
int		R_nhough_hppoly_search(R_NHOUGH_HPPOLY *pHough, R_NHOUGH_HPPOLY_SHAPE *pShape, R_RECT tArea, R_POINT *tPos, int *nAngle);

///
/// \brief R_nhough_hppoly_getline
/// \param pShape		多角形形状データ構造体
/// \param tPos			画像空間上での多角形の原点位置
/// \param nAngle		画像空間上での多角形の角度
/// \param result		多角形各辺のR_LINE構造体
///
/// \return	int			検出された最大投票数
///
///	多角形形状データ及び位置、角度から多角形各辺をR_LINE構造体に変換する
int		R_nhough_hppoly_getline(R_NHOUGH_HPPOLY_SHAPE *pShape, R_POINT tPos, int nAngle, R_LINE *result);

///
/// \brief R_nhough_hppoly_disp
/// \param pShape		多角形形状データ構造体
/// \param tArea		表示範囲
/// \param x			多角形原点のx座標
/// \param y			多角形原点のy座標
/// \param nAngle		多角形の角度
///
///	多角形を任意の位置に描画する
void	R_nhough_hppoly_disp(R_NHOUGH_HPPOLY_SHAPE *pShape, R_RECT tArea, int x, int y, int nAngle);

//!
//! \brief R_nhough_hppoly_clearrange
//! \return
//!
//! 登録多角形をクリアする。
//! setrange、setshapeは複数登録可能なため登録し直す場合はクリアを実行すること
int		R_nhough_hppoly_clear_range(R_NHOUGH_HPPOLY *pHough);

//!
//! \brief R_nhough_hppoly_clear_tbl
//! \param pHough
//! \return
//!
//!	投票テーブルをクリアする
int		R_nhough_hppoly_clear_tbl(R_NHOUGH_HPPOLY *pHough);

#ifdef __cplusplus
}
#endif

#endif	// R_NHOUGH_H_
