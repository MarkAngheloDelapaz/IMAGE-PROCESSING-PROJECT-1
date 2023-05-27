/******************************************************************************
	
	R_CS_FIL.H

	ファイル操作関数ヘッダ
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
	2004.04.07.000		オリジナル
	2004.04.21.001		DrawPlane描画関数(R_draw系)を組み込み
	2004.08.30.002		ディレクトリコピーを追加
	2007.11.20.003		ネットワーク接続関連関数追加
	2007.11.22.004		ディレクトリ削除メニュー、関連関数追加
	1.07b1.4	2008.08.07	以下の関数を追加
								R_CS_SaveGmd()
								R_CS_SaveGmdSetComment()
				2008.10.03	ディレクトリ削除メニューに選択削除追加
	1.07b1.5	2008.12.09	R_CS_LoadGmdWithComment追加
	1.07b2.3	2009.11.05	以下の関数名変更(旧名称はマクロ化)
								R_CS_FileBitmapSave
								R_CS_FileBitmapLoad
								R_CS_FileGmdSave
								R_CS_FileGmdSetComment
								R_CS_FileGmdLoad
								R_CS_FileGmdLoadWithComment
							以下の関数を追加
								R_CS_FileGmdSetAddress()
								R_CS_FileGmdSaveWithComment()
	1.07ｂ5.7	2014.09.04	komatsu
							画像保存/読込のパスを指定できるように変更
								R_CS_FileBitmapLoadWithPath
								R_CS_FileGmdLoadWithCommentAndPath
								R_CS_FileSetWorkDir
							ディレクトリを削除できるように変更
								R_CS_FileDeleteDir
	1.07ｂ7.5	2016.02.29	ディレクトリ削除日数指定対応	oshima
								R_CS_SetImageDirAutoDeleteKeepDay
******************************************************************************/

#ifndef __R_CS_FIL_H__
#define __R_CS_FIL_H__

#include "R_bitmap.h"

#ifdef __cplusplus
extern "C"
{
#endif


// --- define ---
typedef enum deltype{
	WITHOUT_DELETE = 0,
	WITHOUT_FILE,
	WITHOUT_GMD,
	WITHOUT_GMD_BMP,
	WITHOUT_GMD_BMP_CSV,
	SPECIFY_KEEPING_DAY,		//<1.07b7.5> oshima
	SPECIFY_KEEPING_DAY_WITH_TIMESTAMP		//<Ver.5.044> miyazaki
}FILE_DELTYPE;

// --- variable ---
extern struct winb_t R_CS_WinbFileMenu;

// --- prototype ---
//int R_CS_SaveBitmap(void);
//int R_CS_LoadBitmap(void);
//int R_CS_SaveGmd(void);													//<1.07b1.4>
//int R_CS_LoadGmd(void);
//int R_CS_LoadGmdWithComment(char *comment,int size);						//2008.12.9 oshima
//<1.07b2.3> 関数名変更
int R_CS_FileBitmapSave(void);												// 画像セーブ(BMP)
int R_CS_FileBitmapLoad(void);												// 画像ロード(BMP)
int R_CS_FileBitmapLoadWithPath(char *path);								// 画像ロード(BMP)						<1.07b5.7> パスを指定できるように変更	2014.09.04	komatsu
int R_CS_FileGmdSetAddress(unsigned char *pImage, int nType);				//<1.07b2.3> 画像(GMD)アクセス時の画像設定(アドレス指定)
int R_CS_FileGmdSetComment(char *pComment);									// 画像セーブ(GMD)時のコメント設定
int R_CS_FileGmdSave(void);													// 画像セーブ(GMD)
int R_CS_FileGmdSaveWithComment(char *pComment);							//<1.07b2.3> 画像セーブ(GMDコメント付き)
int R_CS_FileGmdLoad(void);													// 画像ロード(GMD)
int R_CS_FileGmdLoadWithComment(char *comment,int size);					// 画像ロード(GMDコメント付き)
int R_CS_FileGmdLoadWithCommentAndPath(char *path, char *comment, int size);// 画像ロード(GMDコメント付き,パス指定あり)	<1.07b5.7> パスを指定できるように変更	2014.09.04	komatsu
void	R_CS_FileGmdLoad_SetPostLoadFunc(void (*func)(int*, const char*, const char*));

int R_CS_FileSetWorkDir(char *path);										//<1.07b5.7> パスを指定できるように変更	2014.09.04	komatsu
int R_CS_FileDeleteDir(char *path, FILE_DELTYPE type, int msglevel);		//<1.07b5.7> ディレクトリ削除関数	2014.09.04	komatsu

//<1.07b2.3> 旧関数名互換用
#define R_CS_SaveBitmap				R_CS_FileBitmapSave
#define R_CS_LoadBitmap				R_CS_FileBitmapLoad
#define R_CS_SaveGmd				R_CS_FileGmdSave
#define R_CS_SaveGmdSetComment		R_CS_FileGmdSetComment
#define R_CS_LoadGmd				R_CS_FileGmdLoad
#define R_CS_LoadGmdWithComment		R_CS_FileGmdLoadWithComment

int R_CS_FileCopy(int src, int dst);
int R_CS_DirCopy(int src, int dst);

int R_CS_FileNetworkSetNetCommandDir(char *dir);		// NETコマンドディレクトリの設定  2007.11.20  by matsu
char* R_CS_FileNetworkGetNetCommandDir(void);			// NETコマンドディレクトリの参照  2007.11.20  by matsu
int R_CS_FileNetworkSetComputerName(char *name);		// 接続先コンピュータ名の設定  2007.11.20  by matsu
char* R_CS_FileNetworkGetComputerName(void);			// 接続先コンピュータ名の参照  2007.11.20  by matsu
int R_CS_FileNetworkSetDirectoryName(char *name);		// 接続先ディレクトリ名の設定  2007.11.20  by matsu
char* R_CS_FileNetworkGetDirectoryName(void);			// 接続先ディレクトリ名の参照  2007.11.20  by matsu
int R_CS_FileDeleteDirSetPath(char *path);				// ディレクトリ削除を行うパスの設定  2007.11.22
char* R_CS_FileDeleteDirGetPath(void);					// ディレクトリ削除を行うパスの参照  2007.11.22

int R_CS_SetFileLoadIdNumber(int nId);					//2008.12.15	基板複数枚挿し時、FileLoad先基板ID設定
int R_CS_GetFileLoadIdNumber(void);						//2008.12.15	基板複数枚挿し時、FileLoad先基板ID参照
char *R_CS_GetFileNameBuff(void);
int R_CS_SetFileNameBuff(char *filename);

int R_CS_SetImageDirAutoDeleteKeepDay(int nDay);		// 画像ディレクトリを残す日数指定　2016.02.29 oshima

#ifdef __cplusplus
}
#endif

#endif
