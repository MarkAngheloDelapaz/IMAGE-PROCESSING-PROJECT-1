/******************************************************************************
	R_geos.h / R_geos.c
	
	変更履歴
	Ver 3.28	2001/02/05	新規作成
	Ver 3.50	2009/02/10	DOS & LINUX ソース統合
******************************************************************************/

// 幾何学マッチングライブラリ

#ifndef	R_GEOS_H_
#define	R_GEOS_H_

#define GEOS_MAX_TEMPLATE	100
#define GEOS_COARSE_MODE	0
#define GEOS_DETAIL_MODE	1
#define WITH_ROTATE			0
#define WITHOUT_ROTATE		1

typedef struct {
	int				name;				//	サーチパターンの名前
	int				org_x;				//	サーチパターンの原点ｘ座標(サーチ領域中心)
	int				org_y;				//	サーチパターンの原点ｙ座標(サーチ領域中心)
	int				search_xs;			//	サーチパターン領域左上ｘ座標
	int				search_ys;			//	サーチパターン領域左上ｙ座標
	int				search_xw;			//	サーチパターン領域ｘ方向サイズ
	int				search_yw;			//	サーチパターン領域ｙ方向サイズ
	int				pat_xs;				//	パターン中心ｘ座標
	int				pat_ys;				//　パターン中心ｙ座標
	int				pat_xw;				//	パターンｘ方向サイズ
	int				pat_yw;				//	パターンｙ方向サイズ
	int				edge_th;			//	エッジ画像しきい値
	int				edge_num;			//	エッジ点個数
	int				max_r;				//	ハフ変換テーブル最大ｒ
	int				angle_ratio;		//	角度ヒストグラム抽出敷居値率
	unsigned short* hough_tbl;			//	ハフ変換テーブル
	unsigned long*	hough_angle_hist;	//	ハフ変換テーブルの角度ヒストグラム
	unsigned char*	image;				//	登録画像
	signed short*	heisin_data[5];		//	並進一般化ハフ登録データ
	int				heisin_data_num[5];	//	並進一般化ハフ登録データ個数
	int				saisyu_ly;
	int				kouho_ly;
	int				mode;				//検査モード（０：回転付き１：回転なし）
}GEOS_SEARCH_PATTERN;

typedef struct {
	int		x;
	int		y;
	int		angle;
	int		val;
}GEOS_SEARCH_RESULT;

#ifdef GEOS_FILE
	unsigned char 		*pGeosBufferGray[3];
	unsigned short 		*pGeosHoughTbl;
	unsigned long 		*pGeosHoughAngleHist;
	unsigned short 		*pGeosHeisinTbl;
	
	int					geos_window_xw;
	int					geos_window_yw;
	int					geos_window_xs;
	int					geos_window_ys;
	int					geos_search_pat_count;
	int					geos_pat_size[ GEOS_MAX_TEMPLATE ];
	GEOS_SEARCH_PATTERN *geos_pat[ GEOS_MAX_TEMPLATE ];
#else
	extern unsigned char 		*pGeosBufferGray[3];
	extern unsigned short 		*pGeosHoughTbl;
	extern unsigned short 		*pGeosHeisinTbl;
	extern unsigned long 		*pGeosHoughAngleHist;
	extern int					geos_window_xw;
	extern int					geos_window_yw;
	extern int					geos_window_xs;
	extern int					geos_window_ys;
	extern int					geos_search_pat_count;
	extern int					geos_pat_size[ GEOS_MAX_TEMPLATE ];
	extern GEOS_SEARCH_PATTERN *geos_pat[ GEOS_MAX_TEMPLATE ];
#endif

int R_geos_open( void );
int R_geos_usepat( int name, int xs, int ys, int xsize, int ysize ,int th,unsigned char *mask);
int R_geos_search( int pn , GEOS_SEARCH_RESULT *result,unsigned char *mask);
void R_geos_draw_box(int org_x,int org_y,int xsize,int ysize,int thita);
int R_geos_change_search_mode( int pn, int mode );
int R_geos_save_template( int pn, char *file_name );
int R_geos_load_template(char* file_name);
int R_geos_change_kouho_ly( int pn,int ly );
int R_geos_change_saisyu_ly( int pn,int ly );

// プロトタイプ宣言追加 2009/07/21
int		R_geos_draw_pattern(int pn);
int		R_geos_draw_houghtbl(int pn);
int		R_geos_draw_anglehist(int pn);
int		R_geos_get_table_data(char* file_name);

// 内部関数プロトタイプ宣言追加 2009/07/15
int		r_geos_hough_tbl_clear(unsigned short* tbl, int xsize, int ysize);
int		r_geos_hough_hist_clear(unsigned long* hist, int size);
int		r_geos_hough_voting(int xs, int ys, int xw, int yw, int org_x, int org_y, int th, int max_r, unsigned char* sp,unsigned short* tbl);
int		r_geos_get_hough_angle_hist(unsigned short* tbl, unsigned long* hist, int rsize, int p_th);
int		r_geos_get_heisin_data(int xs, int ys, int xw, int yw, unsigned char* sp, int pn, int layer);
int		r_geos_resize_image(unsigned char* sp, unsigned char* dp, int bai);
int		r_geos_get_angle(unsigned long* tmp, unsigned long* trg, int* angle, int* val, int mode);
int		r_geos_generalized_voting(int xs, int ys, int xw, int yw, signed short* data, int num, unsigned char* sp, int thita, unsigned short* tbl, int ly);
int		r_geos_get_max_position(unsigned short* tbl, int* ox, int* oy,int xsize, int ysize);
// 内部関数プロトタイプ宣言追加 2009/07/21
int		r_geos_cut_image( unsigned char *sp, unsigned char *dp, int xs, int ys, int xw, int yw);
int		r_geos_draw_any_houghtbl( unsigned short *houghtbl ,int rsize );
int		r_geos_draw_any_anglehist( unsigned long *hist_org ,int angle);
void	r_geos_draw_heisin_data( signed short *data, int num ,int org_x,int org_y);
int		r_geos_draw_any_heisintbl( unsigned short *houghtbl,int xsize,int ysize);

#endif	// R_GEOS_H_
