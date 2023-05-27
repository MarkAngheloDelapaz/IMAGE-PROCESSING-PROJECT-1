/******************************************************************************
	R_TES.H / R_TES.C
	
	変更履歴
				1994/08/27	テンプレートの最大数を変更
				1995/12/28	get_nmatch関数に範囲外の値を与えていたのを修正。
	Ver.3.40	2003/07/23	R_tes_seach のサーチページ指定を追加
	Ver.3.50	2008/08/21	R_tes_set_target_plane関数追加
	Ver.3.50	2009/03/30	DOS & LINUX ソース統合
	Ver.3.50	2009/07/15	R_tes_t2g、R_tes_compress1プロトタイプ宣言追加
	Ver.4.01	2012/09/13	R_tes_make_templateエラー値変更
******************************************************************************/

#ifndef	R_TES_H_
#define	R_TES_H_

#define	TES_VERTICAL	1
#define	TES_HORIZONTAL	2

#define	FIXED_LEVEL		0
#define	AUTO_LEVEL		1
#define	ROUGH	0 				//	サーチモード	0:rough only 1:rough to fine
#define	ROUGH2FINE	1 			//	サーチモード	0:rough only 1:rough to fine


//#ifdef TES_FILE
//	int SearchTesPage = 0;
//#else
	extern int SearchTesPage;
//#endif

typedef	struct {
	int		x,y;				//	start point
	int		xl,yl;				//	template size
	int		edge_total[ 5 ];	//	3value edge total counts
	int		level[ 5 ];			//	3value edge transration level
	int		*tpd[ 5 ];			//	pointer of 3value picture data
	int		x_offset,y_offset;	//	マークをつける左上からのオフセット座標の１０倍値
	int		search_xs;			//	サーチウィンドー　Ｘ始端位置２の倍数
	int		search_ys;			//	サーチウィンドー　Ｙ始端位置２の倍数
	int		search_xw;			//	サーチウィンドー　Ｘサイズ１６の倍数
	int		search_yw;			//	サーチウィンドー　Ｙサイズ１６の倍数
	int		rough_ly;			//	候補点サーチのレイヤー
	int		fine_ly;			//	最終点サーチのレイヤー
	int		rough_low;			//	候補点サーチの一致率下限値
	int		fine_low;			//	最終点サーチの一致率下限値
	int		search_mode; 		//	サーチモード	0:rough only 1:rough to fine
} TES_TEMPLATE;


typedef	struct {
	int		x;
	int		y;
	int		xl;
	int		yl;
	int		level[ 5 ];		// 3value level
	int		*tpd[ 5 ];		// pointer of 3value picture data
} TES_TEMPLATE_OLD;


typedef struct {
	int		x;
	int		y;
	int		val;
} TES_RESULT;

TES_TEMPLATE *R_tes_get_tempalate( int n );
int R_tes_set_target_page(int page);	// SearchTesPage
int R_tes_get_target_page(void);
int R_tes_set_work_plane( int plane );
int R_tes_make_template( int n, int x, int y, int xl, int yl, int level );
int R_tes_erase_template( int n );
int R_tes_search( int n, TES_RESULT *rs );
void R_tes_disp_result( int n, int x, int y, int mode, TES_RESULT *result );
int R_tes_disp_all_template( int n );
int R_tes_set_search_window( int n, int x, int y, int xw, int yw );
int R_tes_set_rough_layer( int n, int layer );
int R_tes_set_fine_layer( int n, int layer );
int R_tes_set_rough_lower( int n, int lower );
int R_tes_set_fine_lower( int n, int lower );
int R_tes_set_search_mode( int n, int mode );
int R_tes_disp_template_scale(int n, int layer, int x, int y ,int scale);	// 2009/07/22

TES_TEMPLATE *R_tes_get_template( int );
int R_tes_disp_template( int, int, int, int );
#ifdef __DOS__
	int	R_tes_get_nmatch(int n, char* od, int x, int y, int layer);
#elif __GNUC__
	int	R_tes_get_nmatch(int n, unsigned char* od, int x, int y, int layer);
#endif
void R_tes_initial( void );

void R_tes_compress8(int x, int y, int xw, int yw, unsigned char *dp ) ;
int R_tes_g2t( int xw, int yw, int threshold, unsigned char *sp, unsigned char *dp ) ;
int R_tes_compress(int x, int y, int xw, int yw, unsigned char *dp, int layer );

int	R_tes_write_template( int n, int handle );
int	R_tes_read_template( int n, int handle );
int	R_tes_read_template_old( int n, int handle );

int		R_tes_t2g(int xw, int yw, unsigned char *s_p, unsigned char *dp);
void	R_tes_compress1(int x, int y, int xw, int yw, unsigned char *dp);
void	R_tes_compress2(int x, int y, int xw, int yw, unsigned char *dp);
void	R_tes_compress4(int x, int y, int xw, int yw, unsigned char *dp);
void	R_tes_compress16(int x, int y, int xw, int yw, unsigned char *dp);

//	r_tesasm.asm
int get_nmatch( void *xtop, int n, int yl, int x, void *pat, int nmc );
int tes_get_edgetotal( void *template_address, int xsize, int ysize );
#ifdef __GNUC__
	int	get_nmatch_(void* xtop, int n, int yl, int x, void* pat, int nmc);
	int	tes_get_edgetotal_(void* template_address, int xsize, int ysize);
#endif

#endif	// R_TES_H_
