/******************************************************************************
	R_mmio.h / R_mmio.c
	
	変更履歴
	Ver 3.49	2006/11/30	正式リリース
	Ver 3.50	2008/07/09	マクロから関数へ変更
******************************************************************************/

#ifndef	R_MMIO_H_
#define	R_MMIO_H_

#ifdef __cplusplus
extern "C" {
#endif

unsigned int	R_mmio_outpd(unsigned long port, unsigned int value);
unsigned int	R_mmio_inpd(unsigned long port);
unsigned int	R_mmio_outpw(unsigned long port, unsigned int value);
unsigned int	R_mmio_inpw(unsigned long port);
unsigned int	R_mmio_outp(unsigned long port, int value);
unsigned int	R_mmio_inp(unsigned long port);

#ifdef __cplusplus
}
#endif

#endif	// R_MMIO_H_
