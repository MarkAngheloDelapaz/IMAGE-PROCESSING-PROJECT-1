/******************************************************************************
	R_efil.h / R_efil.c
	
	変更履歴
	Ver 3.50	2008.03.10	正式リリース
				2008.07.29	R_efil_get_result_memsize追加
	Ver 3.50	2009.02.12	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_EFIL_H_
#define	R_EFIL_H_

#include "R_stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

//結果構造体の型宣言(ユーザーで宣言された変数をR_efil_openに引渡して初期化)
typedef struct{
	int	nMemSize;				//全メモリサイズ
	int nMaxNumber;				//最大格納座標点数
	int nFoundNumber;			//検出座標点数
	R_POINT *pPoint;			//検出座標
	int *pValue;				//検出エッジ強度
	int *pAngle;				//検出エッジ勾配方向
}R_EFIL_RESULT;
//パラメータ構造体の型宣言(基本的には外部には現れない)
typedef struct{
	int nMemSize;				//全メモリサイズ
	int nGaussSigma;			//Gaussianカーネルのσ値
	int nGaussSize;				//Gaussianカーネルのサイズ
	R_RECT sRect;				//検査領域					（ユーザー変更可能）
	int nThresh;				//抽出点しきい値			（ユーザー変更可能）
	int *pMaskX;				//X偏微分型Gaussianカーネルの先頭アドレス
	int *pMaskY;				//Y偏微分型Gaussianカーネルの先頭アドレス
	int *pImageX;				//検査バッファ（１階Ｘ偏微分用）
	int *pImageY;				//検査バッファ（１階Ｙ偏微分用）
}R_EFIL_PARAMETER;
typedef struct{
	R_POINT sSrcPoint;			//拡大中心位置
	R_RECT	sDstRect;			//拡大表示領域
	int		nMag;				//拡大倍率
	R_POINT *pDotPoint;			//描画点群の座標先頭アドレス
	int		nDotNumber;			//描画点群数
	int		nDotMag;			//描画点座標の倍数
	int		nColor;				//描画点の色
	int		nMode;				//描画モード
}R_EFIL_DISP_EXPANSION;
//プロトタイプ宣言
int R_efil_parameter_open( int gauss_size, int gauss_sigma );				//検査パラメータの初期化
int R_efil_parameter_close( int nParaHandle );								//検査パラメータの終了
int R_efil_result_open( int max_number );									//結果格納先の初期化
int R_efil_result_close( int nResHandle );									//結果格納先の終了
int R_efil_set_area( int nParaHandle, R_RECT sRect );						//検査領域の設定
int R_efil_set_thresh( int nParaHandle, int nThresh );						//しきい値の設定
int R_efil_canny( int nParaHandle, int nResHandle, unsigned char *pImage );	//Cannyフィルタの実行
int R_efil_canny_with_nosense( int nParaHandle, int nResHandle, unsigned char *pImage, R_RECT *pNoSenseArea );	//2009.08.10 by nakahara added
int R_efil_get_result_number( int nResHandle );								//検出エッジ数の取得
R_POINT *R_efil_get_result_point( int nResHandle );							//検出エッジ座標のポインタ取得
int R_efil_point_compress( R_POINT *pPoint, int nNumber, int nComp  );		//エッジ座標の等分圧縮
int R_efil_get_para_memsize( int nParaHandle );								//検査パラメータのメモリサイズの取得
int R_efil_get_para_gsize( int nParaHandle );								//ガウシアンマスクのサイズ取得
int R_efil_get_para_gsigma( int nParaHandle );								//ガウシアンマスクのσ値取得
R_RECT *R_efil_get_para_rect( int nParaHandle );							//検査領域のポインタ取得
int R_efil_get_result_memsize( int nResHandle );							//検査結果のメモリサイズの取得			2008.07.29追加
int   R_efil_find_edge_on_line( int nParaHandle, unsigned char *pImage, R_POINT *p0, R_POINT *p1, R_POINT *rp, int mode );	//直線上のエッジエッジ検出
void R_efil_draw_set_expimage_srcpoint( R_EFIL_DISP_EXPANSION *pExDisp, R_POINT pPoint );	//画像拡大位置座標の設定
void R_efil_draw_set_expimage_dstrect( R_EFIL_DISP_EXPANSION *pExDisp, R_RECT pRect );		//画像表示先の領域設定
void R_efil_draw_set_expimage_mag( R_EFIL_DISP_EXPANSION *pExDisp, int nMag );				//画像拡大率の設定
void R_efil_draw_set_expimage_color( R_EFIL_DISP_EXPANSION *pExDisp, int nColor );			//点群表示色の設定
void R_efil_draw_set_expimage_mode( R_EFIL_DISP_EXPANSION *pExDisp, int nMode );			//拡大表示モードの設定
void R_efil_draw_set_expimage_dot( R_EFIL_DISP_EXPANSION *pExDisp, R_POINT *pPnt, int nPntNumber, int nPntMag );	//点群表示用データの設定
int R_efil_draw_disp_expimage( R_EFIL_DISP_EXPANSION *pExDisp, int nSrcNo );				//拡大表示実行
int R_efil_draw_disp_expimage_with_cursor( R_EFIL_DISP_EXPANSION *pExDisp, R_RECT sRect, int nSrcNo);	//拡大表示実行(カーソル操作付)

int		R_efil_do_c( unsigned char *pImage, int *pImageX, int *pImageY, int nGaussSize, int nGaussSigma, int xsize,int ysize ,short *maskX,short *maskY,int div);
int*	R_efil_get_result_angle( int nResHandle );
int*	R_efil_get_result_value( int nResHandle );

int	R_efil_dogfilter(int nParaHandle, unsigned char* pImage);	// 2009/07/30
int	R_line_correction(int v1, int v2, int v3, int v4, int p, int q, int max);	// 2009/07/30

// ライブラリ内部関数 R_ef_do.asm
int	R_efil_do(   unsigned char *pImage, int *pImageX, int *pImageY, int nGaussSize, int nGaussSigma, int xsize,int ysize ,short *maskX,short *maskY,int div);
int	R_efil_do11( unsigned char *pImage, int *pImageX, int *pImageY, int nGaussSize, int nGaussSigma, int xsize,int ysize ,short *maskX,short *maskY,int div);
#ifdef __GNUC__
	int	R_efil_do_(unsigned char* pImage, int* pImageX, int* pImageY, int nGaussSize, int nGaussSigma, int xsize, int ysize, short* maskX, short* maskY, int div);
	int	R_efil_do11_(unsigned char* pImage, int* pImageX, int* pImageY, int nGaussSize, int nGaussSigma, int xsize, int ysize, short* maskX, short* maskY, int div);
#endif

#ifdef __cplusplus
}
#endif
	
#endif	// R_EFIL_H_
