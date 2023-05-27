/******************************************************************************
	R_CLSCAM.H / R_CLSCAM.C
	
	•ÏX—š—ğ
	Ver 3.47	2005/11/01	³®ƒŠƒŠ[ƒX
	Ver 3.47	2006/01/07	R_clscam_set_capture_offsetŠÖ”’Ç‰Á
******************************************************************************/

#ifndef	R_CLSCAM_H_
#define	R_CLSCAM_H_

#define	CLSCAM_NUCL7500D	0x00400000
#define	CLSCAM_NORMAL_MODE	0

int	R_clscam_open(int nID);
int	R_clscam_init(int hCam, int nCameraType, int nMode);
int	R_clscam_set_capture_timing(int hCam, int nStartTiming, int nEndTiming);
int	R_clscam_set_scan_clock(int hCam, int nScanClock);
int	R_clscam_set_initiator_plane(int hCam, void* pPlane0, void* pPlane1, void* pPlane2);
int	R_clscam_set_capture_line(int hCam, int nCaptureLine);
int	R_clscam_get_video_auto(int hCam);
int	R_clscam_get_video_auto_end_check(int hCam);
int	R_clscam_set_capture_offset(int hCam, int nRedOffset, int nGreenOffset, int nBlueOffset);

#endif	// R_CLSCAM_H_
