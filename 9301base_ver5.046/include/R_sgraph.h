/******************************************************************************
	R_sgraph.h /R_sgraph.c
	
	変更履歴
	Ver.3.48	2006/05/08	R_SVGA_boxの座標の引き数を short -> int に変更	by matsu
							(ヘッダの宣言と違っていた)
	Ver.3.49	2006/10/18	OKとERROR定義を削除、R_stdlib.hをインクルード	by araki
	Ver 3.50	2009/01/29	DOS & LINUX ソース統合
******************************************************************************/

#ifndef R_SGRAPH_H_
#define R_SGRAPH_H_

#include "R_stdlib.h"
#include "R_video.h"
#include "R_graph.h"

#ifdef __cplusplus
extern "C" {
#endif

//change
#ifdef SVGA_DRAW
#define RED				( int )0x00ff0000
#define GREEN			( int )0x0000ff00
#define BLUE			( int )0x000000ff
#define CYAN			( int )0x0000ffff
#define MAZENTA			( int )0x00ff00ff
#define YELLOW			( int )0x00ffff00
#endif

//change 99/6/10*****************
#ifdef SVGA_CHR_MODE
	int chrdisp_mode=0;
#else
	extern int chrdisp_mode;
#endif

#define OVER_LAY 0
#define RE_WRITE 1
#define	R_SVGASetChrdispMode(mode) chrdisp_mode=mode
#define	R_SVGASetChrdispMode(mode) chrdisp_mode=mode
//change *************************

#ifdef	SVGA_GRAPH
	int	SvgaDispGrayBarAxisX = 0;
	int	SvgaDispGrayBarAxisY = 0;
#else
	extern int	SvgaDispGrayBarAxisX;
	extern int	SvgaDispGrayBarAxisY;
#endif




int	R_SVGA_chrdisp(int xpos, int ypos, char *p);
int R_SVGA_chrdisp_hfont(int xpos, int ypos, char code);
int R_SVGA_chrdisp_zfont(int xpos, int ypos, unsigned short code);

int	R_SVGA_chrdisp_center_scale(int line, char *p);
int R_SVGA_invdisp(int xpos, int ypos, char *p);
int R_SVGA_invdisp_hfont(int xpos, int ypos, char code);
int R_SVGA_invdisp_zfont(int xpos, int ypos, unsigned short code);
int R_SVGA_chrdisp_modify(int xpos, int ypos, char *p, int mode);
int R_SVGA_chrdisp_modify_hfont(int xpos, int ypos, char code, int mode);
int R_SVGA_chrdisp_modify_zfont(int xpos, int ypos,
								unsigned short code, int mode);


void R_SVGA_circle(int x, int y, int r, int mode);
void R_SVGA_circle_fill(int x, int y, int r, int mode);
void R_SVGA_ellipse(int x, int y, int a, int b, double t, int mode);

void R_SVGA_pset(int x, int y, int mode);
void R_SVGA_line(int x1, int y1, int x2, int y2, int mode, long pat);
void R_SVGA_bin_fill(int p, int x, int y, int mode);
void R_SVGA_gray_fill(int x1, int y1, int x2, int y2, int level);
void R_SVGA_bin_reverse(int x1, int y1, int x2, int y2);
int  R_SVGA_dsp_gray_bar(void);


int  R_SVGA_get(int xs, int ys, int xe, int ye, unsigned long *buf);
int  R_SVGA_put(unsigned long *buf);


void R_SVGA_chrdisp_system_scale_font(int posx, int posy, char *p);
void R_SVGA_chrdisp_system_font(int posx, int posy, char *p);
void R_SVGA_chrdisp_system_small_font(int posx, int posy, char *p);
int  R_SVGA_chrdisp_center(int line, char *p);
void R_SVGA_box(int x1, int y1, int x2, int y2, int mode, long pat);
int	R_SVGA_box_push( int x, int y, int xe, int ye, int mode, int pat );
int	R_SVGA_box_pop( void );
void	R_SVGA_erase_system_font( int x1, int y1, int length );
void	R_SVGA_erase_font( int x, int y, int length_font );
int	R_SVGA_draw_charactor(int xpos, int ypos,char *p,int color);

int	R_draw_charactor(int xpos, int ypos, char *p,char* lpSrc,int color);

int R_SVGA_circle_push(int x,int y,int r,int mode,int pat);
int R_SVGA_circle_pop(void);

int R_SVGA_pset_in_rect( R_BOX org, R_POINT *pt, int color, int offx, int offy );				//任意点が４角形内にある場合のみ描画	by naka

#ifdef __cplusplus
}
#endif

#endif	// R_SGRAPH_H_
