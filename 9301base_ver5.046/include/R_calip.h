/******************************************************************************
	R_calip.h
	
	変更履歴
	Ver 3.50	2009.03.24	R_calip_line_l2d,R_calip_degree_l2l関数追加
	Ver 3.50	2009.04.02	DOS & LINUX ソース統合
	Ver 3.50	2009.06.30	R_calip_find_lineV()書き直し
	Ver 4.01	2009.08.07	R_calip_change_line_with_org追加
	Ver 4.01 	2010.02.24	R_calip_rotate_point_integer追加				by nakahara
				2011.01.05	R_calip_mpoint_open()で、R_POINTのアドレス設定が間違っていたのを修正	by sassa
	Ver4.01		2013.05.16	R_calip_intersection x軸に垂直な場合のバグ修正
							R_stdlibにR_DLINEを追加
							R_calip_find_lineの内部の処理をR_calip_find_line_fineへ移行				by komatsu
******************************************************************************/

#ifndef	R_CALIP_H_
#define	R_CALIP_H_

#include "R_robust.h"
#include "R_stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define R_MINIMUM_SQUARE_MODE		0
#define R_ROBUST_STATICS_MODE		0x01
#define R_MAX_AREA_DETECT			0
#define R_MAX_VALUE_DETECT			0x02

// --- define ---

//LegacyFunction	->	R_DLINEを使用してください
typedef struct {
	int c;
	float a;
	float b;
} R_LINEF;

typedef double (*DFUNC)();

typedef struct{
	int amp;			//ブローブ高さ
	int start_pos;		//ブローブ開始位置
	int end_pos;		//ブローブ終了位置
	int peak_pos;		//ブローブピーク位置
	int area;			//ブローブ面積
}R_CALIP_PROTRUDE_RESULT;
typedef struct{
	R_LINE lineeq;		//基準直線
	R_CALIP_PROTRUDE_RESULT	convex;
	R_CALIP_PROTRUDE_RESULT	concave;
}R_CALIP_PROTRUDE;


typedef struct{
	int nSrcPointNumber;			//多角形の頂点数
	R_POINT *pointSrcPnt;			//多角形の頂点座標
	int *npAntiPodalStartNumber;	//各頂点に対するAnti-podal pointの候補点の開始頂点番号
	int *npAntiPodalEndNumber;		//各頂点に対するAnti-podal pointの候補点の終了番号
	int	nMemorySize;				//ハンドルのメモリサイズ
}R_CALIP_RC;

// --- variable ---
#ifdef _R_CALIP_FILE_
	ROBUST_CONDITION RCalipRobustCondition;
	int *RCalipData = NULL;
	R_POINT RCalipPoint[1024];
	double *RCalipSplineTable_P;
	double *RCalipSplineTable_A;
	double *RCalipSplineTable_B;
	double *RCalipSplineTempD;
	double *RCalipSplineTempH;
	double *RCalipCoX;
	double *RCalipCoY;
#else
	extern ROBUST_CONDITION RCalipRobustCondition;
	extern int *RCalipData;
	extern R_POINT RCalipPoint[];
	extern double  *RCalipSplineTable_P;
	extern double  *RCalipSplineTable_A;
	extern double  *RCalipSplineTable_B;
	extern double  *RCalipSplineTempD;
	extern double  *RCalipSplineTempH;
	extern double *RCalipCoX;
	extern double *RCalipCoY;
#endif

int R_calip_find_line( R_POINT *pt, int pt_num, R_LINEF *res, int mode );							//直線検出A
int R_calip_find_intersection( R_LINEF sline, R_LINEF dline, R_POINT *pt );							//２直線の交点A
int R_calip_find_intersection_B(R_LINE line1, R_LINE line2, int *x, int *y);						//２直線の交点(直線Ｂ）
int R_calip_check_clockwise( R_POINT a, R_POINT b, R_POINT c );										//点のベクトルに対する時計回り存在判別
int R_calip_tri_area( R_POINT a, R_POINT b, R_POINT c );											//三角形の面積
int R_calip_thinning( unsigned char *sp, unsigned char *dp, R_BOX area );							//細線化
int r_cconc( int inb[] );																			//細線化内部関数(対象点の周囲確認し白の値が何個あるか)
int R_calip_convex( int num, R_POINT *p, R_POINT *res);												//凸包
int R_calip_inner_product( R_POINT p1, R_POINT p2, R_POINT q1, R_POINT q2 );						//ベクトル内積
int R_calip_outer_product( R_POINT p1, R_POINT p2, R_POINT q1, R_POINT q2 );						//ベクトル外積
int R_calip_distance_l2p( R_POINT p1,R_POINT p2, R_POINT org,int *distance);						//点と直線との距離(直線：２点入力)
int R_calip_distance_l2p_A( R_LINEF line, R_POINT org, int *distance );								//点と直線との距離(直線：A)
int R_calip_distance_l2p_B(R_LINE line, R_POINT p);													//点と直線との距離(直線：B)
int R_calip_distance_l2p_sum(R_LINE line, R_POINT *p, int n);										//複数の点と直線の距離平方値の平均を求める
int R_calip_distance_p2p(R_POINT p0, R_POINT p1);													//点と点の距離
int R_calip_distance_l2l(R_LINE line0, R_LINE line1);												//平行２直線の距離
int R_calip_get_coX_fromline(R_LINE tLine, int nY, int *pX);										//直線上の点のX座標を求める(Y座標を与えたとき)
int R_calip_get_coY_fromline(R_LINE tLine, int nX, int *pY);										//直線上の点のY座標を求める(X座標を与えたとき)
int R_calip_stat_data_mode( int *data, int start, int end );										//データ列最頻値
int R_calip_stat_data_max( int *data, int start, int end );											//データ列最大値
int R_calip_stat_data_min( int *data, int start, int end );											//データ列最小値
double R_calip_stat_data_stdev( int *data, int start, int end );									//データ列標準偏差
double R_calip_stat_data_average( int *data, int start, int end );									//データ列平均
int R_calip_stat_data_movingave( int *data_src, int *data_dst, int start, int end, int step );		//データ列移動平均を行う
int R_calip_find_conic(R_DPOINT p[], int num, double a[]);											//最小二乗法による円錐直線近似
int R_calip_calc_conicinfo(double a[], double p[]);													//円錐曲線からの情報計算
void R_calip_shift_line(R_LINE line, int length, int *result);										//直線を一定距離平行移動したときの切片
int R_calip_point_in_square( R_POINT org, R_POINT *pt );												//任意の点の４角形内存在判別
int R_calip_rectangle_area( R_POINT *pt );															//任意四角形の面積
void R_calip_data_swap_n( int* x0, int* x1 );														//２つのデータの入れ替え(int)
void R_calip_data_swap_d( double* x0, double* x1 );													//２つのデータの入れ替え(double)
int R_calip_rotate_point( R_DPOINT *p_target, R_DPOINT p_org, double radian );						//座標の回転(ラジアン)
int R_calip_gauss_elimination(double a[], double b[], double x[], int n);							//連立１次方程式を解く
int R_calip_find_lineH(R_POINT *p, int n, R_LINE *line);											//最小二乗法による直線近似(０～４５）
int R_calip_find_lineV(R_POINT *p, int n, R_LINE *line);											//最小二乗法による直線近似(４５～９０）
int R_calip_find_line_fineH(R_POINT *p, int n, R_DLINE *line);										//最小二乗法による直線近似(０～４５）詳細版
int R_calip_find_line_fineV(R_POINT *p, int n, R_DLINE *line);										//最小二乗法による直線近似(４５～９０）詳細版
int R_calip_protrude_open( int max_number );														//１次元データ点列凹凸検出関数のオープン
int R_calip_protrude_line( int *data, int number, int mode, int thr_offset_convex, int thr_offset_concave, R_CALIP_PROTRUDE *res );		//１次元データ配列の近似直線からの突起情報を計算します
int R_calip_protrude_disp_result( int *data, int number, int y_offset, R_CALIP_PROTRUDE *res );			//凸凹結果の描画
int R_calip_calc_intercept(R_POINT *point, int number, int a0, int b0, int64_t *c0 );					//傾きが既知の直線の切片を最小二乗法により求める	2017.04.08	komatsu	cをlonglongに拡張
void R_calip_calc_intercept_bylength(R_POINT p, R_LINE line, int length, int *result);				//傾き、指定した点からの距離が既知の直線の切片を求める
int R_calip_find_circle(R_DPOINT p[], int number, double *result_coeff );							//最小二乗法による円近似
int R_calip_spline_open( int max_number );															//3次-Bスプライン関数の初期化
void R_calip_spline_close( void );																	//3次-Bスプライン関数の終了
int R_calip_spline_2d( R_POINT *s_pnt, int number, int div, R_DPOINT *d_pnt );						//3次-Bスプライン関数による補間
int R_calip_intersection_line_on_rect( R_RECT *rect, R_LINE *l, R_POINT *point1, R_POINT *point2 );	//直線と長方形の交点
int R_calip_point_in_rect( R_POINT *point, R_RECT *rect );											//任意の点の長方形内存在判別
int R_calip_remove_far_point(int nNumSrc, R_POINT *pPointSrc, int *pNumDst, R_POINT *pPointDst, int nDistance, int nLength);//連続している点列から外れ点を除去する

int R_calip_rc_ready( int hConvexHandle, int *nRcHandle );											//Rotate Caliper使用準備
int R_calip_rc_release( int nRcHandle );															//Rotate Caliper終了
int R_calip_rc_get_width( int hRcHandle, R_POINT *pEdgePoint, R_POINT *pVertex, int *pWidth );		//凸多角形の"幅”計算
void R_calip_calc_parallel_line_through_point( R_POINT *pSrcPoint, R_LINE *pSrcLine, R_LINE *pDstLine);	//ある直線に平行な、ある点を通る直線の式を計算
int R_calip_calc_line_with_2point( R_POINT *pSrcPoint1, R_POINT *pSrcPoint2, R_LINE *pLine );		//2点を通る直線の式
int R_calip_mpoint_open( int number );																//R_MPOINT初期化(ハンドルを返す)
void R_calip_mpoint_close( int hMpointHandle );														//R_MPOINT解放
int R_calip_mpoint_show_padd( int hMpointHandle );													//R_MPOINTハンドルから点群(R_POINT *)の先頭アドレスを得る
int R_calip_mpoint_show_curnumber( int hMpointHandle );												//R_MPOINTハンドルから点群(R_POINT *)の現在有効な点の数を得る
int R_calip_mpoint_show_maxnumber( int hMpointHandle );												//R_MPOINTハンドルから点群(R_POINT *)の最大格納可能な点の数を得る
int R_calip_convex_mpoint( int hMultiPoint, int hConvexPoint );										//R_MPOINTハンドル→R_MPOINTハンドルの凸包
int R_calip_draw_mpoint( int color, int hMultiHandle, int mode );									//R_MPOINTハンドルの点群描画
int R_calip_draw_connect_mpoint( int color, int hMultiHandle, int mode, ULONG pattern );			//R_MPOINTハンドルの点間を結ぶ線分の表示

int R_calip_line_l2d(R_LINE *pSrcLine,double radian,R_LINE *pDstLine);								//既知の直線に指定角度のなす直線を求める
double R_calip_degree_l2l(R_LINE *pLine1,R_LINE *pLine2);											//２直線のなす角を求める

int R_calip_intersection( R_LINE l1, R_LINE l2, R_POINT *pt );										//２直線の交点(256倍値)
int R_calip_intersection_fine( R_DLINE *l1, R_DLINE *l2, R_DPOINT *pt );							//２直線の交点(浮動小数点)
void R_calip_multi_point_compress( int nNumber, R_POINT *pPoint, int nScale );						//点群のスケールダウン
void R_calip_change_line_with_org( R_LINE *pSrcLine, R_LINE *pDstLine, R_POINT *pSrcOrg, R_POINT *pDstOrg );		//原点の移動に従って直線の方程式を再計算	//2009.08.07 by nakahara

void R_calip_point_swap( R_POINT *point1, R_POINT *point2 );
int R_calip_rotate_point_integer( R_POINT tSrcPoint, R_POINT tCenter, int nAngle, R_POINT *pDstPoint );		//点の回転

#ifdef __cplusplus
}
#endif

#endif	// R_CALIP_H_
