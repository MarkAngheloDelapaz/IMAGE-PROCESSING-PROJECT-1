/******************************************************************************
	R_CAMLNK.H / R_CAMLNK.C
	
	�ύX����
	Ver 3.49	2006/11/22	���������[�X
	Ver 3.50	2008/07/04	�Ή��J������ǉ�
	Ver 3.50	2009/02/13	DOS & LINUX �\�[�X����
******************************************************************************/

#ifndef	R_CAMLNK_H_
#define	R_CAMLNK_H_

int	R_camlnk_sio_init(int nCameraType);
int	R_camlnk_sio_write(int nAdr, int nData);
int	R_camlnk_sio_read(int nAdr);

#endif	// R_CAMLNK_H_
