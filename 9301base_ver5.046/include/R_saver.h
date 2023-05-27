/*
ＴＳＣ－９３０１
	　スクリーンセーバーライブラリヘッダ
	変更履歴
		Ver 3.42	2004/04/22	R_draw_scr_saver_disp対応
		Ver 3.50	2009/02/12	DOS & LINUX ソース統合
*/

#ifndef	R_SAVER_H_
#define	R_SAVER_H_

#define		SIZE256_240		1
#define		SIZE512_480		2
#define		SCR_SAVER_OFF	0
#define		SCR_SAVER_ON	1
#define		SCR_LEFT		0
#define		SCR_RIGHT		1
#define		SCR_UP			0
#define		SCR_DOWN		1

#ifdef	R_SAVER 
	int		Scr_x=100;
	int		Scr_y=100;
	char	Scr_xdir=SCR_RIGHT;
	char	Scr_ydir=SCR_DOWN;
	char	Scr_time=0;
	int		Scr_count=0;
	int		Scr_disp=SCR_SAVER_OFF;
	int		Scr_cur_x=0;
	int		Scr_cur_y=0;
	int		Scr_mode;
	char	*scrwork;
#else
extern int		Scr_x;
extern int		Scr_y;
extern char		Scr_xdir;
extern char		Scr_ydir;
extern char		Scr_time;
extern int		Scr_count;
extern int		Scr_disp;
extern int		Scr_cur_x;
extern int		Scr_cur_y;
extern int		Scr_mode;
extern char		*scrwork;
#endif


void R_disp_scr_saver( int );
void R_clear_scr_saver( void );

#endif	// R_SAVER_H_
