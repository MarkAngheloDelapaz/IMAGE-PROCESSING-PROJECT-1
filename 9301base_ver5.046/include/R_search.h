/******************************************************************************
	グレーサーチ用ヘッダファイル

	R_gray.hにもグレーサーチ関連あり

	Ver x.xx	2001.04.18	グレーサーチエリア可変対応
	Ver 3.36	2003.05.20	R_search_gray,R_search_set_page追加(ページ指定対応)
	Ver 3.40	2003/08/07	R_gs_xseach()	MMX時の位置ずれ対策
							R_gs_usepat()	MMXでテンプレートサイズが６４未満のとき従来コードで動作させる
				2003/09/12	名前でテンプレート登録時の不具合修正
	Ver 3.50	2009/02/16	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_SEARCH_H_
#define	R_SEARCH_H_

#include "R_gray.h"		// 2009/02/16 by araki

#ifdef __cplusplus
extern "C" {
#endif

#define		ON_DISPLAY		0x1				//サーチポイント表示
#define		MARK_DISPLAY	0x1				//サーチポイント表示
#define		AXIL_DISPLAY	0x2				//サーチ座標表示
#define		ALL_DISPLAY		0x3				//サーチポイント、サーチ座標表示
#define		NON_DISPLAY		0x0				//前回表示ポイント消去
#define		WAIT			0x80			//同期待ち表示


#define		GO			0
#define		NG			-1

#define		MIX_SEARCH		(unsigned int)0x01		//差分、相関ミックス
#define		RASEN			(unsigned int)0x02		//螺旋
#define		SKIP_H			(unsigned int)0x04		//ｓｋｉｐ横
#define		SKIP_V			(unsigned int)0x08		//ｓｋｉｐ縦
#define		SKIP_HV			(unsigned int)0x0c		//ｓｋｉｐ縦横

#define		NOT_VIDEO_IN		0
#define		VIDEO_IN			0x10

#define		MOUSE_CHECK			1
#define		NON_MOUSE_CHECK		0
#define		CHECK_LIKENESS		2
#define		NON_CHECK_LEVEL		4
#define		GS_HIGH_PRECISION	0x100

#define		NORMAL_RESOLUTION	1
#define		HIGH_RESOLUTION		2
#define		SUPER_RESOLUTION	3


#define		T_LIMIT			2		//	テンプレートの濃淡チェック値
#define		TEACH_LIMIT		800

/*	R_gray.hに移動
typedef struct {
	int		x;
	int		y;
} SAXIL;
*/

#ifndef __x86_64__
#ifdef	SEARCH_FILE
	int			Valsub;				// サーチ成功時の候補点差分値
	int	avg_calc_mode = 0;			//	0:nomal 1:h_mode 2:sh_mode
	SAXIL Axil[200]={0,0};			//R_searchで定義,R_sgrayで参照 1999.11.12 ohshima
	int SearchGrayPage=0;			//グレーサーチ実行ページ
#else
	extern	int	Valsub;
	extern	int	avg_calc_mode;
//	extern	SAXIL *Axil;
	extern	SAXIL Axil[];			// 2014.01.07 by araki
	extern	int SearchGrayPage;
#endif
#endif

typedef struct {
	int	x;
	int	y;
	int	val[10];
} PIXEL;

typedef struct {
	unsigned char	*sw_p;			// サーチウィンドウのポインター
	unsigned char	*sp_p;			// サーチパターンのポインター
	int			sw_xw;			// サーチウィンドウのｘサイズ
	int			sw_yw;			//				 	 ｙサイズ
	int			sp_xw;			// サーチパターンのｘサイズ
	int			sp_yw;			// 				　 ｙサイズ
	int			ly;				// レイヤーナンバー
	int			val;			// 相関係数
} SEARCH_TMP;


// 最大サーチサイズ		2001.04.18  by matsu
/*	R_gray.hに移動
typedef struct {
	int	x;
	int	y;
} SEARCH_MAX_SIZE;
*/

#if defined(SEARCH_FILE) && !defined(__x86_64__)
	SEARCH_MAX_SIZE SearchMaxSize = { 1024, 1024 };
#else
	extern SEARCH_MAX_SIZE SearchMaxSize;
#endif


#ifdef __x86_64__
int		R_search_get_limit(R_GS_OBJ*, GS_SEARCH_PATTERN*, R_PLANE*, int mode);
int		R_search_gray(R_GS_OBJ*, GS_SEARCH_PATTERN*, R_PLANE*, GS_SEARCH_RESULT* result);
int		R_gs_xsearch(R_GS_OBJ*, GS_SEARCH_PATTERN*, GS_SEARCH_RESULT* result);
int		R_get_avg(R_GS_OBJ*, int x, int y, int ly);
int		R_get_val_select(R_GS_OBJ*, GS_SEARCH_PATTERN*, SEARCH_TMP* stmp, SEARCH_TMP* stmp1, PIXEL* px, int x, int y);
int		R_get_val_select_b(R_GS_OBJ*, GS_SEARCH_PATTERN*, SEARCH_TMP* stmp, SEARCH_TMP* stmp1, PIXEL* px, int x, int y);
void	R_search_near(R_GS_OBJ*, GS_SEARCH_PATTERN*, PIXEL* px, int x, int y, SEARCH_TMP* stmp);
void	R_search_near_b(R_GS_OBJ*, GS_SEARCH_PATTERN*, PIXEL* px, int x, int y, SEARCH_TMP* stmp);
int		R_gs_set_search_mode(GS_SEARCH_PATTERN*, int mode);
int		R_gs_get_search_mode(GS_SEARCH_PATTERN*);
int		R_gs_set_resolution(GS_SEARCH_PATTERN*, int resolution);
int		R_gs_get_resolution(GS_SEARCH_PATTERN*);
int		R_gs_set_complex(GS_SEARCH_PATTERN*, int complex);
int		R_gs_get_complex(GS_SEARCH_PATTERN*);
int		R_gs_set_mid_lower(GS_SEARCH_PATTERN*, int lower);
int		R_gs_get_mid_lower(GS_SEARCH_PATTERN*);
int		R_gs_set_last_lower(GS_SEARCH_PATTERN*, int lower);
int		R_gs_get_last_lower(GS_SEARCH_PATTERN*);
int		R_gs_set_mid_percentage(GS_SEARCH_PATTERN*, int per, int ly);
int		R_gs_get_mid_percentage(GS_SEARCH_PATTERN*, int ly);
int		R_get_val_select_mlt(R_GS_OBJ*, GS_SEARCH_PATTERN*, SEARCH_TMP* stmp, SEARCH_TMP* stmp1, PIXEL* px, int x, int y);
int		R_get_val_select_mlt_b(R_GS_OBJ*, GS_SEARCH_PATTERN*, SEARCH_TMP* stmp, SEARCH_TMP* stmp1, PIXEL* px, int x, int y);
#else
int	 R_search_usepat( int page, int name, int x, int y , int x_size, int y_size , int x_offset, int y_offset, int mode);					//ver3.36
int	 R_search_setpat( int page, int n, int x, int y , int x_size, int y_size, int x_offset, int y_offset, int mode );				//ver3.36
int  R_gs_defpat( int name, int video_sw );
int	 R_gs_set_search_mode( int n, int mode );
int	 R_gs_get_search_mode( int n );
int	 R_gs_set_resolution( int n, int mode );
int	 R_gs_get_resolution( int n );
int	 R_gs_set_complex( int n, int mode );
int	 R_gs_get_complex( int n );
int	 R_gs_set_search_window( int n, int x, int y, int xw, int yw );
int	 R_gs_set_mid_lower( int n, int lower );
int	 R_gs_get_mid_lower( int n );
int	 R_gs_set_last_lower( int n, int lower );
int	 R_gs_get_last_lower( int n );
int	 R_gs_set_mid_percentage( int n, int per, int ly );
int	 R_gs_get_mid_percentage( int n, int ly );
int	 R_disp_template( int n );
int	 R_load_template( char *file_name );
int	 R_save_template( int n, char *file_name );
int	 R_write_template( int, int );
int	 R_read_template( int, int );
int	 R_search_get_limit( int page, int n, int mode );				//ver3.36
int  R_search_gray( int page, int n, GS_SEARCH_RESULT *result );	//ver3.36
int  R_gs_xsearch( int n, GS_SEARCH_RESULT *result );
int  R_get_avg( int x, int y, int ly);
int  R_get_val_select( int n, SEARCH_TMP *stmp, SEARCH_TMP *stmp1, PIXEL *px,int x, int y);
int  R_get_val_select_mmx( int n, SEARCH_TMP *stmp, SEARCH_TMP *stmp1, PIXEL *px,int x, int y);
int  R_get_val_select_b( int n, SEARCH_TMP *stmp, SEARCH_TMP *stmp1, PIXEL *px, int x, int y);
int  R_get_val_select_b_mmx(int n, SEARCH_TMP *stmp, SEARCH_TMP *stmp1, PIXEL *px, int x, int y);
int  R_get_val_select_mlt( int n, SEARCH_TMP *stmp, SEARCH_TMP *stmp1, PIXEL *px, int x, int y);
int  R_get_val_select_mlt_b( int n, SEARCH_TMP *stmp, SEARCH_TMP *stmp1, PIXEL *px, int x, int y);
void R_search_near( int pn, PIXEL *px, int x, int y, SEARCH_TMP *stmp );
void R_search_near_b( int pn, PIXEL *px, int x, int y, SEARCH_TMP *stmp );
int	 R_get_sizeof_template_data( int n );
void R_gs_disp_result( int n, int x, int y, int mode, GS_SEARCH_RESULT *result );
void R_disp_time( int x, int y, int time );
#endif
void R_get_pixel_xy( PIXEL *px,int ly );

#define R_gs_usepat(name, x, y , x_size, y_size , x_offset, y_offset, mode) R_search_usepat( SearchGrayPage, name, x, y , x_size, y_size , x_offset, y_offset, mode)		//ver3.36
#define R_gs_setpat(n, x, y , x_size, y_size, x_offset, y_offset, mode) R_search_setpat( SearchGrayPage, n, x, y , x_size, y_size, x_offset, y_offset, mode )	//ver3.36
#define R_get_limit(n, mode) R_search_get_limit( SearchGrayPage, n, mode )			//ver3.36
#define R_gs_search(n, result)	R_search_gray( SearchGrayPage, n, result )			//ver3.36
#define R_search_set_page(page) SearchGrayPage=page		//ver3.36

int  r_val_check( int n, SEARCH_TMP *stmp, SEARCH_TMP *stmp1, int i, int j, PIXEL *px );
int  r_val_check_b( int n, SEARCH_TMP *stmp, SEARCH_TMP *stmp1, int i, int j, PIXEL *px );
#ifdef __x86_64__
int  r_val_check_mmx(R_GS_OBJ*, GS_SEARCH_PATTERN*, SEARCH_TMP* stmp, SEARCH_TMP* stmp1, int i, int j, PIXEL* px);
int  r_val_check_b_mmx(R_GS_OBJ*, GS_SEARCH_PATTERN*, SEARCH_TMP* stmp, SEARCH_TMP* stmp1, int i, int j, PIXEL* px);
int  r_check_template(GS_SEARCH_PATTERN*, int ly);
void r_get_sp_pmt(R_GS_OBJ*, GS_SEARCH_PATTERN*);
void r_set_const(R_GS_OBJ*, GS_SEARCH_PATTERN*, SEARCH_TMP* stmp, int ly);
#else
int  r_val_check_mmx( int n, SEARCH_TMP *stmp, SEARCH_TMP *stmp1, int i, int j, PIXEL *px );
int  r_val_check_b_mmx( int n, SEARCH_TMP *stmp, SEARCH_TMP *stmp1, int i, int j, PIXEL *px );
int  r_check_template( int n , int ly);
void r_get_sp_pmt( int n );
void r_set_const( int n, SEARCH_TMP *stmp, int ly );
#endif
int	 r_check_axil( SAXIL *axil, int n, int i, int j);
int	 r_check_axil_area( SAXIL *axil, int n, int i, int j, int area );
int  r_save_val( int n, int mode, FILE *fp ) ;



int R_get_gs_patern_number(int name) ;

int R_gs_check_name( int name);

// 2001.04.18 サーチサイズ変更仮対応(高画素カメラ用)  by matsu
int R_gs_set_max_area(int size_x, int size_y);




//	mltshr.c
#ifdef __x86_64__
int	R_search_gray_multi(R_GS_OBJ*, GS_SEARCH_PATTERN*, R_PLANE*, GS_SEARCH_RESULT* result, int p_counts, int fine_ly);
int	R_gs_multi_xsearch(R_GS_OBJ*, GS_SEARCH_PATTERN*, GS_SEARCH_RESULT* result, int p_counts, int fine_ly);
#else
int R_search_gray_multi( int page, int p_name, GS_SEARCH_RESULT *result, int p_counts, int fine_ly);		//ver3.36
int R_gs_multi_xsearch( int p_name, GS_SEARCH_RESULT *result, int p_counts, int fine_ly);
#define R_gs_multi_search(p_name, result, p_counts, fine_ly) R_search_gray_multi( SearchGrayPage, p_name, result, p_counts, fine_ly )	//ver 3.36
#endif

//	r_gsmltd.c
void R_gs_disp_result_mlt( int n, int x, int y, int mode, GS_SEARCH_RESULT *result , int counts);

//	r_gs_bv.c
#ifdef __x86_64__
int	R_search_gray_bestval(R_GS_OBJ*, GS_SEARCH_PATTERN*, R_PLANE*, GS_SEARCH_RESULT* result, int fine_ly);
int	R_gs_bestval_xsearch(R_GS_OBJ*, GS_SEARCH_PATTERN*, GS_SEARCH_RESULT* result, int fine_ly);
#else
int R_search_gray_bestval( int page, int p_name, GS_SEARCH_RESULT *result, int fine_ly );		//ver3.36
int R_gs_bestval_xsearch( int p_name, GS_SEARCH_RESULT *result, int fine_ly );
#define R_gs_bestval_search( p_name, result, fine_ly ) R_search_gray_bestval( SearchGrayPage, p_name, result, fine_ly )	//ver3.36
#endif

#ifdef __cplusplus
}
#endif

#endif	// R_SEARCH_H_
