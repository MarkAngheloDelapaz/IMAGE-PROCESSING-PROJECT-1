/******************************************************************************
	R_CAMLNK.H / R_CAMLNK.C
	
	変更履歴
	Ver 3.49	2006/11/22	正式リリース
	Ver 3.50	2008/07/04	対応カメラを追加
	Ver 3.50	2009/02/13	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_CAMLNK_H_
#define	R_CAMLNK_H_

int	R_camlnk_sio_init(int nCameraType);
int	R_camlnk_sio_write(int nAdr, int nData);
int	R_camlnk_sio_read(int nAdr);

#endif	// R_CAMLNK_H_
