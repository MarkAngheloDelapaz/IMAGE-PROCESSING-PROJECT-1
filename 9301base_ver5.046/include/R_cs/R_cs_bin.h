/******************************************************************************

	R_CS_BIN.H

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	Version		2006.05.09.000		オリジナル

******************************************************************************/

// --- define ---
#define BORDER_4_CONNECTIVITY 0		//4連結
#define BORDER_8_CONNECTIVITY 1		//8連結

// --- variable ---

// --- prototype ---
int R_CS_BinaryBorderFollowing( R_POINT tStart, int nPage, int nConnectivity, int nMax, R_POINT *pBorder );

