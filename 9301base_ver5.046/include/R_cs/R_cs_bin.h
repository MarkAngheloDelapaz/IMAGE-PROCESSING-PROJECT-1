/******************************************************************************

	R_CS_BIN.H

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	����
	Version		2006.05.09.000		�I���W�i��

******************************************************************************/

// --- define ---
#define BORDER_4_CONNECTIVITY 0		//4�A��
#define BORDER_8_CONNECTIVITY 1		//8�A��

// --- variable ---

// --- prototype ---
int R_CS_BinaryBorderFollowing( R_POINT tStart, int nPage, int nConnectivity, int nMax, R_POINT *pBorder );

