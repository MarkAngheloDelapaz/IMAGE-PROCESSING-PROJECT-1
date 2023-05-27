/******************************************************************************

	R_cs_cap.h

	履歴
	<version>	<date>		<name>		<comment>
	0.01		2007.09.10	matsu		新規作成(R_cs.cから分離＆カメラ設定関数追加)
	0.02		2007.09.27	sassa		以下の関数を追加(デバッグ中)
										R_CS_CaptureMultiN()
										R_CS_CaptureMultiWaitN()
										R_CS_CaptureDisplayImage()
										R_CS_CaptureChangeDisplayId()
				2007.09.28	matsu		以下の関数を追加
										R_CS_CaptureSetGain()
										R_CS_CaptureGetGainLimit()
	1.07b1.4	2008.06.21	matsu		以下の関数を追加
											R_CS_CaptureSetInitiatorAddress()
											R_CS_CaptureGetInitiatorAddress()
	1.07b1.5	2009.05.22	saito		KPFR31PCL(VGA RAW出力 COLOR CAMERA）対応
	1.07b1.9	2009.07.16	matsu		R_CS_CaptureGetCameraType()追加
	1.07b2.1	2009.08.10	araki		ファイル名R_CS_CAP.HをR_cs_cap.hに変更
				2009.08.10	sassa		R_CS_CaptureCameraChange()修正(ライブラリ3.50以降で
										CO-CZ-43複数使用時にスルー表示できない不具合修正)
				2010.01.07	sassa		R_CS_Captureの引数modeをR_CS_CameraCaptureに渡す際に、CAPTURE_WAIT以外の
										BITを全て渡すように修正(元はGV_NO_VRAMとGV_NO_WAITのみを渡していた)
	1.07b2.9	2010.11.02	oshima		R_CS_CaptureSetVideoInitMode(),R_CS_CaptureGetVideoInitMode()の追加
										

******************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

// --- define ---

extern R_CS_CAMERA 	*R_CS_CameraPara;

// --- prototype ---
int R_CS_CaptureOpen(void);
int R_CS_CaptureClose(void);

// 画像取り込み関数
int R_CS_Capture(int camera_nb, int capture_nb, int mode);											// 画面取り込み
int R_CS_CaptureWait(int camera_nb);																// 画面取り込み終了待ち
int R_CS_CaptureEndCheck(int camera_nb);															// 画像取り込み完了終了確認  追加 2008.01.08  by matsu
int R_CS_CaptureMultiN(int total_nb, int *camera_nb, int *capture_nb, int mode);					// 画面取り込み(カメラ複数台)
int R_CS_CaptureMultiWaitN(int total_nb, int *camera_nb);											// 画面取り込み終了待ち(カメラ複数台)
#define R_CS_CaptureMulti(camera_nb, capture_nb, mode)		R_CS_CaptureMultiN(2, camera_nb, capture_nb, mode)
#define R_CS_CaptureMultiWait(camera_nb)					R_CS_CaptureMultiWaitN(2, camera_nb)

int R_CS_CaptureCameraChange(int camera_nb);														// 画面取り込み画像切替
int R_CS_CaptureDisplayImage(int camera_nb, int capture_nb);										// 取り込んだ画像をモニターに表示
int R_CS_CaptureChangeDisplayId(void);																// 画像ボードのIDをモニターに出力している番号に切り替える

// カメラ基本設定関数
int R_CS_CaptureSetConfiguration(int nCameraNo, int nType, int nBoardId, int nChannel);				// 接続状況の設定
int R_CS_CaptureSetCondition(int nCameraNo, int nNum, int *pShutterSpeed, int *pInitiatorPage);		// 取り込み条件の設定

// カメラ拡張設定関数
int R_CS_CaptureSetSize(int nCameraNo, int nSize);													// 取り込みサイズの設定
int R_CS_CaptureSetMode(int nCameraNo, int nMode);													// カメラモードの設定
int R_CS_CaptureSetSequence(int nCameraNo, int nSequence);											// 取り込みモードの設定
int R_CS_CaptureSetInitiatorMode(int nCameraNo, int nInitiator);									// イニシエータモードの設定
int R_CS_CaptureSetPlane(int nCameraNo, int nPlane);												// 取り込みプレーンの設定
int R_CS_CaptureSetBinLevel(int nCameraNo, int nBinLevel);											// 取り込み２値レベルの設定

int R_CS_CaptureSetGain(int nCameraNo, int nGain);													// カメラゲインの設定  2007.09.28  by matsu
int R_CS_CaptureGetGainLimit(int nCameraNo, int *pGainMin, int *pGainMax);							// カメラゲインの取得  2007.09.28  by matsu

int R_CS_CaptureSetInitiatorAddress(int nCameraNo, int nCaptureNo, unsigned char *pAddress);		//<1.07b1.4> イニシエータ先アドレスの指定  2008.06.21  by matsu
unsigned char *R_CS_CaptureGetInitiatorAddress(int nCameraNo, int nCaptureNo);						//<1.07b1.4> イニシエータ先アドレスの取得  2008.06.21  by matsu

int R_CS_CaptureGetCameraType(int nCameraNo);														//<1.07b1.9>  カメラタイプの取得

int R_CS_CaptureSetVideoInitMode(int nCameraNo,int nMode);											//ボード初期化設定
int R_CS_CaptureGetVideoInitMode(int nCameraNo);													//ボード初期化設定の取得

int	R_CS_CaptureSetVideoInitShutterMode(int nCameraNo, int mode);
int	R_CS_CaptureGetVideoInitShutterMode(int nCameraNo);

int	R_CS_CaptureSetVideoInitOtherMode(int nCameraNo, int mode);

// 旧仕様
int R_CS_SetCameraPara(int camera_type,int video_size);						// カメラの設定(全カメラ同時)
int R_CS_CaptureSetCameraNumber(int nNum);									// カメラ台数の設定

#ifdef __cplusplus
}
#endif
