/******************************************************************************

	R_CS_CAL.H

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	Version		2006.05.09.000		オリジナル
	1.07b1.5	2009.02.02			#ifndef __R_CS_CAL_H__～追加
				2009.02.06			構造体R_CS_CALIB_FOCUS_PARAに以下のメンバ追加
										R_POINT	tDispPosition;		表示位置
										int		nColor;				表示色
******************************************************************************/
#ifndef __R_CS_CAL_H__
#define __R_CS_CAL_H__

// --- define ---
#define CALIB_DIR_INT			0x1			//内側へサーチ
#define CALIB_DIR_EXT			0x2			//外側へサーチ
#define CALIB_DIR_HORIZONTAL	0x10		//平行線を検出
#define CALIB_DIR_VERTICAL		0x20		//垂直線を検出

#define CALIB_MODE_SINGLE			0x00000001		//1本の線を検出
#define CALIB_MODE_DOUBLE			0x00000002		//2本の線を検出

#include "R_cs_gry.h"

typedef struct{
	int					*pRateX;							//X方向分解能(um/pixel * 1000)へのポインタ
	int					*pRateY;							//Y方向分解能(um/pixel * 1000)へのポインタ
	int					nTargetRateMin;						//目標倍率最小値(um/pixel * 1000)
	int					nTargetRateMax;						//目標倍率最大値(um/pixel * 1000)
	int					nPitchUm;							//測定するピッチの値[um]
	int					nXw;								//測定エリア幅　[pixel]
	int					nYw;								//測定エリア長さ[pixel]
	int					nDirection;							//サーチする方向(CALIB_DIR_INT or CALIB_DIR_EXT)
	int					nColor;								//サーチする色(ES_BLACK or ES_WHITE)
	R_CS_GRAY_ES_PARA	tEsPara;							//エッジサーチパラメータ構造体
	R_POINT				*pPoint;							//座標列用バッファ
	int					nMode;								//動作モード(CALIB_MODE_SINGLE or CALIB_MODE_DOUBLE)
} R_CS_CALIB_MAGNIFICATION_PARA;

typedef struct{
	void		(*pFunc)(R_POINT *pCenter, int *pDegree);	//画像取り込み&位置決め関数へのポインタ

	int			nLimitDegree;								//ワーク傾きリミット[deg](10倍値)
	int			nLimitGapX;									//ワーク位置ずれXリミット[pixel]
	int			nLimitGapY;									//ワーク位置ずれYリミット[pixel]
	R_POINT		tTargetPosi;								//ワーク中心　目標位置[pixel]

	R_POINT		tFocusAreaVector;							//ピント測定　測定エリア中心[pixel](ワーク中心からの相対座標)
	int			nFocusAreaXw;								//ピント測定　測定エリア幅　[pixel]
	int			nFocusAreaYw;								//ピント測定　測定エリア長さ[pixel]
	int			nFocusLimitLevel;							//ピント測定　目標エッジ濃度(0～255)
	int			nFocusEdgeDirection;						//ピント測定　エッジ向き(CALIB_DIR_HORIZONTAL or CALIB_DIR_VERTICAL)
	int			nFocusPageExe;								//ピント測定　対象グレーページ番号
	int			nFocusPageTemp;								//ピント測定　テンポラリ用グレーページ番号
	int			*pFocusBuffer;								//ピント測定　データ列用バッファ
} R_CS_CALIB_POSI_PARA;

typedef struct{
	R_RECT	tArea;										//測定エリア
	int		nLimitLevel;								//目標エッジ濃度(0～255)
	int		nPageExe;									//測定対象グレーページ番号
	int		nPageTemp;									//テンポラリ用グレーページ番号
	int		*pBuffer;									//データ列用バッファ
	void	(*Capture)(void);							//画像取り込み関数へのポインタ
	R_POINT	tDispPosition;								//<1.07b1.5> 表示位置
	int		nColor;										//<1.07b1.5> 表示色
} R_CS_CALIB_FOCUS_PARA;

// --- variable ---

// --- prototype ---
int R_CS_CalibrateMagnification(R_CS_CALIB_MAGNIFICATION_PARA *pPara);
void R_CS_CalibratePosition(R_CS_CALIB_POSI_PARA *pPara);
int R_CS_CalibrateFocus(R_CS_CALIB_FOCUS_PARA *pPara);

#endif
