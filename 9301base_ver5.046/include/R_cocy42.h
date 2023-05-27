/******************************************************************************
	R_COCY42.H / R_COCY42.C
	
	変更履歴
	Ver 3.45	2005/03/22	暫定ソース
	Ver 3.47	2005/12/01	CO-CY-42複数枚対応
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
