/*	Ｒ＿ｆｉｌｔｅｒ.ｈ
	フィルターライブラリー

				2003/09/12	R_or_bin_area,R_and_bin_area追加
	Ver 3.47	2006/01/11	R_abs_gray_memory追加						by tanaka
				2006/01/16	R_move_gray_subpixel追加					by tanaka
	Ver 3.48	2006/04/07	R_move_gray_modify 新規追加 				by Shimizu K
	Ver 3.49	2006/09/26	R_add/sub_gray_offset()関数(MMX使用)の追加	by tanaka
	Ver 3.50	2007/12/06	R_filter_smooth_median追加					by saito
				2008/12/15	R_move_gray_modifyにMOVE_DOUBLEを追加		by tanaka
	Ver 3.50	2009/02/16	DOS & LINUX ソース統合
	Ver 3.50	2009/07/15	プロトタイプ宣言追加
	Ver 4.01	2017/08/18	R_mirror_gray_memory_horizontally/vertically関数追加
*/

#ifndef R_FILTER_H_
#define R_FILTER_H_

#include <mmintrin.h>	// _m64

#include "R_stdlib.h"
#include "R_plane.h"

#ifdef __x86_64__
#include "R_plane.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define		FILTER_12	0
#define		FILTER_13	1
#define		FILTER_15	2
#define		FILTER_21	3
#define		FILTER_31	4
#define		FILTER_51	5
#define		FILTER_33	6
#define		FILTER_55	7


#define		X_DIRECTION		0
#define		Y_DIRECTION		1
#define		XY_DIRECTION	2


// R_move_gray_modify(R_filtc4.c)
#define		MOVE_HALF			1
#define		MOVE_HALF_SAMP		2
#define		MOVE_CCW_HALF		3
#define		MOVE_CCW_TWO3RD		4
#define		MOVE_DOUBLE			5

#define		FILTER_RECTANGLE	0
#define		FILTER_CIRCLE		1
#define		FILTER_RECTANGLEXY	2



// R_contrc.c
// R_expand.c
int R_contrct(int mode, int mem_no)	;
void R_l_filter12(unsigned long int *address );		// 2009/07/15
void R_l_filter12I(unsigned long int *address );	// 2009/07/15
void R_l_filter13(unsigned long int *address );		// 2009/07/15
void R_l_filter13I(unsigned long int *address );	// 2009/07/15
void R_l_filter15(unsigned long int *address );
void R_l_filter15I(unsigned long int *address );

// r_contr1.asm
void R_c_filter12(unsigned long int *address );
void R_c_filter13(unsigned long int *address );
void R_c_filter12I(unsigned long int *address );
void R_c_filter13I(unsigned long int *address );
void R_c_filter21(unsigned long int *address );
void R_c_filter31(unsigned long int *address );
void R_c_filter51(unsigned long int *address );
void R_c_filter21I(unsigned long int *address );
void R_c_filter31I(unsigned long int *address );
void R_c_filter51I(unsigned long int *address );
void R_c_filter12_mmx(unsigned long int *address );
void R_c_filter13_mmx(unsigned long int *address );
void R_c_filter15_mmx(unsigned long int *address );
void R_c_filter21_mmx(unsigned long int *address );
void R_c_filter31_mmx(unsigned long int *address );
void R_c_filter51_mmx(unsigned long int *address );
void R_c_filter12I_mmx(unsigned long int *address );
void R_c_filter13I_mmx(unsigned long int *address );
void R_c_filter15I_mmx(unsigned long int *address );
void R_c_filter21I_mmx(unsigned long int *address );
void R_c_filter31I_mmx(unsigned long int *address );
void R_c_filter51I_mmx(unsigned long int *address );
#ifdef __GNUC__
	void	R_c_filter21_(unsigned long int* address);
	void	R_c_filter31_(unsigned long int* address);
	void	R_c_filter51_(unsigned long int* address);
	void	R_c_filter21I_(unsigned long int* address);
	void	R_c_filter31I_(unsigned long int* address);
	void	R_c_filter51I_(unsigned long int* address);
	void	R_c_filter12I_mmx_(unsigned long int* address);
	void	R_c_filter13I_mmx_(unsigned long int* address);
	void	R_c_filter15I_mmx_(unsigned long int* address);
	void	R_c_filter21I_mmx_(unsigned long int* address);
	void	R_c_filter31I_mmx_(unsigned long int* address);
	void	R_c_filter51I_mmx_(unsigned long int* address);
#endif

// R_contrw.c
int R_contrct_window(int mode, int mem_no, int xs, int ys, int xe, int ye ) ;
void R_c_filter12w( int page, int x_right, int y_top, int x_left, int y_bottom ) ;
void R_c_filter13w( int page, int x_left, int y_top, int x_right, int y_bottom ) ;
void R_c_filter15w( int page, int x_right, int y_top, int x_left, int y_bottom ) ;
void R_c_filter12w_mmx( int page, int x1, int y1, int x2, int y2 );		// 2009/07/15
void R_c_filter13w_mmx( int page, int x1, int y1, int x2, int y2 );		// 2009/07/15
void R_c_filter15w_mmx( int page, int x1, int y1, int x2, int y2 );		// 2009/07/15
void R_c_filter21w_mmx( int page, int x1, int y1, int x2, int y2 );		// 2009/07/15
void R_c_filter31w_mmx( int page, int x1, int y1, int x2, int y2 );		// 2009/07/15
void R_c_filter51w_mmx( int page, int x1, int y1, int x2, int y2 );		// 2009/07/15

// r_contr2.asm
void R_c_filter21w( int page, int x_left, int y_top, int x_right, int y_bottom ) ;
void R_c_filter31w( int page, int x_left, int y_top, int x_right, int y_bottom ) ;
void R_c_filter51w( int page, int x_left, int y_top, int x_right, int y_bottom ) ;

int R_expand(int mode, int mem_no)	;
void R_e_filter12(unsigned long int *address );
void R_e_filter13(unsigned long int *address );
void R_e_filter15(unsigned long int *address );
void R_e_filter12I(unsigned long int *address );
void R_e_filter13I(unsigned long int *address );
void R_e_filter15I(unsigned long int *address );
// R_expan1.asm
void R_e_filter21(unsigned long int *address );
void R_e_filter31(unsigned long int *address );
void R_e_filter51(unsigned long int *address );
void R_e_filter21I(unsigned long int *address );
void R_e_filter31I(unsigned long int *address );
void R_e_filter51I(unsigned long int *address );
void R_e_filter12_mmx(unsigned long int *address );
void R_e_filter13_mmx(unsigned long int *address );
void R_e_filter15_mmx(unsigned long int *address );
void R_e_filter21_mmx(unsigned long int *address );
void R_e_filter31_mmx(unsigned long int *address );
void R_e_filter51_mmx(unsigned long int *address );
void R_e_filter12I_mmx(unsigned long int *address );
void R_e_filter13I_mmx(unsigned long int *address );
void R_e_filter15I_mmx(unsigned long int *address );
void R_e_filter21I_mmx(unsigned long int *address );
void R_e_filter31I_mmx(unsigned long int *address );
void R_e_filter51I_mmx(unsigned long int *address );
#ifdef __GNUC__
	void	R_e_filter21_(unsigned long int* address);
	void	R_e_filter31_(unsigned long int* address);
	void	R_e_filter51_(unsigned long int* address);
	void	R_e_filter21I_(unsigned long int* address);
	void	R_e_filter31I_(unsigned long int* address);
	void	R_e_filter51I_(unsigned long int* address);
	void	R_e_filter12I_mmx_(unsigned long int* address);
	void	R_e_filter13I_mmx_(unsigned long int* address);
	void	R_e_filter15I_mmx_(unsigned long int* address);
	void	R_e_filter21I_mmx_(unsigned long int* address);
	void	R_e_filter31I_mmx_(unsigned long int* address);
	void	R_e_filter51I_mmx_(unsigned long int* address);
#endif

// R_expanw.c
int R_expand_window(int mode, int mem_no, int xs, int ys, int xe, int ye ) ;
void R_e_filter12w( int page, int x_right, int y_top, int x_left, int y_bottom ) ;
void R_e_filter13w( int page, int x_left, int y_top, int x_right, int y_bottom ) ;
void R_e_filter15w( int page, int x_right, int y_top, int x_left, int y_bottom ) ;
void R_e_filter12w_mmx( int page, int x1, int y1, int x2, int y2 );		// 2009/07/15
void R_e_filter13w_mmx( int page, int x1, int y1, int x2, int y2 );		// 2009/07/15
void R_e_filter15w_mmx( int page, int x1, int y1, int x2, int y2 );		// 2009/07/15
void R_e_filter21w_mmx( int page, int x1, int y1, int x2, int y2 );		// 2009/07/15
void R_e_filter31w_mmx( int page, int x1, int y1, int x2, int y2 );		// 2009/07/15
void R_e_filter51w_mmx( int page, int x1, int y1, int x2, int y2 );		// 2009/07/15
void R_e_filter51wc( int page, int x_left, int y_top, int x_right, int y_bottom );	// 2009/07/27

//	R_expan2.asm
void R_e_filter21w( int page, int x_left, int y_top, int x_right, int y_bottom ) ;
void R_e_filter31w( int page, int x_left, int y_top, int x_right, int y_bottom ) ;
void R_e_filter51w( int page, int x_left, int y_top, int x_right, int y_bottom ) ;

// R_filter.c
int R_min_filter(int src_mem_no, int dst_mem_no,int xs, int ys, int xe, int ye);
int R_max_filter(int src_mem_no, int dst_mem_no,int xs, int ys, int xe, int ye);
int R_averaging(int src_mem_no, int dst_mem_no,int xs, int ys, int xe, int ye);
int R_laplacian(int src_mem_no, int dst_mem_no,int xs, int ys, int xe, int ye) ;
int R_sobel(int src_mem_no, int dst_mem_no, int direction, int xs, int ys, int xe, int ye);
int R_median(int src_mem_no, int dst_mem_no,int xs, int ys, int xe, int ye) ;
int R_roberts(int src_mem_no, int dst_mem_no,int xs, int ys, int xe, int ye);
int R_sharp(int src_mem_no, int dst_mem_no,int xs, int ys, int xe, int ye) ;
int R_defferential(int src_mem_no, int dst_mem_no, int direction, int xs, int ys, int xe, int ye);
int R_move_gray_memory(int src_mem_no, int dst_mem_no,int xs, int ys, int xe, int ye) ;
int R_add_gray_memory(int src_mem_no1, int src_mem_no2, int dst_mem_no,int xs, int ys, int xe, int ye) ;
int R_sub_gray_memory(int src_mem_no1, int src_mem_no2, int dst_mem_no,int xs, int ys, int xe, int ye) ;
int R_abs_gray_memory(int src_mem_no1, int src_mem_no2, int dst_mem_no,int xs, int ys, int xe, int ye) ;
int R_move_gray_subpixel(int src, int dst, R_BOX region, int x, int y);
int R_move_gray_modify(int src, int dst, int mode, R_RECT area, R_POINT point );

int R_move_bin_memory(int src_mem_no, int dst_mem_no,int xs, int ys, int xe, int ye) ;
int R_not_bin_memory(int src_mem_no, int dst_mem_no,int xs, int ys, int xe, int ye) ;
int R_and_bin_memory(int src_mem_no1, int src_mem_no2, int dst_mem_no,int xs, int ys, int xe, int ye) ;
int R_or_bin_memory(int src_mem_no1, int src_mem_no2, int dst_mem_no,int xs, int ys, int xe, int ye) ;
int R_xor_bin_memory(int src_mem_no1, int src_mem_no2, int dst_mem_no,int xs, int ys, int xe, int ye) ;
int R_or_bin_area( int exe_no, int mask_no, R_POINT p, R_RECT mask );
int R_and_bin_area( int exe_no, int mask_no, R_POINT p, R_RECT mask );

int R_add_gray_offset(int offs, int src_mem_no, int dst_mem_no,int xs, int ys, int xe, int ye);
int R_sub_gray_offset(int offs, int src_mem_no, int dst_mem_no,int xs, int ys, int xe, int ye);

int R_move_bin_plane(R_PLANE* src_plane, R_PLANE* dst_plane,R_RECT rect, R_POINT p);

int R_filter_smooth_median(int src_mem_no, int dst_mem_no, R_RECT *area, int filter_size, int filter_type);
int R_add_darken_memory( int ratio, int src_mem_tg, int nTgXs, int nTgYs, int src_mem_st, int dst_mem, int nStXs, int nStYs, int nStXe, int nStYe );
int R_average_two_memory( int src_mem_tg, int nTgXs, int nTgYs, int src_mem_st, int dst_mem, int nStXs, int nStYs, int nStXe, int nStYe );




// ライブラリ内部関数・変数

// R_filtc4.c
extern __m64	gray_mask[9];

// R_filta1.asm ライブラリ内部関数
void	Sub_averaging1(unsigned char* src_p, unsigned char* dst_p, int ys,int ye,int xs,int xe);
void	Sub_averaging2(unsigned char* pad1, unsigned char* pad2,unsigned char *pad3);
void	Sub_sharp1(unsigned char* src_p, unsigned char* dst_p,int ys,int ye,int xs,int xe);
void	Sub_sharp2(unsigned char *pad1,unsigned char *pad2,unsigned char *pad3);
void	Sub_laplacian1(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int xs, int xe);
void	Sub_laplacian2(unsigned char* pad1, unsigned char* pad2, unsigned char* pad3);
#ifdef __GNUC__	// 2009/07/23 by araki
	void	Sub_averaging1_(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int xs, int xe);
	void	Sub_sharp1_(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int xs, int xe);
	void	Sub_laplacian1_(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int xs, int xe);
	void	Sub_averaging2_(unsigned char* pad1, unsigned char* pad2, unsigned char* pad3);
	void	Sub_sharp2_(unsigned char* pad1, unsigned char* pad2, unsigned char* pad3);
	void	Sub_laplacian2_(unsigned char* pad1, unsigned char* pad2, unsigned char* pad3);
#endif	// __GNUC__

// R_filta2.asm ライブラリ内部関数
void	Sub_min_filter1(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int osa, int oea);
void	Sub_min_filter2(unsigned char *pad1,unsigned char *pad2,unsigned char *pad3);
void	Sub_min_filter1_sse2(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int osa, int oea);
void	Sub_min_filter2_sse2(unsigned char *pad1,unsigned char *pad2,unsigned char *pad3);
void	Sub_max_filter1(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int osa, int oea);
void	Sub_max_filter2(unsigned char* pad1, unsigned char* pad2, unsigned char* pad3);
void	Sub_max_filter1_sse2(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int osa, int oea);
void	Sub_max_filter2_sse2(unsigned char* pad1, unsigned char* pad2, unsigned char* pad3);
void	Sub_median1(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int xs, int xe);
void	Sub_roberts1(unsigned char* src_p, unsigned char* dst_p,int ys,int ye,int xs,int xe);
#ifdef __GNUC__	// 2009/07/27 by araki
	void	Sub_min_filter1_(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int osa, int oea);
	void	Sub_min_filter2_(unsigned char* pad1, unsigned char* pad2, unsigned char* pad3);
	void	Sub_min_filter1_sse2_(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int osa, int oea);
	void	Sub_min_filter2_sse2_(unsigned char* pad1, unsigned char* pad2, unsigned char* pad3);
	void	Sub_max_filter1_(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int osa, int oea);
	void	Sub_max_filter2_(unsigned char* pad1, unsigned char* pad2, unsigned char* pad3);
	void	Sub_max_filter1_sse2_(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int osa, int oea);
	void	Sub_max_filter2_sse2_(unsigned char* pad1, unsigned char* pad2, unsigned char* pad3);
	void	Sub_median1_(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int xs, int xe);
	void	Sub_roberts1_(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int xs, int xe);
#endif	// __GNUC__

// R_filta3.asm ライブラリ内部関数
void	Sub_defferentialx1(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int xs, int xe);
void	Sub_defferentialx2(unsigned char* pad1, unsigned char* pad2, unsigned char* pad3);
void	Sub_defferentialy1(unsigned char* src_p, unsigned char*dst_p, int ys, int ye, int xs, int xe);
void	Sub_defferentialy2(unsigned char* pad1, unsigned char*pad2, unsigned char* pad3);
void	Sub_defferentialxy1(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int xs, int xe);
void	Sub_sobelx1(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int xs, int xe);
void	Sub_sobelx2(unsigned char* pad1, unsigned char* pad2, unsigned char* pad3);
void	Sub_sobely1(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int xs, int xe);
void	Sub_sobely2(unsigned char* pad1, unsigned char* pad2, unsigned char* pad3);
void	Sub_sobelxy1(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int xs, int xe);
#ifdef __GNUC__	// 2009/07/27 by araki
	void	Sub_defferentialx1_(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int xs, int xe);
	void	Sub_defferentialx2_(unsigned char* pad1, unsigned char* pad2, unsigned char* pad3);
	void	Sub_defferentialy1_(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int xs, int xe);
	void	Sub_defferentialy2_(unsigned char* pad1, unsigned char* pad2, unsigned char* pad3);
	void	Sub_defferentialxy1_(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int xs, int xe);
	void	Sub_sobelx1_(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int xs, int xe);
	void	Sub_sobelx2_(unsigned char* pad1, unsigned char* pad2, unsigned char* pad3);
	void	Sub_sobely1_(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int xs, int xe);
	void	Sub_sobely2_(unsigned char* pad1, unsigned char* pad2, unsigned char* pad3);
	void	Sub_sobelxy1_(unsigned char* src_p, unsigned char* dst_p, int ys, int ye, int xs, int xe);
#endif	// __GNUC__

// R_filta4.asm ライブラリ内部関数
void	r_move_mmx1(unsigned char* p, unsigned char* pp, int j, unsigned long *masks, unsigned long* maske, int H_step);
void	r_move_mmx(unsigned char* p, unsigned char* pp, int i, int j, unsigned long* masks,unsigned long* maske, int h_step);
void	r_move_mmx8(unsigned char* p, unsigned char* pp, int i, int j, int h_step);
void	r_move_sse1(unsigned char* p, unsigned char* pp, int j, unsigned long* masks, unsigned long* maske, int H_step);
void	r_move_sse(unsigned char* p, unsigned char* pp, int i, int j, unsigned long* masks, unsigned long* maske, int h_step);
void	r_move_sse16(unsigned char* p, unsigned char* pp, int i, int j, int h_step);
void	r_memcpy_sse2_128(unsigned char* p, unsigned char* pp, int j);
void	r_memcpy_sse2_4096(unsigned char* p, unsigned char* pp, int j);
void	r_gray_move_half_image( int src, int dst, R_RECT area, R_POINT point);
void	r_gray_move_half_image_step( int src, int dst, R_RECT area, R_POINT point);
void	r_gray_move_ccw_half_image( int src, int dst, R_RECT area, R_POINT rpoint);
void	r_rotccw(unsigned char *sp, unsigned char *dp, int x, int y);
#ifdef __GNUC__	// 2009/07/27 by araki
	void	r_move_mmx1_(unsigned char* p, unsigned char* pp, int j, unsigned long* masks, unsigned long* maske, int H_step);
	void	r_move_mmx_(unsigned char* p, unsigned char* pp, int i, int j, unsigned long* masks, unsigned long* maske, int h_step);
	void	r_move_mmx8_(unsigned char* p, unsigned char* pp, int i, int j, int h_step);
	void	r_move_sse1_(unsigned char* p, unsigned char* pp, int j, unsigned long* masks, unsigned long* maske, int H_step);
	void	r_move_sse_(unsigned char* p, unsigned char* pp, int i, int j, unsigned long* masks, unsigned long* maske, int h_step);
	void	r_move_sse16_(unsigned char* p, unsigned char* pp, int i, int j, int h_step);
	void	r_memcpy_sse2_128_(unsigned char* p, unsigned char* pp, int j);
	void	r_memcpy_sse2_4096_(unsigned char* p, unsigned char* pp, int j);
	void	r_gray_move_half_image_(int src, int dst, R_RECT area, R_POINT point);
	void	r_gray_move_half_image_step_(int src, int dst, R_RECT area, R_POINT point);
	void	r_gray_move_ccw_half_image_(int src, int dst, R_RECT area, R_POINT point);
	void	r_rotccw_(unsigned char* sp, unsigned char* dp, int x, int y);
#endif	// __GNUC__

// R_filtc6.c
/*!
 * @brief	グレー画像の左右反転
 * @param	src_mem_no	元画像のページ
 * @param	dst_mem_no	反転画像の転送先のページ
 * @param	p_rect		処理する矩形領域、NULLを指定すると画像全体を反転する。
 * @retval	OK		正常終了
 * @retval	OK以外	異常終了
 */
int	R_mirror_gray_memory_horizontally(int src_mem_no, int dst_mem_no, const R_RECT* p_rect);

/*!
 * @brief	グレー画像の上下反転
 * @param	src_mem_no	元画像のページ
 * @param	dst_mem_no	反転画像の転送先のページ
 * @param	p_rect		処理する矩形領域、NULLを指定すると画像全体を反転する。
 * @retval	OK		正常終了
 * @retval	OK以外	異常終了
 */
int	R_mirror_gray_memory_vertically(int src_mem_no, int dst_mem_no, const R_RECT* p_rect);

#ifdef __x86_64__
int	RContract(R_PLANE* plane, R_RECT* rect, int mode);
int	RExpand(R_PLANE* plane, R_RECT* rect, int mode);
int	RMinFilter(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect);
int	RMaxFilter(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect);
int	RAveraging(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect);
int	RSharp(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect);
int	RLaplacian(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect);
int	RMedian(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect);
int	RMedian9(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect);
int	RMedian15vh(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect);
int	RMedian15hv(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect);
int	RMedian15n(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect, R_PLANE*);
int	RMedian15v(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect);
int	RRoberts(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect);
int	RSobelX(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect);
int	RSobelY(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect);
int	RSobelXY(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect);
int	RDifferentialX(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect);
int	RDifferentialY(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect);
int	RDifferentialXY(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect);
int	RMemcpySSE2_4096(void* dest, const void* src, size_t count);
int	RMemcpySSE2_128(void* dest, const void* src, size_t count);
int	RConvolution5(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect, int* parameter, int div);
int	RConvolution(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect, int xsize, int ysize, int* parameter, int div);

/*
typedef struct {
	int	xsize;
	int	ysize;
	int	div;
	int	mode;
	void*	param;
} R_CONVO_PARAM;

R_CONVO_PARAM*	RConvoParaOpen(int xsize, int ysize, int* parameter, int div);
int	RConvoParaClose(R_CONVO_PARAM* convo);
int	RConvolution(R_PLANE* planeSrc, R_PLANE* planeDst, R_RECT* rect, R_CONVO_PARAM* convo);
*/
#endif

#ifdef __cplusplus
}
#endif

#endif	// R_FILTER_H_
