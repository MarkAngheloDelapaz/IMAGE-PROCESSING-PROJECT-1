/******************************************************************************
	R_COCZ23.H / R_COCZ23.C
	
	ïœçXóöó
	Ver 3.50	2009/02/13	êVãKçÏê¨
******************************************************************************/

#ifndef	R_COCZ23_H_
#define	R_COCZ23_H_

int		isCOCZ23(void);
int		isCOCZ27(void);
int		isCOCZ31(void);

int		R_regulate_COCZ27(int Vrt, int Vrb, int Vref);
int		SetDac6325(int ch, int data);

void	r_cocz23_wait_dispt(void);

int		RCocz23SetCameraChannel(int flag, int channel, int bin_level);
int		RCocz23ChangeVideoMode(int mode);
int		RCocz23GetVideo(int mode, int flag);
int		RCocz23GetVideoAuto(int mode, int flag);
int		RCocz23GetVideoAutoEndCheck(void);
int		RCocz23DisplayControl(int mode);
int		RCocz23SetBinLevel(int level);
void	RCocz23SetLinePage(int mode);
int		RCocz23ChangeCameraMode(int nMode);
int		RCocz23ChangeVideoCard(int CardId);
int		RCocz23ChangeVideoCard(int CardId);
void	RCocz23OutputJudge(short int judge);
void	RCocz23OutputStatus(short int mode);
int		RCocz23CheckStatusSw(void);
int		RCocz23InputExternalIO(void);
void	RCocz23OutputExternalIO(int value);
int		RCocz23CheckExternalIOStart(void);

#endif	// R_COCZ23_H_
