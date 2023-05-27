/******************************************************************************
	グラフィックプログラムライブラリー
			for COCZ39
	<R_dgraph.h>
	変更履歴
		Ver x.xx	2004. 3. 4	ベース作成	by saito
		Ver.3.48	2006.05.08	R_draw_lineで long pattern -> ULONG pattern に修正  by matsu
		Ver 3.49	2006/09/13	色指定変更
		Ver 3.49	2006/09/21	R_draw_cls()で全色消去モード追加	by saito
		Ver 3.49	2006/10/26	DRAW_BLUE,DRAW_MAGENTA,DRAW_CYAN追加
		Ver 3.50	2009/03/23	R_draw_box_fill関数追加,DRAW_HATCHING追加
		Ver 3.50	2009/04/02	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_DGRAPH_H_
#define	R_DGRAPH_H_

#include "R_stdlib.h"
#include "R_graph.h"
#include "R_font.h"

#ifdef __cplusplus
extern "C" {
#endif

/*	文字装飾指定
#define DISP_NORMAL			0
#define DISP_H2				0x0001
#define DISP_V2				0x0010
#define DISP_4				0x0011
#define DISP_H4				0x0012
#define DISP_V4				0x0021
#define DISP_16				0x0022
*/

//#define FONT32 32

//色指定
#define DRAW_ALL			0x00008000
//--- 20061116 ADD (KIS) ------------------------------
#define DRAW_BLACK			0x00000000
//--- 20061116 END ------------------------------------
#define DRAW_WHITE			0x00000007	// 0x00000000
#define DRAW_GREEN			0x00000002	// 0x00400000
#define DRAW_RED			0x00000004	// 0x00200000
#define DRAW_YELLOW			0x00000006	// 0x00600000
#define DRAW_BLUE			0x00000001	// CO-CZ-43 only
#define DRAW_MAGENTA		0x00000005	// CO-CZ-43 only
#define DRAW_CYAN			0x00000003	// CO-CZ-43 only
#define	DRAW_COLOR_MASK_39	0x00000007
#define	DRAW_COLOR_MASK_43	0x0000000F
#define DRAW_NORMAL			0x10000000
#define DRAW_ERASE			0x20000000
#define DRAW_REVERSE		0x30000000
#define DRAW_HATCHING		0x40000000

//プロトタイプ宣言
//int		R_draw_canvas_open( int board_num );
//int		R_draw_canvas_change( int board_id );
//--- 20061116 DEL (KIS) ------------------------------
int	R_draw_font_close( void );
//int	R_draw_font_close_user( int font_no );
//--- 20061116 DEL END --------------------------------

int		R_draw_chrdisp(int color,int mode, int posx, int posy, const char* p);
int		R_draw_chrdisp_system_font(int color,int mode, int posx, int posy, const char* p);
int		R_draw_printf(int color,int mode, int posx, int posy, const char* format, ...);
int		R_draw_printf_system_font(int color,int mode, int posx, int posy, char *format, ...);
int		R_draw_pset(int color, int posx, int posy, int mode);
int		R_draw_line(int color, int xs, int ys, int xe, int ye, int mode, ULONG pattern);	// long pattern -> ULONG pattern に修正  2006.05.08  by matsu
void	R_draw_system_box(void);
void	R_draw_system_title( char *title );

int		R_draw_printf(int color, int mode, int posx, int posy, const char* format, ...);
int		R_draw_printf_system_font(int color,int mode, int posx, int posy, char *format, ...);
int		R_draw_box(int color, R_RECT area, int mode, ULONG pattern);
int		R_draw_box_fill(int color, R_RECT area, int mode);
int		R_draw_circle(int color, int xpos, int ypos, int r, int mode);
int		R_draw_circle_fill(int color, int xpos, int ypos, int r, int mode);
int		R_draw_ellipse(int color, int xpos, int ypos, int lr, int sr, double st, int mode);
int		R_draw_disp_cross(int color, int xpos, int ypos, int size, int mode);
int		R_draw_disp_X(int color, int xpos, int ypos, int size, int mode);
int		R_draw_corner_box(int color, R_RECT area, int size);
int		R_draw_corner(int color, int xs, int ys, int xe, int ye, int size);
void	R_draw_reverse( int color, int x1, int y1, int x2, int y2 );
void	R_draw_cls( int color, int x1, int y1, int x2, int y2 );
void	R_clsd( void );
void	R_draw_line_equation( int color, R_RECT rect, R_LINE l );							//直線方程式の描画		2008.01.17 by naka


// DRAWプレーン系情報参照関数
int		R_draw_consult_font_x( int mode );
int		R_draw_consult_font_y( int mode );
int		R_draw_consult_system_font_x( int mode );
int		R_draw_consult_system_font_y( int mode );
int		R_draw_consult_canvas_x( void );
int		R_draw_consult_canvas_y( void );

#ifdef __cplusplus
}
#endif

#endif	// R_DGRAPH_H_
