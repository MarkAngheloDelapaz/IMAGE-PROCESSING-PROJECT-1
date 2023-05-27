/******************************************************************************

	R_CS_ANA.H

	画像解析関数ヘッダ
	バージョンアップ時は、必ず履歴の追加を行い、
	グループ内で変更内容をアナウンスすること！！

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	Version		2004.04.07.000		オリジナル
				2004.04.23.001		DrawPlane描画関数(R_draw系)を組み込み
				2004.10.06.002		ヒストグラム表示メニュー追加
				2006.05.01.003		画像処理デモメニュー追加
				2010.02.19			R_winb.hをincludeに追加

******************************************************************************/

// --- define ---
#include "R_winb.h"			//<1.07b2.7>  2010.02.19  by mastu

// --- variable ---
extern struct winb_t R_CS_WinbAnalysisMenu;

// --- prototype ---
int R_CS_analysis_open(void);
int R_CS_analysis_close(void);
