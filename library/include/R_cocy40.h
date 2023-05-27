/******************************************************************************
	R_COCY40.H / R_COCY40.C
	
	変更履歴
	Ver 3.45	2004/11/02	暫定ソース
	Ver 3.46	2005/08/09	正式リリース
	Ver 3.47	2005/10/04	RCocy40GetField関数追加
	Ver 3.47	2005/10/04	RCocy40SetInitiatorMemory関数追加
	Ver 3.47	2006/01/07	RCocy40ContinuousInitiator関数追加
	Ver 3.48	2006/03/08	boot Version Display
	Ver 3.48	2006/03/15	IKTU50対応（Ver.2）
	Ver 3.48	2006/03/17	DrawPlane対応（Ver.2）
	Ver 3.48	2006/03/17	RCocy40SioInit/Putc/Getc関数追加
	Ver 3.48	2006/04/10	RCocy40CaptureShutterEndCheck関数追加
	Ver 3.48	2006/04/14	RCocy40CaptureEndCheckの戻り値を変更
	Ver 3.48	2006/07/25	メーカコード、シリアルナンバー表示
******************************************************************************/

#ifndef	R_COCY40_H_
#define	R_COCY40_H_

extern int	COCY40;

int		RCocy40Open(int nBusNo, int nDeviceNo);
int		RCocy40Init(struct cocy34_t* lpCocy40, int nCameraType, int nVideoSize, int nMode);
int		RCocy40GetwEEPROM(struct cocy34_t* lpCocy40, int nAdr);
int		RCocy40PutwEEPROM(struct cocy34_t* lpCocy40, int nAdr, int nData);
int		RCocy40SetDacBU2505(struct cocy34_t* lpCocy40, int nChannel, int nData);
int		RCocy40SetPLL(struct cocy34_t* lpCocy40, int nAdr, int nData);
int		RCocy40SetHorizontalArea(struct cocy34_t* lpCocy40, int nStart, int nEnd);
int		RCocy40GetHorizontalAreaStart(struct cocy34_t* lpCocy40);
int		RCocy40GetHorizontalAreaEnd(struct cocy34_t* lpCocy40);
int		RCocy40SetVerticalArea(struct cocy34_t* lpCocy40, int nStart, int nEnd);
int		RCocy40GetVerticalAreaStart(struct cocy34_t* lpCocy40);
int		RCocy40GetVerticalAreaEnd(struct cocy34_t* lpCocy40);

int		RCocy40SetVideoSizeEx(struct cocy34_t* lpCocy40, int nSize, int nYSize);

int		RCocy40SetShutterSpeed(struct cocy34_t* lpCocy40, int nShutterSpeed);

int		RCocy40CaptureStart(struct cocy34_t* lpCocy40, int nMode);

// CO-CY-34 compatible function
int		RCocy40SetCameraMode(struct cocy34_t*, int mode);
int		RCocy40SetDisplayMode(struct cocy34_t*, int nMode);
int		RCocy40CaptureEndWait(struct cocy34_t*);
int		RCocy40CaptureEndCheck(struct cocy34_t*);
int		RCocy40CheckStatusSw(struct cocy34_t*);
int		RCocy40CheckExternalIOStart(struct cocy34_t*);
void	RCocy40OutputExternalIO(struct cocy34_t*, int);
void	RCocy40OutputStatus(struct cocy34_t*, int);
int		RCocy40InputExternalIO(struct cocy34_t*);
void	RCocy40OutputJudge(struct cocy34_t*, int);
void	RCocy40Output(struct cocy34_t*, int);
int		RCocy40GetField(struct cocy34_t*);
int		RCocy40SetInitiatorMemory(struct cocy34_t*, void*);
int		RCocy40ContinuousInitiator(struct cocy34_t*, void*, int);
int		RCocy40CaptureShutterEndCheck(struct cocy34_t*);

int		RCocy40SioInit(struct cocy34_t*, int bps, int len, int parity, int stops, int flow);
int		RCocy40SioPutc(struct cocy34_t*, int data);
int		RCocy40SioGetc(struct cocy34_t*);

#endif	// R_COCY40_H_
