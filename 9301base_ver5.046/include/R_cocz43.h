/******************************************************************************
	R_COCZ43.H / R_COCZ43.C
	
	変更履歴
	Ver 3.49	2006/08/08	正式リリース
	Ver 3.50	2007/11/16	CM-200MCL対応
	Ver 3.50	2007/12/21	RCocz43GpioControl追加
	Ver 3.50	2008/01/15	IK-TF5C対応
	Ver 3.50	2008/03/07	R_planeヘッダファイル統合
	Ver 3.50	2008/03/24	ラインセンサカメラ用関数追加
	Ver 3.50	2008/03/24	KP-F200PCL対応
	Ver 3.50	2008/05/09	ビニング表示対応
	Ver 3.50	2008/06/26	IK-TF5P対応（ビニングモード）
	Ver 3.50	2008/07/04	CM-030PMCL-RH対応
	Ver 3.50	2008/07/29	RCocz43CameraIsPresent関数追加
	Ver 3.50	2008/09/25	RCocz43CheckVSタイミング修正
	Ver 3.50	2008/09/26	512×480時VGA表示改善
	Ver 3.50	2008/10/23	RCocz43Set/GetDisplayMode関数追加
	Ver 3.50	2008/12/08	カメラ初期化ルーチン修正
	Ver 3.50	2008/12/10	KP-F80PCLパーシャルスキャン対応
	Ver 3.50	2009/01/19	転送＆露光・オーバラップモード追加
	Ver 3.50	2009/02/26	外部トリガ入力遅延対応
	Ver 3.50	2008/02/27	KP-F31PCLパーシャルスキャン対応
	Ver 3.50	2008/03/05	IK-TF7P対応
	Ver 3.50	2009/06/26	SUi51・12bit出力対応
	Ver 4.01	2010/02/18	Scatter/Gather対応
	Ver 4.01	2010/03/30	SUi74画素数可変対応
	Ver 4.01	2010/06/08	AT-140CL/AT-200CL対応
	Ver 4.01	2010/09/14	CO-CZ-43複数枚マルチプロセス対応
	Ver 4.01	2010/11/05	SUi51/74トリガパルス幅改善
	Ver 4.01	2010/12/24	KP-F31PCLパーシャル＆オーバラップ対応
	Ver 4.01	2011/01/27	東芝製カメラ通信コマンド拡張
	Ver 4.01	2011/03/15	RCocz43DmaToVram/EndCheck関数追加
	Ver 4.01	2011/03/23	VCC-F22V39APCL対応
	Ver 4.01	2011/04/04	PoCL-Lite対応(CSCLV90BC3)
	Ver 4.01	2011/04/12	VCC-GC20V41PCL対応
	Ver 4.01	2011/06/29	RCocz43SetLinePage関数追加
	Ver 4.01	2011/07/19	VCC-G21V21RM対応
	Ver 4.01	2011/07/27	VCC-GC20U11PCL対応
	Ver 4.01	2011/09/05	VCC-FC20U19PCL&VCC-F22U29APCL対応
	Ver 4.01	2011/09/09	AM-800CL対応
	Ver 4.01	2011/09/14	VCC-G21V21RM・ビニング対応
	Ver 4.01	2012/01/24	NED_RemoteWrite関数・タイムアップ変更
	Ver 4.01	2012/01/25	XCL-5005パーシャルスキャン対応
	Ver 4.01	2012/11/09	XCL-C32対応
	Ver 4.01	2012/11/21	VCC-GC20U11PCL・VGAサイズ対応
	Ver 4.01	2013/02/07	XCL-S600対応
	Ver 4.01	2013/06/21	XCM8040SAT2V1対応
	Ver 4.01	2013/07/31	acA2040-180kc対応
	Ver 5.010	2018/12/20	i386 on x86_64対応
******************************************************************************/

#ifndef	R_COCZ43_H_
#define	R_COCZ43_H_

#ifdef __cplusplus
extern "C" {
#endif

#define	COCZ43_EEPROM_ADR_MAKER_ID_CODE	0x40
#define	COCZ43_EEPROM_ADR_PRODUCT_TIME	0x50

#include "R_stdlib.h"	// R_RECT
#include "R_plane.h"
#include "R_cocz33.h"

#ifdef __x86_64__
#include "R_memmgr.h"
#endif

extern int	COCZ43;

#ifdef __GNUC__
int		RCocz43OpenAll(int bMultiProcess, int nBoardID);
#endif

enum RCocz43OpenMode {
	PHYSICAL = 0,
	VIRTUAL_CAMERA,
	DISPLAY_ONLY
};
int		RCocz43Open(int nBusNo, int nDeviceNo, enum RCocz43OpenMode mode);

int		RCocz43DrawPlaneDMAStart(struct cocz33_t*);
int		RCocz43DrawPlaneDMAStop(struct cocz33_t*);

int		RCocz43GetwEEPROM(struct cocz33_t*, int nAdr);
int		RCocz43PutwEEPROM(struct cocz33_t*, int nAdr, int nData);

int		RCocz43CheckVS(struct cocz33_t* lpCocz43);
int		RCocz43CheckDispt(struct cocz33_t* lpCocz43);

int		RCocz43SioInit(struct cocz33_t*, int bps, int len, int parity, int stops, int flow);
int		RCocz43SioPutc(struct cocz33_t*, int nData);
int		RCocz43SioGetc(struct cocz33_t*);

int		RCocz43SetCameraChannel(struct cocz33_t*, int nFlag, int nChannel, int nBinLevel);
int		RCocz43ChangeVideoMode(struct cocz33_t*, int nMode);
int		RCocz43ChangeCameraMode(struct cocz33_t*, int nMode);
int		RCocz43DisplayControl(struct cocz33_t*, int nMode);

int		RCocz43SetDisplayPage(struct cocz33_t*, int);
int		RCocz43GetDisplayPage(struct cocz33_t*);

int		RCocz43Init(struct cocz33_t*, int nCameraType, int nFrameSize, int nCameraMode, int nMode);
int		RCocz43GetVideo(struct cocz33_t*, int nMode, int nFlag);
int		RCocz43GetVideoAuto(struct cocz33_t*, int, int);
int		RCocz43GetVideoAutoEndCheck(struct cocz33_t*);
int		RCocz43GetVideoAutoShutterEndCheck(struct cocz33_t*);

int		RCocz43SetScanLine(struct cocz33_t*, int);
int		RCocz43GetScanLine(struct cocz33_t*);
int		RCocz43SetScanRate(struct cocz33_t*, int);
int		RCocz43GetScanRate(struct cocz33_t*);
int		RCocz43GetVideoAutoLineCheck(struct cocz33_t*);

int		RCocz43InitLookUpTable(struct cocz33_t*, const unsigned char ucLut[]);
int		RCocz43PixelClock(struct cocz33_t*);

int		RCocz43SetWinMxSize(struct r_display_t* pDisplay, int nWindow, int nMxSize);
int		RCocz43SetWinDataAdr(struct r_display_t* pDisplay, int nWindow, int nAdr);
int		RCocz43SetWinBinAdr(struct r_display_t* pDisplay, int nWindow, int nAdr);

int		RCocz43DisplaySetWinMode(struct r_display_t* pDisplay, int nWindow, int nMode);
int		RCocz43DisplayGetWinMode(struct r_display_t* pDisplay, int nWindow);
int		RCocz43DisplaySetWinPage(struct r_display_t* pDisplay, int nWindow, int nPage);
int		RCocz43DisplayGetWinPage(struct r_display_t* pDisplay, int nWindow);
int		RCocz43DisplaySetWinDisplay(struct r_display_t* pDisplay, int nMode);
int		RCocz43DisplayGetWinDisplay(struct r_display_t* pDisplay);
int		RCocz43DisplaySetWinRect(struct r_display_t* pDisplay, int nWindow, R_RECT* pRect);
int		RCocz43DisplayGetWinRect(struct r_display_t* pDisplay, int nWindow, R_RECT* pRect);
int		RCocz43DisplaySetWinZoom(struct r_display_t* pDisplay, int nWindow, int nZoom);
int		RCocz43DisplayGetWinZoom(struct r_display_t* pDisplay, int nWindow);
int		RCocz43DisplaySetWinOffset(struct r_display_t* pDisplay, int nWindow, R_POINT* pPoint);
int		RCocz43DisplayGetWinOffset(struct r_display_t* pDisplay, int nWindow, R_POINT* pPoint);
int		RCocz43DisplaySetWinSwitch(struct r_display_t* pDisplay, int nWindow, int nSwitch);
int		RCocz43DisplayGetWinSwitch(struct r_display_t* pDisplay, int nWindow);
int		RCocz43DisplaySetWinFrame(struct r_display_t* pDisplay, int nWindow, int nMode);
int		RCocz43DisplayGetWinFrame(struct r_display_t* pDisplay, int nWindow);
int		RCocz43DisplaySetWinHalftone(struct r_display_t* pDisplay, int nMode);
int		RCocz43DisplayGetWinHalftone(struct r_display_t* pDisplay);
int		RCocz43DisplaySetWinPriority(struct r_display_t* pDisplay, int nMode);
int		RCocz43DisplayGetWinPriority(struct r_display_t* pDisplay);
int		RCocz43DisplaySetWinMxSize(struct r_display_t* pDisplay, int nWindow, int nMxSize);
int		RCocz43DisplayDDC(struct r_display_t* pDisplay);

int		RCocz43DmaGrayMemory(struct cocz33_t* lpCocz43, int nSrcNum, int nDstNum, R_RECT* pRect);
int		RCocz43PrintScreen(struct cocz33_t* lpCocz43, void* pDataBuffer);
int		RCocz43GpioControl(struct cocz33_t* lpCocz43, int nBit, int nMode);
int		RCocz43GpioSetSyncOutDelay(struct cocz33_t* lpCocz43, int nBit, int nTurnOnDelay, int nTurnOffDelay);

int		RCocz43SetCameraGain(struct cocz33_t*, int);
int		RCocz43GetCameraGain(struct cocz33_t*);

int		RCocz43CameraIsPresent(struct cocz33_t*);
int		RCocz43CameraFval(struct cocz33_t* lpCocz43);
int		RCocz43TriggerInputIsOn(struct cocz33_t* lpCocz43);

int		RCocz43SetShutterSpeed(struct cocz33_t*, int nMicroSec);

int		RCocz43SetDisplayMode(struct cocz33_t*, int nMode);
int		RCocz43GetDisplayMode(struct cocz33_t*);
int		RCocz43SetInitiatorMode(struct cocz33_t* lpCocz43, int nMode);
int		RCocz43GetInitiatorMode(struct cocz33_t* lpCocz43);

int		RCocz43SetPartialScanMode(struct cocz33_t*, int nMode, int nStart, int nWidth);

void	RCocz43GetVideoReset(struct cocz33_t*);
int		RCocz43GetVideoOverlap(struct cocz33_t*, int, int);
int		RCocz43GetVideoOverlapEndCheck(struct cocz33_t*, int);
int		RCocz43GetVideoOverlapShutterEndCheck(struct cocz33_t*, int);
int		RCocz43GetVideoOverlapIsReady(struct cocz33_t* lpCocz43);

int		RCocz43SetShutterTiming(struct cocz33_t* lpCocz43, int nTurnOnTiming, int nTurnOffTiming);
int		RCocz43SetSyncOutTiming(struct cocz33_t* lpCocz43, int nSignal, int nTurnOnTiming, int nTurnOffTiming);

int		RCocz43SetTriggerInputDelay(struct cocz33_t* lpCocz43, int nMicroSec);
int		RCocz43GetTriggerInputDelay(struct cocz33_t* lpCocz43);

int		RCocz43CameraLinkInit(struct cocz33_t* lpCocz43, int nCameraType);
int		RCocz43CameraLinkRead(struct cocz33_t* lpCocz43, int nAdr);
int		RCocz43CameraLinkWrite(struct cocz33_t* lpCocz43, int nAdr, int nData);

int		RCocz43DisplayWindowReset(struct cocz33_t* lpCocz43);

int		RCocz43PrintBoardStatus(struct cocz33_t* lpCocz43, int nMode);

void	RCocz43SetLinePage(struct cocz33_t* lpCocz43, int nMode);

int		RCocz43SetCaptureVramOffset(struct cocz33_t* lpCocz43, unsigned long seg0, unsigned long seg1);

int		RCocz43ClisbeeCommandWrite(struct cocz33_t* lpCocz43, const char* command);
int		RCocz43ClisbeeCommandWriteTimeout(struct cocz33_t* lpCocz43, const char* command, int timeout);

int		RCocz43SetHorizontalTrimmingOffset(struct cocz33_t* lpCocz43, int num, int offset);

int		RCocz43SetInitiatorOffset(struct cocz33_t* lpCocz43, int bytes);

int		RCocz43SetCaptureStartPixel(struct cocz33_t* lpCocz43, int start);
int		RCocz43GetCaptureStartPixel(struct cocz33_t* lpCocz43);

#ifdef __x86_64__
int		RCocz43GrabStart(struct cocz33_t* lpCocz43, int nMode, struct r_memmgr* pMemmgrNode, unsigned long nOffset);
int					RCocz43DmaMemoryInit(struct cocz33_t* lpCocz43, int num);
struct r_memmgr*	RCocz43DmaMemoryAlloc(struct cocz33_t* lpCocz43, int size);
int					RCocz43DmaMemoryFree(struct cocz33_t* lpCocz43, struct r_memmgr* pMemmgrNode);
#endif	// __x86_64__

#ifdef __GNUC__
int		RCocz43DmaToVram(struct cocz33_t* lpCocz43, int nPage, R_RECT* pRect);
int		RCocz43DmaEndCheck(struct cocz33_t* lpCocz43);
int		RCocz43AutoBayerColorInterpolation(struct cocz33_t* lpCocz43, R_PLANE* plane, int interpolation, int white_balance);
#endif	// __GNUC__

#ifdef __cplusplus
}
#endif

#endif	// R_COCZ43_H_
