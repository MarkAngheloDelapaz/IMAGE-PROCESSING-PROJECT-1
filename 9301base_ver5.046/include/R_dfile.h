/******************************************************************************

	データファイル処理ライブラリヘッダファイル

	Ver.3.47	2006.02.10	登録
							<R_datafile_load(),R_save_datafile()のみ>
	Ver.3.48	2006.03.22	R_TYPE_POINT,R_TYPE_DPOINT,R_TYPE_RECT,R_TYPE_BOX,R_TYPE_LINE対応
							配列の保存に対応
	Ver.3.49	2006.09.27	ラベルの後ろにインデント用のスペースがある場合の読み書き対応
							R_datafile_save()の配列時の出力書式に間違いがあったので修正
	Ver.3.50	2007.05.25	R_datafile_save ラベルの空白除去した時もmallocを大きくする
				2007.10.10	R_datafile_save mallocサイズが間違っていたので修正
	Ver.4.01	2009.09.29	R_datafile_save_history()追加
	Ver.4.01	2012.09.04	R_datafile_saveバグフィクス
	Ver.4.02	2014.01.08	INT,POINT,RECT,BOXにおいてデータファイルの末尾に上限下限を追記できるように変更	by komatsu

******************************************************************************/

#ifndef	R_DFILE_H_
#define	R_DFILE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
// -> R_stdlib.h に登録
#define R_TYPE_INT			0	// an integral number (decimal number)
#define R_TYPE_CHAR			1	// character type
#define R_TYPE_DOUBLE		2	// double (decimal fraction format)
#define R_TYPE_HEX			3	// an integral number (hexadecimal number)
#define R_TYPE_STRING		4	// character string 
#define R_TYPE_SHORT		5	// short int (decimal number)
#define R_TYPE_SHORT_HEX	6	// short int (hexadecimal number)
#define R_TYPE_POINT		7	// R_POINT struct		2006.03.22 by matsu
#define R_TYPE_DPOINT		8	// R_DPOINT struct		"
#define R_TYPE_RECT			9	// R_RECT struct		"
#define R_TYPE_BOX			10	// R_BOX struct			"
#define R_TYPE_LINE			11	// R_LINE struct		"
*/


// R_datafile_save_history()
#define DATAFILE_DATA		0x0000		// 出力データ	データ出力
#define DATAFILE_LABEL		0x0001		// 				ラベル出力
#define DATAFILE_NEW_FILE	0x0010		// ファイルモード	ファイル新規作成
#define DATAFILE_NO_TIME	0x0100		// ログデータ	時間出力なし
#define DATAFILE_ALL_LOG	0x0200		// 				変更無し時のログも残す

//		2015.07.16	komatsu
typedef struct {
	int		*nMax;						// データ最大値(INT:0，POINT：0,1，RECT：0-3)
	int		*nMin;						// データ最小値(INT:0，POINT：0,1，RECT：0-3)
} R_DATAFILE_ITEM_LIMIT;

// パラメータファイル要素プロトタイプ
typedef struct {
	char	*pLabel;					// パラメータラベル
	void	*pData;						// パラメータ要素
	int		nType;						// パラメータ型種類
	int		nLength;					// バッファーサイズ
	int		nArray;						// データ配列数		2006.03.22 by mastu
	R_DATAFILE_ITEM_LIMIT	*pLimit;
//	int		nMax[4];					// データ最大値(INT:0のみ，POINT：0,1，RECT：0-3使用)		2014.01.08	komatsu
//	int		nMin[4];					// データ最小値(INT:0のみ，POINT：0,1，RECT：0-3使用)		2014.01.08	komatsu
} R_DATAFILE_ITEM;


int R_datafile_load(R_DATAFILE_ITEM *pList, const char* pFileName);
int R_datafile_save(R_DATAFILE_ITEM *pList, const char* pFileName);
int R_datafile_save_withMode(R_DATAFILE_ITEM *pList, const char* pFileName, char *sMode);
int R_datafile_save_history(R_DATAFILE_ITEM *pList, char *pFileName, int nMode);		// ver.4.01 2009.09.29

int R_datafile_chklimit(R_DATAFILE_ITEM *pList);
int R_datafile_openlimit(R_DATAFILE_ITEM *pList);
int R_datafile_closelimit(R_DATAFILE_ITEM *pList);

int R_datafile_count(R_DATAFILE_ITEM *pList);
int R_datafile_append(R_DATAFILE_ITEM *pTarget, R_DATAFILE_ITEM *pItem);
//R_DATAFILE_ITEM *R_datafile_connect(R_DATAFILE_ITEM *pItem0, R_DATAFILE_ITEM *pItem1);
int R_file_save_history(char *pLoadFileName, char *pSaveFileName, int nMode);		// 2019.11.29 kitamura

#ifdef __cplusplus
}
#endif

#endif	// R_DFILE_H_
