/******************************************************************************

	R_CS_RES.H

	検査結果管理(トレンドモニタ？)関数ヘッダ

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	Version		2004.07.06.000		オリジナル
				2006.05.09.001		平均、σの値が間違っていたのを修正
				2008.09.09			R_CS_ResultMonitorDisp()で描画する項目番号のチェックを追加

******************************************************************************/
#ifndef __R_CS_RES_H__
#define __R_CS_RES_H__

#include "R_stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- define ---
#define RESULT_MONITOR_MAX        8
#define RESULT_MONITOR_ITEM_MAX   1000
#define RESULT_MONITOR_SELECT_MAX 4

// --- define ---
//検査結果アイテム構造体
typedef struct{
	int		*pData;						//検査結果の値へのポインタ
	int		*pDataBase;					//検査結果の基準値へのポインタ
	int		*pDataBaseMin;				//検査結果の運用最小値へのポインタ
	int		*pDataBaseMax;				//検査結果の運用最大値へのポインタ
	int		*pDataLimitMin;				//検査結果のリミット最小値へのポインタ
	int		*pDataLimitMax;				//検査結果のリミット最大値へのポインタ
	int		nDataError;					//検査結果がエラーのときの値
	char	*sNameJp;					//検査結果の項目名（日本語）
	char	*sNameEn;					//検査結果の項目名（英語)
	int		nDataMin;					//グラフ表示用 Y軸最小値
	int		nDataMax;					//グラフ表示用 Y軸最大値
	int		nDataDiv;					//グラフ表示用 X軸分割数（棒グラフ）
}R_CS_RESULT_MONITOR_ITEM;

//検査結果グラフ表示構造体
typedef struct{
	R_RECT	tArea;						//描画エリア
	int		nTotalNum;					//描画するデータ配列の個数
	int		nItemNum[4];				//描画するデータの項目番号
	int		nDataMax;
	int		nDataMin;
}R_CS_RESULT_MONITOR_DISP;

//検査結果統計値構造体
typedef struct{
	int		*pBuffer;			//検査結果格納バッファ
	int		nMax;				//最大値
	int		nMin;				//最小値
	int		nRange;				//レンジ(最大値-最小値)
	double	dAverage;			//平均値
	double	dStdEv;				//標準偏差
}RESULT_MONITOR_RES;

//検査結果管理構造体
typedef struct{
	int					nDataMaxNum;	//検査結果を格納する最大値
	int					nItemNum;		//検査結果の個数(RESULT_MONITOR_ITEMの個数)
	int					nCurrentNum;	//格納バッファの現在の番号
	int					nRingBuffFlag;	//格納バッファが一巡したかをしめすフラグ
	R_CS_RESULT_MONITOR_ITEM	*pItem;			//検査結果アイテム構造体
	RESULT_MONITOR_RES	*pRes;			//検査結果統計値構造体

	int					nLastSaved;		//逐次保存のインデックス
}RESULT_MONITOR;


// --- variable ---

// --- prototype ---
int R_CS_ResultMonitorOpen( R_CS_RESULT_MONITOR_ITEM tItem[], int nMax );
int R_CS_ResultMonitorClose( int nHandle );
int R_CS_ResultMonitorCounterUp( int nHandle );
int R_CS_ResultMonitorCounterClear( int nHandle );
int R_CS_ResultMonitorDisp( int nHandle, R_CS_RESULT_MONITOR_DISP *pDispPara );
int R_CS_ResultMonitorSelectItem( int nHandle );
int R_CS_ResultMonitorFileSave( int nHandle, int nFileHandle, int nCounter );
int R_CS_ResultMonitorFileSaveSequential(int nHandle, char *filename, int nCounter );
int R_CS_ResultMonitorFileLoad(int nHandle, int nFileHandle, int nMax);

int R_CS_ResultMonitorGetCount(R_CS_RESULT_MONITOR_ITEM *tItem);
int R_CS_ResultMonitorAppend(R_CS_RESULT_MONITOR_ITEM *pTarget, R_CS_RESULT_MONITOR_ITEM *pItem);
int R_CS_SaveResultMonitorLimitValue(R_CS_RESULT_MONITOR_ITEM *pTarget, char *pFileName);
int R_CS_LoadResultMonitorLimitValue(R_CS_RESULT_MONITOR_ITEM *pTarget, char *pFileName);

RESULT_MONITOR **R_CS_GetResultMonitor();
#ifdef __cplusplus
}
#endif

#endif
