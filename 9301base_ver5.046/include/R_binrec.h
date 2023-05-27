/*
　２値認識用ヘッダファイル
	
	Ver 3.50	2009/02/12	DOS & LINUX ソース統合
*/

#ifndef	R_BINREC_H_
#define	R_BINREC_H_

#ifdef __cplusplus
extern "C" {
#endif

//	線認識モード(LINERCG_PARA.mode)
#define	DIR_SINCOS	0
#define	DIR_X		1
#define	DIR_135		2
#define	DIR_Y		3
#define	DIR_45		4

//	線認識パラメータ構造体
typedef struct {
	int		page;
	int		x;
	int		y;
	int		level;
	int		noise;
	int		mode;
	long	sind;
	long	cosd;
} LINERCG_PARA;

//　線認識構造体
typedef struct {
	int		runstrt[ 512 ];
	int		runleng[ 512 ];
	int		runcol[ 512 ];

	int		dir;
	int 	rn;
	int		black;
	int		white;

	int		x1;
	int		y1;
	int		x2;
	int		y2;
	int		xc;
	int		yc;
	int		w;
	int		color;
} LINERCG;

#if	defined(R_BINREC)
	LINERCG_PARA	Lrcg_prm;
#else
	extern LINERCG_PARA	Lrcg_prm;
#endif

//	ページ設定
#define	R_linercg_set_page(p)		Lrcg_prm.page = p;
//	認識位置設定
#define	R_linercg_set_point(a,b)	Lrcg_prm.x = a;	Lrcg_prm.y = b;
//	２値化レベル設定
#define	R_linercg_set_level(n)		Lrcg_prm.level = n;
//	最大ノイズ幅設定
#define	R_linercg_set_noise(n)		Lrcg_prm.noise = n;

//	線認識方向設定（°単位）
void R_linercg_set_dir1( int );
//	線認識方向設定（ｓｉｎθ、ｃｏｓθ）
void R_linercg_set_dir2( double, double );
//	線認識方向設定（２点指定）
void R_linercg_set_dir3( int, int, int, int );

//	線認識
int R_linercg( LINERCG * );
//　任意角度ランテーブル作成
int R_linerun( LINERCG * );
//　固定角度ランテーブル作成
int R_linerun_sp( LINERCG * );
//　ラン情報の取得
int R_get_runinfo( LINERCG *, int );
//	円上の白／黒を数える
void R_get_color_circle( int, int, int, int, int *, int * );
//	直線上の白／黒を数える
void R_get_color_line( int, int [], int *, int * );
//	指定画素（複数）の白／黒を数える
void R_get_color_point( int, int [], int *, int *t );
//	指定エリアの白を数える
int R_count_bin_area( int page, int x1, int y1, int x2, int y2 );

#if	defined(R_BINREC)
int	pixel_table[ 256 ] = {
		0,	1,	1,	2,	1,	2,	2,	3,	1,	2,	2,	3,	2,	3,	3,	4,
		1,	2,	2,	3,	2,	3,	3,	4,	2,	3,	3,	4,	3,	4,	4,	5,
		1,	2,	2,	3,	2,	3,	3,	4,	2,	3,	3,	4,	3,	4,	4,	5,
		2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
		1,	2,	2,	3,	2,	3,	3,	4,	2,	3,	3,	4,	3,	4,	4,	5,
		2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
		2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
		3,	4,	4,	5,	4,	5,	5,	6,	4,	5,	5,	6,	5,	6,	6,	7,
		1,	2,	2,	3,	2,	3,	3,	4,	2,	3,	3,	4,	3,	4,	4,	5,
		2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
		2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
		3,	4,	4,	5,	4,	5,	5,	6,	4,	5,	5,	6,	5,	6,	6,	7,
		2,	3,	3,	4,	3,	4,	4,	5,	3,	4,	4,	5,	4,	5,	5,	6,
		3,	4,	4,	5,	4,	5,	5,	6,	4,	5,	5,	6,	5,	6,	6,	7,
		3,	4,	4,	5,	4,	5,	5,	6,	4,	5,	5,	6,	5,	6,	6,	7,
		4,	5,	5,	6,	5,	6,	6,	7,	5,	6,	6,	7,	6,	7,	7,	8,
	};
#else
	extern int pixel_table[];	// 2009/07/28
#endif

// R_SFTBIN.asm
void	R_ShiftBinMemory(void* p, int dx, int dy);
#ifdef __GNUC__
	void	R_ShiftBinMemory_(void* adr, int d_x, int d_y);
#endif

#ifdef __cplusplus
}
#endif

#endif	// R_BINREC_H_
