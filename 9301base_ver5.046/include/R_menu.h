/*

　画面操作ライブラリ（Ｒ＿ＭＥＮＵ．Ｃ）用ヘッダファイル

	ver			comment											name
	3.32b		R_select_file_change_max_fileｵｰﾌﾟﾝ				ohshima
	3.36b		R_popup_message追加								matsu		2003.05.13
	3.39		R_winb対応  		  							hirano		2003.06.24
	3.42		ドライブ変更をZ:まで対応						sassa		2004.04.14
	3.42		R_win_message_click(),R_win_message_wait()追加	shimizu		2004.05.02
	3.44		R_input_keyboard()にて大文字選択時'.'->':'、小文字選択時'.'->';'、'*'->'.'に変更 2004.10.06 by matsu
	3.48		static DriveNumber -> static int DriveNumberに修正	matsu		2006.05.08
	3.50		ソース内ではstatic宣言なので以下の関数の宣言削除	matsu	2007.10.31
					DrawKeyBoard, ReadKeyBoard, DrawKeySw
	3.50		DOS & LINUX ソース統合										2009.04.21
	4.01		R_select_file_with_timestamp関数追加						2015.12.15
*/

#ifndef	R_MENU_H_
#define	R_MENU_H_

#include "R_file.h"	// R_get_diskfree

#ifdef __cplusplus
extern "C" {
#endif

#ifdef	R_MENU
	int 	Menu_beep = OFF;

	struct fname_t	*MenuFile = NULL;
/*
	static char	MenuPath[ 128 ];

	static char	MenuDrive[ 3 ];
	static char	MenuDir[ 128 ];
	static char	MenuFname[ 9 ];
	static char	MenuExt[ 5 ];

	static char	MenuWork1[ 128 ];
	static char	MenuWork2[ 128 ];
	static char	MenuPathBak[ 128 ];
*/
#else
	extern	int Menu_beep;
	#define	R_menu_beep(n)	Menu_beep = n;
#endif







	#define	MENU_BEEP_TIME	20	//R_winb対応

enum error_pos {
	SELECT_FILE,
	SET_PARAMETER,
	WIN_MENU,
	PUSH_WINDOW,
	SCR_SWITCH,
	OPEN_WINDOW,
	WIN_MESSAGE,
	INPUT_STRING	//,
};

enum error_no {
	MALLOC_ERR,
	WIN_SIZE_ERR,
	SWITCH_NEST_ERR	//,
};

#define SCRSW_NEST		20
#define	SCRSW_MAX		1000
#define	PARA_MAX		100
#define	WIN_NEST_MAX	100

// メッセージ表示
#define POPUP_NON_CLOSE -1
#define POPUP_WAIT_CLICK 0

#define	CENTER	0
#define	UPPER	1
#define	LOWER	2


//　画面スイッチ用
#ifdef	R_MENU
int	ScrSw_select = 0;
int ScrSw_nest = -1;
int ScrSw_total = 0;
int ScrSw_top[ SCRSW_NEST ];
int ScrSw_num[ SCRSW_NEST ];
int	ScrSw_x1[ SCRSW_MAX ];
int	ScrSw_x2[ SCRSW_MAX ];
int	ScrSw_y1[ SCRSW_MAX ];
int	ScrSw_y2[ SCRSW_MAX ];
#else
extern int	ScrSw_select;
extern int ScrSw_nest;
extern int ScrSw_total;
extern int ScrSw_top[];
extern int ScrSw_num[];
extern int ScrSw_x1[];
extern int ScrSw_x2[];
extern int ScrSw_y1[];
extern int ScrSw_y2[];
#endif

#ifdef	R_MENU
int		Cursor_x1b;
int		Cursor_x2b;
int		Cursor_y1b;
int		Cursor_y2b;
#endif

//　ウインドウ設定用
struct win_t {
	char	*title;			/*　タイトル		*/
	int		xlen;			/*　ｘ方向文字数	*/
	int		ylen;			/*　ｙ方向文字数	*/
	int		n;				/*　選択項目数		*/
	char	*str[];			/*　文字列			*/
};

//　ウィンドウメッセイジ用
struct message_t {
	int		xlen;
	int		ylen;
	char	*str[];
};

//　パラメータ設定構造体
struct setpara_t {
	char	*para_name;
	long	*para_value;
	long	para_vmax;
	long	para_vmin;
	long	para_step;
};

//　パラメータ設定構造体(メッセージ付き）
struct setpara_tm {
	char	*para_name;
	int	*para_value;
	int	para_vmax;
	int	para_vmin;
	int	para_step;
	char	**menu;
};

//　ポップアップメニュー用
#ifdef R_MENU
static int	Menu_number = 0;
static struct {
	struct win_t *win;
	int		cx;
	int		cy;
	int		top;
} Menu[ WIN_NEST_MAX ];
#endif

//　ファイル名検索用構造体
struct fname_t {
	char	name[ 13 ];
	int		mode;
};

//　ファンクション設定用
#ifdef	R_MENU
struct message_t func_mes = { 14, 1, "function" };
#endif

//　画面サイズ
struct scrsize_t {
	int		font;
	int		xs;
	int		ys;
	int		xp;
	int		yp;
};

#ifdef R_MENU
int		Screen_xsize[ 4 ][ 3 ] = {
			{ 85, 64, 42 },
			{ 85, 64, 42 },
			{ 42, 32, 21 },
			{ 42, 32, 21 }
		};
int		Screen_ysize[ 4 ][ 3 ] = {
			{ 40, 30, 20 },
			{ 40, 30, 20 },
			{ 20, 15, 10 },
			{ 20, 15, 10 }
		};
#else
extern int Screen_xsize[ 3 ][ 3 ];
extern int Screen_ysize[ 3 ][ 3 ];
#endif

//	画面スイッチ関数
void	R_init_scr_switch( void );						/*　初期化					*/
void	R_close_scr_switch( void );						/*　終了処理				*/
void	R_erase_scr_switch( void );						/*　反転表示復元			*/
int		R_append_scr_switch( int, int, int, int );		/*　画面スイッチ追加		*/
int		R_check_scr_switch( void );						/*　画面スイッチチェック	*/

//　ウィンドウ関数
void	R_open_window( int, int, int, int );			/*　ウィンドウのオープン	*/
void	R_close_window( void );							/*　ウィンドウのクローズ	*/
void	R_win_message( int, int, struct message_t * );
void	R_push_window( void );
void	R_pop_window( void );
void	R_open_window2( int, int, int, int );
void	R_open_function(char* []);
void	R_close_function( void );

int R_WinMessage( int, char * );
int R_win_message_click( int, char * );
int R_win_message_wait( int, char *, unsigned int );


//　ファイル選択関数
int		R_select_file(const char*, char*, char*);
int		R_select_file_with_timestamp(const char* path, char* fname, const char* title);

//　パラメータ変更関数
int		R_set_parameter( struct setpara_t [], int, char * );
int		R_set_parameter2( struct setpara_t [], int, char * );
int R_set_parameter_with_mess( struct setpara_tm p[], char *title );

//	ポップアップｗｉｎｄｏｗ
int R_win_menu( int, int, struct win_t * );
void R_close_win_menu( void );

//　文字列入力
int R_input_string( int, int, char *, char *, char *, char * );
int R_input_data( int, int, int *, int, int, char * );
int R_input_keyboard(char*, const char*, const char*, int);


//	その他
int		R_get_file(const char*, struct fname_t [], int);	/*　ファイル＆ディレクトリー検索関数			*/
int		R_get_file_only( char *, struct fname_t [], int );	/*　ファイル検索関数			*/
void	R_cursor_window( int, int, int, int );
//unsigned long R_get_diskfree( int drive );
//3.32b	by ohshima
int 	R_select_file_change_max_file(int max);

void	strinp_screen( char *, int, char * );
void	original_screen( char *, char *[] );
void	selfile_screen( int, int );
void	menu_screen( char *[], int, int );
long	check_step( long );
void	renew_para( long *, int, int, int );
void	chgpara_screen( struct setpara_t *, long *, int, int );
void	conv_han2zen( char * );
void	put_win_str( int, int, struct win_t *, int );
void	winsel_screen( int, int, struct win_t * );
void	R_menu_error( int, int );
void	R_get_screen_size( struct scrsize_t * );
void	set_cursor_window( int, int, int, int );
void	undo_cursor_window( void );

void	makescr_selfile(char* title, char* fstr[]);
int		GetRealPath(const char* path);

char character_inc( char *, char );
char character_dec( char *, char );

/*
ソース内ではstatic宣言なので削除  2007.08.30  by matsu
void DrawKeyBoard(int , int , int );
int ReadKeyBoard(int );
void DrawKeySw(int , int , int , int );
*/

// メッセージ表示
int R_popup_message( int mode, char *format, ... );

#ifdef __DOS__	// 	2011.10.28	by matsunaga
#ifdef	R_MENU
	static struct win_t MenuChangeDrive = {
			"ドライブ選択",
			14,
			4,
			25,
			{
				"　Ａ　",
				"　Ｃ　",
				"　Ｄ　",
				"　Ｅ　",
				"　Ｆ　",
				"　Ｇ　",
				"　Ｈ　",
				"　Ｉ　",
				"　Ｊ　",
				"　Ｋ　",
				"　Ｌ　",
				"　Ｍ　",
				"　Ｎ　",
				"　Ｏ　",
				"　Ｐ　",
				"　Ｑ　",
				"　Ｒ　",
				"　Ｓ　",
				"　Ｔ　",
				"　Ｕ　",
				"　Ｖ　",
				"　Ｗ　",
				"　Ｘ　",
				"　Ｙ　",
				"　Ｚ　",
			}
	};

	static int DriveNumber[ 25 ] = {
		1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26
	};		// static DriveNumber -> static int DriveNumber  2006.05.08  by matsu

#endif
#endif

//ファイル最大数指定(VGAモードも兼用)
#ifdef	R_MENU
int nMaxSelectFile = 200;		//	98.10.22 (KATO)
#else
extern int nMaxSelectFile;		//	98.10.22 (KATO)
#endif

// 2013.04.19
int	R_input_keyboard_set_max_string_length(int length);
int	R_input_keyboard_get_max_string_length(void);

#ifdef __cplusplus	// 2010/11/11 by araki
}
#endif

#endif	// R_MENU_H_
