/******************************************************************************
	R_INCY116.H / R_INCY116.C
	
	•ÏX—š—ğ
	Ver 4.01	2011/04/14	V‹K’Ç‰Á
******************************************************************************/

#ifndef	R_INCY116_H_
#define	R_INCY116_H_

#ifdef __cplusplus
extern "C" {
#endif

int	r_incy116_open(int id);
int	r_incy116_inp(int handle, int channel);
int	r_incy116_inpw(int handle, int channel);
int	r_incy116_outp(int handle, int channel, int data);
int	r_incy116_outpw(int handle, int channel, int data);


unsigned long	r_incy116_base_port(int id);

#ifdef __DOS__
void	r_incy116_print_info(int, int, int);
#elif __GNUC__
int	RIncy116OpenALL(int num, int multi);
#endif

#ifdef __cplusplus
}
#endif

#endif	// R_INCY116_H_
