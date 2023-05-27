/******************************************************************************
	R_XRCAM.H / R_XRCAM.C
	
	変更履歴
	Ver 3.45	2005/03/23	正式リリース
	Ver 3.47	2005/11/08	C7943CA-02対応
	Ver 3.47	2005/11/08	R_xrcam_init他追加
	Ver 3.47	2005/12/01	外部同期モード追加
	Ver 3.50	2009/02/13	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_XRCAM_H_
#define	R_XRCAM_H_

#define	XRCAM_C9321CA_02	0x00800000
#define	XRCAM_C7943CA_02	0x00800001
#define	XRCAM_NORMAL_MODE	0
#define	XRCAM_TRIGGER_MODE	1

int	R_xrcam_open(int nID);
int	R_xrcam_init(int hCam, int nCameraType, int nMode);
int	R_xrcam_get_video_auto(int hCam, char* pImage);
int	R_xrcam_get_video_auto_end_check(int hCam);
int	R_xrcam_get_video_auto_shutter_check(int hCam);

int	R_xrcam_get_fx_size(int hCam);
int	R_xrcam_get_mx_size(int hCam);
int	R_xrcam_get_fy_size(int hCam);
int	R_xrcam_get_horizontal_offset(int hCam);
int	R_xrcam_get_vertical_offset(int hCam);

int	R_xrcam_set_horizontal_offset(int hCam, int nOffset);
int	R_xrcam_set_vertical_offset(int hCam, int nOffset);

int	R_xrcam_set_trigger_cycle(int hCam, int nCycleTime);
int	R_xrcam_get_trigger_cycle(int hCam);
int	R_xrcam_trigger_start(int hCam);
int	R_xrcam_trigger_stop(int hCam);

#endif	// R_XRCAM_H_
