/******************************************************************************

	R_CS_CAM.H
	
	画像取り込み関数ヘッダ
	バージョンアップ時は、必ず履歴の追加を行い、
	グループ内で変更内容をアナウンスすること！！

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	<version>	<date>				<name>		<comment>
	0.01		2004.03.15						作成
	0.02		2006.09.08						VM_1024_768追加
	0.03		2007.02.02						VM_1280_1024、VM_1600_1200追加
	0.04		2007.09.27			sassa		以下の関数を追加(デバッグ中)
													R_CS_CameraShutterWait()
													R_CS_CameraDisplayImage()
													R_CS_CameraChangeDisplayId()
				2007.09.28			matsu		以下の関数を追加
													R_CS_CameraSetGain()
													R_CS_CameraGetGainLimit()
	1.07b1.4	2008.06.21			matsu		以下の関数を追加
													R_CS_CameraSetShutterSpeed()
													R_CS_CameraGetShutterSpeed()
													R_CS_CameraSetInitiatorPage()
													R_CS_CameraGetInitiatorPage()
													R_CS_CameraSetInitiatorAddress()
													R_CS_CameraGetInitiatorAddress()
				2008.08.25			matsu		R_CS_CameraSetInitiatorPage()マクロ→関数化
	1.07b1.6	2009.05.27			saito		KPFR31PCL(VGA RAW出力 COLOR CAMERA）対応
	1.07b1.9	2009.07.16			matsu		R_CS_CameraGetType()追加
	1.07b2.1	2009.08.10			araki		ファイル名R_CS_CAM.HをR_cs_cam.hに変更
				2009.08.10			sassa		R_CS_CameraDisplayImage()、R_CS_CameraChangeDisplayId()修正
												(R_cs_cap.c/hのR_CS_CaptureCameraChange()修正に伴い)
	1.07b2.7	2010.02.03			matsu		CAM_XCL5005対応
				2010.02.20			matsu		Linux対応
	1.07b2.8	2010.04.24			oshima		IK-TF5P/7P対応
												DisplaySize Wide対応
												CAM_KPFR80PCL対応
												(暫定でCAM_KPFR80PCLをR_cs_cam.hで定義、ライブラリ対応次第削除)
	1.07b2.9	2010.11.02			oshima		R_video_initのMode設定対応
	1.07b3.0	2010.11.15			sassa		CAPTURE_MAX_NBを3→4に変更
	1.07b3.0	2010.12.16			tanaka		VM_2432_2048を定義

	1.07b3.0	2011.03.25			saito		9301lib:ver4.01beta44以降 R_video.h で CAM_KPFR80PCL対応
												それ以前のバージョンのために、暫定コードは残す
	1.07b5.2	2013.11.03			oshima		VM_2048_2048を定義
	1.07b7.7	2016.04.15			oshima		CAPTURE_MAX_NBを4→10に変更

	
												
******************************************************************************/
#ifndef __R_CS_CAM_H__
#define __R_CS_CAM_H__

//暫定
#ifndef CAM_KPFR80PCL
	#define	CAM_KPFR80PCL		0xA2000000		// HITACHI KOKUSAI DENKI KP-FR80PCL	(Bayer XGA  36fps)
	#define	__R_CS_CAM_TEMPORARY_MODE__	1
#endif


// 画像サイズ
#define VM_256_240		0
#define VM_320_240		1
#define VM_512_240		2
#define VM_640_240		3
#define VM_512_480		4
#define VM_640_480		5
#define VM_1024_768		6		// 2006.09.08
#define VM_1280_1024	7		// 2007.02.02
#define VM_1600_1200	8		// 2007.02.02
//#define VM			9		// CAM_CSB4000CL用
#define VM_2432_2048	10		// 2010.12.16
#define VM_2048_1088	11		// 2012.03.26 
#define VM_AOI			12		// 2012.11.28	kitamura

#define VM_2032_1080	13		// 2013.05.01	saito
#define VM_2048_2048	14		// 2013.11.03	oshima
#define	VM_XXX_YYY		15		// 2016.02.12	araki



// カメラタイプ
#define R_CS_CAMERA_GRAY	0		//<1.07b1.5>
#define R_CS_CAMERA_3CCD	1		//<1.07b1.5>
#define R_CS_CAMERA_BAYER	2		//<1.07b1.5>


#define RANDOM	STANDBY
#define CAPTURE_AUTO 0
#define CAPTURE_WAIT 1

//#define CAPTURE_MAX_NB 4		// 3→4に変更	2010.11.15 by sassa
#define CAPTURE_MAX_NB 10		// 4→10に変更	2016.4.15 by oshima

// メンバ変数へは原則的に直接アクセスしないこと
// 将来的に名称変更や隠蔽を行う可能性があります
typedef struct {
	int nInitialized;									//<1.07b2.8> 初期化フラグ追加  2010.05.06  by matsu
	int nBoardId;										// 基板ID
	int nChannel;										// カメラchannel		0..3
	int nName;											// カメラ名称			TI-124A/XCHR50/..
	int nSize;											// 取り込みサイズ		640x480/512x480/256x240/..
	int nMode;											// カメラモード			NORMAL/RANDOM
	int nSequence;										// 取り込みモード		CAPTURE_WAIT/CAPTURE_AUTO
	int nInitiator;										// イニシエータモード	ON/OFF
	int nPlane;											// 取り込みプレーン		GRAY_PLANE/BIN_PLANE
	int nBinLevel;										// 取り込み２値レベル	0..255
	int nCaptureFlag;									// GetVideo時のモード	F_SHUTTER|XCHR50|INITIATOR...
	int nShutter[CAPTURE_MAX_NB];						// シャッタースピード
	int nInitiatorPage[CAPTURE_MAX_NB];					// イニシエータページ	1..
	unsigned char *pInitiatorAddress[CAPTURE_MAX_NB];	//<1.07b1.4> イニシエータアドレス(nInitiatorPageと排他使用)		2008.06.21  by matsu
	int nGain;											// カメラゲイン(10倍値)									2007.09.28  by matsu
	int nType;											//<1.07b1.9> カメラタイプ	R_CS_CAMERA_GRAY / R_CS_CAMERA_COLOR / R_CS_CAMERA_BAYER
	int nInitMode;										// カメラ初期化モード	カメラ無しの場合：VI_BOARD_INIT_ONLY	2010.11.01 oshima
	int	nVideoInitShutterMode;							// R_video_init
	int	nVideoInitOtherMode;							// R_video_init
} R_CS_CAMERA;

// --- prototype (public)---

int R_CS_CameraInitial(R_CS_CAMERA *camera);
int R_CS_CameraChange(R_CS_CAMERA *camera);
int R_CS_CameraCapture(R_CS_CAMERA *camera, int capture_nb, int mode);
int R_CS_CameraCaptureWait(R_CS_CAMERA *camera);
int R_CS_CameraCaptureEndCheck(R_CS_CAMERA *camera);							// 画像取り込み完了終了確認  追加 2008.01.08  by matsu
int R_CS_CameraShutterWait(R_CS_CAMERA *camera);

int R_CS_CameraDisplayImage(R_CS_CAMERA *camera, int capture_nb);
int R_CS_CameraChangeDisplayId(void);

int R_CS_CameraGetColorMode(void);
int R_CS_CameraSetColorMode(int mode);

int R_CS_CameraGetSize(R_CS_CAMERA *pCamera, int *pSizeX, int *pSizeY);

int R_CS_CameraSetGain(R_CS_CAMERA *camera, int nGain);									// 追加  2007.09.28
int R_CS_CameraGetGainLimit(R_CS_CAMERA *camera, int *pGainMin, int *pGainMax);			// "

int R_CS_CameraSetInitiatorPage(R_CS_CAMERA *pCamera, int nCaptureNumber, int nPage);
int R_CS_CameraGetType(R_CS_CAMERA *pCamera);													//<1.07b1.9> カメラタイプの取得

#define R_CS_CameraSetShutterSpeed(pCamera, nCaptureNumber, nShutterSpeed)		((pCamera)->nShutter[(nCaptureNumber)] = nShutterSpeed)		//<1.07b1.4> 追加  2008.06.21
#define R_CS_CameraGetShutterSpeed(pCamera, nCaptureNumber)						((pCamera)->nShutter[(nCaptureNumber)])						//<1.07b1.4> 追加  2008.06.21
//#define R_CS_CameraSetInitiatorPage(pCamera, nCaptureNumber, nPage)				((pCamera)->nInitiatorPage[(nCaptureNumber)] = nPage)		//<1.07b1.4> 追加  2008.06.21
#define R_CS_CameraGetInitiatorPage(pCamera, nCaptureNumber)					((pCamera)->nInitiatorPage[(nCaptureNumber)])				//<1.07b1.4> 追加  2008.06.21
#define R_CS_CameraSetInitiatorAddress(pCamera, nCaptureNumber, pAddress)		((pCamera)->pInitiatorAddress[(nCaptureNumber)] = pAddress)		//<1.07b1.4> 追加  2008.06.21
#define R_CS_CameraGetInitiatorAddress(pCamera, nCaptureNumber)					((pCamera)->pInitiatorAddress[(nCaptureNumber)])				//<1.07b1.4> 追加  2008.06.21
#define R_CS_CameraSetCamlinkSioInitMode( val )									(SetCamlinkSioInitMode=val)

#endif
