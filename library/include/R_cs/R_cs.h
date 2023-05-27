/******************************************************************************

	R_CS.H

	共通ＶＧＡメニューシステム
	メインヘッダ

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	Version		Date		Name		Comment
				2004.07.11				オリジナル
	1.07		2008.01.07				バージョン表示方法新規追加
				2008.01.29				メニュー動作設定追加
				2008.02.08				通信BIT、コマンドをヘッダに移動
	1.07b1.4	2008.08.08				R_CS_SetImageAutoSaveAddress()追加
				2008.09.26				検査モードに INSPECT_PFILE 追加
				2008.10.03				R_CS_SetResultMonitorDispTotalNum()、R_CS_SetResultMonitorDispArea追加
	1.07b1.5	2008.12.9				R_CS_SetFunctionManualInspectFileAllReady(),
										R_CS_SetFunctionManualInspectFileAllClosing(),
										R_CS_SetFunctionCurrentCameraChange(),
										R_CS_SetPluralInspectionMode(),
										R_CS_SetPluralInspectionCount(),
										R_CS_GetPluralInspectionCount(),
										R_CS_SetFunctionThroughImageReady(),
										R_CS_SetFunctionThroughImageClosing()の追加
										全画像保存(ﾜｰｸ無し以外)対応
				2009.01.07				R_CS_VER_SOFTNAME(実行ファイル名表示)追加
				2009.05.08				R_CS_SetFunctionThroughUser(),
										R_CS_SetMenuColor(),R_CS_GetMenuColor(),
										R_CS_DispCounterSub(),
										R_CS_SetDirectlyInspectionName(),
										R_CS_SetDirectlyImageNameの追加
	1.07b1.6	2009.05.27	saito		通信方法（R_CS_SetCommunicationMode()で設定）の追加
	1.07b2.0	2009.07.27	oshima		R_CS_GetPfileResultMonitorNumber,R_CS_SetPfileResultMonitorNumberの追加
										R_CS_GetResultSaveMode,R_CS_SetCounterFileItemの追加
	1.07b2.2	2009.08.12	matsu		R_CS_SetMenuUserAnalysis追加
				2009.09.15	matsu		検査モード追加(マニュアル検査時)　INSPECT_CAPTURE、INSPECT_GMD、INSPECT_BMP
				2009.09.30	matsu		以下の関数追加
											R_CS_ChangeLogMode()
											R_CS_CheckChangeParameter()
											R_CS_SetChangeLogFileSystem()
											R_CS_SetChangeLogFilePackage()
											R_CS_SetChangeLogFileType()
	1.07b2.4	2009.11.05	oshima		画像解析前後ユーザ設定関数追加
											R_CS_SetFunctionAnalysisReady()
											R_CS_SetFunctionAnalysisClose()
	1.07b2.7	2010.02.19	matsu		R_winb.hをincludeに追加
	1.07b2.9	2010.06.04	oshima		R_CS_SetCommentBufferForInspectManual()の追加
				2010.08.26	oshima		R_CS_SetFunctionUserClearLast()の追加
				2010.09.03	matsu		R_CS_RESULT_DATAのコメントサイズ変更(R_pfileと同サイズ)
	1.07b3.7	2011.10.21	sassa		NG結果表示コメントを16->64文字に変更
	1.07b4.2	2012.05.16	oshima		R_CS_GetCounterDispMode()関数追加
	1.07b4.4	2012.08.23	araki		R_CS_SetFunctionUserCommandTransmit関数追加
	1.07b4.4	2012.08.28	araki		R_CS_AutoModeCommandRecheckDisable関数追加
	1.07b4.8	2013.04.09	araki		通信方法にR_CS_COMMUNICATION_COMMAND_DPRAMを追加
	1.07b5.3	2013.11.03	oshima		INSPECT_OTHER,INSPECT_CMDの追加
	1.07b6.1	2014.12.11	oshima		R_CS_GetResultMonitorHandle,R_CS_GetPfileResultの追加
	5.019		2019.10.30	kitamura	R_CS_Get系関数複数追加
******************************************************************************/
#ifndef __R_CS_H__
#define __R_CS_H__

#include "R_winb.h"			//<1.07b2.7>  2010.02.19  by mastu

#include "R_cs_cam.h"
#include "R_cs_res.h"
#include "R_cs_cnt.h"
#include "R_cs_io.h"		// @m
#include "R_data.h"
#include "R_cs_cap.h"		// 2007.09.11 @m
#include "R_cs_lan.h"

//#include "RCsInspect/RCsInspect.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- define ---
//グレープレーン番号
#define GPLANE_BASE1  4
#define GPLANE_BASE2  5
#define GPLANE_BASE3  6

// バージョン表示方法  新規追加 Ver.1.07 2008.01.07  by matsu
#define R_CS_VER_TIME1		0					// タイムスタンプ表示("[yyyy.mm.dd HH:MM:SS]")
#define R_CS_VER_TIME2		1					// タイムスタンプ表示２("VersionHH.MM")<デフォルト>
#define R_CS_VER_USER		2					// ユーザ指定
#define R_CS_VER_SOFTNAME	3					//<1.07b1.5> 実行ファイル名(拡張子を除く)


//通信方法		//1.07b1.6 2009.5.25 saito  10以降追加			//通信方法		ボード
#define R_CS_COMMUNICATION_CAPTURE_BOARD_IO				0x00	//BIT			CAPTURE_BOARD
#define R_CS_COMMUNICATION_COMMAND						0x01	//COMMAND		DLIO
#define R_CS_COMMUNICATION_USER							0x02	//ユーザー定義	ユーザー定義
#define R_CS_COMMUNICATION_LED_BOARD_IO					0x03	//BIT			GPIO

#define R_CS_COMMUNICATION_BIT_CAPTURE					0x10	//BIT			CAPTURE_BOARD	R_CS_COMMUNICATION_CAPTURE_BOARD_IO	と同じ
#define R_CS_COMMUNICATION_BIT_GPIO						0x11	//BIT			GPIO			R_CS_COMMUNICATION_LED_BOARD_IO		と同じ
#define R_CS_COMMUNICATION_BIT_DLIO						0x12	//BIT			DLIO			Reserved(未対応）
#define R_CS_COMMUNICATION_COMMAND_GPIO					0x13	//COMMAND		GPIO			Reserved(未対応）
#define R_CS_COMMUNICATION_COMMAND_DLIO					0x14	//COMMAND		DLIO			R_CS_COMMUNICATION_COMMAND			と同じ
#define R_CS_COMMUNICATION_COMMAND_FAST_DLIO_MASTER		0x15	//COMMAND_FAST	DLIO			R_dlio_com()使用　MASTER設定
#define R_CS_COMMUNICATION_COMMAND_FAST_DLIO_SLAVE		0x16	//COMMAND_FAST	DLIO			R_dlio_com()使用　SLAVE設定
#define R_CS_COMMUNICATION_COMMAND_DPRAM				0x17	//COMMAND		DPRAM			CP-CZ-47 DP-RAM通信



// LEDボード用InputBit割付(R_CS_COMMUNICATION_LED_BOARD_IO設定時)
#define	R_CS_INPUT_START			0x01
#define	R_CS_INPUT_COUNTER_CLEAR	0x02
#define	R_CS_INPUT_LOT_INITIAL		0x10

// コマンド通信時コマンド割付(R_CS_COMMUNICATION_COMMAND設定時)
#define R_CS_CMD_INSPECT_START	'S'
#define R_CS_CMD_COUNTER_CLEAR	'C'
#define R_CS_CMD_LOT_INITIAL	'L'
#define R_CS_CMD_RESULT_SEND	'R'


//カウンタ表示モード
#define COUNTER_DISP_TOTAL 0
#define COUNTER_DISP_ALL   1
#define COUNTER_DISP_NODISP 2

//結果表示モード
#define RES_DISP_NOTHING 0
#define RES_DISP_DETAIL  1
#define RES_DISP_GRAPH   2

//NGコード
#define NG_NO_READY			-1		//検査未実行
#define NG_NO_WORK			 1		//ワークなし
#define NG_INSP				99		//認識エラー
#define NG_CAPTURE			98		//画像取り込みエラー
#define NG_LIGHTING			97		//照明エラー

//カウンター最大数
#define COUNTER_MAX_NUMBER  8

//カメラ最大数
#define CAMERA_MAX_NUMBER   4

//検査モード(ビット定義)
#define INSPECT_AUTO			0x00000001		// オート稼動
#define INSPECT_MANUAL			0x00000002		// マニュアル稼動
#define INSPECT_STEP			0x00000004		// ステップ判定
#define INSPECT_PFILE			0x00000008		//<1.07b1.4> ファイル連続認識

//(検査画像がどこにあるかを示す)
#define INSPECT_TARGET_MASK		0x000F0000		//<1.07b2.2> 対象画像マスク
#define INSPECT_CAPTURE			0x00010000		//<1.07b2.2> 取り込み画像	(マニュアル認識時に設定)
#define INSPECT_GMD				0x00020000		//<1.07b2.2> GMD画像		(マニュアル認識時に設定)
#define INSPECT_BMP				0x00040000		//<1.07b2.2> BMP画像		(マニュアル認識時に設定)
#define INSPECT_OTHER			0x00080000		//<1.07b5.3> OTHERファイル	(マニュアル認識時に設定)
#define INSPECT_CMD				0x00100000		//<1.07b5.4> CMD画像		(マニュアル認識時に設定)


//デバッグモード
#define DEBUG_NO_COMMUNICATION	0x0001		// I/O通信なし
#define DEBUG_NO_VERSION_CHECK	0x1000		// バージョン整合性チェックをしない  2008.01.29


//画像セーブモード
#define IMAGE_SAVE_OFF					0
#define IMAGE_SAVE_ALL					1
#define IMAGE_SAVE_NG					2
#define IMAGE_SAVE_GO					3
#define IMAGE_SAVE_NG_WITHOUT_NOWORK	4
#define IMAGE_SAVE_ALL_WITHOUT_NOWORK	5	//全画像保存(ﾜｰｸ無し以外) 2008.12.9

//オートモード処理コード
#define R_CS_CODE_NO_SIGNAL			0
#define R_CS_CODE_INSPECT_START		1
#define R_CS_CODE_COUNTER_CLEAR		2
#define R_CS_CODE_LOT_INITIAL		3
#define R_CS_CODE_RESULT_SEND		4
#define R_CS_CODE_ELSE				255

//カウンタテーブル番号
#define RESULT_TOTAL_NUMBER 		6
#define RESULT_MANU_NUMBER  		7

//ユーザ設定メニュー数
#define R_CS_USER_MENU_NB   		8

//メニュー動作設定  2008.01.29
#define R_CS_MENU_NORMAL		0			// デフォルトモード
#define R_CS_MENU_ERROR_EXIT	1			// メニュー関数でエラー発生でR_CS終了
#define R_CS_MENU_EXIT_DISABLE	2			// メインメニューで終了表示をOFFにする

//4分割表示画像メモリ
#define R_CS_GPLANE_DIVIDE			3
#define R_CS_BINPLANE_DIVIDE		3

// 画面表示位置
#define R_CS_WINDOW_POSITION0		0		// 左上
#define R_CS_WINDOW_POSITION1		1		// 右上
#define R_CS_WINDOW_POSITION2		2		// 左下
#define R_CS_WINDOW_POSITION3		3		// 右下

//R_CS_AfterInspection用定義
#define NO_COUNT_UP_DETAIL			0x010
#define NO_COUNT_UP_TOTAL			0x020
#define NO_COUNT_UP_PRODUCT			0x040
#define NO_COUNT_UP_RESULT_MONITOR	0x080
#define NO_IMAGE_SAVE				0x100
#define NO_DISP_COUNTER				0x200
#define NO_DISP_GRAPH				0x400
#define NO_DISP_RESULT				0x800
#define	OVER_WRITE_IMAGE_FILE		0x1000

//オート・マニュアルモード切替時用定義
#define R_CS_CANNOT_AUTO_MODE_CLOSING	1
#define R_CS_CANNOT_AUTO_MODE_READY		1

//保存ディレクトリ追番設定用定義
#define R_CS_IMGDIR_ADDNO_NOADD		-1							//!< <1.07b9.6>	追番なし
#define R_CS_IMGDIR_ADDNO_DEFAULT	2							//!< <1.07b9.6>	デフォルトは追番2桁


typedef struct {
	int nCounterColumn;				//カウンタ表示列
	int nCounterLine;				//カウンタ表示行
	int nInfoColumn;				//システム情報表示列
	int nInfoLine;					//システム情報表示行
	int nJudgeColumn;				//判定結果表示列
	int nJudgeLine;					//判定結果表示行
	int nSizeColumn;				//最大表示列数
	int nSizeLine;					//最大表示行数
	int nLotInfoColumn;				//ロット情報表示列
	int nLotInfoLine;				//ロット情報表示行
	int nPfileInfoLine;				//PFILE情報表示行
	int nPfileInfoColumn;			//PFILE情報表示列
	int nLanInfoColumn;				//Lan情報表示列
	int nLanInfoLine;				//Lan情報表示行
} DRAW_ITEM;

#include "R_pfile.h"
typedef struct {
	int		nCode;										//結果フラグ(0がGO、1以上がNGコード)
	char	sNgComment[64];								//NG結果表示コメント		16->64文字に変更	2011.10.21 by sassa
	char	sImageComment[PFILE_SEQ_PATH_NAME_MAX];		//画像ファイルに埋め込むコメント
														//<1.07b2.9> コメントサイズ変更 128 -> PFILE_SEQ_PATH_NAME_MAX(=256)  2010.09.03  by matsu
}R_CS_RESULT_DATA;


// --- variable ---
//R_CS.Cの中に実体がある
//extern R_CS_CAMERA 	R_CS_CameraPara[];
//extern R_CS_CAMERA 	*R_CS_CameraPara;
extern R_CS_RESULT_MONITOR_ITEM *R_CS_ResultMonitorItem[];		//検査結果管理アイテム
extern R_CS_RESULT_DATA R_CS_Result[];							//検査結果データ
extern R_CS_COUNTER_DATA R_CS_Counter[];						//検査カウンタ
extern DRAW_ITEM R_CS_DrawItem;									//画面描画アイテム構造体
extern int R_CS_ResultDispMode;									//検査結果表示モード
extern int R_CS_ResultMonitorNumMax;							//検査結果管理データ保存最大数
extern int R_CS_FlagDispVersion;								//バージョン表示ON/OFFフラグ(ONならバージョンを、OFFならタイムスタンプを表示)
																// →R_CS_SetDispVersionMode()で設定すること @m 2007.09.04
extern int R_CS_FlagInitialTime;								//時間初期化ON/OFFフラグ(ONならソフト起動時に時間設定を行う)
																// →R_CS_InitialTime()で設定すること  2006.09.08
extern int R_CS_FlagInitialCamera;								//カメラ初期化ON/OFFフラグ(ONならソフト起動時に時間設定を行う)

extern R_DATAFILE_ITEM *_R_CS_User_ParaFileItemSys;				// 機台データファイル  内部変数化  2006.09.12
extern R_DATAFILE_ITEM *_R_CS_User_ParaFileItemPkg;				// 機種データファイル
extern R_DATAFILE_ITEM *_R_CS_User_ParaFileItemTyp;				// 機種データファイル（チップなど）2007.08.30 saito
extern char *_R_CS_User_ParaFileNameSys;						// 機台データファイル名
extern char *_R_CS_User_ParaFileNamePkg;						// 機種データファイル名
extern char *_R_CS_User_ParaFileNameTyp;						// 機種データファイル名（チップなど）2007.08.30 saito


//結果ファイル命名ルール
#define RESULT_FILENAME_TYPE_SERIAL	1
#define RESULT_FILENAME_TYPE_PLANE	0
int R_CS_GetResultFileNameType();								//!<	<1.07b9.6>	<br> 結果ファイル名の命名ルール	<br> 1のときはリザルトファイル名にロットNOを付加する <br> 0のときはユーザ設定名のまま保存する

//ユーザー関数に実体がある
//extern R_CS_COUNTER_CLASS R_CS_User_NgClass[];				//NGクラス
//extern R_CS_COUNTER_NG_CODE R_CS_User_NgCode[];				//NGコード
//extern R_DATAFILE_ITEM R_CS_User_ParaFileItemSys[];			//機台データファイル
//extern R_DATAFILE_ITEM R_CS_User_ParaFileItemPkg[];			//機種データファイル
//extern char R_CS_User_ParaFileNameSys[];						// 機台データファイル名
//extern char R_CS_User_ParaFileNamePkg[];						// 機種データファイル名
//extern char R_CS_User_ResultMonitorFileName[8][16];				//検査結果保存ファイル名

// --- prototype ---
int R_CS( int argc, char *argv[] );

int R_CS_ImageSave(int number);
int R_CS_ImageSavePluralInspection(int number, int total_number);
int R_CS_ImageSaveLastStatus(void);										//<1.07b3.6> 画像保存状態の取得

int R_CS_SetInspectionInfo(int camera_nb,int posi_nb,int inspect_nb,int capture_nb,int result_nb,int cycle_nb,char *name,char *p_name);
int R_CS_SetPackageName(char *pName);
int R_CS_SetTypeName(char *pName);

//int R_CS_Capture(int camera_nb, int capture_nb, int mode);			R_CS_CAP.Hに移動
//int R_CS_CaptureWait(int camera_nb);									R_CS_CAP.Hに移動
//int R_CS_CaptureMulti( int *camera_nb, int *capture_nb, int mode);	R_CS_CAP.Hに移動
//int R_CS_CaptureMultiWait(int *camera_nb);							R_CS_CAP.Hに移動

int R_CS_GrayMemoryDivideDisp( int src, int dst, int number );
int R_CS_SetInspectionName(int num, char* j_name, char* e_name);

int R_CS_SetShutterSpeed(int camera_nb,int capture_nb,int speed);
int R_CS_GetShutterSpeed(int camera_nb,int capture_nb);

void R_CS_CounterAllClear(void);
int  R_CS_SetImagePara(int camera_nb,int posi_nb,int capture_nb,int addresss,char *fname);
int  R_CS_SetImagePara_MultiInsp(int camera_nb, int posi_nb, int inspect_nb, int capture_nb, int address, char *fname);
int  R_CS_SetImageName(int camera_nb, int posi_nb, int capture_nb, char *name_j, char *name_e);				// 取込み画像名称の設定  2008.01.08  by matsu
int  R_CS_SetImageAutoSaveAddress(int camera_nb, int posi_nb, int capture_nb, unsigned char *pAddress);		//<1.07b1.4> 自動画像保存アドレスの設定
int  R_CS_GetCounterDispMode(void);
void R_CS_DispCounter(void);
//int  R_CS_CaptureCameraChange(int camera_nb);							R_CS_CAP.Hに移動
void R_CS_DispInspectTime(int t1, int t2, int t3, int t4 );
void R_CS_DispMenuUserSetting();
int  R_CS_SetMenuUserSetting(struct winb_item_t* item);
int  R_CS_SetMenuUserAnalysis(struct winb_item_t* item);					//<1.07b2.2> ユーザ画像解析メニューの設定
int  R_CS_SetMenuUserFile(struct winb_item_t* item);
void R_CS_DispTitle(int mode);
int  R_CS_AfterInspection(int number,int mode);
int  R_CS_AutoModeEndCheck(int code);
int  R_CS_GetCommunicationMode(void);
int  R_CS_SetCommunicationMode(int mode,int board_id);
void R_CS_SetVideoMemory(int gray_number,int bin_number);
void R_CS_DispResult(int number, int mode);
void R_CS_DispGraph(int number);
int  R_CS_BinMemoryDivideDisp( int src, int dst, int number );
int  R_CS_GetImageSaveMode(void);
int  R_CS_SetCameraPara(int camera_type, int video_size);

int R_CS_InformationMode(int mode);											// @m 2006.09.08
#define R_CS_InitialTime( val )		(R_CS_FlagInitialTime = val)			// @m 時間初期化するかを設定  2006.09.08
#define R_CS_InitialCamera( val )	(R_CS_FlagInitialCamera = val)			// カメラ初期化するかを設定  2008.12.9 oshima

int R_CS_SetParameterFileSystem(R_DATAFILE_ITEM *pList, char *pFileName);	// @m 2006.09.12
int R_CS_SetParameterFilePackage(R_DATAFILE_ITEM *pList, char *pFileName);	// @m 2006.09.12
int R_CS_SetParameterFileType(R_DATAFILE_ITEM *pList, char *pFileName);		// @sai 2007.08.22
int R_CS_SetLimitFileName(char *pFileName, int nNum);						// 2019.10.31 kitamura

int R_CS_SetToolConfigFileName(char *pFileName);							// @m 2006.09.13
//int R_CS_SetResultMonitorFileName(char **pFileNameList);					// @m 2006.09.13
int R_CS_SetResultMonitorItem(int nNum, char *sFileName, R_CS_RESULT_MONITOR_ITEM *pList);	// @m 2007.01.17
int R_CS_SetResultMonitorDispTotalNum(int nTotalNumber);					// @s 2008.10.03
int R_CS_SetResultMonitorDispArea(int nGraphNum, R_RECT tDispArea);			// @s 2008.10.03

int R_CS_GetParameterFileSystem(R_DATAFILE_ITEM **pList, char **pFileName);	//<1.07b2.8> 形式変更  2010.04.12  by matsu
int R_CS_GetParameterFilePackage(R_DATAFILE_ITEM **pList, char **pFileName);
int R_CS_GetParameterFileType(R_DATAFILE_ITEM **pList, char **pFileName);	// @sai 2007.08.30
char* R_CS_GetToolConfigFileName(void);										// 2007.11.16 形式変更

int R_CS_SetFunctionUserInitial(int (*pFunc)(void));						// @m 2006.11.16
int R_CS_SetFunctionUserInitialLast(int (*pFunc)(void));					// @m 2007.02.13
int R_CS_SetFunctionManualCapture(int (*pFunc)(void));						// @m 2006.09.23
int R_CS_SetFunctionManualInspectCapture(void (*pFunc)(void));				// @m "
int R_CS_SetFunctionManualInspectRepeat(void (*pFunc)(void));				// @m "
int R_CS_SetFunctionCaptureThrough(void (*pFunc)(int));						// @m "
int R_CS_SetFunctionUserClear(void (*pFunc)(void));							// @m "
int R_CS_SetFunctionUserClearLast(void (*pFunc)(void));						// <1.07b2.9> 2010.08.26 oshima
int R_CS_SetFunctionAutoModeReady(int (*pFunc)(void));						// @m 2006.11.16
int R_CS_SetFunctionAutoModeClosing(int (*pFunc)(void));					// @m "
int R_CS_SetFunctionAutoModeExecute(int (*pFunc)(int));						// @m "
int R_CS_SetFunctionUserCommunication(int (*pFunc)(void));					// @m "
int R_CS_SetFunctionUserCommunicationEndCheck(int (*pFunc)(void));			// @m "
int R_CS_SetFunctionInspect(int (*pFunc)(int mode,int number,R_CS_RESULT_DATA *Result));
int R_CS_SetFunctionUserCommandTransmit(int (*pFunc)(char*));
int R_CS_SetFunctionUserLanExecute(int (*pFunc)(int));

int R_CS_SetCounterNgCode(R_CS_COUNTER_NG_CODE *pNgCodeList);
int R_CS_SetCounterNgClass(R_CS_COUNTER_CLASS *pNgClassList);
R_CS_COUNTER_DATA *R_CS_GetCounterDataStruct(int nNumber);
int R_CS_SetFunctionDisplayCounter(int (*pFunc)(int));

int R_CS_SetDispJudgePosition(int nColumn, int nLine);						// @m 2006.10.26
int R_CS_SetDispCounterPosition(int nColumn, int nLine);					// @m 2006.11.07
int R_CS_SetDispLotInfoPosition(int nColumn, int nLine);					// @m 2006.11.30
int R_CS_SetDispInfoPosition(int nColumn, int nLine);						// @m 2006.10.07
int R_CS_SetDispPfilePosition(int nColumn, int nLine);						//<1.07b1.4>  2008.09.26

int R_CS_SetDispCounterCategoryMode(int nMode);								// @m 2006.11.07

int R_CS_SetInspectMode(int nMode);											// @m 2007.02.07
int R_CS_GetInspectMode(void);												// @m 2007.02.07
int R_CS_GetInspectManualMode(void);										//<1.07b2.2> マニュアル検査モードの取得
int R_CS_SetDebugMode(int nMode);											// @m 2007.02.07
int R_CS_GetDebugMode(void);												// @m 2007.02.07

int R_CS_SetDispVersionMode(int nMode);										// バージョン表示モードを設定  @m 2007.09.04
int R_CS_GetDispVersionMode(void);											// バージョン表示モードを取得  @m 2007.09.04
int R_CS_SetDispVersionString(char *pString);								// バージョン表示文字列の設定  2008.01.07  by matsu

void R_CS_CallInspectSequentialFileSetting(void);

int R_CS_SetDispDefaultZoom(int nZoom);
int R_CS_GetDispDefaultZoom(void);
int R_CS_SetDispThroughZoom(int nZoom);
int R_CS_GetDispThroughZoom(void);
int R_CS_SetCameraCheckMode(int nMode);
int R_CS_GetCameraCheckMode(void);

int R_CS_SetDisplayWindow(int nWindowNo, int nPosition, int nCameraNumber, int nZoomMode);
int R_CS_SetMenuMode(int nMode);
int R_CS_GetMenuMode(void);

int R_CS_GetCurrentCameraNumber(void);
int R_CS_SetCurrentCameraNumber(int nCameraNumber);				//<1.07b5.7>	2014/09/04	komatsu

int R_CS_GetCurrentInspectionNumber(void);
int R_CS_SetCurrentInspectionNumber(int nInspNumber);			//2017.04.17	komatsu

int R_CS_GetCurrentOverallNumber(void);							//2020.01.16	komatsu
int R_CS_SetCurrentOverallNumber(int nInspNumber);							//2020.01.16	komatsu

int R_CS_SetTimerCaptureThrough(int time);						// 追加2007.11.05  by matsu
int R_CS_GetTimerCaptureThrough(void);						// "

int R_CS_SetNetworkPcName(char *pPcName);						// 2007.11.16追加
int R_CS_SetNetworkDirName(char *pDirName);						// 2007.11.16追加
char* R_CS_GetNetworkPcName(void);								// 2007.11.16追加
char* R_CS_GetNetworkDirName(void);								// 2007.11.16追加

int R_CS_SetFunctionManualInspectFileAllReady(int (*pFunc)(int));		//2008.12.9
int R_CS_SetFunctionManualInspectFileAllClosing(int (*pFunc)(int));		//2008.12.9
int R_CS_SetFunctionCurrentCameraChange(int (*pFunc)(void));
int R_CS_SetPluralInspectionMode(int nMode);
void R_CS_SetPluralInspectionCount(int nCount);
int R_CS_GetPluralInspectionCount(void);
int R_CS_PositionChange(int nNum);
int R_CS_SetFunctionThroughImageReady(int (*pFunc)(void));
int R_CS_SetFunctionThroughImageClosing(int (*pFunc)(void));

int R_CS_SetFunctionThroughImageUser(int (*pFunc)(void));					//2009.5.8
int R_CS_SetMenuColor(int color);											//2009.5.8
int R_CS_GetMenuColor(void);												//2009.5.8
void R_CS_DispCounterSub(int nPosiX, int nPosiY, int nNumber);				//2009.5.8
int R_CS_SetDirectlyInspectionName(int num, char* j_name, char* e_name);	//2009.5.8
int R_CS_SetDirectlyImageName(int image_nb, int address, char *fname);		//2009.5.8
int R_CS_SetPfileResultMonitorNumber(int num);								//2009.7.27
int R_CS_GetPfileResultMonitorNumber(void);									//2009.7.27
int R_CS_GetResultSaveMode(void);											//2009.7.27
int R_CS_SetCounterFileItem(char *pStrName,char *pStrVal);					//2009.7.27

int R_CS_GetImgDirAddNoDigit(void);

int R_CS_ChangeLogMode(int nMode);											//<1.07b2.2> パラメータファイル変更履歴出力モード設定
int R_CS_CheckChangeParameter(void);										//<1.07b2.2> パラメータファイル変更チェック
int R_CS_SetChangeLogFileSystem(char *pFileName);							//<1.07b2.2> システムパラメータログファイルの設定(デフォルト:"log_sys.csv")
int R_CS_SetChangeLogFilePackage(char *pFileName);							//<1.07b2.2> 機種データ(パッケージデータ)ログファイルの設定(デフォルト:"log_pkg.csv")
int R_CS_SetChangeLogFileType(char *pFileName);								//<1.07b2.2> 機種データ(チップデータ)ログファイルの設定(デフォルト:"log_typ.csv")
int R_CS_SetChangeLogFileLim(char *pFileName, int nNum);						// 2019.12.4 kitamura limファイルログファイルの設定(デフォルト:"log_lim0.csv～log_lim3.csv")

int R_CS_SetFunctionAnalysisReady(int (*pFunc)(void));						//<1.07b2.4> 2009.11.5
int R_CS_SetFunctionAnalysisClose(int (*pFunc)(void));						//<1.07b2.4> 2009.11.5

int R_CS_SetCommentBufferForInspectManual(char *buff,int size);				//<1.07b2.9> 2010.06.04

int R_CS_WriteLog(char *format, ... );										//<1.07b3.5> ログデータファイル出力				2011.07.11
int R_CS_WriteLogMode(int mode);											//<1.07b3.5> ログデータファイル出力モード設定	2011.07.11

int R_CS_SetNgCodeMax(int number);											//<1.07b3.8> ＮＧコード最大番号の設定	2011.10.24
int R_CS_GetNgCodeMax(void);												//<1.07b3.8> ＮＧコード最大番号の取得	2011.10.24

void	R_CS_AutoModeCommandRecheckDisable(void);							//<1.07b4.4>

// システム関数
//int r_cs_change_zoom(int mode);

//ユーザー関数の中に実体がある
//void R_CS_User_ManualCaptureInspect(void);				@m 2006.09.23削除
//int R_CS_User_Communication(void);
//void R_CS_User_Clear(void);
//int R_CS_User_Inspect(int mode,int number,R_CS_RESULT_DATA *Result);
//int R_CS_User_AutoModeReady(void);
//int R_CS_User_AutoModeClosing(void);
//int R_CS_User_AutoModeExecute(int code);
//int R_CS_User_CommunicationEndCheck(void);
//void R_CS_User_ManualInspectRepeat(void);					@m 2006.09.23削除
//void R_CS_User_CaptureImage(int capture_num);				@m 2006.09.23削除
//int R_CS_User_Initial(void);

int r_CS_Check9301libVersion(int nMajor, int nMinor, int nBeta);		//<1.07b1.5>
int r_CS_isInformationMode(void);										//<1.07b2.7>  2010.02.20  by matsu

int r_CS_UserCommandTransmit(char* command);

int R_CS_GetPFileFileNubmer(void);										//<1.07b5.7>	2014/09/04	komatsu
int	R_CS_SetPFileFileType(int type);									//<1.07b8.3>	2016.05.23	araki

int R_CS_GetResultMonitorHandle(int number);							//<1.073.4>		2014.12.11	oshima
int R_CS_GetPfileResult(void);											//<1.073.4>		2014.12.11	oshima

int R_CS_SetParaInspectFileUser(struct winb_para_tm *user);
void R_CS_DelParaInspectFileUser();

int R_CS_Lan_SetLanInfo(const R_CS_LANINFO *info);						// ユーザプログラムからのロット通信設定強制書きかえ
R_CS_LAN_HANDLE R_CS_Lan_GetHandle();									// Lan通信Socektハンドルを取得

int R_CS_ProductSetLotNo(char *LotNo);

int R_CS_SetSoftVer(char *ver);

int R_CS_GetLotSerial(char *target);

int R_CS_SetImageSaveDirName(char *path);
int R_CS_SetImageLoadDirName(char *path);
int R_CS_SetImageGoChkDirName(char *path);
int R_CS_SetImageNGChkDirName(char *path);
int R_CS_SetImageManualDirName(char *path);
int R_CS_SetResultSaveDirName(char *path);

//ver5.019
const char* R_CS_GetImageSaveDirName(void);
char* R_CS_GetImageLoadDirName(void);
char* R_CS_GetImageGoChkDirName(void);
char* R_CS_GetImageNGChkDirName(void);
char* R_CS_GetImageManualDirName(void);
const char* R_CS_GetResultSaveDirName(void);
char* R_CS_GetImageSaveDirNameFull(void);
char* R_CS_GetResultSaveDirNameFull(void);

int	R_CS_ImageSaveDirName_Is_DateAndLotNumber(void);

void R_CS_GetDisplaySize(int *disp_x, int *disp_y);

//void R_CS_InspectSetHandle(R_CS_INSPECT_HANDLE handle, int cam_no);

#ifdef __cplusplus
}
#endif

#endif
