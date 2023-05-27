/******************************************************************************
	R_gsmask.h / R_gsmask.c
	
	変更履歴
	Ver 3.50	2009/02/16	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_GSMASK_H_
#define	R_GSMASK_H_

#include "R_gray.h"

#define GSMASK_MAX_MASK			50
#define GSMASK_MAX_MASK_NUMBER	32
#define GSMASK_MASK_MINUS		0
#define GSMASK_MASK_PLUS		1

typedef struct{
	int			mask_number;				// サーチパターンの個数
	int			mask_xs[GSMASK_MAX_MASK_NUMBER];	// サーチパターンのｘ開始座標
	int			mask_ys[GSMASK_MAX_MASK_NUMBER];	// 				　 ｙ開始座標
	int			mask_xw[GSMASK_MAX_MASK_NUMBER];	// サーチパターンのｘサイズ
	int			mask_yw[GSMASK_MAX_MASK_NUMBER];	// 				　 ｙサイズ
	int			usepat_flag;						//
	int 		tmp_pn;
} GSMASK_TEMPINFO;
typedef struct {
	unsigned char  	*msk;							//
	int				pixel;
	int				average;
	int				coeff;
	int				max_xs;
	int				max_ys;
	int				max_xsize;
	int				max_ysize;
	int				mask_number;
	int				mask_xs[GSMASK_MAX_MASK_NUMBER];
	int				mask_ys[GSMASK_MAX_MASK_NUMBER];
	int				mask_xw[GSMASK_MAX_MASK_NUMBER];
	int				mask_yw[GSMASK_MAX_MASK_NUMBER];
} GSMASK_PATTERN;

#ifdef	R_GSMASK
	int gsmask_normal_tmp_count;
	int gsmask_mask_tmp_count;
	int gsmask_mask_tmpnum[GSMASK_MAX_MASK];
	int gsmask_target_tmpnum[GSMASK_MAX_MASK];
	GSMASK_TEMPINFO mtemp;
	GSMASK_PATTERN *gs_mask[GSMASK_MAX_MASK];
#else
	extern int gsmask_normal_tmp_count;
	extern int gsmask_mask_tmp_count;
	extern int gsmask_mask_number[GSMASK_MAX_MASK];
	extern int gsmask_target_tmp[GSMASK_MAX_MASK];
	extern GSMASK_TEMPINFO mtemp;
	extern GSMASK_PATTERN *gs_mask[GSMASK_MAX_MASK];
#endif



int R_gsmsk_defadrs(void);
int R_gsmsk_set_nosense(int name);
void R_gsmsk_clear_mtemp(void);
int R_gsmsk_usepat(int name);
int R_gsmsk_set_maskinfo(int xs,int ys,int xsize,int ysize,int tpn,int get_mode);
int R_gsmsk_disp_template_and_mask(int pn,int mag);
int R_gsmsk_search(int name,GS_SEARCH_RESULT *result);
void R_gsmsk_disp_allinfo(void);
int R_gsmsk_get_pn_from_name(int name);
#ifdef __DOS__
	int	R_gsmsk_save_template(int name,unsigned char *file_name);
	int	R_gsmsk_load_template(unsigned char *file_name);
#elif __GNUC__
	int	R_gsmsk_save_template(int name, char* file_name);
	int	R_gsmsk_load_template(char* file_name);
#endif
int R_gsmsk_make_name(unsigned char buff[]);

int gsmsk_check_best_size(int xsize,int ysize);
int gsmsk_disp_tmp_expand( int pn, int mag ,int xs,int ys);
void gsmsk_disp_axis( int xx, int yy, int xo, int yo );
int gsmsk_get_box_area_n( int step, int *x, int *y, int *xs, int *ys, int xmax, int ymax, int xmin, int ymin ,int mag,int pn,int get_mode);
int gsmsk_check_maskinfo(int xs,int ys,int xsize,int ysize);
void gsmsk_disp_mtemp(int mag);
void gsmsk_disp_maskinfo(int num,int mag,int xsize,int ysize,int name);
void gsmsk_get_max_void_area(int pn,int *mxs,int *mys,int *mxw,int *myw);
void gsmsk_change_image_offset(unsigned char *sp,unsigned char *dp,int soff,int doff,int xw,int yw,int bright);
int gsmsk_get_average(unsigned char *mk,unsigned char *tp,int xsize,int ysize,int pix);
int gsmsk_get_coefficient(unsigned char *mk,unsigned char *tp,int xsize,int ysize,int avg);
void gsmsk_disp_VGA_maskinfo(int pn);
int gsmsk_check_only_gstemp(int pn);
int gsmsk_get_mask_number(int pn);
void gsmsk_subpixel_cut_image(unsigned char *sp,unsigned char *dp,int xs,int ys,int xsize,int ysize);
int gsmsk_calculate_corelation(unsigned char *sp,unsigned char *tp,unsigned char *mp,int pix,int xsize,int ysize,int tpave,int coeff);

#endif	// R_GSMASK_H_
