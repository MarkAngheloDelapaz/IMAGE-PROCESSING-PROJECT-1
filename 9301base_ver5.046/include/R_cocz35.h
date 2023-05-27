/******************************************************************************
	R_COCZ35.H / R_COCZ35.C
	
	変更履歴
	Ver 3.20	1999/12/12	正式リリース
				2000/02/15	RCocz35Shutter追加
				2000/09/27	CS8530D-01対応
				2000/11/17	内部関数RCocz35SetCameraChannel追加
	Ver 3.30	2001/08/24	CS8531ランダムシャッタ改善
	Ver 3.33	2002/11/15	CS8550Di/CS8541D対応
	Ver 3.34	2003/01/20	CO-CZ-35 Revision.2 対応
	Ver 3.35	2003/03/27	XC-HR50 STANBYへ変更直後の同期流れ対策
	Ver 3.38	2003/04/18	XC-HR50 NORMALへ変更直後の同期流れ対策
	Ver 3.38	2003/05/15	複数枚同時取込関数CaptureMulti追加
	Ver 3.37	2003/05/23	XC-HR50ノーマル取り込み不具合修正
	Ver 3.38	2003/05/28	XC-HR50 オートシーケンス取込対応
	Ver 3.39	2003/06/19	ANY_FIELD オートシーケンス取込対応
	Ver 3.39	2003/06/23	GetVideo PROGRESSIVE-ANY_FIELD修正
	Ver 3.39	2003/06/24	ChangeCameraMode再設定時タイマ省略
	Ver 3.39	2003/06/24	RCocz35SetCameraChannelにNO_CHECKを追加
	Ver 3.41	2003/12/03	CaptureMultiにｵｰﾄｼｰｹﾝｽﾓｰﾄﾞ追加
	Ver 3.43	2004/09/30	ランダムシャッタ時の_disable変更
	Ver 3.44	2004/10/07	長時間割込み対策
	Ver 3.45	2004/10/21	RCocz35CaptureMultiバス負荷軽減
	Ver 3.45	2004/10/22	RCocz35Openの戻り値をID番号に変更
	Ver 3.45	2005/04/01	RCocz35GetVideoAutoEndCheck修正
							Ver3.44で常にERRORリターンしていた。
	Ver 3.46	2005/04/01	RCocz35ChangeVideoMode関数改善
							VM_BOARD_SYNC対応
	Ver 3.46	2005/04/07	RCocz35GetVideoAutoEndCheck修正
							Ver3.45でTimeUpエラーが発生していた。
	Ver 3.47	2005/10/27	XCHR50とCS8550DのANY_FIELD取込修正
							Ver3.44でXCHR50とCS8550DでANY_FIELD取込不能になっていた。
	Ver 3.47	2005/11/14	XCST50ランダムシャッタ修正
	Ver 3.48	2006/03/02	GetVideoAuto関数、CS8530DのBINNING対応
	Ver 3.49	2007/01/06	初期化ルーチン変更
	Ver 3.49	2007/03/29	R_copy用ルーチン関数化
	Ver 3.50	2008/11/04	キャプチャ時スルー表示自動切替改善
******************************************************************************/

#ifndef __R_COCZ35_H__
#define __R_COCZ35_H__

#include "R_cocz33.h"

extern int	COCZ35;

int	RCocz35Open(int i, int j);
int	RCocz35SetPLL(struct cocz33_t*, int, int);

int	RCocz35SetCameraHorizontalArea(struct cocz33_t*, int);
int	RCocz35GetCameraHorizontalArea(struct cocz33_t*);
int	RCocz35SetCameraVerticalArea(struct cocz33_t*, int);
int	RCocz35GetCameraVerticalArea(struct cocz33_t*);
int	RCocz35SetDisplayHorizontalArea(struct cocz33_t*, int);
int	RCocz35GetDisplayHorizontalArea(struct cocz33_t*);
int	RCocz35SetDisplayVerticalArea(struct cocz33_t*, int);
int	RCocz35GetDisplayVerticalArea(struct cocz33_t*);
int	RCocz35SetShutterSpeed(struct cocz33_t*, int);
int	RCocz35Shutter(struct cocz33_t*, int);

int	RCocz35ChangeVideoMode(struct cocz33_t*, int);
int	RCocz35ChangeCameraMode(struct cocz33_t*, int);
int	RCocz35GetVideo(struct cocz33_t*, int nMode, int nFlag);
int	RCocz35SetCameraChannel(struct cocz33_t*, int nFlag, int nChannel, int nBinLevel);

int	RCocz35GetVideoAuto(struct cocz33_t*, int, int);
int	RCocz35GetVideoAutoEndCheck(struct cocz33_t*);

int	RCocz35CaptureMulti(int nBoard, int nMode, int nFlag);

int	RCocz35PrintScreen(struct cocz33_t* lpCocz35, void* pDataBuffer);

int	RCocz35InitAdcReference(struct cocz33_t* lpCocz35);

#define	RCocz35DisplayControl				RCocz33DisplayControl
#define	RCocz35SetTriggerHigh(lpCocz35)		outp((lpCocz35)->nOutPort5, (lpCocz35)->nOutData5 &= ~0x08)	// Trigger high
#define	RCocz35SetTriggerLow(lpCocz35)		outp((lpCocz35)->nOutPort5, (lpCocz35)->nOutData5 |=  0x08)	// Trigger Low
#define	RCocz35GetShutterSpeed(lpCocz35)	(lpCocz35)->nShutterSpeed

#endif
