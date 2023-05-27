/******************************************************************************
	R_gray.c / R_projec.c / R_gray.h

	更新履歴
	Ver.3.48	2006.05.08	R_projection_x/y()のプロトタイプ宣言で戻り値の指定がなかったので追加  by matsu
	Ver.3.49	2006.12.20	hav4_mmx関数追加 by tanaka
	Ver.3.50	2009.02.12	DOS & LINUX ソース統合
	Ver.4.01	2017.04.24	MAX_TEMPLATE_TIMESを200から999に変更
******************************************************************************/

#ifndef	R_GRAY_H_
#define	R_GRAY_H_

#include <stdio.h>	// 2009/07/21追加

#include "R_rotate.h"

#ifdef __x86_64__
#include <stdint.h>
#include "R_plane.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

//#define		NULL	2009/07/21 コメントアウト

#define		OK		0
#define		ERROR	(-1)
#define		YES		1
#define		NO		0


#define		INITIAL_PTN_AREA	0
#define		CONTINUE_PTN_AREA	1


#define		LAYER1		0x01
#define		OUT_OF_FOCUS 0x80
#define		NON_LAYER4	0x10
#define		NON_LAYER8	0x08
#define		NON_LAYER16	0x00

// #define		LAYER1		1
#define		LAYER2		2
#define		LAYER4		4
#define		LAYER8		8
#define		LAYER16		16

//#define		MAX_TEMPLATE_TIMES	200			// 
#define		MAX_TEMPLATE_TIMES	999			// 2017.04.24

#define		IS_8BIT_PROCESS	0x48

typedef struct {
	int			name;		/*	サーチパターンの名前	*/
	int			org_x;		/*	切り出した時の左上のＸ座標	*/
	int			org_y;		/*	切り出した時の左上のＹ座標	*/
	int			x_size;		/*	サーチパターンのＸ方向の幅	*/
	int			y_size;		/*	サーチパターンのＹ方向の幅	*/
	int			x_offset;	/*	マークをつける左上からのオフセットＸ座標の１０倍値	*/
	int			y_offset;	/*	マークをつける左上からのオフセットＹ座標の１０倍値	*/
	int			area_size;	/*	エリアサイズこの構造体＋画像メモリーサイズ	*/
	int			search_mode; /*	サーチモード	*/
	unsigned char	*a1;	/*	生画像の先頭アドレス　画像データーは、６BITに圧縮されて格納されます。	*/
	unsigned char	*a2;	/*	１／２に圧縮した画像の先頭アドレス	*/
	unsigned char	*a4;	/*	１／４に圧縮した画像の先頭アドレス	*/
	unsigned char	*a8;	/*	１／８に圧縮した画像の先頭アドレス	*/
	unsigned char	*a16;	/*	１／１６に圧縮した画像の先頭アドレス	*/
	int			search_xs;	/*	サーチウィンドー　Ｘ始端位置２の倍数	*/
	int			search_ys;	/*	サーチウィンドー　Ｙ始端位置２の倍数	*/
	int			search_xw;	/*	サーチウィンドー　Ｘサイズ１６の倍数	*/
	int			search_yw;	/*	サーチウィンドー　Ｙサイズ１６の倍数	*/
	int			kouho_ly;	/*	候補点サーチのレイヤー	*/
	int			saisyu_ly;	/*	最終点サーチのレイヤー	*/
	int			kouho_low;	/*	候補点サーチの相関係数下限値	*/
	int			saisyu_low;	/*	最終点サーチの相関係数下限値	*/
	int			kouho_limit;/*	候補点サーチの差分絶対値	*/
	union {
		int			reserve;	
		unsigned char	mode[4];	// mode[0] データ処理ビット数
	};
	int			org_sabun[5];// 各レイヤーの差分絶対値のリミットの割合
	int			org_soukan[5];// 各レイヤーの相関値の最悪値
	int			avg;		/*	明るさの平均値           ＿ 2	*/
	unsigned int	pmt[5];		/*	パラメーター１　Σ（Ｉi－Ｉ）	*/
	} GS_SEARCH_PATTERN	;
#define	FILE_SIZE_OF_GS_SEARCH_PATTERN	160

typedef struct {
	int			x;		/*	見つけた時のＸ座標の１０倍値	*/
	int			y;		/*	見つけた時のＹ座標の１０倍値	*/
	int			val;	/*	相関係数値(～10000)	*/
	} GS_SEARCH_RESULT	;


#ifdef	GRAY_FILE
	int			gs_search_pat_count=0;		/*	登録済みサーチパターン個数	*/
	int			gs_search_pat_enable=MAX_TEMPLATE_TIMES;	/*	登録可能数	*/
	GS_SEARCH_PATTERN	*gs_pat[MAX_TEMPLATE_TIMES];		/*	サーチパターンの構造体のポインター	*/
	int			gs_pat_size[MAX_TEMPLATE_TIMES];			/*	サーチパターンの構造体のサイズ	*/
#else
	extern int			gs_search_pat_count;	/*	登録済みサーチパターン個数	*/
	extern int			gs_search_pat_enable;	/*	登録可能数	*/
	extern GS_SEARCH_PATTERN	*gs_pat[MAX_TEMPLATE_TIMES];	/*	サーチパターンの構造体のポインター	*/
	extern int			gs_pat_size[MAX_TEMPLATE_TIMES];		/*	サーチパターンの構造体のサイズ	*/
#endif

typedef struct {
	int		x;
	int		y;
} SAXIL;

// 最大サーチサイズ		2001.04.18  by matsu
typedef struct {
	int	x;
	int	y;
} SEARCH_MAX_SIZE;

#ifdef __x86_64__
	typedef struct {
		int		py_x;				// データーのＸ
		int		py_y;				// データーのＹ
		int		py_xw;				// データーのＸ幅
		int		py_yw;				// データーのＹ幅
		int		py_axw;				// 平均データーのＸ幅
		int		py_ayw;				// 平均データーのＹ幅
		unsigned char*	py_avg;		// ２データーでの平均値の先頭番地
		union {
			unsigned char*	py_a[5];
			struct {
				unsigned char*	py_a1;		// １層目の生データーの先頭番地
				unsigned char*	py_a2;		// １／　２データーの先頭番地
				unsigned char*	py_a4;		// １／　４データーの先頭番地
				unsigned char*	py_a8;		// １／　８データーの先頭番地
				unsigned char*	py_a16;		// １／１６データーの先頭番地
			};
		};
		int		Valsub;				// サーチ成功時の候補点差分値
		int		avg_calc_mode;		//	0:nomal 1:h_mode 2:sh_mode
		SAXIL*	Axil;
		void*	h_avg_work;
		SEARCH_MAX_SIZE	SearchMaxSize;
		int		bit_depth;			// 6(6bit演算) or 8(8bit演算)
	} R_GS_OBJ;
#else

#ifdef __DOS__
	extern short int	py_x	;	/*	データーのＸ	*/
	extern short int	py_y	;	/*	データーのＹ	*/
	extern short int	py_xw	;	/*	データーのＸ幅	*/
	extern short int	py_yw	;	/*	データーのＹ幅	*/
	extern short int	py_axw	;	/*	平均データーのＸ幅	*/
	extern short int	py_ayy	;	/*	平均データーのＹ幅	*/
#elif __GNUC__
	extern int			py_x;		/*	データーのＸ	*/
	extern int			py_y;		/*	データーのＹ	*/
	extern int			py_xw;		/*	データーのＸ幅	*/
	extern int			py_yw;		/*	データーのＹ幅	*/
	extern int			py_axw;		/*	平均データーのＸ幅	*/
	extern int			py_ayy;		/*	平均データーのＹ幅	*/
#endif

extern unsigned char	*py_avg	;	/*	１／　２データーでの平均値の先頭番地	*/
extern unsigned char	*py_a1	;	/*	１層目の生データーの先頭番地	*/
extern unsigned char	*py_a2	;	/*	１／　２データーの先頭番地	*/
extern unsigned char	*py_a4	;	/*	１／　４データーの先頭番地	*/
extern unsigned char	*py_a8	;	/*	１／　８データーの先頭番地	*/
extern unsigned char	*py_a16	;	/*	１／１６データーの先頭番地	*/

extern unsigned char	*GS_SEARCH_ADDRESS;			// グレーサーチ処理用アドレス
#endif



int R_make_py_av_mem(short x, short y,short xw, short yw, short bx, short by, int mode) ;

#ifdef __x86_64__
	int	R_gray_make_py_av(R_GS_OBJ*, R_PLANE*, int x, int y, int xw, int yw, int bx, int by, int mode);
	int	R_gray_make_py_av_h(R_GS_OBJ*, R_PLANE*, int x, int y, int xw, int yw, int bx, int by, int mode);
#elif defined(__DOS__)
	int	R_gray_make_py_av(int page, short x, short y,short xw, short yw, short bx, short by, int mode) ;		//ver3.36
	int	R_gray_make_py_av_h(int page, short x, short y,short xw, short yw, short bx, short by, int mode) ;		//ver3.36
	int	R_gray_make_py_av_mmx(int page, short x, short y,short xw, short yw, short bx, short by, int mode) ;	//ver3.36
#elif __GNUC__
	int	R_gray_make_py_av(int page, int x, int y, int xw, int yw, int bx, int by, int mode);
	int	R_gray_make_py_av_h(int page, int x, int y, int xw, int yw, int bx, int by, int mode);
	int	R_gray_make_py_av_mmx(int page, int x, int y, int xw, int yw, int bx, int by, int mode);
#endif

int R_gray_make_py_av_h_mmx(int page, short x, short y,short xw, short yw, short bx, short by, int mode) ;	//ver3.36
int R_gray_make_py_av_sh_mmx(int page, short x, short y,short xw, short yw, short bx, short by, int mode);	//ver3.36
int R_gray_make_py_av_sh(int page, short x, short y,short xw, short yw, short bx, short by, int mode);		//ver3.36
int R_gray_make_py_only(int page, short x, short y,short xw, short yw, short bx, short by, int mode);		//ver3.36

int R_dsp_tp(unsigned char *s_po, short int xw, short int yw, short int x, short int y);
#ifdef __DOS__
	int R_dsp_tp_v(unsigned char *s_po, short int xw, short int yw, short int x, short int y,int bright, int expand);
#elif __GNUC__
	int R_dsp_tp_v(unsigned char*s_po, int xw, int yw, int x, int y, int bright, int expand);
#endif
int R_dspsvga_tp_v(unsigned char *s_po, int xw, int yw, int x, int y,int bright, int expand);
int R_gs_defadrs( int mode );
int R_gray_make_template( int page, short x, short y,short xw, short yw ) ;									//ver3.36

#define R_make_py_av( x, y, xw, yw, bx, by, mode) R_gray_make_py_av(SearchGrayPage, x, y, xw, yw, bx, by, mode)				//ver3.36
#define R_make_py_av_mmx( x, y, xw, yw, bx, by, mode) R_gray_make_py_av_mmx(SearchGrayPage, x, y, xw, yw, bx, by, mode)		//ver3.36
#define R_make_py_av_h(x, y, xw, yw, bx, by, mode) R_gray_make_py_av_h(SearchGrayPage, x, y, xw, yw, bx, by, mode)			//ver3.36
#define R_make_py_av_h_mmx(x, y, xw, yw, bx, by, mode) R_gray_make_py_av_h_mmx(SearchGrayPage, x, y, xw, yw, bx, by, mode)	//ver3.36
#define R_make_py_av_sh_mmx(x, y, xw, yw, bx, by, mode) R_gray_make_py_av_sh_mmx(SearchGrayPage, x, y, xw, yw, bx, by, mode)	//ver3.36
#define R_make_py_av_sh(x, y, xw, yw, bx, by, mode) R_gray_make_py_av_sh(SearchGrayPage, x, y, xw, yw, bx, by, mode)			//ver3.36
#define R_make_py_only(x, y, xw, yw, bx, by, mode) R_gray_make_py_only(SearchGrayPage, x, y, xw, yw, bx, by, mode)			//ver3.36
#define R_make_template( x, y, xw, yw ) R_gray_make_template( SearchGrayPage, x, y, xw, yw )									//ver3.36

//	以下内部使用専用（ユーザーからは使用しません！！）

/*	プロトタイプ宣言　ソースファイル：Ｒ＿ｇｒａｙ.Ｃ	*/


char *g2p4(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k);
void r_gs_close(void);


/*	プロトタイプ宣言　ソースファイル：ｒ＿ｇｓ＿ｐｙ.ａｓｍ	*/


// R_gs_py.asm
//char *R_g2p4_filter(char *sp1, char *d_p, int y_k, int k);
char*	R_g2p4_filter(unsigned char* sp1, unsigned char* d_p, int y_k, int k);	// 2009/07/27
//char *p12p2av(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_x);
char*	p12p2av(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev_x);	// 2009/07/27
//char *R_avh2av(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_y);
char*	R_avh2av(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev_y);	// 2009/07/27
//char *p12p2av_div(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev);
char*	p12p2av_div(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);	// 2009/07/27
//char *R_avh2av_div(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev);
char*	R_avh2av_div(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);	// 2009/07/27
//char *p12p2av_96(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev);
char*	p12p2av_96(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);	// 2009/07/27
//char *g2p2av(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_x);
char*	g2p2av(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev_x);		// 2009/07/27
//char *g2p2av_96(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev);
char*	g2p2av_96(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);		// 2009/07/27
//char *g2p2av_div(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev);
char*	g2p2av_div(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);	// 2009/07/27
//char *R_avh2av_96(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev);
char*	R_avh2av_96(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);	// 2009/07/27
//char *R_g2p4(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k);
char*	R_g2p2(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k);	// 2009/07/27
char*	R_g2p4(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k);	// 2009/07/27
#ifdef __GNUC__
	char*	R_g2p4_filter_(unsigned char* sp1, unsigned char* d_p, int y_k, int k);
	char*	R_avh2av_div_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);
	char*	g2p2av_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev_x);
	char*	p12p2av_96_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);
	char*	g2p2av_96_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);
	char*	R_avh2av_96_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);
	char*	R_g2p2_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k);
	char*	R_g2p4_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k);
#endif

char *p12p2av_mmx(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_x);

//R_gspymx.asm
//char *R_g2p4_mmx(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k);
char*	R_g2p4_mmx(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k);	// 2009/07/27
//char *hav4_mmx(         char *sp2,char *d_p,int x_k, int y_k,        int m, int dev_x,short *w);
char*	hav4_mmx(unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int m, int dev_x, short* w);		// 2009/07/27
//char *hav4_div_mmx(         char *sp2,char *d_p,int x_k, int y_k,        int m, int dev,short *w);
char*	hav4_div_mmx(unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int m, int dev, short* w);	// 2009/07/27
//char *R_g2p2_mmx(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k);
char*	R_g2p2_mmx(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k);	// 2009/07/27
//char *R_avh2av_mmx(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_y);
char*	R_avh2av_mmx(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev_y);	// 2009/07/27
//char *R_avh2av_div_mmx(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev);
char*	R_avh2av_div_mmx(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);	// 2009/07/27
//char *hav_mmx(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_x);
char*	hav_mmx(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev_x);	// 2009/07/27
//char *hav_div_mmx(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev);
char*	hav_div_mmx(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);	// 2009/07/27
//void g2p1_c8_mmx(char *sp1,char *sp2,int xw, int yw);
void	g2p1_c8_mmx(unsigned char* sp1, unsigned char* sp2, int xw, int yw);
//void g2p1_c16_mmx(char *sp1,char *sp2,int xw, int yw);
void	g2p1_c16_mmx(unsigned char* sp1, unsigned char* sp2, int xw, int yw);
#ifdef __GNUC__
	char*	hav4_mmx_(unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int m, int dev, short* w);
	char*	hav_mmx_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev_x);
	char*	hav4_div_mmx_(unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int m, int dev, short* w);
	char*	R_avh2av_mmx_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev_y);
	char*	R_avh2av_div_mmx_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);
	char*	R_g2p4_mmx_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k);
	char*	R_g2p2_mmx_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k);
	char*	hav_div_mmx_(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int x_k, int y_k, int k, int m, int dev);
	void	g2p1_c8_mmx_(unsigned char* sp1, unsigned char* sp2, int xw, int yw);
	void	g2p1_c16_mmx_(unsigned char* sp1, unsigned char* sp2, int xw, int yw);
#endif


char *g2p4_filter(char *sp1,char *d_p, int y_k, int k);
char *R_g2p22_mmx(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k);

// R_gray.c
#ifndef __x86_64__
//void g2p1_c(char *sp1,char *sp2,int xw, int yw);
void	g2p1_c(unsigned char* sp1, unsigned char* sp2, int xw, int yw);	// 2009/07/27
//char *g2pav_filter(char *sp1, char *sp2, char *d_p, int y_k, int k, int m, int dev);
unsigned char*	g2pav_filter(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int y_k, int k, int m, int dev);	// 2009/07/27
//char *g2pav_div_filter(char *sp1, char *sp2, char *d_p, int y_k, int k, int m, int dev);
unsigned char*	g2pav_div_filter(unsigned char* sp1, unsigned char* sp2, unsigned char* d_p, int y_k, int k, int m, int dev);	// 2009/07/27
#endif




//	R_make_pa_,R_make_pa_new_		つかっていない

/*	プロトタイプ宣言　ソースファイル：ｒ＿ｇｓ＿ｖａｌ.ａｓｍ	*/
int gs_get_val_k(  unsigned char *img_p, long img_x, unsigned char img_avg,
			      unsigned char *tmp_p, long tmp_x, long tmp_y, unsigned char tmp_avg, long pmt);
#ifdef __DOS__
	int	gs_get_val(unsigned char* img_p, long img_x, unsigned char img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, unsigned char tmp_avg, long pmt);
	int	gs_get_val_mmx(unsigned char* img_p, long img_x, unsigned char img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, unsigned char tmp_avg, long pmt);
	int	gs_get_valsub(unsigned char* img_p, long img_x, unsigned char img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, unsigned char tmp_avg, long limmit);
	int	gs_get_valsub_b(unsigned char* img_p, long img_x, unsigned char img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, unsigned char tmp_avg, long limmit);
	int	gs_get_val_b(unsigned char* img_p, long img_x, unsigned char img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, unsigned char tmp_avg, long pmt);
	int	gs_get_val_b_mmx(unsigned char* img_p, long img_x, unsigned char img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, unsigned char tmp_avg, long pmt);
#elif defined(__GNUC__) && defined(__i386__)
	int	gs_get_val(unsigned char* img_p, long img_x, int img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, int tmp_avg, long pmt);
	int	gs_get_val_mmx(unsigned char* img_p, long img_x, int img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, int tmp_avg, long pmt);
	int	gs_get_valsub(unsigned char* img_p, long img_x, int img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, int tmp_avg, long limmit);
	int	gs_get_valsub_b(unsigned char* img_p, long img_x, int img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, int tmp_avg, long limmit);
	int	gs_get_val_b(unsigned char* img_p, long img_x, int img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, int tmp_avg, long pmt);
	int	gs_get_val_b_mmx(unsigned char* img_p, long img_x, int img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, int tmp_avg, long pmt);
#endif

#if defined(__GNUC__) && defined(__i386__)
	int gs_get_val_mmx_(unsigned char* img_p, long img_x, int img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, int tmp_avg, long pmt);
	int gs_get_val_b_mmx_(unsigned char* img_p, long img_x, int img_avg, unsigned char* tmp_p, long tmp_x, long tmp_y, int tmp_avg, long pmt);
#endif




char *g2p2av_c(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_x);
char *g2p2av_c8(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_x);
char *g2p1p2av(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_x,char *dp1);
void R_avh2av_c(char *sp1,char *sp2,char *d_p,int x_k, int y_k, int k, int m, int dev_y);

// r_gsb.asm
void R_gray2bin(unsigned char sho, int x, int y, int xs, int ys, unsigned char *g_adr, unsigned char * b_adr);
void R_gray2bin16(unsigned char sho, int x, int y, int xs, int ys, unsigned char *g_adr, unsigned char * b_adr);
void R_gray2bin_r(unsigned char sho, int x, int y, int xs, int ys, unsigned char *g_adr, unsigned char * b_adr);
void R_gray2bin16_r(unsigned char sho, int x, int y, int xs, int ys, unsigned char *g_adr, unsigned char * b_adr);


// R_projec.c
int R_projection_x( int page, int x1, int y1, int x2, int y2, int *x_project);		// 戻り値int追加 2006.05.08  by matsu
int R_projection_y( int page, int x1, int y1, int x2, int y2, int *y_project);		// "

// R_projea.asm
int	r_projc_x_mmx1(unsigned long *pntr,int *x_project,int H_step,int y2,unsigned long *masks);
int	r_projc_x_mmx( unsigned long *pntr,int *x_project,int H_step,int y2,unsigned long *masks,unsigned long *maske,int xe);
int	r_projc_x_sse1(unsigned long *pntr,int *x_project,int H_step,int y2,unsigned long *masks);
int	r_projc_x_sse( unsigned long *pntr,int *x_project,int H_step,int y2,unsigned long *masks,unsigned long *maske,int xe);
int r_projc_ysum_sse2(unsigned long *p,int y1,int H_step,int *sum);
int r_projc_ysum_mmx(unsigned long *p,int y1,int H_step,int *sum);
#ifdef __GNUC__
	int	r_projc_x_mmx1_(unsigned long* pntr, int* x_project, int H_step, int y2, unsigned long* masks);
	int	r_projc_x_mmx_(unsigned long* pntr, int* x_project, int H_step, int y2, unsigned long* masks, unsigned long* maske, int xe);
	int	r_projc_x_sse1_(unsigned long* pntr, int* x_project, int H_step, int y2, unsigned long* masks);
	int	r_projc_x_sse_(unsigned long* pntr, int* x_project, int H_step, int y2, unsigned long* masks, unsigned long* maske, int xe);
	int	r_projc_ysum_sse2_(unsigned long* p, int y1, int H_step, int* sum);
	int	r_projc_ysum_mmx_(unsigned long* p, int y1, int H_step, int* sum);
#endif

// r_rotate.c
// void R_RotateGrayMemory(int sp, int dp, int xc, int yc, int bs, int bb);

//R_projec.c
int R_projection_bin_x( int page, int x1, int y1, int x2, int y2, int *x_project);
int R_projection_bin_y( int page, int x1, int y1, int x2, int y2, int *y_project);

#ifdef __x86_64__
uint64_t	RProjectionX(R_PLANE* plane, R_RECT* rect, int* data);
uint64_t	RProjectionY(R_PLANE* plane, R_RECT* rect, int* data);
#endif

#ifdef __cplusplus
}
#endif

#endif	// R_GRAY_H_
