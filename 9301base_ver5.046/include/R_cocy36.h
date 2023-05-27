/******************************************************************************
	R_COCY36.H / R_COCY36.C
	
	変更履歴
	Ver 3.20	1999/12/12	正式リリース
	Ver 3.24	2000/01/17	SKC-130対応
	Ver 3.29	2001/06/01	TL-2048FD対応
	Ver 3.29	2001/09/13	TL-5150UFD対応
	Ver 3.29	2001/09/17	MKS-5000-40対応
	Ver 3.30	2002/01/24	SKC-133、CS3910対応
	Ver 3.42	2004/03/23	CaptureAuto、EndCheck追加
	Ver 3.44	2004/10/12	MKS-7450-40対応
	Ver 3.44	2004/10/12	cocy36_t構造体にtimeAutoCapture追加
	Ver 3.45	2005/02/16	C9321CA-02対応
	Ver 3.46	2005/08/11	CO-CY-36複数枚対応
	Ver 3.46	2005/08/16	CS3910BH対応
	Ver 3.47	2005/09/06	cocy36_t構造体にnShutterSpeed追加
	Ver 3.47	2005/09/06	RCocy36SetShutterSpeed追加
	Ver 3.47	2005/11/15	RCocy36Init、MKS-5000-40初期化バグ修正
	Ver 3.47	2005/11/18	M_cocy36.c追加
	Ver 3.47	2005/11/28	e7450D対応
	Ver 3.48	2006/02/17	cocy36_t構造体にnScanClockLimit追加
	Ver 3.48	2006/07/26	RCocy36GetOffsetAdr/GetSpanAdr削除
	Ver 3.50	2007/09/21	CAMERA_E7450D_7488対応
	Ver 3.50	2008/03/03	キャプチャ時バス負荷軽減
	Ver 3.50	2008/03/26	CO-CZ-43使用時SU74&SU51対応
	Ver 3.50	2008/06/02	CO-CY-45対応
	Ver 3.50	2008/06/04	CV-M4+対応
	Ver 5.005	2018/07/04	XCM6040SAT2対応
******************************************************************************/

#ifndef R_COCY36_H_
#define R_COCY36_H_

#define	COCY45_EEPROM_ADR_MAKER_ID_CODE	0x40
#define	COCY45_EEPROM_ADR_PRODUCT_TIME	0x50

#include "R_time.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int	COCY36;
extern int	COCY45;

struct cocy36_t {
	int				nID;
	int				nBoard;
	int				nS5933Port;
	int				nRegisterPort;
	int				nIrq;
	
	void*			lpImageAddress;
	int				nWithShadingCorrection;
	unsigned char*	lpOffsetPtr;
	unsigned char*	lpSpanPtr;
	int				nDeviceMode;
	int				nCameraMode;
	int				nLineSize;
	int				nScanLine;
	int				nScanClock;
	
	int				nCaptureByte;
	int				nRemainByte;
	
	RTIME			timeAutoCapture;
	int				nAutoCaptureTimeLimit;
	int				nAutoCaptureTimeNext;
	
	int				nShutterSpeed;
	int				nScanClockLimit;
	
	RTIME			timeEndCheck;
	int				nEndCheckInterval;
	int				nEndCheckResult;
	
	volatile unsigned long	ulStatus[4];
};

typedef struct cocy36_t	RCOCY36;

#define	CAMERA_TL5150FD		0x00010000
#define	CAMERA_SKC130		0x00020001
#define	CAMERA_CS3910		0x00020002
#define	CAMERA_CS3910_RS	0x00120002
#define	CAMERA_TL2048FD		0x00010003
#define	CAMERA_TL5150UFD	0x00010004
#define	CAMERA_MKS5000_40	0x00010005
#define	CAMERA_SKC133		0x00020006
#define	CAMERA_SKC133_RS	0x00120006
#define	CAMERA_MKS7450_40	0x00010007
#define	CAMERA_C9321CA_02	0x00020008
#define	CAMERA_CS3910BH		0x00020009
#define	CAMERA_CS3910BH_RS	0x00120009
#define	CAMERA_E7450D		0x0001000A
#define	CAMERA_E7450D_7488	0x0001000B
#define	CAMERA_SU74			0x0001000C		// CO-CZ-44
#define	CAMERA_SU51			0x0001000D		// CO-CZ-44
#define	CAMERA_SUI74		0x0001000E		// CO-CZ-44
#define	CAMERA_SUI51		0x0001000F		// CO-CZ-44
#define	CAMERA_CVM4P		0x0002000A
#define	CAMERA_CVM4P_RS		0x0012000A
#define	CAMERA_CM141PMCL_RS	0x0012000B		// CO-CZ-43
#define	CAMERA_KPF145WCL_RS	0x0012000C		// CO-CZ-43
#define	CAMERA_XCM6040SAT2	0x00010010		// CO-CZ-44

#define	COCY36_ID_MAX	(int)3

extern struct cocy36_t	Cocy36Resource[COCY36_ID_MAX + 1];
extern struct cocy36_t*	lpCurrentCocy36;

struct cocy36_t* RCocy36Create(int nID);
int		RCocy36Init(struct cocy36_t*, int line_size);
int		RCocy36Capture(struct cocy36_t*, unsigned char*, int);
int		RCocy36CaptureAuto(struct cocy36_t*, unsigned char*, int);
int		RCocy36CaptureEndCheck(struct cocy36_t*);
int		RCocy36SetShutterSpeed(struct cocy36_t*, int);
int		RCocy36GetShutterSpeed(struct cocy36_t*);

// 内部関数
int		RCocy36Open(int nBusNo, int nDeviceNo);
int		RCocy45Open(int nBusNo, int nDeviceNo);
int		RCocy36Exist(void);
int		RCocy36CaptureEx(struct cocy36_t*, unsigned char*, int, int);
int		RCocy36SetCaptureTiming(struct cocy36_t*, int, int);
int		RCocy36GetCaptureTiming(struct cocy36_t*, int*, int*);
int		RCocy36SetCaptureLine(struct cocy36_t*, int, int);
int		RCocy36GetCaptureLine(struct cocy36_t*, int*, int*);
int		RCocy36SetStartPulse(struct cocy36_t*, int, int);
int		RCocy36SetScanClock(struct cocy36_t*, int);
int		RCocy36GetScanClock(struct cocy36_t*);
int		RCocy36SetScanLine(struct cocy36_t*, int nLine);
int		RCocy36GetScanLine(struct cocy36_t*);
int		RCocy36GetLineSize(void);

#ifdef __cplusplus
}
#endif

#endif	// R_COCY36_H_
