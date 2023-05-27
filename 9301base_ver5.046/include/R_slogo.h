/******************************************************************************
	R_SLOGO.H / R_SLOGO.C
	
	変更履歴
	Ver 3.50	2009/07/15	R_SLOGO_H_定義追加
******************************************************************************/

#ifndef	R_SLOGO_H_
#define	R_SLOGO_H_

#define		LOGO_NORMAL_1	1
#define		LOGO_EDGE_1		2
#define		LOGO_NORMAL_2	3
#define		LOGO_EDGE_2		4
#define		LOGO_CHAR_1		0x10
#define		LOGO_CHAR_2		0x20
#define     LOGO_RED		0x0000ff00
#define     LOGO_GREEN		0x00ff0000
#define		LOGO_BLUE		0xff000000

//変更前の白黒ロゴ用
int R_SVGA_draw_logo(int x, int y, int mode);
//変更後のフルカラーロゴ用
//int R_SVGA_draw_logo(int x, int y, int mode,int color);

#endif	// R_SLOGO_H_
