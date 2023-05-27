/******************************************************************************
	R_bayer.h / R_bayer.c
	
	ïœçXóöó
	Ver.4.01	2013.08.08	êVãKçÏê¨
******************************************************************************/

#ifndef R_BAYER_H_
#define R_BAYER_H_

#include "R_stdlib.h"
#include "R_plane.h"

#ifdef __cplusplus
extern "C" {
#endif

int	R_bayer2color(int src_page, int array_type, R_RECT* src_rect, R_PLANE* dst_plane, R_POINT* dst_point, int interpolation, int white_balance);
#define	R_BAYER_ARRAY_RGGB	1
#define	R_BAYER_ARRAY_GRBG	2
#define	R_BAYER_ARRAY_GBRG	3
#define	R_BAYER_ARRAY_BGGR	4
#define	R_BAYER_INTERPOLATION_1	0

int	R_color2gray_rgb(R_PLANE* src_plane, R_RECT* rect, int dst_red_page, int dst_green_page, int dst_blue_page, R_POINT* point);
#define	R_color2gray_rb(src_plane, rect, dst_red_page, dst_blue_page, point)	R_color2gray_rgb(src_plane, rect, dst_red_page, 0, dst_blue_page, point)
int	R_calc_white_balance_parameter(R_PLANE* plane, R_RECT* rect);
int	R_video_bayer_color_interpolation(R_PLANE* plane);

#ifdef __cplusplus
}
#endif

#endif	// R_BAYER_H_
