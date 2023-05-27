/******************************************************************************

	R_CS_SAV.C	

	自動画像セーブ関数ヘッダ
	バージョンアップ時は、必ず履歴の追加を行い、
	グループ内で変更内容をアナウンスすること！！

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	Version
	2004.03.15.000
	2007.11.16.001	matsu		R_CS_SaveImgAutoSetDriveFreeSpaceCheck追加
	1.07b1.4	2008.08.08		R_CS_SaveImgAutoSetAddress()追加
	1.07b3.1	2011.05.17		R_CS_SaveImgAutoGetLast系追加(画像自動セーブ最終状態取得)
	1.07b6.6	2015.06.09		R_CS_SaveImgAutoSetDirectoryName()追加
	1.07b8.0	2016.04.25		R_CS_SaveImgGetLastFileName()追加
	5.004		2018.05.21		ImagePath.csvのヘッダーをマクロ定義
	5.004		2018.05.29		R_CS_SaveImgAutoGetRootDirName関数追加
	5.019		2019.10.30		R_CS_SaveDataAutoGetRootDirName関数追加

******************************************************************************/

#ifndef R_CS_SAV_H
#define R_CS_SAV_H

#ifdef __cplusplus
extern "C"{
#endif


// --- define ---
#include "R_cs.h"
#include <stdarg.h>

//初期化フラグ(HDD保存orメモリ保存)
#define SAVE_IMG_OPEN_HDD    0x0000
#define SAVE_IMG_OPEN_MEMORY 0x0001
#define SAVE_IMG_MEM_64      0x0010
#define SAVE_IMG_MEM_128     0x0020
#define SAVE_IMG_MEM_256     0x0030

//保存モード
#define SAVE_IMG_MAX			0x0000		//最大枚数までセーブしたら、保存しない
#define SAVE_IMG_RING			0x0001		//最大枚数までセーブしたら、カウンタを0にしてセーブ続行
#define SAVE_IMG_RANDOM			0x0002		//<1.07b3.0> 最大枚数までセーブしたら、ランダムモードでセーブ続行

#define SAVE_IMG_FAST			0x0010		//ディスクアクセスが遅くなる前にディレクトリ切り替えを行う

#define SAVE_IMG_METHOD			0xFF00		//<1.07b3.0> 保存方法マスク
#define SAVE_IMG_OFF			0x0000		//<1.07b3.0> 保存しない
#define SAVE_IMG_ALL			0x0100		//<1.07b3.0> 全画像保存
#define SAVE_IMG_LIST			0x0200		//<1.07b3.0> 指定番号保存
#define SAVE_IMG_NO_LIST		0x0300		//<1.07b3.0> 指定番号以外保存
#define SAVE_IMG_DIR_ALL		0x0400		//<1.07b3.0> 全画像分類保存
#define SAVE_IMG_DIR_LIST		0x0500		//<1.07b3.0> 指定番号分類保存
#define SAVE_IMG_DIR_NO_LIST	0x0600		//<1.07b3.0> 指定番号以外分類保存


#define SAVE_IMG_INIMODE_DEFAULT		0x00000000
#define SAVE_IMG_INIMODE_NOMKDIR		0x00000001
#define SAVE_IMG_INIMODE_MKDIR_DATE_LOT	0x00000010		//ディレクトリ名に日時とLotNoを入れるモード 2018.03.29 by miyazaki

#define	SAVE_IMG_DIRECTORY_NAME_FORMAT	"%Y%m%d_%H%M%S"	// YYYYmmdd_HHMMSS
#define	SAVE_IMG_STRLEN_OF_DIRECTORY_NAME_FORMAT	15

#define SAVE_IMG_IMAGE_PATH_CSV_HEADER "Total,CapNo,NgCode,ImageName\n" //ImagePath.csvのヘッダー 20180521 miyazaki

/* memo
	type						method					list
	保存しない					SAVE_IMG_OFF			-
	全画像保存					SAVE_IMG_ALL			-
	NG画像保存					SAVE_IMG_NO_LIST		0
	OK画像保存					SAVE_IMG_LIST			0
	NG画像保存(ワーク無し除く)	SAVE_IMG_NO_LIST		0,1
	全画像保存(ワーク無し除く)	SAVE_IMG_NO_LIST		1
	全画像分類保存				SAVE_IMG_DIR_ALL,		-
	NG画像分類保存				SAVE_IMG_DIR_NO_LIST	0
	指定番号分類保存			SAVE_IMG_DIR_LIST		n1,n2,n3,...
*/

// --- prototype (public)---
int R_CS_SaveImgAutoExecute(int number, char *format, ...);
int R_CS_SaveImgAutoExecuteCode(int number, int code, const char *format, ...);	//<1.07b3.0> 画像自動セーブ実行(ＮＧコード判定有り)
int R_CS_SaveImgAutoExecuteCodeV(int number, int code, const char *format, va_list arglist);
int R_CS_SaveImgAutoExecuteCode_withList(const R_CS_COUNTER_DATA *monitor, const R_CS_RESULT_DATA *result, int number, int code, const char *format, ...);
int R_CS_SaveImgAutoExecuteCode_withListV(const R_CS_COUNTER_DATA *monitor, const R_CS_RESULT_DATA *result, int number, int code, const char *format, va_list arglist);
																				//<1.07b9.6> 画像自動セーブ実行(ＮＧコード判定有り,リスト保存あり)
int R_CS_SaveImgAutoInitial(void);
int R_CS_SaveImgAutoInitial_withMode(int mode);
int R_CS_SaveImgAutoMemoryExport(void);
int R_CS_SaveImgAutoSetDirName(char *dir_name);
int R_CS_SaveImgAutoSetImgDirName(char *dir_name);
int R_CS_SaveImgAutoSetDataDirName(char *dir_name);
int R_CS_SaveImgAutoSetFileName(int number, char *name, int page);
int R_CS_SaveImgAutoSetAddress(int number, unsigned char *pAddress);	//<1.07b1.4> 画像セーブする画像アドレスの設定  2008.08.08
int R_CS_SaveImgAutoSetMaxNum(int max);
int R_CS_SaveImgAutoSetMode(int mode);
int R_CS_SaveImgAutoSetCodeList(int num, int *list);					//<1.07b3.0> ＮＧコードリストの設定
int R_CS_SaveImgAutoSetSkipNumber(int num);								//<1.07b3.0> 画像保存スキップ数
int R_CS_SaveImgAutoSetNgCodeMax(int number);							//<1.07b3.8> ＮＧコード最大番号の設定	2011.10.27
int R_CS_SaveImgAutoGetNgCodeMax(void);									//<1.07b3.8> ＮＧコード最大番号の取得	2011.10.27

int R_CS_SaveImgAutoOpen(int open_flag);
int R_CS_SaveImgAutoClose(void);

const char* R_CS_SaveImgAutoGetRootDirName(void);
char* R_CS_SaveImgAutoGetWorkDirName(void);
const char* R_CS_SaveDataAutoGetRootDirName(void);				// ver5.019
char* R_CS_SaveDataAutoGetWorkDirName(void);
int R_CS_SaveImgAutoGetSaveCount(int nNum);
int R_CS_SaveImgAutoSetDriveFreeSpaceCheck(int mode);			// 空き容量チェック(デフォルト：する)  2007.11.16

int R_CS_SaveImgAutoGetLastCode(void);									//<1.07b3.1> 画像自動セーブ最終状態取得(ＮＧコード)
int R_CS_SaveImgAutoGetLastNumber(void);								//<1.07b3.1> 画像自動セーブ最終状態取得(画像ファイル番号)
char* R_CS_SaveImgAutoGetLastDirName(void);								//<1.07b3.1> 画像自動セーブ最終状態取得(画像保存ディレクトリ)

void	R_CS_SaveImgSetOverWriteEnable(int enable);						// 2015.05.29

int R_CS_SaveImgAutoSetDirectoryName(char *name);						//<1.07b6.6>保存ディレクトリ名の設定
int R_CS_SaveImgAutoChangeDirectoryName(char *name, int digit);
const char* R_CS_SaveImgGetLastFileName(void);							//<1.07b8.0> 最新の画像保存ファイル名の取得
void R_CS_SaveImgClearLastFileName(void);								//<1.07b8.0> 最新の画像保存ファイル名のクリア

void R_CS_SaveImgResumeON(void);

int R_CS_SaveImgAutoCheckCode(int code);

int R_CS_MakeImagePathEmptyFile(void);

#ifdef __cplusplus
}
#endif

#endif
