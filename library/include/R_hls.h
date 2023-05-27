/******************************************************************************
	R_hls.h / R_hls.c
	
	•ÏX—š—ğ
	Ver 4.01	2015.02.26	V‹K’Ç‰Á
******************************************************************************/

#ifndef	R_HLS_H_
#define	R_HLS_H_

#ifdef __cplusplus
extern "C" {
#endif

int	R_hls_open(int channel, int mode);
// mode
#define	HLS_DEFAULT_MODE	0
#define	HLS_MASTER_MODE		1
#define	HLS_SLAVE_MODE		2

int	R_hls_final_satellite(int handle);
int	R_hls_set_final_satellite(int handle, int fs);
int	R_hls_make_own_bits(int handle, int sa, int bits);
int	R_hls_inp(int handle, int sa, int offset);
int	R_hls_inpw(int handle, int sa);
int	R_hls_outp(int handle, int sa, int offset, int data);
int	R_hls_outpw(int handle, int sa, int data);
int	R_hls_on(int handle, int sa, int offset, int bit);
int	R_hls_off(int handle, int sa, int offset, int bit);

#ifdef __cplusplus
}
#endif

#endif	// R_HLS_H_
