/************************************************************************
	‚qQ‚†‚†‚”D‚ˆ			‚e‚e‚sƒ‰ƒCƒuƒ‰ƒŠ[—pƒwƒbƒ_[ƒtƒ@ƒCƒ‹
							Ver. 1.0	Jun. 3,	1994	F.Shigeoka
							modify	Jun. 15, 1994	Makoto.Shimizu
	Ver 3.50	2009/06/08	R_fft2AR_fft1•s‹ï‡C³
************************************************************************/

#ifndef	R_FFT_H_
#define	R_FFT_H_

#ifdef __x86_64__
#include "R_plane.h"
#include "R_rect.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define FFT_NORMAL			0		/*@‡‚e‚e‚s@*/
#define FFT_INVERSE			1		/*@‹t‚e‚e‚s@*/
#define	SPECTRUM_NORMAL		0		/*@’Êí‚Ì‚e‚e‚si’¼—¬•ª‚ª—¼’[j */
#define SPECTRUM_OPTICAL	1   	/*@ŒõŠw“I‚e‚e‚si’¼—¬•ª‚ª’†‰›j */
#define	DISP_NORMAL			0		/*@’Êí‚É‰æ‘œ‰»@ */
#define	DISP_MAX			1		/*@³‹K‰»‚µ‚Ä‰æ‘œ‰»@ */

#define	_PI	3.14159265358979323846

int R_fft2(float *a_rl, float *a_im, int n_x, int n_y, int inverse, int opt);
int R_rvmtx(float *a, float *b, int n_x, int n_y);
int R_fft1(float x[], float y[], int n, int inverse, int opt);
int R_make_sintbl(int n, float sintbl[]);
int	R_make_bitrev(int n, int bitrev[]);
int R_fft_load_image(char *filename, float *a_rl, float *a_im, int n_x, int n_y);
int R_fft_save_image(float *a_rl, char *filename, int n_x, int n_y);
#ifdef __x86_64__
int R_fft_set_image(R_PLANE* plane, R_RECT* rect, float* a_rl, float* a_im);
#else
int R_fft_set_image(unsigned char *src_image, float *a_rl, float *a_im, int x_start, int y_start, int x_size, int y_size);
#endif
int R_fft_disp_image(float *a_rl, unsigned char *dst_image, int x_start, int y_start, int x_size, int y_size, int disp);
int R_make_spct(float *a_rl, float *a_im, float *spct, int n_x, int n_y);
int	R_filter_circle(float *a_rl, float *a_im, int n_x, int n_y, int r1, int r2);
int R_filter_spct(float *src_spct, float *dst_spct, int n_x, int n_y, int r1, int r2);

#ifdef __cplusplus
}
#endif

#endif	// R_FFT_H_
