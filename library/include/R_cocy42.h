/******************************************************************************
	R_COCY42.H / R_COCY42.C
	
	�ύX����
	Ver 3.45	2005/03/22	�b��\�[�X
	Ver 3.47	2005/12/01	CO-CY-42�������Ή�
******************************************************************************/

#ifndef	R_COCY42_H_
#define	R_COCY42_H_

extern int	COCY42;

struct cocy42_t {
	int			nBoard;
	int			nSysPort;
	int			nRegPort;
	int			nIrq;
};

extern struct cocy42_t	Cocy42Resource[4];
extern struct cocy42_t*	lpCurrentCocy42;

int		RCocy42Open(int nBusNo, int nDeviceNo);

#endif	// R_COCY42_H_
