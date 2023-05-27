/******************************************************************************
	R_COCY34.H / R_COCY34.C
	
	変更履歴
	Ver 3.10	1999/04/14	720x480対応
				1999/06/24	ライン指定取り込み対応
				1999/06/28	Luma Gain調整関数追加
	Ver 3.20	1999/11/11	FxSize,MxSize設定追加(SetVideoSize)
				1999/12/17	Color_addressの追加
				2000/01/18	IK-TU40Dランダムシャッタ対応
				2000/01/21	ランダムシャッタタイマー追加
	Ver 3.28	2000/08/18	ランダムシャッタStart,End追加
	Ver 3.31	2002/07/23	HIGH_SHUTTER対応
	Ver 3.44	2004/10/12	長時間割込み対策
	Ver 3.45	2004/12/27	CO-CY-40暫定対応
	Ver 3.46	2005/05/31	CO-CY-40対応
	Ver 3.46	2005/06/03	cocy34_t構造体にtimeCapture追加
	Ver 3.46	2005/08/18	DMA Control bit(+0x10C)セット変更
	Ver 3.47	2005/10/04	R_cocy34_get_field関数追加
	Ver 3.47	2005/10/04	R_cocy34_change_video_card関数追加
	Ver 3.47	2005/10/04	R_cocy34_set_initiator_memory関数追加
	Ver 3.47	2006/01/07	R_cocy34_continuous_initiator関数追加
	Ver 3.48	2006/03/17	cocy34_t構造体にnVersion、nInfo追加
	Ver 3.48	2006/03/17	R_cocy34_sio_init/putc/getc関数追加
	Ver 3.48	2006/04/10	R_cocy34_get_video_auto_shutter_end_check関数追加
	Ver 3.48	2006/04/10	R_cocy34_set/get_shutter_speed関数追加
	Ver 3.48	2006/04/14	RCocy34CaptureEndCheckの戻り値を変更
	Ver 3.48	2006/04/14	R_cocy34_change_video_card Color_address[0]の切替
	Ver 3.49	2006/12/19	RCocy34CaptureEndCheckのチェック条件変更
	Ver 4.01	2009/08/24	640×480キャプチャ不具合修正
	Ver 4.01	2009/08/28	インタレースキャプチャ改善
******************************************************************************/

#ifndef	R_COCY34_H_
#define	R_COCY34_H_

#include "R_time.h"

extern int	COCY34;

#define	BOARD_ID_COCY34		(int)34
#define	BOARD_ID_COCY40		(int)40

// Video Size
#define	VS_512x480		0x0000
#define	VS_512x240		0x0004
#define	VS_256x240		0x0006
#define	VS_640x480		0x0008
#define	VS_640x240		0x000C
#define	VS_320x240		0x000E
#define	VS_512x480P		0x8000
#define	VS_640x480P		0x8008

#define	VS_512x480x32	0x0000
#define	VS_512x240x32	0x0004
#define	VS_256x240x32	0x0006
#define	VS_640x480x32	0x0008
#define	VS_640x240x32	0x000C
#define	VS_320x240x32	0x000E

#define	VS_720x480x32	0x0028
#define	VS_720x240x32	0x002C
#define	VS_360x240x32	0x002E

#define	VS_640x480x8	0x101
#define	VS_320x240x8	0x102
#define	VS_640x240x8	0x103
#define	VS_512x480x8	0x111
#define	VS_256x240x8	0x112
#define	VS_512x240x8	0x113
#define	VS_720x480x8	0x121
#define	VS_360x240x8	0x122
#define	VS_720x240x8	0x123

// Display Mode
#define	DM_MEMORY		1
#define	DM_THROUGH		2

struct cocy34_t {
	unsigned long	nBaseAddress;
	int				nIrq;
	void*			lpImage;
	void*			lpBaseImage;
	unsigned long	lpRiscCode;
	
	int						nVersion;
	int						nInfo;
	int						nSysPort;		// 40
	int						nRegPort;		// 40
	int						nBoard;
	volatile unsigned long	nStatus;		// 40
	int						nShutterSpeed;	// 40
	int						nCameraType;	// 40
	
	int				nVideoSize;
	int				nDisplayMode;
	int				nFxSize;
	int				nFySize;
	int				nMxSize;
	
	unsigned long	nCodeLoopThrough;
	unsigned long	nCodeLoopCapture;
	unsigned long	nCodeShotAnyField;
	unsigned long	nCodeShotOddField;
	unsigned long	nCodeShotEvenField;
	unsigned long	nCodeCaptureLoop;
	
	int				nCameraMode;
	int				nDrawMode;
	int				nDispLine;
	int				nDispColumn;
	int				nGraphPage;
	int 			CurrentCaptureMode;		//00/08/18 by ohshima
	
	RTIME			timeCapture;
};

extern struct cocy34_t	Cocy34Resource[4];
extern struct cocy34_t*	lpCurrentCocy34;




// CO-CY34 専用ライブラリ

int		RCocy34Open(int i, int j);
int		RCocy34SelectCard(int nID);
int		RCocy34SetVideoSize(struct cocy34_t*, int size);
int		RCocy34SetVideoSizeEx(struct cocy34_t*, int nSize, int nYSize);
int		RCocy34SetDisplayMode(struct cocy34_t*, int mode);
int		RCocy34Capture(struct cocy34_t*, int mode);
int		RCocy34CaptureStart(struct cocy34_t*, int mode);
int		RCocy34CaptureEndCheck(struct cocy34_t*);
int		RCocy34CaptureEndWait(struct cocy34_t*);
int		RCocy34SetCameraMode(struct cocy34_t*, int mode);
int		RCocy34SetVideoChannel(struct cocy34_t* lpCocy34, int nChannel);

void	RCocy34Output(struct cocy34_t*, int);
void	RCocy34OutputJudge(struct cocy34_t*, int);
void	RCocy34OutputStatus(struct cocy34_t*, int);
int		RCocy34CheckStatusSw(struct cocy34_t*);
int		RCocy34InputExternalIO(struct cocy34_t*);
void	RCocy34OutputExternalIO(struct cocy34_t*, int);
int		RCocy34CheckExternalIOStart(struct cocy34_t*);

#define	RCocy34SetVSync(lpCocy34)			(*(volatile unsigned long*)(lpCocy34->nBaseAddress + 0x200) &= ~0x010000)
#define	RCocy34ResetVSync(lpCocy34)			(*(volatile unsigned long*)(lpCocy34->nBaseAddress + 0x200) |=  0x010000)
#define	RCocy34SetSyncOutDisable(lpCocy34)	(*(volatile unsigned long*)(lpCocy34->nBaseAddress + 0x200) &= ~0x000200)
#define	RCocy34SetSyncOutEnable(lpCocy34)	(*(volatile unsigned long*)(lpCocy34->nBaseAddress + 0x200) |=  0x000200)
#define	RCocy34SetTriggerHigh(lpCocy34)		(*(volatile unsigned long*)(lpCocy34->nBaseAddress + 0x200) &= ~0x000100)
#define	RCocy34SetTriggerLow(lpCocy34)		(*(volatile unsigned long*)(lpCocy34->nBaseAddress + 0x200) |=  0x000100)
#define	RCocy34IndexIsHigh(lpCocy34)		!(*(volatile unsigned long*)(lpCocy34->nBaseAddress + 0x200) & 0x000002)
#define	RCocy34IndexIsLow(lpCocy34)			(*(volatile unsigned long*)(lpCocy34->nBaseAddress + 0x200) & 0x000002)
#define	RCocy34IsOddField(lpCocy34)			(0 == (*(volatile unsigned long*)(lpCocy34->nBaseAddress + 0x000) & 0x20))
#define	RCocy34IsEvenField(lpCocy34)		(0 != (*(volatile unsigned long*)(lpCocy34->nBaseAddress + 0x000) & 0x20))




// CO-CY34 & CO-CY-40 共通ライブラリ

#define	R_cocy34_get_fx_size()				lpCurrentCocy34->nFxSize
#define	R_cocy34_get_fy_size()				lpCurrentCocy34->nFySize
#define	R_cocy34_get_image_address()		lpCurrentCocy34->lpImage

#define		R_cocy34_set_video_size(nSize)		R_cocy34_set_video_size_ex(nSize, 0)

int			R_cocy34_init(int nCameraType, int nVideoSize, int nCameraMode);
int			R_cocy34_set_video_size_ex(int nSize, int YSize);
int			R_cocy34_set_display_mode(int nMode);
int			R_cocy34_get_video(int nMode);
int			R_cocy34_get_video_start(int nMode);
int			R_cocy34_get_video_end_wait(void);
int			R_cocy34_get_video_end_check(void);
int			R_cocy34_get_video_auto_shutter_end_check(void);
int			R_cocy34_change_camera_mode(int nMode);
int			R_cocy34_channel(int nChannel);
int			R_cocy34_set_shutter_speed(int nMicroSec);
int			R_cocy34_get_shutter_speed(void);
void		R_cocy34_output_judge(int nJudge);
void		R_cocy34_output_status(int nStatus);
int			R_cocy34_check_status_sw(void);
void		R_cocy34_wait_status_sw(void);
int			R_cocy34_input_external_io(void);
void		R_cocy34_output_external_io(int nData);
void		R_cocy34_output(int nData);
int			R_cocy34_check_external_io_start(void);
void		R_cocy34_wait_external_io_start(void);

int			R_cocy34_get_field(void);
int			R_cocy34_change_video_card(int nID);
int			R_cocy34_set_initiator_memory(void*);
int			R_cocy34_continuous_initiator(void*, int);

int			R_cocy34_sio_init(int bps, int len, int parity, int stops, int flow);
int			R_cocy34_sio_putc(int);
int			R_cocy34_sio_getc(void);

#endif	// R_COCY34_H_
