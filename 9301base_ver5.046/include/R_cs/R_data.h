/******************************************************************************

	データファイル　セーブ・ロード関数ヘッダ
	バージョンアップ時は、必ず履歴の追加を行い、
	グループ内で変更内容をアナウンスすること！！

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	Version		2004.03.15.000
				2004.04.23.001		DrawPlane描画関数(R_draw系)を組み込み

				2006.09.12.002		R_DataLoad/Savefileをライブラリ関数の
									R_datafile_load/saveに変更
				2007.11.05.003		r_cs_DataFileDisp追加(R_DataFileDispはlegacy扱い)
******************************************************************************/
#ifndef __R_DATA_H__
#define __R_DATA_H__

#include "R_dfile.h"
// --- define ---
// --- パラメータファイル構造体 ---
// TSC-9301ライブラリ化
/*
#define ITEM_LENGTH_MAX		128		// 1行の最大文字数

#define ERR_FILE_OPEN	-1			// ファイルオープンエラー
#define ERR_FILE_READ	-2			// データ読み込みエラー
#define ERR_FILE_LABEL	-3			// ラベル不一致エラー
#define ERR_FILE_DATA	-4			// データ変替えラー

#define TYPE_INT	0				// int型
#define TYPE_CHAR	1				// char型
#define TYPE_DOUBLE	2				// double型
#define TYPE_HEX	3				// int(16進)型
#define TYPE_STRING	4				// 文字列型

typedef struct {
	char *label;					// パラメータラベル
	void *para;						// パラメータ要素
	int type;						// パラメータ型種類
} R_PARA_FILE_ITEM;					// パラメータファイル要素プロトタイプ
*/
// --- variable ---

// --- prototype ---
//int R_DataLoadfile(R_PARA_FILE_ITEM *item, char *file_name);
//int R_DataSavefile(R_PARA_FILE_ITEM *item, char *file_name);
void R_DataFileDisp(R_DATAFILE_ITEM *item, char *tytle);				// legacy
int r_cs_DataFileDisp(char *fname);										// 2007.11.05  by matsu

#endif
