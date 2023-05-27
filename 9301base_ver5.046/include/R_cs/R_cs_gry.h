/******************************************************************************

	R_CS_GRY.H

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	Version		2006.05.09.000		オリジナル
	1.07b1.5	2009.02.06			#ifndef __R_CS_GRY_H__～追加

******************************************************************************/
#ifndef __R_CS_GRY_H__
#define __R_CS_GRY_H__

#ifdef __cplusplus
extern "C" {
#endif


// --- define ---
// エッジサーチの方向(dir)
#define	ES_UP		0x01		//↑
#define	ES_DOWN		0x02		//↓
#define	ES_RIGHT	0x04		//→
#define	ES_LEFT		0x08		//←

// 円領域をエッジサーチするときの方向
#define	ES_CIR_EXT	0x01		//円外側にサーチ
#define	ES_CIR_INT	0x02		//円内側にサーチ

#define ES_GRAY		0x0010
#define ES_BIN		0x0020

#define ES_ROUND	0x0100		// グレーエッジサーチの丸め込み
#define ES_DEBUG	0x8000		// エッジサーチ点表示

typedef struct {
	int page;			// サーチページ
	int level;			// エッジ最小階調差
	int average;		// 平均化回数
	int noise;			// ノイズ幅
	int slice;			// エッジ検出レベル
} R_CS_GRAY_ES_PARA;

typedef struct {
	R_POINT		tCenter;		//サーチエリアの中心座標[pixel]
	int			nRadius;		//サーチエリアの中心からサーチ開始点までの距離[pixel]
	int			nLength;		//サーチする距離[pixel]
	int			nDirection;		//サーチ方向(ES_CIR_EXT or ES_CIR_INT)
	int			nDegreeStart;	//サーチ開始角度[deg]
	int			nDegreeEnd;		//サーチ終了角度[deg]
	int			nDegreeStep;	//サーチ角度ステップ[deg]
} R_CS_GRAY_ES_CIR_PARA;

typedef struct {
	R_POINT		tCenter;		//サーチエリアの中心座標[pixel]
	R_POINT		*pStart;		//サーチ開始点　座標列[pixel]
	int			nNumber;		//サーチ開始点数
	int			nLength;		//サーチする距離[pixel]
	int			nDirection;		//サーチ方向(ES_CIR_EXT or ES_CIR_INT)
} R_CS_GRAY_ES_CURVE_PARA;

// --- variable ---

// --- prototype ---
void R_CS_GrayEdgeSearchSetParameter(R_CS_GRAY_ES_PARA para);
int R_CS_GrayEdgeSearchArea(R_POINT *edge, R_RECT area, int step, int dir, int mode);
int R_CS_GrayEdgeSearchCircle( R_POINT *pEdge, R_CS_GRAY_ES_CIR_PARA *pPara, int nMode );
int R_CS_GrayEdgeSearchCurve( R_POINT *pEdge, R_CS_GRAY_ES_CURVE_PARA *pPara, int nMode );

#ifdef __cplusplus
}
#endif
#endif
