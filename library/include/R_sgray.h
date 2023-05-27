/******************************************************************************

	R_SGRAY.H

	<< history >>
		Ver 3.40	2003/08/07	R_sgray_get_limit_svga(）のR_make_py_av_mem()をR_gray_make_py_av()に変更
					2003/09/12	名前でテンプレート登録時の不具合修正
		Ver 3.50	2009/06/09	再インクルード防止
******************************************************************************/

#ifndef	R_SGRAY_H_
#define	R_SGRAY_H_

#define AXIL_OVERLAT 0x00
#define AXIL_REWRITE 0x04


int R_gs_defpat_svga( int name, int mode );
int R_disp_template_svga( int n );
int R_dsp_tp_v_svga(unsigned char *s_po,short int xw, short int yw, short int x, short int y,int bright, int expand);
void R_gs_disp_result_svga( int n, int x, int y, int offsetX,int offsetY,int mode, GS_SEARCH_RESULT *result );
void R_disp_time_svga( int x, int y, int time,int mode );
int R_sgray_get_limit_svga( int page, int n, int mode );		//ver3.36
//int R_get_limit_svga( int n, int mode );
int R_sgray_usepat_svga( int page, int name, int x, int y , int x_size, int y_size , int x_offset, int y_offset, int mode );	//ver3.36
//int R_gs_usepat_svga( int name, int x, int y , int x_size, int y_size , int x_offset, int y_offset, int mode );
#define R_get_limit_svga( n, mode ) R_sgray_get_limit_svga( SearchGrayPage, n, mode )		//ver3.36
#define R_gs_usepat_svga( name, x, y, x_size,  y_size, x_offset, y_offset, mode ) R_sgray_usepat_svga( SearchGrayPage, name, x, y, x_size,  y_size, x_offset, y_offset, mode )		//ver3.36

extern unsigned char SVGA_GRAYSEARCH;

//change 00/8/29*****************
extern int SVGADrawUsepat;

#define NON_WRITE 0
#define WRITE 1
#define	R_GSSetDrawUsepat(mode) SVGADrawUsepat=mode
//change *************************

#endif	// R_SGRAY_H_
