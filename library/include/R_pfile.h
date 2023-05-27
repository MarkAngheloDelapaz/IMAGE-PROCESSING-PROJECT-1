/******************************************************************************
	R_PFILE.H / R_PFILE.C
	
	変更履歴
	Ver 3.41	2003/12/27	正式リリース
	Ver 3.42	2004/01/29	ドライブ移動対応
	Ver 3.47	2005/11/17	PFILE_SEQ_FILE_MAX_NB 4->64 に変更
	Ver 3.48	2006/05/08	static SetFileNameNumber -> static int SetFileNameNumber 修正
	Ver 3.48	2006/07/13	SVGA表示対応
	Ver 3.49	2007/03/06	DRAW_PLANE表示対応
				2007/03/06	文字列等整理
				2007/03/07	r_pfile_sequential_view()のロード順を変更(ロードの高速化)
	Ver 3.50	2007/04/21	R_pfile_sequential_load_and_call_on_subdir()のマウス設定可能に
	Ver 3.50	2009/04/06	DOS & LINUX ソース統合
	Ver 4.01	2011/06/01	最大ファイル数設定対応
	Ver 4.01	2013/11/03	ファイルOTHER形式対応
	Ver 4.01	2013/11/12	ファイルCMD形式対応
	Ver 4.01	2014/12/10	R_pfile_sequential_set_func_otherの追加
******************************************************************************/

#ifndef	R_PFILE_H_
#define	R_PFILE_H_

#ifdef __cplusplus
extern "C" {
#endif

//画像ファイルの種類
#define FILE_GMD 	0x00000000				//GMDファイル
#define FILE_BMP 	0x00000001				//BMPファイル
#define FILE_OTHER	0x00000002				//OTHERファイル
#define FILE_CMD	0x00000004				//CMDファイル

//メニューの項目番号
#define PFILE_SEQ_MENU1 0	//メニュー番号1
#define PFILE_SEQ_MENU2 1	//メニュー番号2
#define PFILE_SEQ_MENU3 2	//メニュー番号3

//表示モード
#define PFILE_SEQ_NO_WAIT		0x00000000	//マウスクリック待ちなし
#define PFILE_SEQ_WAIT_CLICK	0x00000001	//常にマウスクリック待ち
#define PFILE_SEQ_WAIT_ERROR	0x00000002	//実行関数がERROR時のみマウスクリック待ち
#define PFILE_SEQ_WAIT_OK		0x00000004	//実行関数がOK時のみマウスクリック待ち
#define PFILE_SEQ_MODE_SUBDIR	0x10000000	//R_pfile_sequential_load_and_call_on_subdir時も設定を有効にする

#define PFILE_SEQ_FILE_NAME_MAX	16			//ファイル名の最大文字数
#define PFILE_SEQ_PATH_NAME_MAX	256			//パス名の最大文字数
#define PFILE_SEQ_EXTENSION_NAME_MAX	5	//拡張子名の最大文字数

typedef struct{								//PFILE_SEQデータ構造体
	char name   [PFILE_SEQ_PATH_NAME_MAX];	//現在ロードしているファイル名(12文字まで)
	char path   [PFILE_SEQ_PATH_NAME_MAX];	//現在ロードしているファイルのフルパス(256文字まで)
	char comment[PFILE_SEQ_PATH_NAME_MAX];	//現在ロードしているファイルのコメント(GMD形式のみ,256文字まで)
}PFILE_SEQ_INFO;

//関数プロトタイプ宣言
int R_pfile_sequential_load_and_call(void);
int R_pfile_sequential_load_and_call_with_path(char *currentpath);
int R_pfile_sequential_load_and_call_on_subdir(void);
int R_pfile_sequential_set_filetype( int number, int file_type );
int R_pfile_sequential_set_filename( char *name, int page );
int R_pfile_sequential_set_func( int (*func)(PFILE_SEQ_INFO *info) );
int R_pfile_sequential_set_func_dir( int (*dir_func)(char *path) );
int R_pfile_sequential_set_menu( int number, char *j, char *e, void (*func)(void) );
int R_pfile_sequential_set_mode( int mode );
int R_pfile_sequential_set_dirname( char *dir_name );
int R_pfile_sequential_set_file_extension( char *extension_name );
int R_pfile_sequential_set_disp_time( int disp_time );
void R_pfile_set_svga_mode(void);
void R_pfile_set_draw_plane_mode(void);
void R_pfile_set_max_file_number(int number);
int R_pfile_sequential_set_func_other( int (*other_func)(PFILE_SEQ_INFO *info) );

#ifdef __cplusplus
}
#endif

#endif	// R_PFILE_H_
