//--------------------------------------------------------------------
//	ＳＶＧＡマウスライブラリ（Ｒ＿ｓｃｕｒｓｏｒ．ｃ）用ヘッダファイル
//
//	ＷＡＴＣＯＭ　Ｃ／３８６　＋　ＤＯＳ４Ｇ
//--------------------------------------------------------------------

#ifndef	R_SCURSO_H_
#define	R_SCURSO_H_

#ifdef __cplusplus
extern "C" {
#endif

#define	NO_TOUCH	0	// 下記の２つのキーとも押下なし
#define	EXECUTE		1	// EXECUTEキーが入力された
#define	CANCEL		2	// CANCELキーが入力された
#define	ERASE		0	// カーソルが消去中
#define	DRAW		1	// カーソルが表示中

#ifdef SVGA_CURSOR
	int	R_SVGA_cursor_flag	= 0;
	int	R_SVGA_cursor_x		= 0;
	int	R_SVGA_cursor_y		= 0;
	int	R_SVGA_cursor_xb	= 0;
	int	R_SVGA_cursor_yb	= 0;
	int	R_SVGA_cursor_state	= 0;
	int	R_SVGA_cursor_BoxAreaWindowAxisX	= 0;
	int	R_SVGA_cursor_BoxAreaWindowAxisY	= 0;
	int	R_SVGA_cursor_BoxAreaWindowEndAxisX	= 0;
	int	R_SVGA_cursor_BoxAreaWindowEndAxisY	= 0;
	
	int	R_SVGA_cursor_BoxUnchangeFlag = 0;	//01/06/04 by ohshima
#else
	extern int	R_SVGA_cursor_flag;
	extern int	R_SVGA_cursor_x;
	extern int	R_SVGA_cursor_y;
	extern int	R_SVGA_cursor_xb;
	extern int	R_SVGA_cursor_yb;
	extern int	R_SVGA_cursor_state;
	extern int	R_SVGA_cursor_BoxAreaWindowAxisX;
	extern int	R_SVGA_cursor_BoxAreaWindowAxisY;
	extern int	R_SVGA_cursor_BoxAreaWindowEndAxisX;
	extern int	R_SVGA_cursor_BoxAreaWindowEndAxisY;

	extern int	R_SVGA_cursor_BoxUnchangeFlag;	//01/06/04 by ohshima
#endif

void	R_SVGA_move_cursor(int x, int y);	// VGAマウスカーソルの移動
void	R_SVGA_draw_cursor(int x, int y);	// VGAマウスカーソルの表示
void	R_SVGA_erase_cursor(void);				// VGAマウスカーソルの消去
int		R_SVGA_get_cursor(void);				// マウスカーソル状態の取得
int		R_SVGA_get_position(void);				// マウスカーソルによる座標指定
void	R_SVGA_cursor_on(void);					// VGAマウスカーソル表示オン
void	R_SVGA_cursor_off(void);				// VGAマウスカーソル表示オフ

int		R_SVGA_get_box_area_n(int step, int* xs, int* ys, int* xw, int* yw, int xmax, int ymax, int xmin, int ymin);
#define	R_SVGA_get_box_area16(xs, ys, xw, yw, xmax, ymax, xmin, ymin)	R_SVGA_get_box_area_n(16, xs, ys, xw, yw, xmax, ymax, xmin, ymin)
#define	R_SVGA_get_box_area(xs, ys, xw, yw, xmax, ymax, xmin, ymin)		R_SVGA_get_box_area_n(1, xs, ys, xw, yw, xmax, ymax, xmin, ymin)

int		R_SVGA_get_circle_area_n(int step, int *x, int *y, int *r, int rmax, int rmin);

//エリア設定オプション	//01/06/04 by ohshima
#define UNCHANGE_POSITION	0x01
#define UNCHANGE_SIZE		0x02
#define R_SVGA_set_box_area_unchange_posi()		R_SVGA_cursor_BoxUnchangeFlag|=UNCHANGE_POSITION
#define R_SVGA_set_box_area_unchange_size()		R_SVGA_cursor_BoxUnchangeFlag|=UNCHANGE_SIZE
#define R_SVGA_set_box_area_default()			R_SVGA_cursor_BoxUnchangeFlag=0

#ifdef __cplusplus
}
#endif

#endif	// R_SCURSO_H_
