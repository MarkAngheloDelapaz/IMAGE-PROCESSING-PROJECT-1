/******************************************************************************
　　マウスライブラリー（ｃｕｒｓｏｒ．ｃ）用ヘッダファイル

	ＦＯＲ　ＣＯ─ＣＺ─２３

	ＷＡＴＣＯＭ　Ｃ／３８６　＋　ＤＯＳ４Ｇ

	更新履歴
				1994/03/02	R_get_box_area関連関数を統括化
				1999/09/02	640x480対応(FX_SIZE系->FxSize系，Bin_x系の廃止
	Ver.3.48	2006.07.20	R_get_box_area_n R_get_circle_area_n 高解像度対応
	Ver 3.50	2009/01/30	DOS & LINUX ソース統合
	Ver 3.50	2009/02/02	内部関数r_mouse_～追加
	Ver 4.01	2011/06/30	R_draw_cursor、R_erase_cursor・アラインメント書き込み
	Ver 4.01	2013/08/27	R_cursor_set/get_speed関数追加
	Ver 4.01	2016/06/03	R_cursor_fit_to_display_window関数追加
******************************************************************************/

#ifndef R_CURSOR_H_
#define R_CURSOR_H_

#include "R_stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define		NO_TOUCH	0	// 下記の２つのキーとも押下なし
#define		EXECUTE		1	// EXECUTEキーが入力された
#define		CANCEL		2	// CANCELキーが入力された
#define		ERASE		0	// カーソルが消去中
#define		DRAW		1	// カーソルが表示中

extern R_RECT	RCursorWindow;
#define		Cursor_x1		(RCursorWindow.left)
#define		Cursor_x2		(RCursorWindow.right)
#define		Cursor_y1		(RCursorWindow.top)
#define		Cursor_y2		(RCursorWindow.bottom)

extern R_RECT	RCursorPrevWindow;
#define		OLD_Cursor_x1	(RCursorPrevWindow.left)
#define		OLD_Cursor_x2	(RCursorPrevWindow.right)
#define		OLD_Cursor_y1	(RCursorPrevWindow.top)
#define		OLD_Cursor_y2	(RCursorPrevWindow.bottom)

#ifdef	CURSOR
	unsigned int	csrbak[ 64 ];
	short	R_cursor_flag = 0;
	short	R_cursor_state = 0;
	unsigned int	csrdat[ 3 ][ 16 ] = {
			{ 0x00000003,	0x0000003f,	0x000001fe,	0x00000ffe,
			  0x0000007e,	0x000000fe,	0x000001fc,	0x000003ec,
			  0x000007cc,	0x00000f88,	0x00001f08,	0x00003e08,
			  0x00001c00,	0x00000800,	0x00000000,	0x00000000
			},
			{ 0x00000001,	0x00000006,	0x0000003e,	0x000001fc,
			  0x00000ffc,	0x000003fc,	0x000001f8,	0x000003f8,
			  0x000007f8,	0x00000fb0,	0x00001f10,	0x00003e00,
			  0x00001c00,	0x00000800,	0x00000000,	0x00000000
			},
			{ 0x00000007,	0x000001fe,	0x0000007e,	0x000001fc,
			  0x000007cc,	0x00001f08,	0x00001c00,	0x00000000,	
			  0,0,0,0,
			  0,0,0,0
			}
	};
#else
	extern unsigned int	csrbak[];
//	extern int		Cursor_x1;
//	extern int		Cursor_x2;
//	extern int		Cursor_y1;
//	extern int		Cursor_y2;
//	extern int		OLD_Cursor_x1,OLD_Cursor_x2,OLD_Cursor_y1,OLD_Cursor_y2;
	extern short	R_cursor_flag ;
	extern short	R_cursor_state;
	extern unsigned int	csrdat[3][16];
#endif

#ifdef __DOS__
	extern short	R_cursor_x;
	extern short	R_cursor_y;
	extern short	R_cursor_xb;
	extern short	R_cursor_yb;
#elif __GNUC__
	extern int		R_cursor_x;
	extern int		R_cursor_y;
	extern int		R_cursor_xb;
	extern int		R_cursor_yb;
#endif

/* ５１２×４８０用マウスカーソル
0000 0000 0000 0011
0000 0000 0011 1111
0000 0001 1111 1110
0000 1111 1111 1110
0000 0011 1111 1110
0000 0000 1111 1110
0000 0001 1111 1100
0000 0011 1111 1100
0000 0111 1101 1100
0000 1111 1001 1000
0001 1111 0000 1000
0011 1110 0000 1000
0001 1100 0000 0000
0000 1000 0000 0000
0000 0000 0000 0000
0000 0000 0000 0000
*/

/* ２５６×２４０用マウスカーソル
0000 0000 0000 0001
0000 0000 0000 0110
0000 0000 0011 1110
0000 0001 1111 1100
0000 1111 1111 1100
0000 0011 1111 1100
0000 0001 1111 1000
0000 0011 1111 1000
0000 0111 1111 1000
0000 1111 1011 0000
0001 1111 0001 0000
0011 1110 0001 0000
0001 1100 0000 0000
0000 1000 0000 0000
0000 0000 0000 0000
0000 0000 0000 0000
*/

#ifdef __DOS__
	void	R_move_cursor(short, short);		/* カーソルの移動		*/
	void	R_draw_cursor(short, short);		/* カーソルの表示		*/
#elif __GNUC__
	void	R_move_cursor(int, int);			/* カーソルの移動		*/
	void	R_draw_cursor(int, int);			/* カーソルの表示		*/
#endif
void	R_erase_cursor( void );					/* カーソルの消去		*/
void	R_cursor_on( void );
void	R_cursor_off( void );
int		R_get_position( void );
int		R_see_current_position( int *x, int *y );

/*!
*	@brief	カーソル状態の取得
*
*	@param	なし
*
*	@return	押しボタン状態を返します。また、現在座標をグローバル変数にセットします。
*			NO_TOUCH	押されていない<BR>
*			EXECUTE		左ボタンのみ押されている<BR>
*			CANCEL		右ボタンのみ押されている<BR>
*			EXECUTE|CANCEL	両方のボタンが押されている<BR>
*			現在x座標　→　int R_cursor_x<BR>
*			現在y座標　→　int R_cursor_y
*/
int		R_get_cursor( void );

int		R_get_box_area_n( int n, int *x, int *y, int *xl, int *yl, int xmax, int ymax, int xmin, int ymin );
int		R_get_box_area16( int *x, int *y, int *xl, int *yl, int xmax, int ymax, int xmin, int ymin );
int		R_get_box_area( int *x, int *y, int *xl, int *yl, int xmax, int ymax, int xmin, int ymin );
int		R_get_circle_area_n(int step, int *x, int *y, int *r, int rmax, int rmin);

//エリア設定オプション	//01/06/04 by ohshima
#ifdef R_CURSOR
	int R_cursor_BoxUnchangeFlag=0;
#else
	extern int R_cursor_BoxUnchangeFlag;
#endif
#define UNCHANGE_POSITION	0x01
#define UNCHANGE_SIZE		0x02
#define R_set_box_area_unchange_posi()		R_cursor_BoxUnchangeFlag|=UNCHANGE_POSITION
#define R_set_box_area_unchange_size()		R_cursor_BoxUnchangeFlag|=UNCHANGE_SIZE
#define R_set_box_area_default()			R_cursor_BoxUnchangeFlag=0

/************************************************************************
					R_curini.c
************************************************************************/
void	R_set_cursor_window( int, int, int, int );
int		R_init_cursor( void );					/* 初期化				*/
void	R_cursor_set_window(R_RECT* pWindow);
void	R_cursor_get_window(R_RECT* pWindow);

/*!
 * @brief R_cursor_fit_to_display_window
 * @brief 画像表示ウィンドウの座標で管理する。
 * @param yes	0以外：画像プレーン座標で管理する。
 *				0:DRAWプレーン座標で管理する。
 */
void	R_cursor_fit_to_display_window(int yes);

/*!
 * @brief R_cursor_is_fit_to_display_window
 * @retval	!0	画像プレーン座標で管理されている。
 * @retval	0	DRAWプレーン座標で管理されている。
 */
int		R_cursor_is_fit_to_display_window(void);

// ライブラリ内部関数
int		r_mouse_init(void);
void	r_mouse_set_position(int x, int y);
#ifdef __DOS__
int		r_mouse_get_position(short* px, short* py);
#elif __GNUC__
int		r_mouse_get_position(int* px, int* py);
#endif
void	r_mouse_set_window(R_RECT* pRect);
int		r_mouse_set_device(const char*);
char*	r_mouse_get_device(void);

#ifdef QT32
void	r_mouse_set_button_status(int button);
int		r_mouse_set_pos_status(int x, int y);
#else
int	R_cursor_set_speed(int rate);
int	R_cursor_get_speed(void);
#endif

/************************************************************************
					R_msd_yn.c
************************************************************************/
int		R_mouse_de_yesno(void);
int		R_mouse_de_yesno_with_brink_message( int y, char *p );

#ifdef __cplusplus	// 2010/11/11 by araki
}
#endif

#endif	// R_CURSOR_H_
