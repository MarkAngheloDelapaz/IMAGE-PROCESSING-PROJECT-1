/******************************************************************************

	検査 プログラム

    system name		チップ外観認識(MCR00x Index)
    program name	"TMCR00I.C"

											Authered. by	K.Matsunaga
											Image Processing Group
											Production System Development Division
											ROHM CO.,LTD.
											21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
											615-8585, JAPAN

	履歴
    history
	<version>		<date>			<name>			<comment>
	Ver. 1.00.0		2008-03-26		M.Takahashi		new
	Ver. 1.01.0		2008-05-08		M.Takahashi		オーバーコートの判定値を場所により変更する。
	Ver. 1.01.1		2008-05-16		M.Takahashi		画像保存変更
	Ver. 1.01.2		2008-05-20		M.Takahashi		Overcoat Y edge 判定変更
	Ver. 1.01.3		2008-06-12		M.Takahashi		チップ無し判定ミス対策

	Ver. 1.02.0		2008-06-16		M.Takahashi		CB side メッキ付着検査追加
	Ver. 1.02.1		2008-06-19		M.Takahashi		CB side メッキ付着検査　膨張、縮退処理を追加
	Ver. 1.02.2		2008-06-30		M.Takahashi		チップ無し時の電極検出レベル設定を変更
	Ver. 1.02.3		2008-06-30		M.takahashi		インデックステーブルの汚れによるインデックスガイド検出ミス対策 1
	Ver. 1.02.4		2008-07-02		M.takahashi		インデックステーブルの汚れによるインデックスガイド検出ミス対策 2
	Ver. 1.02.5		2008-07-02		M.takahashi		電極外周部分のＮＧ番号を修正
	Ver. 1.02.6		2008-07-04		M.takahashi		マニュアル検査時の数値をμｍでも表示
	Ver. 1.02.7		2008-07-28		M.Takahashi		バーコード設定の製品タイプデータを変更("04"->"40")
	Ver. 1.02.8		2008-08-05		M.Takahashi		インデックスガイド位置検出ミス対策

	Ver. 1.02.9		2009-09-10      A.Jeerasak      Revise Alarm print out
	Ver. 1.02.9		2009-09-10		M.Takahashi		インデックステーブルの汚れによるチップ有無判定ミス対策
													電極検出２値化レベル（IGuideEsLevel.nData）が
													インデックステーブルの濃度より低い値にならない様にした。
	Ver. 1.02.A     2010-02-08      A.Jeerasak      Reduce NG54 - NG55 MISS
	Ver. 1.02.A		2010-05-21		M.Tanaka		Change over from Dos to Linux
					2010-06-17      A.Jeerasak      Clear Data Alarm
    Ver. 1.02.A1    2010.07.17  	A.Jeerasak		Addition M/C Spec From 12 --> 13 [Support Sosei Lot]
    Ver. 1.02.B     2010.08.26      A.Jeerasak      Change Parameter Judge [Reduce NG11, 19, 21, 22, 54]
					2011.01.29      A.Jeerasak      Count2 Improvement
	Ver. 1.02.B1    2011.08.17      A.Jeerasak      NG Limit Check
         1.02.B2    2011.09.12      A.Jeerasak      OverCoat Sum Area Check
					2012.03.14      A.Jeerasak      Print Out Semi Lot NG count Cancel
	Ver. 1.03       2012.03.28      A.Jeerasak      Kuro Chip Insp Addition
					2012.04.19      A.Jeerasak      Cancle Pocket Don't Insp
					2012.06.07      A.Jeerasak      Date Time Adjustable
					2012.11.14      A.Jeerasak      No Work Signal
    Ver. 1.04       2012.12.13      A.Jeerasak      USB key check

    Ver. 2.00		2017.02.04      Tanaka          RIST MCR004 Ver1.04から派生（TWA6606F4に対応）
                    2017.01.10		Tanaka			SetElecEdge 電極エッジの端でダレているところを見ない
	Ver. 2.01		2017.02.25		Tanaka			標印左右ずれのリミットを導入
	Ver. 2.02		2017.02.26		Tanaka			add TWA_InspC2Xsize
													add TWA_Mezu_Area
													modify OverCoat Mask(mark)
	Ver. 2.02a      2017.02.27      Tanaka			bug fix  display NG99->NG02 (kaiki_keisu)
															C2Xmin parameter input prob
	Ver. 2.02b      2017.03.01      M.Tanaka        Bin_Ma_Bin_ini modify
													JPW mask modify
													OverCoat No Mark (no mask) hang fix
	Ver. 2.03       2017.04.11      M.Tanaka        ESR25 suport(BETA)
	Ver. 2.04       2017.04.12      M.Tanaka        add Lighting adjustment menu/Lighting monitor/USB key lock
	Ver. 2.04.1     2017.04.19      M.Tanaka        ESR25 overkill adjust
	Ver. 2.04.2     2017.04.24      M.Tanaka        ESR25 G2 Zure improvement
	Ver. 2.04.3     2017.05.02      M.Tanaka        ESR25 G2 Nijimi improvement
	Ver. 2.04.4     2017.05.08      M.Tanaka        MCR03 RIST barcode read fix
	Ver. 2.04.5     2017.05.09      M.Tanaka		ESR25 Mark '4' overkill fix
    Ver. 2.04.6     2017.05.19      M.Tanaka        RIST MCR03 Automotive output fix
                                                    MCR03 Mark debug print modify
    Ver. 2.04.7     2017.05.19      M.Tanaka        RIST MCR03 Automotive output fix
    Ver. 2.04.8     2017.05.23      M.Tanaka        RIST MCR03 Automotive output fix DR-CY-80 port
	Ver. 2.04.9     2017.05.29      M.Tanaka        RIST/REPI MCR03  Mark detection separate
                    2017.06.13		P.Pichet		New USB KeyCertification
					2017.08.07		P.Pichet		Improvement for support slip VCR.
													Addition menu set bright level of the monitoring
	Ver. 2.04.10    2017.06.05      M.Tanaka        RIST MCR03  Mark Bin level adjust
	Ver. 2.05		2017.05.17		M.Tanaka		MCR10 support base by T_MCR10 Ver 9.01
					2017.06.08      M.Tanaka        MCR03/ESR25/MCR10 Mark planeNo. bug fix
	Ver. 2.05.1     2017.06.16		M.Tanaka		add C1/C12 detection / barcord setting for (MCR18)
					2017.06.19		M.Tanaka		MCR10 G2_search_gray bug fix
	Ver. 2.05.2		2017.06.20		M.Tanaka		MCR10/18/ESR25 TWA_Mark_Level  clmin < Bin_Ma_l_ini[][0] bug fix
													ALL G2 edge search debug
					2017.06.21						MCR10/18 change G2 level sample position
													MCR10 change MORE x*_mb[],y*_mp[] store position
													MCR10 Bin_Ma_l_ini low value -10(50)
    Ver. 2.05.3     2017.06.27      M.Tanaka        MCR10 Mark Search range correction (hl_x,hl_y +2-> +4)
                                                          Mark segmentation slx2 3-> 4
                                                    MCR18 Mark TWA_R_n=4 use max fileter + char '4' judge adjust
	Ver. 2.05.4     2017.07.06      M.Tanaka        TWA_JudgeC2SizeX step_inspect debug print position change
					2017.07.10						MaskMarkingMCR03 angle bug fix
													ES_G2_xp 4 -> 8
	Ver. 2.06       2017.07.24      M.Tanaka		NWT-70UM CAMERA/IO/DRX124 support
													(Beta) MCR01/Lan Enable
	Ver. 2.06.1     2017.08.05		M.Tanaka		NWT-70UM I/O debug
	Ver. 2.06.2		2017.08.17		M.Tanaka		NWT-70UM Add Index search
	Ver. 2.07       2017.09.15      H.Mori			SFR25 support
	Ver. 2.08(2.07B)2017.10.25      M.Tanaka		Size check before execute R_gray2bin
													add receipt data output command
													add monitor terminal & ceramic bright level for nitto
	Ver. 2.09       2017.11.08      M.Tanaka        LTR50L support
	Ver. 2.09.1		2018.01.04		Jhong			QR-code abnormal <0000>
    Ver. 2.09.2     2018.01.15      M.Tanaka        add Mark 'L' (m-ohm)
                                                    fix Warning
    Ver. 2.09.3     2018.01.26      M.Tanaka        bug Fix Mark 'R' abnormal
	Ver. 2.10       2017.11.18      H.Mori			SFR25 support
	Ver. 2.11       2017.11.20      H.Mori			SFR25&LTR50L  back support
	Ver. 2.12       2018.02.15      H.Mori			(Ver. 2.09.1 & Ver. 2.09.2 & Ver. 2.09.3) & Ver. 2.11 are merged.
	Ver. 2.20       2018.03.01      H.Mori			LTR18L  surface support
	Ver. 2.21		2021.02.11		Zedrick			Marking Accuracy up
	Ver. 2.22		2021.04.20		T.Shiotsuka		Glass IBUTSU Judgement
	Ver. 2.23		2021.06.09		T.shiotsuka		"6","8","9" LTR18.c add Logic TWA_Mark_error();
													IN-CY-76 119 common program
													Start option /n119= LRN02(IN-CY-76)
													Start option /n76= LRN04(IN-CY-119)
	Ver.2.24		2021.10.15		T.shiotsuka		Marking segmentation ys ;5 -> 10  xs ;15 -> 65
													KURO Electrode area 2 area -> 4 area LTR18.C
													Y position adjustment chip angle InspBinBl( void );
													G2 edge search up/low length LTR18.C
													G2 edge search area inside G2 egde LTR18.C
******************************************************************************/

#define elec_offset_inner  15
#define elec_offset_outer  5

/*
  --- memo ---
  本プログラムは以下のハードウェア仕様で動作します
	TWA6606F4
	画像ボード	: CO-CZ-43
    カメラ		: KP-F31PCL
	I/Oボード	: DR-CY-80A
	LEDボード	：任意(DSW:1,使用CH:1)
	NWT-70UM
	画像ボード	: CO-CZ-43
	カメラ		: acA2000-340km
	I/Oボード	: IN-CY-122 (IN 16bit/OUT 16bit)
	LEDボード	：DR-X-147A (DSW:opt設定)
*/


// --- include header file ---

// standard library
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <math.h>			// Math functions
#ifdef __DOS__
	#include <conio.h>
	#include <sys\stat.h>
	#include <fcntl.h>
	#include <io.h>
	#include <dos.h>
#elif __GNUC__
	#include "R_conio.h"
	#include <sys/stat.h>
	#include <time.h>
#endif
#include <termio.h>
#include <unistd.h>

// TSC-9301 library
#include "R_stdio.h"
#include "R_stdlib.h"
#include "R_video.h"
#include "R_graph.h"
#include "R_dgraph.h"
#include "R_menu.h"
#include "R_winb.h"
#include "R_gpio.h"
#include "R_time.h"
#include "R_dmenu.h"
#include "R_filter.h"
#include "R_bgray.h"
#include "R_edge.h"
#include "R_meas.h"			// ２値画像処理ライブラリ用ヘッダ
#include "R_robust.h"
#include "R_cursor.h"
#include "R_gray.h"
#include "R_search.h"
#include "R_bitmap.h"
#include "R_incy56.h"		// ＩＮーＣＹー５６（差動ドライバーインターフェース）ライブラリー
#include "R_camlnk.h"
#include "R_drx124.h"
#include "R_system.h"

#include "R_binrec.h"
#include "R_trans.h"		// Ｒ＿ｔｒａｎｓ関数を使用する
#include "R_math.h"


// common library
#include "R_cs.h"
#include "R_cdraw.h"
#include "R_led.h"
//#include "R_cs_cmd.h"
#include "R_cs_fil.h"
#include "R_cs_sav.h"
#include "R_cs_lan.h"
#include "R_timeif.h"		//2012.12.04 saito


// application header
#include "tp_sub.h"
#include "tmcr00i.h"
#include "SpecSet.h"
#include "Setting.h"
#include "gpioinfo.h"
#include "mcr03.h"
#include "mcr10.h"
#include "mcr18.h"
#include "esr25.h"
#include "ltr50.h"
#include "ltr18.h"																	//v2.20

//USB Key (REPI) //<9499>
#include "authenticate.c"
// --- define ---
#define NORMAL			0
#define ALL_GO			1
#define ALL_NG_NOWORK	2
#define ALL_NG_REVERSE	3
#define ALL_NG			4


// 処理時間管理
static RTIME RTProcessTime, RTWaitTime;
static int ProcessTime;

#define ProcessTimerStart()		ProcessTime = 0; R_time_start(&RTProcessTime);
#define ProcessTimerPause()		ProcessTime += R_time_read(&RTProcessTime)
#define ProcessTimerResume()	R_time_start(&RTProcessTime)
#define ProcessTimerStop()		ProcessTime += R_time_read(&RTProcessTime)
#define ProcessTimerGet()		ProcessTime
#define ProcessTimerDisplay()	(R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 1, Disp_line-1, "time:%.3fms  ", ProcessTime/1000.0))

TWA_TIME_RESULT twaTime[2];

// --- prototype ---
int		LTR_SearchApYpos( int grayPn, int binPn );



static int	Dummy(int nMenuNumber){	return OK;	}
static int	InitialUserOnR_CS(void);
static int	InitialUserOnR_CSLast(void);
static int	AutoModeReady(void);
static int	AutoModeClosing(void);
static int	AutoModeExecute(int code);
static int  AutoModeExecuteNwt( int code );
static int	InspectManual(int mode, int number, R_CS_RESULT_DATA *Result);
static int	ParameterInspSet(int nMenuNumber);			//
static int	JudgeValueDisp(int nMenuNumber);			// 判定値表示
static int	ParameterSysSet(int nMenuNumber);
static int	LedAdjust(int nMenuNumber);				// 照明調整
static int LedLevel(int nMenuNumber);
static void	CaptureLedAdjust(int mode);				// 照明調整用画像取り込み
static int	CaptureManual(void);					// マニュアル検査用画像取り込み
static void	CaptureThrough(int capture_nb);			// スルー表示用画像取り込み
static int	DispFailImage(int nMenuNumber);				// Fail 画像表示
static int	Print2(int nMenuNumber);				    // Print test
static int	NGLimitCHK(int nMenuNumber);				// NG Limit On/Off 2011.08.17
static int	MonitorCHK(int nMenuNumber);				// 2017.08.07
static int	SetDateTime(int nMenuNumber);				// DateTime
static int	GetOption( char *opt );
static void Ussage(void);
static int	Initial(void);
static int	InitialBoard( void );
static int nDispMode = 0; //9499
int		InitDRCY80( void );
int		InitINCY122( void );
int		InitINCY76( void );
int		InitINCY119( void ); //<z1>
void    ProgExit(void);
int		CheckParameterFile( void );
int		data_read( void );
int		data_read2( void );
int		NGLimitDataLoadfile( void );
static int	InitialInspectionData(void);
void	SetMcacineNo( void );
static int	InspectStart(int mode);
static int	InspectStartNwt(int mode);
void	TP_CheckAlarm( void );
int		OutputJudge( void );
int		OutputJudgeNwt( void );
int 	NG_Limit_Check( void );
int		BinLevelSet( THRESHOLD_PARAMETER *pThresh );
int		TP_SearchElecXrough( int binPn );
int		TP_SearchElecYrough( int binPn );
int		TP_SearchElecXroughLTR( int binPn );
int		TP_SearchElecYroughLTR( int binPn );
int		ChipEsLevelSet( void );
void	ChipGray2Bin( void );
void	SetElecEdge( int elec );
int		CheckElecEdge( int elec );
void	ElecCornerPosSet1st( int elec );
void	ElecC12CornerPosSet1st( int elec );
void	ElecC1CornerPosSet1st( int elec );
int		InspBinBl( void );
int		Regression( int elec, int side );
int		ChipBinMeasure( void );
void	ElecCornerPosSet( void );
int		ElecInspLevelSet( void );
int		InspElecSize( void );
void	InspBinSetElectrode( void );
int		JudgeElecEdge( int elec, int side );
int		BlbinSet( int xs,int ys,int xe,int ye,int grayPn );
int		InspElecEdge( void );
int		OvercoatInspLevelSet( void );
void	InspBinSetOvercoat( void );
int		PlatingInspLevelSet( void );
void	InspBinSetPlating( void );
int     ManualInspectFileAllReady(int nMenuNumber);
int		BinLevelSet_WithMask( THRESHOLD_PARAMETER *pThresh ,R_RECT *aMask);
int		GetBinG2Rough(int GrayPn);
void	InspBinSetOvercoatBlack( void );			//v2.11
int		OvercoatInspLevelSetBlack( void );			//v2.11

static int	ExchangeUnit( INSPECT_PARAMETER *pSrcPara, INSPECT_PARAMETER *pDstPara );
int	TWA_LED_set( int page, int level ,int Judge_LED_White, R_LED_PARA *pLedPara);


//ＩＯチェック
static int MenuIOCheck( int nMenuNumber ){	R_CS_IOCheck();		return OK;	};
//static int	Counter_cls(int nMenuNumber);				// Counter Clear 2012.03.14


//
static void PkgParaSaveLog( void );							//パラメータ変更ログ		ver3.02 saito

void	TWA_Ibutsu_error( void );							//Glass IBUTSU error Ver2.21<shio>
int	Index_Ibutsu_check( int grayPn, int binPn );			//Glass IBUTSU check Ver2.21<shio>
void	TWA_output_Stop( int mode, int io );				//Glass IBUTSU alarm Ver2.21<shio>

//int	SearchApp_Glass(void);
//int		G2App_Glass(void);


// --- variable ---
int ManualMode	= OFF;
int StepMode	= OFF;
int DebugMode	= OFF;
int LogMode		= OFF;
int ImageAutoSaveAfterInspection	= OFF;
//static int CommandStatus = OFF;
static int ResultCode;

FILE *LogFile;

MACHINE_PARAMETER	SystemPara;

INSPECT_PARAMETER	InspPara, InspParaM;

//その他の変数
//static int				InspValue			= 0;							// 検査 抵抗値

extern int				Std_rpm;											// 設備基準回転数(rmp)

// --- Inspection Inage(Gray)
int						GetGrayPn			= (int)NULL;					// Get capture		gray memory
int						InspGrayPn			= (int)NULL;					// Inspection		gray memory
int						backupInspGrayPn	= (int)NULL;					// Inspection		gray memory
int						InspGray3Pn			= (int)NULL;					// Inspection		gray memory
int						InspGray4Pn			= (int)NULL;					// Inspection		gray memory

int						InspApGrayPn		= (int)NULL;					// Inspection		gray memory
int						InspOcGrayPn		= (int)NULL;					// Inspection		gray memory
// --- Inspection Inage(Bin)
int						InspBinPn			= (int)NULL;					// Inspection		bin memory
int						InspApBinPn			= (int)NULL;					// Inspection		bin memory
int						InspOcBinPn			= (int)NULL;					// Inspection		bin memory

// --- Chip size ---
static int				ChipSize[4]			= { 400, 200, 0, 0 };
// --- Chip corner size ---
//static	int				ChipCornerSize[4]	= { 10, 10, 0, 0 };
static	int				ChipCornerSize[4]	= { 30, 15, 0, 0 };

// --- Bin level set ---
THRESHOLD_PARAMETER		IGuideEsPara, ChipEsLSetPara, G2EsLSetPara,InspElecLSetPara, InspOcLSetPara, InspPlLSetPara;
// --- Gray to Bin level ---
//v2.11	BIN_LEVEL_PARAMETER		IGuideEsLevel, ChipEsLevel, G2EsLevel, ElecInspLevel, OcoatInspLevel, PlatingInspLevel, IndexLevel;
BIN_LEVEL_PARAMETER		IGuideEsLevel, ChipEsLevel, G2EsLevel, ElecInspLevel, OcoatInspLevel, PlatingInspLevel, IndexLevel, OcoatInspLevelBlack;
extern int				Bin_Mz_l[5];									//C1 mezumari Binary level			//<z3>

// --- Edge seach ---
EDGE_SEARCH_PARAMETER	IndexEsPara;


// --- ２値計測条件（ 特徴量演算　演算方式指定 ） ---
BIN_MEASURE_PARAMETER	BinMeasChip,		// Chip search
						BinMeasOcoat,		// Overcoat Insp.
						BinMeasElec,		// Elec Insp.
						BinMeasPlating;		// Plating Insp.

// --- Chip position data
        int				ChipBlobNo			= 0;			// Chip blob No.( bin measure )
struct	ChipCornerPosition {												// 外形角位置データ
	int		dx;
	int		dy;
}		ChipCorPos[4][4];

// Electrode and Overcoat data
Overcoat_data	Elec[3], Overcoat,ElecC12[2],ElecC1[2];

// --- Chip 傾き ---
    int         		ChipAtan			= 0;				// Chip 角度×１０の値（アークタンジェント）

// Overcoat insp.
// --- Chip size ---
static int				InspOcOffsetSize[4]	= { 5, 3, 0, 0 };				// Overcoat insp. area offset size
static int				InspOcOffsetCor[4]	= { 40, 30, 0, 0 };				// Overcoat insp. area offset corner size
static int				InspOcOutside[4][8];								// Overcoat insp. 検査除外範囲
static int				InspOcOutsideCor[4][8];								// Overcoat insp. 検査除外範囲
static int				InspOcOffsetSizeBlack[4]	= { 5, 3, 0, 0 };				// Overcoat insp. area offset size	v2.11
static int				InspOcOffsetCorBlack[4]	= { 40, 30, 0, 0 };				// Overcoat insp. area offset corner size v2.11
// Plating insp.
//	--- CB side ---
static int				InspPlOffsetSize[4]	= { 0, 5, 0, 0 };				// Plating insp. area offset size
static int				InspPlatingArea[2][4];
static int				InspPlatingPos[2][8];

// 回帰係数・平均値・標準偏差
Regression_def  ElecEdge[3][8];	// ChipEdge[4], ChipXside, ChipYside,

// 外形エッジデータ最頻値
//struct	frequency_rate {
//	int		dx;
//	int		dy;
//	int		dn;
//}		 ElecFreqRate[3][4][700],ElecFreqMax[3][4];			  2017.1.10 tanaka Rate 使われていないのでコメント化

// --- ロバスト推定条件構造体
ROBUST_CONDITION		RobustCond			=	{
													-50,					// deg_min;				最小角度 10倍値[deg]
													50,						// deg_max;				最大角度 10倍値[deg]
													5,						// deg_step;			角度ステップ 10倍値[deg]
													0,						// frequency_rate_min;	座標点数に対する最頻値の度数の最低割合(0-100%)
													0,						// moving_average;		ヒストグラム移動平均区間
													ROBUST_MODE				// method;				ヒストグラム評価条件
												};		//-5～5[deg]の範囲を、1[deg]刻みでチェック

R_POINT					*Src_point			= NULL;
R_POINT					*Src_point_adr;
R_POINT					*Dst_point			= NULL;
R_POINT					*Dst_point_adr;
R_LINE					ApproxLine;


int			RobustDegWidth	= 100;
int			RobustDegStep	= 5;


static int nJudgeMode = 0;		//AllGo,AllNG 用フラグ

// 基板ID(DSW番号)
// マルチプロセスで起動するので、ID番号をかえる必要がある
// →起動オプションで基板IDを設定するので、ソース内では全て_BOARD_RDSW_0として記述する
// →PCI2724CでもDrCy83ChをID番号として使用する
#define DRAW_POSI_IO_STATUS_X			60					// 通信状態　表示位置X
#define DRAW_POSI_IO_STATUS_Y			30					// 通信状態　表示位置Y

static int DrCy83Ch = _BOARD_RDSW_0;
static int DrX124Ch = _BOARD_JPSW0;
static int HandleDrx124 = 0;			// DR-X-124ハンドル
static int GpioInPort = -1;
static int GpioOutPort = -1;
static int CommunicationDeviceShareId = 0;
static int CommunicationDeviceStartBit = -1;
static int CommunicationDeviceUseLen = 0;
static R_POINT DrawPosi_IoStatus = {DRAW_POSI_IO_STATUS_X, DRAW_POSI_IO_STATUS_Y};
static int UserTimeDelay(int nTimer);

// 日東工業テーピング対応  2017.07.11 tanaka
#define NWT_INSP_START		0x8000				// Insp. start

static int SetBit(int nBit, int nFlag);
static void OutputBusyOffNwt(void);
static void OutputBusyOnNwt(void);
static void OutputLockOffNwt(void);
static void OutputLockOnNwt(void);
static void OutputJudgeGoNwt(void);
static void OutputJudgeNgNwt(void);
static void OutputBinReverseNwt(void);
static void OutputBinSystemNwt(void);
static void OutputBinNormalNwt(void);
static void OutputNoworkOnNwt(void);
static void OutputNoworkOffNwt(void);
static void OutputNoworkOn(void);			//<shio>
static void OutputNoworkOff(void);			//<shio>
static long long int InputByte(void);
static long long int GetStartBit(long long int nData);
static long long int GetColletNumber(long long int nData);
static long long int GetWorkState(long long int nData);
static long long int GetCounterClear(long long int nData);
static int GetOutputStatus(void);
static int UserTimeDelay(int nTimer);
static int user_getch(void);
static int user_kbhit(void);
static int CheckChip = 0;
static int Result_CheckChip[2] = {0,0};
static int NGStart  = 0;
static int Result_NGStart[2]   = {0,0};
static int PocketNo = 0;
static int Result_PocketNo[2]  = {-1,-1};
#define		MAX_LIGHT_MONI  10
static int El_aveL[MAX_LIGHT_MONI],El_aveR[MAX_LIGHT_MONI],G2_ave[MAX_LIGHT_MONI];
static int light_monitor_count = 0;
static int TP_Bright_LowerLimit[3] = {0,0,0};

//LED調整
#define LED_CH_MAX	6										// 照明基板ch数
//#define DRCY80CH	_BOARD_RDSW_1							// 照明基板DSW番号
#define GPDAC_DSW		_BOARD_RDSW_0						// 照明基板DSW番号
#define GPDAC_DSW_Val	0									// 照明基板DSW番号
static int		LedCurrent[LED_CH_MAX];						// 照明電流 [uA]
static int		LedCaptureNo =0;							// 照明調整時の取り込み番号
static int		SampleAreaNum;
static R_RECT	SampleArea[8];

static R_LED_ADJUST LedAdjustPara = {"照明調整", "Light Setting", CaptureLedAdjust, NULL,
	{
		{ "斜光照明"		,"Slant   Light"	,LED_CH1|LED_CH2|LED_CH3|LED_CH4,	(int *)&LedCurrent[0],	0,	100000,	1000	},
//		{ "同軸照明"		,"Coaxial Light"	,LED_CH4,							(int *)&LedCurrent[1],	0,	100000,	1000	},
		{	NULL	},
	}
};


static R_LED_PARA LedPara = {
	GPDAC_DSW,
	{
		{	0		,&LedCurrent[0]	},
		{	1		,&LedCurrent[0]	},
		{	2		,&LedCurrent[0]	},
		{	3		,&LedCurrent[0]	},
//		{	3		,&LedCurrent[1]	},
		{	-1		,NULL			},
	}
};

#define USER_DRX124_PATTERN				8					// DR-X-124パターン数
#define USER_DRX124_CH_NUMBER			6
#define LED_CONTROLL_DR_CY_80			80
#define LED_CONTROLL_DRX124				124
#define BRIGHTNESS_CHECK_AREA_NUMBER	4
int			LedControllMode = LED_CONTROLL_DR_CY_80;
// DR-X-124パターン毎の設定　1枚だけを使用する場合のみ(LED_CONTROLL_DR_X_124_SINGLE)、この構造体を使用する
int			nLedBoardType;										// 使用する照明ボードの種類
R_RECT		rectLedCheckArea[4];								// MenuLightSet_DrX124_Singleで使用

typedef struct {
	int nPatternNumber;							// 点灯パターン数
	int nCameraNo[USER_DRX124_PATTERN];			// 各パターンのカメラ番号
	int nCaptureNo[USER_DRX124_PATTERN];		// 各パターンの取り込み番号
} DRX124_SETTING;
static DRX124_SETTING *_Drx124_Setting = NULL;
static DRX124_SETTING Drx124_Setting_Cam1_Cap1 = {	1,	{0},		{0}			};	// カメラ1台　取り込み1回

static void CaptureLedAdjustCap0(int nMode);
static void CaptureLedAdjustEndCap0(int nMode);
static int MenuLightSet_DrX124_Single(void);
static void GetRect(R_RECT *area,  int xmax, int ymax, int xmin, int ymin );
static int MenuLightSet(int nMenuNumber);
static int DrX124ConditionSet(void);
static int DrX124ResetTurn(void);

// ロット情報
// ロットデータ構造体
typedef struct {
	char		sLotNumber[64];				// ロット番号
	char		sPackageName[64];			// 機種名
	char		sMarkCharacter[64];			// 標印文字
	int			nMarkNumber[4];				// 標印文字数
	int			nMarkLines;					// 標印行数
	char		sTapingDirName[64];			// テーピング方向
} LOT_DATA;

static LOT_DATA LotData = {		// ロットデータ構造体
	"-",			// ロット番号
	"-",			// 機種名
	"-",			// 標印文字
	{0,},			// 標印文字数
	0,				// 標印行数				2013/05/21 by sassa
	"-"				// テーピング方向
};
static int TypeChange_ByQrCode(char *sQrCode, LOT_DATA *pLotData);
static int AutoTypeChangeLanUser(int command);
static char QrCode[512];		// QRコード文字列



// ＬＥＤ照明
static	int				TP_BrightFlag		= 0;							//LED照明照度監視 (0:なし 1:あり 異常連続n)
static	int				TP_Bright_st		= 0;							//LED照明照度監視 基準レベル
static	int				TP_Bright_d			= 0;							//LED照明照度監視 レベル
static	int				TP_Bright_xs		= 640-50;						//LED照明照度監視 Area xs
static	int				TP_Bright_ys		= 240-100;						//LED照明照度監視 Area ys
static	int				TP_Bright_xl		= 20;							//LED照明照度監視 Area xl
static	int				TP_Bright_yl		= 200;							//LED照明照度監視 Area yl
static	int				TP_Bright_Stop		= OFF;							//LED照明異常　機台停止
static	int				TP_Bright_n			= 0;							//LED照明異常　連続n

static	int				TP_Bright_SetLevel  = 100;
static	int				TP_Bright_SetPercent= 95;

static  int             NG_Limit_OnOff[100];                                 //2011.08.17


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////             MARKING INSPECTION MCR03                      /////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

extern int             G2_pos[4];											//Ｇ２位置検出データ( lex, rix, upy, loy )
extern int             Ap_pos[6];											//外形位置検出データ( lex, rix, leuy, lely, riuy, rily )

        int				Index_pos[6];										//インデックステーブル　エッジ位置( uy, dy, ly )

extern KAIKI		ue, shita, migi, hidari, w, l, Ap_up, Ap_lo, Ap_ri, Ap_le, Ap_w, Ap_l;
extern MEANSD		up_y, lo_y, le_x, ri_x, Ap_up_y, Ap_lo_y, Ap_le_x, Ap_ri_x;




//        int				TWA_Judge_Ma		= ERROR;						//判定結果
        int				Ma_pos[4];											//標印位置検出データ( lex, rix, upy, loy )

//標印
            int				TWA_R_n				= 0;							//Resistance value character number F/J



//static	unsigned int			TWA_test;											//検査テストモード（0:通常　1:Ｇ２検査　2:外形検査）

#define	Mark_DEBUG			1;
static	int				Nijimi		= ERROR;
int			 		TWA_X_size			= 640;
int			 		TWA_Y_size			= 480;

//フィルター
static	int				G2_contrct_x		= 3;							//Ｇ２縮退Ｘサイズ
static	int				G2_contrct_y		= 3;							//Ｇ２縮退Ｙサイズ
static	int				G2_expand_x			= 3;							//Ｇ２膨張Ｘサイズ
static	int				G2_expand_y			= 3;							//Ｇ２膨張Ｙサイズ

static	int				Ap_contrct_x		= 3;							//外形縮退Ｘサイズ
static	int				Ap_contrct_y		= 3;							//外形縮退Ｙサイズ
static	int				Ap_expand_x			= 3;							//外形膨張Ｘサイズ
static	int				Ap_expand_y			= 3;							//外形膨張Ｙサイズ

int             	IGuidePos[2]		= { 0, 0 };						// Index guide position
int                 ElecPosRough[3][4];									// 電極位置検出データ[side][pos]

int						bIsNitto =0;										 // 日東機動作
int						bIsMCR01 =0;										 // MCR01検査
int                     bIsMCR03 =0;                                         // MCR03検査
int                     bIsMCR10 =0;                                         // MCR10検査
int                     bIsMCR18 =0;                                         // MCR18検査
int                     bIsESR25 =0;                                         // ESR25検査
int                     bIsSFR25 =0;                                         // SFR25検査
int						bIsLTR50 =0;										 // LTR50検査
int						bIsLTR18 =0;										 // LTR18検査										//v2.20
int						bIsG2Straight = 0;									 // G2樹脂品（外形と接触するもの）
int						bIsG2Separate = 1;									 // G2ガラス品と樹脂で個別品
int g2level;

static	int				TWA_Ibutsu_Stop		= OFF;							// Index IBUTSU　機台停止

/*
#define RESULT_MONITOR_FILE_NAME	"result.csv"

static R_CS_RESULT_MONITOR_ITEM ResultMonitorList[] = {
	// 検査情報
	{	&ResultCode							,NULL,NULL,NULL,NULL,NULL,-1	,"NgCode"		,"NgCode"		,0,0,0	},		// NG code
	{	&SaveImageCount						,NULL,NULL,NULL,NULL,NULL,-1	,"SaveImageNo"	,"SaveImageNo"	,0,0,0	},

	{	&MoldSurfaceResult.tWorkCenter.x	,NULL,NULL,NULL,NULL,NULL,-1	,"CenterX[um]"	,"CenterX[um]"	,0,0,0	},		// ワークセンター位置Ｘ
	{	&MoldSurfaceResult.tWorkCenter.y	,NULL,NULL,NULL,NULL,NULL,-1	,"CenterY[um]"	,"CenterY[um]"	,0,0,0	},		// ワークセンター位置Ｙ

	{	NULL,	},
};
*/

// --- bilingual menu struct ---

static struct winb_para_menu_t WinbParaOnOff[] = {
	{	"ＯＦＦ"	,"   OFF",	},				// 0
	{	"  ＯＮ"	,"    ON",	},				// 1
};

int				WorkParaDummy;
static struct	winb_para_menu_t WorkParaCommentDummy[] = {
	{	"未使用",	"NO USE",	},
};

static struct	winb_para_menu_t WinbParaMeasMode[] = {
	{	"通常",					"Normal",					},			// 0
	{	"微小ラン無視",			"Minute run disregard",		},			// 1
	{	"微小面積無視",			"Minute area disregard",	},			// 2
};

static struct	winb_para_menu_t WinbParaMeasColor[] = {
	{	"なし",					"Nothing",					},			// 0
	{	"黒を削除",				"Black is deleted",			},			// 1
	{	"白を削除",				"White is deleted",			},			// 2
	{	"白黒とも削除",			"B and W are deleted",		},			// 3
};

static struct winb_para_tm WinbmParaInspect[] = {
	{	"--- Index guide edge search ---",	"--- Index guide edge search ---",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
    {	"I.Guide search level オフセット",	"I.Guide search level offset",		&IGuideEsLevel.nOffset,			100,	-100,	1		},
	{	"I.Guide search level 下限値",		"I.Guide search level lower limit",	&IGuideEsLevel.nLower,			255,	0,		1		},
	{	"I.Guide search level 上限値",		"I.Guide search level upper limit",	&IGuideEsLevel.nUpper,			255,	0,		1		},
	{	"--- Chip edge search ----------",	"--- Chip edge search ----------",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
    {	"チップ検出 level オフセット",		"Chip search level offset",			&ChipEsLevel.nOffset,			100,	-100,	1		},
	{	"チップ検出 level 下限値",			"Chip search level lower limit",	&ChipEsLevel.nLower,			255,	0,		1		},
	{	"チップ検出 level 上限値",			"Chip search level upper limit",	&ChipEsLevel.nUpper,			255,	0,		1		},
	{	"--- G2 edge search ----------",	"--- G2 edge search ----------",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"Ｇ２エッジ検出 level オフセット",		"G2 search level offset",			&G2EsLevel.nOffset,				100,	-100,	1		},
	{	"Ｇ２エッジ検出 level 下限値",			"G2 search level lower limit",		&G2EsLevel.nLower,				255,	0,		1		},
	{	"Ｇ２エッジ検出 level 上限値",			"G2 search level upper limit",		&G2EsLevel.nUpper,				255,	0,		1		},
	{	"--- Bin measure [Chip pos.] ---",	"--- Bin measure [Chip pos.] ---",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"Bin measure Mode    (chip pos.)",	"Bin measure Mode    (chip pos.)",	&BinMeasChip.nMode,				2,		0,		0,		WinbParaMeasMode		},
	{	"Bin measure Run size(chip pos.)",	"Bin measure Run size(chip pos.)",	&BinMeasChip.nRun,				639,	0,		1								},
	{	"Bin measure Area    (chip pos.)",	"Bin measure Area    (chip pos.)",	&BinMeasChip.nArea,				5000,	0,		1								},
	{	"Bin measure Color   (chip pos.)",	"Bin measure Color   (chip pos.)",	&BinMeasChip.nColor,			3,		0,		0,		WinbParaMeasColor		},

	{	"--- Overcoat Insp. ------------",	"--- Overcoat Insp. ------------",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"Overcoat Insp. level オフセット",	"Overcoat Insp. level offse",		&OcoatInspLevel.nOffset,		100,	-100,	1		},
	{	"Overcoat Insp. level 下限値",		"Overcoat Insp. level lower limit",	&OcoatInspLevel.nLower,			255,	0,		1		},
	{	"Overcoat Insp. level 上限値",		"Overcoat Insp. level upper limit",	&OcoatInspLevel.nUpper,			255,	0,		1		},
	{	"--- Bin measure [Overcoat] ----",	"--- Bin measure [Overcate] ----",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"Bin measure Mode    (Overcoat)",	"Bin measure Mode    (Overcoat)",	&BinMeasOcoat.nMode,			2,		0,		0,		WinbParaMeasMode		},
	{	"Bin measure Run size(Overcoat)",	"Bin measure Run size(Overcoat)",	&BinMeasOcoat.nRun,				639,	0,		1								},
	{	"Bin measure Area    (Overcoat)",	"Bin measure Area    (Overcoat)",	&BinMeasOcoat.nArea,			5000,	0,		1								},
	{	"Bin measure Color   (Overcoat)",	"Bin measure Color   (Overcoat)",	&BinMeasOcoat.nColor,			3,		0,		0,		WinbParaMeasColor		},
	{	"--- Insp. area [Overcoat] -----",	"--- Insp. area [Overcoat] -----",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"Offset area   X[um] (Overcoat)",	"Offset area   X[um] (Overcoat)",	&InspOcOffsetSize[0],			100,	0,		1								},
	{	"Offset area   Y[um] (Overcoat)",	"Offset area   Y[um] (Overcoat)",	&InspOcOffsetSize[1],			100,	0,		1								},
	{	"Offset corner X[um] (Overcoat)",	"Offset corner X[um] (Overcoat)",	&InspOcOffsetCor[0],			100,	0,		1								},
	{	"Offset corner Y[um] (Overcoat)",	"Offset corner Y[um] (Overcoat)",	&InspOcOffsetCor[1],			100,	0,		1								},
	{	"- Overcoat Insp. Black(Rimen) -",	"- Overcoat Insp. Black(Rimen) -",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},//v2.11
	{	"Overcoat Insp.Black level オフセット",	"Overcoat Insp.Black level offse",		&OcoatInspLevelBlack.nOffset,	100,	-100,	1		},
	{	"Overcoat Insp.Black level 下限値",		"Overcoat Insp.Black level lower limit",&OcoatInspLevelBlack.nLower,	255,	0,		1		},
	{	"Overcoat Insp.Black level 上限値",		"Overcoat Insp.Black level upper limit",&OcoatInspLevelBlack.nUpper,	255,	0,		1		},
	{	"-Insp. area[Overcoat]Black(Rimen)-",	"-Insp.area[Overcoat]Black(Rimen)-",	&WorkParaDummy,    		0,		0,		0,		WorkParaCommentDummy	},//2.11
	{	"Offset area   X[um] Black(Overcoat)",	"Offset area   X[um] Black(Overcoat)",	&InspOcOffsetSizeBlack[0],200,	0,		1								},
	{	"Offset area   Y[um] Black(Overcoat)",	"Offset area   Y[um] Black(Overcoat)",	&InspOcOffsetSizeBlack[1],200,	0,		1								},
	{	"Offset corner X[um] Black(Overcoat)",	"Offset corner X[um] Black(Overcoat)",	&InspOcOffsetCorBlack[0],100,	0,		1								},
	{	"Offset corner Y[um] Black(Overcoat)",	"Offset corner Y[um] Black(Overcoat)",	&InspOcOffsetCorBlack[1],100,	0,		1								},

	{	"--- Bin measure [Elec] --------",	"--- Bin measure [Elec] --------",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"Electrode Insp. level オフセット",	"Electrode Insp. level offse",		&ElecInspLevel.nOffset,			100,	-100,	1		},
	{	"Electrode Insp. level 下限値",		"Electrode Insp. level lower limit",&ElecInspLevel.nLower,			255,	0,		1		},
	{	"Electrode Insp. level 上限値",		"Electrode Insp. level upper limit",&ElecInspLevel.nUpper,			255,	0,		1		},
	{	"--- Bin measure [Elec] --------",	"--- Bin measure [Elec] --------",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"Bin measure Mode    (Elec)",		"Bin measure Mode    (Elec)",		&BinMeasElec.nMode,				2,		0,		0,		WinbParaMeasMode		},
	{	"Bin measure Run size(Elec)",		"Bin measure Run size(Elec)",		&BinMeasElec.nRun,				639,	0,		1								},
	{	"Bin measure Area    (Elec)",		"Bin measure Area    (Elec)",		&BinMeasElec.nArea,				5000,	0,		1								},
	{	"Bin measure Color   (Elec)",		"Bin measure Color   (Elec)",		&BinMeasElec.nColor,			3,		0,		0,		WinbParaMeasColor		},

	{	"--- Bin measure [Plating] -----",	"--- Bin measure [latingc] -----",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"Plating Insp. level オフセット",	"Plating Insp. level offse",		&PlatingInspLevel.nOffset,		100,	-100,	1		},
	{	"Plating Insp. level 下限値",		"Plating Insp. level lower limit",	&PlatingInspLevel.nLower,		255,	0,		1		},
	{	"Plating Insp. level 上限値",		"Plating Insp. level upper limit",	&PlatingInspLevel.nUpper,		255,	0,		1		},
	{	"--- Bin measure [Plating] -----",	"--- Bin measure [Plating] -----",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"Bin measure Mode    (Plating)",	"Bin measure Mode    (Plating)",	&BinMeasPlating.nMode,			2,		0,		0,		WinbParaMeasMode		},
	{	"Bin measure Run size(Plating)",	"Bin measure Run size(Plating)",	&BinMeasPlating.nRun,			639,	0,		1								},
	{	"Bin measure Area    (Plating)",	"Bin measure Area    (Plating)",	&BinMeasPlating.nArea,			5000,	0,		1								},
	{	"Bin measure Color   (Plating)",	"Bin measure Color   (Plating)",	&BinMeasPlating.nColor,			3,		0,		0,		WinbParaMeasColor		},
	{	"--- Insp. area [Plating] ------",	"--- Insp. area [Plating] ------",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"Offset area in  Y [um] (Plating)",	"Offset area in  Y[um] (Plating)",	&InspPlOffsetSize[0],			100,	0,		1								},
	{	"Offset area out Y [um] (Plating)",	"Offset area out Y[um] (Plating)",	&InspPlOffsetSize[1],			100,	0,		1								},

	{	"--- Robust condition ----------",	"--- Robust condition ----------",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"Robust condition deg width[*10]",	"Robust condition deg width[*10]",	&RobustDegWidth,				1800,	0,		1								},
	{	"Robust condition deg step [*10]",	"Robust condition deg step [*10]",	&RobustDegStep,					1800,	1,		1								},

	{	"--- Index Guide Brightness ----",	"--- Index Guide Brightness ----",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"Standard Level",                	"Standard Level",                  	&TP_Bright_st,			   	  255,	    0,		1								},
	{   "Warning Frequency [0=OFF]",        "Warning Frequency [0=OFF]"       , &TP_BrightFlag,               100,      0,      1								},
	{	"--- Led Set Brightness ----",	"--- Led Set Brightness ----",			&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"LED Bin Level",                	"LED Bin Level",                  	&TP_Bright_SetLevel,	 	  255,	    0,		1								},
	{   "LED OK Percentage",				"LED OK Percentage"       ,			&TP_Bright_SetPercent,         100,      0,      1								},

	{	"--- Bin Level ----",			"--- Bin Level ----",					&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"Bin Mezumari 0",                	"Bin Mezumari 0",                  	&Bin_Mz_l[0],			 	  255,	    0,		1								},
	{	"Bin Mezumari 1",                	"Bin Mezumari 1",                  	&Bin_Mz_l[1],			 	  255,	    0,		1								},
	{	"Bin Mezumari 2",                	"Bin Mezumari 2",                  	&Bin_Mz_l[2],			 	  255,	    0,		1								},
	{	"Bin Mezumari 3",                	"Bin Mezumari 3",                  	&Bin_Mz_l[3],			 	  255,	    0,		1								},
	{	"Bin Mezumari 4",                	"Bin Mezumari 4",                  	&Bin_Mz_l[4],			 	  255,	    0,		1								},

	{	"--- Light Level ----",			"--- Light Level ----",					&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"Elec Left  Lower Limit",            "Elec Left  Lower Limit",          &TP_Bright_LowerLimit[0],	  255,	    0,		1								},
	{	"G2         Lower Limit",            "G2         Lower Limit",          &TP_Bright_LowerLimit[1],	  255,	    0,		1								},
	{	"Elec Right Lower Limit",            "Elec Right Lower Limit",          &TP_Bright_LowerLimit[2],	  255,	    0,		1								},

	{	"-------------------------------",	"-------------------------------",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	NULL	},
};

static struct winb_para_tm WinbmParaInspectUser[] = {
	{	"------------------------------",	"------------------------------",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"機台基準回転数",					"M/C Standard rpm",					&Std_rpm,						6000,	10,		10,		NULL			},
	{	NULL	},
};

static struct winb_para_menu_t WinbParaResultOutput[] = {
	{	"                                Normal"	,"                                Normal"	},		// RESULT_OUTPUT_NORMAL		0
	{	"                                ALL GO"	,"                                ALL GO"	},		// RESULT_OUTPUT_ALL_GO		1
	{	"  ALL NG NoWork(NG,NoworkOn,ReverseOn)"	,"  ALL NG NoWork(NG,NoworkOn,ReverseOn)"	},		// RESULT_OUTPUT_ALL_NG		2
	{	"ALL NG Reverse(NG,NoworkOff,ReverseOn)"	,"ALL NG Reverse(NG,NoworkOff,ReverseOn)"	},		// RESULT_OUTPUT_ALL_NG		3
	{	"      ALL NG (NG,NoworkOff,ReverseOff)"	,"      ALL NG (NG,NoworkOff,ReverseOff)"	},		// RESULT_OUTPUT_ALL_NG		4
};

static struct winb_para_menu_t WinbParaResultmarkdisp[] = {
	{	"                                No Display"	,"                          NO"	},		//0 9499
	{	"                                Display"	,"                              YES"	},		//	1 9499
};

static struct winb_para_tm WinbmParaSystem[] = {
    {	"Ｘ方向分解能\ [*1000um/pixel]",		"Rate X [*1000um/pixel]",			&SystemPara.nRateX,				100000,	0,		1		},
    {	"Ｙ方向分解能\ [*1000um/pixel]",		"Rate Y [*1000um/pixel]",			&SystemPara.nRateY,				100000,	0,		1		},
	{	"------------------------------",	"------------------------------",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"シャッタースピード",				"Shutter Speed(Top    Light)",		&SystemPara.nShutterSpeed,		20000,	10,		10		},
	{	"取り込み前タイマ[us]",				"Capture Timer[us]",				&SystemPara.nCaptureTimer,		20000,	0,		10		},
	{	"------------------------------",	"------------------------------",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"照明電流(ドーム) [uA]",			"Light Current(Dome)    [uA]",		&LedCurrent[0],					100000,	0,		100		},
	{	"照明電流(同軸)   [uA]",			"Light Current(Coaxial) [uA]",		&LedCurrent[1],					100000,	0,		100		},
	{	"------------------------------",	"------------------------------",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"機台基準回転数",					"M/C Standard rpm",					&Std_rpm,						4000,	10,		10,		NULL			},
	{	"------------------------------",	"------------------------------",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	"判定結果変更"					,"Change Judge Mode"		,&nJudgeMode				,4			,0		,0		,WinbParaResultOutput },
	{	"Marking Character Match Disp."					,"Marking Character Match Disp."		,&nDispMode				,1			,0		,0		,WinbParaResultmarkdisp },
	{	NULL	},
};

static struct winb_para_tm WinbmParaSystemUser[] = {
	{	"------------------------------",	"------------------------------",	&WorkParaDummy,    				0,		0,		0,		WorkParaCommentDummy	},
	{	NULL	},
};

static struct winb_para_tm NGLimitOnOff[] = {
    {	"No Work",					"No Work                [ NG1  ]",	&NG_Limit_OnOff[1],				0,	0,	0,    WinbParaOnOff 		},
    {	"Can't Insp.",				"Can't Insp.            [ NG2  ]",	&NG_Limit_OnOff[2],				0,	0,	0,    WinbParaOnOff 		},
    {	"Index pos. Error",			"Index pos. Error       [ NG3  ]",	&NG_Limit_OnOff[3],				1,	0,	0,    WinbParaOnOff 		},
    {	"Tape pos. Error",			"Tape pos. Error        [ NG4  ]",	&NG_Limit_OnOff[4],				1,	0,	0,    WinbParaOnOff 		},
    {	"Chip Reverse",				"Chip Reverse           [ NG5  ]",	&NG_Limit_OnOff[5],				1,	0,	0,    WinbParaOnOff 		},
    {	"Focus Error",				"Focus Error            [ NG6  ]",	&NG_Limit_OnOff[6],				1,	0,	0,    WinbParaOnOff 		},
    {	"G2 X size",				"G2 X size              [ NG11 ]",	&NG_Limit_OnOff[11],			1,	0,	0,    WinbParaOnOff 		},
    {	"G2 Y size",				"G2 Y size              [ NG12 ]",	&NG_Limit_OnOff[12],			1,	0,	0,    WinbParaOnOff 		},
    {	"G2 Kake X",				"G2 Kake X              [ NG13 ]",	&NG_Limit_OnOff[13],			1,	0,	0,    WinbParaOnOff 		},
    {	"G2 Kake Y",				"G2 Kake Y              [ NG14 ]",	&NG_Limit_OnOff[14],			1,	0,	0,    WinbParaOnOff 		},
    {	"G2 position X Error",		"G2 position X Error    [ NG15 ]",	&NG_Limit_OnOff[15],			1,	0,	0,    WinbParaOnOff 		},
    {	"G2 position Y Error",		"G2 position Y Error    [ NG16 ]",	&NG_Limit_OnOff[16],			1,	0,	0,    WinbParaOnOff 		},
    {	"Overcoat kake X",			"Overcoat kake X        [ NG17 ]",	&NG_Limit_OnOff[17],			1,	0,	0,    WinbParaOnOff 		},
    {	"Overcoat kake Y",			"Overcoat kake Y        [ NG18 ]",	&NG_Limit_OnOff[18],			1,	0,	0,    WinbParaOnOff 		},
    {	"Overcoat kake XY",			"Overcoat kake XY       [ NG19 ]",	&NG_Limit_OnOff[19],			1,	0,	0,    WinbParaOnOff 		},
    {	"CHIP X size",				"CHIP X size            [ NG21 ]",	&NG_Limit_OnOff[21],			1,	0,	0,    WinbParaOnOff 		},
    {	"CHIP Y size",				"CHIP Y size            [ NG22 ]",	&NG_Limit_OnOff[22],			1,	0,	0,    WinbParaOnOff 		},
    {	"CHIP Kake X",				"CHIP Kake X            [ NG23 ]",	&NG_Limit_OnOff[23],			1,	0,	0,    WinbParaOnOff 		},
    {	"CHIP Kake Y",				"CHIP Kake Y            [ NG24 ]",	&NG_Limit_OnOff[24],			1,	0,	0,    WinbParaOnOff 		},
    {	"CHIP Kake corner",			"CHIP Kake corner       [ NG25 ]",	&NG_Limit_OnOff[25],			1,	0,	0,    WinbParaOnOff 		},
    {	"Plating adhesion Error",	"Plating adhesion Error [ NG31 ]",	&NG_Limit_OnOff[31],			1,	0,	0,    WinbParaOnOff 		},
    {	"Marking Error",			"Marking Error          [ NG41 ]",	&NG_Limit_OnOff[41],			1,	0,	0,    WinbParaOnOff 		},
    {	"Mark segment Error",		"Mark segment Error     [ NG42 ]",	&NG_Limit_OnOff[42],			1,	0,	0,    WinbParaOnOff 		},
    {	"Elec. X size",				"Elec. X size           [ NG51 ]",	&NG_Limit_OnOff[51],			1,	0,	0,    WinbParaOnOff 		},
    {	"Elec. Y size",				"Elec. Y size           [ NG52 ]",	&NG_Limit_OnOff[52],			1,	0,	0,    WinbParaOnOff 		},
    {	"Elec. edge KakeX",			"Elec. edge KakeX       [ NG53 ]",	&NG_Limit_OnOff[53],			1,	0,	0,    WinbParaOnOff 		},
    {	"Elec. edge KakeY",			"Elec. edge KakeY       [ NG54 ]",	&NG_Limit_OnOff[54],			1,	0,	0,    WinbParaOnOff 		},
    {	"Elec. Kake",				"Elec. Kake             [ NG55 ]",	&NG_Limit_OnOff[55],			1,	0,	0,    WinbParaOnOff 		},
    {	"Elec. black Error",		"Elec. black Error      [ NG56 ]",	&NG_Limit_OnOff[56],			1,	0,	0,    WinbParaOnOff 		},
    {	"Side Elec. X size",		"Side Elec. X size      [ NG57 ]",	&NG_Limit_OnOff[57],			1,	0,	0,    WinbParaOnOff 		},
    {	"Resistance",				"Resistance",						&NG_Limit_OnOff[81],			1,	0,	0,    WinbParaOnOff 		},
    {	"Ninshiki",					"Ninshiki",							&NG_Limit_OnOff[82],			1,	0,	0,    WinbParaOnOff 		},
	{	NULL	},
};
//static int TestFunction(int nNumber);

//メニューの設定
struct winb_t WinbSettingInspMenu2 = {
    {	"検査設定2", "Inspect Setting2"	},
	20, 21, 21,
    {
		{	"検査設定",             "Inspect Setting",		ParameterInspSet,			NULL				},
		{	"Search Index Setting", "Search Index Setting",	SetInspSearchIndex,			NULL				},
		{	"Search Guide Setting", "Search Guide Setting",	SetInspSearchGuide,			NULL				},
        {	"Index Table Bright",   "Index Table Bright",	SetInspTP_IndexTableBright,	NULL				},
		{	"Index Guide Bright",   "Index Guide Bright",	SetInspTPCheckBright,		NULL				},
		{	"Search Elect Setting", "Search Elect Setting",	SetInspSearchElect,     	NULL				},
		{	"Search G2ES Setting",  "Search G2ES Setting",	SetInspTWA_G2_ES_search,    NULL				},
		{	"ChipPos Setting",      "ChipPos Setting",      SetInspChipPos,            	NULL				},
        {	"Reverse Setting",      "Reverse Setting",      SetInspReverse,           	NULL				},
        {	"ChipSize Setting",     "ChipSize Setting",     SetInspChipSize,        	NULL				},
        {	"Electrode Setting",    "Electrode Setting",    SetInspElectrode,        	NULL				},
		{	"Mark Setting",			"Mark Setting",			SetInspMark,				NULL				},
		{	"OverCoat Setting",     "OverCoat Setting",		SetInspOverCoat,			NULL				},
        {	"Plating Setting",      "Plating Setting",		SetInspPlating,             NULL				},
        {	"Pinhole Setting",      "Pinhole Setting",		SetInspPinhole,             NULL				},
		{	"C2Size Setting",       "C2Size Setting",		SetInspC2Xsize,             NULL				},
		{	"Medumari Setting",     "Medumzri Setting",		SetInspMezuArea,            NULL				},
        {	"SDl Setting",          "SDl Setting",          SetInspSDl,                 NULL				},
        {	"APl Setting",          "APl Setting",          SetInspAPl,                 NULL				},
		{	"G2 Zure Setting",      "G2 Zure Setting",      SetInspZure,                NULL				},
		{	"C1 Space Setting",     "C1 Space Setting",     SetInspC1Space,             NULL				},

	}
};

//メニューの設定(任意)
struct winb_t WinbSettingInspMenu = {
	{	"検査設定", "Inspect Setting"	},
	20, 4, 4,
	{
        {	"検査設定2>",			"Inspect Setting2>",	Dummy,          			&WinbSettingInspMenu2},
		{	"判定値表\示",		"Judgment value",		JudgeValueDisp,				NULL				},
		{	"仕様設定",			"Spec setting",			SpecSet,					NULL				},
		{	"Print test",   	"Print test",			Print2,						NULL				},
	}
};

struct winb_t WinbSettingSystemMenu = {
	{	"システム設定", "System Setting"	},
	20, 4, 4,
	{
		{	"システム設定"		,"System Setting"		,&ParameterSysSet				,NULL	},
		{	"IOチェック"			,"IOcheck"				,MenuIOCheck					,NULL	},
		{	"NG limit On/OFF"	,"NG limit On/OFF"		,NGLimitCHK						,NULL	},    //2011.08.17
		{	"Monitoring Br Set"	,"Monitoring Br Set"	,MonitorCHK						,NULL	},	  //2017.08.07
	}
};

struct winb_item_t WinbSettingItem_repi[] = {
	{	"検査設定 >"		," Inspect Setting   >"		,Dummy				,&WinbSettingInspMenu	},
	{	"システム設定 >"	," System Setting    >"		,Dummy				,&WinbSettingSystemMenu	},
	{	"照明調整"			,"   Light Setting    "		,LedAdjust			,NULL					},
	{	"照明調整2"			,"   Light Setting2    "	,LedLevel			,NULL					},

	{	NULL	},															// 必ず必要
};

struct winb_item_t WinbSettingItem_rist[] = {
	{	"検査設定 >"		," Inspect Setting   >"		,Dummy				,&WinbSettingInspMenu	},
	{	"システム設定 >"	," System Setting    >"		,Dummy				,&WinbSettingSystemMenu	},
	{	"照明調整2"			,"   Light Setting2    "	,LedLevel			,NULL					},

	{	NULL	},															// 必ず必要
};

struct winb_item_t WinbSettingItem_Nitto[] = {
	{	"検査設定 >"		," Inspect Setting   >"		,Dummy				,&WinbSettingInspMenu	},
	{	"システム設定 >"	," System Setting    >"		,Dummy				,&WinbSettingSystemMenu	},
	{	"照明調整3"			,"   Light Setting3    "	,MenuLightSet		,NULL					},

	{	NULL	},															// 必ず必要
};

struct winb_item_t *WinbSettingItem = WinbSettingItem_rist;

struct winb_item_t WinbFileItem[] = {
	{	"Fail 画像表\示"			,"Fail Display"			,DispFailImage		,NULL					},

	{	NULL	},															// 必ず必要
};



struct	ParameterCheckItem {
	char		Prod[32];
	char		Insp[32];
	char		Ver[32];
}		SyFile, JdFile, TyFile;

//機台データ・機種データの設定(任意)
//R_CS UserCordingStart

// 機台情報（検査条件）
static char SystemFileName[] = "TP-S_sy.csv";
R_DATAFILE_ITEM SystemFileData[] = {
	// --- パラメータ確認項目（不一致の場合は、エラー停止）
	{	"SyFile.Prod",					SyFile.Prod,					R_TYPE_STRING	,32		},
	{	"SyFile.Insp",					SyFile.Insp,					R_TYPE_STRING	,32		},
	{	"SyFile.Ver",					SyFile.Ver,						R_TYPE_STRING	,32		},
	// --- 機台情報（検査条件）
	{	"MachineName",					&MachineName,					R_TYPE_STRING	,32		},
	{	"SystemPara.nRateX",			&SystemPara.nRateX,				R_TYPE_INT				},
	{	"SystemPara.nRateY",			&SystemPara.nRateY,				R_TYPE_INT				},
	{	"ShutterSpeed",					&SystemPara.nShutterSpeed,		R_TYPE_INT				},
	{	"CaptureTimer",					&SystemPara.nCaptureTimer,		R_TYPE_INT				},
	{	"CameraName",					&SystemPara.nCameraName,		R_TYPE_INT				},
	{	"LedCurrent",					&LedCurrent,					R_TYPE_INT		,0	,6	},
	{	"SampleAreaNum",				&SampleAreaNum,					R_TYPE_INT				},
	{	"SampleArea",					&SampleArea,					R_TYPE_RECT		,0	,8	},
	// --- Index guide edge search level
	{	"IGuideEsLevel.nOffset",		&IGuideEsLevel.nOffset,			R_TYPE_INT				},
	{	"IGuideEsLevel.nLower",			&IGuideEsLevel.nLower,			R_TYPE_INT				},
	{	"IGuideEsLevel.nUpper",			&IGuideEsLevel.nUpper,			R_TYPE_INT				},
	// --- Chip edge search level
	{	"ChipEsLevel.nOffset",			&ChipEsLevel.nOffset,			R_TYPE_INT				},
	{	"ChipEsLevel.nLower",			&ChipEsLevel.nLower,			R_TYPE_INT				},
	{	"ChipEsLevel.nUpper",			&ChipEsLevel.nUpper,			R_TYPE_INT				},
	// --- G2 edge search level
	{	"G2EsLevel.nOffset",			&G2EsLevel.nOffset,				R_TYPE_INT				},
	{	"G2EsLevel.nLower",				&G2EsLevel.nLower,				R_TYPE_INT				},
	{	"G2EsLevel.nUpper",				&G2EsLevel.nUpper,				R_TYPE_INT				},
	// --- Bin measure(Chip pos.)
	{	"BinMeasChip.nMode",			&BinMeasChip.nMode,				R_TYPE_INT				},
	{	"BinMeasChip.nRun",				&BinMeasChip.nRun,				R_TYPE_INT				},
	{	"BinMeasChip.nArea",			&BinMeasChip.nArea,				R_TYPE_INT				},
	{	"BinMeasChip.nColor",			&BinMeasChip.nColor,			R_TYPE_INT				},
	// --- Bin measure(Overcoat Insp.)
	{	"BinMeasOcoat.nMode",			&BinMeasOcoat.nMode,			R_TYPE_INT				},
	{	"BinMeasOcoat.nRun",			&BinMeasOcoat.nRun,				R_TYPE_INT				},
	{	"BinMeasOcoat.nArea",			&BinMeasOcoat.nArea,			R_TYPE_INT				},
	{	"BinMeasOcoat.nColor",			&BinMeasOcoat.nColor,			R_TYPE_INT				},
	// --- Bin measure(Elec Insp.)
	{	"BinMeasElec.nMode",			&BinMeasElec.nMode,				R_TYPE_INT				},
	{	"BinMeasElec.nRun",				&BinMeasElec.nRun,				R_TYPE_INT				},
	{	"BinMeasElec.nArea",			&BinMeasElec.nArea,				R_TYPE_INT				},
	{	"BinMeasElec.nColor",			&BinMeasElec.nColor,			R_TYPE_INT				},
	// --- Bin measure(Plating Insp.)
	{	"BinMeasPlating.nMode",			&BinMeasPlating.nMode,			R_TYPE_INT				},
	{	"BinMeasPlating.nRun",			&BinMeasPlating.nRun,			R_TYPE_INT				},
	{	"BinMeasPlating.nArea",			&BinMeasPlating.nArea,			R_TYPE_INT				},
	{	"BinMeasPlating.nColor",		&BinMeasPlating.nColor,			R_TYPE_INT				},

	// --- Overcoat Insp. level
	{	"OcoatInspLevel.nOffset",		&OcoatInspLevel.nOffset,		R_TYPE_INT				},
	{	"OcoatInspLevel.nLower",		&OcoatInspLevel.nLower,			R_TYPE_INT				},
	{	"OcoatInspLevel.nUpper",		&OcoatInspLevel.nUpper,			R_TYPE_INT				},
	// --- Overcoat Insp. level Black v2.11
	{	"OcoatInspLevelBlack.nOffset",	&OcoatInspLevelBlack.nOffset,	R_TYPE_INT				},
	{	"OcoatInspLevelBlack.nLower",	&OcoatInspLevelBlack.nLower,	R_TYPE_INT				},
	{	"OcoatInspLevelBlack.nUpper",	&OcoatInspLevelBlack.nUpper,	R_TYPE_INT				},
	// --- Elec Insp. level
	{	"ElecInspLevel.nOffset",		&ElecInspLevel.nOffset,			R_TYPE_INT				},
	{	"ElecInspLevel.nLower",			&ElecInspLevel.nLower,			R_TYPE_INT				},
	{	"ElecInspLevel.nUpper",			&ElecInspLevel.nUpper,			R_TYPE_INT				},
	// --- Plating Insp. level
	{	"PlatingInspLevel.nOffset",		&PlatingInspLevel.nOffset,		R_TYPE_INT				},
	{	"PlatingInspLevel.nLower",		&PlatingInspLevel.nLower,		R_TYPE_INT				},
	{	"PlatingInspLevel.nUpper",		&PlatingInspLevel.nUpper,		R_TYPE_INT				},

	// --- Overcoat Insp. area offset
	{	"InspOcOffsetSize",				&InspOcOffsetSize,				R_TYPE_INT		,0	,2	},
	{	"InspOcOffsetCor",				&InspOcOffsetCor,				R_TYPE_INT		,0	,2	},
	// --- Overcoat Insp. area offset Black			v2.11
	{	"InspOcOffsetSizeBlack",		&InspOcOffsetSizeBlack,			R_TYPE_INT		,0	,2	},
	{	"InspOcOffsetCorBlack",			&InspOcOffsetCorBlack,			R_TYPE_INT		,0	,2	},
	// --- Plating Insp. area offset
	{	"InspPlOffsetSize",				&InspPlOffsetSize,				R_TYPE_INT		,0	,2	},

	// --- Robust data
	{	"RobustDegWidth",				&RobustDegWidth,				R_TYPE_INT				},
	{	"RobustDegStep",				&RobustDegStep,					R_TYPE_INT				},
	// --- Index Guide Bright level
	{	"IndexGuideBright",				&TP_Bright_st,					R_TYPE_INT				},
	{	"BrightWarning",				&TP_BrightFlag,					R_TYPE_INT				},
	// --- LED Set Bright level
	{	"LEDSetBrightLevel",			&TP_Bright_SetLevel,			R_TYPE_INT				},
	{	"LEDSetPercent",				&TP_Bright_SetPercent,			R_TYPE_INT				},

	// --- Electrode G2  Bright level
	{	"BrightElectrode",				&TP_Bright_LowerLimit,			R_TYPE_INT		,0  ,3  },

	// --- Setubi data
	{	"Std_rpm",						&Std_rpm,						R_TYPE_INT				},

	// --- Mezumari Bin level
	{	"BinMezumari",					&Bin_Mz_l,					    R_TYPE_INT		,0  ,5  }, //C1 mezumari Binary level			//<z3>
	{	NULL	},
};

// 検査パラメータ（判定値）
static char JudgeFilename[] = "TP-S_Jd.csv";
R_DATAFILE_ITEM JudgeFileData[] = {
	// --- パラメータ確認項目（不一致の場合は、エラー停止）
	{	"JdFile.Prod",					&JdFile.Prod,					R_TYPE_STRING	,32		},
	{	"JdFile.Insp",					&JdFile.Insp,					R_TYPE_STRING	,32		},
	{	"JdFile.Ver",					&JdFile.Ver,					R_TYPE_STRING	,32		},
	// --- Judge data
	// --- G2 Insp. 判定値
	{	"InspParaM.G2Xmin_0_",			&InspParaM.G2Xmin[0],			R_TYPE_INT		,0	,3	},			// G2 X size 最小値[X,Y,code]
	{	"InspParaM.G2Xmax_0_",			&InspParaM.G2Xmax[0],			R_TYPE_INT		,0	,3	},			// G2 X size 最大値[X,Y,code]
	{	"InspParaM.G2Xrange_0_",		&InspParaM.G2Xrange[0],			R_TYPE_INT		,0	,3	},			// G2 X max-min[X,Y,code]
	{	"InspParaM.G2Xedge_0_",			&InspParaM.G2Xedge[0],			R_TYPE_INT		,0	,3	},			// G2 X エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureXmin_0_",		&InspParaM.G2ZureXmin[0],		R_TYPE_INT		,0	,3	},			// G2 X Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureXmax_0_",		&InspParaM.G2ZureXmax[0],		R_TYPE_INT		,0	,3	},			// G2 X Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2Ymin_0_",			&InspParaM.G2Ymin[0],			R_TYPE_INT		,0	,3	},			// G2 Y size 最小値[X,Y,code]
	{	"InspParaM.G2Ymax_0_",			&InspParaM.G2Ymax[0],			R_TYPE_INT		,0	,3	},			// G2 Y size 最大値[X,Y,code]
	{	"InspParaM.G2Yrange_0_",		&InspParaM.G2Yrange[0],			R_TYPE_INT		,0	,3	},			// G2 Y max-min[X,Y,code]
	{	"InspParaM.G2Yedge_0_",			&InspParaM.G2Yedge[0],			R_TYPE_INT		,0	,3	},			// G2 Y エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureYmin_0_",		&InspParaM.G2ZureYmin[0],		R_TYPE_INT		,0	,3	},			// G2 Y Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureYmax_0_",		&InspParaM.G2ZureYmax[0],		R_TYPE_INT		,0	,3	},			// G2 Y Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2YSdl_0_",			&InspParaM.G2YSdl[0],			R_TYPE_INT		,0	,3	},			// G2 Y SDtoG2検査 判定値[X,Y,code]
	{	"InspParaM.G2YApl_0_",			&InspParaM.G2YApl[0],			R_TYPE_INT		,0	,3	},			// G2 Y SDtoAP検査 判定値[X,Y,code]
	{	"InspParaM.OCoatXsize_0_",		&InspParaM.OCoatXsize[0],		R_TYPE_INT		,0	,3	},			// Overcoat X カケ検査 判定値[X,Y,code]
	{	"InspParaM.OCoatYsize_0_",		&InspParaM.OCoatYsize[0],		R_TYPE_INT		,0	,3	},			// Overcoat Y カケ検査 判定値[X,Y,code]
	{	"InspParaM.OCoatXYsize_0_",		&InspParaM.OCoatXYsize[0],		R_TYPE_INT		,0	,3	},			// Overcoat XYカケ検査 判定値[X,Y,code]
	// --- G2 position
	{	"InspParaM.G2PosXmin_0_",		&InspParaM.G2PosXmin[0],		R_TYPE_INT		,0	,3	},			// G2 X position min[X,Y,code]
	{	"InspParaM.G2PosXmax_0_",		&InspParaM.G2PosXmax[0],		R_TYPE_INT		,0	,3	},			// G2 X position max[X,Y,code]
	{	"InspParaM.G2PosYmin_0_",		&InspParaM.G2PosYmin[0],		R_TYPE_INT		,0	,3	},			// G2 Y position min[X,Y,code]
	{	"InspParaM.G2PosYmax_0_",		&InspParaM.G2PosYmax[0],		R_TYPE_INT		,0	,3	},			// G2 Y position max[X,Y,code]
	// --- Plating adhesion
	{	"InspParaM.Plating_0_",			&InspParaM.Plating[0],			R_TYPE_INT		,0	,3	},			// Plating adhesion size[X,Y,code]
	// --- Chip(Appearence) Insp. 判定値
	{	"InspParaM.ChipXmin_0_",		&InspParaM.ChipXmin[0],			R_TYPE_INT		,0	,3	},			// Chip X size 最小値[X,Y,code]
	{	"InspParaM.ChipXmax_0_",		&InspParaM.ChipXmax[0],			R_TYPE_INT		,0	,3	},			// Chip X size 最大値[X,Y,code]
	{	"InspParaM.ChipXrange_0_",		&InspParaM.ChipXrange[0],		R_TYPE_INT		,0	,3	},			// Chip X max-min[X,Y,code]
	{	"InspParaM.ChipXedge_0_",		&InspParaM.ChipXedge[0],		R_TYPE_INT		,0	,3	},			// Chip X エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.ChipYmin_0_",		&InspParaM.ChipYmin[0],			R_TYPE_INT		,0	,3	},			// Chip Y size 最小値[X,Y,code]
	{	"InspParaM.ChipYmax_0_",		&InspParaM.ChipYmax[0],			R_TYPE_INT		,0	,3	},			// Chip Y size 最大値[X,Y,code]
	{	"InspParaM.ChipYrange_0_",		&InspParaM.ChipYrange[0],		R_TYPE_INT		,0	,3	},			// Chip Y max-min[X,Y,code]
	{	"InspParaM.ChipYedge_0_",		&InspParaM.ChipYedge[0],		R_TYPE_INT		,0	,3	},			// Chip Y エッジカケ検査 判定値[X,Y,code]
	// --- Electrode Insp.
	{	"InspParaM.ElecXmin_0_",		&InspParaM.ElecXmin[0],			R_TYPE_INT		,0	,3	},			// 電極寸法 X最小値[X,Y,code]
	{	"InspParaM.ElecXmax_0_",		&InspParaM.ElecXmax[0],			R_TYPE_INT		,0	,3	},			// 電極寸法 X最大値[X,Y,code]
	{	"InspParaM.ElecXrange_0_",		&InspParaM.ElecXrange[0],		R_TYPE_INT		,0	,3	},			// 電極寸法 X max-min[X,Y,code]
	{	"InspParaM.ElecXedge_0_",		&InspParaM.ElecXedge[0],		R_TYPE_INT		,0	,3	},			// 電極寸法 X エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.ElecYmin_0_",		&InspParaM.ElecYmin[0],			R_TYPE_INT		,0	,3	},			// 電極寸法 Y最小値[X,Y,code]
	{	"InspParaM.ElecYmax_0_",		&InspParaM.ElecYmax[0],			R_TYPE_INT		,0	,3	},			// 電極寸法 Y最大値[X,Y,code]
	{	"InspParaM.ElecYrange_0_",		&InspParaM.ElecYrange[0],		R_TYPE_INT		,0	,3	},			// 電極寸法 Y max-min[X,Y,code]
	{	"InspParaM.ElecYedge_0_",		&InspParaM.ElecYedge[0],		R_TYPE_INT		,0	,3	},			// 電極寸法 Y エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.ElecXsub_0_",		&InspParaM.ElecXsub[0],			R_TYPE_INT		,0	,3	},			// 電極幅左右寸法差[X,Y,code]
	{	"InspParaM.ElecKake_0_",		&InspParaM.ElecKake[0],			R_TYPE_INT		,0	,3	},			// 電極カケ検査判定値[%,nu,code]
	{	"InspParaM.ElecBlack_0_",		&InspParaM.ElecBlack[0],		R_TYPE_INT		,0	,3	},			// 電極変色判定値[%,nu,code]
	{	"InspParaM.ElecC2Xmin_0_",		&InspParaM.ElecC2Xmin[0],		R_TYPE_INT		,0	,3	},			// C2電極被り小[X,Y,code]
	{	"InspParaM.ElecC1Mezumari_0_",	&InspParaM.ElecC1Mezumari[0],	R_TYPE_INT		,0	,3	},			// C1目詰まり[%,nu,code]
	{	"InspParaM.SideElecXmin_0_",	&InspParaM.SideElecXmin[0],		R_TYPE_INT		,0	,3	},			// 側面電極被り込み寸法 X最小値[X,Y,code]
	{	"InspParaM.MarkLevel_0_",		&InspParaM.MarkLevel[0],		R_TYPE_INT		,0	,3	},			//
	{	"InspParaM.MarkShift_0_",		&InspParaM.MarkShift[0],		R_TYPE_INT		,0	,3	},			//
	{	"InspParaM.IbutsuXsize_0_",		&InspParaM.IbutsuXsize[0],		R_TYPE_INT		,0	,3	},			// Glass Ibutsu X 検査 判定値[X,Y,code]
	{	"InspParaM.IbutsuYsize_0_",		&InspParaM.IbutsuYsize[0],		R_TYPE_INT		,0	,3	},			// Glass Ibutsu Y 検査 判定値[X,Y,code]

	// --- G2 Insp. 判定値
	{	"InspParaM.G2Xmin_1_",			&InspParaM.G2Xmin[1],			R_TYPE_INT		,0	,3	},			// G2 X size 最小値[X,Y,code]
	{	"InspParaM.G2Xmax_1_",			&InspParaM.G2Xmax[1],			R_TYPE_INT		,0	,3	},			// G2 X size 最大値[X,Y,code]
	{	"InspParaM.G2Xrange_1_",		&InspParaM.G2Xrange[1],			R_TYPE_INT		,0	,3	},			// G2 X max-min[X,Y,code]
	{	"InspParaM.G2Xedge_1_",			&InspParaM.G2Xedge[1],			R_TYPE_INT		,0	,3	},			// G2 X エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureXmin_1_",		&InspParaM.G2ZureXmin[1],		R_TYPE_INT		,0	,3	},			// G2 X Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureXmax_1_",		&InspParaM.G2ZureXmax[1],		R_TYPE_INT		,0	,3	},			// G2 X Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2Ymin_1_",			&InspParaM.G2Ymin[1],			R_TYPE_INT		,0	,3	},			// G2 Y size 最小値[X,Y,code]
	{	"InspParaM.G2Ymax_1_",			&InspParaM.G2Ymax[1],			R_TYPE_INT		,0	,3	},			// G2 Y size 最大値[X,Y,code]
	{	"InspParaM.G2Yrange_1_",		&InspParaM.G2Yrange[1],			R_TYPE_INT		,0	,3	},			// G2 Y max-min[X,Y,code]
	{	"InspParaM.G2Yedge_1_",			&InspParaM.G2Yedge[1],			R_TYPE_INT		,0	,3	},			// G2 Y エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureYmin_1_",		&InspParaM.G2ZureYmin[1],		R_TYPE_INT		,0	,3	},			// G2 Y Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureYmax_1_",		&InspParaM.G2ZureYmax[1],		R_TYPE_INT		,0	,3	},			// G2 Y Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2YSdl_1_",			&InspParaM.G2YSdl[1],			R_TYPE_INT		,0	,3	},			// G2 Y SDtoG2検査 判定値[X,Y,code]
	{	"InspParaM.G2YApl_1_",			&InspParaM.G2YApl[1],			R_TYPE_INT		,0	,3	},			// G2 Y SDtoAP検査 判定値[X,Y,code]
	{	"InspParaM.OCoatXsize_1_",		&InspParaM.OCoatXsize[1],		R_TYPE_INT		,0	,3	},			// Overcoat X カケ検査 判定値[X,Y,code]
	{	"InspParaM.OCoatYsize_1_",		&InspParaM.OCoatYsize[1],		R_TYPE_INT		,0	,3	},			// Overcoat Y カケ検査 判定値[X,Y,code]
	{	"InspParaM.OCoatXYsize_1_",		&InspParaM.OCoatXYsize[1],		R_TYPE_INT		,0	,3	},			// Overcoat XYカケ検査 判定値[X,Y,code]
	// --- G2 position
	{	"InspParaM.G2PosXmin_1_",		&InspParaM.G2PosXmin[1],		R_TYPE_INT		,0	,3	},			// G2 X position min[X,Y,code]
	{	"InspParaM.G2PosXmax_1_",		&InspParaM.G2PosXmax[1],		R_TYPE_INT		,0	,3	},			// G2 X position max[X,Y,code]
	{	"InspParaM.G2PosYmin_1_",		&InspParaM.G2PosYmin[1],		R_TYPE_INT		,0	,3	},			// G2 Y position min[X,Y,code]
	{	"InspParaM.G2PosYmax_1_",		&InspParaM.G2PosYmax[1],		R_TYPE_INT		,0	,3	},			// G2 Y position max[X,Y,code]
	// --- Plating adhesion
	{	"InspParaM.Plating_1_",			&InspParaM.Plating[1],			R_TYPE_INT		,0	,3	},			// Plating adhesion size[X,Y,code]
	// --- Chip(Appearence) Insp. 判定値
	{	"InspParaM.ChipXmin_1_",		&InspParaM.ChipXmin[1],			R_TYPE_INT		,0	,3	},			// Chip X size 最小値[X,Y,code]
	{	"InspParaM.ChipXmax_1_",		&InspParaM.ChipXmax[1],			R_TYPE_INT		,0	,3	},			// Chip X size 最大値[X,Y,code]
	{	"InspParaM.ChipXrange_1_",		&InspParaM.ChipXrange[1],		R_TYPE_INT		,0	,3	},			// Chip X max-min[X,Y,code]
	{	"InspParaM.ChipXedge_1_",		&InspParaM.ChipXedge[1],		R_TYPE_INT		,0	,3	},			// Chip X エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.ChipYmin_1_",		&InspParaM.ChipYmin[1],			R_TYPE_INT		,0	,3	},			// Chip Y size 最小値[X,Y,code]
	{	"InspParaM.ChipYmax_1_",		&InspParaM.ChipYmax[1],			R_TYPE_INT		,0	,3	},			// Chip Y size 最大値[X,Y,code]
	{	"InspParaM.ChipYrange_1_",		&InspParaM.ChipYrange[1],		R_TYPE_INT		,0	,3	},			// Chip Y max-min[X,Y,code]
	{	"InspParaM.ChipYedge_1_",		&InspParaM.ChipYedge[1],		R_TYPE_INT		,0	,3	},			// Chip Y エッジカケ検査 判定値[X,Y,code]
	// --- Electrode Insp.
	{	"InspParaM.ElecXmin_1_",		&InspParaM.ElecXmin[1],			R_TYPE_INT		,0	,3	},			// 電極寸法 X最小値[X,Y,code]
	{	"InspParaM.ElecXmax_1_",		&InspParaM.ElecXmax[1],			R_TYPE_INT		,0	,3	},			// 電極寸法 X最大値[X,Y,code]
	{	"InspParaM.ElecXrange_1_",		&InspParaM.ElecXrange[1],		R_TYPE_INT		,0	,3	},			// 電極寸法 X max-min[X,Y,code]
	{	"InspParaM.ElecXedge_1_",		&InspParaM.ElecXedge[1],		R_TYPE_INT		,0	,3	},			// 電極寸法 X エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.ElecYmin_1_",		&InspParaM.ElecYmin[1],			R_TYPE_INT		,0	,3	},			// 電極寸法 Y最小値[X,Y,code]
	{	"InspParaM.ElecYmax_1_",		&InspParaM.ElecYmax[1],			R_TYPE_INT		,0	,3	},			// 電極寸法 Y最大値[X,Y,code]
	{	"InspParaM.ElecYrange_1_",		&InspParaM.ElecYrange[1],		R_TYPE_INT		,0	,3	},			// 電極寸法 Y max-min[X,Y,code]
	{	"InspParaM.ElecYedge_1_",		&InspParaM.ElecYedge[1],		R_TYPE_INT		,0	,3	},			// 電極寸法 Y エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.ElecXsub_1_",		&InspParaM.ElecXsub[1],			R_TYPE_INT		,0	,3	},			// 電極幅左右寸法差[X,Y,code]
	{	"InspParaM.ElecKake_1_",		&InspParaM.ElecKake[1],			R_TYPE_INT		,0	,3	},			// 電極カケ検査判定値[%,nu,code]
	{	"InspParaM.ElecBlack_1_",		&InspParaM.ElecBlack[1],		R_TYPE_INT		,0	,3	},			// 電極変色判定値[%,nu,code]
	{	"InspParaM.ElecC2Xmin_1_",		&InspParaM.ElecC2Xmin[1],		R_TYPE_INT		,0	,3	},			// C2電極被り小[X,Y,code]
	{	"InspParaM.ElecC1Mezumari_1_",	&InspParaM.ElecC1Mezumari[1],	R_TYPE_INT		,0	,3	},			// C1目詰まり[%,nu,code]
	{	"InspParaM.SideElecXmin_1_",	&InspParaM.SideElecXmin[1],		R_TYPE_INT		,0	,3	},			// 側面電極被り込み寸法 X最小値[X,Y,code]
	{	"InspParaM.MarkLevel_1_",		&InspParaM.MarkLevel[1],		R_TYPE_INT		,0	,3	},			//
	{	"InspParaM.MarkShift_1_",		&InspParaM.MarkShift[1],		R_TYPE_INT		,0	,3	},			//
	{	"InspParaM.IbutsuXsize_1_",		&InspParaM.IbutsuXsize[1],		R_TYPE_INT		,0	,3	},			// Glass Ibutsu X 検査 判定値[X,Y,code]
	{	"InspParaM.IbutsuYsize_1_",		&InspParaM.IbutsuYsize[1],		R_TYPE_INT		,0	,3	},			// Glass Ibutsu Y 検査 判定値[X,Y,code]

	// --- G2 Insp. 判定値
	{	"InspParaM.G2Xmin_2_",			&InspParaM.G2Xmin[2],			R_TYPE_INT		,0	,3	},			// G2 X size 最小値[X,Y,code]
	{	"InspParaM.G2Xmax_2_",			&InspParaM.G2Xmax[2],			R_TYPE_INT		,0	,3	},			// G2 X size 最大値[X,Y,code]
	{	"InspParaM.G2Xrange_2_",		&InspParaM.G2Xrange[2],			R_TYPE_INT		,0	,3	},			// G2 X max-min[X,Y,code]
	{	"InspParaM.G2Xedge_2_",			&InspParaM.G2Xedge[2],			R_TYPE_INT		,0	,3	},			// G2 X エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureXmin_2_",		&InspParaM.G2ZureXmin[2],		R_TYPE_INT		,0	,3	},			// G2 X Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureXmax_2_",		&InspParaM.G2ZureXmax[2],		R_TYPE_INT		,0	,3	},			// G2 X Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2Ymin_2_",			&InspParaM.G2Ymin[2],			R_TYPE_INT		,0	,3	},			// G2 Y size 最小値[X,Y,code]
	{	"InspParaM.G2Ymax_2_",			&InspParaM.G2Ymax[2],			R_TYPE_INT		,0	,3	},			// G2 Y size 最大値[X,Y,code]
	{	"InspParaM.G2Yrange_2_",		&InspParaM.G2Yrange[2],			R_TYPE_INT		,0	,3	},			// G2 Y max-min[X,Y,code]
	{	"InspParaM.G2Yedge_2_",			&InspParaM.G2Yedge[2],			R_TYPE_INT		,0	,3	},			// G2 Y エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureYmin_2_",		&InspParaM.G2ZureYmin[2],		R_TYPE_INT		,0	,3	},			// G2 Y Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureYmax_2_",		&InspParaM.G2ZureYmax[2],		R_TYPE_INT		,0	,3	},			// G2 Y Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2YSdl_2_",			&InspParaM.G2YSdl[2],			R_TYPE_INT		,0	,3	},			// G2 Y SDtoG2検査 判定値[X,Y,code]
	{	"InspParaM.G2YApl_2_",			&InspParaM.G2YApl[2],			R_TYPE_INT		,0	,3	},			// G2 Y SDtoAP検査 判定値[X,Y,code]
	{	"InspParaM.OCoatXsize_2_",		&InspParaM.OCoatXsize[2],		R_TYPE_INT		,0	,3	},			// Overcoat X カケ検査 判定値[X,Y,code]
	{	"InspParaM.OCoatYsize_2_",		&InspParaM.OCoatYsize[2],		R_TYPE_INT		,0	,3	},			// Overcoat Y カケ検査 判定値[X,Y,code]
	{	"InspParaM.OCoatXYsize_2_",		&InspParaM.OCoatXYsize[2],		R_TYPE_INT		,0	,3	},			// Overcoat XYカケ検査 判定値[X,Y,code]
	// --- G2 position
	{	"InspParaM.G2PosXmin_2_",		&InspParaM.G2PosXmin[2],		R_TYPE_INT		,0	,3	},			// G2 X position min[X,Y,code]
	{	"InspParaM.G2PosXmax_2_",		&InspParaM.G2PosXmax[2],		R_TYPE_INT		,0	,3	},			// G2 X position max[X,Y,code]
	{	"InspParaM.G2PosYmin_2_",		&InspParaM.G2PosYmin[2],		R_TYPE_INT		,0	,3	},			// G2 Y position min[X,Y,code]
	{	"InspParaM.G2PosYmax_2_",		&InspParaM.G2PosYmax[2],		R_TYPE_INT		,0	,3	},			// G2 Y position max[X,Y,code]
	// --- Plating adhesion
	{	"InspParaM.Plating_2_",			&InspParaM.Plating[2],			R_TYPE_INT		,0	,3	},			// Plating adhesion size[X,Y,code]
	// --- Chip(Appearence) Insp. 判定値
	{	"InspParaM.ChipXmin_2_",		&InspParaM.ChipXmin[2],			R_TYPE_INT		,0	,3	},			// Chip X size 最小値[X,Y,code]
	{	"InspParaM.ChipXmax_2_",		&InspParaM.ChipXmax[2],			R_TYPE_INT		,0	,3	},			// Chip X size 最大値[X,Y,code]
	{	"InspParaM.ChipXrange_2_",		&InspParaM.ChipXrange[2],		R_TYPE_INT		,0	,3	},			// Chip X max-min[X,Y,code]
	{	"InspParaM.ChipXedge_2_",		&InspParaM.ChipXedge[2],		R_TYPE_INT		,0	,3	},			// Chip X エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.ChipYmin_2_",		&InspParaM.ChipYmin[2],			R_TYPE_INT		,0	,3	},			// Chip Y size 最小値[X,Y,code]
	{	"InspParaM.ChipYmax_2_",		&InspParaM.ChipYmax[2],			R_TYPE_INT		,0	,3	},			// Chip Y size 最大値[X,Y,code]
	{	"InspParaM.ChipYrange_2_",		&InspParaM.ChipYrange[2],		R_TYPE_INT		,0	,3	},			// Chip Y max-min[X,Y,code]
	{	"InspParaM.ChipYedge_2_",		&InspParaM.ChipYedge[2],		R_TYPE_INT		,0	,3	},			// Chip Y エッジカケ検査 判定値[X,Y,code]
	// --- Electrode Insp.
	{	"InspParaM.ElecXmin_2_",		&InspParaM.ElecXmin[2],			R_TYPE_INT		,0	,3	},			// 電極寸法 X最小値[X,Y,code]
	{	"InspParaM.ElecXmax_2_",		&InspParaM.ElecXmax[2],			R_TYPE_INT		,0	,3	},			// 電極寸法 X最大値[X,Y,code]
	{	"InspParaM.ElecXrange_2_",		&InspParaM.ElecXrange[2],		R_TYPE_INT		,0	,3	},			// 電極寸法 X max-min[X,Y,code]
	{	"InspParaM.ElecXedge_2_",		&InspParaM.ElecXedge[2],		R_TYPE_INT		,0	,3	},			// 電極寸法 X エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.ElecYmin_2_",		&InspParaM.ElecYmin[2],			R_TYPE_INT		,0	,3	},			// 電極寸法 Y最小値[X,Y,code]
	{	"InspParaM.ElecYmax_2_",		&InspParaM.ElecYmax[2],			R_TYPE_INT		,0	,3	},			// 電極寸法 Y最大値[X,Y,code]
	{	"InspParaM.ElecYrange_2_",		&InspParaM.ElecYrange[2],		R_TYPE_INT		,0	,3	},			// 電極寸法 Y max-min[X,Y,code]
	{	"InspParaM.ElecYedge_2_",		&InspParaM.ElecYedge[2],		R_TYPE_INT		,0	,3	},			// 電極寸法 Y エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.ElecXsub_2_",		&InspParaM.ElecXsub[2],			R_TYPE_INT		,0	,3	},			// 電極幅左右寸法差[X,Y,code]
	{	"InspParaM.ElecKake_2_",		&InspParaM.ElecKake[2],			R_TYPE_INT		,0	,3	},			// 電極カケ検査判定値[%,nu,code]
	{	"InspParaM.ElecBlack_2_",		&InspParaM.ElecBlack[2],		R_TYPE_INT		,0	,3	},			// 電極変色判定値[%,nu,code]
	{	"InspParaM.ElecC2Xmin_2_",		&InspParaM.ElecC2Xmin[2],		R_TYPE_INT		,0	,3	},			// C2電極被り小[X,Y,code]
	{	"InspParaM.ElecC1Mezumari_2_",	&InspParaM.ElecC1Mezumari[2],	R_TYPE_INT		,0	,3	},			// C1目詰まり[%,nu,code]
	{	"InspParaM.SideElecXmin_2_",	&InspParaM.SideElecXmin[2],		R_TYPE_INT		,0	,3	},			// 側面電極被り込み寸法 X最小値[X,Y,code]
	{	"InspParaM.MarkLevel_2_",		&InspParaM.MarkLevel[2],		R_TYPE_INT		,0	,3	},			//
	{	"InspParaM.MarkShift_2_",		&InspParaM.MarkShift[2],		R_TYPE_INT		,0	,3	},			//
	{	"InspParaM.IbutsuXsize_2_",		&InspParaM.IbutsuXsize[2],		R_TYPE_INT		,0	,3	},			// Glass Ibutsu X 検査 判定値[X,Y,code]
	{	"InspParaM.IbutsuYsize_2_",		&InspParaM.IbutsuYsize[2],		R_TYPE_INT		,0	,3	},			// Glass Ibutsu Y 検査 判定値[X,Y,code]

	// --- G2 Insp. 判定値
	{	"InspParaM.G2Xmin_3_",			&InspParaM.G2Xmin[3],			R_TYPE_INT		,0	,3	},			// G2 X size 最小値[X,Y,code]
	{	"InspParaM.G2Xmax_3_",			&InspParaM.G2Xmax[3],			R_TYPE_INT		,0	,3	},			// G2 X size 最大値[X,Y,code]
	{	"InspParaM.G2Xrange_3_",		&InspParaM.G2Xrange[3],			R_TYPE_INT		,0	,3	},			// G2 X max-min[X,Y,code]
	{	"InspParaM.G2Xedge_3_",			&InspParaM.G2Xedge[3],			R_TYPE_INT		,0	,3	},			// G2 X エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureXmin_3_",		&InspParaM.G2ZureXmin[3],		R_TYPE_INT		,0	,3	},			// G2 X Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureXmax_3_",		&InspParaM.G2ZureXmax[3],		R_TYPE_INT		,0	,3	},			// G2 X Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2Ymin_3_",			&InspParaM.G2Ymin[3],			R_TYPE_INT		,0	,3	},			// G2 Y size 最小値[X,Y,code]
	{	"InspParaM.G2Ymax_3_",			&InspParaM.G2Ymax[3],			R_TYPE_INT		,0	,3	},			// G2 Y size 最大値[X,Y,code]
	{	"InspParaM.G2Yrange_3_",		&InspParaM.G2Yrange[3],			R_TYPE_INT		,0	,3	},			// G2 Y max-min[X,Y,code]
	{	"InspParaM.G2Yedge_3_",			&InspParaM.G2Yedge[3],			R_TYPE_INT		,0	,3	},			// G2 Y エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureYmin_3_",		&InspParaM.G2ZureYmin[3],		R_TYPE_INT		,0	,3	},			// G2 Y Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureYmax_3_",		&InspParaM.G2ZureYmax[3],		R_TYPE_INT		,0	,3	},			// G2 Y Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2YSdl_3_",			&InspParaM.G2YSdl[3],			R_TYPE_INT		,0	,3	},			// G2 Y SDtoG2検査 判定値[X,Y,code]
	{	"InspParaM.G2YApl_3_",			&InspParaM.G2YApl[3],			R_TYPE_INT		,0	,3	},			// G2 Y SDtoAP検査 判定値[X,Y,code]
	{	"InspParaM.OCoatXsize_3_",		&InspParaM.OCoatXsize[3],		R_TYPE_INT		,0	,3	},			// Overcoat X カケ検査 判定値[X,Y,code]
	{	"InspParaM.OCoatYsize_3_",		&InspParaM.OCoatYsize[3],		R_TYPE_INT		,0	,3	},			// Overcoat Y カケ検査 判定値[X,Y,code]
	{	"InspParaM.OCoatXYsize_3_",		&InspParaM.OCoatXYsize[3],		R_TYPE_INT		,0	,3	},			// Overcoat XYカケ検査 判定値[X,Y,code]
	// --- G2 position
	{	"InspParaM.G2PosXmin_3_",		&InspParaM.G2PosXmin[3],		R_TYPE_INT		,0	,3	},			// G2 X position min[X,Y,code]
	{	"InspParaM.G2PosXmax_3_",		&InspParaM.G2PosXmax[3],		R_TYPE_INT		,0	,3	},			// G2 X position max[X,Y,code]
	{	"InspParaM.G2PosYmin_3_",		&InspParaM.G2PosYmin[3],		R_TYPE_INT		,0	,3	},			// G2 Y position min[X,Y,code]
	{	"InspParaM.G2PosYmax_3_",		&InspParaM.G2PosYmax[3],		R_TYPE_INT		,0	,3	},			// G2 Y position max[X,Y,code]
	// --- Plating adhesion
	{	"InspParaM.Plating_3_",			&InspParaM.Plating[3],			R_TYPE_INT		,0	,3	},			// Plating adhesion size[X,Y,code]
	// --- Chip(Appearence) Insp. 判定値
	{	"InspParaM.ChipXmin_3_",		&InspParaM.ChipXmin[3],			R_TYPE_INT		,0	,3	},			// Chip X size 最小値[X,Y,code]
	{	"InspParaM.ChipXmax_3_",		&InspParaM.ChipXmax[3],			R_TYPE_INT		,0	,3	},			// Chip X size 最大値[X,Y,code]
	{	"InspParaM.ChipXrange_3_",		&InspParaM.ChipXrange[3],		R_TYPE_INT		,0	,3	},			// Chip X max-min[X,Y,code]
	{	"InspParaM.ChipXedge_3_",		&InspParaM.ChipXedge[3],		R_TYPE_INT		,0	,3	},			// Chip X エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.ChipYmin_3_",		&InspParaM.ChipYmin[3],			R_TYPE_INT		,0	,3	},			// Chip Y size 最小値[X,Y,code]
	{	"InspParaM.ChipYmax_3_",		&InspParaM.ChipYmax[3],			R_TYPE_INT		,0	,3	},			// Chip Y size 最大値[X,Y,code]
	{	"InspParaM.ChipYrange_3_",		&InspParaM.ChipYrange[3],		R_TYPE_INT		,0	,3	},			// Chip Y max-min[X,Y,code]
	{	"InspParaM.ChipYedge_3_",		&InspParaM.ChipYedge[3],		R_TYPE_INT		,0	,3	},			// Chip Y エッジカケ検査 判定値[X,Y,code]
	// --- Electrode Insp.
	{	"InspParaM.ElecXmin_3_",		&InspParaM.ElecXmin[3],			R_TYPE_INT		,0	,3	},			// 電極寸法 X最小値[X,Y,code]
	{	"InspParaM.ElecXmax_3_",		&InspParaM.ElecXmax[3],			R_TYPE_INT		,0	,3	},			// 電極寸法 X最大値[X,Y,code]
	{	"InspParaM.ElecXrange_3_",		&InspParaM.ElecXrange[3],		R_TYPE_INT		,0	,3	},			// 電極寸法 X max-min[X,Y,code]
	{	"InspParaM.ElecXedge_3_",		&InspParaM.ElecXedge[3],		R_TYPE_INT		,0	,3	},			// 電極寸法 X エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.ElecYmin_3_",		&InspParaM.ElecYmin[3],			R_TYPE_INT		,0	,3	},			// 電極寸法 Y最小値[X,Y,code]
	{	"InspParaM.ElecYmax_3_",		&InspParaM.ElecYmax[3],			R_TYPE_INT		,0	,3	},			// 電極寸法 Y最大値[X,Y,code]
	{	"InspParaM.ElecYrange_3_",		&InspParaM.ElecYrange[3],		R_TYPE_INT		,0	,3	},			// 電極寸法 Y max-min[X,Y,code]
	{	"InspParaM.ElecYedge_3_",		&InspParaM.ElecYedge[3],		R_TYPE_INT		,0	,3	},			// 電極寸法 Y エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.ElecXsub_3_",		&InspParaM.ElecXsub[3],			R_TYPE_INT		,0	,3	},			// 電極幅左右寸法差[X,Y,code]
	{	"InspParaM.ElecKake_3_",		&InspParaM.ElecKake[3],			R_TYPE_INT		,0	,3	},			// 電極カケ検査判定値[%,nu,code]
	{	"InspParaM.ElecBlack_3_",		&InspParaM.ElecBlack[3],		R_TYPE_INT		,0	,3	},			// 電極変色判定値[%,nu,code]
	{	"InspParaM.SideElecXmin_3_",	&InspParaM.SideElecXmin[3],		R_TYPE_INT		,0	,3	},			// 側面電極被り込み寸法 X最小値[X,Y,code]
	{	"InspParaM.ElecC2Xmin_3_",		&InspParaM.ElecC2Xmin[3],		R_TYPE_INT		,0	,3	},			// C2電極被り小[X,Y,code]
	{	"InspParaM.ElecC1Mezumari_3_",	&InspParaM.ElecC1Mezumari[3],	R_TYPE_INT		,0	,3	},			// C1目詰まり[%,nu,code]
	{	"InspParaM.MarkLevel_3_",		&InspParaM.MarkLevel[3],		R_TYPE_INT		,0	,3	},			//
	{	"InspParaM.MarkShift_3_",		&InspParaM.MarkShift[3],		R_TYPE_INT		,0	,3	},			//
	{	"InspParaM.IbutsuXsize_3_",		&InspParaM.IbutsuXsize[3],		R_TYPE_INT		,0	,3	},			// Glass Ibutsu X 検査 判定値[X,Y,code]
	{	"InspParaM.IbutsuYsize_3_",		&InspParaM.IbutsuYsize[3],		R_TYPE_INT		,0	,3	},			// Glass Ibutsu Y 検査 判定値[X,Y,code]

	// --- G2 Insp. 判定値
	{	"InspParaM.G2Xmin_4_",			&InspParaM.G2Xmin[4],			R_TYPE_INT		,0	,3	},			// G2 X size 最小値[X,Y,code]
	{	"InspParaM.G2Xmax_4_",			&InspParaM.G2Xmax[4],			R_TYPE_INT		,0	,3	},			// G2 X size 最大値[X,Y,code]
	{	"InspParaM.G2Xrange_4_",		&InspParaM.G2Xrange[4],			R_TYPE_INT		,0	,3	},			// G2 X max-min[X,Y,code]
	{	"InspParaM.G2Xedge_4_",			&InspParaM.G2Xedge[4],			R_TYPE_INT		,0	,3	},			// G2 X エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureXmin_4_",		&InspParaM.G2ZureXmin[4],		R_TYPE_INT		,0	,3	},			// G2 X Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureXmax_4_",		&InspParaM.G2ZureXmax[4],		R_TYPE_INT		,0	,3	},			// G2 X Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2Ymin_4_",			&InspParaM.G2Ymin[4],			R_TYPE_INT		,0	,3	},			// G2 Y size 最小値[X,Y,code]
	{	"InspParaM.G2Ymax_4_",			&InspParaM.G2Ymax[4],			R_TYPE_INT		,0	,3	},			// G2 Y size 最大値[X,Y,code]
	{	"InspParaM.G2Yrange_4_",		&InspParaM.G2Yrange[4],			R_TYPE_INT		,0	,3	},			// G2 Y max-min[X,Y,code]
	{	"InspParaM.G2Yedge_4_",			&InspParaM.G2Yedge[4],			R_TYPE_INT		,0	,3	},			// G2 Y エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureYmin_4_",		&InspParaM.G2ZureYmin[4],		R_TYPE_INT		,0	,3	},			// G2 Y Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2ZureYmax_4_",		&InspParaM.G2ZureYmax[4],		R_TYPE_INT		,0	,3	},			// G2 Y Zure検査 判定値[X,Y,code]
	{	"InspParaM.G2YSdl_4_",			&InspParaM.G2YSdl[4],			R_TYPE_INT		,0	,3	},			// G2 Y SDtoG2検査 判定値[X,Y,code]
	{	"InspParaM.G2YApl_4_",			&InspParaM.G2YApl[4],			R_TYPE_INT		,0	,3	},			// G2 Y SDtoAP検査 判定値[X,Y,code]
	{	"InspParaM.OCoatXsize_4_",		&InspParaM.OCoatXsize[4],		R_TYPE_INT		,0	,3	},			// Overcoat X カケ検査 判定値[X,Y,code]
	{	"InspParaM.OCoatYsize_4_",		&InspParaM.OCoatYsize[4],		R_TYPE_INT		,0	,3	},			// Overcoat Y カケ検査 判定値[X,Y,code]
	{	"InspParaM.OCoatXYsize_4_",		&InspParaM.OCoatXYsize[4],		R_TYPE_INT		,0	,3	},			// Overcoat XYカケ検査 判定値[X,Y,code]
	// --- G2 position
	{	"InspParaM.G2PosXmin_4_",		&InspParaM.G2PosXmin[4],		R_TYPE_INT		,0	,3	},			// G2 X position min[X,Y,code]
	{	"InspParaM.G2PosXmax_4_",		&InspParaM.G2PosXmax[4],		R_TYPE_INT		,0	,3	},			// G2 X position max[X,Y,code]
	{	"InspParaM.G2PosYmin_4_",		&InspParaM.G2PosYmin[4],		R_TYPE_INT		,0	,3	},			// G2 Y position min[X,Y,code]
	{	"InspParaM.G2PosYmax_4_",		&InspParaM.G2PosYmax[4],		R_TYPE_INT		,0	,3	},			// G2 Y position max[X,Y,code]
	// --- Plating adhesion
	{	"InspParaM.Plating_4_",			&InspParaM.Plating[4],			R_TYPE_INT		,0	,3	},			// Plating adhesion size[X,Y,code]
	// --- Chip(Appearence) Insp. 判定値
	{	"InspParaM.ChipXmin_4_",		&InspParaM.ChipXmin[4],			R_TYPE_INT		,0	,3	},			// Chip X size 最小値[X,Y,code]
	{	"InspParaM.ChipXmax_4_",		&InspParaM.ChipXmax[4],			R_TYPE_INT		,0	,3	},			// Chip X size 最大値[X,Y,code]
	{	"InspParaM.ChipXrange_4_",		&InspParaM.ChipXrange[4],		R_TYPE_INT		,0	,3	},			// Chip X max-min[X,Y,code]
	{	"InspParaM.ChipXedge_4_",		&InspParaM.ChipXedge[4],		R_TYPE_INT		,0	,3	},			// Chip X エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.ChipYmin_4_",		&InspParaM.ChipYmin[4],			R_TYPE_INT		,0	,3	},			// Chip Y size 最小値[X,Y,code]
	{	"InspParaM.ChipYmax_4_",		&InspParaM.ChipYmax[4],			R_TYPE_INT		,0	,3	},			// Chip Y size 最大値[X,Y,code]
	{	"InspParaM.ChipYrange_4_",		&InspParaM.ChipYrange[4],		R_TYPE_INT		,0	,3	},			// Chip Y max-min[X,Y,code]
	{	"InspParaM.ChipYedge_4_",		&InspParaM.ChipYedge[4],		R_TYPE_INT		,0	,3	},			// Chip Y エッジカケ検査 判定値[X,Y,code]
	// --- Electrode Insp.
	{	"InspParaM.ElecXmin_4_",		&InspParaM.ElecXmin[4],			R_TYPE_INT		,0	,3	},			// 電極寸法 X最小値[X,Y,code]
	{	"InspParaM.ElecXmax_4_",		&InspParaM.ElecXmax[4],			R_TYPE_INT		,0	,3	},			// 電極寸法 X最大値[X,Y,code]
	{	"InspParaM.ElecXrange_4_",		&InspParaM.ElecXrange[4],		R_TYPE_INT		,0	,3	},			// 電極寸法 X max-min[X,Y,code]
	{	"InspParaM.ElecXedge_4_",		&InspParaM.ElecXedge[4],		R_TYPE_INT		,0	,3	},			// 電極寸法 X エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.ElecYmin_4_",		&InspParaM.ElecYmin[4],			R_TYPE_INT		,0	,3	},			// 電極寸法 Y最小値[X,Y,code]
	{	"InspParaM.ElecYmax_4_",		&InspParaM.ElecYmax[4],			R_TYPE_INT		,0	,3	},			// 電極寸法 Y最大値[X,Y,code]
	{	"InspParaM.ElecYrange_4_",		&InspParaM.ElecYrange[4],		R_TYPE_INT		,0	,3	},			// 電極寸法 Y max-min[X,Y,code]
	{	"InspParaM.ElecYedge_4_",		&InspParaM.ElecYedge[4],		R_TYPE_INT		,0	,3	},			// 電極寸法 Y エッジカケ検査 判定値[X,Y,code]
	{	"InspParaM.ElecXsub_4_",		&InspParaM.ElecXsub[4],			R_TYPE_INT		,0	,3	},			// 電極幅左右寸法差[X,Y,code]
	{	"InspParaM.ElecKake_4_",		&InspParaM.ElecKake[4],			R_TYPE_INT		,0	,3	},			// 電極カケ検査判定値[%,nu,code]
	{	"InspParaM.ElecBlack_4_",		&InspParaM.ElecBlack[4],		R_TYPE_INT		,0	,3	},			// 電極変色判定値[%,nu,code]
	{	"InspParaM.ElecC2Xmin_4_",		&InspParaM.ElecC2Xmin[4],		R_TYPE_INT		,0	,3	},			// C2電極被り小[X,Y,code]
	{	"InspParaM.ElecC1Mezumari_4_",	&InspParaM.ElecC1Mezumari[4],	R_TYPE_INT		,0	,3	},			// C1目詰まり[%,nu,code]
	{	"InspParaM.SideElecXmin_4_",	&InspParaM.SideElecXmin[4],		R_TYPE_INT		,0	,3	},			// 側面電極被り込み寸法 X最小値[X,Y,code]
	{	"InspParaM.MarkLevel_4_",		&InspParaM.MarkLevel[4],		R_TYPE_INT		,0	,3	},			//
	{	"InspParaM.MarkShift_4_",		&InspParaM.MarkShift[4],		R_TYPE_INT		,0	,3	},			//
	{	"InspParaM.IbutsuXsize_4_",		&InspParaM.IbutsuXsize[4],		R_TYPE_INT		,0	,3	},			// Glass Ibutsu X 検査 判定値[X,Y,code]
	{	"InspParaM.IbutsuYsize_4_",		&InspParaM.IbutsuYsize[4],		R_TYPE_INT		,0	,3	},			// Glass Ibutsu Y 検査 判定値[X,Y,code]

	// --- system
	{	"InspParaM.Focus",				&InspParaM.Focus,				R_TYPE_INT		,0	,3	},			// レンズ焦点[d,nu,code]
	{	"InspParaM.Bright",				&InspParaM.Bright,				R_TYPE_INT		,0	,3	},			// 照度管理[d,nu,code]

	{	NULL	},
};


// 製品データ
static char TypeFileName[] = "TP-S_Ty.csv";
R_DATAFILE_ITEM TypeFileData[] = {
	// --- パラメータ確認項目（不一致の場合は、エラー停止）
	{	"TyFile.Prod",					&TyFile.Prod,					R_TYPE_STRING	,32		},
	{	"TyFile.Insp",					&TyFile.Insp,					R_TYPE_STRING	,32		},
	{	"TyFile.Ver",					&TyFile.Ver,					R_TYPE_STRING	,32		},
	// ---
//	{	"InspValue",					&InspValue,						R_TYPE_INT				},
//	{	"MarkChar",						&MarkChar,						R_TYPE_STRING	,10		},
	{	"InspClass",					&InspClass,						R_TYPE_INT				},
	{	"SpecType",						&SpecType,						R_TYPE_STRING   ,3	    },

	{	NULL	},
};



static	int				ProjectBuf[2560];									// projectデーターを格納するバッファー
static	int				ProjectBuf1[2560];									// projectデーターを格納するバッファー
static	int				ProjectBuf2[2560];									// projectデーターを格納するバッファー

        char			Comment[2048];                                      // Comment Buffer
        char			Comment1[200];										// Comment Buffer
        char			Comment2[200];										// Comment Buffer

MEASDATA Mpx;

        int              Center_C;                                           //GEE V1.02A

R_CS_COUNTER_NG_CODE NgCode1[] = {
	// Insp. Error
	{ NG_NO_WORK,				"NO Work   ",			"no work",					COUNTER_CLASS1 | COUNTER_NO_INC	},	// 1
	{ NG_CANT_INSP,				"Can'tInsp.",			"Can'n Insp.",				COUNTER_CLASS1 	},					// 2
	{ NG_INDEX_POS,				"Index Pos.",			"Index pos. Error",			COUNTER_CLASS1 | COUNTER_NO_INC	},	// 3
	{ NG_TAPE_POS,				"Tape Pos. ",			"Tape pos. Error",			COUNTER_CLASS1 | COUNTER_NO_INC	},	// 4
	{ NG_REVERSE,				"Reverse   ",			"Chip reverse",				COUNTER_CLASS1 | COUNTER_NO_INC	},	// 5
	{ NG_FOCUS,					"Focus     ",			"Focus Error",				COUNTER_CLASS1 | COUNTER_NO_INC	},	// 6
	{ NG_GLASS_IBUTSU,			"Glass IBUTSU",			"IBUTSU  Error",			COUNTER_CLASS9 | COUNTER_NO_INC },	// 58
	{ NG_LIGHTING,				"LightingEr",			"Lighting Error",			COUNTER_CLASS9 | COUNTER_NO_INC	},	// 97
	{ NG_CAPTURE,				"Capture Er",			"Capture Error",			COUNTER_CLASS9 | COUNTER_NO_INC	},	// 98
	{ NG_INSP,					"Inspect Er",			"Inspect Error",			COUNTER_CLASS9 | COUNTER_NO_INC },	// 99
	// G2 Insp.
	{ NG_G2_X_SIZE,				"G2 X size ",			"G2 X size",				COUNTER_CLASS2	},					// 11
	{ NG_G2_Y_SIZE,				"G2 Y size ",			"G2 Y size",				COUNTER_CLASS2	},					// 12
	{ NG_G2_KAKE_X,				"G2 Kake X ",			"G2 Kake X",				COUNTER_CLASS2	},					// 13
	{ NG_G2_KAKE_Y,				"G2 Kake Y ",			"G2 Kake Y",				COUNTER_CLASS2	},					// 14
	{ NG_G2_POS_X,				"G2 Pos. X ",			"G2 Pos. X",				COUNTER_CLASS2	},					// 15
	{ NG_G2_POS_Y,				"G2 Pos. Y ",			"G2 Pos. Y",				COUNTER_CLASS2	},					// 16
	{ NG_OCOAT_X,				"Overcoat X",			"Overcoat kake X",			COUNTER_CLASS2	},					// 17
	{ NG_OCOAT_Y,				"Overcoat Y",			"Overcoat kake Y",			COUNTER_CLASS2	},					// 18
	{ NG_OCOAT_XY,				"OvercoatXY",			"Overcoat kake XY",			COUNTER_CLASS2	},					// 19
	// Chip(Appearence) Insp.
	{ NG_CHIP_X_SIZE,			"Chip Xsize",			"Chip Xsize",				COUNTER_CLASS3	},					// 21
	{ NG_CHIP_Y_SIZE,			"Chip Ysize",			"Chip Ysize",				COUNTER_CLASS3	},					// 22
	{ NG_CHIP_KAKE_X,			"Chip KakeX",			"Chip Kake X",				COUNTER_CLASS3	},					// 23
	{ NG_CHIP_KAKE_Y,			"Chip KakeY",			"Chip Kake Y",				COUNTER_CLASS3	},					// 24
	{ NG_CHIP_CORNER,			"ChipCorner",			"Chip Kake corner",			COUNTER_CLASS3	},					// 25
	// Plating adhesion
	{ NG_PL_ADHESION,			"Plating   ",			"Plating adhesion",			COUNTER_CLASS4	},					// 31
	// Marking Insp.
	{ NG_MARKING,				"MarkingErr",			"Marking Error",			COUNTER_CLASS5	},					// 41
	{ NG_MARK_SEGMENT,			"M segment ",			"Marking segment",			COUNTER_CLASS5	},					// 42
	// Electrode Insp.
	{ NG_ELEC_X_SIZE,			"Elec.Xsize",			"Elec. X size",				COUNTER_CLASS6	},					// 51
	{ NG_ELEC_Y_SIZE,			"Elec.Ysize",			"Elec. Y size",				COUNTER_CLASS6	},					// 52
	{ NG_ELEC_EDGE_X,			"Elec.edgeX",			"Elec. edge X",				COUNTER_CLASS6	},					// 53
	{ NG_ELEC_EDGE_Y,			"Elec.edgeY",			"Elec. edge Y",				COUNTER_CLASS6	},					// 54
	{ NG_ELEC_KAKE,				"Elec.kake ",			"Elec. Kake",				COUNTER_CLASS6	},					// 55
	{ NG_ELEC_BLACK,			"Elec.Black",			"Elec. Black",				COUNTER_CLASS6	},					// 56
	{ NG_SIDE_ELEC,				"Side Elec.",			"Side Elec. X size",		COUNTER_CLASS6	},					// 57
	// M/C NG box
	{ MC_RESISTANCE,			"M/C NG [R]",			"M/C NG [Resistance]",		COUNTER_CLASS18 | COUNTER_NO_INC	},	// 81
	{ MC_NINSHIKI,				"M/C NG [N]",			"M/C NG [Ninshiki]",		COUNTER_CLASS19 | COUNTER_NO_INC	},	// 82
	{ NG_MARKING,				"Marking NG",			"Marking NG",				COUNTER_CLASS7	},	// 77

	{ 0,						NULL,					NULL,						0				},
};

//2012.03.14
extern int                      insp_sum1[20];
extern int		   			    count_total[20], count_R[20], count_N[20], count_L[20], count_Re[20];
extern int                      pass_sum[20];                                               // Count Go
extern int                      count1[20][100];

static int                      Clear_flag = 0;
static int                      Work_count=0;             //2011.08.17
static int                      NG_Limit[2][100];         //2011.08.17
int                      		Lot_num = 0;
int                             Poket_flag = 0;           //2012.04.19
FILE *fin1;
//int                             PilotFlag;
//static char Charactor_drive[25] = {'b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
//static char Charactor_drive[25] = {b,c,d,e,f,g,h,i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

/**********************************************************
	関数名：	ＲＯＨＭ
	機  能：	ＲＯＨＭメイン関数
	形  式：	int ROHM( int argc, char *argv[] )
	引き数：	int argc
				char *argv[]
	戻り値：	OK		正常終了
				ERROR	異常終了
	解  説：
	留意事項：
**********************************************************/

int		ROHM( int argc, char *argv[] )
	{
		int						dn;

		// Option
//			for( dn=1; dn<=argc; dn++ ) {
			for( dn=1; dn<argc; dn++ ) {
				switch( GetOption( argv[dn] ) ) {
					case OK:
						break;
					case ERROR:
						return OK;
					default:
						return ERROR;
				}
			}
		// 初期化
			if(OK!=Initial()) return ERROR;

		// R_CSシステム起動
			if(OK!=R_CS(argc,argv))		return ERROR;

		// プログラム終了
			ProgExit();

		return OK;

	}


/**********************************************************
	関数名：	ＧｅｔＯｐｔｉｏｎ
	機  能：	オプション取得
	形  式：	static int GetOption(char *opt)
	引き数：
	戻り値：	OK		正常終了
				ERROR	異常終了
	解  説：
	留意事項：
**********************************************************/

static int	GetOption( char *opt )
	{

		char StringWork[4096];						// char型データ作業エリア
		while( *opt++ == '/' ) {
			switch( *opt++ ) {
				case '?':
				case 'h':
				case 'H': //ヘルプ
					Ussage();
					return ERROR;
				case 'a':
				case 'A':
					if( strncmp( opt, "dmin", 4 )==0 )	Option |= ADMIN_MODE;
					if( strncmp( opt, "DMIN", 4 )==0 )	Option |= ADMIN_MODE;
					break;
				case 'b':
				case 'B':
					if( (strncmp( opt, "ack", 3 )==0  || strncmp( opt, "ACK", 3 )==0) ){
						Option |= BACKINSP_MODE;
						strcpy(InspectionName, "TP-Back");
					}
					break;
				case 'c':
				case 'C':
					if (0 == strncmp(opt, "om=", 3)) {
						if (4 == strlen(opt)) {
							int	c = opt[3];
							if (('0' <= c) && (c <= '3')) {
								CommunicationDeviceShareId = c - '0';
							}
						}if (5 < strlen(opt)){
							int i = 0;
							char *s2nd;
							strcpy(StringWork,opt + 3);
							CommunicationDeviceShareId = 999;
							while(1){
								if(i < strlen(StringWork)){
									if(StringWork[i] == ','){
										StringWork[i] = '\0';
										s2nd = StringWork + i + 1;
										break;
									}
								}else{
									return ERROR;
								}
								i++;
							}
							CommunicationDeviceStartBit = atoi(StringWork);
							CommunicationDeviceUseLen = atoi(s2nd);
						}
					}
					break;
				case 'd':
				case 'D':
					Option |= DEBUG_MODE;
//					SetDebugMode(ON);
					break;
				case 's':
				case 'S':
//					Option |= SETTING_MODE;
					break;
				case 'm':
				case 'M':
					if( strncmp( opt, "echa", 4 )==0 ){
						Option |= MECHA_CHECK;
					}
					break;
				case 'n':
				case 'N':
					if( strncmp( opt, "80", 2 )==0 )	Option |= NO_DRCY80;	// DR-CY-93なし
//					if( strncmp( opt, "83", 2 )==0 )	Option |= NO_DRCY83;	// DR-CY-93なし
//					if( strncmp( opt, "84", 2 )==0 )	Option |= NO_INCY84;	// IN-CY-84なし
//					if( strncmp( opt, "76", 2 )==0 )	Option |= NO_INCY76;	// <shio> recovery
					if( strncmp( opt, "119", 2 )==0 )	Option |= NO_INCY119; //<z1>
					if( strncmp( opt, "okey",4)==0 )	Option |= NO_USBKEY;	// key check しない
					if( strncmp( opt, "itto",4)==0 ){
						Option |= NO_DRCY80;
						//<z1> Option |= NO_INCY76;
						Option |= NO_INCY119; //<z1>
						Option |= IO_INCY122;
						LedControllMode=LED_CONTROLL_DRX124;
						bIsNitto = 1;
						WinbSettingItem = WinbSettingItem_Nitto;
					}
					if(0==strncmp(opt,"124", 3))		Option |= NO_DRX124;	// DR-X-124なし
					break;
				case '1':	// "124=?"	DR-X-124のID指定	2012/03/29 by sassa
					if(     0==strncmp(opt,"24=", 3)){
						switch(opt[3]){
							case '0':	DrX124Ch = _BOARD_JPSW0;	break;
							case '1':	DrX124Ch = _BOARD_JPSW1;	break;
							case '2':	DrX124Ch = _BOARD_JPSW2;	break;
							case '3':	DrX124Ch = _BOARD_JPSW3;	break;
						}
					}
					break;
				case 'r':
                case 'R':
                    if( strncmp( opt, "epi", 3 )==0 || strncmp( opt, "EPI", 3 )==0 ){
						Option |= REPI;
						WinbSettingItem = WinbSettingItem_repi;
					}
                    break;
                default:
					printf("\x07\nillegal option!!\n");
					Ussage();
					return ERROR2;
			}
		}

		return OK;

	}	// GetOption( char *opt ) end


/**********************************************************
	関数名：	Ｕｓｓａｇｅ
	機  能：	オプション表示
	形  式：	static void ussage(void)
	引き数：	なし
	戻り値：	なし
	解  説：	オプションコマンドを表示する
	留意事項：
**********************************************************/

static void Ussage(void)
	{
		printf( "\n space & next option !!");
		printf( "\n   /admin administrator mode");
		printf( "\n   /N80   DR-CY-80    (I/O Board) is not checked");
//		printf( "\n   /N83   DR-CY-83/93 (Constant Current Board) is not checked.");
//		printf( "\n   /N84   IN-CY-84    (Differential I/O Board) is not checked.");
		printf( "\n   /D     Debug Mode (Display debug info.)");
		printf( "\n   /S     Setting Mode");
		printf( "\n   /H     Help (This Message)");
	}


/**********************************************************
	関数名：	Ｉｎｉｔｉａｌ
	機  能：	初期化
	形  式：	int Initial(void)
	引き数：	なし
	戻り値：	OK		正常終了
				ERROR	異常終了
	解  説：
	留意事項：	R_CSライブラリの動作設定など最低限の初期化を行います
				基本的にはR_CS_User_Initial()に記述します
				※画像解像度等はR_CS起動後に設定されます
**********************************************************/

static int	Initial(void)
	{
		int						res;

		// R_Draw系関数オープン
			R_DrawOpen();

		// ボード初期化
			if(OK!=(res=InitialBoard()))	return res;
			DrawMessage( " board initialize complete ", DRAW_GREEN, 0, 0 );

		// R_CSライブラリの動作設定
			R_CS_InformationMode(ON);						// 情報表示モードON
			R_CS_InitialTime(OFF);							// 時間初期化しない
			R_LedAdjustFocusMode(ON);						// 照明調整にピント評価値表示追加

		// パラメータ等ファイル設定
			R_CS_SetToolConfigFileName("cs_tool.csv");							// ツール機能設定ファイル
			R_CS_SetParameterFileSystem(SystemFileData, SystemFileName);		// 機台データファイル
			R_CS_SetParameterFilePackage(JudgeFileData, JudgeFilename);			// 機種データファイル（判定値）
			R_CS_SetParameterFileType(TypeFileData, TypeFileName);				// 機種データファイル（製品データ）

			R_datafile_load(SystemFileData, SystemFileName);					// 機台データファイル
			strcpy(ProductName,SyFile.Prod);									// 20170113 tanaka システムファイルの読み込み品種名を品種名とする
			bIsG2Separate = 0;
			bIsG2Straight = 0;
			if(strncmp(ProductName,"MCR03",5)==0){
				bIsMCR03 = 1;           // 20170204 tanaka 機種限定
				bIsG2Straight = 1;		// 20170808 tanaka defaultの形状を設定
			} else if(strncmp(ProductName,"MCR10",5)==0){
				bIsMCR10 = 1;
				bIsG2Separate = 1;		// 20170808 tanaka defaultの形状を設定
			} else if(strncmp(ProductName,"ESR10",5)==0){
				bIsMCR10 = 1;
				bIsG2Separate = 1;		// 20170808 tanaka defaultの形状を設定
			} else if(strncmp(ProductName,"MCR18",5)==0){
				bIsMCR18 = 1;
				bIsG2Separate = 1;		// 20170808 tanaka defaultの形状を設定
			} else if(strncmp(ProductName,"ESR18",5)==0){
				bIsMCR18 = 1;
				bIsG2Separate = 1;		// 20170808 tanaka defaultの形状を設定
			} else if(strncmp(ProductName,"MCR01",5)==0){
				bIsMCR01 = 1;
				bIsG2Straight = 1;		// 20170808 tanaka defaultの形状を設定
			} else if(strncmp(ProductName,"ESR25",5)==0){
				bIsESR25 = 1;
				bIsG2Separate = 1;		// 20170808 tanaka defaultの形状を設定
			} else if(strncmp(ProductName,"SFR25",5)==0){
				bIsSFR25 = 1;
				if((Option & BACKINSP_MODE)){
					bIsG2Straight = 1;
				}else{
					bIsG2Separate = 1;		// 20170808 tanaka defaultの形状を設定
				}
			} else if(strncmp(ProductName,"LTR50",5)==0 || strncmp(ProductName,"LTR50L",6)==0){
				bIsLTR50 = 1;
				if(0!=(Option & BACKINSP_MODE)){									//v2.11
					bIsG2Straight = 1;
				}else{
					bIsG2Separate = 1;
				}
			} else if(strncmp(ProductName,"LTR18",5)==0 || strncmp(ProductName,"LTR18L",6)==0){					//v2.20
				bIsLTR18 = 1;
				if(0!=(Option & BACKINSP_MODE)){
					bIsG2Straight = 1;
				}else{
					bIsG2Separate = 1;
				}
			} else{
				sprintf(Comment,"Invalid product name");
				DrawMessage( Comment, DRAW_RED, 2, 2 );
				return ERROR;
			}
			R_datafile_load(JudgeFileData, JudgeFilename);						// 機種データファイル（判定値）
			R_datafile_load(TypeFileData, TypeFileName);						// 機種データファイル（製品データ）
			if( OK != CheckParameterFile() )	return ERROR;
			if( OK != SetsubiDataLoadfile() )	return ERROR;					// 設備データ読み込み
			if( OK != NGLimitDataLoadfile() )	return ERROR;					// 2011.08.17

		// ユーザー初期化関数設定
			R_CS_SetFunctionUserInitial(InitialUserOnR_CS);
			R_CS_SetFunctionUserInitialLast(InitialUserOnR_CSLast);

		// メニュー関数の登録
			R_CS_SetFunctionManualCapture(CaptureManual);
			R_CS_SetFunctionCaptureThrough(CaptureThrough);
			R_CS_SetFunctionAutoModeReady(AutoModeReady);
			R_CS_SetFunctionAutoModeClosing(AutoModeClosing);
			if((Option & IO_INCY122) ==0){
				R_CS_SetFunctionAutoModeExecute(AutoModeExecute);
			} else {
				R_CS_SetFunctionAutoModeExecute(AutoModeExecuteNwt);
			}
			R_CS_SetFunctionInspect(InspectManual);
			if((1==bIsNitto)||(0==(Option & BACKINSP_MODE))){		//v2.11 日東の時or裏面じゃない時
				R_CS_SetFunctionUserLanExecute(AutoTypeChangeLanUser);
			}
		// ユーザ設定メニューの設定
			R_CS_SetMenuUserSetting(WinbSettingItem);
			R_CS_SetMenuUserFile(WinbFileItem);
			R_CS_SetFunctionManualInspectFileAllReady(ManualInspectFileAllReady);
			Pilot_mode = OFF;													// パイロットモード OFF
			OutputPilotOff();

		return OK;

	}	// Initial(void) end


/**********************************************************
	関数名：	ＩｎｉｔｉａｌＢｏａｒｄ
	機  能：	ボード初期化
	形  式：	static int InitialBoard(void)
	引き数：	無し
	戻り値：	OK		正常終了
				ERROR	異常終了
	解  説：	各種ボードを初期化する
	留意事項：
**********************************************************/

static int	InitialBoard( void )
	{

		// DR-X-124初期化
		if(0 == (Option&NO_DRX124) && (LedControllMode == LED_CONTROLL_DRX124 )){
				if(0>(HandleDrx124=R_drx124_open(DrX124Ch))){	// ハンドルなので、0以上ならオープン成功
					printf("DR-X-124/136 open error!\n");
					R_DrawPopupMessage(POPUP_WAIT_CLICK, "DR-X-124/136 open error!");
					return ERROR;
				}
				_Drx124_Setting = &Drx124_Setting_Cam1_Cap1;
				// 照明調整時の濃度チェックエリア
				R_rect_init_by_point(&rectLedCheckArea[0],  10, 200,  60, 250);
				R_rect_init_by_point(&rectLedCheckArea[1],  70, 200, 120, 250);
				R_rect_init_by_point(&rectLedCheckArea[2], 130, 200, 180, 250);
				R_rect_init_by_point(&rectLedCheckArea[3], 190, 200, 240, 250);
		} else {
			printf("*I* DR-X-124 no check\n");
		}

		// I/O board initialize (DR-CY-80)
			if( Option & NO_DRCY80 ) {
				sprintf( Comment, "DR-CY-80 is not checked" );
				DrawMessage( Comment, DRAW_YELLOW, 1, 0 );
			} else if( InitDRCY80() != OK ) {
				return ERROR;
			}

	// I/O board initialize (IN-CY-76)or(IN-CY-119)     2021.06.10<shio>
		//	if( (Option & NO_INCY76) && (Option & NO_INCY119)){
					if(Option & NO_INCY119){ //11911
				sprintf( Comment, "IN-CY-119 is not checked" );
				DrawMessage( Comment, DRAW_YELLOW, 1, 0 );
			}
			else {
				//InitINCY76();
				InitINCY119(); //<z1>
			}

		// I/O board initialize (DR-CY-80)
			if( Option & IO_INCY122 ) {
				if( InitINCY122() != OK ) {
					return ERROR;
				}
			}
			return OK;

}	// InitialBoard( void ) end


//------------------------------------------------
//		Initial I/O DR-CY-80
//------------------------------------------------

int		InitDRCY80( void )
	{
		int						ch;
		int						dsw = 0;
		int						res;
		int						d_mode, d_color;
//struct gpio_info*	info;

		if(!r_system_is_virtual_io()){
		// initialze
			Drcy80Ch = ERROR;
		// DR-CY-80 初期化
			for( ch=0x00000; ch!=0x10000; ch+=0x01000, dsw++ ) {
				if( R_gpio_open(ch)==OK ) {
					if((nGpioPort[(ch>>12)&0xf]>>24) == PCI_GPIO_DRCY80) {
//					if((nGpioPort[(ch>>12)&0xf]>>24) == PCI_GPIO_DRCY83) {
						Drcy80Ch = ch;
//						info = r_gpio_info((ch>>12)&0xf);
//						info->with_gpdac=0;
//						R_gpdac_open(ch);
						break;
					}
				}
			}
		// result
			if( Drcy80Ch == ERROR ) {
				sprintf( Comment, "Can not find board [DR-CY-80]" );
				d_color = DRAW_RED;
				d_mode = 2;
				res = ERROR;
			} else if( Drcy80Ch != GPDAC_DSW ) {
				sprintf( Comment1, "Setting of a [DR-CY-80 DSW:%X] is wrong.", dsw );
				sprintf( Comment2, "Turn setting of a [DR-CY-80 DSW] into [%X]", GPDAC_DSW_Val );
				sprintf( Comment, "%s\n%s", Comment1, Comment2 );
				d_color = DRAW_RED;
				d_mode = 2;
				res = ERROR2;
			} else {
		//		R_CS_IOCheckRegisterBoard( Drcy80Ch, -1, "DR-CY-80" );	// I/Oチェック用にボード登録
				sprintf( Comment, " DR-CY-80 DSW:%X ", dsw );
				d_color = DRAW_GREEN;
				d_mode = 1;
				res = OK;
			}
			DrawMessage( Comment, d_color, d_mode, 0 );
		} else {
			Drcy80Ch = 0;
//			nGpioPort[0] = 0 | (PCI_GPIO_DRCY80 << 24);
			res = OK;
		}
		return( res );

	}	// InitDRCY80( void ) end


//------------------------------------------------
//		Initial I/O DR-CY-80
//------------------------------------------------

int		InitINCY76( void )
	{
		int						ch;
		int						dsw = 0;
		int						res;
		int						d_mode, d_color;

		// initialze
			Incy76Ch = ERROR;
		// IN-CY-76 初期化
			//for( ch=0x00000; ch!=0x10000; ch+=0x01000, dsw++ ) {
			{
				ch=0x2000;
				if( R_gpio_open(ch)==OK ) {
					if((nGpioPort[(ch>>12)&0xf]>>24) == PCI_GPIO_INCY76) {
						Incy76Ch = ch;
			//			break;
					}
				}
			}
		// result
			if( Incy76Ch == ERROR ) {
				sprintf( Comment, "Cannot find board [IN-CY-76]\nCannot print Data!!" );
				d_color = DRAW_YELLOW;
				d_mode = 2;
				res = ERROR;
			} else {
	//			R_CS_IOCheckRegisterBoard( Incy76Ch, -1, "IN-CY-76" );	// I/Oチェック用にボード登録
				sprintf( Comment, " IN-CY-76 DSW:%X ", dsw );
				d_color = DRAW_GREEN;
				d_mode = 1;
				res = OK;
			}
			DrawMessage( Comment, d_color, d_mode, 0 );

			return( res );

	}	// InitINCY76( void ) end

//------------------------------------------------
//		Initial I/O IN-CY-122
//------------------------------------------------


int		InitINCY119( void )
		{
			int						ch;
			int						dsw = 0;
			int						res;
			int						d_mode, d_color;

			// initialze
				Incy119Ch = ERROR;
			// IN-CY-119 初期化
				//for( ch=0x00000; ch!=0x10000; ch+=0x01000, dsw++ ) {
				{
					ch=0x2000;
					if( R_gpio_open(ch)==OK ) {
						if((nGpioPort[(ch>>12)&0xf]>>24) == PCI_GPIO_INCY119) {
							Incy119Ch = ch;
				//			break;
						}
					}
				}
			// result
				if( Incy119Ch == ERROR ) {
					sprintf( Comment, "Can not find board [IN-CY-119]\nCan not a data print !!" );
					d_color = DRAW_YELLOW;
					d_mode = 2;
					res = ERROR;
				} else {
		//			R_CS_IOCheckRegisterBoard( Incy76Ch, -1, "IN-CY-76" );	// I/Oチェック用にボード登録
					sprintf( Comment, " IN-CY-119 DSW:%X ", dsw );
					d_color = DRAW_GREEN;
					d_mode = 1;
					res = OK;
				}
				DrawMessage( Comment, d_color, d_mode, 0 );

				return( res );

		}	// InitINCY119( void ) end
	//<----<z1> End ---->
//------------------------------------------------
//		Initial I/O IN-CY-122
//------------------------------------------------


int		InitINCY122( void )
	{
		int	mask;

		GpioInPort	= DrCy83Ch | CNa_IN_port_1;
		GpioOutPort	= DrCy83Ch | CNa_OUT_port_1;

		{
			if (OK != R_gpio_open(DrCy83Ch)) {	// 基板IDは、DR-CY-83設定と同じ変数を使う
				char*	string = "GPIO board open error!";
				printf("ERROR: %s\n", string);
				R_DrawPopupMessage(POPUP_WAIT_CLICK, string);
				return ERROR;
			}
			if(!r_system_is_virtual_io()){
				int	type = R_gpio_board_type(DrCy83Ch);
				if ((PCI_GPIO_PCI2724C != type) && (PCI_GPIO_INCY122 != type)) {
					char*	string = "GPIO board is not PCI-2724 series!";
					printf("ERROR: %s\n", string);
					R_DrawPopupMessage(POPUP_WAIT_CLICK, string);
					return ERROR;
				}
			}
			if(CommunicationDeviceShareId == 999){
				int i;
				mask = 0x00;
				for(i = 0; i < CommunicationDeviceUseLen; i++,mask++){
					mask<<=1;
				}
				mask <<= CommunicationDeviceStartBit;

			}else{
				mask = 0xFFFF;
				mask <<= CommunicationDeviceShareId * 8;
			}
			if (0 != R_gpio_set_writable_bits(DrCy83Ch, mask)) {
				char*	string = "GPIO board setting error!";
				printf("ERROR: %s\n", string);
				R_DrawPopupMessage(POPUP_WAIT_CLICK, string);
				return ERROR;
			}
			GpioInPort	= DrCy83Ch | (CNa_IN_port_1 + CommunicationDeviceShareId);
			GpioOutPort	= DrCy83Ch | (CNa_OUT_port_1 + CommunicationDeviceShareId);
		}
			return( OK );
	}	// InitINCY122( void ) end

//================================================
//	Check Parameter
//================================================

int		CheckParameterFile( void )
	{
		int						ans = OK;

		// 機台データファイル
			if( 0 != strcmp( ProductName,		SyFile.Prod ) )	ans = ERROR;
			if( 0 != strcmp( InspectionName,	SyFile.Insp ) )	ans = ERROR;
			if( 0 != strcmp( ProgramVer,		SyFile.Ver ) )	ans = ERROR;
			if( OK != ans ) {
				sprintf( Comment, "System File[%s] ERROR\n[%s][%s][%s]\n[%s][%s][%s]",
									SystemFileName, SyFile.Prod, SyFile.Insp, SyFile.Ver, ProductName, InspectionName, ProgramVer );
				DrawMessage( Comment, DRAW_RED, 2, 0 );
				return ERROR;
			}
		// 機種データファイル（判定値）
			if( 0 != strcmp( ProductName,		JdFile.Prod ) )	ans = ERROR;
			if( 0 != strcmp( InspectionName,	JdFile.Insp ) )	ans = ERROR;
			if( 0 != strcmp( ProgramVer,		JdFile.Ver ) )	ans = ERROR;
			if( OK != ans ) {
				sprintf( Comment, "Judge File[%s] ERROR\n[%s][%s][%s]\n[%s][%s][%s]",
									JudgeFilename, JdFile.Prod, JdFile.Insp, JdFile.Ver, ProductName, InspectionName, ProgramVer );
				DrawMessage( Comment, DRAW_RED, 2, 0 );
				return ERROR;
			}
		// 機種データファイル（製品データ）
			if( 0 != strcmp( ProductName,		TyFile.Prod ) )	ans = ERROR;
			if( 0 != strcmp( InspectionName,	TyFile.Insp ) )	ans = ERROR;
			if( 0 != strcmp( ProgramVer,		TyFile.Ver ) )	ans = ERROR;
			if( OK != ans ) {
				sprintf( Comment, "Type File[%s] ERROR\n[%s][%s][%s]\n[%s][%s][%s]",
									TypeFileName, TyFile.Prod, TyFile.Insp, TyFile.Ver, ProductName, InspectionName, ProgramVer );
				DrawMessage( Comment, DRAW_RED, 2, 0 );
				return ERROR;
			}
		// パラメータ確認項目一致
			return(ans);

	}


/********************************************************************
	関数名：	ＩｎｉｔｉａｌＵｓｅｒＯｎＲ＿ＣＳ
	機  能：	R_CS関数初期設定
	形  式：	int InitialUserOnR_CS(void)
	引き数：	なし
	戻り値：	OK		正常終了
				ERROR	異常終了
	解  説：
	留意事項：	※ R_CSへの登録方法
				R_CS_SetFunctionUserInitial(InitialUserOnR_CS);
**********************************************************************/
int InitialUserOnR_CS(void)
	{
		int						res;
		int						pn;

		// バージョン表示モードを設定
			sprintf( Comment, "Ver. %s", ProgramVer );
			if(!(Option & MECHA_CHECK))		R_CS_SetDispVersionString( Comment );
			else							R_CS_SetDispVersionString( "ADJUST");
			R_CS_SetDispVersionMode( R_CS_VER_USER );

		// 検査情報		※カメラ関係の設定の前に実行する
//			sprintf( Comment, "%s[%s%s%s]%s", ProductName, MarkChar, DataClass[InspClass], PilotDisp[PilotFlag], MachineName );
			sprintf( Comment, "%s[%s ]%s", ProductName, MarkChar,  MachineName );
			res = R_CS_SetInspectionInfo(1, 1, 1, 1, 1, 1, InspectionName, Comment);
			if(res!=OK)		return ERROR;

		// カメラ関連設定
			if (CAM_ACA2000KM == SystemPara.nCameraName){
				int	result = R_video_init(CAM_ACA2000KM, VM_640_480_EXT, SHUTTER, 0);
				if (OK != result) {
					printf(" *E* R_video_init(CAM_ACA2000KM) [%d]\n", result);
					return ERROR;
				}
				res = R_CS_SetCameraPara(CAM_ACA2000KM, VM_AOI);
				if(res!=OK){
					printf(" *E* R_CS_SetCameraPara(CAM_ACA2000KM) [%d]:", res);
					switch(res){
						case ERROR:		printf(" Camera Intialize\n");	break;
						case ERROR2:	printf(" Camera Change\n");		break;
					}
					return ERROR;
				}
			} else {
				res = R_CS_SetCameraPara(CAM_KPF31PCL, VM_640_480);
				if(res!=OK){
					printf(" *E* R_CS_SetCameraPara [%d]:", res);
					switch(res){
						case ERROR:		printf(" Camera Intialize\n");	break;
						case ERROR2:	printf(" Camera Change\n");		break;
					}
					return ERROR;
				}
			}

		// Gray address & Bin address set
			if( R_video_init_memory( 11, 11, RESO_DEFAULT ) != OK ) {
				strcpy( Comment, " *** Gray & Bin Memory Error !! ***" );
				DrawMessage( Comment, DRAW_RED, 2, 0 );
				return ERROR;
			}
            InspGray3Pn	= BASE3_GRAYADDRESS;
            InspGray4Pn	= BASE4_GRAYADDRESS;


        // メモリ初期化
			for( pn=0; pn<10; pn++ ) {
				GrayClsPn( pn, 0, 0, FxSize-1, FySize-1 );
				BinClsPn( pn, 0, 0, FxSize-1, FySize-1 );
			}

		// シャッタスピードの設定
			res = R_CS_SetShutterSpeed(0, 0, SystemPara.nShutterSpeed);
			if(res!=OK)		return ERROR2;

		// 取り込み画像の設定
///			if(OK!=R_CS_SetImagePara(0, 0, 1, BASE_GRAYADDRESS, "?????")){
///				printf("Camera ch0 SettingError\n");
///			}
			if(OK!=R_CS_SetImagePara(0, 0, 1, SAVE_GRAYADDRESS, "?????")){
				printf("Camera ch0 SettingError\n");
			}

		// 検査パラメータ初期化
			InitialInspectionData();

		// NGカウンタ設定
///			R_CS_SetCounterNgCode(NgCode);
///			R_CS_SetCounterNgClass(NgClass);
			SetCounter();

		// 通信設定
			if((Option & IO_INCY122) == 0){
				R_CS_SetCommunicationMode(R_CS_COMMUNICATION_LED_BOARD_IO, GPDAC_DSW);
			} else {
				R_CS_SetCommunicationMode(R_CS_COMMUNICATION_USER, GpioInPort);
			}

		// R_bgray_thresholdingの初期化を行う
		//	if( ERROR == R_bgray_thresholding_open( THRESH_DEPTH_8BIT )) {
		//		sprintf( Comment, " *** R_bgray_thresholding_open ERROR *** \n [ malloc Error ] " );
		//		DrawMessage( Comment, DRAW_RED, 2, 0 );
		//		return ERROR;
		//	}

		// 特徴量格納領域確保
			res = R_measure_open( &Mpx, 2000 );
			if( res != OK ) {
				sprintf( Comment, " *** R_measure_open ERROR *** \n [ Error code : %d ] ", res );
				DrawMessage( Comment, DRAW_RED, 2, 0 );
				return ERROR;
			}

		// ロバスト推定ライブラリのオープン
			res = R_robust_open(MAX_POINT);
			if( res != OK ) {
				sprintf( Comment, " *** R_robust_open ERROR *** \n [ Error code : %d ] ", res );
				DrawMessage( Comment, DRAW_RED, 2, 0 );
				return ERROR;
			}
		// 座標点群の領域確保
			if(NULL == ( Src_point_adr = (R_POINT *)malloc(MAX_POINT * sizeof(R_POINT))))return ERROR;
			if(NULL == ( Dst_point_adr = (R_POINT *)malloc(MAX_POINT * sizeof(R_POINT))))return ERROR;
			Src_point = Src_point_adr;
			Dst_point = Dst_point_adr;

		// --- 検査用関数初期化 ---
			if(OK!=R_gs_defadrs( INITIAL_PTN_AREA )){		//グレーサーチ初期化
				sprintf( Comment, " !! gray search initialize error !! ");
				DrawMessage( Comment, DRAW_RED, 2, 0 );
				return ERROR;
			}

		// --- 機台No.設定 ---

			SetMcacineNo();
		// --- insp. Type name set
//		sprintf( Comment, "%s[%s%s%s]%s", ProductName, MarkChar, DataClass[InspClass], PilotDisp[PilotFlag], MachineName );
		sprintf( Comment, "%s[%s %s]%s", ProductName, MarkChar, DataClass[InspClass],  MachineName );
		res = R_CS_SetInspectionInfo(1, 1, 1, 1, 1, 1, InspectionName, Comment);
		if(res!=OK)		return ERROR;
		R_CS_DispTitle(INSPECT_MANUAL);

		// --- 初期化完了フラグ
			InitializeFlag = ERROR;

	// 検査プログラム初期化
//	if(OK!=InspectionOpen()){
//		printf("*E* InspectionOpen\n");
//		return ERROR;
//	}

//	R_CS_SetResultMonitorItem(0, RESULT_MONITOR_FILE_NAME, ResultMonitorList);

		return OK;

	}	// InitialUserOnR_CS(void) end


//------------------------------------------------
//	InitialUserOnR_CSLast
//------------------------------------------------

static int InitialUserOnR_CSLast(void)
	{

		// 取り込み画像の設定
			if(OK!=R_CS_SetImagePara(0, 0, 1, BASE_GRAYADDRESS, "?????")){
				printf("Camera ch0 SettingError\n");
			}

		//INDEX検査用　R_TRANS_DATA構造体のOPEN
			if( OK != R_trans_data_open( &IndexTrans, TRANS_TYPE_INT, FxSize ) ){
				printf("R_trans_open()  error!!   --INDEX--\n");
				return ERROR;
			} else {
				printf(" InspPara.tIndex.tTrans.id=%x\n", IndexTrans.id);
				R_trans_para_set_count_smooth( IndexTrans, 1 );		// 平滑化回数の設定
				R_trans_para_set_index       ( IndexTrans, 1 );		// 何番目のｴｯｼﾞをとるか
				R_trans_para_set_thickness   ( IndexTrans, 5 );		// 平滑化処理幅
				R_trans_para_set_diff_step   ( IndexTrans, 1 );		// １次微分間隔
				R_trans_para_set_thresh_amp  ( IndexTrans, 1000 );	// 振幅しきい値
				R_trans_para_set_thresh_area ( IndexTrans, 100 );	// 面積しきい値
			}

		// DR-X-124設定
			if(0 == (Option&NO_DRX124) && (LedControllMode == LED_CONTROLL_DRX124 )){
				if(OK != DrX124ConditionSet()){
					R_DrawPopupMessage(POPUP_WAIT_CLICK,"DR-X-124 Condisiton Set Error");		// クリック待ちするように修正
					return ERROR;
				}
				// CO-CZ-43のトリガ出力を有効にする
				// (R_CSでカメラ設定＝CO-CZ-43設定をした後に行う必要がある)
				{
					int nOldId = R_video_get_card_id();
					R_change_video_card(0);
					R_video_gpio_control(VGPIO_BIT0, VGPIO_SYNC);
					R_change_video_card(nOldId);
				}
			}

			// 業者調整モード時はバーコード設定無しで稼働できる
			if((Option & MECHA_CHECK)){
				InitializeFlag = OK;
				TWA_Spec		= SpecResin;	//通常品
				TWA_Insp_level	= Insp_Low;		//通常検査
				TWA_Insp_mode   = Insp_Normal;
				InspClass		= CLASS_J;		// J級
			}
			// 裏面検査時は、バーコード設定無しでも稼働出来る
			if((Option & BACKINSP_MODE)){
				InitializeFlag = OK;
				TWA_Spec		= SpecResin;		//MCR01 Nitto
				TWA_Insp_level	= Insp_Low;			//通常検査
				TWA_Insp_mode   = Insp_Pinhole;
				InspClass       = CLASS_J;			// J級
			}

			if(bIsNitto){
				TP_Bright_xs		= 50;						//LED照明照度監視 Area xs
				TP_Bright_ys		= 20;						//LED照明照度監視 Area ys
				TP_Bright_xl		= 200;						//LED照明照度監視 Area xl
				TP_Bright_yl		= 20;						//LED照明照度監視 Area yl
			}
		return OK;
	}


//------------------------------------------------
//	SetMcacineNo
//------------------------------------------------

void	SetMcacineNo( void )
	{
		char					machine[10];

		sprintf( machine, "%-5s", MachineName );

        R_draw_input_keyboard( DRAW_YELLOW, DISP_H2, MachineName, machine, "Input Machine No.", 0 );

	}

//------------------------------------------------
//	Date Time Set  2012.06.07
//------------------------------------------------

static int SetDateTime( int nMenuNumber )
	{
        char					DateTime[10], DateTimeYear[10]={""};
        char                    dos_command[500];

//		ctLotStart = time(NULL);
//		memcpy(&Lot_StartDateTime , localtime(&ctLotStart) ,sizeof(Lot_StartDateTime));
		sprintf( DateTime, "%-10s", DateTimeYear );

		R_draw_input_keyboard( DRAW_YELLOW, DISP_H2, DateTimeYear, DateTime, "Input Date Time", 0 );

		sprintf( Comment, "Do you want to Save Date-Time" );
		R_chrdisp(  3, 10, Comment );
		sprintf( Comment, "%s", "YES:Left(UP) NO:Right(DOWN)" );
		R_chrdisp(  8, 12, Comment );

		if( R_mouse_de_yesno() ) {
			sprintf( dos_command, "date %s", DateTimeYear );
			system( dos_command );
		}

		return OK;
	}

/**********************************************************
	関数名：	ＰｒｏｇＥｘｉｔ
	機  能：	プログラム終了処理
	形  式：	void ProgExit(void)
	引き数：	なし
	戻り値：
	解  説：
	留意事項：
**********************************************************/
void ProgExit(void)
{
//	if(OK!=InspectionClose()){
//		printf("*E* InspectionOpen\n");
//	}

}

/**********************************************************
	関数名：	ＡｕｔｏＭｏｄｅＲｅａｄｙ
	機  能：	オート動作の準備を行う
	形  式：	int AutoModeReady(void)
	引き数：	無し
	戻り値：	OK		正常終了
				ERROR	異常終了
	解  説：	BUSY信号をクリアするなど、通信可能な状態にする
	留意事項：	※ R_CSへの登録方法
				R_CS_SetFunctionAutoModeReady(AutoModeReady);
**********************************************************/
int AutoModeReady(void)
{
	InspGrayPn = backupInspGrayPn;
	R_CS_SetImagePara(0, 0, 1, GetGrayPn, "?????");

	if((Option & NO_DRCY80)==0){
		OutputBusyOff();
		OutputLockOff();
		OutputJudgeNg();
		OutputAlarmOff();	//ALARM OFF
		OutputSaveBusyOff();
	}
	if(Option & IO_INCY122){
		OutputBusyOnNwt();
		OutputLockOnNwt();
		OutputJudgeNgNwt();
	//	OutputAlarmOff();	//ALARM OFF
	//	OutputSaveBusyOff();
	}
	if(0 == (Option&NO_DRX124) && (LedControllMode == LED_CONTROLL_DRX124 )){
		DrX124ResetTurn();			// 点灯パターンをリセットする	 USBの接続が切れていた場合は再接続する
	}
	return OK;
}

/**********************************************************
	関数名：	ＡｕｔｏＭｏｄｅＣｌｏｓｉｎｇ
	機  能：	オート動作の終了処理を行う
	形  式：	int AutoModeClosing(void)
	引き数：	無し
	戻り値：	OK		正常終了
				ERROR	異常終了
	解  説：	BUSY信号をオンするなど、通信不可能な状態にする
	留意事項：	※ R_CSへの登録方法
				R_CS_SetFunctionAutoModeClosing(AutoModeClosing);
**********************************************************/
int AutoModeClosing(void)
{
	if((Option & NO_DRCY80)==0){
		OutputBusyOn();
		OutputLockOn();
		OutputJudgeNg();
	}
	if(Option & IO_INCY122){
		OutputBusyOffNwt();
		OutputLockOffNwt();
		OutputJudgeNgNwt();
	}
	backupInspGrayPn = InspGrayPn;
	InspGrayPn = MANUAL_GRAYADDRESS;
	R_CS_SetImagePara(0, 0, 1, MANUAL_GRAYADDRESS, "?????");

	return OK;
}

/**********************************************************
	関数名：	ＡｕｔｏＭｏｄｅＥｘｅｃｕｔｅ
	機  能：	オート動作で、通信の外部トリガのチェックを行う
	形  式：	int AutoModeExecute(int code)
	引き数：	int code	オート動作コード
	戻り値：	OK		正常終了
				ERROR	異常終了
	解  説：
	留意事項：	※ R_CSへの登録方法
				R_CS_SetFunctionAutoModeExecute(AutoModeExecute);
					R_CS_CODE_NO_SIGNAL			0
					R_CS_CODE_INSPECT_START		1	0x01
					R_CS_CODE_COUNTER_CLEAR		2	0x02
					R_CS_CODE_LOT_INITIAL		3	0x10
					R_CS_CODE_RESULT_SEND		4
					R_CS_CODE_ELSE				255
**********************************************************/

int		AutoModeExecute(int code)
	{
		int						ret=OK;
//		int                     n,dn;    //2012.03.14
//		R_CS_COUNTER_DATA		*pCounter	= R_CS_GetCounterDataStruct(0);			// カウンタ構造体へのポインタ取得 //2012.03.14

//			dn = 0x0ff & ~(R_gpio_read( Incy76Ch | CNa_IN_port_1 ));                //2012.03.14
//			if(dn==0x0ff) Lot_num++;                                                //2012.03.14
		switch(code){
			case R_CS_CODE_INSPECT_START:
				InspectStart(INSPECT_AUTO);
				OutputBusyOff();
				break;
			case R_CS_CODE_COUNTER_CLEAR:
//				OutputBusyOn();
//				R_CS_CounterAllClear();
//				R_CS_DispCounter();
//				R_time_delay(100000);
//				OutputBusyOff();
				break;
//			case R_CS_CODE_ELSE:
//				return ERROR;
//				break;
			case R_CS_CODE_LOT_INITIAL:
				OutputBusyOn();
				R_time_delay(100000);
//				SpecSetBackSide(0);
				OutputBusyOff();
				break;
			default:
				break;
		}

		// Spec set
			if((Option & NO_DRCY80)==0){
				if( CheckSpecSet() == 0 ) {
					if((1==bIsNitto)||(0==(Option & BACKINSP_MODE))){		//v2.11 日東の時or裏面じゃない時
						if( OK == SpecSet(0) ){
							if(GLMode != YES){
								R_CS_CounterAllClear();
								R_CS_DispCounter();
								Clear_flag = 1;                //2011.01.29
							}
						}
					}else{
						if(GLMode != YES){
							R_CS_CounterAllClear();
							R_CS_DispCounter();
							Clear_flag = 1;                //2011.01.29
						}
					}

			if( GLMode != YES && nJudgeMode != 0 ){
			nJudgeMode = 0; //9499-auto return to normal
			R_draw_popup_message( DRAW_GREEN, DISP_4, 1000*1000, "ALL INSPECTIONS IS SET TO NORMAL CONDITION"); //9499-END
			}

		}
	}
			//2012.03.14
/*                if( (R_CS_CounterGetTotalCount( pCounter )+R_CS_CounterGetNgCount( pCounter, MC_RESISTANCE )) == 0 ){
					for(n=0; n<20; n++){
						insp_sum1[n]=0;
						count_total[n]=0, count_R[n]=0, count_N[n]=0, count_L[n]=0, count_Re[n]=0;
						pass_sum[n] = 0;                                               // Count Go
						for (dn=0; dn<100; dn++){
			                count1[n][dn] = 0;
						}
						Lot_num = 0;
					}
				}
*/

		// Check alarm
			if(!(Option & MECHA_CHECK))   TP_CheckAlarm();

		// Setsubi data check
		//<z1>			if((Option & NO_INCY76)==0){
			if((Option & NO_INCY119)==0){ //<z1> <shio>
				SetsubiDataCheck();
			}
		return ret;
	}

static int AutoModeExecuteNwt( int code )
{
	int i;
	long long int data;
	int function;
	int nStartBit;
	int nColletNumber;
	int nWorkState;
//	if(0<SystemPara.nWaitIoRead)	UserTimeDelay(SystemPara.nWaitIoRead);							// I/O読み取り前タイマ[usec](ポーリング時にバスを占有してしまうのを防ぐのに必要)		0なら実行しない 2013/05/03 by sassa
	UserTimeDelay(100);
	data = InputByte();

	code = code;	// 引数を未使用のときにでるワーニング対策

	// IOの状態を表示する
	//if(SystemPara.nFlagDispIoStatus){
	{
		int posi_x = DrawPosi_IoStatus.x;
		int posi_y = DrawPosi_IoStatus.y;
		R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_x, posi_y  , "[IO Status]");
		R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_x, posi_y+1, "IN :%03llX", data);
		R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_x, posi_y+2, "OUT:%02X", GetOutputStatus());	// 2015.01.15 by araki
	}

	// スルーモードの時キャプチャを行う
//	if(ON == ThroughCapture){
//		if(R_time_read(&ThroughTimer) > ThroughInt){
//			CaptureThroughUser(0);
//			R_time_start(&ThroughTimer);
//		}
//	}

	// スタートがONならファンクション信号をチェックして、それに従った処理を行う
	nStartBit = GetStartBit(data);
	if(0 == nStartBit){			// 初期値は0でON

	//	if (SystemPara.nFlagDispTimeOfWaitTrigger) {
	//		R_gui_display_clock_stop(_guiDisplayClock);
	//	}

	//	if( 0 < SystemPara.nWaitSignalInput){		// タイマが入っている場合は、待ってから再度データを読む
	//		// 入力が安定するまでのタイマ
	//		if(0<SystemPara.nWaitSignalInput)	UserTimeDelay(SystemPara.nWaitSignalInput);		// 0なら実行しない
	//		data = InputByte();
	//	}

		// 共通ステーションタイプ
		if(CommunicationDeviceShareId == 999){
			unsigned long long int mask = 0;
			for(i = 0; i < CommunicationDeviceUseLen; i++) mask = (mask<<1) + 1;

			{	data = ( ~data) & mask;	/*printf("reverse off\n");*/ }
		}else {
			{	data = ( ~data) & 0xFFFF;	/*printf("reverse off\n");*/ }
		}

			// カウンタクリアチェック
			if(0x00ff==(GetCounterClear(data) & 0x60ff)){
				// 通信情報の表示
				{
					int posi_x = DrawPosi_IoStatus.x;
					int posi_y = DrawPosi_IoStatus.y + 3;
					//R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_x, posi_y+2, "[Mode]");
					R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_x, posi_y+2, "Mode  :COUNTER CLEAR");
				}
				OutputBusyOffNwt();
				NinshikiDataWrite();
				R_CS_CounterAllClear();
				R_CS_DispCounter();
				R_time_delay(100000);
				OutputBusyOnNwt();
				goto COMMON_END;
			}
			// レシートデータ作成
			if(0x00fe==(GetCounterClear(data) & 0x60ff)){
				// 通信情報の表示
				{
					int posi_x = DrawPosi_IoStatus.x;
					int posi_y = DrawPosi_IoStatus.y + 3;
					//R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_x, posi_y+2, "[Mode]");
					R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_x, posi_y+2, "Mode  :WRITE DATA");
				}
				OutputBusyOffNwt();
				NinshikiDataWrite();
				R_time_delay(100000);
				OutputBusyOnNwt();
				goto COMMON_END;
			}
			// オートロットスタートチェック
		//	if((0<=SystemPara.tInputPara.nCheckLotStart)&&(SystemPara.tInputPara.nCheckLotStart==GetLotStart(data))){
		//		AutoLotStart();
		//		goto COMMON_END;
		//	}
			// 教示モードチェック
		//	tInputState.nRegistration = GetRegistration(data);
		//	if(tInputState.nRegistration==-1){							// ビット番号orビット数異常  2015/06/02 kitamura
		//		printf("Registnumber parameter %d,%d(not 1-%d)", SystemPara.tInputPara.nRegistrationBitStart, SystemPara.tInputPara.nRegistrationBitSize, nMaxBitNumber);
		//		tInputState.nRegistration = -2;				// 使用しないフラグを立てる 2015/06/02 kitamura
		//	}else if(tInputState.nRegistration!=-2){		// 使用する場合
		//		if((0<=SystemPara.tInputPara.nCheckRegistration_0)&&(tInputState.nRegistration == SystemPara.tInputPara.nCheckRegistration_0)){								// 0度のRegistration
		//			for(i=0;i<USER_CAMERA_NUMBER;i++)	SystemPara.nInspImageRotateMode[i] = 0;		// 強制的に回転無しにする
		//			AutoInspectStart(AUTO_START_REGISTER);
		//			goto COMMON_END;
		//		}else if((0<=SystemPara.tInputPara.nCheckRegistration_90)&&(tInputState.nRegistration == SystemPara.tInputPara.nCheckRegistration_90)){							// 90度のRegistration 未対応
		//			R_DrawPopupMessage(POPUP_WAIT_CLICK, "Now, no function registration rotate 90 %x\n", SystemPara.tInputPara.nCheckRegistration_90);
		//			goto COMMON_END;
		//		}else if((0<=SystemPara.tInputPara.nCheckRegistration_180)&&(tInputState.nRegistration == SystemPara.tInputPara.nCheckRegistration_180)){						// 180度のRegistration 未対応
		//			for(i=0;i<USER_CAMERA_NUMBER;i++)	SystemPara.nInspImageRotateMode[i] = 1;		// 強制的に180度回転する
		//			AutoInspectStart(AUTO_START_REGISTER);
		//			goto COMMON_END;
		//		}else if((0<=SystemPara.tInputPara.nCheckRegistration_270)&&(tInputState.nRegistration == SystemPara.tInputPara.nCheckRegistration_270)){						// 270度のRegistration 未対応
		//			R_DrawPopupMessage(POPUP_WAIT_CLICK, "Now, no function registration rotate 270 %x\n", SystemPara.tInputPara.nCheckRegistration_270);
		//			goto COMMON_END;
		//		}
		//	}
			// オートノズルチェックモードチェック
		//	if((0<=SystemPara.tInputPara.nCheckAutoNozzleCheck)&&(SystemPara.tInputPara.nCheckAutoNozzleCheck==GetAutoNozzleCheck(data))){
		//		AutoNozzleCheck();
		//		goto COMMON_END;
		//	}
			// マニュアルスタートチェック
		//	if((0<=SystemPara.tInputPara.nCheckManualStart)&&(SystemPara.tInputPara.nCheckManualStart==GetManualStart(data))){
		//		AutoInspectStart(AUTO_START_MANUAL);
		//		goto COMMON_END;
		//	}
			// コレット番号
			nColletNumber = (~GetColletNumber(data)) & 0xff;// +1;			// メカから受け取る値+1
		//	InspectResult.nColletNumber = nColletNumber;		// コレット番号。メカから受け取る数値+1(1からにするため)
		//	if(InspectResult.nColletNumber<=0){					// ビット番号orビット数異常or使用しない場合は0を入れる
		//		InspectResult.nColletNumber = 0;
		//	}

			// リトライスタートフラグチェック			ワークスステートチェック前に移動
		//	if((0<=SystemPara.tInputPara.nCheckRetryStartBit)&&(SystemPara.tInputPara.nCheckRetryStartBit==GetRetryStartBit(data))){				AutoInspectStart(AUTO_START_RETRY);				goto COMMON_END;	}	// リトライスタート

			PocketNo=nColletNumber;
			// ワーク状態チェック
			nWorkState = GetWorkState(data);
			if(nWorkState == 3){
											InspectStartNwt(INSPECT_AUTO);
											goto COMMON_END;																// Normal
			} else if(nWorkState == 1){
											NGStart = 1;
											InspectStartNwt(INSPECT_AUTO);
											NGStart = 0;
											goto COMMON_END;																// NoWork
			} else if(nWorkState == 2){
											CheckChip = 1;
											InspectStartNwt(INSPECT_AUTO);
											CheckChip = 0;
											goto COMMON_END;																// NGWork
			}
	}
COMMON_END:

	// 自動機種切り替え
//	if(OFF != AutoTypeChangeMode)	AutoTypeChange();

	// Check alarm
		//if(!(Option & MECHA_CHECK))		TP_CheckAlarm();
		TP_CheckAlarm();

	// キーボードデバッグ
	if(Option&DEBUG_MODE){
		if(user_kbhit()){
			int key;
			key = user_getch();

			switch(key){
				case 's':
				case 'S':
					InspectStartNwt(INSPECT_AUTO);
					break;
			//	case '1':
			//		AutoInspectStart(AUTO_START_NO_WORK);
			//		break;
			//	case '2':
			//		AutoInspectStart(AUTO_START_NG_WORK);
			//		break;
			//	case '3':
			//		AutoInspectStart(AUTO_START_RETRY);
			//		break;
			//	case '4':
			//		AutoInspectStart(AUTO_START_MANUAL);
			//		break;
			//	case '5':
			//		AutoInspectStart(AUTO_START_REGISTER);
			//		break;
			//	case 'c':
			//	case 'C':
			//		AutoCounterClear();
			//		break;
			//	case 'n':
			//	case 'N':
			//		AutoNozzleCheck();
			//		break;
			//	case 'l':		// 2011/01/12 by sassa
			//	case 'L':
			//		AutoLotStart();
			//		break;
			}
		}
	}

	return OK;
}


/******************************************************************************
	関数名：	ＩｎｓｐｅｃｔＭａｎｕａｌ
	機  能：	マニュアル検査実行
	形  式：	int InspectManual(int mode, int number, R_CS_RESULT_DATA *Result)
	引き数：	int mode					検査モード
												INSPECT_AUTO
												INSPECT_MANUAL
												INSPECT_STEP
				int number					検査番号
				R_CS_RESULT_DATA *Result	検査結果構造体
	戻り値：
				値		定数		意　味
				0		OK		良品
				-1		ERROR	不良品・異常終了
	解  説：	R_CS関数内より、マニュアル判定、ステップ判定でこのルーチンが呼ばれます。
	留意事項：	メイン→認識→メモリ判定／ステップ判定 で実行する関数
				マニュアル判定時にR_CS_RESULT_DATAに設定した値はカウンタ処理しません
	留意事項：	※ R_CSへの登録方法
				R_CS_SetFunctionInspect(InspectManual);
******************************************************************************/
int InspectManual(int mode, int number, R_CS_RESULT_DATA *pResult)
{
	int res = 0;

/*
	if(mode&INSPECT_AUTO){
		SetManualMode(OFF);
		SetStepMode(OFF);
	}else if(mode&INSPECT_MANUAL){
		SetManualMode(ON);
		if(mode&INSPECT_STEP)	SetStepMode(ON);
		else					SetStepMode(OFF);
	}
*/
	// 検査対象Gray Pn
///		InspGrayPn	= BASE_GRAYADDRESS;
///		R_move_gray_memory( InspGrayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
		R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );
/*
		InspGray3Pn	= BASE3_GRAYADDRESS;
		R_move_gray_memory( InspGray3Pn, GrayP0, 0, 0, FxSize-1, FySize-1 );

		InspGray4Pn	= BASE4_GRAYADDRESS;
		R_move_gray_memory( InspGray4Pn, GrayP0, 0, 0, FxSize-1, FySize-1 );
*/

	// --- 検査開始 ---
		ProcessTimerStart();

		res = Inspect( pResult );

	// --- 検査終了 ---
		ProcessTimerStop();
//		if(mode==INSPECT_MANUAL)		ProcessTimerDisplay();
		ProcessTimerDisplay();

		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "manual inspect");

//	pResult->nCode = OK;
if(nJudgeMode == ALL_GO) R_DrawPrintf( DRAW_RED, DISP_4, 4, 6, "WARNING : JUDGEMENTS ARE BYPASS!!"); //9499
	return res;
}

/**********************************************************
	関数名：	ＩｎｓｐｅｃｔＳｔａｒｔ
	機  能：	検査を行います
	形  式：	static int InputInspectStart(int mode)
	引き数：	int mode	検査モード
								INSPECT_AUTO	オート稼動
								INSPECT_MANUAL	マニュアル稼動
								INSPECT_STEP	ステップ判定
	戻り値：	OK		取り込み成功
				ERROR	取り込み失敗
				ERROR2	START信号が入っていない
	解  説：
	留意事項：
**********************************************************/

static int	InspectStart(int mode)
	{
		int						dn;
		int						res;
		int                     NG_count;
		R_LED_PARA				*pLedPara;
		R_CS_COUNTER_DATA		*pCounter = R_CS_GetCounterDataStruct(0);		// カウンタ構造体へのポインタ取得

			twa_TimeStart( &twaTime[0] );
		// NG Limit check
			Work_count ++;          //2011.08.17

		// 検査結果出力
			switch( OutputJudge() ) {
				case ERROR:
					DrawMessage( "Waiting for judgment output", DRAW_RED, 5, 0 );
					R_CS_Result[0].nCode = NG_NO_WORK;
					for( dn=0; dn<3; dn++ ) {
						R_time_delay( 10000 );
						if( OutputJudge() == OK )	break;
						R_CS_Result[0].nCode = NG_NO_WORK;
					}
					DrawMessageOff();
					break;
				case ERROR2:
					R_CS_Result[0].nCode = NG_CAPTURE;
					GrayClsPn( BASE_GRAYADDRESS, 0, 0, FxSize-1, FySize-1 );
					break;
			}

			twa_TimeRead( &twaTime[0],0 );

		// 画像転送
			switch( R_CS_CaptureEndCheck(0) ) {
				case OK:
					if( GetGrayPn == (int)NULL ) {	// 起動後1回目
						R_CS_Result[0].nCode = NG_CANT_INSP;
						GetGrayPn = BASE_GRAYADDRESS;
					} else {
						InspGrayPn = GetGrayPn;
						if( GetGrayPn == BASE_GRAYADDRESS )	GetGrayPn = BASE2_GRAYADDRESS;
						else								GetGrayPn = BASE_GRAYADDRESS;
					}
					// --- 取り込み画像の設定
					if(OK!=R_CS_SetImagePara(0, 0, 1, GetGrayPn, "?????")){
						R_CS_Result[0].nCode = NG_CANT_INSP;
					}
					break;
				case 1:
					if( R_CS_CaptureWait(0) != OK )	R_CS_Result[0].nCode = NG_CAPTURE;
					break;
				default:
					R_CS_Result[0].nCode = NG_CAPTURE;
					break;
			}

		// 画像取り込み
			// --- LED set
			pLedPara = &LedPara;
			// --- 画面取り込み終了待ち
			R_time_delay(SystemPara.nCaptureTimer);
			// --- 照明点灯
			R_LedOn(pLedPara);
			// --- 取り込み
			res = R_CS_Capture(0, 0, 0);
			if( res != OK ) {
				R_CS_Result[0].nCode = NG_CAPTURE;
				printf("*E* Capture ERROR:%d\n", res);
			}
			// --- 照明消灯
			R_LedOff(pLedPara);

		// --- スタート信号ＯＦＦ確認
			OutputBusyOn();
			twa_TimeRead( &twaTime[0],1 );

		// Auto mode Check
			if( mode & INSPECT_AUTO ) {
			// --- M/C Cancle Poket Check   //2012.04.19
				if( CanclePok_sig() == 0 )	Poket_flag = 1;
				else                        Poket_flag = 0;
			}
			R_time_start( &RTWaitTime );
			while( 0 == CheckInspStart() ) {
				// --- タイムアウト時
				if( 3000000 < R_time_read(&RTWaitTime) ) {
					R_CS_Result[0].nCode = NG_CANT_INSP;
					sprintf( Comment, "Insp. start signal ERROR" );
					DrawMessage( Comment, DRAW_RED, 0, 0 );
				}
			}
///			OutputBusyOff();

		// 判定出力クリア
			OutputJudgeNg();
			twa_TimeRead( &twaTime[0],2 );

			NG_count = NO;
			if( mode & INSPECT_AUTO ) {
				if( CheckPilotMode() != 0 ) NG_count = YES;
				if( Pilot_mode == ON ) NG_count = YES;
			}
		// Auto mode
			if( mode & INSPECT_AUTO ) {
				if( NG_count == YES ) {
				// --- M/C NG box count
					if( CheckResistanceNG() == 0 )	R_CS_CounterUp( pCounter, MC_RESISTANCE );
					if( CheckNinshikiNG() == 0 )	R_CS_CounterUp( pCounter, MC_NINSHIKI );
				}
			}

		// この時点でGray_address[BASE_GRAYADDRESS]に画像が取り込まれています
		// int InspectManual(int mode, R_CS_RESULT_DATA *pCsResult)

		// --- 画像保存用 ---
			R_move_gray_memory( InspGrayPn, SAVE_GRAYADDRESS, 0, 0, FxSize-1, FySize-1 );
			R_move_gray_memory( InspGrayPn, MANUAL_GRAYADDRESS, 0, 0, FxSize-1, FySize-1 );
			twa_TimeRead( &twaTime[0],3 );

		// --- 検査開始 ---
			if( 0 == R_CS_Result[0].nCode )	Inspect( &R_CS_Result[0]);
			else							R_time_delay(12*1000);		// 500->12 20170213 tanaka
			twa_TimeRead( &twaTime[0],4 );
		// --- 検査終了 ---
			ResultCode = R_CS_Result[0].nCode;							// ResultMonitorでNGコードを保存するため
			R_CS_AfterInspection(0, mode|NO_COUNT_UP_RESULT_MONITOR);

		// Data Back Up Counter
//			Data_Lot_count();

		// NG Limit Check     2011.08.17
			if( Work_count == NG_Limit[0][0] ){
				NG_Limit_Check();
			    Work_count = 0;
			}
			twa_TimeRead( &twaTime[0],5 );
			twa_TimeDisp(&twaTime[0]);

	if(nJudgeMode == ALL_GO) R_DrawPrintf( DRAW_RED, DISP_4, 4, 6, "WARNING : JUDGEMENTS ARE BYPASS!!"); //9499
		return OK;

	}	// InspectStart(int mode) end


static int	InspectStartNwt(int mode)
	{
		int						dn;
		int						res;
		int                     NG_count;
		R_CS_COUNTER_DATA		*pCounter = R_CS_GetCounterDataStruct(0);		// カウンタ構造体へのポインタ取得

		// 通信情報の表示
		{
			int posi_x = DrawPosi_IoStatus.x;
			int posi_y = DrawPosi_IoStatus.y + 3;
			//R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_x, posi_y+2, "[Mode]");
			if(mode == INSPECT_AUTO){
				if(Result_NGStart[0]){
					R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_x, posi_y+2, "Mode :NG Start      ,%3d",Result_PocketNo[0]);
				} else if(Result_CheckChip[0]){
					R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_x, posi_y+2, "Mode :NoWork Start  ,%3d",Result_PocketNo[0]);
				} else {
					R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_x, posi_y+2, "Mode :Normal Start  ,%3d",Result_PocketNo[0]);
				}
			}
		}
			twa_TimeStart( &twaTime[0] );
		// NG Limit check
			Work_count ++;          //2011.08.17

		// 検査結果出力
			OutputJudgeNwt();
R_time_delay(1000); // setup time
		//	switch( OutputJudgeNwt() ) {
		//		case ERROR:
		//			DrawMessage( "Waiting for judgment output", DRAW_RED, 5, 0 );
		//			R_CS_Result[0].nCode = NG_NO_WORK;
		//			for( dn=0; dn<3; dn++ ) {
		//				R_time_delay( 10000 );
		//				if( OutputJudgeNwt() == OK )	break;
		//				R_CS_Result[0].nCode = NG_NO_WORK;
		//			}
		//			DrawMessageOff();
		//			break;
		//		case ERROR2:
		//			R_CS_Result[0].nCode = NG_CAPTURE;
		//			GrayClsPn( BASE_GRAYADDRESS, 0, 0, FxSize-1, FySize-1 );
		//			break;
		//	}

			OutputBusyOffNwt();
			OutputLockOffNwt();
			if(mode == INSPECT_AUTO){
				Result_CheckChip[1]=Result_CheckChip[0];
				Result_CheckChip[0]=CheckChip;
				Result_NGStart[1]  =Result_NGStart[0];
				Result_NGStart[0]  =NGStart;
				Result_PocketNo[1] =Result_PocketNo[0];
				Result_PocketNo[0] =PocketNo;
			}
			twa_TimeRead( &twaTime[0],0 );

		// 画像転送
			switch( R_CS_CaptureEndCheck(0) ) {
				case OK:
					if( GetGrayPn == (int)NULL ) {	// 起動後1回目
						R_CS_Result[0].nCode = NG_CANT_INSP;
						GetGrayPn = BASE_GRAYADDRESS;
					} else {
						InspGrayPn = GetGrayPn;
						if( GetGrayPn == BASE_GRAYADDRESS )	GetGrayPn = BASE2_GRAYADDRESS;
						else								GetGrayPn = BASE_GRAYADDRESS;
					}
					// --- 取り込み画像の設定
					if(OK!=R_CS_SetImagePara(0, 0, 1, GetGrayPn, "?????")){
						R_CS_Result[0].nCode = NG_CANT_INSP;
					}
					break;
				case 1:
					if( R_CS_CaptureWait(0) != OK )	R_CS_Result[0].nCode = NG_CAPTURE;
					break;
				default:
					R_CS_Result[0].nCode = NG_CAPTURE;
					break;
			}

		// 画像取り込み
			// --- 画面取り込み終了待ち
			R_time_delay(SystemPara.nCaptureTimer);
			// --- 取り込み
			if(Option&DEBUG_MODE){
				R_move_gray_memory(0,InspGrayPn,0,0,FxSize-1,FySize-1);
				R_move_gray_memory(0,GetGrayPn ,0,0,FxSize-1,FySize-1);
			} else {
				res = R_CS_Capture(0, 0, 0);
				if( res != OK ) {
					R_CS_Result[0].nCode = NG_CAPTURE;
					printf("*E* Capture ERROR:%d\n", res);
				}
			}
			OutputLockOnNwt();
			twa_TimeRead( &twaTime[0],1 );

		// Auto mode Check
		//	if( mode & INSPECT_AUTO ) {
		//	// --- M/C Cancle Poket Check   //2012.04.19
		//		if( CanclePok_sig() == 0 )	Poket_flag = 1;
		//		else                        Poket_flag = 0;
		//	}
			R_time_start( &RTWaitTime );

		// 判定出力クリア
		//	OutputJudgeNgNwt();
			twa_TimeRead( &twaTime[0],2 );

			NG_count = NO;
			if( mode & INSPECT_AUTO ) {
				if( CheckPilotMode() != 0 ) NG_count = YES;
				if( Pilot_mode == ON ) NG_count = YES;
			}
		// Auto mode
	//		if( mode & INSPECT_AUTO ) {
	//			if( NG_count == YES ) {
	//			// --- M/C NG box count
	//				if( CheckResistanceNG() == 0 )	R_CS_CounterUp( pCounter, MC_RESISTANCE );
	//				if( CheckNinshikiNG() == 0 )	R_CS_CounterUp( pCounter, MC_NINSHIKI );
	//			}
	//		}

		// この時点でGray_address[BASE_GRAYADDRESS]に画像が取り込まれています
		// int InspectManual(int mode, R_CS_RESULT_DATA *pCsResult)

		// --- 画像保存用 ---
			R_move_gray_memory( InspGrayPn, SAVE_GRAYADDRESS, 0, 0, FxSize-1, FySize-1 );
			R_move_gray_memory( InspGrayPn, MANUAL_GRAYADDRESS, 0, 0, FxSize-1, FySize-1 );
			twa_TimeRead( &twaTime[0],3 );

		// --- 検査開始 ---
			if( 0 == R_CS_Result[0].nCode )	Inspect( &R_CS_Result[0]);
			else							R_time_delay(12*1000);		// 500->12 20170213 tanaka
			twa_TimeRead( &twaTime[0],4 );
		// --- 検査終了 ---
			ResultCode = R_CS_Result[0].nCode;							// ResultMonitorでNGコードを保存するため
			// 画像ファイルに埋め込むコメント
			sprintf( R_CS_Result[0].sImageComment, "INDEX,%d,state,%d",Result_PocketNo[1],(Result_NGStart[1]<<1) + Result_CheckChip[1] );

			if((!Result_NGStart[1]) && (!Result_CheckChip[1])){
				R_CS_AfterInspection(0, mode|NO_COUNT_UP_RESULT_MONITOR);
			} else {
				R_CS_AfterInspection(0, mode|NO_COUNT_UP_RESULT_MONITOR
											|NO_COUNT_UP_DETAIL
											|NO_COUNT_UP_TOTAL
											|NO_COUNT_UP_PRODUCT);
			}

			// --- スタート信号ＯＦＦ確認
			while( 0 == CheckInspStartNwt() ) {
				// --- タイムアウト時
				if( 3000000 < R_time_read(&RTWaitTime) ) {
					R_CS_Result[0].nCode = NG_CANT_INSP;
					sprintf( Comment, "Insp. start signal ERROR" );
					DrawMessage( Comment, DRAW_RED, 0, 0 );
				}
			}
			OutputBusyOnNwt();

		// Data Back Up Counter
//			Data_Lot_count();

		// NG Limit Check     2011.08.17
			if( Work_count == NG_Limit[0][0] ){
				NG_Limit_Check();
				Work_count = 0;
			}
			twa_TimeRead( &twaTime[0],5 );
			twa_TimeDisp(&twaTime[0]);


if(nJudgeMode == ALL_GO) R_DrawPrintf( DRAW_RED, DISP_4, 4, 6, "WARNING : JUDGEMENTS ARE BYPASS!!"); //9499
		return OK;

	}	// InspectStart(int mode) end


//================================================
//	検査結果出力
//================================================

int		OutputJudge( void )
	{
		int						i_mode = R_CS_GetInspectMode();
		int						res = OK;

		// 判定読み取り確認
			if((i_mode&INSPECT_AUTO)&&(CheckJudgeRead()==0))	return ERROR2;
		// Lock ON
			OutputLockOn();
		// 判定出力


		if(NORMAL == nJudgeMode){
			switch(R_CS_Result[0].nCode){
				case OK:				// OK判定のとき
					OutputJudgeGo();
					OutputReverseOff();
					OutputNoworkOff();                             //No work Signal
					break;
				case NG_NO_WORK:		// NO WORK判定のとき
					OutputJudgeNg();
					OutputReverseOn();
					OutputNoworkOn();                             //No work Signal
					break;
				case NG_CANT_INSP:		// 検査不可
				case NG_INDEX_POS:		// Index pos. Error
				case NG_REVERSE:		// Chip Reverse
				case NG_FOCUS:			// Focus Error
					OutputJudgeNg();
					OutputReverseOn();
					OutputNoworkOff();                             //No work Signal
					R_move_gray_memory( InspGrayPn, FAIL_GRAYADDRESS, 0, 0, FxSize-1, FySize-1 );
					break;
				default:				// NG判定のとき
					OutputJudgeNg();
					OutputReverseOff();
					OutputNoworkOff();                             //No work Signal
					R_move_gray_memory( InspGrayPn, FAIL_GRAYADDRESS, 0, 0, FxSize-1, FySize-1 );
					break;
			}

		}else{
			switch(nJudgeMode){
				case ALL_GO:				// OK判定のとき
					OutputJudgeGo();
					OutputReverseOff();
					OutputNoworkOff();                             //No work Signal
					break;
				case ALL_NG_NOWORK:			// NO WORK判定のとき
					OutputJudgeNg();
					OutputReverseOn();
					OutputNoworkOn();                             //No work Signal
					break;
				case ALL_NG_REVERSE:		// Chip Reverse
					OutputJudgeNg();
					OutputReverseOn();
					OutputNoworkOff();                             //No work Signal
					R_move_gray_memory( InspGrayPn, FAIL_GRAYADDRESS, 0, 0, FxSize-1, FySize-1 );
					break;
				default:				// NG判定のとき
					OutputJudgeNg();
					OutputReverseOff();
					OutputNoworkOff();                             //No work Signal
					R_move_gray_memory( InspGrayPn, FAIL_GRAYADDRESS, 0, 0, FxSize-1, FySize-1 );
					break;
			}

		}

		// 判定読み取り確認
			if(i_mode&INSPECT_AUTO) {
				R_time_start( &RTWaitTime );
				while( CheckJudgeRead() != 0 ) {
					// --- タイムアウト時
					if( 5000000 < R_time_read(&RTWaitTime) ) {
						res = ERROR;
						break;
					}
				}
			}
		// Lock OFF
			OutputLockOff();

		// ＮＧデータクリア
			R_CS_Result[0].nCode = 0;

		return(res);

	}	// OutputJudge( void ) end

int		OutputNoworkOn( void )
	{
		if(((Option & NO_INCY119)!=0)&&((Option & NO_INCY76)==0)){		//<shio>
			OutputNoworkOn76();                             //No work Signal
		}else{
			OutputNoworkOn119();                             //No work Signal
		}
	}
int		OutputNoworkOff( void )
	{
		if(((Option & NO_INCY119)!=0)&&((Option & NO_INCY76)==0)){		//<shio>
			OutputNoworkOff76();                             //No work Signal
		}else{
			OutputNoworkOff119();                             //No work Signal
		}
	}


static int outputcounter = 0;

int		OutputJudgeNwt( void )
	{
		int						i_mode = R_CS_GetInspectMode();
		int						res = OK;
outputcounter++;
// 判定読み取り確認
	//		if((i_mode&INSPECT_AUTO)&&(CheckJudgeRead()==0))	return ERROR2;
		// Lock ON
	//		OutputLockOn();
		// 判定出力


		if(NORMAL == nJudgeMode){
			switch(R_CS_Result[0].nCode){
				case OK:					// OK判定のとき
//printf("A %d\n",outputcounter); fflush(stdout);
					OutputJudgeGoNwt();
					OutputBinNormalNwt();
					OutputNoworkOffNwt();    //No work Signal
					break;
				case NG_NO_WORK:			// NO WORK判定のとき
					if(!Result_CheckChip){
//						printf("B %d\n",outputcounter); fflush(stdout);
						OutputJudgeNgNwt();
					} else{
//						printf("C %d\n",outputcounter); fflush(stdout);
						OutputJudgeGoNwt();
					}
					OutputBinNormalNwt();
					OutputNoworkOnNwt();    //No work Signal
					break;
				case NG_CANT_INSP:			// 検査不可
				case NG_INDEX_POS:			// Index pos. Error
				case NG_FOCUS:				// Focus Error
					OutputJudgeNgNwt();
					OutputBinSystemNwt();
					OutputNoworkOffNwt();                             //No work Signal
					R_move_gray_memory( InspGrayPn, FAIL_GRAYADDRESS, 0, 0, FxSize-1, FySize-1 );
					break;
				case NG_REVERSE:			// Chip Reverse
					OutputJudgeNgNwt();
					OutputBinReverseNwt();
					OutputNoworkOffNwt();                             //No work Signal
					R_move_gray_memory( InspGrayPn, FAIL_GRAYADDRESS, 0, 0, FxSize-1, FySize-1 );
					break;
				default:					// NG判定のとき
					OutputJudgeNgNwt();
					OutputBinNormalNwt();
					OutputNoworkOffNwt();   //No work Signal
					R_move_gray_memory( InspGrayPn, FAIL_GRAYADDRESS, 0, 0, FxSize-1, FySize-1 );
					break;
			}

		}else{

//printf("D %d\n",outputcounter); fflush(stdout);
			switch(nJudgeMode){
				case ALL_GO:				// OK判定のとき
					OutputJudgeGoNwt();
					OutputBinNormalNwt();
					OutputNoworkOffNwt();	//No work Signal
					break;
				case ALL_NG_NOWORK:			// NO WORK判定のとき
					OutputJudgeNgNwt();
					OutputBinNormalNwt();
					OutputNoworkOnNwt();    //No work Signal
					break;
				case ALL_NG_REVERSE:		// Chip Reverse
					OutputJudgeNgNwt();
					OutputBinReverseNwt();
					OutputNoworkOffNwt();   //No work Signal
					R_move_gray_memory( InspGrayPn, FAIL_GRAYADDRESS, 0, 0, FxSize-1, FySize-1 );
					break;
				default:					// NG判定のとき
					OutputJudgeNgNwt();
					OutputBinNormalNwt();
					OutputNoworkOffNwt();	//No work Signal
					R_move_gray_memory( InspGrayPn, FAIL_GRAYADDRESS, 0, 0, FxSize-1, FySize-1 );
					break;
			}

		}

		// 判定読み取り確認
	//		if(i_mode&INSPECT_AUTO) {
	//			R_time_start( &RTWaitTime );
	//			while( CheckJudgeRead() != 0 ) {
	//				// --- タイムアウト時
	//				if( 5000000 < R_time_read(&RTWaitTime) ) {
	//					res = ERROR;
	//					break;
	//				}
	//			}
	//		}
		// Lock OFF
	//		OutputLockOff();

		// ＮＧデータクリア 結果出力後
		R_CS_Result[0].nCode = 0;

		return(res);

	}	// OutputJudge( void ) end


/******************************************************************************
	関数名：	ＣａｐｔｕｒｅＭａｎｕａｌ
	機  能：	マニュアル検査用取り込み関数
	形  式：	static void CaptureManual(void)
	引き数：	無し
	戻り値：	無し
	解  説：
	留意事項：	画像を取込みます。
******************************************************************************/

static int	CaptureManual(void)
	{
		R_LED_PARA	*pLedPara;

		pLedPara = &LedPara;

		// 取り込み画像の設定
/////			if(OK!=R_CS_SetImagePara(0, 0, 1, BASE_GRAYADDRESS, "?????")){
/////				printf("Camera ch0 SettingError\n");
/////			}

		R_LedOn(pLedPara);			// 照明点灯
		R_CS_Capture(0, 0, 0);		// 取り込み(0:未充填検査用 1:リード検査用)
		R_LedOff(pLedPara);			// 照明消灯
		R_CS_CaptureWait(0);

		R_time_delay(50*1000);

		return OK;
	}


/******************************************************************************
	関数名：	ＣａｐｔｕｒｅＴｈｒｏｕｇｈ
	機  能：	スルー画像取得用取り込み関数
	形  式：	static void CaptureThrough(int capture_nb)
	引き数：	int capture_nb		取込み番号
	戻り値：	無し
	解  説：
	留意事項：	画像を取込みます。
******************************************************************************/
static void CaptureThrough(int capture_nb)
{
	R_LED_PARA *pLedPara;
	int x0, y0;
	int ret;

	x0 = FxSize/2;
	y0 = FySize/2;

	R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 1, 1, "No.%d", capture_nb);

	pLedPara = &LedPara;

	// 取り込み画像の設定
////		if(OK!=R_CS_SetImagePara(0, 0, 1, BASE_GRAYADDRESS, "?????")){
////			printf("Camera ch0 SettingError\n");
////		}

	// 照明点灯
	if(!bIsNitto)	R_LedOn(pLedPara);

	// 取り込み
	ret=R_CS_Capture(0, capture_nb, 0);

	// Bright level check
	TP_CheckBright();

	// 照明消灯
	if(!bIsNitto)	R_LedOff(pLedPara);

    // Bright level check
    TP_CheckBright();

	if(OK==ret)		ret=R_CS_CaptureWait(0);
//	R_move_gray_memory( BASE_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );
	R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

	R_time_delay(50*1000);
}


////////////////////////////////////////////////////////////
//              Print test                                //
////////////////////////////////////////////////////////////
static int	Print2(int nMenuNumber)
	{
			if(bIsNitto)	NinshikiDataWrite();
			else			Print1();
			return OK;
	}

////////////////////////////////////////////////////////////
//      NG Limit On / Off     2011.08.17                  //
////////////////////////////////////////////////////////////

static int	NGLimitCHK(int nMenuNumber)
	{
			R_DrawWinbParameterWithMess( NGLimitOnOff, "NG Limit Setting On/Off", "NG Limit Setting On/Off");
			return OK;
	}

////////////////////////////////////////////////////////////
//      Bright Monitoring Set    2017.08.07               //
////////////////////////////////////////////////////////////

static int MonitorCHK(int nMenuNumber)
	{
	char Comment[200];

		R_draw_popup_message(DRAW_GREEN, DISP_NORMAL, 1000000,"*** New Save Brightness ***");

		sprintf( Comment, "Do you want to Save Bright");
		R_chrdisp(  20, 10, Comment );
		sprintf( Comment, "%s", "YES:Left(Click) NO:Right(Click)" );
		R_chrdisp(  28, 12, Comment );

		// Bright level check
		TP_CheckBright();

	   if( R_mouse_de_yesno() == YES ){
		   // Save Bright
		   TP_Bright_st = TP_Bright_d;
		   R_draw_popup_message(DRAW_GREEN, DISP_NORMAL, 1000000,"*** Save Completed ***");
	   }
	   else	   R_draw_popup_message(DRAW_GREEN, DISP_NORMAL, 1000000,"*** Don't Save ***");

	   BinClsPn( 0, 0, 0, FxSize-1, FySize-1 );
	   return OK;
	}

////////////////////////////////////////////////////////////
//      Counter Cls     //2012.03.14                      //
////////////////////////////////////////////////////////////
/*
static int	Counter_cls(int nMenuNumber)
	{
	char Comment[200];
		sprintf( Comment, "Do you want to reset Semi Lot[%2d]", Lot_num+1 );
		R_chrdisp(  3, 10, Comment );
		sprintf( Comment, "%s", "YES:Left(UP) NO:Right(DOWN)" );
		R_chrdisp(  8, 12, Comment );

		if( R_mouse_de_yesno() ) {
			if(Lot_num<20) Lot_num++;
			else           Lot_num = 0;
		}
		BinClsPn( 0, 0, 0, FxSize-1, FySize-1 );
			return OK;
	}
*/
/**********************************************************
	関数名：	ＰａｒａｍｅｔｅｒＩｎｓｐＳｅｔ
	機  能：	検査設定
	形  式：	static void ParameterInspSet(void)
	引き数：	なし
	戻り値：	なし
	解  説：
	留意事項：
**********************************************************/

static int	ParameterInspSet(int nMenuNumber)
	{
		int						res;
		char	UsbKeyInformation[1024];
		if((Option & REPI) != 0){
	        if( 0==(Option & NO_USBKEY) && OK!=UsbKeyCertification( UsbKeyInformation, 14 )) return ERROR;
		} else {
			if( 0==(Option & NO_USBKEY) && OK!=UsbKeyCertification_rist( &UsbKeyInfo ) ) return ERROR;	//2017.06.13
		}

		if(Option & ADMIN_MODE){
			res = R_DrawWinbParameterWithMess( WinbmParaInspect, "検査パラメータ設定", "Setting Inspect Parameter");
		} else {
			res = R_DrawWinbParameterWithMess( WinbmParaInspectUser, "検査パラメータ設定", "Setting Inspect Parameter");
		}

		if( res == 1 ) {
			// --- シャッタスピードの設定
			R_CS_SetShutterSpeed(0, 0, SystemPara.nShutterSpeed);
			if(0 == (Option&NO_DRX124) && (LedControllMode == LED_CONTROLL_DRX124 )){
				R_drx124_set_time(HandleDrx124, 1, SystemPara.nShutterSpeed);
			}
			InitialInspectionData();
			//for(i=0;i<2;i++){
			//	if( OK!=ExchangeData( &MoldSurfacePara[i], i) ){
			//		if(R_get_language()) R_DrawPopupMessage( 1000000,"Data Error!!"  );
			//		else R_DrawPopupMessage( 1000000,"データが不正です"  );
			//		return;
			//	}
			//}

			// --- insp. Type name set
//			sprintf( Comment, "%s[%s%s%s]%s", ProductName, MarkChar, DataClass[InspClass], PilotDisp[PilotFlag], MachineName );
            sprintf( Comment, "%s[%s %s]%s", ProductName, MarkChar, DataClass[InspClass],  MachineName );
			res = R_CS_SetInspectionInfo(1, 1, 1, 1, 1, 1, InspectionName, Comment);
			if(res!=OK)		return ERROR;
			R_CS_DispTitle(INSPECT_MANUAL);

//			R_CS_CommandDisplayStatusMode(CommandStatus);
			R_datafile_save( JudgeFileData, JudgeFilename );			// 機種データファイル保存
			R_datafile_save( SystemFileData, SystemFileName );			// 機台データファイル保存
			R_datafile_save( TypeFileData, TypeFileName );				// 機台データファイル保存
			PkgParaSaveLog();											// ver3.02 saito
		}

		return OK;
	}


//================================================
//	判定値表示
//================================================

static int	JudgeValueDisp(int nMenuNumber)
{
	int						dc, dl;
	int						i;

	for(i=0;i<5;i++){
		R_DrawCls();

		dc=1;	dl=1;
		sprintf( Comment, "%s[%s %s]%s Spec %d", ProductName, MarkChar, DataClass[InspClass], MachineName, i );
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "%s %s Judgment value", Comment, InspectionName );

		dc=3;	dl=+2;


		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "No.  : Item              = X um( X bit)   Y um( Y bit)" );
		dl++;
		// G2 Insp. 判定値
		// --- G2 X size 最小値
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: G2 X size min     =%5d(%3dbit)  %5d(%3dbit)", InspParaM.G2Xmin[i].nCode,
																					InspParaM.G2Xmin[i].nXsize, InspPara.G2Xmin[i].nXsize,
																					InspParaM.G2Xmin[i].nYsize, InspPara.G2Xmin[i].nYsize );
		dl++;
		// --- G2 X size 最大値
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: G2 X size max     =%5d(%3dbit)  %5d(%3dbit)", InspParaM.G2Xmax[i].nCode,
																					InspParaM.G2Xmax[i].nXsize, InspPara.G2Xmax[i].nXsize,
																					InspParaM.G2Xmax[i].nYsize, InspPara.G2Xmax[i].nYsize );
		dl++;
		// --- G2 X max-min
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: G2 X max-min      =%5d(%3dbit)  %5d(%3dbit)", InspParaM.G2Xrange[i].nCode,
																					InspParaM.G2Xrange[i].nXsize, InspPara.G2Xrange[i].nXsize,
																					InspParaM.G2Xrange[i].nYsize, InspPara.G2Xrange[i].nYsize );
		dl++;
		// --- G2 X エッジカケ検査 判定値
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: G2 X edge         =%5d(%3dbit)  %5d(%3dbit)", InspParaM.G2Xedge[i].nCode,
																					InspParaM.G2Xedge[i].nXsize, InspPara.G2Xedge[i].nXsize,
																					InspParaM.G2Xedge[i].nYsize, InspPara.G2Xedge[i].nYsize );
		dl++;
		// --- G2 X Zure 検査 判定値[X,Y]
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: G2 Zure X min     =%5d(%3dbit)  %5d(%3dbit)", InspParaM.G2ZureXmin[i].nCode,
																					InspParaM.G2ZureXmin[i].nXsize, InspPara.G2ZureXmin[i].nXsize,
																					InspParaM.G2ZureXmin[i].nYsize, InspPara.G2ZureXmin[i].nYsize );
		dl++;
		// --- G2 X Zure 検査 判定値[X,Y]
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: G2 Zure X max     =%5d(%3dbit)  %5d(%3dbit)", InspParaM.G2ZureXmax[i].nCode,
																					InspParaM.G2ZureXmax[i].nXsize, InspPara.G2ZureXmax[i].nXsize,
																					InspParaM.G2ZureXmax[i].nYsize, InspPara.G2ZureXmax[i].nYsize );
		dl++;
		// --- G2 Y size 最小値
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: G2 Y size min     =%5d(%3dbit)  %5d(%3dbit)", InspParaM.G2Ymin[i].nCode,
																					InspParaM.G2Ymin[i].nXsize, InspPara.G2Ymin[i].nXsize,
																					InspParaM.G2Ymin[i].nYsize, InspPara.G2Ymin[i].nYsize );
		dl++;
		// --- G2 Y size 最大値[X,Y]
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: G2 Y size max     =%5d(%3dbit)  %5d(%3dbit)", InspParaM.G2Ymax[i].nCode,
																					InspParaM.G2Ymax[i].nXsize, InspPara.G2Ymax[i].nXsize,
																					InspParaM.G2Ymax[i].nYsize, InspPara.G2Ymax[i].nYsize );
		dl++;
		// --- G2 Y max-min[X,Y]
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: G2 Y max-min      =%5d(%3dbit)  %5d(%3dbit)", InspParaM.G2Yrange[i].nCode,
																					InspParaM.G2Yrange[i].nXsize, InspPara.G2Yrange[i].nXsize,
																					InspParaM.G2Yrange[i].nYsize, InspPara.G2Yrange[i].nYsize );
		dl++;
		// --- G2 Y エッジカケ検査 判定値[X,Y]
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: G2 Y edge         =%5d(%3dbit)  %5d(%3dbit)", InspParaM.G2Yedge[i].nCode,
																					InspParaM.G2Yedge[i].nXsize, InspPara.G2Yedge[i].nXsize,
																					InspParaM.G2Yedge[i].nYsize, InspPara.G2Yedge[i].nYsize );
		dl++;
		// --- G2 Y Zure 検査 判定値[X,Y]
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: G2 Zure Y min     =%5d(%3dbit)  %5d(%3dbit)", InspParaM.G2ZureYmin[i].nCode,
																					InspParaM.G2ZureYmin[i].nXsize, InspPara.G2ZureYmin[i].nXsize,
																					InspParaM.G2ZureYmin[i].nYsize, InspPara.G2ZureYmin[i].nYsize );
		dl++;
		// --- G2 Y Zure 検査 判定値[X,Y]
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: G2 Zure Y max     =%5d(%3dbit)  %5d(%3dbit)", InspParaM.G2ZureYmax[i].nCode,
																					InspParaM.G2ZureYmax[i].nXsize, InspPara.G2ZureYmax[i].nXsize,
																					InspParaM.G2ZureYmax[i].nYsize, InspPara.G2ZureYmax[i].nYsize );
		dl++;
		// --- G2 Y Sdl 検査 判定値[X,Y]
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: G2 Y SDtoG2       =%5d(%3dbit)  %5d(%3dbit)", InspParaM.G2YSdl[i].nCode,
																					InspParaM.G2YSdl[i].nXsize, InspPara.G2YSdl[i].nXsize,
																					InspParaM.G2YSdl[i].nYsize, InspPara.G2YSdl[i].nYsize );
		dl++;
		// --- G2 Y Apl 検査 判定値[X,Y]
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: G2 Y SDtoAp       =%5d(%3dbit)  %5d(%3dbit)", InspParaM.G2YApl[i].nCode,
																					InspParaM.G2YApl[i].nXsize, InspPara.G2YApl[i].nXsize,
																					InspParaM.G2YApl[i].nYsize, InspPara.G2YApl[i].nYsize );
		dl++;
		// --- Overcoat X カケ検査 判定値[X,Y]
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Overcoat X size   =%5d(%3dbit)  %5d(%3dbit)", InspParaM.OCoatXsize[i].nCode,
																					InspParaM.OCoatXsize[i].nXsize, InspPara.OCoatXsize[i].nXsize,
																					InspParaM.OCoatXsize[i].nYsize, InspPara.OCoatXsize[i].nYsize );
		dl++;
		// --- Overcoat Y カケ検査 判定値[X,Y]
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Overcoat Y size   =%5d(%3dbit)  %5d(%3dbit)", InspParaM.OCoatYsize[i].nCode,
																					InspParaM.OCoatYsize[i].nXsize, InspPara.OCoatYsize[i].nXsize,
																					InspParaM.OCoatYsize[i].nYsize, InspPara.OCoatYsize[i].nYsize );
		dl++;
		// --- Overcoat XYカケ検査 判定値[X,Y]
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Overcoat XYsize   =%5d(%3dbit)  %5d(%3dbit)", InspParaM.OCoatXYsize[i].nCode,
																					InspParaM.OCoatXYsize[i].nXsize, InspPara.OCoatXYsize[i].nXsize,
																					InspParaM.OCoatXYsize[i].nYsize, InspPara.OCoatXYsize[i].nYsize );
		dl++;
		// G2 position
		// --- G2 X position min
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: G2 X pos. min     =%5d(%3dbit)  %5d(%3dbit)", InspParaM.G2PosXmin[i].nCode,
																					InspParaM.G2PosXmin[i].nXsize, InspPara.G2PosXmin[i].nXsize,
																					InspParaM.G2PosXmin[i].nYsize, InspPara.G2PosXmin[i].nYsize );
		dl++;
		// --- G2 X position max
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: G2 X pos. max     =%5d(%3dbit)  %5d(%3dbit)", InspParaM.G2PosXmax[i].nCode,
																					InspParaM.G2PosXmax[i].nXsize, InspPara.G2PosXmax[i].nXsize,
																					InspParaM.G2PosXmax[i].nYsize, InspPara.G2PosXmax[i].nYsize );
		dl++;
		// G2 Y position min
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: G2 Y pos. min     =%5d(%3dbit)  %5d(%3dbit)", InspParaM.G2PosYmin[i].nCode,
																					InspParaM.G2PosYmin[i].nXsize, InspPara.G2PosYmin[i].nXsize,
																					InspParaM.G2PosYmin[i].nYsize, InspPara.G2PosYmin[i].nYsize );
		dl++;
		// G2 Y position max
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: G2 Y pos. max     =%5d(%3dbit)  %5d(%3dbit)", InspParaM.G2PosYmax[i].nCode,
																					InspParaM.G2PosYmax[i].nXsize, InspPara.G2PosYmax[i].nXsize,
																					InspParaM.G2PosYmax[i].nYsize, InspPara.G2PosYmax[i].nYsize );
		dl++;
		// Plating adhesion
		// Plating adhesion size
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Plating adhesion  =%5d(%3dbit)  %5d(%3dbit)", InspParaM.Plating[i].nCode,
																					InspParaM.Plating[i].nXsize, InspPara.Plating[i].nXsize,
																					InspParaM.Plating[i].nYsize, InspPara.Plating[i].nYsize );
		dl++;
		// Chip(Appearence) Insp. 判定値
		// --- Chip X size 最小値
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Chip X size min   =%5d(%3dbit)  %5d(%3dbit)", InspParaM.ChipXmin[i].nCode,
																					InspParaM.ChipXmin[i].nXsize, InspPara.ChipXmin[i].nXsize,
																					InspParaM.ChipXmin[i].nYsize, InspPara.ChipXmin[i].nYsize );
		dl++;
		// --- Chip X size 最大値
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Chip X size max   =%5d(%3dbit)  %5d(%3dbit)", InspParaM.ChipXmax[i].nCode,
																					InspParaM.ChipXmax[i].nXsize, InspPara.ChipXmax[i].nXsize,
																					InspParaM.ChipXmax[i].nYsize, InspPara.ChipXmax[i].nYsize );
		dl++;
		// --- Chip X max-min
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Chip X max-min    =%5d(%3dbit)  %5d(%3dbit)", InspParaM.ChipXrange[i].nCode,
																					InspParaM.ChipXrange[i].nXsize, InspPara.ChipXrange[i].nXsize,
																					InspParaM.ChipXrange[i].nYsize, InspPara.ChipXrange[i].nYsize );
		dl++;
		// --- Chip X エッジカケ検査 判定値
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Chip X edge       =%5d(%3dbit)  %5d(%3dbit)", InspParaM.ChipXedge[i].nCode,
																					InspParaM.ChipXedge[i].nXsize, InspPara.ChipXedge[i].nXsize,
																					InspParaM.ChipXedge[i].nYsize, InspPara.ChipXedge[i].nYsize );
		dl++;
		// --- Chip Y size 最小値
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Chip Y min        =%5d(%3dbit)  %5d(%3dbit)", InspParaM.ChipYmin[i].nCode,
																					InspParaM.ChipYmin[i].nXsize, InspPara.ChipYmin[i].nXsize,
																					InspParaM.ChipYmin[i].nYsize, InspPara.ChipYmin[i].nYsize );
		dl++;
		// --- Chip Y size 最大値
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Chip Y max        =%5d(%3dbit)  %5d(%3dbit)", InspParaM.ChipYmax[i].nCode,
																					InspParaM.ChipYmax[i].nXsize, InspPara.ChipYmax[i].nXsize,
																					InspParaM.ChipYmax[i].nYsize, InspPara.ChipYmax[i].nYsize );
		dl++;
		// --- Chip Y max-min
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Chip Y max-min    =%5d(%3dbit)  %5d(%3dbit)", InspParaM.ChipYrange[i].nCode,
																					InspParaM.ChipYrange[i].nXsize, InspPara.ChipYrange[i].nXsize,
																					InspParaM.ChipYrange[i].nYsize, InspPara.ChipYrange[i].nYsize );
		dl++;
		// --- Chip Y エッジカケ検査 判定値
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Chip Y edge       =%5d(%3dbit)  %5d(%3dbit)", InspParaM.ChipYedge[i].nCode,
																					InspParaM.ChipYedge[i].nXsize, InspPara.ChipYedge[i].nXsize,
																					InspParaM.ChipYedge[i].nYsize, InspPara.ChipYedge[i].nYsize );
		dl++;
		// Electrode Insp.
		// --- 電極寸法 X最小値
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Elec X min        =%5d(%3dbit)  %5d(%3dbit)", InspParaM.ElecXmin[i].nCode,
																					InspParaM.ElecXmin[i].nXsize, InspPara.ElecXmin[i].nXsize,
																					InspParaM.ElecXmin[i].nYsize, InspPara.ElecXmin[i].nYsize );
		dl++;
		// --- 電極寸法 X最大値
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Elec X max        =%5d(%3dbit)  %5d(%3dbit)", InspParaM.ElecXmax[i].nCode,
																					InspParaM.ElecXmax[i].nXsize, InspPara.ElecXmax[i].nXsize,
																					InspParaM.ElecXmax[i].nYsize, InspPara.ElecXmax[i].nYsize );
		dl++;
		// --- 電極寸法 X max-min
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Elec X max-min    =%5d(%3dbit)  %5d(%3dbit)", InspParaM.ElecXrange[i].nCode,
																					InspParaM.ElecXrange[i].nXsize, InspPara.ElecXrange[i].nXsize,
																					InspParaM.ElecXrange[i].nYsize, InspPara.ElecXrange[i].nYsize );
		dl++;
		// --- 電極寸法 X エッジカケ検査 判定値
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Elec X edge       =%5d(%3dbit)  %5d(%3dbit)", InspParaM.ElecXedge[i].nCode,
																					InspParaM.ElecXedge[i].nXsize, InspPara.ElecXedge[i].nXsize,
																					InspParaM.ElecXedge[i].nYsize, InspPara.ElecXedge[i].nYsize );
		dl++;
		// --- 電極寸法 Y最小値
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Elec Y min        =%5d(%3dbit)  %5d(%3dbit)", InspParaM.ElecYmin[i].nCode,
																					InspParaM.ElecYmin[i].nXsize, InspPara.ElecYmin[i].nXsize,
																					InspParaM.ElecYmin[i].nYsize, InspPara.ElecYmin[i].nYsize );
		dl++;
		// --- 電極寸法 Y最大値
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Elec Y max        =%5d(%3dbit)  %5d(%3dbit)", InspParaM.ElecYmax[i].nCode,
																					InspParaM.ElecYmax[i].nXsize, InspPara.ElecYmax[i].nXsize,
																					InspParaM.ElecYmax[i].nYsize, InspPara.ElecYmax[i].nYsize );
		dl++;
		// --- 電極寸法 Y max-min
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Elec Y max-min    =%5d(%3dbit)  %5d(%3dbit)", InspParaM.ElecYrange[i].nCode,
																					InspParaM.ElecYrange[i].nXsize, InspPara.ElecYrange[i].nXsize,
																					InspParaM.ElecYrange[i].nYsize, InspPara.ElecYrange[i].nYsize );
		dl++;
		// --- 電極寸法 Y エッジカケ検査 判定値
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Elec Y edge       =%5d(%3dbit)  %5d(%3dbit)", InspParaM.ElecYedge[i].nCode,
																					InspParaM.ElecYedge[i].nXsize, InspPara.ElecYedge[i].nXsize,
																					InspParaM.ElecYedge[i].nYsize, InspPara.ElecYedge[i].nYsize );
		dl++;
		// --- 電極幅左右寸法差
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Elec X sub        =%5d(%3dbit)  %5d(%3dbit)", InspParaM.ElecXsub[i].nCode,
																					InspParaM.ElecXsub[i].nXsize, InspPara.ElecXsub[i].nXsize,
																					InspParaM.ElecXsub[i].nYsize, InspPara.ElecXsub[i].nYsize );
		dl++;
		// --- 電極カケ検査判定値(%)
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Elec X kake       =%5d(%3dbit)  %5d(%3dbit)", InspParaM.ElecKake[i].nCode,
																					InspParaM.ElecKake[i].nXsize, InspPara.ElecKake[i].nXsize,
																					InspParaM.ElecKake[i].nYsize, InspPara.ElecKake[i].nYsize );
		dl++;
		// --- 電極変色判定値(%)
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Elec black   (％) =%5d(%3d)     %5d(%3d)", InspParaM.ElecBlack[i].nCode,
																					InspParaM.ElecBlack[i].nData1, InspPara.ElecBlack[i].nData1,
																					InspParaM.ElecBlack[i].nData2, InspPara.ElecBlack[i].nData2 );
		dl++;
		// --- 側面電極被り込み寸法 X最小値
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: Side Elec min     =%5d(%3dbit)  %5d(%3dbit)", InspParaM.SideElecXmin[i].nCode,
																					InspParaM.SideElecXmin[i].nXsize, InspPara.SideElecXmin[i].nXsize,
																					InspParaM.SideElecXmin[i].nYsize, InspPara.SideElecXmin[i].nYsize );
		dl++;
		// --- Ibutsu X 検査 判定値[X,Y]  2021-04-20<shio>
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: IBUTSU X size   =%5d(%3dbit)  %5d(%3dbit)", InspParaM.IbutsuXsize[i].nCode,
																					InspParaM.IbutsuXsize[i].nXsize, InspPara.IbutsuXsize[i].nXsize,
																					InspParaM.IbutsuXsize[i].nYsize, InspPara.IbutsuXsize[i].nYsize );
		dl++;
		// --- Ibutsu Y 検査 判定値[X,Y]
		R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, dc, dl, "NG %2d: IBUTSU Y size   =%5d(%3dbit)  %5d(%3dbit)", InspParaM.IbutsuYsize[i].nCode,
																					InspParaM.IbutsuYsize[i].nXsize, InspPara.IbutsuYsize[i].nXsize,
																					InspParaM.IbutsuYsize[i].nYsize, InspPara.IbutsuYsize[i].nYsize );
		dl++;
		// --- 入力待ち
		R_DrawMouseDeYesnoWithKeyboard();

	}
	R_DrawCls();

	return OK;

}	// JudgeValueDisp(int nMenuNumber) end

/**********************************************************
	関数名：	ＰａｒａｍｅｔｅｒＳｙｓＳｅｔ
	機  能：	システム設定
	形  式：	static void ParameterSysSet(void)
	引き数：	なし
	戻り値：	なし
	解  説：
	留意事項：
**********************************************************/

static int	ParameterSysSet(int nMenuNumber)
	{
		ratex =  SystemPara.nRateX; //9499
		ratey =  SystemPara.nRateY; //9499
		compare =  nJudgeMode; //9499
		int						res;
		char	UsbKeyInformation[1024];

		if((Option & REPI) != 0){
	        if( 0==(Option & NO_USBKEY) && OK!=UsbKeyCertification( UsbKeyInformation, 15 )) return ERROR;
		} else {
			if( 0==(Option & NO_USBKEY) && OK!=UsbKeyCertification_rist( &UsbKeyInfo ) ) return ERROR;	//2017.06.13
		}
		if( Option & ADMIN_MODE ){
			res = R_DrawWinbParameterWithMess( WinbmParaSystem, "システムパラメータ設定", "Setting System Parameter");
		} else {
			res = R_DrawWinbParameterWithMess( WinbmParaSystemUser, "システムパラメータ設定", "Setting System Parameter");
		}


		if( res == 1 ) {
			// --- シャッタスピードの設定
			R_CS_SetShutterSpeed(0, 0, SystemPara.nShutterSpeed);
			if(0 == (Option&NO_DRX124) && (LedControllMode == LED_CONTROLL_DRX124 )){
				R_drx124_set_time(HandleDrx124, 1, SystemPara.nShutterSpeed);
			}
			// --- 検査パラメータ初期化
			InitialInspectionData();


//9499-start

if ((instru != 0 && SystemPara.nRateX != ratex) || (instru != 0 && SystemPara.nRateY != ratey)){
R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- Modification Denied! Unauthorized USB KEY -- " );
SystemPara.nRateX = ratex;
SystemPara.nRateY = ratey;

		}		//9499-END


//			R_datafile_save(JudgeFileData, JudgeFilename);			// 機種データファイル保存
else{
			R_datafile_save(SystemFileData, SystemFileName);			// 機台データファイル保存
			R_datafile_save( TypeFileData, TypeFileName );				// 機台データファイル保存
			PkgParaSaveLog();											// ver1.04
		}
	}

//9499-start

		if (instru != 0 && nJudgeMode != compare){
				R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- Modification Denied! Unauthorized USB KEY -- " );
			  nJudgeMode = compare; //normal condition

			}

		if(nJudgeMode != compare) {
			R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- WARNING -- \nDo you want to modify the Inspection mode? \nYES:Left(Click) | NO:Right(Click)" );
			while(R_get_cursor() == NO_TOUCH);
			if( CANCEL == R_get_cursor()){		//Cancel == 2
				nJudgeMode = compare;			//if Cancel return
				R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, " Inspection Mode Modification Cancelled" );
			}

		}

switch( nJudgeMode ) {
case 0:
R_draw_popup_message( DRAW_GREEN, DISP_4, POPUP_WAIT_CLICK, "-- Inspection Mode: Normal Condition -- " );
break;

case 1:
R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- Inspection Mode: ALL GO -- " );
break;

case 2:
R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- Inspection Mode: ALL NG (NO WORK) -- " );
break;

case 3:
R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- Inspection Mode: ALL NG (REVERSE) -- " );
break;

case 4:
R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- Inspection Mode: ALL NG -- " );
break;
}
//9499-END
		return OK;
	}


/**********************************************************
	関数名：	ＬｅｄＡｄｊｕｓｔ
	機  能：	照明調整
	形  式：	static int LedAdjust(void)
	引き数：
	戻り値：	OK		正常終了
				ERROR	異常終了
	解  説：
	留意事項：

**********************************************************/
static int LedAdjust(int nMenuNumber)
{
	R_LED_ADJUST *pLedAdjustPara;
	int result;
	int val;
//	int nChangeOrNot = OFF;
	char	UsbKeyInformation[1024];

	if((Option & REPI) != 0){
		if( 0==(Option & NO_USBKEY) && OK!=UsbKeyCertification( UsbKeyInformation, 16 )) return ERROR;
	} else {
		if( 0==(Option & NO_USBKEY) && OK!=UsbKeyCertification_rist( &UsbKeyInfo ) ) return ERROR;	//2017.06.13
	}

	R_DrawCls();

	pLedAdjustPara = &LedAdjustPara;

	SampleAreaNum = 1;
	SampleArea[0].left = 250;   SampleArea[0].top    = 170;
	SampleArea[0].right= 390;   SampleArea[0].bottom = 290;
    R_LedAdjustSamplingAreaSet(&SampleArea[0], SampleAreaNum);

	result = R_LedAdjust(GPDAC_DSW, pLedAdjustPara);

//	R_LedAdjustSamplingAreaGet(&SampleArea, &SampleAreaNum);
	val = R_get_histgram(SampleArea[0].left, SampleArea[0].top, SampleArea[0].right, SampleArea[0].bottom, Gray_address[0]);
	if(( val < 160 ) && ( val > 120 )){
		R_DrawBox( DRAW_GREEN, SampleArea[0], GRAPH_DRAW, DASHED_LINE );
		sprintf( Comment, "Bright Std. PASS:%3d", val );
		DrawMessage( Comment, DRAW_GREEN, 2, 10 );
	}else{
		R_DrawBox( DRAW_RED, SampleArea[0], GRAPH_DRAW, DASHED_LINE );
		sprintf( Comment, "Bright Std. FAIL:%3d", val );
		DrawMessage( Comment, DRAW_RED, 2, 10 );
	}

//	if( LedAdjustPara != pLedAdjust ) nChangeOrNot = ON;
	if(result==YES){
//		R_datafile_save(JudgeFileData, JudgeFilename);			// 機種データファイル保存
		R_datafile_save(SystemFileData, SystemFileName);			// 機台データファイル保存
		PkgParaSaveLog();											// ver3.02 saito
	}

	return OK;
}

/**********************************************************
	関数名：	ＬｅｄＬｅｖｅ
	機  能：	照明調整
	形  式：	static int Led(void)
	引き数：
	戻り値：	OK		正常終了
				ERROR	異常終了
	解  説：
	留意事項：

**********************************************************/
static int LedLevel(int nMenuNumber)
{
	R_LED_ADJUST *pLedAdjustPara;
	int result;
	int val;
//	int nChangeOrNot = OFF;
	char	UsbKeyInformation[1024];

	if((Option & REPI) != 0){
		if( 0==(Option & NO_USBKEY) && OK!=UsbKeyCertification( UsbKeyInformation, 16 )) return ERROR;
	} else {
		if( 0==(Option & NO_USBKEY) && OK!=UsbKeyCertification_rist( &UsbKeyInfo ) ) return ERROR;	//2017.06.13
	}

	R_DrawCls();
	val=LedCurrent[0];
//	pLedAdjustPara = &LedAdjustPara;
	result = TWA_LED_set(MANUAL_GRAYADDRESS,TP_Bright_SetLevel,TP_Bright_SetPercent,&LedPara);

	if(result==0){
		//LedCurrent[0]=
		R_datafile_save(SystemFileData, SystemFileName);			// 機台データファイル保存
		PkgParaSaveLog();
	} else {
		LedCurrent[0]=val;
	}

	return OK;
}
//================================================
//	Fail 画像表示
//================================================

static int	DispFailImage(int nMenuNumber)
	{

	//	R_move_gray_memory( FAIL_GRAYADDRESS, BASE_GRAYADDRESS, 0, 0, FxSize-1, FySize-1 );
		R_move_gray_memory( FAIL_GRAYADDRESS, MANUAL_GRAYADDRESS, 0, 0, FxSize-1, FySize-1 );
		R_move_gray_memory( FAIL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );

		return OK;

	}


/**********************************************************
	関数名：	ＣａｐｔｕｒｅＬｅｄＡｄｊｕｓｔ
	機  能：	LED調整用画像取り込み(LED点灯なし、戻り値なし)
	形  式：	static void CaptureLedAdjust(void)
	引き数：	無し
	戻り値：	なし
	解  説：
	留意事項：
**********************************************************/
static void CaptureLedAdjust(int mode)
{
	int ret;

	ret=R_CS_Capture(0, LedCaptureNo, 0);
	if(OK==ret)		ret=R_CS_CaptureWait(0);
//	R_move_gray_memory( BASE_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );
	R_move_gray_memory( MANUAL_GRAYADDRESS, GrayP0, 0, 0, FxSize-1, FySize-1 );


}

/**********************************************************
	関数名：	ＩｎｉｔｉａｌＩｎｓｐｅｃｔｉｏｎＤａｔａ
	機  能：
	形  式：	static int InitialInspectionData(void)
	引き数：
	戻り値：
	解  説：
	留意事項：
**********************************************************/
static int	InitialInspectionData(void)
{

	if(OK!=ExchangeUnit(&InspParaM, &InspPara)){
		R_DrawPopupMessage( POPUP_WAIT_CLICK, " Data Error!! " );
		return ERROR;
	}

	return OK;
}


/**********************************************************
	関数名：	ＥｘｃｈａｎｇｅＵｎｉｔ
	機  能：
	形  式：	static int ExchangeUnit(INSPECT_PARAMETER *pSrcPara, INSPECT_PARAMETER *pDstPara)
	引き数：	INSPECT_PARAMETER *pSrcPara
				INSPECT_PARAMETER *pDstPara
	戻り値：
	解  説：
	留意事項：
**********************************************************/

static int	ExchangeUnit( INSPECT_PARAMETER *pSrcPara, INSPECT_PARAMETER *pDstPara )
{
    int i;

		// Check rate
			if( SystemPara.nRateX <= 0 )	return ERROR;
			if( SystemPara.nRateY <= 0 )	return ERROR;

		// --- Chip size
			ChipSize[DataXbit]				= (ChipSize[DataXum]				*1000+500)/SystemPara.nRateX;
			ChipSize[DataYbit]				= (ChipSize[DataYum]				*1000+500)/SystemPara.nRateY;
		// --- Chip corner size
			ChipCornerSize[DataXbit]		= (ChipCornerSize[DataXum]			*1000+500)/SystemPara.nRateX;
			ChipCornerSize[DataYbit]		= (ChipCornerSize[DataYum]			*1000+500)/SystemPara.nRateY;
		// --- Insp. area offset
			InspOcOffsetSize[DataXbit]		= (InspOcOffsetSize[DataXum]		*1000+500)/SystemPara.nRateX;
			InspOcOffsetSize[DataYbit]		= (InspOcOffsetSize[DataYum]		*1000+500)/SystemPara.nRateY;
			InspOcOffsetCor[DataXbit]		= (InspOcOffsetCor[DataXum]			*1000+500)/SystemPara.nRateX;
			InspOcOffsetCor[DataYbit]		= (InspOcOffsetCor[DataYum]			*1000+500)/SystemPara.nRateY;
			InspOcOffsetSizeBlack[DataXbit]	= (InspOcOffsetSizeBlack[DataXum]	*1000+500)/SystemPara.nRateX;	//v2.11
			InspOcOffsetSizeBlack[DataYbit]	= (InspOcOffsetSizeBlack[DataYum]	*1000+500)/SystemPara.nRateY;	//v2.11
			InspOcOffsetCorBlack[DataXbit]	= (InspOcOffsetCorBlack[DataXum]	*1000+500)/SystemPara.nRateX;	//v2.11
			InspOcOffsetCorBlack[DataYbit]	= (InspOcOffsetCorBlack[DataYum]	*1000+500)/SystemPara.nRateY;	//v2.11
			InspPlOffsetSize[DataXbit]		= (InspPlOffsetSize[DataXum]		*1000+500)/SystemPara.nRateX;
			InspPlOffsetSize[DataYbit]		= (InspPlOffsetSize[DataYum]		*1000+500)/SystemPara.nRateY;

	for(i=0;i<5;i++){
		// G2 Insp. 判定値
			// --- G2 X size 最小値
			pDstPara->G2Xmin[i].nCode		= pSrcPara->G2Xmin[i].nCode			= NG_G2_X_SIZE;
			pDstPara->G2Xmin[i].nXsize		= (pSrcPara->G2Xmin[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->G2Xmin[i].nYsize		= (pSrcPara->G2Xmin[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- G2 X size 最大値
			pDstPara->G2Xmax[i].nCode		= pSrcPara->G2Xmax[i].nCode			= NG_G2_X_SIZE;
			pDstPara->G2Xmax[i].nXsize		= (pSrcPara->G2Xmax[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->G2Xmax[i].nYsize		= (pSrcPara->G2Xmax[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- G2 X max-min
			pDstPara->G2Xrange[i].nCode		= pSrcPara->G2Xrange[i].nCode		= NG_G2_X_SIZE;
			pDstPara->G2Xrange[i].nXsize	= (pSrcPara->G2Xrange[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->G2Xrange[i].nYsize	= (pSrcPara->G2Xrange[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- G2 X エッジカケ検査 判定値
			pDstPara->G2Xedge[i].nCode		= pSrcPara->G2Xedge[i].nCode		= NG_G2_KAKE_X;
			pDstPara->G2Xedge[i].nXsize		= (pSrcPara->G2Xedge[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->G2Xedge[i].nYsize		= (pSrcPara->G2Xedge[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- G2 X ズレ検査 判定値
			pDstPara->G2ZureXmin[i].nCode   = pSrcPara->G2ZureXmin[i].nCode		= NG_G2_X_SIZE;
			pDstPara->G2ZureXmin[i].nXsize  = (pSrcPara->G2ZureXmin[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->G2ZureXmin[i].nYsize  = (pSrcPara->G2ZureXmin[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- G2 X ズレ検査 判定値
			pDstPara->G2ZureXmax[i].nCode   = pSrcPara->G2ZureXmax[i].nCode		= NG_G2_X_SIZE;
			pDstPara->G2ZureXmax[i].nXsize  = (pSrcPara->G2ZureXmax[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->G2ZureXmax[i].nYsize  = (pSrcPara->G2ZureXmax[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- G2 Y size 最小値
			pDstPara->G2Ymin[i].nCode		= pSrcPara->G2Ymin[i].nCode			= NG_G2_Y_SIZE;
			pDstPara->G2Ymin[i].nXsize		= (pSrcPara->G2Ymin[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->G2Ymin[i].nYsize		= (pSrcPara->G2Ymin[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- G2 Y size 最大値
			pDstPara->G2Ymax[i].nCode		= pSrcPara->G2Ymax[i].nCode			= NG_G2_Y_SIZE;
			pDstPara->G2Ymax[i].nXsize		= (pSrcPara->G2Ymax[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->G2Ymax[i].nYsize		= (pSrcPara->G2Ymax[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- G2 Y max-min
			pDstPara->G2Yrange[i].nCode		= pSrcPara->G2Yrange[i].nCode		= NG_G2_Y_SIZE;
			pDstPara->G2Yrange[i].nXsize	= (pSrcPara->G2Yrange[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->G2Yrange[i].nYsize	= (pSrcPara->G2Yrange[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- G2 Y エッジカケ検査 判定値
			pDstPara->G2Yedge[i].nCode		= pSrcPara->G2Yedge[i].nCode		= NG_G2_KAKE_Y;
			pDstPara->G2Yedge[i].nXsize		= (pSrcPara->G2Yedge[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->G2Yedge[i].nYsize		= (pSrcPara->G2Yedge[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- G2 Y ズレ検査 判定値
			pDstPara->G2ZureYmin[i].nCode   = pSrcPara->G2ZureYmin[i].nCode		= NG_G2_Y_SIZE;
			pDstPara->G2ZureYmin[i].nXsize  = (pSrcPara->G2ZureYmin[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->G2ZureYmin[i].nYsize  = (pSrcPara->G2ZureYmin[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- G2 Y ズレ検査 判定値
			pDstPara->G2ZureYmax[i].nCode   = pSrcPara->G2ZureYmax[i].nCode		= NG_G2_Y_SIZE;
			pDstPara->G2ZureYmax[i].nXsize  = (pSrcPara->G2ZureYmax[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->G2ZureYmax[i].nYsize  = (pSrcPara->G2ZureYmax[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- G2 Y Sdl検査 判定値
			pDstPara->G2YSdl[i].nCode       = pSrcPara->G2YSdl[i].nCode		    = NG_G2_Y_SIZE;
			pDstPara->G2YSdl[i].nXsize      = (pSrcPara->G2YSdl[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->G2YSdl[i].nYsize      = (pSrcPara->G2YSdl[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- G2 Y Apl検査 判定値
			pDstPara->G2YApl[i].nCode       = pSrcPara->G2YApl[i].nCode		    = NG_G2_Y_SIZE;
			pDstPara->G2YApl[i].nXsize      = (pSrcPara->G2YApl[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->G2YApl[i].nYsize      = (pSrcPara->G2YApl[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- Overcoat X カケ検査 判定値
			pDstPara->OCoatXsize[i].nCode	= pSrcPara->OCoatXsize[i].nCode		= NG_OCOAT_X;
			pDstPara->OCoatXsize[i].nXsize	= (pSrcPara->OCoatXsize[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->OCoatXsize[i].nYsize	= (pSrcPara->OCoatXsize[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- Overcoat Y カケ検査 判定値
			pDstPara->OCoatYsize[i].nCode	= pSrcPara->OCoatYsize[i].nCode		= NG_OCOAT_Y;
			pDstPara->OCoatYsize[i].nXsize	= (pSrcPara->OCoatYsize[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->OCoatYsize[i].nYsize	= (pSrcPara->OCoatYsize[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- Overcoat XYカケ検査 判定値
			pDstPara->OCoatXYsize[i].nCode	= pSrcPara->OCoatXYsize[i].nCode	= NG_OCOAT_XY;
			pDstPara->OCoatXYsize[i].nXsize	= (pSrcPara->OCoatXYsize[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->OCoatXYsize[i].nYsize	= (pSrcPara->OCoatXYsize[i].nYsize	*1000+500)/SystemPara.nRateY;
		// G2 position
			// --- G2 X positio
			pDstPara->G2PosXmin[i].nCode	= pSrcPara->G2PosXmin[i].nCode		= NG_G2_POS_X;
			pDstPara->G2PosXmin[i].nXsize	= (pSrcPara->G2PosXmin[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->G2PosXmin[i].nYsize	= (pSrcPara->G2PosXmin[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- G2 X positio
			pDstPara->G2PosXmax[i].nCode	= pSrcPara->G2PosXmax[i].nCode		= NG_G2_POS_X;
			pDstPara->G2PosXmax[i].nXsize	= (pSrcPara->G2PosXmax[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->G2PosXmax[i].nYsize	= (pSrcPara->G2PosXmax[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- G2 Y positio
			pDstPara->G2PosYmin[i].nCode	= pSrcPara->G2PosYmin[i].nCode		= NG_G2_POS_Y;
			pDstPara->G2PosYmin[i].nXsize	= (pSrcPara->G2PosYmin[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->G2PosYmin[i].nYsize	= (pSrcPara->G2PosYmin[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- G2 Y positio
			pDstPara->G2PosYmax[i].nCode	= pSrcPara->G2PosYmax[i].nCode		= NG_G2_POS_Y;
			pDstPara->G2PosYmax[i].nXsize	= (pSrcPara->G2PosYmax[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->G2PosYmax[i].nYsize	= (pSrcPara->G2PosYmax[i].nYsize	*1000+500)/SystemPara.nRateY;
		// Plating adhesion
			// --- Plating adhesion size
			pDstPara->Plating[i].nCode		= pSrcPara->Plating[i].nCode		= NG_PL_ADHESION;
			pDstPara->Plating[i].nXsize		= (pSrcPara->Plating[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->Plating[i].nYsize		= (pSrcPara->Plating[i].nYsize		*1000+500)/SystemPara.nRateY;
		// Chip(Appearence) Insp. 判定値
			// --- Chip X size 最小値
			pDstPara->ChipXmin[i].nCode		= pSrcPara->ChipXmin[i].nCode		= NG_CHIP_X_SIZE;
			pDstPara->ChipXmin[i].nXsize	= (pSrcPara->ChipXmin[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->ChipXmin[i].nYsize	= (pSrcPara->ChipXmin[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- Chip X size 最大値
			pDstPara->ChipXmax[i].nCode		= pSrcPara->ChipXmax[i].nCode		= NG_CHIP_X_SIZE;
			pDstPara->ChipXmax[i].nXsize	= (pSrcPara->ChipXmax[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->ChipXmax[i].nYsize	= (pSrcPara->ChipXmax[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- Chip X max-min
			pDstPara->ChipXrange[i].nCode	= pSrcPara->ChipXrange[i].nCode		= NG_CHIP_X_SIZE;
			pDstPara->ChipXrange[i].nXsize	= (pSrcPara->ChipXrange[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->ChipXrange[i].nYsize	= (pSrcPara->ChipXrange[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- Chip X エッジカケ検査 判定値
			pDstPara->ChipXedge[i].nCode	= pSrcPara->ChipXedge[i].nCode		= NG_CHIP_KAKE_X;
			pDstPara->ChipXedge[i].nXsize	= (pSrcPara->ChipXedge[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->ChipXedge[i].nYsize	= (pSrcPara->ChipXedge[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- Chip Y size 最小値
			pDstPara->ChipYmin[i].nCode		= pSrcPara->ChipYmin[i].nCode		= NG_CHIP_Y_SIZE;
			pDstPara->ChipYmin[i].nXsize	= (pSrcPara->ChipYmin[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->ChipYmin[i].nYsize	= (pSrcPara->ChipYmin[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- Chip Y size 最大値
			pDstPara->ChipYmax[i].nCode		= pSrcPara->ChipYmax[i].nCode		= NG_CHIP_Y_SIZE;
			pDstPara->ChipYmax[i].nXsize	= (pSrcPara->ChipYmax[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->ChipYmax[i].nYsize	= (pSrcPara->ChipYmax[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- Chip Y max-min
			pDstPara->ChipYrange[i].nCode	= pSrcPara->ChipYrange[i].nCode		= NG_CHIP_Y_SIZE;
			pDstPara->ChipYrange[i].nXsize	= (pSrcPara->ChipYrange[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->ChipYrange[i].nYsize	= (pSrcPara->ChipYrange[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- Chip Y エッジカケ検査 判定値
			pDstPara->ChipYedge[i].nCode	= pSrcPara->ChipYedge[i].nCode		= NG_CHIP_KAKE_Y;
			pDstPara->ChipYedge[i].nXsize	= (pSrcPara->ChipYedge[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->ChipYedge[i].nYsize	= (pSrcPara->ChipYedge[i].nYsize	*1000+500)/SystemPara.nRateY;
		// Electrode Insp.
			// --- 電極寸法 X最小値
			pDstPara->ElecXmin[i].nCode		= pSrcPara->ElecXmin[i].nCode		= NG_ELEC_X_SIZE;
			pDstPara->ElecXmin[i].nXsize	= (pSrcPara->ElecXmin[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->ElecXmin[i].nYsize	= (pSrcPara->ElecXmin[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- 電極寸法 X最大値
			pDstPara->ElecXmax[i].nCode		= pSrcPara->ElecXmax[i].nCode		= NG_ELEC_X_SIZE;
			pDstPara->ElecXmax[i].nXsize	= (pSrcPara->ElecXmax[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->ElecXmax[i].nYsize	= (pSrcPara->ElecXmax[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- 電極寸法 X max-min
			pDstPara->ElecXrange[i].nCode	= pSrcPara->ElecXrange[i].nCode		= NG_ELEC_X_SIZE;
			pDstPara->ElecXrange[i].nXsize	= (pSrcPara->ElecXrange[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->ElecXrange[i].nYsize	= (pSrcPara->ElecXrange[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- 電極寸法 X エッジカケ検査 判定値
			pDstPara->ElecXedge[i].nCode	= pSrcPara->ElecXedge[i].nCode		= NG_ELEC_EDGE_X;
			pDstPara->ElecXedge[i].nXsize	= (pSrcPara->ElecXedge[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->ElecXedge[i].nYsize	= (pSrcPara->ElecXedge[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- 電極寸法 Y最小値
			pDstPara->ElecYmin[i].nCode		= pSrcPara->ElecYmin[i].nCode		= NG_ELEC_Y_SIZE;
			pDstPara->ElecYmin[i].nXsize	= (pSrcPara->ElecYmin[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->ElecYmin[i].nYsize	= (pSrcPara->ElecYmin[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- 電極寸法 Y最大値
			pDstPara->ElecYmax[i].nCode		= pSrcPara->ElecYmax[i].nCode		= NG_ELEC_Y_SIZE;
			pDstPara->ElecYmax[i].nXsize	= (pSrcPara->ElecYmax[i].nXsize		*1000+500)/SystemPara.nRateX;
            pDstPara->ElecYmax[i].nYsize	= (pSrcPara->ElecYmax[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- 電極寸法 X max-min
			pDstPara->ElecYrange[i].nCode	= pSrcPara->ElecYrange[i].nCode		= NG_ELEC_Y_SIZE;
			pDstPara->ElecYrange[i].nXsize	= (pSrcPara->ElecYrange[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->ElecYrange[i].nYsize	= (pSrcPara->ElecYrange[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- 電極寸法 X エッジカケ検査 判定値
			pDstPara->ElecYedge[i].nCode	= pSrcPara->ElecYedge[i].nCode		= NG_ELEC_EDGE_Y;
			pDstPara->ElecYedge[i].nXsize	= (pSrcPara->ElecYedge[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->ElecYedge[i].nYsize	= (pSrcPara->ElecYedge[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- 電極幅左右寸法差
			pDstPara->ElecXsub[i].nCode		= pSrcPara->ElecXsub[i].nCode		= NG_ELEC_X_SIZE;
			pDstPara->ElecXsub[i].nXsize	= (pSrcPara->ElecXsub[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->ElecXsub[i].nYsize	= (pSrcPara->ElecXsub[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- 電極カケ検査判定値
			pDstPara->ElecKake[i].nCode		= pSrcPara->ElecKake[i].nCode		= NG_ELEC_KAKE;
			pDstPara->ElecKake[i].nXsize	= (pSrcPara->ElecKake[i].nXsize		*1000+500)/SystemPara.nRateX;
			pDstPara->ElecKake[i].nYsize	= (pSrcPara->ElecKake[i].nYsize		*1000+500)/SystemPara.nRateY;
			// --- 電極変色判定値(%)
			pDstPara->ElecBlack[i].nCode	= pSrcPara->ElecBlack[i].nCode		= NG_ELEC_BLACK;
			pDstPara->ElecBlack[i].nData1	= pSrcPara->ElecBlack[i].nData1;
			pDstPara->ElecBlack[i].nData2	= pSrcPara->ElecBlack[i].nData2;
			// --- 側面電極被り込み寸法 X最小値
			pDstPara->SideElecXmin[i].nCode	= pSrcPara->SideElecXmin[i].nCode	= NG_SIDE_ELEC;
			pDstPara->SideElecXmin[i].nXsize= (pSrcPara->SideElecXmin[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->SideElecXmin[i].nYsize= (pSrcPara->SideElecXmin[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- C2電極被り込み寸法 X最小値
			pDstPara->ElecC2Xmin[i].nCode =	pSrcPara->ElecC2Xmin[i].nCode	= NG_SIDE_ELEC;
			pDstPara->ElecC2Xmin[i].nXsize= (pSrcPara->ElecC2Xmin[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->ElecC2Xmin[i].nYsize= (pSrcPara->ElecC2Xmin[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- C1目詰まり寸法 X最小値
			pDstPara->ElecC1Mezumari[i].nCode =	pSrcPara->ElecC1Mezumari[i].nCode	= NG_SIDE_ELEC;
			pDstPara->ElecC1Mezumari[i].nXsize= (pSrcPara->ElecC1Mezumari[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->ElecC1Mezumari[i].nYsize= (pSrcPara->ElecC1Mezumari[i].nYsize	*1000+500)/SystemPara.nRateY;

			// ---電極ー標印間寸法 X最小値
			pDstPara->MarkShift[i].nCode	= pSrcPara->MarkShift[i].nCode	= NG_MARKING;
			pDstPara->MarkShift[i].nData1	= (pSrcPara->MarkShift[i].nData1	*1000+500)/SystemPara.nRateX;
			pDstPara->MarkShift[i].nData2	= (pSrcPara->MarkShift[i].nData2 	*1000+500)/SystemPara.nRateX;
	}
            // --- レンズ焦点判定値
			pDstPara->Focus.nCode			= pSrcPara->Focus.nCode				= NG_FOCUS;
			pDstPara->Focus.nData1			= pSrcPara->Focus.nData1;
			pDstPara->Focus.nData2			= pSrcPara->Focus.nData2;
			// --- 照度管理
			pDstPara->Bright.nCode			= pSrcPara->Bright.nCode			= NG_LIGHTING;
			pDstPara->Bright.nData1			= pSrcPara->Bright.nData1;
			pDstPara->Bright.nData2			= pSrcPara->Bright.nData2;

			// --- Ibutsu X 検査 判定値		2021-04-20<shio>
			pDstPara->IbutsuXsize[i].nCode	= pSrcPara->IbutsuXsize[i].nCode		= NG_GLASS_IBUTSU;
			pDstPara->IbutsuXsize[i].nXsize	= (pSrcPara->IbutsuXsize[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->IbutsuXsize[i].nYsize = (pSrcPara->IbutsuXsize[i].nYsize	*1000+500)/SystemPara.nRateY;
			// --- Ibutsu Y 検査 判定値
			pDstPara->IbutsuYsize[i].nCode	= pSrcPara->IbutsuYsize[i].nCode		= NG_GLASS_IBUTSU;
			pDstPara->IbutsuYsize[i].nXsize	= (pSrcPara->IbutsuYsize[i].nXsize	*1000+500)/SystemPara.nRateX;
			pDstPara->IbutsuYsize[i].nYsize	= (pSrcPara->IbutsuYsize[i].nYsize	*1000+500)/SystemPara.nRateY;

		return OK;

}	// ExchangeUnit( INSPECT_PARAMETER *pSrcPara, INSPECT_PARAMETER *pDstPara ) end


//================================================
//		Check Alarm
//================================================

void	TP_CheckAlarm( void )
	{
		int						res		= OK;

		// 照度監視アラーム
			if((TP_Bright_Stop==ON)&&(res==OK)) {
				if( JPN_DISP == R_get_language() )	sprintf( Comment, "照明異常\n 認識窓を清掃してください" );
				else								sprintf( Comment, "Illumination is abnormal\nClean Cover Glass" );
				res = ERROR2;
			}
		// ガラス異物監視アラーム   2021.05.14<shio>
			if((TWA_Ibutsu_Stop==ON)&&(res==OK)) {
				if( JPN_DISP == R_get_language() )	sprintf( Comment, "ガラス異物\n 認識窓を清掃してください" );
				else								sprintf( Comment, "IBUTSU on the Glass \nClean Cover Glass" );
				res = ERROR2;
			}

		// アラーム信号出力
			if(res!=OK) {
				if((Option & NO_DRCY80)==0){
					OutputAlarmOn();
				}
				DrawMessage( Comment, DRAW_RED, 2, 0 );
				// アラームクリア
				switch( res ) {
					case ERROR2:
						TP_Bright_Stop = OFF;
						TWA_Ibutsu_Stop = OFF;			//2021.05.14<shio>
						break;
					default:
						break;
				}
				if((Option & NO_DRCY80)==0){
					OutputAlarmOff();
				}
			}

	}	// TP_CheckAlarm( void ) end


/******************************************************************************
	関数名：	Ｉｎｓｐｅｃｔ
	機  能：
	形  式：	int Inspect( int mode, R_CS_RESULT_DATA *Result )
	引き数：	int mode	検査モード
								INSPECT_AUTO	オート稼動
								INSPECT_MANUAL	マニュアル稼動
								INSPECT_STEP	ステップ判定
				R_CS_RESULT_DATA *Result
	戻り値：	OK		正常終了
				ERROR	異常終了
	解  説：
	留意事項：

typedef struct {
	int		nCode;					//結果フラグ(0がGO、1以上がNGコード)
	char	sNgComment[16];			//NG結果表示コメント
	char	sImageComment[128];		//画像ファイルに埋め込むコメント
}R_CS_RESULT_DATA;

		// 画像がGray_address[BASE_GRAYADDRESS]に取り込まれています

******************************************************************************/

int		Inspect( R_CS_RESULT_DATA *pCsResult )
	{
		int						res;
		int						ans_Ma; 			//marking addition
		int						xmin, xmax, xsize;
		int						ymin, ymax, ysize;
		int						side,elec=0;
		int						i_mode	= R_CS_GetInspectMode();
		int                     pt[10];
        int                     (*InspMarkFunc)(void);
        R_RECT					area;
		int						i;
//		int						bgrayflag;

		for(i=0;i<20;i++)	twaTime[1].nTime[i]=0;
		twa_TimeStart( &twaTime[1] );
		// Initial
			// --- Check initial setting
			if( OK != InitializeFlag ) {
				pCsResult->nCode = NG_CANT_INSP;
				DrawMessage( "Please set up inspection specification.", DRAW_RED, 0, 0 );
				return ERROR;
			}
			// --- Inspection
//			InspGrayPn	= BASE_GRAYADDRESS;
			InspBinPn	= BinP1;

			//インデックステーブルエッジ検出
			// Index_pos,IGudePos 裏面検査でインデックスサーチを行わない為、外郭の仮の値を初期値として代入
			Index_pos[0]=0        +20;  // 20171025  裏面検査でのエリア外はみ出し防止 tanaka
			Index_pos[1]=FySize-1 -20;  // 20171025  裏面検査でのエリア外はみ出し防止 tanaka
			Index_pos[2]=0;
			Index_pos[3]=0;
			Index_pos[4]=0;
			Index_pos[5]=0;
			IGuidePos[DataX]=600;	// DataX =0 左側 FxSize-1;			// Nitto
			IGuidePos[1]    =0;		// Weld
			if(bIsNitto && ((Option & BACKINSP_MODE) ==0)){		// 表面検査のみインデックス認識する
				switch( TWA_Index_search() ) {
					case OK:
						break;
					case ERROR:
						pCsResult->nCode = NG_CANT_INSP;
						return ERROR;
						break;
					case ERROR2:
//						return( TWA_Hyouri_Insp() );
						pCsResult->nCode = NG_CANT_INSP;
						return ERROR;
						break;
				}
			}

			// Poket Check    //2012.04.19
			if( Poket_flag == 1 ){
				pCsResult->nCode = NG_NO_WORK;
				return ERROR;
			}

		// Bright check
			if( OK != TP_CheckBright() ) {
				pCsResult->nCode = NG_LIGHTING;
				return ERROR;
			}

			twa_TimeRead( &twaTime[1],0 );

		// Index guide 位置サーチ
			InspBinPn	= BinP1;
			if(!bIsNitto){
				if( (1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE)) ){		// v2.20
					if( TP_SearchGuideFmRight() != OK ) {
						pCsResult->nCode = NG_CANT_INSP;
						return ERROR;
					}
				}else{
					if( TP_SearchGuide() != OK ) {
						pCsResult->nCode = NG_CANT_INSP;
						return ERROR;
					}
				}
			}

			twa_TimeRead( &twaTime[1],1 );

		// Index table level
			if( TP_IndexTableBright() != OK ){
				pCsResult->nCode = NG_CANT_INSP;
				return ERROR;
			}

			twa_TimeRead( &twaTime[1],2 );

		// 電極位置検出
			InspBinPn	= BinP1;
			switch( TP_SearchElect() ) {
				case OK:
					break;
				case ERROR:
					if(bIsNitto){
						if((Option & BACKINSP_MODE) ==0){
								pCsResult->nCode = NG_OCOAT_XY;	//20170913
						} else {
								pCsResult->nCode = NG_ELEC_Y_SIZE;
						}
					} else		pCsResult->nCode = NG_CANT_INSP;
					return ERROR;
					break;
				case ERROR2:	// NO Work
					pCsResult->nCode = NG_NO_WORK;
					return ERROR;
					break;
				case ERROR3:	// Reverse
					if(bIsNitto){
								pCsResult->nCode = NG_OCOAT_XY;	//20170913
					} else		pCsResult->nCode = NG_REVERSE;
					return ERROR;
					break;
				case ERROR4:	// electrode X size error
					pCsResult->nCode = NG_ELEC_X_SIZE;
					return ERROR;
					break;
				case ERROR5:	// electrode Y size error
					pCsResult->nCode = NG_ELEC_Y_SIZE;
					return ERROR;
					break;
				case ERROR6:	// On the galss IBUTSU error
					pCsResult->nCode = NG_GLASS_IBUTSU;			//2021.05.14<shio>
					return ERROR;
					break;
				default:
					pCsResult->nCode = NG_CANT_INSP;
					return ERROR;
					break;
			}
			//v2.20 if(!bIsLTR50){
			if( (!bIsLTR50) && (!bIsLTR18) ){
				//NORMAL
				G2_pos[0]	= ElecPosRough[DataLeft][2];
				G2_pos[1]	= ElecPosRough[DataRight][0];
				G2_pos[2]   = (ElecPosRough[DataLeft][PosYmin]+ElecPosRough[DataRight][PosYmin])/2;
				G2_pos[3]   = (ElecPosRough[DataLeft][PosYmax]+ElecPosRough[DataRight][PosYmax])/2;

				Ap_pos[0]	= ElecPosRough[DataLR][PosXmin];
				Ap_pos[1]	= ElecPosRough[DataLR][PosXmax];
				Ap_pos[2]	= ElecPosRough[DataLeft][PosYmin];
				Ap_pos[3]	= ElecPosRough[DataLeft][PosYmax];
				Ap_pos[4]	= ElecPosRough[DataRight][PosYmin];
				Ap_pos[5]	= ElecPosRough[DataRight][PosYmax];
			} else {
				// LTR
                G2_pos[0]	= (ElecPosRough[DataUpper][PosXmin]+ElecPosRough[DataLower][PosXmin])/2;
				G2_pos[1]	= (ElecPosRough[DataUpper][PosXmax]+ElecPosRough[DataLower][PosXmax])/2;
				G2_pos[2]   =  ElecPosRough[DataUpper][PosYmax];
				G2_pos[3]   =  ElecPosRough[DataLower][PosYmin];

				Ap_pos[0]	= ElecPosRough[DataUpper][PosXmin];
				Ap_pos[1]	= ElecPosRough[DataUpper][PosXmax];
				Ap_pos[2]	= ElecPosRough[DataLower][PosXmin];
				Ap_pos[3]	= ElecPosRough[DataLower][PosXmax];
                Ap_pos[4]	= ElecPosRough[DataUL][PosYmin];
				Ap_pos[5]	= ElecPosRough[DataUL][PosYmax];
			}
			twa_TimeRead( &twaTime[1],3 );


			// 端子部、セラミック（オーバーコート部）明るさモニタ
			if(bIsNitto){		//
				int El_xs,El_xe,El_ys,El_ye;
				int G2_xs,G2_xe,G2_ys,G2_ye;
				int El_aveGL,G2_aveG,El_aveGR;
				int n = light_monitor_count % MAX_LIGHT_MONI;
				int j;
				// 左端子
				El_xs = ElecPosRough[DataLeft][PosXmin]+5;
				El_xe = ElecPosRough[DataLeft][PosXmax]-5;
				if(El_xs < 0)	El_xs = 0;
				if(El_xs > El_xe)	El_xe=El_xs+5;
				El_ys = ElecPosRough[DataLeft][PosYmin]+5;
				El_ye = ElecPosRough[DataLeft][PosYmax]-5;
				if(El_ys < 0)		El_ys = 0;
				if(El_ys > El_ye)	El_ye=El_ys+5;
				El_aveL[n] = R_get_histgram(El_xs,El_ys,El_xe,El_ye,Gray_address[InspGrayPn]);
				// 右端子
				El_xs = ElecPosRough[DataRight][PosXmin]+5;
				El_xe = ElecPosRough[DataRight][PosXmax]-5;
				if(El_xs < 0)	El_xs = 0;
				if(El_xs > El_xe)	El_xe=El_xs+5;
				El_ys = ElecPosRough[DataRight][PosYmin]+5;
				El_ye = ElecPosRough[DataRight][PosYmax]-5;
				if(El_ys < 0)		El_ys = 0;
				if(El_ys > El_ye)	El_ye=El_ys+5;
				El_aveR[n] = R_get_histgram(El_xs,El_ys,El_xe,El_ye,Gray_address[InspGrayPn]);
				// G2部分
				G2_xs = ElecPosRough[DataLeft][PosXmax]+5;
				G2_xe = ElecPosRough[DataRight][PosXmin]-5;
				if(G2_xs < 0)	    G2_xs = 0;
				if(G2_xs > G2_xe)	G2_xe=G2_xs+5;
				G2_ys = ElecPosRough[DataLR][PosYmin]+5;
				G2_ye = ElecPosRough[DataLR][PosYmax]-5;
				if(G2_ys < 0)		G2_ys = 0;
				if(G2_ys > El_ye)	G2_ye=G2_ys+5;
				G2_ave[n] = R_get_histgram(G2_xs,G2_ys,G2_xe,G2_ye,Gray_address[InspGrayPn]);

				light_monitor_count++;
				if(light_monitor_count >= MAX_LIGHT_MONI){
					El_aveGL = El_aveGR = G2_aveG = 0;
					for(j=n;j<n+MAX_LIGHT_MONI;j++){
						El_aveGL += El_aveL[j%MAX_LIGHT_MONI];
						El_aveGR += El_aveR[j%MAX_LIGHT_MONI];
						G2_aveG  += G2_ave[j%MAX_LIGHT_MONI];
					}
					El_aveGL /= MAX_LIGHT_MONI;
					El_aveGR /= MAX_LIGHT_MONI;
					G2_aveG  /= MAX_LIGHT_MONI;
					R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 5, 30 ,"Bright Level  :%3d (%3d) %3d (%3d) %3d (%3d)   %d" , El_aveGL,TP_Bright_LowerLimit[0],G2_aveG,TP_Bright_LowerLimit[1],El_aveGR,TP_Bright_LowerLimit[2],light_monitor_count);
					if(TP_Bright_LowerLimit[0]>0){
						if(	El_aveGL < TP_Bright_LowerLimit[0]
						||	G2_aveG  < TP_Bright_LowerLimit[1]
						||	El_aveGR < TP_Bright_LowerLimit[2] ){
							R_DrawPopupMessageWithPara(POPUP_WAIT_CLICK,DISP_4,DRAW_RED,"illumination abnormal\nPlease clean the glass\n<clear: mouse click>");
							light_monitor_count = 0;
							pCsResult->nCode = NG_LIGHTING;
							return ERROR;
						}
					}
				}
			}

			if(bIsNitto && ((Option & BACKINSP_MODE) !=0)){		// 裏面検査のみインデックス認識する
				Index_pos[0]=ElecPosRough[DataLeft][PosYmin]-50;
				Index_pos[1]=ElecPosRough[DataLeft][PosYmax]+50;
				IGuidePos[DataX]=ElecPosRough[DataLR][PosXmax]+20;	// DataX =0 左側 FxSize-1;			// Nitto
				IGuidePos[1]    =ElecPosRough[DataLR][PosXmin]-20;	// Weld
				if(Index_pos[0] < 20)	        Index_pos[0]=20;
				if(Index_pos[1] > FySize-20)    Index_pos[1]=FySize-20;
				if(IGuidePos[DataX] > 600)		IGuidePos[DataX]=600;
				if(IGuidePos[1] < 0)			IGuidePos[1]=0;
			}

	//Ｇ２　エッジ検出
		//指定エリアのヒストグラムを取る。
/*
		if(OK==R_bgray_thresholding_open(THRESH_DEPTH_8BIT))	bgrayflag = 1;

		R_get_histgram( (G2_pos[0]+G2_pos[1])/2-(G2_pos[1]-G2_pos[0])/4, G2_pos[2]-20, (G2_pos[0]+G2_pos[1])/2+(G2_pos[1]-G2_pos[0])/4, G2_pos[3]+20, Gray_address[InspGrayPn] );
		g2level = R_bgray_thresholding( Bright, 50, 255, THRESH_OTSU );
		//	if( i_mode & INSPECT_STEP ) {
		{
				R_move_gray_memory( InspGrayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				area.left	= (G2_pos[0]+G2_pos[1])/2-(G2_pos[1]-G2_pos[0])/4;
				area.right	= (G2_pos[0]+G2_pos[1])/2+(G2_pos[1]-G2_pos[0])/4;
				area.top	= G2_pos[2]-20;
				area.bottom	= G2_pos[3]+20;
				R_DrawBox( DRAW_YELLOW, area, GRAPH_DRAW, SOLID_LINE );
				sprintf( Comment, "Insp. level (Ocoat:%d)", g2level );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				R_DrawCls();
			}
		if(bgrayflag)	R_bgray_thresholding_close();
*/

		//v2.20 if(!bIsLTR50){
		if( (!bIsLTR50) && (!bIsLTR18) ){
			G2EsLevelSet(0);
			g2level = G2EsLevel.nData;
	//		g2level = GetBinG2Rough(InspGrayPn);
	//		g2level = 80;
			BinClsPn( InspBinPn, 0, 0, FxSize-1, FySize-1 );
			R_gray2bin( g2level, 0, 0, IGuidePos[DataX]+1, FySize, Gray_address[InspGrayPn], Bin_address[InspBinPn] );
		} else {
			G2EsLevelSet(0);
			g2level = G2EsLevel.nData;
			BinClsPn( InspBinPn, 0, 0, FxSize-1, FySize-1 );
			R_gray2bin( g2level, 0, 0, FxSize/2, FySize, Gray_address[InspGrayPn], Bin_address[InspBinPn] );
			G2EsLevelSet(1);
			g2level = G2EsLevel.nData;
			R_gray2bin( g2level, FxSize/2, 0, FxSize/2, FySize, Gray_address[InspGrayPn], Bin_address[InspBinPn] );
		}

		//v2.20 if(!bIsLTR50){	// normal (MCR,ESR,SFR)
		if( (!bIsLTR50) && (!bIsLTR18) ){	// normal (MCR,ESR,SFR)
			if( TWA_G2_ES_search( 0 ) != OK ){
				pCsResult->nCode = NG_CANT_INSP;
				return ERROR;
			}
		} else {		// LTR
			if(bIsLTR18){									//v2.20
				if( TWA_G2_ES_searchLTR_l18( 0 ) != OK ){
					pCsResult->nCode = NG_CANT_INSP;
					return ERROR;
				}
			}else{
				if( TWA_G2_ES_searchLTR( 0 ) != OK ){
					pCsResult->nCode = NG_CANT_INSP;
					return ERROR;
				}
			}
		}
		twa_TimeRead( &twaTime[1],4 );

	//回帰係数計算
		if( KAIKI_keisuu(0) != OK ){
			pCsResult->nCode = NG_CANT_INSP;
			return( NG_CANT_INSP );
		}

	//平均値・標準偏差
		TWA_meansd();

	//回帰係数計算
		if( KAIKI_keisuu(1) != OK ){
			pCsResult->nCode = NG_CANT_INSP;
			return( NG_CANT_INSP );
		}

	// G2 std. edge position set
		if( G2StdEdgeSet() != OK ) return( NG_CANT_INSP );

		twa_TimeRead( &twaTime[1],5 );

		G2_pos[0] = hidari.kaiki_ex;
		G2_pos[1] = migi.kaiki_ex;
		G2_pos[2] = ue.kaiki_ey;
		G2_pos[3] = shita.kaiki_ey;

		//v2.11 if(bIsESR25 || bIsMCR10 || bIsMCR18 || bIsSFR25) {	//xxxxx
		if((bIsESR25 || bIsMCR10 || bIsMCR18 || bIsSFR25)&&((Option & BACKINSP_MODE) ==0)) {	//xxxxx
            InspBinPn	= BinP2;
            TWA_Insp_Ap(0);
		} else if( bIsLTR50){
			InspBinPn	= BinP2;
			TWA_Insp_ApLTR(0);
		} else if( bIsLTR18){										//v2.20
			InspBinPn	= BinP2;
			TWA_Insp_ApLTR_l18(0);
		}
		// チップ位置検出
            InspBinPn	= BinP1;
			switch( SearchChipPos() ) {
				case OK:
					break;
				case ERROR4:
					pCsResult->nCode = NG_NO_WORK;
					return ERROR;
					break;
                case ERROR3:
                    pCsResult->nCode = NG_ELEC_Y_SIZE;
                    return ERROR;
                    break;

                default:
					pCsResult->nCode = NG_CANT_INSP;
					return ERROR;
					break;
			}

		twa_TimeRead( &twaTime[1],6 );

		// 表/裏確認
			switch( InspReverse() ) {
				case OK:
					break;
				case ERROR:
					pCsResult->nCode = NG_CANT_INSP;
					return ERROR;
					break;
				case ERROR2:
					pCsResult->nCode = NG_REVERSE;
					return ERROR;
					break;
				default:
					pCsResult->nCode = NG_CANT_INSP;
					return ERROR;
					break;
			}

		twa_TimeRead( &twaTime[1],7 );

		// Inspection chip size
			switch( InspChipSize() ) {
				case OK:
					break;
				case ERROR:		// bolb No. error
				case ERROR4:	// regression analysis error
					pCsResult->nCode = NG_CANT_INSP;
					return ERROR;
					break;
				case ERROR2:	// Chip X size error
				case ERROR5:	// Chip X size error
					pCsResult->nCode = NG_CHIP_X_SIZE;
					return ERROR;
					break;
				case ERROR3:	// Chip Y size error
				case ERROR6:	// Chip Y size error
					pCsResult->nCode = NG_CHIP_Y_SIZE;
					return ERROR;
					break;
				case ERROR7:    // Elec Black Color
					pCsResult->nCode = NG_ELEC_BLACK;
					return ERROR;
					break;
				default:		// Can not insp.
					pCsResult->nCode = NG_CANT_INSP;
					return ERROR;
					break;
			}

        twa_TimeRead( &twaTime[1],13 );

		// Inspection Electrode( a foreign body [substance].)
			InspBinPn	= BinP1;
			res = InspElectrode();
			switch( res ) {
				case OK:
					break;
				case ERROR:		// Can't Inspection
					pCsResult->nCode = NG_CANT_INSP;
					return ERROR;
					break;
				case ERROR2:	// bin measure error
					pCsResult->nCode = NG_CANT_INSP;
					return ERROR;
					break;
				case ERROR3:	// electrode X size error
					pCsResult->nCode = NG_ELEC_X_SIZE;
					return ERROR;
					break;
				case ERROR4:	// electrode Y size error
					pCsResult->nCode = NG_ELEC_Y_SIZE;
					break;
					return ERROR;
				case ERROR5:	// Elec. Foreign substance X size Error
					pCsResult->nCode = NG_ELEC_KAKE;
					return ERROR;
				case ERROR6:	// Elec. Foreign substance Y size Error
					pCsResult->nCode = NG_ELEC_KAKE;
					return ERROR;
				case ERROR7:	// Elec. Foreign substance XandY size Error
					pCsResult->nCode = NG_ELEC_KAKE;
					return ERROR;
				default:
					if( res > 0 )	pCsResult->nCode = res;
					else			pCsResult->nCode = NG_CANT_INSP;
					return ERROR;
					break;
			}

            twa_TimeRead( &twaTime[1],14 );
			if(!bIsMCR01 && (Option & MECHA_CHECK)){
				goto ADJUST_MODE;
			}
			if((Option & BACKINSP_MODE)){
				goto NoMark;
			}

/***        MARKING ADDITION       ***/
			Ma_pos[0] = Ma_pos[1] = Ma_pos[2] = Ma_pos[3]=-1;
			if(bIsMCR03!=0){
                ans_Ma = TWA_kensa_m03();
            } else if(bIsMCR10!=0){
                ans_Ma = TWA_kensa_m10();
            } else if(bIsMCR18!=0){
                ans_Ma = TWA_kensa_m18();
            } else if(bIsESR25!=0 || bIsSFR25!=0){
                ans_Ma  = TWA_kensa_me25();
			} else if(bIsLTR50!=0){
				ans_Ma  = TWA_kensa_l50();
			} else if(bIsLTR18!=0){
				 nDispMode1 = nDispMode; //9499
				ans_Ma  = TWA_kensa_l18();							//v2.20
			} else if(bIsMCR01){
				goto NoMark;
			}

            twa_TimeRead( &twaTime[1],15 );

//			if( TWA_Judge_Ma != OK ) {
                switch( ans_Ma  ) {
					case ERROR:
					case ERROR4:
//						return( 9 );
						pCsResult->nCode = NG_MARKING;
						return ERROR;
						break;
 					case ERROR2:
					case ERROR3:
						pCsResult->nCode = NG_MARK_SEGMENT;
						return ERROR;
						break;
					case ERROR5:                                           //GEE V8.93P
//                        TWA_Judge_Ma = OK;
						pCsResult->nCode = NG_MARKING;
						return ERROR;
						break;
					case OK:
//                        TWA_Judge_Ma = OK;
                        break;
                    default:
						pCsResult->nCode = NG_MARKING;
						return ERROR;
						break;
				}
//				if ( ans_Ma == ERROR || ans_Ma == ERROR4 ) {
//                TWA_Judge_Ma = ERROR;
//				}
//			}

//			if(TWA_Judge_Ma != OK ) {
//					pCsResult->nCode = NG_MARKING;
//					return ERROR;
//			}

NoMark:
			// Inspection Overcoat( a foreign body [substance].)
				InspBinPn	= BinP2;
				res = InspOvercoat();
				switch( res ) {
					case OK:
						 break;
					case ERROR:		// Can't Inspection
						pCsResult->nCode = NG_CANT_INSP;
						return ERROR;

					default:
						if( res > 0 )	pCsResult->nCode = res;
						else			pCsResult->nCode = NG_CANT_INSP;
						return ERROR;
				}

			//v2.20 if( ((1==bIsSFR25)&&(Option & BACKINSP_MODE))||((1==bIsLTR50)&&(Option & BACKINSP_MODE)) ){
			if( ((1==bIsSFR25)&&(Option & BACKINSP_MODE))||((1==bIsLTR50)&&(Option & BACKINSP_MODE))||((1==bIsLTR18)&&(Option & BACKINSP_MODE)) ){
				InspBinPn	= BinP2;
				res = InspOvercoatBlack();
				switch( res ) {
					case OK:
						 break;
					case ERROR:		// Can't Inspection
						pCsResult->nCode = NG_CANT_INSP;
						return ERROR;

					default:
						if( res > 0 )	pCsResult->nCode = res;
						else			pCsResult->nCode = NG_CANT_INSP;
						return ERROR;
				}
			}

            twa_TimeRead( &twaTime[1],16 );

			if((Option & BACKINSP_MODE)){
				goto BACKINSP_END;
			}

//v2.20 if(bIsLTR50)	goto SKIPplat;
if((bIsLTR50)||(bIsLTR18))	goto SKIPplat;
			// Inspection Plating ( CB Side )
				InspBinPn	= BinP3;
				res = InspPlating();
				switch( res ) {
					case OK:
						break;
					case ERROR:		// Can't Inspection
						pCsResult->nCode = NG_CANT_INSP;
						return ERROR;

					default:
						if( res > 0 )	pCsResult->nCode = res;
						else			pCsResult->nCode = NG_CANT_INSP;
						return ERROR;
				}
SKIPplat:
			twa_TimeRead( &twaTime[1],12 );

		//Pinhole insp.
            Pinhole_insp();

		// C2電極被り込み幅検査
	//		if( Insp_C2_mode != 0 ) {
				//v2.20 if(!bIsLTR50){	//normal
				if( (!bIsLTR50) && (!bIsLTR18) ){	//normal
					if( TWA_InspC2Xsize() != OK ){
						pCsResult->nCode = NG_ELEC_X_SIZE;
						return ERROR;
					}
				} else {		//LTR
					if(bIsLTR18){										//v2.20
						if( TWA_InspC2Ysize_l18() != OK ){
							pCsResult->nCode = NG_ELEC_Y_SIZE;
							return ERROR;
						}
					}else{
						if( TWA_InspC2Ysize() != OK ){
							pCsResult->nCode = NG_ELEC_Y_SIZE;
							return ERROR;
						}
					}
				}
	//		}

		//C1 Mezumari inspection //<z3>
			//v2.20 if(!bIsLTR50){
			if((!bIsLTR50)&&(!bIsLTR18)){
				if( TWA_Mezu_Area() != OK ){
					pCsResult->nCode = NG_ELEC_X_SIZE;
					return ERROR;	//<z3>
				}
			}

		//v2.20 if(bIsESR25 || bIsMCR10 || bIsMCR18 || bIsSFR25 || bIsLTR50){	//xxxxx
		if(bIsESR25 || bIsMCR10 || bIsMCR18 || bIsSFR25 || bIsLTR50 || bIsLTR18){	//xxxxx
//            g2level = 75;
            InspBinPn	= BinP2;
            BinClsPn( InspBinPn, 0, 0, FxSize-1, FySize-1 );
            R_gray2bin( g2level, 0, 0, IGuidePos[DataX]+1, FySize, Gray_address[InspGrayPn], Bin_address[InspBinPn] );

            TWA_Insp_Ap(1);

			// G2 std. edge position set again  外形上下の傾きを基準とするG2ニジミが発生していた場合回帰直線はアテにならない 20170502 tanaka
			w.kaiki_b = (Ap_up.kaiki_b + Ap_lo.kaiki_b) /2;
			if( G2StdEdgeSet() != OK ) return( NG_CANT_INSP );


		//Ｇ２エッジデータ判定　Ｗ方向
			switch( G2_Judge_w_SDl() ) {
				case OK:
					break;
				case ERROR:
			//		return 21;	// data error
			//		break;
				case ERROR2:
			//		return 1;	// G2up SD to G2up data
			//		break;
				case ERROR3:
			//		return 2;	// G2lo SD to G2lo data
			//		break;
				default:
					pCsResult->nCode = NG_OCOAT_Y;
					return ERROR;
					break;
			}
			if(bIsLTR50){
				switch( G2_Judge_w_SDlLTR() ) {
					case OK:
						break;
					case ERROR:
					case ERROR2:
					case ERROR3:
					default:
						pCsResult->nCode = NG_OCOAT_X;
						return ERROR;
						break;
				}
			}
			if(bIsLTR18){
				switch( G2_Judge_w_SDlLTR_l18() ) { //9499-NOTE
					case OK:
						break;
					case ERROR:
					case ERROR2:
					case ERROR3:
					default:
						pCsResult->nCode = NG_OCOAT_X;
						return ERROR;
						break;
				}
			}
		}
		if(bIsESR25 || bIsMCR10 || bIsMCR18 || bIsSFR25 ){	//xxxxx
		//Ｇ２エッジデータ判定　Ｗ方向
			switch( G2_Judge_w_APl() ) {
				case OK:
					break;
				case ERROR:
		//			return 21;	// data error
		//			break;
				case ERROR2:
		//			return 1;	// Apup SD to G2up data
		//			break;
				case ERROR3:
		//			return 2;	// Aplo SD to G2lo data
		//			break;
				default:
					pCsResult->nCode = NG_OCOAT_Y;
					return ERROR;
					break;
			}
			//Ｇ２ズレ検査
			res = TWA_kensa_zure_w();
			switch( res ) {
				case OK:
					break;
				default:
					pCsResult->nCode = NG_OCOAT_Y;
					return ERROR;
			}
		}
		if( (bIsLTR50)||(bIsLTR18) ){
			switch( TWA_kensa_zure_l()){
				case OK:
					break;
				default:
					pCsResult->nCode = NG_G2_POS_X;
					return ERROR;
			}
		// C1電極間検査
			if(bIsLTR18){
				res= LTR_Judge_C1Space_l18();								//v2.20
			}else{
				res= LTR_Judge_C1Space();
			}
			switch( res ) {
				case OK:
					break;
				default:
					pCsResult->nCode = NG_ELEC_Y_SIZE;
					return ERROR;
			}
		}

ADJUST_MODE:
BACKINSP_END:
		// result
			pCsResult->nCode = OK;

			twa_TimeRead( &twaTime[1],13 );

			return OK;

	}	//	Inspect( int mode, R_CS_RESULT_DATA *pCsResult ) end


//================================================
//		Check Birght
//================================================

int		TP_CheckBright( void )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						xmin, xmax;
		int						ymin, ymax;
		int						res;
		int						posi_res_x;
		int						posi_res_y;
		int						value;

		R_RECT					area;

		// Check flag
			if( TP_BrightFlag == 0 ) {
				TP_Bright_d		= 0;
				TP_Bright_Stop	= OFF;
				TP_Bright_n		= 0;
				return OK;
			}

		// check area set
			xmin = TP_Bright_xs;
			xmax = xmin + TP_Bright_xl-1;
			if( xmax >= FxSize ) {
				xmax = FxSize-1;
				xmin = xmax-TP_Bright_xl+1;
			}
			ymin = TP_Bright_ys;
			ymax = ymin + TP_Bright_yl-1;
			if( ymax >= FySize ) {
				ymax = FySize-1;
				ymin = ymax-TP_Bright_yl+1;
			}

		// 明るさの計測
			TP_Bright_d = R_get_histgram( xmin, ymin, xmax, ymax, Gray_address[grayPn] );
			res = abs( TP_Bright_d - TP_Bright_st );

			// --- comment
			posi_res_x = 0;
			posi_res_y = 30;
			area.left  =xmin;
			area.right =xmax;
			area.top   =ymin;
			area.bottom=ymax;

			if( res > InspPara.Bright.nData1 ) {
				R_DrawBox( DRAW_RED, area, GRAPH_DRAW, DOTTED_LINE );
			} else {
				R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, DOTTED_LINE );
			}
			if( i_mode & INSPECT_STEP ) {
				R_DrawCls();
				R_move_gray_memory(grayPn,0,0,0,FxSize-1,FySize-1);
				// --- comment
				R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"[Index Guide Bright]");
				R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+2 ,"Bright Level  :%3d (%d)[limit difer %d]" ,TP_Bright_d,TP_Bright_st,InspPara.Bright.nData1 );
				if( res > InspPara.Bright.nData1 ) {
					R_DrawBox( DRAW_RED, area, GRAPH_DRAW, DOTTED_LINE );
				} else {
					R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, DOTTED_LINE );
				}
				while( !R_DrawGetCursor());
				R_DrawCls();
				while( R_DrawGetCursor());

			}

			R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y ,"Bright Level  :%3d (%d)[limit difer %d]" ,TP_Bright_d,TP_Bright_st,InspPara.Bright.nData1 );

			// 基準データと比較
			if(((i_mode&INSPECT_AUTO) || (Option&DEBUG_MODE)) && (0<TP_BrightFlag)) {
//			if((0<TP_BrightFlag)) {
				if( res > InspPara.Bright.nData1 ) {
					TP_Bright_n++;
					if( TP_Bright_n >= TP_BrightFlag )	TP_Bright_Stop = ON;
					return ERROR;
				} else {
					TP_Bright_n = 0;
				}
			} else {
				TP_Bright_Stop	= OFF;
				TP_Bright_n		= 0;
			}
		// result
			return OK;

	}	// TP_CheckBright( void ) end


//================================================
//	TP_IndexTableBright
//================================================

int		TP_IndexTableBright( void )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						xsize	= 300;
		int						ysize	= 50;
		int						offset_l= 10;
		int						sh_count, count;
		int						level;
        int                     posi_res_x, posi_res_y;
        R_RECT					area;

		// measure area set
			area.left	= 0;
			area.top	= 0;
			area.right	= area.left + xsize - 1;
			area.bottom	= area.top + ysize - 1;

		// 明るさの計測
            if( 0 > R_get_histgram( area.left, area.top, area.right, area.bottom, Gray_address[grayPn] ) ){
				// --- Viwe
				if( i_mode & INSPECT_STEP ) {
					BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
					// --- comment
					sprintf( Comment, " === ERROR === \n level setting error of an Index table " );
					DrawMessage( Comment, DRAW_RED, 2, 2 );
				}
				return ERROR;
			}

		// Index Table set
			sh_count = xsize*ysize*95/100;
			count = 0;
			for( level=0; level<256; level++ ){
				count += Bright[level];
				if( count > sh_count )	break;
			}
			if(!bIsNitto){
				IndexLevel.nUpper	= level + offset_l;
			} else {
				IndexLevel.nUpper = 0;		// Index が白っぽくみえるため変更
			}
			if( 255 < IndexLevel.nUpper )	IndexLevel.nUpper = 255;
			// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
				// --- comment
            //	sprintf( Comment, " Index table Level max : %3d ", IndexLevel.nUpper );
            //	DrawMessage( Comment, DRAW_GREEN, 2, 2 );
                posi_res_x = 0;
                posi_res_y = 30;

                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"[Index Table Bright]");
                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+2 ,"Indel table Level max  :%3d" ,IndexLevel.nUpper);
                R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, DOTTED_LINE );

                while( !R_DrawGetCursor());
                R_DrawCls();
                while( R_DrawGetCursor());


            }

			return OK;

	}	// TP_IndexTableBright( void ) end


//================================================
//	Search Index guide
//================================================

int		TP_SearchGuide( void )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						trans_mode;
		int						trans_pos[2];
		int						xmin, xmax, xsize, rx;
		int						ymin[2], ymax[2];
		int						guide_pos[2];
		int						dn, dmin;
		int						side;
		int						offset_x = 3;
		int						res = OK;


		// area set
			xmin	= FxSize/2;
			xmax	= FxSize-1;
			xsize	= xmax-xmin+1;
			//v2.20 if( (1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE)) ){
			//if( (1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE)) || (1==bIsLTR18)&&(0!=(Option & BACKINSP_MODE)) ){
			if( (1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE)) || (1==bIsLTR18) ){
				ymin[0]	= 55;
				ymax[0]	= 105;
				ymin[1]	= FySize-105;
				ymax[1]	= FySize-55;
				// Initial
				trans_mode = TRANS_MODE_BtoW | TRANS_CHOICE_N_BEGIN | TRANS_SMOOTH_MEDIAN;
			//	trans_mode = TRANS_MODE_BtoW | TRANS_CHOICE_MAX_AMP | TRANS_SMOOTH_MEDIAN;
			//	trans_mode = TRANS_MODE_BtoW | TRANS_CHOICE_N_BEGIN | TRANS_SMOOTH_AVE;
			//	trans_mode = TRANS_MODE_BtoW | TRANS_CHOICE_MAX_AMP | TRANS_SMOOTH_AVE;
			//	trans_mode = TRANS_MODE_BtoW | TRANS_CHOICE_MAX_AREA | TRANS_SMOOTH_MEDIAN;
			}else{
				ymin[0]	= 5;
				ymax[0]	= 55;
				ymin[1]	= FySize-55;
				ymax[1]	= FySize-5;
				// Initial
				trans_mode = TRANS_MODE_BtoW | TRANS_CHOICE_MAX_AMP | TRANS_SMOOTH_MEDIAN;
			}
		// Guide search
			// --- インデックステーブル、ガイドの隙間部分
			// --- projection_y
			side = 0;
			R_projection_y( grayPn, xmin, ymin[side], xmax, ymax[side], ProjectBuf1 );
			side = 1;
			R_projection_y( grayPn, xmin, ymin[side], xmax, ymax[side], ProjectBuf2 );
			for( dn=0; dn<xsize; dn++ )	ProjectBuf[dn] = ProjectBuf1[dn]+ProjectBuf2[dn];
			dmin = ProjectBuf[0];
			for( dn=1; dn<xsize; dn++ ) {
				if( dmin>ProjectBuf[dn] ) {
					dmin = ProjectBuf[dn];
					rx = xmin+dn;
				}
			}
			// --- インデックスガイド位置検出
			for( side=0; side<2; side++ ) {
				xmin	= rx;//-50;
				xmax	= rx+50;
				if( FxSize<=xmax )	xmax = FxSize-1;
				xsize	= xmax-xmin+1;
				// --- projection_y
				R_projection_y( grayPn, xmin, ymin[side], xmax, ymax[side], ProjectBuf );
				// --- edge search
				trans_pos[side] = R_trans_edge_detect_int( &IndexTrans, ProjectBuf, xsize, trans_mode );
				if( 0 < trans_pos[side] ) {
					guide_pos[side] = xmin+trans_pos[side]/1000;
				} else {
					guide_pos[side] = ERROR;
				}
			}

		// Data check
			if((0<guide_pos[0])&&(0<guide_pos[1])) {
                IGuidePos[DataX] = min( guide_pos[0], guide_pos[1])-offset_x;
			} else if( 0<guide_pos[0] ) {
                IGuidePos[DataX] = (guide_pos[0])-offset_x;
			} else if( 0<guide_pos[1] ) {
                IGuidePos[DataX] = (guide_pos[1])-offset_x;
			} else {
				IGuidePos[DataX] = FxSize-1;
				res = ERROR;
			}

		// サーチ位置表示
			// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
                // --- Index guide pos.
				if( res == OK ) {
					for( side=0; side<2; side++ ) {
						R_DrawLine( DRAW_GREEN, xmin, ymin[side], xmin, ymax[side], DRAW_REVERSE, SOLID_LINE );
						R_DrawLine( DRAW_GREEN, xmax, ymin[side], xmax, ymax[side], DRAW_REVERSE, SOLID_LINE );
						if( 0>guide_pos[side] ) continue;
						R_DrawLine( DRAW_RED, guide_pos[side], ymin[side], guide_pos[side], ymax[side], DRAW_REVERSE, DASHED_LINE );
					}
					R_DrawLine( DRAW_GREEN, IGuidePos[DataX], 0, IGuidePos[DataX], FySize-1, DRAW_REVERSE, DASHED_LINE );
					// --- comment
                    sprintf( Comment, "Index guide position guide_pos[0]=%d,guide_pos[1]=%d",guide_pos[0],guide_pos[1] );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					R_DrawCls();
				} else {
					// --- comment
					sprintf( Comment, "Index guide position is undetectable" );
					DrawMessage( Comment, DRAW_RED, 2, 2 );
				}
			}

		// result
			return(res);

	}	// TP_SearchGuide( void ) end


int		TP_SearchGuideFmRight( void )										// v2.20
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						trans_mode;
		int						trans_pos[2];
		int						xmin, xmax, xsize, rx;
		int						ymin[2], ymax[2];
		int						guide_pos[2];
		int						dn, dmin;
		int						side;
		int						offset_x = 3;
		int						res = OK;

		// area set
			xmin	= FxSize/2;
			xmax	= FxSize-20;
			xsize	= xmax-xmin+1;

				ymax[0]	= FySize/2-1;
				ymin[0]	= ymax[0]-100;
				ymin[1]	= FySize/2+1;
				ymax[1]	= ymin[1]+100;
				// Initial
			//	trans_mode = TRANS_MODE_WtoB | TRANS_CHOICE_N_END | TRANS_SMOOTH_MEDIAN;
				trans_mode = TRANS_MODE_BtoW | TRANS_CHOICE_N_END | TRANS_SMOOTH_MEDIAN;

		// Guide search
			// --- インデックステーブル、ガイドの隙間部分
			// --- projection_y
		//	side = 0;
		//	R_projection_y( grayPn, xmin, ymin[side], xmax, ymax[side], ProjectBuf1 );
		//	side = 1;
		//	R_projection_y( grayPn, xmin, ymin[side], xmax, ymax[side], ProjectBuf2 );
		//	for( dn=0; dn<xsize; dn++ )	ProjectBuf[dn] = ProjectBuf1[dn]+ProjectBuf2[dn];
		//	dmin = ProjectBuf[0];
		//	for( dn=1; dn<xsize; dn++ ) {
		//		if( dmin>ProjectBuf[dn] ) {
		//			dmin = ProjectBuf[dn];
		//			rx = xmin+dn;
		//		}
		//	}
			// --- インデックスガイド位置検出
			for( side=0; side<2; side++ ) {
		//		xmin	= rx;//-50;
		//		xmax	= rx+50;
		//		if( FxSize<=xmax )	xmax = FxSize-1;
		//		xsize	= xmax-xmin+1;
				// --- projection_y
				R_projection_y( grayPn, xmin, ymin[side], xmax, ymax[side], ProjectBuf );
				if( i_mode & INSPECT_STEP ) {
					R_RECT temp_area;
					temp_area.top = ymin[side];
					temp_area.bottom = ymax[side];
					temp_area.left = xmin;
					temp_area.right = xmax;
					R_draw_box(DRAW_GREEN, temp_area, DRAW_REVERSE, SOLID_LINE);
				}
				// --- edge search
				trans_pos[side] = R_trans_edge_detect_int( &IndexTrans, ProjectBuf, xsize, trans_mode );
				if( 0 < trans_pos[side] ) {
					guide_pos[side] =  xmin+trans_pos[side]/1000;
				} else {
					guide_pos[side] = ERROR;
				}
			}

		// Data check
			if((0<guide_pos[0])&&(0<guide_pos[1])) {
				IGuidePos[DataX] = min( guide_pos[0], guide_pos[1] )-offset_x;
			} else if( 0<guide_pos[0] ) {
				IGuidePos[DataX] = guide_pos[0]-offset_x;
			} else if( 0<guide_pos[1] ) {
				IGuidePos[DataX] = guide_pos[1]-offset_x;
			} else {
				IGuidePos[DataX] = FxSize-1;
				res = ERROR;
			}

		// サーチ位置表示
			// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
                // --- Index guide pos.
				if( res == OK ) {
					for( side=0; side<2; side++ ) {
						R_DrawLine( DRAW_GREEN, xmin, ymin[side], xmin, ymax[side], DRAW_REVERSE, SOLID_LINE );
						R_DrawLine( DRAW_GREEN, xmax, ymin[side], xmax, ymax[side], DRAW_REVERSE, SOLID_LINE );
						if( 0>guide_pos[side] ) continue;
						R_DrawLine( DRAW_RED, guide_pos[side], ymin[side], guide_pos[side], ymax[side], DRAW_REVERSE, DASHED_LINE );
					}
					R_DrawLine( DRAW_GREEN, IGuidePos[DataX], 0, IGuidePos[DataX], FySize-1, DRAW_REVERSE, DASHED_LINE );
					// --- comment
                    sprintf( Comment, "Index guide position guide_pos[0]=%d,guide_pos[1]=%d",guide_pos[0],guide_pos[1] );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					R_DrawCls();
				} else {
					// --- comment
					sprintf( Comment, "Index guide position is undetectable" );
					DrawMessage( Comment, DRAW_RED, 2, 2 );
				}
			}

		// result
			return(res);

	}	// TP_SearchGuideFmRight( void ) end


//================================================
//	Elec. search level set
//================================================

int		ElecEsLevelSet( void )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						thr;

		// Search bin level set
			// --- Gray memory
			IGuideEsPara.nGrayPn = grayPn;
			// --- area
			if( ((1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE))) || ((1==bIsLTR18)&&(0!=(Option & BACKINSP_MODE))) ){
				IGuideEsPara.nXmax = IGuidePos[DataX] - 50;
				IGuideEsPara.nXmin = IGuideEsPara.nXmax - 300;
				IGuideEsPara.nYmin = FySize/2 - 50;
				IGuideEsPara.nYmax = FySize/2 + 50;
			}else{
				IGuideEsPara.nXmin = 32;
				IGuideEsPara.nXmax = IGuidePos[DataX];		//FxSize-32-1;
				IGuideEsPara.nYmin = 32;
				IGuideEsPara.nYmax = FySize-32;
			}
			// ---
			IGuideEsPara.nStartLevel	= IGuideEsLevel.nLower;			// ヒストグラムの計算範囲の開始点(start>=0)
			IGuideEsPara.nEndLevel		= 250;									// ヒストグラムの計算範囲の終了点
			IGuideEsPara.nMethod		= THRESH_OTSU;							// しきい値選定の方法
			// --- level set
			thr = BinLevelSet( &IGuideEsPara );
			// --- Check level
			if( thr < 0 ) {
				// --- comment
				if( i_mode & INSPECT_STEP ) {
					sprintf( Comment, " === ERROR === \n Setup of Electrode ES bin level was not completed. " );
					DrawMessage( Comment, DRAW_RED, 2, 2 );
				}
//				return ERROR;
//				IGuideEsPara.nLevel = IGuideEsLevel.nLower;
				IGuideEsPara.nLevel = IndexLevel.nUpper;
			}

			//v2.20 if((1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE))){
//			if((1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE)) || (1==bIsLTR18)&&(0!=(Option & BACKINSP_MODE))){
//				if( IGuideEsLevel.nData < IGuideEsLevel.nLower )	IGuideEsLevel.nData = IGuideEsLevel.nLower;
//			}else{
				IGuideEsLevel.nData = IGuideEsPara.nLevel + IGuideEsLevel.nOffset;
//			}
			if( IGuideEsLevel.nData < IGuideEsLevel.nLower )	IGuideEsLevel.nData = IGuideEsLevel.nLower;
			if( IGuideEsLevel.nData < IndexLevel.nUpper )		IGuideEsLevel.nData = IndexLevel.nUpper;
			if( IGuideEsLevel.nData > IGuideEsLevel.nUpper )	IGuideEsLevel.nData = IGuideEsLevel.nUpper;

		// result
			return OK;

	}	// ElecEsLevelSet( void ) end


//================================================
//	Search Electrode
//		Index guide 位置検出用２値化画像を使用している
//================================================

int		TP_SearchElect( void )
	{
		int						grayPn	= InspGrayPn;
		int						binPn	= InspBinPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						xsize;
		int						ysize;
		R_RECT					area;
        int                     posi_res_x, posi_res_y;
        BIN_FILTER_PARAMETER	BinFilter;    // Filter condition

		// Search bin level set
			if( ElecEsLevelSet() != OK )	return ERROR;
		// Gray to Bin
			xsize = IGuidePos[DataX]-IGuidePos[1]+1;
			ysize = Index_pos[1]-Index_pos[0]+1;
			BinClsPn( binPn, 0, 0, FxSize-1, FySize-1 );
			R_gray2bin( IGuideEsLevel.nData, IGuidePos[1], Index_pos[0], xsize, ysize, Gray_address[grayPn], Bin_address[binPn] );
			// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
				// --- comment
            //	sprintf( Comment, "Electrode search bin level : %d ", IGuideEsLevel.nData );
            //	DrawMessage( Comment, DRAW_GREEN, 2, 2 );
                posi_res_x = 0;
                posi_res_y = 30;

                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"[Electrode search bin level)]");
                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+2 ,"Electrode search bin level      : %4d",IGuideEsLevel.nData);
                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+3 ,"I.Guide search level offset     : %4d",IGuideEsLevel.nOffset);
                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+4 ,"I.Guide search level lower limit: %4d",IGuideEsLevel.nLower);
                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+5 ,"I.Guide search level upper limit: %4d",IGuideEsLevel.nUpper);
                while( !R_DrawGetCursor());
                R_DrawCls();
                while( R_DrawGetCursor());

            }
		// ノイズ除去
			BinFilter.nBinPn = binPn;
			BinFilter.nXmin = 0;
			BinFilter.nXmax = FxSize-1;
			BinFilter.nYmin = 10;
			BinFilter.nYmax = FySize-10;
			// ---
			FilterBinExpand(  &BinFilter, Ap_expand_x,  Ap_expand_y );
			FilterBinExpand(  &BinFilter, Ap_expand_x,  Ap_expand_y );
			FilterBinContrct( &BinFilter, Ap_contrct_x, Ap_contrct_y );
			FilterBinContrct( &BinFilter, Ap_contrct_x, Ap_contrct_y );
			// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
				// --- comment
				sprintf( Comment, "Electrode search bin image\n noise clear " );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
			}

		// 電極左右位置サーチ
			//v2.20 if(!bIsLTR50){
			if((!bIsLTR50) && (!bIsLTR18)){
				switch( TP_SearchElecXrough( binPn ) ){
					case OK:
						break;
					case ERROR2:	// ワーク無し
						return ERROR2;
						break;
					case ERROR3:	// 表\裏逆
						return ERROR3;
						break;
					case ERROR4:	// Xサイズ
						return ERROR4;
						break;
					default:
						return ERROR;
						break;
				}
			} else {
				switch( TP_SearchElecYroughLTR( binPn ) ){
					case OK:
						break;
					case ERROR2:	// ワーク無し
						return ERROR2;
						break;
					case ERROR3:	// 表\裏逆
						return ERROR3;
						break;
					case ERROR5:	// Yサイズ
						return ERROR5;
						break;
					case ERROR6:	// NG Glass IBUTSU 2021.05.18<shio>
						return ERROR6;
						break;
					default:
						return ERROR;
						break;
				}
			}

			// MCR01は縦寸法を別2値化で検出する
			if(bIsMCR01 && ((Option & BACKINSP_MODE) ==0)){
				// Gray to Bin
				xsize = IGuidePos[DataX]-IGuidePos[1]+1;
				ysize = Index_pos[1]-Index_pos[0]+1;
				BinClsPn( binPn, 0, 0, FxSize-1, FySize-1 );
				//R_gray2bin( IGuideEsLevel.nData, IGuidePos[1], Index_pos[0], xsize, ysize, Gray_address[grayPn], Bin_address[binPn] );
				R_gray2bin( 170, IGuidePos[1], Index_pos[0], xsize, ysize, Gray_address[grayPn], Bin_address[binPn] );
				// --- Viwe
				if( i_mode & INSPECT_STEP ) {
					R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
					R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
					// --- comment
				//	sprintf( Comment, "Electrode search bin level : %d ", IGuideEsLevel.nData );
				//	DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					posi_res_x = 0;
					posi_res_y = 30;

					R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"[Electrode search bin level)]");
					R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+2 ,"Electrode search bin level      : %4d",IGuideEsLevel.nData);
					R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+3 ,"I.Guide search level offset     : %4d",IGuideEsLevel.nOffset);
					R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+4 ,"I.Guide search level lower limit: %4d",IGuideEsLevel.nLower);
					R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+5 ,"I.Guide search level upper limit: %4d",IGuideEsLevel.nUpper);
					while( !R_DrawGetCursor());
					R_DrawCls();
					while( R_DrawGetCursor());

				}
			// ノイズ除去
				BinFilter.nBinPn = binPn;
				BinFilter.nXmin = 0;
				BinFilter.nXmax = FxSize-1;
				BinFilter.nYmin = 10;
				BinFilter.nYmax = FySize-10;
				// ---
				FilterBinExpand(  &BinFilter, Ap_expand_x,  Ap_expand_y );
				FilterBinExpand(  &BinFilter, Ap_expand_x,  Ap_expand_y );
				FilterBinContrct( &BinFilter, Ap_contrct_x, Ap_contrct_y );
				FilterBinContrct( &BinFilter, Ap_contrct_x, Ap_contrct_y );
				// --- Viwe
				if( i_mode & INSPECT_STEP ) {
					R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
					R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
					// --- comment
					sprintf( Comment, "Electrode search bin image\n noise clear " );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				}
			}
			//v2.20 if(!bIsLTR50){
			if( (!bIsLTR50) && (!bIsLTR18) ){
				if( TP_SearchElecYrough( binPn ) != OK ) return ERROR5;

				ChipEsLevel.nData = 200;
				ElecPosRough[DataLR][PosXmin] = ElecPosRough[DataLeft][PosXmin];
				ElecPosRough[DataLR][PosXmax] = ElecPosRough[DataRight][PosXmax];
				ElecPosRough[DataLR][PosYmin] = min(ElecPosRough[DataLeft][PosYmin],ElecPosRough[DataRight][PosYmin]);
				ElecPosRough[DataLR][PosYmax] = max(ElecPosRough[DataLeft][PosYmax],ElecPosRough[DataRight][PosYmax]);

			// view
				if( i_mode & INSPECT_STEP ) {
					// --- area
					area.left	= ElecPosRough[DataLR][PosXmin];
					area.right	= ElecPosRough[DataLR][PosXmax];
					area.top	= ElecPosRough[DataLR][PosYmin];
					area.bottom	= ElecPosRough[DataLR][PosYmax];
					R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
					// --- comment
				//	sprintf( Comment, "Chip position (rough)" );
				//	DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				//	R_DrawCls();

					posi_res_x = 0;
					posi_res_y = 30;

                    R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"[Chip position (rough)]1");
					R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+2 ,"Chip position (%3d,%3d) - (%3d,%3d)" ,
								 ElecPosRough[DataLR][PosXmin],
								 ElecPosRough[DataLR][PosYmin],
								 ElecPosRough[DataLR][PosXmax],
								 ElecPosRough[DataLR][PosYmax]
								 );
					R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, DOTTED_LINE );

					while( !R_DrawGetCursor());
					R_DrawCls();
					while( R_DrawGetCursor());
				}
			} else {
				if( TP_SearchElecXroughLTR( binPn ) != OK ) return ERROR5;

				ChipEsLevel.nData = 200;
				ElecPosRough[DataUL][PosXmin] = min(ElecPosRough[DataUpper][PosXmin],ElecPosRough[DataLower][PosXmin]);
				ElecPosRough[DataUL][PosXmax] = max(ElecPosRough[DataUpper][PosXmax],ElecPosRough[DataLower][PosXmax]);
				ElecPosRough[DataUL][PosYmin] = ElecPosRough[DataUpper][PosYmin];
				ElecPosRough[DataUL][PosYmax] = ElecPosRough[DataLower][PosYmax];

			// view
				if( i_mode & INSPECT_STEP ) {
					// --- area
					area.left	= ElecPosRough[DataUL][PosXmin];
					area.right	= ElecPosRough[DataUL][PosXmax];
					area.top	= ElecPosRough[DataUL][PosYmin];
					area.bottom	= ElecPosRough[DataUL][PosYmax];
					R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
					// --- comment
				//	sprintf( Comment, "Chip position (rough)" );
				//	DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				//	R_DrawCls();

					posi_res_x = 0;
					posi_res_y = 30;

                    R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"[Chip position (rough)]2");
					R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+2 ,"Chip position (%3d,%3d) - (%3d,%3d)" ,
								 ElecPosRough[DataUL][PosXmin],
								 ElecPosRough[DataUL][PosYmin],
								 ElecPosRough[DataUL][PosXmax],
								 ElecPosRough[DataUL][PosYmax]
								 );
					R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, DOTTED_LINE );

					while( !R_DrawGetCursor());
					R_DrawCls();
					while( R_DrawGetCursor());
				}
			}



			return OK;

	}	// TP_SearchElect( void ) end


//================================================
//	Search Electrode X pos.(rough)
//================================================

int		TP_SearchElecXroughLTR( int binPn )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						count, t_count, ans;
		int						xs, xe, ys, ye, rx;
		int						j_count = 20;

		//v2.20 if( (1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE)) ){		//v2.11
		if( (1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE)) || (1==bIsLTR18)&&(0!=(Option & BACKINSP_MODE)) ){
			j_count = 5;
		}
		// 内から外向きにサーチする
		// 上サーチ条件設定
			ys = ElecPosRough[DataUpper][PosYmin]-15;					//2021.10.25<shio>  0 -> -15
			ye = ElecPosRough[DataUpper][PosYmax]+15 ;
			t_count = ye-ys+1;

		//左上
			xs = FxSize/3;
			xe = 20;
			for( rx=xs; rx>xe; rx-- ) {
				count = R_count_bin_area( binPn, rx, ys, rx, ye );
				ans = count*100/t_count;
				if( ans < j_count ) break;
			}
			if( rx <= xe ) return ERROR;
			ElecPosRough[DataUpper][PosXmin] = rx;
		//右上
			xs = FxSize-FxSize/2;
			xe = FxSize-20;
			for( rx=xs; rx<xe; rx++ ) {
				count = R_count_bin_area( binPn, rx, ys, rx, ye );
				ans = count*100/t_count;
				if( ans < j_count ) break;
			}
			if( rx >= xe ) return ERROR;
			ElecPosRough[DataUpper][PosXmax] = rx;
		//サーチ位置表示
			// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				R_DrawLine( DRAW_GREEN, ElecPosRough[DataUpper][PosXmin], ys, ElecPosRough[DataUpper][PosXmin], ye,DRAW_REVERSE, DASHED_LINE );
				R_DrawLine( DRAW_GREEN, ElecPosRough[DataUpper][PosXmax], ys, ElecPosRough[DataUpper][PosXmax], ye,DRAW_REVERSE, DASHED_LINE );
				// --- comment
				sprintf( Comment, "Elec Upper X position" );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
//				R_DrawCls();
			}

		//下サーチ条件設定
			ys = ElecPosRough[DataLower][PosYmin]-15;			//2021.10.25<shio>  0 -> -15
			ye = ElecPosRough[DataLower][PosYmax]+15;
			t_count = ye-ys+1;

		//左下
			xs = FxSize/3;
			xe = 20;
			for( rx=xs; rx>xe; rx-- ) {
				count = R_count_bin_area( binPn, rx, ys, rx, ye );
				ans = count*100/t_count;
				if( ans < j_count ) break;
			}
			if( rx <= xe ) return ERROR;
			ElecPosRough[DataLower][PosXmin] = rx;
		//右下
			xs = FxSize-FxSize/3;
			xe = FxSize-20;
			for( rx=xs; rx<xe; rx++ ) {
				count = R_count_bin_area( binPn, rx, ys, rx, ye );
				ans = count*100/t_count;
				if( ans < j_count ) break;
			}
			if( rx >= xe ) return ERROR;
			ElecPosRough[DataLower][PosXmax] = rx;

		//サーチ位置表示
		// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				R_DrawLine( DRAW_GREEN, ElecPosRough[DataLower][PosXmin], ys, ElecPosRough[DataLower][PosXmin], ye, DRAW_REVERSE, DASHED_LINE );
				R_DrawLine( DRAW_GREEN, ElecPosRough[DataLower][PosXmax], ys, ElecPosRough[DataLower][PosXmax], ye, DRAW_REVERSE, DASHED_LINE );
				// --- comment
				sprintf( Comment, "Elec Lower X position" );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				R_DrawCls();
			}

		return OK;

	}	// TP_SearchElecXroughLTR( int binPn ) end


//================================================
//	Search Electrode X pos.(rough)
//================================================

int		TP_SearchElecXrough( int binPn )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						xn;
		int						count, t_count, wp;
		int						ans_le, ans_ri;
		int						xmin, xmax, rx;
		int						ymin, ymax;
		int						xsize;
		int						mode;

		// サーチ条件設定
			if(bIsSFR25){
				ymin = FySize/2 - 80;
				ymax = FySize/2 + 80;
			}else{
				ymin = FySize/2 - 40;
				ymax = FySize/2 + 40;
			}
			t_count = (ymax-ymin+1)*2;

		// Left
			xmin = 10;
			xmax = FxSize/2 + xmin;
			for( rx=xmin; rx<xmax; rx+=2 ) {
				count = R_count_bin_area( binPn, rx, ymin, rx+1, ymax );
				wp = count*100/t_count;
				if( wp < 20 ) break;
			}
			if( rx >= xmax )	return ERROR;		// 画像異常（画面　白？）
			// --- search
			mode = ES_WHITE;
			xn = 0;
			for( rx=xmin; rx<xmax; rx+=2 ) {
				count = R_count_bin_area( binPn, rx, ymin, rx+1, ymax );
				wp = count*100/t_count;
				if( mode == ES_WHITE ) {
					if( wp > 50 ) {
						xn++;
						if( xn > 2 ) {
							ElecPosRough[DataLeft][0] = rx - xn*2;
							mode = ES_BLACK;
							if(bIsMCR01)	rx+=10;			// 2017.08.22 インデックス見えのマスク
							xn = 0;
						}
					} else {
						xn = 0;
					}
				}
				if( mode == ES_BLACK ) {
					if( wp < 40 ) {
						xn++;
						if( xn > 2 ) {
							ElecPosRough[DataLeft][2] = rx - xn*2;
							break;
						}
					} else {
						xn = 0;
					}
				}
			}
			if( rx >= xmax ) {
				if(  mode == ES_WHITE )	ans_le = ERROR;
				else					ans_le = ERROR2;
			} else {
				ans_le = OK;
				// --- Viwe
				if( i_mode & INSPECT_STEP ) {
					R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
                    // --- Index guide pos.
					R_DrawLine( DRAW_GREEN, ElecPosRough[DataLeft][0], ymin, ElecPosRough[DataLeft][0], ymax, DRAW_REVERSE, DASHED_LINE );
					R_DrawLine( DRAW_GREEN, ElecPosRough[DataLeft][2], ymin, ElecPosRough[DataLeft][2], ymax, DRAW_REVERSE, DASHED_LINE );
					// --- comment
					sprintf( Comment, "Elec Left X position" );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
//					R_DrawCls();
				}
			}

		// Right
			xmax = IGuidePos[DataX];
			xmin = xmax - FxSize/2;
			mode = ES_WHITE;
			xn = 0;
			for( rx=xmax; rx>xmin; rx-=2 ) {
				count = R_count_bin_area( binPn, rx, ymin, rx+1, ymax );
				wp = count*100/t_count;
				if( mode == ES_WHITE ) {
					if( wp > 50 ) {
						xn++;
						if( xn > 2 ) {
							ElecPosRough[DataRight][2] = rx + xn*2;
							mode = ES_BLACK;
							if(bIsMCR01)	rx-=10;			// 2017.08.22 インデックス見えのマスク
							xn = 0;
						}
					} else {
						xn = 0;
					}
				}
				if( mode == ES_BLACK ) {
					if( wp < 40 ) {
						xn++;
						if( xn > 2 ) {
							ElecPosRough[DataRight][0] = rx + xn*2;
							break;
						}
					} else {
						xn = 0;
					}
				}
			}
			if( rx <= xmin ) {
				if(  mode == ES_WHITE )	ans_ri = ERROR;
				else					ans_ri = ERROR2;
			} else {
				ans_ri = OK;
				// --- Viwe
				if( i_mode & INSPECT_STEP ) {
					R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
                    // --- Index guide pos.
					R_DrawLine( DRAW_GREEN, ElecPosRough[DataRight][0], ymin, ElecPosRough[DataRight][0], ymax, DRAW_REVERSE, DASHED_LINE );
					R_DrawLine( DRAW_GREEN, ElecPosRough[DataRight][2], ymin, ElecPosRough[DataRight][2], ymax, DRAW_REVERSE, DASHED_LINE );
					// --- comment
					sprintf( Comment, "Elec Right X position" );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
//					R_DrawCls();
				}
			}

		// Judge
			if(( ans_le == ERROR2 )&&( ans_ri == ERROR2 ))	return ERROR3;		// 表\裏逆
			if(( ans_le == ERROR )&&( ans_ri == ERROR ))	return ERROR2;		// ワーク無し
			xsize = (ElecPosRough[DataRight][2]-ElecPosRough[DataLeft][0]);
			if((InspPara.ChipXmin[TWA_Spec].nXsize > xsize) || (xsize > InspPara.ChipXmax[TWA_Spec].nXsize ))	return ERROR4;
			if(( ans_le == OK )&&( ans_ri == OK ))			return OK;

			return ERROR;

	}	// TP_SearchElecXrough( int binPn ) end

//================================================
//	Search Electrode Y pos.(rough)
//================================================

int		TP_SearchElecYroughLTR( int binPn )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						yn;
		int						count, t_count, wp;
		int						ans_up, ans_lo;
		int						xmin, xmax;
		int						ymin, ymax, ry;
		int						ysize;
		int						mode;

		// サーチ条件設定
			xmin = FxSize/2 - 40;
			xmax = FxSize/2 + 40;
			t_count = (xmax-xmin+1)*2;

		// Upper
			ymin = 10;
			//v2.11	ymax = FySize/2 + ymin;
			ymax = FySize * 0.7;
			for( ry=ymin; ry<ymax; ry+=2 ) {
				count = R_count_bin_area( binPn, xmin, ry, xmax, ry+1 );
				wp = count*100/t_count;
				if( wp < 20 ) break;
			}
			if( ry >= ymax )	return ERROR;		// 画像異常（画面　白？）
			// --- search
			mode = ES_WHITE;
			yn = 0;
			for( ry=ymin; ry<ymax; ry+=2 ) {
				count = R_count_bin_area( binPn, xmin, ry, xmax, ry+1 );
				wp = count*100/t_count;
				if( mode == ES_WHITE ) {
					if( wp > 50 ) {
						yn++;
						if( yn > 2 ) {
							ElecPosRough[DataUpper][PosYmin] = ry - yn*2;
							mode = ES_BLACK;
							yn = 0;
						}
					} else {
						yn = 0;
					}
				}
				if( mode == ES_BLACK ) {
					if( wp < 40 ) {
						yn++;
						if( yn > 2 ) {
							ElecPosRough[DataUpper][PosYmax] = ry - yn*2;
							break;
						}
					} else {
						yn = 0;
					}
				}
			}
			if( ry >= ymax ) {
				if(  mode == ES_WHITE )	ans_up = ERROR;
				else					ans_up = ERROR2;
			} else {
				ans_up = OK;
				// --- Viwe
				if( i_mode & INSPECT_STEP ) {
					R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
					// --- Index guide pos.
					R_DrawLine( DRAW_GREEN, xmin, ElecPosRough[DataUpper][PosYmin], xmax, ElecPosRough[DataUpper][PosYmin], DRAW_REVERSE, DASHED_LINE );
					R_DrawLine( DRAW_GREEN, xmin, ElecPosRough[DataUpper][PosYmax], xmax, ElecPosRough[DataUpper][PosYmax], DRAW_REVERSE, DASHED_LINE );
					// --- comment
					sprintf( Comment, "Elec Upper Y position" );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
//					R_DrawCls();
				}
			}

		// Lower
			ymax = Index_pos[1];
			ymin = ymax - FySize/2;
			mode = ES_WHITE;
			yn = 0;
			for( ry=ymax; ry>ymin; ry-=2 ) {
				count = R_count_bin_area( binPn, xmin, ry, xmax, ry+1 );
				wp = count*100/t_count;
				if( mode == ES_WHITE ) {
					if( wp > 50 ) {
						yn++;
						if( yn > 2 ) {
							ElecPosRough[DataLower][PosYmax] = ry + yn*2;
							mode = ES_BLACK;
							yn = 0;
						}
					} else {
						yn = 0;
					}
				}
				if( mode == ES_BLACK ) {
					if( wp < 40 ) {
						yn++;
						if( yn > 2 ) {
							ElecPosRough[DataLower][PosYmin] = ry + yn*2;
							break;
						}
					} else {
						yn = 0;
					}
				}
			}
			if( ry <= ymin ) {
				if(  mode == ES_WHITE )	ans_lo = ERROR;
				else					ans_lo = ERROR2;
			} else {
				ans_lo = OK;
				// --- Viwe
				if( i_mode & INSPECT_STEP ) {
					R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
					// --- Index guide pos.
					R_DrawLine( DRAW_GREEN, xmin, ElecPosRough[DataLower][PosYmin], xmax, ElecPosRough[DataLower][PosYmin], DRAW_REVERSE, DASHED_LINE );
					R_DrawLine( DRAW_GREEN, xmin, ElecPosRough[DataLower][PosYmax], xmax, ElecPosRough[DataLower][PosYmax], DRAW_REVERSE, DASHED_LINE );
					// --- comment
					sprintf( Comment, "Elec Lower Y position" );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
//					R_DrawCls();
				}
			}

		// Judge
			if(( ans_up == ERROR2 )&&( ans_lo == ERROR2 ))	return ERROR3;		// 表\裏逆
			if(( ans_up == ERROR  )&&( ans_lo == ERROR )){						// ワーク無し
				if( OK != Index_Ibutsu_check( grayPn, binPn)) {
					TWA_Ibutsu_Stop = ON;
					return ERROR6;									// Index Ibutsu  2021.04.20 <shio>
				}else{
					return ERROR2;		// ワーク無し
				}
			}
			ysize = (ElecPosRough[DataLower][PosYmax]-ElecPosRough[DataUpper][PosYmin]);
			if((InspPara.ChipYmin[TWA_Spec].nYsize > ysize) || (ysize > InspPara.ChipYmax[TWA_Spec].nYsize ))	return ERROR5;
			if(( ans_up == OK )&&( ans_lo == OK ))			return OK;

			return ERROR;

	}	// TP_SearchElecXrough( int binPn ) end

//================================================
//	White IBUTSU check at NoWoork
//================================================
// 2021.04.20 <shio>
int	Index_Ibutsu_check( int grayPn, int binPn )
	{
		int						pt[10];
		int						xs, xe, ys, ye;
		int						xl, yl;
		int						xsize, ysize; 		//, lsize;
		int						ans, bn;
//		int						area;
		int						judge_area; 		//, judge_l;
		int						i_mode	= R_CS_GetInspectMode();
//printf("Search Elec %d\n",__LINE__);
		// Insectoin area.
			ys = Index_pos[0] + 120;
			ye = Index_pos[1] - 160;
			xs = IGuidePos[DataX] - 360;
			xe = IGuidePos[DataX] - 80;
			xl = xe - xs + 1;
			yl = ye - ys + 1;
printf("Search Elec %d,xs%d,ys%d,xe%d,ye%d \n",__LINE__, xs, ys,xe,ye );

		//ノイズ除去処理
			pt[0]=0;	pt[1]=0;	pt[2]=FxSize-1;		pt[3]=FySize-1;
			TWA_contrct2( binPn, Ap_contrct_x, Ap_contrct_y, pt );
			TWA_contrct2( binPn, Ap_contrct_x, Ap_contrct_y, pt );
			TWA_expand2( binPn, Ap_expand_x, Ap_expand_y, pt );
			TWA_expand2( binPn, Ap_expand_x, Ap_expand_y, pt );

		// 2 value measurement
			R_set_meas_configration( &Mpx, 0, 5, RM_BLACK|RM_WHITE, RM_AREA );
			if( R_measure_fast( &Mpx, binPn, xs, ys, xl, yl, 1, 3 ) != OK ) {		// 1: white
				R_measure_close( &Mpx );
				R_measure_open( &Mpx, 2000 );
				return ERROR;
			}
		// Judge
			ans = OK;
//			area_sum = 0;
			for( bn=1; bn<=Mpx.bn; bn++ ) {
				xsize = Mpx.imax[bn] - Mpx.imin[bn] + 1;
				ysize = Mpx.jmax[bn] - Mpx.jmin[bn] + 1;
				if( i_mode & INSPECT_STEP ) {
					// x line
					R_box( xs, ys, xe, ye, GRAPH_REVERSE, DASHED_LINE );
					ys = Mpx.center_y[bn];
					xs = Mpx.imin[bn];		xe = Mpx.imax[bn];
					R_line( xs, ys, xe, ys, GRAPH_REVERSE, SOLID_LINE );
					// y line
					xs = Mpx.center_x[bn];
					ys = Mpx.jmin[bn];		ye = Mpx.jmax[bn];
					R_line( xs, ys, xs, ye, GRAPH_REVERSE, SOLID_LINE );
					sprintf( Comment1, "Xsize:%3d(bit)(%3d)  Ysize:%3d(bit)(%3d)", xsize, InspParaM.IbutsuXsize[TWA_Spec].nXsize, ysize, InspParaM.IbutsuYsize[TWA_Spec].nYsize );
					sprintf( Comment2, "Area :%5d(bit)", Mpx.area[bn] );
					//DrawMessage( Comment1, Comment2, 2, 2 );
					sprintf( Comment, "%s\n%s", Comment1, Comment2 );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				}
				if( (xsize >= InspPara.IbutsuXsize[TWA_Spec].nXsize) && (ysize >= InspPara.IbutsuYsize[TWA_Spec].nYsize) ) {
					judge_area = InspPara.IbutsuXsize[TWA_Spec].nXsize * InspPara.IbutsuYsize[TWA_Spec].nYsize;
					if(Mpx.area[bn] > judge_area){
						if( i_mode & INSPECT_STEP ) {
							sprintf( Comment, "                 " );
							sprintf( Comment2, "Area :%5d(bit)(%5d)", Mpx.area[bn], judge_area );
							sprintf( Comment, "%s\n%s", Comment1, Comment2 );
							DrawMessage( Comment, DRAW_GREEN, 2, 2 );
						//	DrawMessage( "", Comment2, 2, 2 );
						}
					ans = ERROR;
					break;
					}
				}
			}
			if(ans != OK) return(ERROR);	// <0000>
			else	    return( OK );	// <0000>
	} 			// Index_Ibutsu_check( void ) end

/************************************************
	Index Ibutsu error
************************************************/
//2021.04.20<shio>
void	TWA_Ibutsu_error( void )
	{

		//ERROR表示
	//		TWA_output_Stop( ON, TWA_io_type );
	//		if( R_language == JPN_DISP )	strcpy( Comment, "Index Ibutsu 付着異常" );
	//		else							strcpy( Comment, "Ibutsu adhesion on the Glass" );
			R_chrdisp_system_scale_font( 1,10, Comment );
			if( R_language == JPN_DISP )	sprintf( Comment, "マウスをクリックして下さい。" );
			else							sprintf( Comment, "Cleaning Glass and click mouse" );
			R_chrdisp_system_scale_font( 1,15, Comment );

		// マウスクリック待ち
			WaitMouseClick();

		// ERROR解除
	//		TWA_output_Stop( OFF, TWA_io_type );
			R_gray_fill( 0, 0, FxSize-1, FySize-1, 0 );
			BinClsPn( 0, 0, 0, FxSize-1, FySize-1 );
			TWA_Ibutsu_Stop = OFF;
	//		Insp_No = 23;			//NG number
	//		pCsResult->nCode = NG_GLASS_IBUTSU;

	}
/*
void	TWA_output_Stop( int mode, int io )
	{
		if( io == TWA_TAPE ) {
			if( mode ) R_on_incy67(  INCY67_chanel_0, 0x08 );
			else       R_off_incy67( INCY67_chanel_0, 0x08 );
		} else {
			if( mode ) R_on_incy67(  INCY67_chanel_0, 0x02 );
			else       R_off_incy67( INCY67_chanel_0, 0x02 );
		}
	}
*/

//================================================
//	Search Electrode Y pos.(rough)
//================================================

int		TP_SearchElecYrough( int binPn )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						count, t_count, ans;
		int						xs, xe, ys, ye, ry;
		int						j_count = 20;

		if(!bIsMCR01 || ((Option & BACKINSP_MODE) !=0)){
			//v2.11 if(bIsMCR01)	j_count = 10;
			if( (bIsMCR01) || ((Option & BACKINSP_MODE) !=0) )	j_count = 10;
		// 内から外向きにサーチする
		// 左サーチ条件設定
			xs = ElecPosRough[DataLeft][0];
			xe = ElecPosRough[DataLeft][2];
			t_count = xe-xs+1;

		//左上
			ys = FySize/2;
			ye = 20;
			for( ry=ys; ry>ye; ry-- ) {
				count = R_count_bin_area( binPn, xs, ry, xe, ry );
				ans = count*100/t_count;
				if( ans < j_count ) break;
			}
			if( ry <= ye ) return ERROR;
			ElecPosRough[DataLeft][1] = ry;
		//左下
			ys = FySize/2;
			ye = FySize-20;
			for( ry=ys; ry<ye; ry++ ) {
				count = R_count_bin_area( binPn, xs, ry, xe, ry );
				ans = count*100/t_count;
				if( ans < j_count ) break;
			}
			if( ry >= ye ) return ERROR;
			ElecPosRough[DataLeft][3] = ry;
		//サーチ位置表示
			// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
                // --- Index guide pos.
				R_DrawLine( DRAW_GREEN, xs, ElecPosRough[DataLeft][1], xe, ElecPosRough[DataLeft][1], DRAW_REVERSE, DASHED_LINE );
				R_DrawLine( DRAW_GREEN, xs, ElecPosRough[DataLeft][3], xe, ElecPosRough[DataLeft][3], DRAW_REVERSE, DASHED_LINE );
				// --- comment
				sprintf( Comment, "Elec Left Y position" );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
//				R_DrawCls();
			}

		//右サーチ条件設定
			xs = ElecPosRough[DataRight][0];
			xe = ElecPosRough[DataRight][2];
			t_count = xe-xs+1;

		//右上
			ys = FySize/2;
			ye = 20;
			for( ry=ys; ry>ye; ry-- ) {
				count = R_count_bin_area( binPn, xs, ry, xe, ry );
				ans = count*100/t_count;
				if( ans < j_count ) break;
			}
			if( ry <= ye ) return ERROR;
			ElecPosRough[DataRight][1] = ry;
		//右下
			ys = FySize/2;
			ye = FySize-20;
			for( ry=ys; ry<ye; ry++ ) {
				count = R_count_bin_area( binPn, xs, ry, xe, ry );
				ans = count*100/t_count;
				if( ans < j_count ) break;
			}
			if( ry >= ye ) return ERROR;
			ElecPosRough[DataRight][3] = ry;

		//サーチ位置表示
		// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				R_DrawLine( DRAW_GREEN, xs, ElecPosRough[DataRight][1], xe, ElecPosRough[DataRight][1], DRAW_REVERSE, DASHED_LINE );
				R_DrawLine( DRAW_GREEN, xs, ElecPosRough[DataRight][3], xe, ElecPosRough[DataRight][3], DRAW_REVERSE, DASHED_LINE );
				// --- comment
				sprintf( Comment, "Elec Right Y position" );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				R_DrawCls();
			}
		} else {   //MCR01
			// 外から内向きにサーチする
			// 左サーチ条件設定
				xs = ElecPosRough[DataLeft][0];
				xe = ElecPosRough[DataLeft][2];
				t_count = xe-xs+1;

			//左上
				ys = Index_pos[0];
				ye = FySize/2;
				for( ry=ys; ry<ye; ry++ ) {
					count = R_count_bin_area( binPn, xs, ry, xe, ry );
					ans = count*100/t_count;
					if( ans > j_count ) break;
				}
				if( ry >= ye ) return ERROR;
				ElecPosRough[DataLeft][1] = ry;
			//左下
				ys = Index_pos[1];
				ye = FySize/2;
				for( ry=ys; ry>ye; ry-- ) {
					count = R_count_bin_area( binPn, xs, ry, xe, ry );
					ans = count*100/t_count;
					if( ans > j_count ) break;
				}
				if( ry <= ye ) return ERROR;
				ElecPosRough[DataLeft][3] = ry;
			//サーチ位置表示
				// --- Viwe
				if( i_mode & INSPECT_STEP ) {
					R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
					// --- Index guide pos.
					R_DrawLine( DRAW_GREEN, xs, ElecPosRough[DataLeft][1], xe, ElecPosRough[DataLeft][1], DRAW_REVERSE, DASHED_LINE );
					R_DrawLine( DRAW_GREEN, xs, ElecPosRough[DataLeft][3], xe, ElecPosRough[DataLeft][3], DRAW_REVERSE, DASHED_LINE );
					// --- comment
					sprintf( Comment, "Elec Left Y position" );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
	//				R_DrawCls();
				}

			//右サーチ条件設定
				xs = ElecPosRough[DataRight][0];
				xe = ElecPosRough[DataRight][2];
				t_count = xe-xs+1;

			//右上
				ys = Index_pos[0];
				ye = FySize/2;
				for( ry=ys; ry<ye; ry++ ) {
					count = R_count_bin_area( binPn, xs, ry, xe, ry );
					ans = count*100/t_count;
					if( ans > j_count ) break;
				}
				if( ry >= ye ) return ERROR;
				ElecPosRough[DataRight][1] = ry;
			//右下
				ys = Index_pos[1];
				ye = FySize/2;
				for( ry=ys; ry>ye; ry-- ) {
					count = R_count_bin_area( binPn, xs, ry, xe, ry );
					ans = count*100/t_count;
					if( ans > j_count ) break;
				}
				if( ry <= ye ) return ERROR;
				ElecPosRough[DataRight][3] = ry;
			//サーチ位置表示
				// --- Viwe
				if( i_mode & INSPECT_STEP ) {
					R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
					// --- Index guide pos.
					R_DrawLine( DRAW_GREEN, xs, ElecPosRough[DataRight][1], xe, ElecPosRough[DataRight][1], DRAW_REVERSE, DASHED_LINE );
					R_DrawLine( DRAW_GREEN, xs, ElecPosRough[DataRight][3], xe, ElecPosRough[DataRight][3], DRAW_REVERSE, DASHED_LINE );
					// --- comment
					sprintf( Comment, "Elec Right Y position" );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					R_DrawCls();
				}
		}
		return OK;

	}	// TP_SearchElecYrough( int binPn ) end


//================================================
//		SearchChipPos
//================================================

int		SearchChipPos( void )
	{
		int						binPn	= InspBinPn;
		int						res;

		// Initialze
			BinClsPn( binPn, 0, 0, FxSize-1, FySize-1 );

		// Search bin level set
			if( ChipEsLevelSet() != OK )	return ERROR;

		// Chip search Bin image
			ChipGray2Bin();

		// Chip edge search
			res = ChipBinMeasure();
			switch( res ) {
				case OK:
					break;
				case ERROR2:				// R_measure ERROR
					return ERROR2;
					break;
				case ERROR3:				// Elec. search ERROR;
					return ERROR3;
					break;
				case ERROR4:				// No Chip
					return ERROR4;
					break;
				case ERROR5:				// Can't insp.
					return ERROR5;
					break;
				default:
					return ERROR;
					break;
			}

		// result
			return( res );

	}	// SearchChipPos( void ) end


//================================================
//	Chip search level set
//================================================

int		ChipEsLevelSet( void )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						thr = -1,thr2 = -1;

		// Search bin level set
			// --- Gray memory
			ChipEsLSetPara.nGrayPn = grayPn;
			// --- area
			//v2.11 if(bIsMCR03  || bIsMCR01){
			//v2.20 if( (bIsMCR03  || bIsMCR01)||((1==bIsSFR25)&&(0!=(Option & BACKINSP_MODE)))||((1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE))) ){
			if( (bIsMCR03  || bIsMCR01)||((1==bIsSFR25)&&(0!=(Option & BACKINSP_MODE)))||((1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE)))||((1==bIsLTR18)&&(0!=(Option & BACKINSP_MODE))) ){
				ChipEsLSetPara.nXmin = ElecPosRough[DataLR][PosXmin];
				ChipEsLSetPara.nXmax = ElecPosRough[DataLR][PosXmax];
				ChipEsLSetPara.nYmin = ElecPosRough[DataLR][PosYmin];
				ChipEsLSetPara.nYmax = ElecPosRough[DataLR][PosYmax];
				ChipEsLSetPara.nStartLevel	= ChipEsLevel.nLower;			// ヒストグラムの計算範囲の開始点(start>=0)
			} else if (bIsESR25 != 0 || bIsSFR25 != 0) {
				ChipEsLSetPara.nXmin = ElecPosRough[DataLR][PosXmin];
				ChipEsLSetPara.nXmax = ElecPosRough[DataLR][PosXmin]+(ElecPosRough[DataLR][PosXmax]-ElecPosRough[DataLR][PosXmin])/3;
				ChipEsLSetPara.nYmin = shita.kaiki_ey;	//ElecPosRough[DataLR][PosYmin];
				ChipEsLSetPara.nYmax = ElecPosRough[DataLR][PosYmax];
				ChipEsLSetPara.nStartLevel	= ChipEsLevel.nLower;			// ヒストグラムの計算範囲の開始点(start>=0)
			//v2.20 } else if (bIsLTR50 != 0) {					//電極の凹み部分を使って2値化レベルを計算する
			} else if ( (bIsLTR50 != 0) || (bIsLTR18 != 0) ) {					//電極の凹み部分を使って2値化レベルを計算する
				ChipEsLSetPara.nXmin = (ElecPosRough[DataLR][PosXmin]+ElecPosRough[DataLR][PosXmax])/2-40;
				ChipEsLSetPara.nXmax = ChipEsLSetPara.nXmin+80;
				ChipEsLSetPara.nYmin = shita.kaiki_ey+3;
				ChipEsLSetPara.nYmax = ChipEsLSetPara.nYmin+15;
				ChipEsLSetPara.nStartLevel	= ChipEsLevel.nLower;			// ヒストグラムの計算範囲の開始点(start>=0)
			} else {	// MCR10,MCR18  下のオーバーコートすきまを使って端子2値化レベルを計算する
				ChipEsLSetPara.nXmin = ElecPosRough[DataLR][PosXmin];
				ChipEsLSetPara.nXmax = ElecPosRough[DataLR][PosXmax];
				ChipEsLSetPara.nYmin = shita.kaiki_ey;	//ElecPosRough[DataLR][PosYmin];
				ChipEsLSetPara.nYmax = ElecPosRough[DataLeft][PosYmax];
				if(ChipEsLSetPara.nYmax > ElecPosRough[DataRight][PosYmax])		ChipEsLSetPara.nYmax = ElecPosRough[DataRight][PosYmax];
				ChipEsLSetPara.nYmax-=3;
				ChipEsLSetPara.nStartLevel	= 40;			// ヒストグラムの計算範囲の開始点(start>=0)
			}
			if(ChipEsLSetPara.nYmin > ChipEsLSetPara.nYmax)	ChipEsLSetPara.nYmin = ChipEsLSetPara.nYmax-1;
			// ---
			ChipEsLSetPara.nEndLevel	= 255;									// ヒストグラムの計算範囲の終了点
			ChipEsLSetPara.nMethod		= THRESH_OTSU;							// しきい値選定の方法
			// --- level set
			thr = BinLevelSet( &ChipEsLSetPara );

			if(bIsMCR10 || bIsMCR18){		// MCR10,MCR18 上のオーバーコートすきまを使って端子2値化レベルを計算する 上下で大きい方を採用する
				if(thr >= 0)	thr = ChipEsLSetPara.nLevel;
				ChipEsLSetPara.nXmin = ElecPosRough[DataLR][PosXmin];
				ChipEsLSetPara.nXmax = ElecPosRough[DataLR][PosXmax];
				ChipEsLSetPara.nYmax = ue.kaiki_ey;	//ElecPosRough[DataLR][PosYmin];
				ChipEsLSetPara.nYmin = ElecPosRough[DataLeft][PosYmin];
				if(ChipEsLSetPara.nYmin < ElecPosRough[DataRight][PosYmin])		ChipEsLSetPara.nYmin = ElecPosRough[DataRight][PosYmin];
				ChipEsLSetPara.nYmin+=3;
				ChipEsLSetPara.nStartLevel	= 40;			// ヒストグラムの計算範囲の開始点(start>=0)

				if(ChipEsLSetPara.nYmin > ChipEsLSetPara.nYmax)	ChipEsLSetPara.nYmin = ChipEsLSetPara.nYmax-1;
				// ---
				ChipEsLSetPara.nEndLevel	= 255;									// ヒストグラムの計算範囲の終了点
				ChipEsLSetPara.nMethod		= THRESH_OTSU;							// しきい値選定の方法
				// --- level set
				thr2 = BinLevelSet( &ChipEsLSetPara );
				if(thr2 >= 0){
					if(thr > ChipEsLSetPara.nLevel)	ChipEsLSetPara.nLevel = thr;
				}
			}
			// --- Check level
			if( thr < 0  && thr2 < 0) {
				// --- comment
				if( i_mode & INSPECT_STEP ) {
					sprintf( Comment, " === ERROR === \n Setup of Chip ES bin level was not completed. " );
					DrawMessage( Comment, DRAW_RED, 2, 2 );
				}
			//	return ERROR;
				G2EsLSetPara.nLevel = 0;
			}
			ChipEsLevel.nData = ChipEsLSetPara.nLevel + ChipEsLevel.nOffset;
			if( ChipEsLevel.nData < ChipEsLevel.nLower )	ChipEsLevel.nData = ChipEsLevel.nLower;
			if( ChipEsLevel.nData > ChipEsLevel.nUpper )	ChipEsLevel.nData = ChipEsLevel.nUpper;

		// result
			return OK;

	}	// ChipEsLevelSet( void ) end


//================================================
//	Chip search Bin image
//================================================

void			ChipGray2Bin( void )
	{
		int						grayPn	= InspGrayPn;
		int						binPn	= InspBinPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						xmin, xmax, xsize;
		int						ymin, ymax, ysize;
		int						posi_res_x,posi_res_y;
        BIN_FILTER_PARAMETER	BinFilter;    // Filter condition

		// Initial
			BinClsPn( binPn, 0, 0, FxSize-1, FySize-1 );
		// チップ検出２値化画像
			xmin	= IGuidePos[1];
			xmax	= IGuidePos[DataX];
			xsize	= xmax-xmin+1;
			ymin	= Index_pos[0];	// 0
			ymax    = Index_pos[1];	// FySize -1
			ysize	= ymax-ymin+1;
			R_gray2bin( ChipEsLevel.nData, xmin, ymin, xsize, ysize, Gray_address[grayPn], Bin_address[binPn] );
			// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
				// --- comment
            //	sprintf( Comment, " Chip search bin level : %d ", ChipEsLevel.nData );
            //	DrawMessage( Comment, DRAW_GREEN, 2, 2 );
                posi_res_x = 0;
                posi_res_y = 30;

                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"[Chip search bin level)]");
                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+2 ,"Chip search bin level        : %4d",ChipEsLevel.nData);
                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+3 ,"Chip search level offset     : %4d",ChipEsLevel.nOffset);
                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+4 ,"Chip search level lower limit: %4d",ChipEsLevel.nLower);
                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+5 ,"Chip search level upper limit: %4d",ChipEsLevel.nUpper);
                while( !R_DrawGetCursor());
                R_DrawCls();
                while( R_DrawGetCursor());

            }

            // 標印を消す
            if(bIsESR25!=0 || bIsSFR25!=0){
                BinFilter.nBinPn = binPn;
                BinFilter.nXmin = (G2_pos[0]+G2_pos[1])/2-125;
                BinFilter.nXmax = (G2_pos[0]+G2_pos[1])/2+125;
                BinFilter.nYmin = (G2_pos[2]+G2_pos[3])/2-125;
                BinFilter.nYmax = (G2_pos[2]+G2_pos[3])/2+125;
                FilterBinContrct( &BinFilter, G2_contrct_x, G2_contrct_y );
                FilterBinContrct( &BinFilter, G2_contrct_x, G2_contrct_y );
                FilterBinContrct( &BinFilter, G2_contrct_x, G2_contrct_y );
                FilterBinContrct( &BinFilter, G2_contrct_x, G2_contrct_y );
                FilterBinContrct( &BinFilter, G2_contrct_x, G2_contrct_y );
                FilterBinContrct( &BinFilter, G2_contrct_x, G2_contrct_y );
				FilterBinExpand(  &BinFilter, G2_expand_x,  G2_expand_y );
                FilterBinExpand(  &BinFilter, G2_expand_x,  G2_expand_y );
                if( i_mode & INSPECT_STEP ) {
                    R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
                    sprintf( Comment, "Contrct Mask");
                    DrawMessage( Comment, DRAW_GREEN, 2, 2 );
                    BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
                }
            }

			// 壁面の端子付着を消す MCR18
			if(bIsMCR18){
				BinFilter.nBinPn = binPn;
				BinFilter.nXmin = ElecPosRough[DataLR][PosXmin];
				BinFilter.nXmax = ElecPosRough[DataLR][PosXmax];
				BinFilter.nYmin = ElecPosRough[DataLR][PosYmin]-10;
				BinFilter.nYmax = ElecPosRough[DataLR][PosYmin]+20;
				FilterBinContrct( &BinFilter,            1,            3 );
				FilterBinContrct( &BinFilter,            1,            3 );
				FilterBinExpand(  &BinFilter,            1,            3 );
				FilterBinExpand(  &BinFilter,            1,            3 );

				BinFilter.nYmin = ElecPosRough[DataLR][PosYmax]-20;
				BinFilter.nYmax = ElecPosRough[DataLR][PosYmax]+10;
				FilterBinContrct( &BinFilter,            1,            3 );
				FilterBinContrct( &BinFilter,            1,            3 );
				FilterBinExpand(  &BinFilter,            1,            3 );
				FilterBinExpand(  &BinFilter,            1,            3 );

			}

			// チップブレーク面が見えるのを消す
			if(bIsMCR01){
				BinFilter.nBinPn = binPn;
				BinFilter.nXmin = ElecPosRough[DataLeft][PosXmin];
				BinFilter.nXmax = FxSize/2-1;
				BinFilter.nYmin = Index_pos[0]-20;
				BinFilter.nYmax = ElecPosRough[DataLeft][PosYmin];
				BinClsPn( binPn, BinFilter.nXmin, BinFilter.nYmin, BinFilter.nXmax, BinFilter.nYmax );
				BinFilter.nYmin = ElecPosRough[DataLeft][PosYmax];
				BinFilter.nYmax = Index_pos[1]+20;
				BinClsPn( binPn, BinFilter.nXmin, BinFilter.nYmin, BinFilter.nXmax, BinFilter.nYmax );

				BinFilter.nXmin = FxSize/2;
				BinFilter.nXmax = ElecPosRough[DataRight][PosXmax];
				BinFilter.nYmin = Index_pos[0]-20;
				BinFilter.nYmax = ElecPosRough[DataRight][PosYmin];
				BinClsPn( binPn, BinFilter.nXmin, BinFilter.nYmin, BinFilter.nXmax, BinFilter.nYmax );
				BinFilter.nYmin = ElecPosRough[DataRight][PosYmax];
				BinFilter.nYmax = Index_pos[1]+20;
				BinClsPn( binPn, BinFilter.nXmin, BinFilter.nYmin, BinFilter.nXmax, BinFilter.nYmax );

				if( i_mode & INSPECT_STEP ) {
					R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
					sprintf( Comment, "Side Mask");
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
				}
			}


			//v2.20 if(bIsLTR50){
			if( (bIsLTR50) || (bIsLTR18) ){
				BinFilter.nBinPn = binPn;
				if(0==(Option & BACKINSP_MODE)){		//v2.11
					BinFilter.nXmin = 0;
					BinFilter.nXmax = FxSize-1;
					BinFilter.nYmin = 10;
					BinFilter.nYmax = FySize-10;
				}else{
					BinFilter.nXmin = min(ElecPosRough[DataUpper][PosXmin],ElecPosRough[DataLower][PosXmin]);
					BinFilter.nXmax = max(ElecPosRough[DataUpper][PosXmax],ElecPosRough[DataLower][PosXmax]);
					BinFilter.nYmin = ElecPosRough[DataUpper][PosYmin] - 15;
					BinFilter.nYmax = ElecPosRough[DataLower][PosYmax] + 15;
				}
				// ---
				FilterBinExpand(  &BinFilter, Ap_expand_x,  Ap_expand_y );
				FilterBinExpand(  &BinFilter, Ap_expand_x,  Ap_expand_y );

			}

        // ノイズ除去
			BinFilter.nBinPn = binPn;
			BinFilter.nXmin = 0;
			BinFilter.nXmax = FxSize-1;
			BinFilter.nYmin = 10;
			BinFilter.nYmax = FySize-10;
			// ---
			FilterBinContrct( &BinFilter, Ap_contrct_x, Ap_contrct_y );
			FilterBinExpand(  &BinFilter, Ap_expand_x,  Ap_expand_y );
			FilterBinExpand(  &BinFilter, Ap_expand_x,  Ap_expand_y );
			FilterBinContrct( &BinFilter, Ap_contrct_x, Ap_contrct_y );

//			FilterBinExpand(  &BinFilter, Ap_expand_x,  Ap_expand_y );
//			FilterBinExpand(  &BinFilter, Ap_expand_x,  Ap_expand_y );
//			FilterBinExpand(  &BinFilter, Ap_expand_x,  Ap_expand_y );
//			FilterBinContrct( &BinFilter, Ap_contrct_x, Ap_contrct_y );
//			FilterBinContrct( &BinFilter, Ap_contrct_x, Ap_contrct_y );
//			FilterBinContrct( &BinFilter, Ap_contrct_x, Ap_contrct_y );






			// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
				// --- comment
				sprintf( Comment, " Chip search bin image \n noise clear " );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
			}

	}	// ChipGray2Bin( void ) end


//================================================
//	Chip bin measure
//================================================

int		ChipBinMeasure( void )
	{
		int						grayPn	= InspGrayPn;
		int						binPn	= InspBinPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						xmin, xmax, xsize;
		int						ymin, ymax, ysize;
		int						bn, rbn;
		int						xc, ox, xl;
		int						yc, oy, yl;
		int						side;
		int						res;
		int                     Center_L, Center_R;              //GEE V1.02A
		int                     Center_Up, Center_Lo;              //GEE V1.02A

		// Initial
			ChipBlobNo = ERROR;
			Elec[DataLeft].BlobNo = Elec[DataRight].BlobNo = ERROR;	// for LTR  Upper 0, Lower 1

		// area set
		//	xmin	= 0;
		//	xmax	= IGuidePos[DataX];
		//	xsize	= xmax-xmin+1;
		//	ymin	= 0;
		//	ymax	= FySize-1;
		//	ysize	= ymax-ymin+1;

			// 2017.08.21 白index部分を省く
			xmin	= IGuidePos[1];
			xmax	= IGuidePos[DataX];
			xsize	= xmax-xmin+1;
			ymin	= Index_pos[0];
			ymax    = Index_pos[1];
			ysize	= ymax-ymin+1;

		// Binary measure
			// 特徴量演算　演算方式指定
				R_set_meas_configration( &Mpx, BinMeasChip.nRun, BinMeasChip.nArea, BinMeasChip.nColor, BinMeasChip.nMode );
			// Measure
				if( R_measure( &Mpx, binPn, xmin, ymin, xsize, ysize, GROUP2 ) != OK ) {
					R_measure_close( &Mpx );
					R_measure_open( &Mpx, 2000 );
					return ERROR2;
				}
			// 白色、面積の大きい上位２つのブローブを電極と判断する。
				R_sort_area(&Mpx);
				for( bn=1; bn<=Mpx.bn; bn++ ) {
					rbn = Mpx.sortnumb[bn];
					if( Mpx.color[rbn] == ColorWhite ) {
						if((Option & BACKINSP_MODE) ==0){		// 表面検査
							//v2.20 if(!bIsLTR50){	//normal
							if( (!bIsLTR50) && (!bIsLTR18) ){	//normal
								if( Mpx.center_x[rbn] < (FxSize/2) ) {
									if( Elec[DataLeft].BlobNo == ERROR )	Elec[DataLeft].BlobNo = rbn;	// Left
									Center_L = Mpx.center_x[rbn];                                           // GEE V1.02A
								} else {
									if( Elec[DataRight].BlobNo == ERROR )	Elec[DataRight].BlobNo = rbn;	// Right
									Center_R = Mpx.center_x[rbn];                                           // GEE V1.02A
								}
							} else {		//LTR
								if( Mpx.center_y[rbn] < (FySize/2) ) {
									if( Elec[DataUpper].BlobNo == ERROR )	Elec[DataUpper].BlobNo = rbn;	// Left
									Center_Up = Mpx.center_y[rbn];                                           // GEE V1.02A
								} else {
									if( Elec[DataLower].BlobNo == ERROR )	Elec[DataLower].BlobNo = rbn;	// Right
									Center_Lo = Mpx.center_y[rbn];                                           // GEE V1.02A
								}
							}
						} else {								// 裏面検査 20171026
							//v2.20 if(!bIsLTR50){	//normal
							if( (!bIsLTR50) && (!bIsLTR18) ){	//normal
								if( Mpx.center_x[rbn] < ElecPosRough[DataLeft][PosXmax] ) {
									if( Elec[DataLeft].BlobNo == ERROR )	Elec[DataLeft].BlobNo = rbn;	// Left
									Center_L = Mpx.center_x[rbn];                                           // GEE V1.02A
								} else {
									if( Mpx.center_x[rbn] > ElecPosRough[DataRight][PosXmin] ) {
										if( Elec[DataRight].BlobNo == ERROR )	Elec[DataRight].BlobNo = rbn;	// Right
										Center_R = Mpx.center_x[rbn];                                           // GEE V1.02A
									}
								}
							} else {
								if( Mpx.center_y[rbn] < ElecPosRough[DataUpper][PosYmax] ) {
									if( Elec[DataUpper].BlobNo == ERROR )	Elec[DataUpper].BlobNo = rbn;		// Left
									Center_Up = Mpx.center_y[rbn];                                           // GEE V1.02A
								} else {
									if( Mpx.center_y[rbn] > ElecPosRough[DataLower][PosYmin] ) {
										if( Elec[DataLower].BlobNo == ERROR )	Elec[DataLower].BlobNo = rbn;	// Right
										Center_Lo = Mpx.center_y[rbn];                                           // GEE V1.02A
									}
								}
							}
						}
						if(( Elec[DataLeft].BlobNo != ERROR )&&( Elec[DataRight].BlobNo != ERROR ))	break;		// LTR Upper 0, Lower 1
					}
				}
				//v2.20 if(!bIsLTR50){	//normal
				if( (!bIsLTR50) && (!bIsLTR18) ){	//normal
					Center_C = (Center_L + Center_R)/2;
				} else {
					Center_C = (Center_Up + Center_Lo)/2;
				}
				// Draw search bin pn
					if( i_mode & INSPECT_STEP ) {
						BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
						R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
						for( side=0; side<2; side++ ) {
							bn = Elec[side].BlobNo;
							if( bn > 0 ) {
								xc = Mpx.center_x[bn];
								yc = Mpx.center_y[bn];
								ox = ( short )( Mpx.ellipse_a[bn]*Mpx.cos[bn] );
								oy = ( short )( Mpx.ellipse_a[bn]*Mpx.sin[bn] );
								ox/=2;
								oy/=2;
								R_DrawLine( DRAW_GREEN, xc-ox, yc-oy, xc+ox, yc+oy, DRAW_REVERSE, DASHED_LINE );	// 横 Line
								ox = ( short )( Mpx.ellipse_b[bn]*Mpx.cos[bn] );
								oy = ( short )( Mpx.ellipse_b[bn]*Mpx.sin[bn] );
								ox/=2;
								oy/=2;
								R_DrawLine( DRAW_GREEN, xc-oy, yc+ox, xc+oy, yc-ox, DRAW_REVERSE, DASHED_LINE );	// 縦 Line
								R_draw_ellipse( DRAW_GREEN, xc, yc, Mpx.ellipse_a[bn]/2, Mpx.ellipse_b[bn]/2, Mpx.sin[bn], DRAW_NORMAL );
								sprintf( Comment, "Electrode pos. %d", side );
								DrawMessage( Comment, DRAW_GREEN, 2, 2 );
							} else {
								sprintf( Comment, "ERROR\nElectrode pos. search %d", side );
								DrawMessage( Comment, DRAW_RED, 2, 0 );
							}
						}
						BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
					}
		// Elec. search result
			res = 0;
			if( Elec[DataLeft].BlobNo == ERROR )	res++;
			if( Elec[DataRight].BlobNo == ERROR )	res++;

        // 端子の左右の高さが大きく違う場合エラー   2017.04.09 tanaka   剥がれ検出暫定
			//v2.20 if(!bIsLTR50){
			if( (!bIsLTR50) && (!bIsLTR18) ){
				if(bIsMCR01==0){
					if( abs((Mpx.ydiff[Elec[DataRight].BlobNo] - Mpx.ydiff[Elec[DataLeft].BlobNo])) > 15 )   res = 1;
				} else if((Option & BACKINSP_MODE)==0) {
					if( abs((Mpx.ydiff[Elec[DataRight].BlobNo] - Mpx.ydiff[Elec[DataLeft].BlobNo])) > 10 )   res = 1;
				} else {
					if( abs((Mpx.ydiff[Elec[DataRight].BlobNo] - Mpx.ydiff[Elec[DataLeft].BlobNo])) > 30 )   res = 1;	// 2017.08.22 MCR01暫定
				}
			} else {
				if((Option & BACKINSP_MODE)==0) {
					if( abs((Mpx.xdiff[Elec[DataLower].BlobNo] - Mpx.xdiff[Elec[DataUpper].BlobNo])) > 40 )   res = 1;
				} else {
					if( abs((Mpx.xdiff[Elec[DataLower].BlobNo] - Mpx.xdiff[Elec[DataUpper].BlobNo])) > 40 )   res = 1;	// 暫定
				}
			}
			//    // G2エリアのC1見え 2017.04.09 tanaka 暫定
	//		if(bIsESR25){
	//			if( Mpx.xdiff[Elec[DataRight].BlobNo] > 70 ) res = 1;  //InspPara.ElecXmax[TWA_Spec].nXsize
	//			if( Mpx.xdiff[Elec[DataLeft].BlobNo]  > 70 ) res = 1;  //InspPara.ElecXmax[TWA_Spec].nXsize
	//		}

        // チップ傾き
			ChipAtan = 0;
			if( res == 0 ){
				//v2.20 if(!bIsLTR50){
				if( (!bIsLTR50) && (!bIsLTR18) ){
					xl = Mpx.center_x[Elec[DataRight].BlobNo] - Mpx.center_x[Elec[DataLeft].BlobNo];
					yl = Mpx.center_y[Elec[DataRight].BlobNo] - Mpx.center_y[Elec[DataLeft].BlobNo];
					ChipAtan = R_atan( xl, yl);
					if( i_mode & INSPECT_STEP ) {
						sprintf( Comment, "atan(X:%d Y:%d):%d", xl, yl, ChipAtan );
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					}
				} else {
					xl = Mpx.center_x[Elec[DataLower].BlobNo] - Mpx.center_x[Elec[DataUpper].BlobNo];
					yl = Mpx.center_y[Elec[DataLower].BlobNo] - Mpx.center_y[Elec[DataUpper].BlobNo];
					ChipAtan = - R_atan( yl, xl);
					if( i_mode & INSPECT_STEP ) {
						sprintf( Comment, "atan(X:%d Y:%d):%d", xl, yl, ChipAtan );
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					}
				}
			}

		// Result
			switch( res ) {
				case 0:
					return OK;
					break;
				case 1:		// search Error
					return ERROR3;
					break;
				case 2:		// NO chip or reverse
					return ERROR4;
					break;
				default:
					return ERROR5;
					break;
			}

	}	// ChipBinMeasure( void ) end


//================================================
//		Insp. chip reverse
//		チップ表\裏確認
//================================================

int		InspReverse( void )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						bnL, bnR;
		int						bnUp,bnLo;
		int						xmin, xmax, xsize;
		int						ymin, ymax, ysize;
		int						r_level, level;
		R_RECT					area;

		// reverse level
//			r_level = (ChipEsLevel.nData/2);
			if((Option & BACKINSP_MODE) ==0){		// 表面検査		  20170922 tanaka
				r_level = IGuideEsLevel.nData -20;  // 表\裏レベル暫定 20170128 tanaka
				if(r_level < ChipEsLevel.nLower)    r_level=ChipEsLevel.nLower;
			} else {								// 裏面検査		  20170922 tanaka
				r_level=ChipEsLevel.nLower;
			}

			//v2.20 if(!bIsLTR50){
			if( (!bIsLTR50) && (!bIsLTR18) ){
				// area set
				bnL = Elec[DataLeft].BlobNo;
				bnR = Elec[DataRight].BlobNo;

				// --- X pos.
				if(bIsMCR01){		// PILOT dot mark misjudge  20171005 tanaka
					xsize		= (Mpx.imin[bnR]-Mpx.imax[bnL])-20;
				} else {
					xsize		= (Mpx.imin[bnR]-Mpx.imax[bnL])/2;
				}
				area.left	= (Mpx.imax[bnL]+Mpx.imin[bnR]-xsize)/2;
				area.right	= (Mpx.imax[bnL]+Mpx.imin[bnR]+xsize)/2;
				// --- Y pos.
				ymin		= max(Mpx.jmin[bnL],Mpx.jmin[bnR]);
				ymax		= min(Mpx.jmax[bnL],Mpx.jmax[bnR]);
				if(bIsMCR01){		// PILOT dot mark misjudge  20171005 tanaka
					ysize		= (ymax-ymin)-20;
				} else {
					ysize		= (ymax-ymin)/2;
				}
				area.top	= (ymin+ymax-ysize)/2;
				area.bottom	= (ymin+ymax+ysize)/2;
			} else {
				// area set
				bnUp= Elec[DataUpper].BlobNo;
				bnLo= Elec[DataLower].BlobNo;
				// --- Y pos.
				ysize		= (Mpx.jmin[bnLo]-Mpx.jmax[bnUp])/2;
				area.top	= (Mpx.jmax[bnUp]+Mpx.jmin[bnLo]-ysize)/2;
				area.bottom	= (Mpx.jmax[bnUp]+Mpx.jmin[bnLo]+ysize)/2;
				// --- X pos.
				xmin		= max(Mpx.imin[bnUp],Mpx.imin[bnLo]);
				xmax		= min(Mpx.imax[bnUp],Mpx.imax[bnLo]);
				xsize		= (xmax-xmin)/2;
				area.left	= (xmin+xmax-xsize)/2;
				area.right	= (xmin+xmax+xsize)/2;
			}

		// 明るさのヒストグラム
			level = R_get_histgram( area.left, area.top, area.right, area.bottom, Gray_address[grayPn] );
			if( level < 0 )	return ERROR;
			// view
				if( i_mode & INSPECT_STEP ) {
					R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
					// --- comment
					sprintf( Comment, "Check reverse[level:%d(%d)]", level, r_level );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					R_DrawCls();
				}

		// チップ表裏確認
			if((Option & BACKINSP_MODE) ==0){		// 表面検査
				if( level > r_level )	return ERROR2;
			} else {								// 裏面検査
				if( level < r_level )	return ERROR2;
			}
			return OK;

	}	// InspReverse( void ) end


/////////////////////////////////////////////////////
// Keijo Chip Inspect 2016.05.04
/////////////////////////////////////////////////////
int	KeijoElectrodeInspect(void)
{
	int                     rx, ry, dn, fail_min, fail_max;
	int                     ds;
	int                     ElecEdgeX[2][4][2][640];					//Data edge y min max
	int                     ElecEdgeY[2][4][2][640];					//Data edge y min max
	int						side;
	int						i_mode = R_CS_GetInspectMode();
	int						res = OK;

	// C1,C12のコーナーの位置が分かっていること
	//ES Data Clear

	for(ds=0;ds<PosYmax+1;ds++){
		for( dn=0; dn<640; dn++ ) {
			ElecEdgeY[0][ds][0][dn] = 0xffff;
			ElecEdgeY[0][ds][1][dn] = 0xffff;
			ElecEdgeY[1][ds][0][dn] = 0xffff;
			ElecEdgeY[1][ds][1][dn] = 0xffff;
			ElecEdgeX[0][ds][0][dn] = 0xffff;
			ElecEdgeX[0][ds][1][dn] = 0xffff;
			ElecEdgeX[1][ds][0][dn] = 0xffff;
			ElecEdgeX[1][ds][1][dn] = 0xffff;
		}
	}

	//v2.20 if(!bIsLTR50){
	if( (!bIsLTR50) && (!bIsLTR18) ){
		//Data Edge Left PosYmin
		side = PosYmin;
		for( rx=0,dn=0; rx<FxSize; rx++ ) {
			if( ElecEdge[DataLeft][side].dataR[rx] != OK )	continue;
			if(bIsMCR18 && ElecC12[DataLeft].CornerPosX[2] !=0xffff && ElecC12[DataLeft].CornerPosX[2] <  ElecEdge[DataLeft][side].dataX[rx]) continue;
			ElecEdgeY[DataLeft][side][0][dn] = ElecEdge[DataLeft][side].dataX[rx];
			ElecEdgeY[DataLeft][side][1][dn] = ElecEdge[DataLeft][side].dataY[rx];
			dn++;
		}
		//Data Edge Left PosYmax
		side = PosYmax;
		for( rx=0,dn=0; rx<FxSize; rx++ ) {
			if( ElecEdge[DataLeft][side].dataR[rx] != OK )	continue;
			if(bIsMCR18 && ElecC12[DataLeft].CornerPosX[3] !=0xffff && ElecC12[DataLeft].CornerPosX[3] <  ElecEdge[DataLeft][side].dataX[rx]) continue;
			ElecEdgeY[DataLeft][side][0][dn] = ElecEdge[DataLeft][side].dataX[rx];
			ElecEdgeY[DataLeft][side][1][dn] = ElecEdge[DataLeft][side].dataY[rx];
			dn++;
		}

		// Elec. Size Y Judge Left Side
		res = OK; fail_min = fail_max = 0;
		for( dn=10; dn<630; dn++ ) {
			if( ElecEdgeY[DataLeft][PosYmin][1][dn+10] == 0xffff )	break;
			if( ElecEdgeY[DataLeft][PosYmax][1][dn+10] == 0xffff )	break;
			if(( ElecEdgeY[DataLeft][PosYmax][1][dn]-ElecEdgeY[DataLeft][PosYmin][1][dn] ) > InspPara.ElecYmax[TWA_Spec].nYsize ){
				fail_max++;
				if( i_mode & INSPECT_STEP ) {
					R_DrawLine( DRAW_RED, ElecEdgeY[DataLeft][PosYmin][0][dn], ElecEdgeY[DataLeft][PosYmin][1][dn], ElecEdgeY[DataLeft][PosYmax][0][dn], ElecEdgeY[DataLeft][PosYmax][1][dn], DRAW_NORMAL, SOLID_LINE );	// 縦 Line
				}
			}

			if(( ElecEdgeY[DataLeft][PosYmax][1][dn]-ElecEdgeY[DataLeft][PosYmin][1][dn] ) < InspPara.ElecYmin[TWA_Spec].nYsize ){
				fail_min++;
				if( i_mode & INSPECT_STEP ) {
					R_DrawLine( DRAW_RED, ElecEdgeY[DataLeft][PosYmin][0][dn], ElecEdgeY[DataLeft][PosYmin][1][dn], ElecEdgeY[DataLeft][PosYmax][0][dn], ElecEdgeY[DataLeft][PosYmax][1][dn], DRAW_NORMAL, SOLID_LINE );	// 縦 Line
				}
			}
		}
		if(( fail_min > InspPara.ElecYmin[TWA_Spec].nXsize )||( fail_max > InspPara.ElecYmax[TWA_Spec].nXsize )){
			if( i_mode & INSPECT_STEP ) {
				sprintf( Comment, "Electrode Left Side W error");
				DrawMessage( Comment,DRAW_RED , 2, 2 );
			}
			res = ERROR6;
		}
		if( res != OK )	return(res);

		//Data Edge Right PosYmin
		side = PosYmin;
		for( rx=0,dn=0; rx<FxSize; rx++ ) {
			if( ElecEdge[DataRight][side].dataR[rx] != OK )	continue;
			if(bIsMCR18 && ElecC12[DataRight].CornerPosX[0] != 0xffff && ElecC12[DataRight].CornerPosX[0] >  ElecEdge[DataRight][side].dataX[rx]) continue;
			ElecEdgeY[DataRight][side][0][dn] = ElecEdge[DataRight][side].dataX[rx];
			ElecEdgeY[DataRight][side][1][dn] = ElecEdge[DataRight][side].dataY[rx];
			dn++;
		}

		//Data Edge Right PosYmax
		side = PosYmax;
		for( rx=0,dn=0; rx<FxSize; rx++ ) {
			if( ElecEdge[DataRight][side].dataR[rx] != OK )	continue;
			if(bIsMCR18 && ElecC12[DataRight].CornerPosX[1] != 0xffff && ElecC12[DataRight].CornerPosX[1] >  ElecEdge[DataRight][side].dataX[rx]) continue;
			ElecEdgeY[DataRight][side][0][dn] = ElecEdge[DataRight][side].dataX[rx];
			ElecEdgeY[DataRight][side][1][dn] = ElecEdge[DataRight][side].dataY[rx];
			dn++;
		}

		// Elec. Size Y Judge Right Side
		res = OK; fail_min = fail_max = 0;
		for( dn=10; dn<630; dn++ ) {
			if( ElecEdgeY[DataRight][PosYmin][1][dn+10] == 0xffff )	break;
			if( ElecEdgeY[DataRight][PosYmax][1][dn+10] == 0xffff )	break;
			if(( ElecEdgeY[DataRight][PosYmax][1][dn]-ElecEdgeY[DataRight][PosYmin][1][dn] ) > InspPara.ElecYmax[TWA_Spec].nYsize ){
				fail_max++;
				if( i_mode & INSPECT_STEP ) {
					R_DrawLine( DRAW_RED, ElecEdgeY[DataRight][PosYmin][0][dn], ElecEdgeY[DataRight][PosYmin][1][dn], ElecEdgeY[DataRight][PosYmax][0][dn], ElecEdgeY[DataRight][PosYmax][1][dn], DRAW_NORMAL, SOLID_LINE );	// 縦 Line
				}
			}
			if(( ElecEdgeY[DataRight][PosYmax][1][dn]-ElecEdgeY[DataRight][PosYmin][1][dn] ) < InspPara.ElecYmin[TWA_Spec].nYsize ){
				fail_min++;
				if( i_mode & INSPECT_STEP ) {
					R_DrawLine( DRAW_RED, ElecEdgeY[DataRight][PosYmin][0][dn], ElecEdgeY[DataRight][PosYmin][1][dn], ElecEdgeY[DataRight][PosYmax][0][dn], ElecEdgeY[DataRight][PosYmax][1][dn], DRAW_NORMAL, SOLID_LINE );	// 縦 Line
				}
			}
		}

		if(( fail_min > InspPara.ElecYmin[TWA_Spec].nXsize )||( fail_max > InspPara.ElecYmax[TWA_Spec].nXsize )){
			if( i_mode & INSPECT_STEP ) {
				sprintf( Comment, "Electrode Right Side W error");
				DrawMessage( Comment,DRAW_RED , 2, 2 );
			}
			res = ERROR6;
		}

		if( res != OK )	return(res);
	} else {
		//Data Edge Upper PosXmin
		side = PosXmin;
		for( ry=0,dn=0; ry<FySize; ry++ ) {
			if( ElecEdge[DataUpper][side].dataR[ry] != OK )	continue;
			ElecEdgeX[DataUpper][side][0][dn] = ElecEdge[DataUpper][side].dataX[ry];
			ElecEdgeX[DataUpper][side][1][dn] = ElecEdge[DataUpper][side].dataY[ry];
			dn++;
		}
		//Data Edge Left PosYmax
		side = PosXmax;
		for( ry=0,dn=0; ry<FySize; ry++ ) {
			if( ElecEdge[DataUpper][side].dataR[ry] != OK )	continue;
			ElecEdgeX[DataUpper][side][0][dn] = ElecEdge[DataUpper][side].dataX[ry];
			ElecEdgeX[DataUpper][side][1][dn] = ElecEdge[DataUpper][side].dataY[ry];
			dn++;
		}

		// Elec. Size Y Judge Left Side
		res = OK; fail_min = fail_max = 0;
		for( dn=10; dn<470; dn++ ) {
			if( ElecEdgeX[DataUpper][PosXmin][0][dn+10] == 0xffff )	break;
			if( ElecEdgeX[DataUpper][PosXmax][0][dn+10] == 0xffff )	break;
			if(( ElecEdgeX[DataUpper][PosXmax][0][dn]-ElecEdgeX[DataUpper][PosXmin][0][dn] ) > InspPara.ElecXmax[TWA_Spec].nXsize ){
				fail_max++;
				if( i_mode & INSPECT_STEP ) {
					R_DrawLine( DRAW_RED, ElecEdgeX[DataUpper][PosXmin][0][dn], ElecEdgeX[DataUpper][PosXmin][1][dn], ElecEdgeX[DataUpper][PosXmax][0][dn], ElecEdgeX[DataUpper][PosXmax][1][dn], DRAW_NORMAL, SOLID_LINE );	// 縦 Line
				}
			}

			if(( ElecEdgeX[DataUpper][PosXmax][0][dn]-ElecEdgeX[DataUpper][PosXmin][0][dn] ) < InspPara.ElecXmin[TWA_Spec].nXsize ){
				fail_min++;
				if( i_mode & INSPECT_STEP ) {
					R_DrawLine( DRAW_RED, ElecEdgeX[DataUpper][PosXmin][0][dn], ElecEdgeX[DataUpper][PosXmin][1][dn], ElecEdgeX[DataUpper][PosXmax][0][dn], ElecEdgeX[DataUpper][PosXmax][1][dn], DRAW_NORMAL, SOLID_LINE );	// 縦 Line
				}
			}
		}
		if(( fail_min > InspPara.ElecXmin[TWA_Spec].nYsize )||( fail_max > InspPara.ElecXmax[TWA_Spec].nYsize )){
			if( i_mode & INSPECT_STEP ) {
				sprintf( Comment, "Electrode Upper Side L error" );
				DrawMessage( Comment,DRAW_RED , 2, 2 );
			}
			res = ERROR5;
		}
		if( res != OK )	return(res);

		//Data Edge Lower PosXmin
		side = PosXmin;
		for( ry=0,dn=0; ry<FySize; ry++ ) {
			if( ElecEdge[DataLower][side].dataR[ry] != OK )	continue;
			ElecEdgeX[DataLower][side][0][dn] = ElecEdge[DataLower][side].dataX[ry];
			ElecEdgeX[DataLower][side][1][dn] = ElecEdge[DataLower][side].dataY[ry];
			dn++;
		}

		//Data Edge Right PosYmax
		side = PosXmax;
		for( ry=0,dn=0; ry<FySize; ry++ ) {
			if( ElecEdge[DataLower][side].dataR[ry] != OK )	continue;
			ElecEdgeX[DataLower][side][0][dn] = ElecEdge[DataLower][side].dataX[ry];
			ElecEdgeX[DataLower][side][1][dn] = ElecEdge[DataLower][side].dataY[ry];
			dn++;
		}

		// Elec. Size Y Judge Right Side
		res = OK; fail_min = fail_max = 0;
		for( dn=10; dn<470; dn++ ) {
			if( ElecEdgeX[DataLower][PosXmin][0][dn+10] == 0xffff )	break;
			if( ElecEdgeX[DataLower][PosXmax][0][dn+10] == 0xffff )	break;
			if(( ElecEdgeX[DataLower][PosXmax][0][dn]-ElecEdgeX[DataLower][PosXmin][0][dn] ) > InspPara.ElecXmax[TWA_Spec].nXsize ){
				fail_max++;
				if( i_mode & INSPECT_STEP ) {
					R_DrawLine( DRAW_RED, ElecEdgeX[DataLower][PosXmin][0][dn], ElecEdgeX[DataLower][PosXmin][1][dn], ElecEdgeX[DataLower][PosXmax][0][dn], ElecEdgeX[DataLower][PosXmax][1][dn], DRAW_NORMAL, SOLID_LINE );	// 縦 Line
				}
			}
			if(( ElecEdgeX[DataLower][PosXmax][0][dn]-ElecEdgeX[DataLower][PosXmin][0][dn] ) < InspPara.ElecXmin[TWA_Spec].nXsize ){
				fail_min++;
				if( i_mode & INSPECT_STEP ) {
					R_DrawLine( DRAW_RED, ElecEdgeX[DataLower][PosXmin][0][dn], ElecEdgeX[DataLower][PosXmin][1][dn], ElecEdgeX[DataLower][PosXmax][0][dn], ElecEdgeX[DataLower][PosXmax][1][dn], DRAW_NORMAL, SOLID_LINE );	// 縦 Line
				}
			}
		}

		if(( fail_min > InspPara.ElecXmin[TWA_Spec].nYsize )||( fail_max > InspPara.ElecXmax[TWA_Spec].nYsize )){
			if( i_mode & INSPECT_STEP ) {
				sprintf( Comment, "Electrode Lower Side L error");
				DrawMessage( Comment,DRAW_RED , 2, 2 );
			}
			res = ERROR5;
		}

		if( res != OK )	return(res);
	}

	return OK;
}

//================================================
//		Insp. chip size
//================================================

int		InspChipSize( void )
	{
		int						bnL , bnR;
		int						bnUp, bnLo;
		int						xmin, xmax, xsize[2], xsizeM[2];
		int						ymin, ymax, ysize[2], ysizeM[2];
		int						judgeXmin, judgeXmax, judgeXminM, judgeXmaxM;
		int						judgeYmin, judgeYmax, judgeYminM, judgeYmaxM;
		int						side;
		int						i_mode = R_CS_GetInspectMode();
		int						d_color;
		int						res = OK;
//		int                     rx, dn, fail_min, fail_max;
//        int                     ds;
//        int                     ElecEdgeY[2][4][2][640];					//Data edge y min max


		// Check bin measure blob No.
			if( Elec[DataLeft].BlobNo <= 0 )	return ERROR;
			if( Elec[DataRight].BlobNo <= 0 )	return ERROR;
			bnL = bnUp = Elec[DataLeft].BlobNo;
			bnR = bnLo = Elec[DataRight].BlobNo;

		// Check rough size
			judgeXmin = InspPara.ChipXmin[TWA_Spec].nXsize* 8/10;
			judgeXmax = InspPara.ChipXmax[TWA_Spec].nXsize*12/10;
			judgeYmin = InspPara.ChipYmin[TWA_Spec].nYsize* 8/10;
			judgeYmax = InspPara.ChipYmax[TWA_Spec].nYsize*12/10;

			//v2.20 if(!bIsLTR50){	// normal
			if( (!bIsLTR50) && (!bIsLTR18) ){	// normal
				// X size
					xmin		= Mpx.imin[bnL];
					xmax		= Mpx.imax[bnR];
					xsize[0]	= xmax-xmin+1;
					if(( xsize[0] < judgeXmin )||( xsize[0] > judgeXmax )) {
						if( i_mode & INSPECT_STEP ) {
							xsizeM[0]	= (xsize[0]		*SystemPara.nRateX+500)/1000;
							judgeXminM	= (judgeXmin	*SystemPara.nRateX+500)/1000;
							judgeXmaxM	= (judgeXmax	*SystemPara.nRateX+500)/1000;
							sprintf( Comment, " Chip X size:%d3[bit] %d4[um](%4d-%4d)", xsize[0], xsizeM[0], judgeXminM, judgeXmaxM );
							DrawMessage( Comment, DRAW_RED, 2, 2 );
						}
						return ERROR2;
					}
				// Y size
					if(( Mpx.ydiff[bnL] < judgeYmin )||( Mpx.ydiff[bnL] > judgeYmax )) {
						if( i_mode & INSPECT_STEP ) {
							ysizeM[0]	= (Mpx.ydiff[bnL]	*SystemPara.nRateY+500)/1000;
							judgeYminM	= (judgeYmin		*SystemPara.nRateY+500)/1000;
							judgeYmaxM	= (judgeYmax		*SystemPara.nRateY+500)/1000;
							sprintf( Comment, " Chip Y(L) size:%d3[bit] %d4[um](%4d-%4d)", Mpx.ydiff[bnL], ysizeM[0], judgeYminM, judgeYmaxM );
							DrawMessage( Comment, DRAW_RED, 2, 2 );
						}
						return ERROR3;
					}
					if(( Mpx.ydiff[bnR] < judgeYmin )||( Mpx.ydiff[bnR] > judgeYmax )) {
						if( i_mode & INSPECT_STEP ) {
							ysizeM[0]	= (Mpx.ydiff[bnR]	*SystemPara.nRateY+500)/1000;
							judgeYminM	= (judgeYmin		*SystemPara.nRateY+500)/1000;
							judgeYmaxM	= (judgeYmax		*SystemPara.nRateY+500)/1000;
							sprintf( Comment, " Chip Y(R) size:%d3[bit] %d4[um](%4d-%4d)", Mpx.ydiff[bnR], ysizeM[0], judgeYminM, judgeYmaxM );
							DrawMessage( Comment, DRAW_RED, 2, 2 );
						}
						return ERROR3;
					}
				} else {		//LTR
					// Y size
						ymin		= Mpx.jmin[bnUp];
						ymax		= Mpx.jmax[bnLo];
						ysize[0]	= ymax-ymin+1;
						if(( ysize[0] < judgeYmin )||( ysize[0] > judgeYmax )) {
							if( i_mode & INSPECT_STEP ) {
								ysizeM[0]	= (ysize[0]		*SystemPara.nRateY+500)/1000;
								judgeYminM	= (judgeYmin	*SystemPara.nRateY+500)/1000;
								judgeYmaxM	= (judgeYmax	*SystemPara.nRateY+500)/1000;
								sprintf( Comment, " Chip Y size:%d[bit] %d4[um](%4d-%4d)", ysize[0], ysizeM[0], judgeYminM, judgeYmaxM );
								DrawMessage( Comment, DRAW_RED, 2, 2 );
							}
							return ERROR3;
						}
					// X size
						if(( Mpx.xdiff[bnUp] < judgeXmin )||( Mpx.xdiff[bnUp] > judgeXmax )) {
							if( i_mode & INSPECT_STEP ) {
								xsizeM[0]	= (Mpx.xdiff[bnUp]	*SystemPara.nRateX+500)/1000;
								judgeXminM	= (judgeXmin		*SystemPara.nRateX+500)/1000;
								judgeXmaxM	= (judgeXmax		*SystemPara.nRateX+500)/1000;
								sprintf( Comment, " Chip X(U) size:%d[bit] %d4[um](%4d-%4d)", Mpx.xdiff[bnUp], xsizeM[0], judgeXminM, judgeXmaxM );
								DrawMessage( Comment, DRAW_RED, 2, 2 );
							}
							return ERROR2;
						}
						if(( Mpx.xdiff[bnLo] < judgeXmin )||( Mpx.xdiff[bnLo] > judgeXmax )) {
							if( i_mode & INSPECT_STEP ) {
								xsizeM[0]	= (Mpx.xdiff[bnLo]	*SystemPara.nRateX+500)/1000;
								judgeXminM	= (judgeXmin		*SystemPara.nRateX+500)/1000;
								judgeXmaxM	= (judgeXmax		*SystemPara.nRateX+500)/1000;
								sprintf( Comment, " Chip X(B) size:%d[bit] %d4[um](%4d-%4d)", Mpx.xdiff[bnLo], xsizeM[0], judgeXminM, judgeXmaxM );
								DrawMessage( Comment, DRAW_RED, 2, 2 );
							}
							return ERROR2;
						}
			}
/*
/////////////////////////////////////////////////////
// Keijo Chip Inspect 2016.05.04
/////////////////////////////////////////////////////
					//ES Data Clear
                        for(ds=0;ds<PosYmax+1;ds++){
                            for( dn=0; dn<640; dn++ ) {
								ElecEdgeY[0][ds][0][dn] = 0xffff;
								ElecEdgeY[0][ds][1][dn] = 0xffff;
								ElecEdgeY[1][ds][0][dn] = 0xffff;
								ElecEdgeY[1][ds][1][dn] = 0xffff;

                            }
                        }
*/
		// Set edge data
			SetElecEdge( DataLeft );
            twa_TimeRead( &twaTime[1],8 );
/*
				//Data Edge Left PosYmin
							side = PosYmin;
						for( rx=0,dn=0; rx<FxSize; rx++ ) {
							if( ElecEdge[DataLeft][side].dataR[rx] != OK )	continue;
                                ElecEdgeY[DataLeft][side][0][dn] = ElecEdge[DataLeft][side].dataX[rx];
                                ElecEdgeY[DataLeft][side][1][dn] = ElecEdge[DataLeft][side].dataY[rx];
								dn++;
						}
						//for( ; dn<640; dn++ ) {	先にデータクリアしているので不要
						//        ElecEdgeY[DataLeft][side][0][dn] = 0xffff;
						//        ElecEdgeY[DataLeft][side][1][dn] = 0xffff;
						//}
						//Data Edge Left PosYmax
							side = PosYmax;
						for( rx=0,dn=0; rx<FxSize; rx++ ) {
							if( ElecEdge[DataLeft][side].dataR[rx] != OK )	continue;
                                ElecEdgeY[DataLeft][side][0][dn] = ElecEdge[DataLeft][side].dataX[rx];
                                ElecEdgeY[DataLeft][side][1][dn] = ElecEdge[DataLeft ][side].dataY[rx];
								dn++;
						}
						//for( ; dn<640; dn++ ) {  先にデータクリアしているので不要
						//        ElecEdgeY[DataLeft][side][0][dn] = 0xffff;
						//        ElecEdgeY[DataLeft][side][1][dn] = 0xffff;
						//}

						// Elec. Size Y Judge Left Side
							res = OK; fail_min = fail_max = 0;
                            for( dn=10; dn<630; dn++ ) {
                                if( ElecEdgeY[DataLeft][PosYmin][1][dn+10] == 0xffff )	break;
                                if( ElecEdgeY[DataLeft][PosYmax][1][dn+10] == 0xffff )	break;
                                if(( ElecEdgeY[DataLeft][PosYmax][1][dn]-ElecEdgeY[DataLeft][PosYmin][1][dn] ) > InspPara.ElecYmax[TWA_Spec].nYsize ){
									fail_max++;
//									ans = ERROR;
									if( i_mode & INSPECT_STEP ) {
                                        R_DrawLine( DRAW_RED, ElecEdgeY[DataLeft][PosYmin][0][dn], ElecEdgeY[DataLeft][PosYmin][1][dn], ElecEdgeY[DataLeft][PosYmax][0][dn], ElecEdgeY[DataLeft][PosYmax][1][dn], DRAW_NORMAL, SOLID_LINE );	// 縦 Line
									}
								}else{
//									fail_max = 0;
								}
                                if(( ElecEdgeY[DataLeft][PosYmax][1][dn]-ElecEdgeY[DataLeft][PosYmin][1][dn] ) < InspPara.ElecYmin[TWA_Spec].nYsize ){
									fail_min++;
//									ans = ERROR;
									if( i_mode & INSPECT_STEP ) {
                                        R_DrawLine( DRAW_RED, ElecEdgeY[DataLeft][PosYmin][0][dn], ElecEdgeY[DataLeft][PosYmin][1][dn], ElecEdgeY[DataLeft][PosYmax][0][dn], ElecEdgeY[DataLeft][PosYmax][1][dn], DRAW_NORMAL, SOLID_LINE );	// 縦 Line
									}
								}else{
//									fail_min = 0;
								}
							}
							if(( fail_min > InspPara.ElecYmin[TWA_Spec].nXsize )||( fail_max > InspPara.ElecYmax[TWA_Spec].nXsize )){
								if( i_mode & INSPECT_STEP ) {
									sprintf( Comment, "Electrode Left Side W error");
									DrawMessage( Comment,DRAW_RED , 2, 2 );
								}
								  res = ERROR6;
							}
							if( res != OK )	return(res);

*/
			SetElecEdge( DataRight );
            twa_TimeRead( &twaTime[1],9 );
/*
						 //Data Edge Right PosYmin
							side = PosYmin;
						for( rx=0,dn=0; rx<FxSize; rx++ ) {
							if( ElecEdge[DataRight][side].dataR[rx] != OK )	continue;
                                ElecEdgeY[DataRight][side][0][dn] = ElecEdge[DataRight][side].dataX[rx];
                                ElecEdgeY[DataRight][side][1][dn] = ElecEdge[DataRight][side].dataY[rx];
								dn++;
						}
						//for( ; dn<640; dn++ ) {  先にデータクリアしているので不要
						//        ElecEdgeY[DataRight][side][0][dn] = 0xffff;
						//        ElecEdgeY[DataRight][side][1][dn] = 0xffff;
						//}
						//Data Edge Right PosYmax
							side = PosYmax;
						for( rx=0,dn=0; rx<FxSize; rx++ ) {
							if( ElecEdge[DataRight][side].dataR[rx] != OK )	continue;
                                ElecEdgeY[DataRight][side][0][dn] = ElecEdge[DataRight][side].dataX[rx];
                                ElecEdgeY[DataRight][side][1][dn] = ElecEdge[DataRight][side].dataY[rx];
								dn++;
						}
						//for( ; dn<500; dn++ ) {  先にデータクリアしているので不要
						//        ElecEdgeY[DataRight][side][0][dn] = 0xffff;
						//        ElecEdgeY[DataRight][side][1][dn] = 0xffff;
						//}
						// Elec. Size Y Judge Right Side
							res = OK; fail_min = fail_max = 0;
                            for( dn=10; dn<630; dn++ ) {
                                if( ElecEdgeY[DataRight][PosYmin][1][dn+10] == 0xffff )	break;
                                if( ElecEdgeY[DataRight][PosYmax][1][dn+10] == 0xffff )	break;
                                if(( ElecEdgeY[DataRight][PosYmax][1][dn]-ElecEdgeY[DataRight][PosYmin][1][dn] ) > InspPara.ElecYmax[TWA_Spec].nYsize ){
									fail_max++;
//									ans = ERROR;
									if( i_mode & INSPECT_STEP ) {
                                        R_DrawLine( DRAW_RED, ElecEdgeY[DataRight][PosYmin][0][dn], ElecEdgeY[DataRight][PosYmin][1][dn], ElecEdgeY[DataRight][PosYmax][0][dn], ElecEdgeY[DataRight][PosYmax][1][dn], DRAW_NORMAL, SOLID_LINE );	// 縦 Line
									}
								}else{
//									fail_max = 0;
								}
                                if(( ElecEdgeY[DataRight][PosYmax][1][dn]-ElecEdgeY[DataRight][PosYmin][1][dn] ) < InspPara.ElecYmin[TWA_Spec].nYsize ){
									fail_min++;
//									ans = ERROR;
									if( i_mode & INSPECT_STEP ) {
                                        R_DrawLine( DRAW_RED, ElecEdgeY[DataRight][PosYmin][0][dn], ElecEdgeY[DataRight][PosYmin][1][dn], ElecEdgeY[DataRight][PosYmax][0][dn], ElecEdgeY[DataRight][PosYmax][1][dn], DRAW_NORMAL, SOLID_LINE );	// 縦 Line
									}
								}else{
//									fail_min = 0;
								}
							}
							if(( fail_min > InspPara.ElecYmin[TWA_Spec].nXsize )||( fail_max > InspPara.ElecYmax[TWA_Spec].nXsize )){
								if( i_mode & INSPECT_STEP ) {
									sprintf( Comment, "Electrode Right Side W error");
									DrawMessage( Comment,DRAW_RED , 2, 2 );
								}
								  res = ERROR6;
							}

							if( res != OK )	return(res);

*/

/////////////////////////////////////////////////////
// Keijo Chip Inspect 2016.05.04
/////////////////////////////////////////////////////



			SetElecEdge( DataLR );			// DataLeft, DataRight の処理が終了している事。
            twa_TimeRead( &twaTime[1],10 );
        // Electrode edge data
			if( CheckElecEdge( DataLR ) != OK )		return ERROR4;

			//v2.20 if(!bIsLTR50){	//normal
			if( (!bIsLTR50) && (!bIsLTR18) ){	//normal
				side = PosYmin;
					ElecEdge[DataLeft][side].MeanX	= ElecEdge[DataRight][side].MeanX	= ElecEdge[DataLR][side].MeanX;
					ElecEdge[DataLeft][side].MeanY	= ElecEdge[DataRight][side].MeanY	= ElecEdge[DataLR][side].MeanY;
					ElecEdge[DataLeft][side].Rev	= ElecEdge[DataRight][side].Rev		= ElecEdge[DataLR][side].Rev;
					ElecEdge[DataLeft][side].Result	= ElecEdge[DataRight][side].Result	= ElecEdge[DataLR][side].Result;
				side = PosYmax;
					ElecEdge[DataLeft][side].MeanX	= ElecEdge[DataRight][side].MeanX	= ElecEdge[DataLR][side].MeanX;
					ElecEdge[DataLeft][side].MeanY	= ElecEdge[DataRight][side].MeanY	= ElecEdge[DataLR][side].MeanY;
					ElecEdge[DataLeft][side].Rev	= ElecEdge[DataRight][side].Rev		= ElecEdge[DataLR][side].Rev;
					ElecEdge[DataLeft][side].Result	= ElecEdge[DataRight][side].Result	= ElecEdge[DataLR][side].Result;
				side = PosXmin;
					ElecEdge[DataLeft][side].MeanX	= ElecEdge[DataLR][side].MeanX;
					ElecEdge[DataLeft][side].MeanY	= ElecEdge[DataLR][side].MeanY;
					ElecEdge[DataLeft][side].Rev	= ElecEdge[DataRight][side].Rev		= ElecEdge[DataLR][side].Rev;
					ElecEdge[DataLeft][side].Result	= ElecEdge[DataLR][side].Result;
				side = PosXmax;
					ElecEdge[DataRight][side].MeanX	= ElecEdge[DataLR][side].MeanX;
					ElecEdge[DataRight][side].MeanY	= ElecEdge[DataLR][side].MeanY;
					ElecEdge[DataLeft][side].Rev	= ElecEdge[DataRight][side].Rev		= ElecEdge[DataLR][side].Rev;
					ElecEdge[DataRight][side].Result= ElecEdge[DataLR][side].Result;
			} else {
				side = PosXmin;
					ElecEdge[DataUpper][side].MeanX	= ElecEdge[DataLower][side].MeanX	= ElecEdge[DataUL][side].MeanX;
					ElecEdge[DataUpper][side].MeanY	= ElecEdge[DataLower][side].MeanY	= ElecEdge[DataUL][side].MeanY;
					ElecEdge[DataUpper][side].Rev	= ElecEdge[DataLower][side].Rev		= ElecEdge[DataUL][side].Rev;
					ElecEdge[DataUpper][side].Result= ElecEdge[DataLower][side].Result	= ElecEdge[DataUL][side].Result;
				side = PosXmax;
					ElecEdge[DataUpper][side].MeanX	= ElecEdge[DataLower][side].MeanX	= ElecEdge[DataUL][side].MeanX;
					ElecEdge[DataUpper][side].MeanY	= ElecEdge[DataLower][side].MeanY	= ElecEdge[DataUL][side].MeanY;
					ElecEdge[DataUpper][side].Rev	= ElecEdge[DataLower][side].Rev		= ElecEdge[DataUL][side].Rev;
					ElecEdge[DataUpper][side].Result= ElecEdge[DataLower][side].Result	= ElecEdge[DataUL][side].Result;
				side = PosYmin;
					ElecEdge[DataUpper][side].MeanX	= ElecEdge[DataUL][side].MeanX;
					ElecEdge[DataUpper][side].MeanY	= ElecEdge[DataUL][side].MeanY;
					ElecEdge[DataUpper][side].Rev	= ElecEdge[DataLower][side].Rev		= ElecEdge[DataUL][side].Rev;
					ElecEdge[DataUpper][side].Result= ElecEdge[DataUL][side].Result;
				side = PosYmax;
					ElecEdge[DataLower][side].MeanX	= ElecEdge[DataUL][side].MeanX;
					ElecEdge[DataLower][side].MeanY	= ElecEdge[DataUL][side].MeanY;
					ElecEdge[DataUpper][side].Rev	= ElecEdge[DataLower][side].Rev		= ElecEdge[DataUL][side].Rev;
					ElecEdge[DataLower][side].Result= ElecEdge[DataUL][side].Result;
			}

			if( CheckElecEdge( DataLeft ) != OK )	return ERROR4;
			if( CheckElecEdge( DataRight ) != OK )	return ERROR4;

			// C2がかぶっていない寸法を計測してしまうためこの位置で判定する
			if((res = KeijoElectrodeInspect())!= OK) return (res);

			//v2.20 if(!bIsLTR50){	//normal
			if( (!bIsLTR50) && (!bIsLTR18) ){	//normal
				side = PosYmin;
					ElecEdge[DataLeft][side].MeanX	= ElecEdge[DataRight][side].MeanX	= ElecEdge[DataLR][side].MeanX;
					ElecEdge[DataLeft][side].MeanY	= ElecEdge[DataRight][side].MeanY	= ElecEdge[DataLR][side].MeanY;
					ElecEdge[DataLeft][side].Rev	= ElecEdge[DataRight][side].Rev		= ElecEdge[DataLR][side].Rev;
					ElecEdge[DataLeft][side].Result	= ElecEdge[DataRight][side].Result	= ElecEdge[DataLR][side].Result;
				side = PosYmax;
					ElecEdge[DataLeft][side].MeanX	= ElecEdge[DataRight][side].MeanX	= ElecEdge[DataLR][side].MeanX;
					ElecEdge[DataLeft][side].MeanY	= ElecEdge[DataRight][side].MeanY	= ElecEdge[DataLR][side].MeanY;
					ElecEdge[DataLeft][side].Rev	= ElecEdge[DataRight][side].Rev		= ElecEdge[DataLR][side].Rev;
					ElecEdge[DataLeft][side].Result	= ElecEdge[DataRight][side].Result	= ElecEdge[DataLR][side].Result;
				side = PosXmin;
					ElecEdge[DataLeft][side].MeanX	= ElecEdge[DataLR][side].MeanX;
					ElecEdge[DataLeft][side].MeanY	= ElecEdge[DataLR][side].MeanY;
					ElecEdge[DataLeft][side].Rev	= ElecEdge[DataRight][side].Rev		= ElecEdge[DataLR][side].Rev;
					ElecEdge[DataLeft][side].Result	= ElecEdge[DataLR][side].Result;
				side = PosXmax;
					ElecEdge[DataRight][side].MeanX	= ElecEdge[DataLR][side].MeanX;
					ElecEdge[DataRight][side].MeanY	= ElecEdge[DataLR][side].MeanY;
					ElecEdge[DataLeft][side].Rev	= ElecEdge[DataRight][side].Rev		= ElecEdge[DataLR][side].Rev;
					ElecEdge[DataRight][side].Result= ElecEdge[DataLR][side].Result;
			} else {
				side = PosXmin;
					ElecEdge[DataUpper][side].MeanX	= ElecEdge[DataLower][side].MeanX	= ElecEdge[DataUL][side].MeanX;
					ElecEdge[DataUpper][side].MeanY	= ElecEdge[DataLower][side].MeanY	= ElecEdge[DataUL][side].MeanY;
					ElecEdge[DataUpper][side].Rev	= ElecEdge[DataLower][side].Rev		= ElecEdge[DataUL][side].Rev;
					ElecEdge[DataUpper][side].Result= ElecEdge[DataLower][side].Result	= ElecEdge[DataUL][side].Result;
				side = PosXmax;
					ElecEdge[DataUpper][side].MeanX	= ElecEdge[DataLower][side].MeanX	= ElecEdge[DataUL][side].MeanX;
					ElecEdge[DataUpper][side].MeanY	= ElecEdge[DataLower][side].MeanY	= ElecEdge[DataUL][side].MeanY;
					ElecEdge[DataUpper][side].Rev	= ElecEdge[DataLower][side].Rev		= ElecEdge[DataUL][side].Rev;
					ElecEdge[DataUpper][side].Result= ElecEdge[DataLower][side].Result	= ElecEdge[DataUL][side].Result;
				side = PosYmin;
					ElecEdge[DataUpper][side].MeanX	= ElecEdge[DataUL][side].MeanX;
					ElecEdge[DataUpper][side].MeanY	= ElecEdge[DataUL][side].MeanY;
					ElecEdge[DataUpper][side].Rev	= ElecEdge[DataLower][side].Rev		= ElecEdge[DataUL][side].Rev;
					ElecEdge[DataUpper][side].Result= ElecEdge[DataUL][side].Result;
				side = PosYmax;
					ElecEdge[DataLower][side].MeanX	= ElecEdge[DataUL][side].MeanX;
					ElecEdge[DataLower][side].MeanY	= ElecEdge[DataUL][side].MeanY;
					ElecEdge[DataUpper][side].Rev	= ElecEdge[DataLower][side].Rev		= ElecEdge[DataUL][side].Rev;
					ElecEdge[DataLower][side].Result= ElecEdge[DataUL][side].Result;
			}
		// Electrode corner pos. set
			ElecCornerPosSet();
            twa_TimeRead( &twaTime[1],11 );

        // Kuro Chip Insp //2012.03.28
			if(InspBinBl() != OK) {
				return res = ERROR7;
			}
            twa_TimeRead( &twaTime[1],12 );

        // Chip size ( Corner )
			// --- X size
			xsize[0] = Elec[DataLR].CornerPosX[2]-Elec[DataLR].CornerPosX[0]+1;
			xsize[1] = Elec[DataLR].CornerPosX[3]-Elec[DataLR].CornerPosX[1]+1;
			if(( xsize[0] < InspPara.ChipXmin[TWA_Spec].nXsize )||( xsize[0] > InspPara.ChipXmax[TWA_Spec].nXsize ))	res = ERROR5;
			if(( xsize[1] < InspPara.ChipXmin[TWA_Spec].nXsize )||( xsize[1] > InspPara.ChipXmax[TWA_Spec].nXsize ))	res = ERROR5;
			if( i_mode & INSPECT_STEP ) {
//				sprintf( Comment1, "Chip(top)    X size:%3d(%3d-%3d)", xsize[0], InspPara.ChipXmin[TWA_Spec].nXsize, InspPara.ChipXmax[TWA_Spec].nXsize );
				xsizeM[0] = (xsize[0]*SystemPara.nRateX+500)/1000;
				sprintf( Comment1, "Chip(top)    X size:%3d[bit] %4d[um](%4d-%4d)", xsize[0], xsizeM[0], InspParaM.ChipXmin[TWA_Spec].nXsize, InspParaM.ChipXmax[TWA_Spec].nXsize );
//				sprintf( Comment2, "Chip(bottom) X size:%3d(%3d-%3d)", xsize[1], InspPara.ChipXmin[TWA_Spec].nXsize, InspPara.ChipXmax[TWA_Spec].nXsize );
				xsizeM[1] = (xsize[1]*SystemPara.nRateX+500)/1000;
				sprintf( Comment2, "Chip(bottom) X size:%3d[bit] %4d[um](%4d-%4d)", xsize[1], xsizeM[1], InspParaM.ChipXmin[TWA_Spec].nXsize, InspParaM.ChipXmax[TWA_Spec].nXsize );
				sprintf( Comment, "%s\n%s", Comment1, Comment2 );
				if( res == OK )	d_color = DRAW_GREEN;
				else			d_color = DRAW_RED;
				DrawMessage( Comment, d_color, 2, 2 );
			}
			if( res != OK )	return(res);
			// --- Y size
			ysize[0] = Elec[DataLR].CornerPosY[1]-Elec[DataLR].CornerPosY[0]+1;
			ysize[1] = Elec[DataLR].CornerPosY[3]-Elec[DataLR].CornerPosY[2]+1;
			if(( ysize[0] < InspPara.ChipYmin[TWA_Spec].nYsize )||( ysize[0] > InspPara.ChipYmax[TWA_Spec].nYsize ))	res = ERROR6;
			if(( ysize[1] < InspPara.ChipYmin[TWA_Spec].nYsize )||( ysize[1] > InspPara.ChipYmax[TWA_Spec].nYsize ))	res = ERROR6;
			if( i_mode & INSPECT_STEP ) {
//				sprintf( Comment1, "Chip(left)  Y size:%3d(%3d-%3d)", ysize[0], InspPara.ChipYmin[TWA_Spec].nYsize, InspPara.ChipYmax[TWA_Spec].nYsize );
				ysizeM[0] = (ysize[0]*SystemPara.nRateY+500)/1000;
				sprintf( Comment1, "Chip(left)  Y size:%3d[bit] %4d[um](%4d-%4d)", ysize[0], ysizeM[0], InspParaM.ChipYmin[TWA_Spec].nYsize, InspParaM.ChipYmax[TWA_Spec].nYsize );
//				sprintf( Comment2, "Chip(right) Y size:%3d(%3d-%3d)", ysize[1], InspPara.ChipYmin[TWA_Spec].nYsize, InspPara.ChipYmax[TWA_Spec].nYsize );
				ysizeM[1] = (ysize[1]*SystemPara.nRateY+500)/1000;
				sprintf( Comment2, "Chip(right) Y size:%3d[bit] %4d[um](%4d-%4d)", ysize[1], ysizeM[1], InspParaM.ChipYmin[TWA_Spec].nYsize, InspParaM.ChipYmax[TWA_Spec].nYsize );
				sprintf( Comment, "%s\n%s", Comment1, Comment2 );
				if( res == OK )	d_color = DRAW_GREEN;
				else			d_color = DRAW_RED;
				DrawMessage( Comment, d_color, 2, 2 );
			}
			if( res != OK )	return(res);

		// result
			return OK;

	}	// InspChipSize( void ) end


//------------------------------------------------
//		Set Electrode edge
//------------------------------------------------

void	SetElecEdge( int elec )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						dn, side, data;
		int						bn;
		int						rx, ry;
		int						valid_xs;
		int						valid_xe;
		int						valid_ys;		//v2.11
		int						valid_ye;		//v2.11

		// Initial	( Data clear )
			for( side=0; side<4; side++ ) {
				for( dn=0; dn<700; dn++ ) {
					ElecEdge[elec][side].dataX[dn]	= 0x0ffff;
					ElecEdge[elec][side].dataY[dn]	= 0x0ffff;
					ElecEdge[elec][side].dataR[dn]	= 0x0ffff;
				}
			}

		// union elec. data
			if( elec == DataLR ) {
				//v2.20 if(!bIsLTR50){  //normal
				if( (!bIsLTR50) && (!bIsLTR18) ){  //normal
					// --- Xmin
					side = PosXmin;
					data = DataLeft;
					for( dn=0; dn<700; dn++ ) {
						ElecEdge[elec][side].dataX[dn]	= ElecEdge[data][side].dataX[dn];
						ElecEdge[elec][side].dataY[dn]	= ElecEdge[data][side].dataY[dn];
						ElecEdge[elec][side].dataR[dn]	= ElecEdge[data][side].dataR[dn];
					}
					// --- Xmax
					side = PosXmax;
					data = DataRight;
					for( dn=0; dn<700; dn++ ) {
						ElecEdge[elec][side].dataX[dn]	= ElecEdge[data][side].dataX[dn];
						ElecEdge[elec][side].dataY[dn]	= ElecEdge[data][side].dataY[dn];
						ElecEdge[elec][side].dataR[dn]	= ElecEdge[data][side].dataR[dn];
					}
					// --- Ymin
					side = PosYmin;
					for( dn=0; dn<700; dn++ ) {
						if( ElecEdge[DataLeft][side].dataR[dn] == OK )			data = DataLeft;
						else if( ElecEdge[DataRight][side].dataR[dn] == OK )	data = DataRight;
						else	continue;
						ElecEdge[elec][side].dataX[dn]	= ElecEdge[data][side].dataX[dn];
						ElecEdge[elec][side].dataY[dn]	= ElecEdge[data][side].dataY[dn];
						ElecEdge[elec][side].dataR[dn]	= ElecEdge[data][side].dataR[dn];
					}
					// --- Ymax
					side = PosYmax;
					for( dn=0; dn<700; dn++ ) {
						if( ElecEdge[DataLeft][side].dataR[dn] == OK )			data = DataLeft;
						else if( ElecEdge[DataRight][side].dataR[dn] == OK )	data = DataRight;
						else	continue;
						ElecEdge[elec][side].dataX[dn]	= ElecEdge[data][side].dataX[dn];
						ElecEdge[elec][side].dataY[dn]	= ElecEdge[data][side].dataY[dn];
						ElecEdge[elec][side].dataR[dn]	= ElecEdge[data][side].dataR[dn];
					}
				} else {		//LTR
					// --- Ymin
					side = PosYmin;
					data = DataUpper;
					for( dn=0; dn<700; dn++ ) {
						ElecEdge[elec][side].dataX[dn]	= ElecEdge[data][side].dataX[dn];
						ElecEdge[elec][side].dataY[dn]	= ElecEdge[data][side].dataY[dn];
						ElecEdge[elec][side].dataR[dn]	= ElecEdge[data][side].dataR[dn];
					}
					// --- Ymax
					side = PosYmax;
					data = DataLower;
					for( dn=0; dn<700; dn++ ) {
						ElecEdge[elec][side].dataX[dn]	= ElecEdge[data][side].dataX[dn];
						ElecEdge[elec][side].dataY[dn]	= ElecEdge[data][side].dataY[dn];
						ElecEdge[elec][side].dataR[dn]	= ElecEdge[data][side].dataR[dn];
					}
					// --- Xmin
					side = PosXmin;
					for( dn=0; dn<700; dn++ ) {
						if( ElecEdge[DataUpper][side].dataR[dn] == OK )			data = DataUpper;
						else if( ElecEdge[DataLower][side].dataR[dn] == OK )	data = DataLower;
						else	continue;
						ElecEdge[elec][side].dataX[dn]	= ElecEdge[data][side].dataX[dn];
						ElecEdge[elec][side].dataY[dn]	= ElecEdge[data][side].dataY[dn];
						ElecEdge[elec][side].dataR[dn]	= ElecEdge[data][side].dataR[dn];
					}
					// --- Xmax
					side = PosXmax;
					for( dn=0; dn<700; dn++ ) {
						if( ElecEdge[DataUpper][side].dataR[dn] == OK )			data = DataUpper;
						else if( ElecEdge[DataLower][side].dataR[dn] == OK )	data = DataLower;
						else	continue;
						ElecEdge[elec][side].dataX[dn]	= ElecEdge[data][side].dataX[dn];
						ElecEdge[elec][side].dataY[dn]	= ElecEdge[data][side].dataY[dn];
						ElecEdge[elec][side].dataR[dn]	= ElecEdge[data][side].dataR[dn];
					}
				}
				// --- end
				return;
			}

		// Chip edge data set
			if(elec == DataLeft){
				//v2.11 valid_xs = Mpx.imin[Elec[elec].BlobNo]+elec_offset_outer;
				if((1==bIsSFR25)&&(0!=(Option & BACKINSP_MODE))){
					valid_xs = Mpx.imin[Elec[elec].BlobNo]+((Mpx.imax[Elec[elec].BlobNo]-Mpx.imin[Elec[elec].BlobNo])/2);
				}else{
					valid_xs = Mpx.imin[Elec[elec].BlobNo]+elec_offset_outer;
				}
				if(bIsMCR01)	valid_xe = ElecPosRough[elec][PosXmax];						// 2017.08.21 斜めブレークで端面の白が見えている場合があるのでマスクする
				else			valid_xe = Mpx.imax[Elec[elec].BlobNo]-elec_offset_inner;
				//v2.20 if((1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE))){
				if( ((1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE))) || ((1==bIsLTR18)&&(0!=(Option & BACKINSP_MODE))) ){
					//valid_ys = Mpx.jmin[Elec[elec].BlobNo]+((Mpx.jmax[Elec[elec].BlobNo]-Mpx.jmin[Elec[elec].BlobNo])/2);
					//valid_ye = Mpx.jmax[Elec[elec].BlobNo]-elec_offset_inner;
					valid_ys = Mpx.jmin[Elec[elec].BlobNo]+((Mpx.jmax[Elec[elec].BlobNo]-Mpx.jmin[Elec[elec].BlobNo])/2)-elec_offset_outer;
					valid_ye = Mpx.jmin[Elec[elec].BlobNo]+((Mpx.jmax[Elec[elec].BlobNo]-Mpx.jmin[Elec[elec].BlobNo])/2)+elec_offset_inner+5;
				}
			} else {
				if(bIsMCR01)	valid_xs = ElecPosRough[elec][PosXmin];						// 2017.08.21 斜めブレークで端面の白が見えている場合があるのでマスクする
				else			valid_xs = Mpx.imin[Elec[elec].BlobNo]+elec_offset_inner;
				//v2.11 valid_xe = Mpx.imax[Elec[elec].BlobNo]-elec_offset_outer;
				if((1==bIsSFR25)&&(0!=(Option & BACKINSP_MODE))){
					valid_xe = Mpx.imax[Elec[elec].BlobNo]-((Mpx.imax[Elec[elec].BlobNo]-Mpx.imin[Elec[elec].BlobNo])/2);
				}else{
					valid_xe = Mpx.imax[Elec[elec].BlobNo]-elec_offset_outer;
				}
				//v2.20 if((1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE))){
				if( ((1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE))) || ((1==bIsLTR18)&&(0!=(Option & BACKINSP_MODE))) ){
					//valid_ys = Mpx.jmin[Elec[elec].BlobNo]+elec_offset_inner;
					//valid_ye = Mpx.jmax[Elec[elec].BlobNo]-((Mpx.jmax[Elec[elec].BlobNo]-Mpx.jmin[Elec[elec].BlobNo])/2);
					valid_ys = Mpx.jmin[Elec[elec].BlobNo]+((Mpx.jmax[Elec[elec].BlobNo]-Mpx.jmin[Elec[elec].BlobNo])/2)-elec_offset_inner-5;
					valid_ye = Mpx.jmin[Elec[elec].BlobNo]+((Mpx.jmax[Elec[elec].BlobNo]-Mpx.jmin[Elec[elec].BlobNo])/2)+elec_offset_outer;
				}
			}
			for( dn=0; dn<Mpx.runsize; dn++ ) {
				if( Mpx.runnumb[dn] == Elec[elec].BlobNo ) {
					ry = Mpx.yaxis[dn];
					rx = Mpx.runstrt[dn];

					//v2.20 if((1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE))){
					if( ((1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE))) || ((1==bIsLTR18)&&(0!=(Option & BACKINSP_MODE))) ){
						// --- Xmin
						side = PosXmin;
						if(valid_ys <= ry && ry <= valid_ye){
							if(( ElecEdge[elec][side].dataX[ry] == 0x0ffff )||( ElecEdge[elec][side].dataX[ry] > Mpx.runstrt[dn] )) {
									ElecEdge[elec][side].dataX[ry] = Mpx.runstrt[dn];
									ElecEdge[elec][side].dataY[ry] = ry;
									ElecEdge[elec][side].dataR[ry] = OK;
							}
						}
						// --- Xmax
						side = PosXmax;
						if(valid_ys <= ry && ry <= valid_ye){
							if(( ElecEdge[elec][side].dataX[ry] == 0x0ffff )||( ElecEdge[elec][side].dataX[ry] < Mpx.runstrt[dn]+Mpx.runleng[dn] )) {
									ElecEdge[elec][side].dataX[ry] = Mpx.runstrt[dn]+Mpx.runleng[dn];
									ElecEdge[elec][side].dataY[ry] = ry;
									ElecEdge[elec][side].dataR[ry] = OK;
							}
						}
					}else{
						// --- Xmin
						side = PosXmin;
						if(( ElecEdge[elec][side].dataX[ry] == 0x0ffff )||( ElecEdge[elec][side].dataX[ry] > Mpx.runstrt[dn] )) {
								ElecEdge[elec][side].dataX[ry] = Mpx.runstrt[dn];
								ElecEdge[elec][side].dataY[ry] = ry;
								ElecEdge[elec][side].dataR[ry] = OK;
						}
						// --- Xmax
						side = PosXmax;
						if(( ElecEdge[elec][side].dataX[ry] == 0x0ffff )||( ElecEdge[elec][side].dataX[ry] < Mpx.runstrt[dn]+Mpx.runleng[dn] )) {
								ElecEdge[elec][side].dataX[ry] = Mpx.runstrt[dn]+Mpx.runleng[dn];
								ElecEdge[elec][side].dataY[ry] = ry;
								ElecEdge[elec][side].dataR[ry] = OK;
						}
					}
					// --- Ymin
					side = PosYmin;
					rx = Mpx.runstrt[dn];
					for( bn=0; bn<Mpx.runleng[dn]; bn++, rx++ ) {
						if(valid_xs <= rx && rx <= valid_xe){					// 2017.1.10 tanaka 電極端のダレているところをみない
							if(( ElecEdge[elec][side].dataY[rx] == 0x0ffff )||( ElecEdge[elec][side].dataY[rx] > Mpx.yaxis[dn] )) {
									ElecEdge[elec][side].dataY[rx] = Mpx.yaxis[dn];
									ElecEdge[elec][side].dataX[rx] = rx;
									ElecEdge[elec][side].dataR[rx] = OK;
							}
						}
					}
					// --- Ymax
					side = PosYmax;
					rx = Mpx.runstrt[dn];
					for( bn=0; bn<Mpx.runleng[dn]; bn++, rx++ ) {
						if(valid_xs <= rx && rx <= valid_xe){					// 2017.1.10 tanaka 電極端のダレているところをみない
							if(( ElecEdge[elec][side].dataY[rx] == 0x0ffff )||( ElecEdge[elec][side].dataY[rx] < Mpx.yaxis[dn] )) {
									ElecEdge[elec][side].dataY[rx] = Mpx.yaxis[dn];
									ElecEdge[elec][side].dataX[rx] = rx;
									ElecEdge[elec][side].dataR[rx] = OK;
							}
						}
					}
				}
			}
			// Electrode エッジ表示
				if( i_mode & INSPECT_STEP ) {
					R_DrawCls();
//					BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
					R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
					// Chip position
					// Position X
						side = PosXmin;
						for( ry=0; ry<FySize; ry++ ) {
							if( ElecEdge[elec][side].dataR[ry] != OK )	continue;
							R_DrawPset( DRAW_GREEN, ElecEdge[elec][side].dataX[ry], ElecEdge[elec][side].dataY[ry], DRAW_REVERSE );
						}
						// Comment disp
							sprintf( Comment, "Electrode search position Xmin" );
							DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					// Position X
						side = PosXmax;
						for( ry=0; ry<FySize; ry++ ) {
							if( ElecEdge[elec][side].dataR[ry] != OK )	continue;
							R_DrawPset( DRAW_GREEN, ElecEdge[elec][side].dataX[ry], ElecEdge[elec][side].dataY[ry], DRAW_REVERSE );
						}
						// Comment disp
							sprintf( Comment, "Electrode search position Xmax" );
							DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					// Position Y
						side = PosYmin;
						for( rx=0; rx<FxSize; rx++ ) {
							if( ElecEdge[elec][side].dataR[rx] != OK )	continue;
							R_DrawPset( DRAW_GREEN, ElecEdge[elec][side].dataX[rx], ElecEdge[elec][side].dataY[rx], DRAW_REVERSE );
						}
						// Comment disp
							sprintf( Comment, "Electrode search position Ymin" );
							DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					// Position Y
						side = PosYmax;
						for( rx=0; rx<FxSize; rx++ ) {
							if( ElecEdge[elec][side].dataR[rx] != OK )	continue;
							R_DrawPset( DRAW_GREEN, ElecEdge[elec][side].dataX[rx], ElecEdge[elec][side].dataY[rx], DRAW_REVERSE );
						}
						// Comment disp
							sprintf( Comment, "Electrode search position Ymax" );
							DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					// Bin clear
						R_DrawCls();
//						BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
				}

	}	// SetElecEdge( int elec ) end


int ElecLRInterpolation(int side)
{
    int         rd,src_n;
    int         xave,yave;
    int         xmin,xmax,ymin,ymax;
    int			i_mode	= R_CS_GetInspectMode();
    R_DPOINT    pLeft,pRight;

    // 左右の平均座標を繋いで傾きを計算する
    // ESR25など、端子のX寸が小さいチップに対する直線式を計算する
    // 左側の平均ポイント
    xave = 0;
    yave = 0;
    src_n = 0;
    for( rd=1; rd<700; rd++ ) {
        if( ElecEdge[DataLeft][side].dataR[rd-1] != OK &&  ElecEdge[DataLeft][side].dataR[rd  ] != OK )	continue;
        if(abs(ElecEdge[DataLeft][side].dataY[rd-1] - ElecEdge[DataLeft][side].dataY[rd  ] ) > 8)  continue;
        xave += ElecEdge[DataLeft][side].dataX[rd];
        yave += ElecEdge[DataLeft][side].dataY[rd];
        src_n++;
    }
    if(src_n != 0) {
        pLeft.x = (double)xave/(double)src_n;
        pLeft.y = (double)yave/(double)src_n;
	} else return (ERROR);

    // 右側の平均ポイント
    xave = 0;
    yave = 0;
    src_n = 0;
    for( rd=0; rd<700; rd++ ) {
        if( ElecEdge[DataRight][side].dataR[rd-1] != OK &&  ElecEdge[DataRight][side].dataR[rd  ] != OK )	continue;
        if(abs(ElecEdge[DataRight][side].dataY[rd-1] - ElecEdge[DataRight][side].dataY[rd  ] ) > 8)  continue;
        xave += ElecEdge[DataRight][side].dataX[rd];
        yave += ElecEdge[DataRight][side].dataY[rd];
        src_n++;
    }
    if(src_n != 0) {
        pRight.x = (double)xave/(double)src_n;
        pRight.y = (double)yave/(double)src_n;
    } else return (ERROR);

    ElecEdge[DataLR][side].MeanX = pLeft.x;
    ElecEdge[DataLR][side].MeanY = pLeft.y;
    if( abs(pRight.x - pLeft.x) > 1e-10 ){
        ElecEdge[DataLR][side].Rev   = (pRight.y - pLeft.y)/(pRight.x - pLeft.x);
    } else {
        ElecEdge[DataLR][side].Rev   = 1.0;
    }

    if( i_mode & INSPECT_STEP ) {
        for( rd=0; rd<700; rd++ ){
            if( ElecEdge[DataLR][side].dataR[rd] != OK )	continue;
            R_DrawPset( DRAW_RED, ElecEdge[DataLR][side].dataX[rd], ElecEdge[DataLR][side].dataY[rd], DRAW_NORMAL );
        }

        if((side==PosYmin)||(side==PosYmax)) {
            xmin = 5;
            xmax = FxSize-5;
            ymin = ElecEdge[DataLR][side].MeanY-ElecEdge[DataLR][side].Rev*(ElecEdge[DataLR][side].MeanX-xmin);
            ymax = ElecEdge[DataLR][side].MeanY-ElecEdge[DataLR][side].Rev*(ElecEdge[DataLR][side].MeanX-xmax);
        } else {
            ymin = 5;
            ymax = FySize-5;
            xmin = ElecEdge[DataLR][side].MeanX-ElecEdge[DataLR][side].Rev*(ElecEdge[DataLR][side].MeanY-ymin);
            xmax = ElecEdge[DataLR][side].MeanX-ElecEdge[DataLR][side].Rev*(ElecEdge[DataLR][side].MeanY-ymax);
        }
        R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, DASHED_LINE );

        // Regression data
		sprintf( Comment1, "ElecLRInterpolation Regr. Elec:%2d side:%2d Regr1\n MeanX:%g MeanY:%g Rev:%g ",
                            DataLR,
                            side,
                            ElecEdge[DataLR][side].MeanX,
                            ElecEdge[DataLR][side].MeanY,
                            ElecEdge[DataLR][side].Rev );
        // --- 座標点群の直線近似
        DrawMessage( Comment1, DRAW_GREEN, 2, 2 );
    }
    return OK;
}

//------------------------------------------------
//		Check Electrode edge
//		回帰解析	a regression analysis.
//		回帰係数	a [the] regression coefficient.
//		回帰曲線	a regression curve.
//------------------------------------------------

int		CheckElecEdge( int elec )
	{
//		int						i_mode	= R_CS_GetInspectMode();
		int						side, dn;
		int						rd;
		double					rev_sum;			// 回帰係数
		double					rev_av;				// 回帰係数
		double					rev_min;			// 回帰係数
		double					rev_max;			// 回帰係数


		// 回帰解析( regression analysis. )
			// data clear
//			for( side=0; side<4; side++ ) {
//				for( dn=0; dn<700; dn++ ) {					2017.1.10 tanaka 使われていないのコメント化
//					ElecFreqRate[elec][side][dn].dx	= 0;
//					ElecFreqRate[elec][side][dn].dy	= 0;
//					ElecFreqRate[elec][side][dn].dn	= 0;
//				}
//				ElecFreqMax[elec][side].dx	= 0;
//				ElecFreqMax[elec][side].dy	= 0;
//				ElecFreqMax[elec][side].dn	= 0;
//			}

//			if( i_mode & INSPECT_STEP ) {
//				if( elec == DataLR )	R_DrawCls();
//			}

		// 回帰解析( regression analysis. )
			switch( elec ) {
				case DataLR:
					for( side=0; side<4; side++ ) {
						switch( side ) {
							case 0:
								//v2.11	if(! bIsLTR50  ){		// normal
								//v2.20 if( (! bIsLTR50)||((1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE))) ){		// normal
								if( (! bIsLTR50)||((1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE)))||((1==bIsLTR18)&&(0!=(Option & BACKINSP_MODE))) ){		// normal
									if( Regression( elec, side ) != OK )	return ERROR;
								} else {
									// Ap Searchの値を使う
									for(rd=0;rd<FxSize;rd++){
										ElecEdge[elec][side].dataR[rd]=0xffff;
										ElecEdge[elec][side].dataX[rd]=0xffff;
										ElecEdge[elec][side].dataY[rd]=0xffff;
									}
									ElecDataSet(elec, side );
									if( Regression( elec, side ) != OK )	return ERROR;
								}
								rev_sum	= 0.0-ElecEdge[elec][side].Rev;
								rev_min	= 0.0-ElecEdge[elec][side].Rev;
								rev_max	= 0.0-ElecEdge[elec][side].Rev;
								break;
							case 1:
                            case 3:
								//v2.11 if(bIsMCR03 || bIsMCR01 || bIsLTR50  ){		// MCR03
								//2.20 if( (bIsMCR03 || bIsMCR01 || bIsLTR50)||((1==bIsSFR25)&&(0!=(Option & BACKINSP_MODE))) ){
								if( (bIsMCR03 || bIsMCR01 || bIsLTR50 || bIsLTR18)||((1==bIsSFR25)&&(0!=(Option & BACKINSP_MODE))) ){
                                    if( Regression( elec, side ) != OK )	return ERROR;
								} else {							// ESR25,MCR10,SFR25
								//    if( ElecLRInterpolation(side) != OK )   return ERROR;
									// Ap Searchの値を使う
									for(rd=0;rd<FxSize;rd++){
										ElecEdge[elec][side].dataR[rd]=0xffff;
										ElecEdge[elec][side].dataX[rd]=0xffff;
										ElecEdge[elec][side].dataY[rd]=0xffff;
									}
									ElecDataSet(elec, side );
									if( Regression( elec, side ) != OK )	return ERROR;
								}
                                rev_sum	+= ElecEdge[elec][side].Rev;
								rev_min	= min(ElecEdge[elec][side].Rev,rev_min);
								rev_max	= max(ElecEdge[elec][side].Rev,rev_max);
								break;
							case 2:
								//v2.11	if(! bIsLTR50  ){		// normal
								//v2.20 if( (! bIsLTR50)||((1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE))) ){		// normal
								if( (! bIsLTR50)||((1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE)))||(! bIsLTR18)||((1==bIsLTR18)&&(0!=(Option & BACKINSP_MODE))) ){		// normal
									if( Regression( elec, side ) != OK )	return ERROR;
								} else {
									// Ap Searchの値を使う
									for(rd=0;rd<FxSize;rd++){
										ElecEdge[elec][side].dataR[rd]=0xffff;
										ElecEdge[elec][side].dataX[rd]=0xffff;
										ElecEdge[elec][side].dataY[rd]=0xffff;
									}
									ElecDataSet(elec, side );
									if( Regression( elec, side ) != OK )	return ERROR;
								}
								rev_sum	+= (0.0-ElecEdge[elec][side].Rev);
								rev_min	= min((0.0-ElecEdge[elec][side].Rev),rev_min);
								rev_max	= max((0.0-ElecEdge[elec][side].Rev),rev_max);
								break;
						}
					}
					rev_av = (rev_sum - rev_min - rev_max)/2;
					for( side=0; side<4; side++ ) {
						switch( side ) {
							case 0:
							case 2:
								ElecEdge[elec][side].Rev = 0.0-rev_av;
								break;
							case 1:
							case 3:
								ElecEdge[elec][side].Rev = rev_av;
								break;
						}
					}
					break;
				case DataLeft:			// DataUpper
					//v2.20 if(!bIsLTR50){		//normal
					if( (!bIsLTR50) && (!bIsLTR18) ){		//normal
						side = PosXmax;
						if( Regression( elec, side ) != OK )		return ERROR;
						if(bIsMCR18){	// 被り込み線の抽出
							side = 5;
							if( Regression( elec, side ) != OK )		return ERROR;
						}
					} else {			// LTR
						side = PosYmax;
						if( Regression( elec, side ) != OK )		return ERROR;
					}
					break;
				case DataRight:			// DataLower
					//v2.20 if(!bIsLTR50){		//normal
					if( (!bIsLTR50) && (!bIsLTR18) ){		//normal
						side = PosXmin;
						if( Regression( elec, side ) != OK )		return ERROR;
						if(bIsMCR18){	// 被り込み線の抽出
							side = 4;
							if( Regression( elec, side ) != OK )		return ERROR;
						}
					} else {			//LTR
						side = PosYmin;
						if( Regression( elec, side ) != OK )		return ERROR;
					}
					break;
				default:
					return ERROR;
					break;
			}

		// Electrode corner pos. set
			ElecCornerPosSet1st( elec );
			if(bIsMCR18){
				ElecC12CornerPosSet1st( elec );
				ElecC1CornerPosSet1st( elec );
			}
		return OK;

	}	// CheckElecEdge( int elec ) end


//------------------------------------------------
//		回帰解析( regression analysis. )
//------------------------------------------------

int		Regression( int elec, int side )
	{
		int						i_mode	= R_CS_GetInspectMode();
		int						dn, rd;
		int						src_n, dst_n;
		int						xmin, xmax;
		int						ymin, ymax;
		float					sum, sumx, sumy;
		float					ex, ey;
		int                     angle, n_Left, n_Right;   //GEE V1.02A
		int                     nm;                       //GEE V1.02A
		int						st,en,av;
		int						valid_xs[2];
		int						valid_xe[2];
		// Initial
			RobustCond.deg_step	= RobustDegStep;
			st=0;	en=0;
			src_n = 0;
			switch(side){
				case 4:
					for( rd=0; rd<700; rd++ ) {
						if( ElecEdge[elec][0].dataR[rd] != OK )	continue;
						Src_point[src_n].x = ElecEdge[elec][0].dataX[rd];
						Src_point[src_n].y = ElecEdge[elec][0].dataY[rd];
						src_n++;
					}
					break;
				case 5:
					for( rd=0; rd<700; rd++ ) {
						if( ElecEdge[elec][2].dataR[rd] != OK )	continue;
						Src_point[src_n].x = ElecEdge[elec][2].dataX[rd];
						Src_point[src_n].y = ElecEdge[elec][2].dataY[rd];
						src_n++;
					}
					break;
				default:
					for( rd=0; rd<700; rd++ ) {
						if( ElecEdge[elec][side].dataR[rd] != OK )	continue;
						Src_point[src_n].x = ElecEdge[elec][side].dataX[rd];
						Src_point[src_n].y = ElecEdge[elec][side].dataY[rd];
						src_n++;
					}
				}

			//v2.11 if(bIsMCR03 || bIsMCR10 || bIsMCR18 || bIsMCR01){// C1の凸部分でエッジを計算してみる 20170226 tanaka
			if( (bIsMCR03||bIsMCR10||bIsMCR18||bIsMCR01) || ((1==bIsSFR25)&&(0!=(Option & BACKINSP_MODE))) ){// C1の凸部分でエッジを計算してみる 20170226 tanaka
				if( (elec==DataLeft && side==PosXmax)
				||	(elec==DataRight && side==PosXmin)){
					if(src_n >=132 ){
						st=src_n/2-66;
						src_n-=st*2;
					}
				}
				if( (elec==DataLeft && side==5)
				||	(elec==DataRight && side==4)){
					if(src_n >=60 ){
						st=20;
						for( rd= 0; rd < st;rd++){
							Src_point[rd+st].x = Src_point[src_n-st+rd].x;
							Src_point[rd+st].y = Src_point[src_n-st+rd].y;
						}
						src_n=st*2;
						st=0;
					}
				}
			}
			angle = nm= 0;
			for(;;){                                       //GEE V1.02A
			// 座標点群の直線成分の検出
				if((side==PosYmin)||(side==PosYmax)||(side==6)||(side==7)) {
					RobustCond.deg_min = ChipAtan+angle-RobustDegWidth;
					RobustCond.deg_max = ChipAtan+angle+RobustDegWidth;
				} else {
					RobustCond.deg_min = ChipAtan+900-RobustDegWidth;
					RobustCond.deg_max = ChipAtan+900+RobustDegWidth;
				}
				if( (elec==DataLeft && side==5)
				||	(elec==DataRight && side==4)){
					for( rd= 0; rd < src_n; rd++){
						Dst_point[rd].x=Src_point[rd].x;
						Dst_point[rd].y=Src_point[rd].y;
					}
					dst_n=src_n;
				} else {
					R_robust_sampling_line( src_n, &Src_point[st], &dst_n, Dst_point, RobustCond, 0 );	// 最頻値相当の成分のみ抽出
				}
				if( dst_n == 0 )	return ERROR;

				//v2.20 if(!bIsLTR50){	// normal
				if( (!bIsLTR50) && (!bIsLTR18) ){	// normal
					if((side==PosXmin)||(side==PosXmax)||(side==4)||(side==5)||(side==6)||(side==7)) break;

					n_Left = n_Right = 0;
					for( dn=0; dn<dst_n; dn++ ){
						 if( Dst_point[dn].x < Center_C ) n_Left ++;
						 if( Dst_point[dn].x > Center_C ) n_Right++;
						 if(( n_Left > 5 )&&( n_Right > 5 )) break;
					}
					if(( n_Left > 5 )&&( n_Right > 5 )) break;
				} else {		// LTR
					break;
					/*
					if((side==PosYmin)||(side==PosYmax)||(side==4)||(side==5)||(side==6)||(side==7)) break;

					n_Left = n_Right = 0;
					for( dn=0; dn<dst_n; dn++ ){
						 if( Dst_point[dn].y < Center_C ) n_Left ++;
						 if( Dst_point[dn].y > Center_C ) n_Right++;
						 //if(( n_Left > 3 )&&( n_Right > 3 )) break;
						 if(( n_Left >=1 )&&( n_Right >= 1 )) break;
					}
					if(( n_Left >= 1 )&&( n_Right >= 1 )) break;
					*/
				}
				angle += 5;
				nm++; if(nm == 50) break;
			}

			//v2.20 if(!bIsLTR50){	// normal
			if( (!bIsLTR50) && (!bIsLTR18) ){	// normal
				if((nm==50)&&(((side==PosYmin)||(side==PosYmax)))){
					RobustCond.deg_min = ChipAtan-RobustDegWidth;
					RobustCond.deg_max = ChipAtan+RobustDegWidth;
					R_robust_sampling_line( src_n, Src_point, &dst_n, Dst_point, RobustCond, 0 );	// 最頻値相当の成分のみ抽出
					if( dst_n == 0 )	return ERROR;
				}
			} else {		// LTR
				if((nm==50)&&(((side==PosXmin)||(side==PosXmax)))){
					RobustCond.deg_min = ChipAtan-RobustDegWidth;
					RobustCond.deg_max = ChipAtan+RobustDegWidth;
					R_robust_sampling_line( src_n, Src_point, &dst_n, Dst_point, RobustCond, 0 );	// 最頻値相当の成分のみ抽出
					if( dst_n == 0 )	return ERROR;
				}
			}

		// Regression
			sumx = sumy = 0;
			for( dn=0; dn<dst_n; dn++ ) {
				sumx += Dst_point[dn].x;
				sumy += Dst_point[dn].y;
			}
			ex = sumx/dst_n;		//平均Ｘ
			ey = sumy/dst_n;		//平均Ｙ
			sum = sumx = sumy = 0;
			for( dn=0; dn<dst_n; dn++ ) {
				//v2.20 if(!bIsLTR50){	// normal
				if( (!bIsLTR50) && (!bIsLTR18) ){	// normal
					if((side==PosYmin)||(side==PosYmax)||(side==6)||(side==7)) {
						sumx += pow((Dst_point[dn].x-ex), 2 );
					} else {
						sumy += pow((Dst_point[dn].y-ey), 2 );
					}
					sum += ( Dst_point[dn].x-ex )*( Dst_point[dn].y-ey);
				} else {		// LTR
					if((side==PosXmin)||(side==PosXmax)||(side==6)||(side==7)) {
						sumy += pow((Dst_point[dn].y-ey), 2 );
					} else {
						sumx += pow((Dst_point[dn].x-ex), 2 );
					}
					sum += ( Dst_point[dn].y-ey )*( Dst_point[dn].x-ex);
				}
			}
			//v2.20 if(!bIsLTR50){	// normal
			if( (!bIsLTR50) && (!bIsLTR18) ){	// normal
				if((side==PosYmin)||(side==PosYmax)||(side==6)||(side==7)) {
					if( sumx == 0 ) {
						ElecEdge[elec][side].Result = ERROR;
						return ERROR;
					}
				} else {
					if( sumy == 0 ) {
						ElecEdge[elec][side].Result = ERROR;
						return ERROR;
					}
				}
			} else {		// LTR
				if((side==PosXmin)||(side==PosXmax)||(side==6)||(side==7)) {
					if( sumy == 0 ) {
						ElecEdge[elec][side].Result = ERROR;
						return ERROR;
					}
				} else {
					if( sumx == 0 ) {
						ElecEdge[elec][side].Result = ERROR;
						return ERROR;
					}
				}
			}
			//v2.20 if(!bIsLTR50){	// normal
			if( (!bIsLTR50) && (!bIsLTR18) ){	// normal
				if(elec==DataLR){
					switch( side ){
						case PosXmin:
							if( ex > ElecPosRough[DataLR][PosXmin] )	ex = ElecPosRough[DataLR][PosXmin];
							break;
						case PosXmax:
							if( ex < ElecPosRough[DataLR][PosXmax] )	ex = ElecPosRough[DataLR][PosXmax];
							break;
					}
				}
			} else {
				if(elec==DataUL){
					switch( side ){
						case PosYmin:
							if( ey > ElecPosRough[DataUL][PosYmin] )	ey = ElecPosRough[DataUL][PosYmin];
							break;
						case PosYmax:
							if( ey < ElecPosRough[DataUL][PosYmax] )	ey = ElecPosRough[DataUL][PosYmax];
							break;
					}
				}
			}
			ElecEdge[elec][side].MeanX	= ex;
			ElecEdge[elec][side].MeanY	= ey;

			//v2.20 if(!bIsLTR50){	// normal
			if( (!bIsLTR50) && (!bIsLTR18) ){	// normal
				if((side==PosYmin)||(side==PosYmax)) {
					if( sumx == 0 )	ElecEdge[elec][side].Rev	= 0.0;
					else			ElecEdge[elec][side].Rev	= sum/sumx;
				} else {
					if( sumy == 0 )	ElecEdge[elec][side].Rev	= 0.0;
					else			ElecEdge[elec][side].Rev	= sum/sumy;
				}
			} else {		// LTR
				if((side==PosXmin)||(side==PosXmax)) {
					if( sumy == 0 )	ElecEdge[elec][side].Rev	= 0.0;
					else			ElecEdge[elec][side].Rev	= sum/sumy;
				} else {
					if( sumx == 0 )	ElecEdge[elec][side].Rev	= 0.0;
					else			ElecEdge[elec][side].Rev	= sum/sumx;
				}
			}
			ElecEdge[elec][side].Result	= OK;
			// View
				if( i_mode & INSPECT_STEP ) {
					for( dn=0; dn<dst_n; dn++ )	R_DrawPset( DRAW_RED, Dst_point[dn].x, Dst_point[dn].y, DRAW_NORMAL );

					sprintf( Comment, " Before src_n:%d \n After  dst_n:%d ", src_n, dst_n );		//直線成分抽出前後の座標点数
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );


					//v2.20 if(!bIsLTR50){	// normal
					if( (!bIsLTR50) && (!bIsLTR18) ){	// normal
						if((side==PosYmin)||(side==PosYmax)||(side==6)||(side==7)) {
							xmin = 5;//11911 try to change this value
							xmax = FxSize-5;
							ymin = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
							ymax = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
						} else {
							ymin = 5;
							ymax = FySize-5;
							xmin = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
							xmax = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
						}
					} else {
						if((side==PosXmin)||(side==PosXmax)||(side==6)||(side==7)) {
							ymin = 5;
							ymax = FySize-5;
							xmin = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
							xmax = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
						} else {
							xmin = 5;
							xmax = FxSize-5;
							ymin = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
							ymax = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
						}
					}
					R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, DASHED_LINE );

					// Regression data
					sprintf( Comment1, "Regression Regr. Elec:%2d side:%2d Regr0\n MeanX:%g MeanY:%g Rev:%g ",
										elec,
										side,
										ElecEdge[elec][side].MeanX,
										ElecEdge[elec][side].MeanY,
										ElecEdge[elec][side].Rev );
					// --- 座標点群の直線近似
					R_robust_get_line( src_n, Src_point, RobustCond, &ApproxLine );							// 直線近似
					sprintf( Comment2, " a=%d  b=%d  c=%d ", ApproxLine.a, ApproxLine.b, ApproxLine.c );		// 直線の係数表示
					// --- comment
					sprintf( Comment, "%s \n %s", Comment1, Comment2 );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				}

		// result
			return OK;

	}	// Regression( int elec, int side ) end


//================================================
//		Electrode corner pos. set
//================================================

void	ElecCornerPosSet1st( int elec )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						dn, sideX, sideY;
		int						rx, ry;
		int						iy, rl, rlmin;

		// Initial
			for( dn=0; dn<4; dn++ ) {
				Elec[elec].CornerPosX[dn] = 0x0ffff;
				Elec[elec].CornerPosY[dn] = 0x0ffff;
				if(bIsESR25 || bIsMCR10 || bIsMCR18  || bIsSFR25){
					Overcoat.CornerPosX[dn]=0x0ffff;
					Overcoat.CornerPosY[dn]=0x0ffff;
				}
			}

		// Corner Xmin Ymin
			sideX	= PosXmin;
			sideY	= PosYmin;
			rlmin	= FxSize;
			for( iy=0; iy<FySize; iy++ ) {
				rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
				ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-rx);
				if( iy == ry ) {
					Elec[elec].CornerPosX[0] = rx;
					Elec[elec].CornerPosY[0] = ry;
					break;
				} else {
					rl = abs( iy - ry );
					if( rl < rlmin ) {
						rlmin = rl;
						Elec[elec].CornerPosX[0] = rx;
						Elec[elec].CornerPosY[0] = ry;
					}
				}
			}
		// Corner Xmin Ymax
			sideX	= PosXmin;
			sideY	= PosYmax;
			rlmin	= FxSize;
			for( iy=0; iy<FySize; iy++ ) {
				rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
				ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-rx);
				if( iy == ry ) {
					Elec[elec].CornerPosX[1] = rx;
					Elec[elec].CornerPosY[1] = ry;
					break;
				} else {
					rl = abs( iy - ry );
					if( rl < rlmin ) {
						rlmin = rl;
						Elec[elec].CornerPosX[1] = rx;
						Elec[elec].CornerPosY[1] = ry;
					}
				}
			}
		// Corner Xmax Ymin
			sideX	= PosXmax;
			sideY	= PosYmin;
			rlmin	= FxSize;
			for( iy=0; iy<FySize; iy++ ) {
				rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
				ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-rx);
				if( iy == ry ) {
					Elec[elec].CornerPosX[2] = rx;
					Elec[elec].CornerPosY[2] = ry;
					break;
				} else {
					rl = abs( iy - ry );
					if( rl < rlmin ) {
						rlmin = rl;
						Elec[elec].CornerPosX[2] = rx;
						Elec[elec].CornerPosY[2] = ry;
					}
				}
			}
		// Corner Xmax Ymax
			sideX	= PosXmax;
			sideY	= PosYmax;
			rlmin	= FxSize;

			for( iy=0; iy<FySize; iy++ ) {
				rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
				ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-rx);
				if( iy == ry ) {
					Elec[elec].CornerPosX[3] = rx;
					Elec[elec].CornerPosY[3] = ry;
					break;
				} else {
					rl = abs( iy - ry );
					if( rl < rlmin ) {
						rlmin = rl;
						Elec[elec].CornerPosX[3] = rx;
						Elec[elec].CornerPosY[3] = ry;
					}
				}
			}

		if(bIsESR25 || bIsMCR10 || bIsMCR18 || bIsSFR25){
		// Corner Xmin Ymin
			rlmin	= FxSize;
			for( iy=0; iy<FySize; iy++ ) {
				rx = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-iy);
				ry = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-rx);
				if( iy == ry ) {
					Overcoat.CornerPosX[PosLup] = rx;
					Overcoat.CornerPosY[PosLup] = ry;
					break;
				} else {
					rl = abs( iy - ry );
					if( rl < rlmin ) {
						rlmin = rl;
						Overcoat.CornerPosX[PosLup] = rx;
						Overcoat.CornerPosY[PosLup] = ry;
					}
				}
			}
		// Corner Xmin Ymax
			rlmin	= FxSize;
			for( iy=0; iy<FySize; iy++ ) {
				rx = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-iy);
				ry = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-rx);
				if( iy == ry ) {
					Overcoat.CornerPosX[PosLlo] = rx;
					Overcoat.CornerPosY[PosLlo] = ry;
					break;
				} else {
					rl = abs( iy - ry );
					if( rl < rlmin ) {
						rlmin = rl;
						Overcoat.CornerPosX[PosLlo] = rx;
						Overcoat.CornerPosY[PosLlo] = ry;
					}
				}
			}
		// Corner Xmax Ymin
			rlmin	= FxSize;
			for( iy=0; iy<FySize; iy++ ) {
				rx = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-iy);
				ry = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-rx);
				if( iy == ry ) {
					Overcoat.CornerPosX[PosRup] = rx;
					Overcoat.CornerPosY[PosRup] = ry;
					break;
				} else {
					rl = abs( iy - ry );
					if( rl < rlmin ) {
						rlmin = rl;
						Overcoat.CornerPosX[PosRup] = rx;
						Overcoat.CornerPosY[PosRup] = ry;
					}
				}
			}
		// Corner Xmax Ymax
			rlmin	= FxSize;
			for( iy=0; iy<FySize; iy++ ) {
				rx = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-iy);
				ry = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-rx);
				if( iy == ry ) {
					Overcoat.CornerPosX[PosRlo] = rx;
					Overcoat.CornerPosY[PosRlo] = ry;
					break;
				} else {
					rl = abs( iy - ry );
					if( rl < rlmin ) {
						rlmin = rl;
						Overcoat.CornerPosX[PosRlo] = rx;
						Overcoat.CornerPosY[PosRlo] = ry;
					}
				}
			}
		}

		// Draw corner pos.
			if( i_mode & INSPECT_STEP ) {
				R_DrawCls();
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				// corner pos.
				for( dn=0; dn<4; dn++ ) {
					rx = Elec[elec].CornerPosX[dn];
					ry = Elec[elec].CornerPosY[dn];
					if(( rx==0x0ffff )||( ry==0x0ffff ))	continue;
					R_DrawDispCross( DRAW_RED, rx, ry, 5, DRAW_NORMAL );
					if(bIsESR25 || bIsMCR10 || bIsMCR18 || bIsSFR25){
						rx = Overcoat.CornerPosX[dn];
						ry = Overcoat.CornerPosY[dn];
						if(( rx==0x0ffff )||( ry==0x0ffff ))	continue;
						R_DrawDispCross( DRAW_RED, rx, ry, 5, DRAW_NORMAL );
					}
				}
				DrawMessage( "ElecCornerPosSet1st Electrode corner pos.", DRAW_GREEN, 2, 2 );
			}

	}	// ElecCornerPosSet1st( int elec ) end

//================================================
//		Electrode C12 corner pos. set
//================================================

void	ElecC12CornerPosSet1st( int elec )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						dn, sideX, sideY;
		int						rx, ry;
		int						iy, rl, rlmin;

		if(elec != DataLeft && elec != DataRight)	return;
		// Initial
			for( dn=0; dn<4; dn++ ) {
				ElecC12[elec].CornerPosX[dn] = 0x0ffff;
				ElecC12[elec].CornerPosY[dn] = 0x0ffff;
			}

		switch(elec){
			case DataLeft:
				ElecC12[DataLeft].CornerPosX[0]  = Elec[DataLR].CornerPosX[0];
				ElecC12[DataLeft].CornerPosY[0]  = Elec[DataLR].CornerPosY[0];
				ElecC12[DataLeft].CornerPosX[1]  = Elec[DataLR].CornerPosX[1];
				ElecC12[DataLeft].CornerPosY[1]  = Elec[DataLR].CornerPosY[1];
				// Corner Xmax Ymin
					sideY	= PosYmin;
					rlmin	= FxSize;
					for( iy=0; iy<FySize; iy++ ) {
						rx = ElecEdge[DataLeft][5    ].MeanX-ElecEdge[DataLeft][5    ].Rev*(ElecEdge[DataLeft][5    ].MeanY-iy);
						ry = ElecEdge[DataLR  ][sideY].MeanY-ElecEdge[DataLR  ][sideY].Rev*(ElecEdge[DataLR  ][sideY].MeanX-rx);
						if( iy == ry ) {
							ElecC12[DataLeft].CornerPosX[2] = rx;
							ElecC12[DataLeft].CornerPosY[2] = ry;
							break;
						} else {
							rl = abs( iy - ry );
							if( rl < rlmin ) {
								rlmin = rl;
								ElecC12[DataLeft].CornerPosX[2] = rx;
								ElecC12[DataLeft].CornerPosY[2] = ry;
							}
						}
					}
				// Corner Xmax Ymax
					sideX	= PosXmax;
					sideY	= PosYmax;
					rlmin	= FxSize;
					for( iy=0; iy<FySize; iy++ ) {
						rx = ElecEdge[DataLeft][5    ].MeanX-ElecEdge[DataLeft][5    ].Rev*(ElecEdge[DataLeft][5    ].MeanY-iy);
						ry = ElecEdge[DataLR  ][sideY].MeanY-ElecEdge[DataLR  ][sideY].Rev*(ElecEdge[DataLR  ][sideY].MeanX-rx);
						if( iy == ry ) {
							ElecC12[DataLeft].CornerPosX[3] = rx;
							ElecC12[DataLeft].CornerPosY[3] = ry;
							break;
						} else {
							rl = abs( iy - ry );
							if( rl < rlmin ) {
								rlmin = rl;
								ElecC12[DataLeft].CornerPosX[3] = rx;
								ElecC12[DataLeft].CornerPosY[3] = ry;
							}
						}
					}

				break;
			case DataRight:
				ElecC12[DataRight].CornerPosX[2] = Elec[DataLR].CornerPosX[2];
				ElecC12[DataRight].CornerPosY[2] = Elec[DataLR].CornerPosY[2];
				ElecC12[DataRight].CornerPosX[3] = Elec[DataLR].CornerPosX[3];
				ElecC12[DataRight].CornerPosY[3] = Elec[DataLR].CornerPosY[3];
				// Corner Xmin Ymin
					sideX	= PosXmin;
					sideY	= PosYmin;
					rlmin	= FxSize;
					for( iy=0; iy<FySize; iy++ ) {
						rx = ElecEdge[DataRight][4    ].MeanX-ElecEdge[DataRight][4    ].Rev*(ElecEdge[DataRight][4    ].MeanY-iy);
						ry = ElecEdge[DataLR   ][sideY].MeanY-ElecEdge[DataLR   ][sideY].Rev*(ElecEdge[DataLR   ][sideY].MeanX-rx);
						if( iy == ry ) {
							ElecC12[DataRight].CornerPosX[0] = rx;
							ElecC12[DataRight].CornerPosY[0] = ry;
							break;
						} else {
							rl = abs( iy - ry );
							if( rl < rlmin ) {
								rlmin = rl;
								ElecC12[DataRight].CornerPosX[0] = rx;
								ElecC12[DataRight].CornerPosY[0] = ry;
							}
						}
					}
				// Corner Xmin Ymax
					sideX	= PosXmin;
					sideY	= PosYmax;
					rlmin	= FxSize;
					for( iy=0; iy<FySize; iy++ ) {
						rx = ElecEdge[DataRight][4    ].MeanX-ElecEdge[DataRight][4    ].Rev*(ElecEdge[DataRight][4    ].MeanY-iy);
						ry = ElecEdge[DataLR   ][sideY].MeanY-ElecEdge[DataLR   ][sideY].Rev*(ElecEdge[DataLR   ][sideY].MeanX-rx);
						if( iy == ry ) {
							ElecC12[DataRight].CornerPosX[1] = rx;
							ElecC12[DataRight].CornerPosY[1] = ry;
							break;
						} else {
							rl = abs( iy - ry );
							if( rl < rlmin ) {
								rlmin = rl;
								ElecC12[DataRight].CornerPosX[1] = rx;
								ElecC12[DataRight].CornerPosY[1] = ry;
							}
						}
					}
				break;
		}

		// Draw corner pos.
			if( i_mode & INSPECT_STEP ) {
				R_DrawCls();
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				// corner pos.
				for( dn=0; dn<4; dn++ ) {
					rx = ElecC12[elec].CornerPosX[dn];
					ry = ElecC12[elec].CornerPosY[dn];
					if(( rx==0x0ffff )||( ry==0x0ffff ))	continue;
					R_DrawDispCross( DRAW_RED, rx, ry, 5, DRAW_NORMAL );
				}
				DrawMessage( "Electrode C12 corner pos.", DRAW_GREEN, 2, 2 );
			}

	}	// ElecCornerPosSet1st( int elec ) end

//================================================
//		Electrode C1 corner pos. set
//================================================

void	ElecC1CornerPosSet1st( int elec )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						dn,ix,iy,rx,ry,rl,side;
		int						sideX,sideY,rlmin;

		if(elec != DataLeft && elec != DataRight)	return;
		// Initial
			for( dn=0; dn<4; dn++ ) {
				ElecC1[elec].CornerPosX[dn] = 0x0ffff;
				ElecC1[elec].CornerPosY[dn] = 0x0ffff;
			}

		for(side=6;side<=7;side++){
			for(ix=0;ix<700;ix++){
				ElecEdge[elec][side].dataR[ix]=0x0ffff;
				ElecEdge[elec][side].dataR[ix]=0x0ffff;
				ElecEdge[elec][side].dataR[ix]=0x0ffff;
			}
		}

		switch(elec){
			case DataLeft:
				if(ElecC12[elec].CornerPosX[2] == 0xffff || ElecC12[elec].CornerPosX[3] == 0xffff)	return;
				// 左 C1電極上
				side = 6;
				for(ix=0;ix<700;ix++){
					if(ElecEdge[elec][PosYmin].dataR[ix]!=OK)	continue;
					if(ElecEdge[elec][PosYmin].dataX[ix]>ElecC12[elec].CornerPosX[2]){
						ElecEdge[elec][side].dataR[ix]=ElecEdge[elec][PosYmin].dataR[ix];
						ElecEdge[elec][side].dataX[ix]=ElecEdge[elec][PosYmin].dataX[ix];
						ElecEdge[elec][side].dataY[ix]=ElecEdge[elec][PosYmin].dataY[ix];
					}
				}
				if( Regression( elec, side ) != OK )		return; // ERROR;

				// 左 C1電極下
				side = 7;
				for(ix=0;ix<700;ix++){
					if(ElecEdge[elec][PosYmax].dataR[ix]!=OK)	continue;
					if(ElecEdge[elec][PosYmax].dataX[ix]>ElecC12[elec].CornerPosX[3]){
						ElecEdge[elec][side].dataR[ix]=ElecEdge[elec][PosYmax].dataR[ix];
						ElecEdge[elec][side].dataX[ix]=ElecEdge[elec][PosYmax].dataX[ix];
						ElecEdge[elec][side].dataY[ix]=ElecEdge[elec][PosYmax].dataY[ix];
					}
				}
				if( Regression( elec, side ) != OK )		return; // ERROR;

				// Corner Xmin Ymin
					sideX	= 5;
					sideY	= 6;
					rlmin	= FxSize;
					for( iy=0; iy<FySize; iy++ ) {
						rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
						ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-rx);
						if( iy == ry ) {
							ElecC1[elec].CornerPosX[0] = rx;
							ElecC1[elec].CornerPosY[0] = ry;
							break;
						} else {
							rl = abs( iy - ry );
							if( rl < rlmin ) {
								rlmin = rl;
								ElecC1[elec].CornerPosX[0] = rx;
								ElecC1[elec].CornerPosY[0] = ry;
							}
						}
					}
				// Corner Xmin Ymax
					sideX	= 5;
					sideY	= 7;
					rlmin	= FxSize;
					for( iy=0; iy<FySize; iy++ ) {
						rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
						ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-rx);
						if( iy == ry ) {
							ElecC1[elec].CornerPosX[1] = rx;
							ElecC1[elec].CornerPosY[1] = ry;
							break;
						} else {
							rl = abs( iy - ry );
							if( rl < rlmin ) {
								rlmin = rl;
								ElecC1[elec].CornerPosX[1] = rx;
								ElecC1[elec].CornerPosY[1] = ry;
							}
						}
					}
				// Corner Xmax Ymin
					sideX	= PosXmax;
					sideY	= 6;
					rlmin	= FxSize;
					for( iy=0; iy<FySize; iy++ ) {
						rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
						ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-rx);
						if( iy == ry ) {
							ElecC1[elec].CornerPosX[2] = rx;
							ElecC1[elec].CornerPosY[2] = ry;
							break;
						} else {
							rl = abs( iy - ry );
							if( rl < rlmin ) {
								rlmin = rl;
								ElecC1[elec].CornerPosX[2] = rx;
								ElecC1[elec].CornerPosY[2] = ry;
							}
						}
					}
				// Corner Xmax Ymax
					sideX	= PosXmax;
					sideY	= 7;
					rlmin	= FxSize;
					for( iy=0; iy<FySize; iy++ ) {
						rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
						ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-rx);
						if( iy == ry ) {
							ElecC1[elec].CornerPosX[3] = rx;
							ElecC1[elec].CornerPosY[3] = ry;
							break;
						} else {
							rl = abs( iy - ry );
							if( rl < rlmin ) {
								rlmin = rl;
								ElecC1[elec].CornerPosX[3] = rx;
								ElecC1[elec].CornerPosY[3] = ry;
							}
						}
					}

				break;
			case DataRight:
				if(ElecC12[elec].CornerPosX[0] == 0xffff || ElecC12[elec].CornerPosX[1] == 0xffff)	return;
				// 右 C1電極上
				side = 6;
				for(ix=0;ix<700;ix++){
					if(ElecEdge[elec][PosYmin].dataR[ix]!=OK)	continue;
					if(ElecEdge[elec][PosYmin].dataX[ix]<ElecC12[elec].CornerPosX[0]){
						ElecEdge[elec][side].dataR[ix]=ElecEdge[elec][PosYmin].dataR[ix];
						ElecEdge[elec][side].dataX[ix]=ElecEdge[elec][PosYmin].dataX[ix];
						ElecEdge[elec][side].dataY[ix]=ElecEdge[elec][PosYmin].dataY[ix];
					}
				}
				if( Regression( elec, side ) != OK )		return; // ERROR;

				// 右 C1電極下
				side = 7;
				for(ix=0;ix<700;ix++){
					if(ElecEdge[elec][PosYmax].dataR[ix]!=OK)	continue;
					if(ElecEdge[elec][PosYmax].dataX[ix]<ElecC12[elec].CornerPosX[1]){
						ElecEdge[elec][side].dataR[ix]=ElecEdge[elec][PosYmax].dataR[ix];
						ElecEdge[elec][side].dataX[ix]=ElecEdge[elec][PosYmax].dataX[ix];
						ElecEdge[elec][side].dataY[ix]=ElecEdge[elec][PosYmax].dataY[ix];
					}
				}
				if( Regression( elec, side ) != OK )		return; // ERROR;

				// Corner Xmin Ymin
					sideX	= PosXmin;
					sideY	= 6;
					rlmin	= FxSize;
					for( iy=0; iy<FySize; iy++ ) {
						rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
						ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-rx);
						if( iy == ry ) {
							ElecC1[elec].CornerPosX[0] = rx;
							ElecC1[elec].CornerPosY[0] = ry;
							break;
						} else {
							rl = abs( iy - ry );
							if( rl < rlmin ) {
								rlmin = rl;
								ElecC1[elec].CornerPosX[0] = rx;
								ElecC1[elec].CornerPosY[0] = ry;
							}
						}
					}
				// Corner Xmin Ymax
					sideX	= PosXmin;
					sideY	= 7;
					rlmin	= FxSize;
					for( iy=0; iy<FySize; iy++ ) {
						rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
						ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-rx);
						if( iy == ry ) {
							ElecC1[elec].CornerPosX[1] = rx;
							ElecC1[elec].CornerPosY[1] = ry;
							break;
						} else {
							rl = abs( iy - ry );
							if( rl < rlmin ) {
								rlmin = rl;
								ElecC1[elec].CornerPosX[1] = rx;
								ElecC1[elec].CornerPosY[1] = ry;
							}
						}
					}
				// Corner Xmax Ymin
					sideX	= 4;
					sideY	= 6;
					rlmin	= FxSize;
					for( iy=0; iy<FySize; iy++ ) {
						rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
						ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-rx);
						if( iy == ry ) {
							ElecC1[elec].CornerPosX[2] = rx;
							ElecC1[elec].CornerPosY[2] = ry;
							break;
						} else {
							rl = abs( iy - ry );
							if( rl < rlmin ) {
								rlmin = rl;
								ElecC1[elec].CornerPosX[2] = rx;
								ElecC1[elec].CornerPosY[2] = ry;
							}
						}
					}
				// Corner Xmax Ymax
					sideX	= 4;
					sideY	= 7;
					rlmin	= FxSize;
					for( iy=0; iy<FySize; iy++ ) {
						rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
						ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-rx);
						if( iy == ry ) {
							ElecC1[elec].CornerPosX[3] = rx;
							ElecC1[elec].CornerPosY[3] = ry;
							break;
						} else {
							rl = abs( iy - ry );
							if( rl < rlmin ) {
								rlmin = rl;
								ElecC1[elec].CornerPosX[3] = rx;
								ElecC1[elec].CornerPosY[3] = ry;
							}
						}
					}

				break;
		}




		// Draw corner pos.
			if( i_mode & INSPECT_STEP ) {
				R_DrawCls();
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				// corner pos.
				for( dn=0; dn<4; dn++ ) {
					rx = ElecC1[elec].CornerPosX[dn];
					ry = ElecC1[elec].CornerPosY[dn];
					if(( rx==0x0ffff )||( ry==0x0ffff ))	continue;
					R_DrawDispCross( DRAW_RED, rx, ry, 5, DRAW_NORMAL );
				}
				DrawMessage( "Electrode C1 corner pos.", DRAW_GREEN, 2, 2 );
			}

	}	// ElecCornerPosSet1st( int elec ) end

//================================================
//		Electrode corner pos. set
//================================================

void	ElecCornerPosSet( void )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						elec;
		int						dn, sideX, sideY;
		int						rx, ry;
		int						ix, iy, rl, rlmin;

		//v2.20 if(!bIsLTR50){	//normal
		if( (!bIsLTR50) && (!bIsLTR18) ){	//normal
		// Corner Xmin Ymin
			// Left
				Elec[DataLeft].CornerPosX[0] = Elec[DataLR].CornerPosX[0];
				Elec[DataLeft].CornerPosY[0] = Elec[DataLR].CornerPosY[0];
			// Right
				sideX	= PosXmin;
				sideY	= PosYmin;
				rlmin	= FxSize;
				elec	= DataRight;
				for( iy=0; iy<FySize; iy++ ) {
					rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
					ry = ElecEdge[DataLR][sideY].MeanY-ElecEdge[DataLR][sideY].Rev*(ElecEdge[DataLR][sideY].MeanX-rx);
					if( iy == ry ) {
						Elec[elec].CornerPosX[0] = rx;
						Elec[elec].CornerPosY[0] = ry;
						break;
					} else {
						rl = abs( iy - ry );
						if( rl < rlmin ) {
							rlmin = rl;
							Elec[elec].CornerPosX[0] = rx;
							Elec[elec].CornerPosY[0] = ry;
						}
					}
				}
		// Corner Xmin Ymax
			// Left
				Elec[DataLeft].CornerPosX[1] = Elec[DataLR].CornerPosX[1];
				Elec[DataLeft].CornerPosY[1] = Elec[DataLR].CornerPosY[1];
			// Right
				sideX	= PosXmin;
				sideY	= PosYmax;
				rlmin	= FxSize;
				elec	= DataRight;
				for( iy=0; iy<FySize; iy++ ) {
					rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
					ry = ElecEdge[DataLR][sideY].MeanY-ElecEdge[DataLR][sideY].Rev*(ElecEdge[DataLR][sideY].MeanX-rx);
					if( iy == ry ) {
						Elec[elec].CornerPosX[1] = rx;
						Elec[elec].CornerPosY[1] = ry;
						break;
					} else {
						rl = abs( iy - ry );
						if( rl < rlmin ) {
							rlmin = rl;
							Elec[elec].CornerPosX[1] = rx;
							Elec[elec].CornerPosY[1] = ry;
						}
					}
				}
		// Corner Xmax Ymin
			// Right
				Elec[DataRight].CornerPosX[2] = Elec[DataLR].CornerPosX[2];
				Elec[DataRight].CornerPosY[2] = Elec[DataLR].CornerPosY[2];
			// Left
				sideX	= PosXmax;
				sideY	= PosYmin;
				rlmin	= FxSize;
				elec	= DataLeft;
				for( iy=0; iy<FySize; iy++ ) {
					rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
					ry = ElecEdge[DataLR][sideY].MeanY-ElecEdge[DataLR][sideY].Rev*(ElecEdge[DataLR][sideY].MeanX-rx);
					if( iy == ry ) {
						Elec[elec].CornerPosX[2] = rx;
						Elec[elec].CornerPosY[2] = ry;
						break;
					} else {
						rl = abs( iy - ry );
						if( rl < rlmin ) {
							rlmin = rl;
							Elec[elec].CornerPosX[2] = rx;
							Elec[elec].CornerPosY[2] = ry;
						}
					}
				}
		// Corner Xmax Ymax
			// Right
				Elec[DataRight].CornerPosX[3] = Elec[DataLR].CornerPosX[3];
				Elec[DataRight].CornerPosY[3] = Elec[DataLR].CornerPosY[3];
			// Left
				sideX	= PosXmax;
				sideY	= PosYmax;
				rlmin	= FxSize;
				elec	= DataLeft;
				for( iy=0; iy<FySize; iy++ ) {
					rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
					ry = ElecEdge[DataLR][sideY].MeanY-ElecEdge[DataLR][sideY].Rev*(ElecEdge[DataLR][sideY].MeanX-rx);
					if( iy == ry ) {
						Elec[elec].CornerPosX[3] = rx;
						Elec[elec].CornerPosY[3] = ry;
						break;
					} else {
						rl = abs( iy - ry );
						if( rl < rlmin ) {
							rlmin = rl;
							Elec[elec].CornerPosX[3] = rx;
							Elec[elec].CornerPosY[3] = ry;
						}
					}
				}
		} else {		// LTR
			// Corner Xmin Ymin
				// Left
					Elec[DataUpper].CornerPosX[0] = Elec[DataUL].CornerPosX[0];
					Elec[DataUpper].CornerPosY[0] = Elec[DataUL].CornerPosY[0];
				// Right
					sideX	= PosXmin;
					sideY	= PosYmin;
					rlmin	= FySize;
					elec	= DataLower;
					for( ix=0; ix<FxSize; ix++ ) {
						ry = ElecEdge[elec  ][sideY].MeanY-ElecEdge[elec  ][sideY].Rev*(ElecEdge[elec  ][sideY].MeanX-ix);
						rx = ElecEdge[DataUL][sideX].MeanX-ElecEdge[DataUL][sideX].Rev*(ElecEdge[DataUL][sideX].MeanY-ry);
						if( ix == rx ) {
							Elec[elec].CornerPosX[0] = rx;
							Elec[elec].CornerPosY[0] = ry;
							break;
						} else {
							rl = abs( ix - rx );
							if( rl < rlmin ) {
								rlmin = rl;
								Elec[elec].CornerPosX[0] = rx;
								Elec[elec].CornerPosY[0] = ry;
							}
						}
					}
			// Corner Ymin Xmax
				// Left
					Elec[DataUpper].CornerPosX[2] = Elec[DataUL].CornerPosX[2];
					Elec[DataUpper].CornerPosY[2] = Elec[DataUL].CornerPosY[2];
				// Right
					sideX	= PosXmax;
					sideY	= PosYmin;
					rlmin	= FySize;
					elec	= DataLower;
					for( ix=0; ix<FxSize; ix++ ) {
						ry = ElecEdge[elec  ][sideY].MeanY-ElecEdge[elec  ][sideY].Rev*(ElecEdge[elec  ][sideY].MeanX-ix);
						rx = ElecEdge[DataUL][sideX].MeanX-ElecEdge[DataUL][sideX].Rev*(ElecEdge[DataUL][sideX].MeanY-ry);
						if( ix == rx ) {
							Elec[elec].CornerPosX[2] = rx;
							Elec[elec].CornerPosY[2] = ry;
							break;
						} else {
							rl = abs( ix - rx );
							if( rl < rlmin ) {
								rlmin = rl;
								Elec[elec].CornerPosX[2] = rx;
								Elec[elec].CornerPosY[2] = ry;
							}
						}
					}
			// Corner Ymax Xmin
				// Right
					Elec[DataLower].CornerPosX[1] = Elec[DataUL].CornerPosX[1];
					Elec[DataLower].CornerPosY[1] = Elec[DataUL].CornerPosY[1];
				// Left
					sideX	= PosXmin;
					sideY	= PosYmax;
					rlmin	= FySize;
					elec	= DataUpper;
					for( ix=0; ix<FxSize; ix++ ) {
						ry = ElecEdge[elec  ][sideY].MeanY-ElecEdge[elec  ][sideY].Rev*(ElecEdge[elec  ][sideY].MeanX-ix);
						rx = ElecEdge[DataUL][sideX].MeanX-ElecEdge[DataUL][sideX].Rev*(ElecEdge[DataUL][sideX].MeanY-ry);
						if( ix == rx ) {
							Elec[elec].CornerPosX[1] = rx;
							Elec[elec].CornerPosY[1] = ry;
							break;
						} else {
							rl = abs( ix - rx );
							if( rl < rlmin ) {
								rlmin = rl;
								Elec[elec].CornerPosX[1] = rx;
								Elec[elec].CornerPosY[1] = ry;
							}
						}
					}
			// Corner Ymax Xmax
				// Right
					Elec[DataLower].CornerPosX[3] = Elec[DataUL].CornerPosX[3];
					Elec[DataLower].CornerPosY[3] = Elec[DataUL].CornerPosY[3];
				// Left
					sideX	= PosXmax;
					sideY	= PosYmax;
					rlmin	= FySize;
					elec	= DataUpper;
					for( ix=0; ix<FxSize; ix++ ) {
						ry = ElecEdge[elec  ][sideY].MeanY-ElecEdge[elec  ][sideY].Rev*(ElecEdge[elec  ][sideY].MeanX-ix);
						rx = ElecEdge[DataUL][sideX].MeanX-ElecEdge[DataUL][sideX].Rev*(ElecEdge[DataUL][sideX].MeanY-ry);
						if( ix == rx ) {
							Elec[elec].CornerPosX[3] = rx;
							Elec[elec].CornerPosY[3] = ry;
							break;
						} else {
							rl = abs( ix - rx );
							if( rl < rlmin ) {
								rlmin = rl;
								Elec[elec].CornerPosX[3] = rx;
								Elec[elec].CornerPosY[3] = ry;
							}
						}
					}

		}
			// Draw corner pos.
			if( i_mode & INSPECT_STEP ) {
				R_DrawCls();
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				// corner pos.
				for( elec=DataLeft; elec<=DataLR; elec++ ) {
					for( dn=0; dn<4; dn++ ) {
						rx = Elec[elec].CornerPosX[dn];
						ry = Elec[elec].CornerPosY[dn];
						if(( rx==0x0ffff )||( ry==0x0ffff ))	continue;
						if( elec != DataLR )	R_DrawDispCross( DRAW_GREEN, rx, ry, 5, DRAW_NORMAL );
						else					R_DrawDispCross( DRAW_RED, rx, ry, 5, DRAW_NORMAL );
					}
				}
				DrawMessage( "ElecCornerPosSet Electrode corner pos.", DRAW_GREEN, 2, 2 );
			}

	}	// ElecCornerPosSet( void ) end


//================================================
//		電極検査
//================================================

int		InspElectrode( void )
	{
//		int						grayPn	= InspGrayPn;
		int						binPn	= InspBinPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						xmin, xmax, xsize, rx, xsizeM;
		int						ymin, ymax, ysize, ry, ysizeM;
		int						bn;
		int						elec;
		int						res;
        R_RECT                  area;

		// Insp. electrode leve set
			if( ElecInspLevelSet() != OK )	return ERROR;

		// Inspection electrode size
			res = InspElecSize();
			switch( res ) {
				case OK:
					break;
				case ERROR:				// bolb No. error
					return ERROR;
					break;
				case ERROR2:			// regression analysis error
					return ERROR;
					break;
				case NG_ELEC_X_SIZE:	// electrode X size error
					return NG_ELEC_X_SIZE;
					break;
				case NG_ELEC_Y_SIZE:	// electrode Y size error
					return NG_ELEC_Y_SIZE;
					break;
				default:
					return ERROR;
					break;
			}

		// Inspection electrode egdg
			res = InspElecEdge();
			if( i_mode & INSPECT_STEP )	R_DrawCls();
			switch( res ) {
				case OK:
					break;
				default:
					return(res);
					break;
			}

		// Insp. Bin page set
			InspBinSetElectrode();

		// Binary measure
			if( i_mode & INSPECT_STEP ) {
				R_move_bin_memory(binPn,0,0,0,FxSize-1,FySize-1);
			}
			// 特徴量演算　演算方式指定
				R_set_meas_configration( &Mpx, BinMeasElec.nRun, BinMeasElec.nArea, BinMeasElec.nColor, BinMeasElec.nMode );
			// Elec side
				for( elec=DataLeft; elec<=DataRight; elec++ ) {
					// Measure area
						xmin = Elec[elec].Pos[PosXmin];		ymin = Elec[elec].Pos[PosYmin];
						xmax = Elec[elec].Pos[PosXmax];		ymax = Elec[elec].Pos[PosYmax];
						xsize = xmax-xmin+1;				ysize = ymax-ymin+1;
					// Measure
						if( R_measure( &Mpx, binPn, xmin, ymin, xsize, ysize, GROUP2 ) != OK ) {
							R_measure_close( &Mpx );
							R_measure_open( &Mpx, 2000 );
							return ERROR;
						}
					// Inspection
                        res = OK;
                        for( bn=1; bn<=Mpx.bn; bn++ ) {
							if( Mpx.color[bn] == ColorWhite )	continue;
							// Judge
//							if( Mpx.xdiff[bn] > InspPara.ElecKake[TWA_Spec].nXsize )		res =  ERROR5;
//							if( Mpx.ydiff[bn] > InspPara.ElecKake[TWA_Spec].nYsize )		res =  ERROR6;
                            if( (( Mpx.xdiff[bn] > InspPara.ElecKake[TWA_Spec].nXsize )&&
                                 ( Mpx.ydiff[bn] > InspPara.ElecKake[TWA_Spec].nYsize )) ||
                                ( Mpx.area[bn] > InspPara.ElecKake[TWA_Spec].nXsize * InspPara.ElecKake[TWA_Spec].nYsize ) ){
                                if( i_mode & INSPECT_STEP ){
                                    area.left	= Mpx.imin[bn];
                                    area.right	= Mpx.imax[bn];
                                    area.top	= Mpx.jmin[bn];
                                    area.bottom	= Mpx.jmax[bn];
                                    R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
                                }
                                res = ERROR7;
                            }
                            // Draw blob
                            if( i_mode & INSPECT_STEP ) {
                                // x line
                                ry		= Mpx.center_y[bn];
                                xmin	= Mpx.imin[bn];
                                xmax	= Mpx.imax[bn];
                                R_DrawLine( DRAW_GREEN, xmin, ry, xmax, ry, DRAW_NORMAL, SOLID_LINE );
                                // y line
                                rx		= Mpx.center_x[bn];
                                ymin	= Mpx.jmin[bn];
                                ymax	= Mpx.jmax[bn];
                                R_DrawLine( DRAW_GREEN, rx, ymin, rx, ymax, DRAW_NORMAL, SOLID_LINE );
                                // data
                                xsize = Mpx.imax[bn] - Mpx.imin[bn] + 1;
                                ysize = Mpx.jmax[bn] - Mpx.jmin[bn] + 1;
                                xsizeM = (xsize*SystemPara.nRateX+500)/1000;
                                ysizeM = (ysize*SystemPara.nRateY+500)/1000;
                                sprintf( Comment1, "Measure       X:%4d Y:%4d(bit), X:%4d Y:%4d(um), Area:%4d(bit)"
                                         , xsize, ysize, xsizeM, ysizeM, Mpx.area[bn] );
                                sprintf( Comment2, "ElecKakeLimit X:%4d Y:%4d(bit)                   Area:%4d(bit)"
                                         , InspPara.ElecKake[TWA_Spec].nXsize, InspPara.ElecKake[TWA_Spec].nYsize
                                         , InspPara.ElecKake[TWA_Spec].nXsize * InspPara.ElecKake[TWA_Spec].nYsize );
                                sprintf( Comment, "%s\n%s", Comment1, Comment2 );
                                if(res==OK) DrawMessage( Comment, DRAW_GREEN, 2, 2 );
                                else        DrawMessage( Comment, DRAW_RED, 2, 2 );
                            }
                            if(res!=OK) return res;
                        }
				}
		// 表示消去
			if( i_mode & INSPECT_STEP )	R_DrawCls();

		return OK;

	}	// InspElectrode( void ) end


//================================================
//	電極検査レベル設定
//	Elec. Insp. level set
//================================================

int		ElecInspLevelSet( void )
	{
			ElecInspLevel.nData = ChipEsLevel.nData + ElecInspLevel.nOffset;
			if( ElecInspLevel.nData < ElecInspLevel.nLower )	ElecInspLevel.nData = ElecInspLevel.nLower;
			if( ElecInspLevel.nData > ElecInspLevel.nUpper )	ElecInspLevel.nData = ElecInspLevel.nUpper;

		// result
			return OK;

	}	// ElecInspLevelSet( void ) end


//================================================
//		Insp. electrode size
//================================================

int		InspElecSize( void )
	{
		int						i_mode	= R_CS_GetInspectMode();
		int						bnL, bnR, elec, dn;
		int						xmin, xmax, xsize1[2], xsize2[2];
		int						ymin, ymax, ysize1[2], ysize2[2];
		int						xsize1M[2],xsize2M[2];
		int						ysize1M[2],ysize2M[2];
		int						res[2][4];
		int						c_mode;
		int						posi_res_x,posi_res_y;
//		int						side;

		// Check bin measure blob No.
			if( Elec[DataLeft].BlobNo <= 0 )	return ERROR;
			if( Elec[DataRight].BlobNo <= 0 )	return ERROR;
			bnL = Elec[DataLeft].BlobNo;
			bnR = Elec[DataRight].BlobNo;

		// Electrode size
			for( elec=DataLeft; elec<=DataRight; elec++ ) {
				// initial
					for( dn=0; dn<4; dn++ )	res[elec][dn] = OK;
				// X size
					xsize1[elec] = Elec[elec].CornerPosX[2]-Elec[elec].CornerPosX[0]+1;
					xsize2[elec] = Elec[elec].CornerPosX[3]-Elec[elec].CornerPosX[1]+1;
					if( xsize1[elec] < InspPara.ElecXmin[TWA_Spec].nXsize )	res[elec][0] = InspPara.ElecXmin[TWA_Spec].nCode;
					if( xsize1[elec] > InspPara.ElecXmax[TWA_Spec].nXsize )	res[elec][0] = InspPara.ElecXmax[TWA_Spec].nCode;
					if( xsize2[elec] < InspPara.ElecXmin[TWA_Spec].nXsize )	res[elec][2] = InspPara.ElecXmin[TWA_Spec].nCode;
					if( xsize2[elec] > InspPara.ElecXmax[TWA_Spec].nXsize )	res[elec][2] = InspPara.ElecXmax[TWA_Spec].nCode;
				// Y size
					ysize1[elec] = Elec[elec].CornerPosY[1]-Elec[elec].CornerPosY[0]+1;
					ysize2[elec] = Elec[elec].CornerPosY[3]-Elec[elec].CornerPosY[2]+1;
					if( ysize1[elec] < InspPara.ElecYmin[TWA_Spec].nYsize )	res[elec][1] = InspPara.ElecYmin[TWA_Spec].nCode;
					if( ysize1[elec] > InspPara.ElecYmax[TWA_Spec].nYsize )	res[elec][1] = InspPara.ElecYmax[TWA_Spec].nCode;
					if( ysize2[elec] < InspPara.ElecYmin[TWA_Spec].nYsize )	res[elec][3] = InspPara.ElecYmin[TWA_Spec].nCode;
					if( ysize2[elec] > InspPara.ElecYmax[TWA_Spec].nYsize )	res[elec][3] = InspPara.ElecYmax[TWA_Spec].nCode;
			}
		// Draw corner pos.
			if( i_mode & INSPECT_STEP ) {
				R_DrawCls();
				for( elec=DataLeft; elec<=DataRight; elec++ ) {
					xmin = Elec[elec].CornerPosX[0];	ymin = Elec[elec].CornerPosY[0];
					xmax = Elec[elec].CornerPosX[2];	ymax = Elec[elec].CornerPosY[2];
						if( res[elec][0] == OK )	c_mode = DRAW_GREEN;
						else						c_mode = DRAW_RED;
						R_DrawLine( c_mode, xmin, ymin, xmax, ymax, DRAW_NORMAL, SOLID_LINE );
					xmin = Elec[elec].CornerPosX[1];	ymin = Elec[elec].CornerPosY[1];
					xmax = Elec[elec].CornerPosX[3];	ymax = Elec[elec].CornerPosY[3];
						if( res[elec][2] == OK )	c_mode = DRAW_GREEN;
						else						c_mode = DRAW_RED;
						R_DrawLine( c_mode, xmin, ymin, xmax, ymax, DRAW_NORMAL, SOLID_LINE );
					xmin = Elec[elec].CornerPosX[0];	ymin = Elec[elec].CornerPosY[0];
					xmax = Elec[elec].CornerPosX[1];	ymax = Elec[elec].CornerPosY[1];
						if( res[elec][1] == OK )	c_mode = DRAW_GREEN;
						else						c_mode = DRAW_RED;
						R_DrawLine( c_mode, xmin, ymin, xmax, ymax, DRAW_NORMAL, SOLID_LINE );
					xmin = Elec[elec].CornerPosX[2];	ymin = Elec[elec].CornerPosY[2];
					xmax = Elec[elec].CornerPosX[3];	ymax = Elec[elec].CornerPosY[3];
						if( res[elec][3] == OK )	c_mode = DRAW_GREEN;
						else						c_mode = DRAW_RED;
						R_DrawLine( c_mode, xmin, ymin, xmax, ymax, DRAW_NORMAL, SOLID_LINE );
                }
				posi_res_x = 0;
				posi_res_y = 30;
				xsize1M[0] = (xsize1[0]*SystemPara.nRateX+500)/1000;
				xsize2M[0] = (xsize2[0]*SystemPara.nRateX+500)/1000;
				ysize1M[0] = (ysize1[0]*SystemPara.nRateY+500)/1000;
				ysize2M[0] = (ysize2[0]*SystemPara.nRateY+500)/1000;
				xsize1M[1] = (xsize1[1]*SystemPara.nRateX+500)/1000;
				xsize2M[1] = (xsize2[1]*SystemPara.nRateX+500)/1000;
				ysize1M[1] = (ysize1[1]*SystemPara.nRateY+500)/1000;
				ysize2M[1] = (ysize2[1]*SystemPara.nRateY+500)/1000;


				R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"[Check electrode size]");
				R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+2 ,"xsize L :%4dum (%3ddot) %4dum (%3ddot) (%3d-%3d)" ,xsize1M[0],xsize1[0],xsize2M[0],xsize2[0],InspParaM.ElecXmin[TWA_Spec].nXsize ,InspParaM.ElecXmax[TWA_Spec].nXsize );
				R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+3 ,"ysize L :%4dum (%3ddot) %4dum (%3ddot) (%3d-%3d)" ,ysize1M[0],ysize1[0],ysize2M[0],ysize2[0],InspParaM.ElecYmin[TWA_Spec].nYsize ,InspParaM.ElecYmax[TWA_Spec].nYsize );
				R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+4 ,"xsize R :%4dum (%3ddot) %4dum (%3ddot) (%3d-%3d)" ,xsize1M[1],xsize1[1],xsize2M[1],xsize2[1],InspParaM.ElecXmin[TWA_Spec].nXsize ,InspParaM.ElecXmax[TWA_Spec].nXsize );
                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+5 ,"ysize R :%4dum (%3ddot) %4dum (%3ddot) (%3d-%3d)" ,ysize1M[1],ysize1[1],ysize2M[1],ysize2[1],InspParaM.ElecYmin[TWA_Spec].nYsize ,InspParaM.ElecYmax[TWA_Spec].nYsize );

				//DrawMessage( "Check electrode size", DRAW_GREEN, 2, 2 );
				//R_DrawCls();
				while( !R_DrawGetCursor());
				R_DrawCls();
				while( R_DrawGetCursor());
			}

		// result
			for( elec=DataLeft; elec<=DataRight; elec++ ) {
				for( dn=0; dn<4; dn++ ) {
					if( res[elec][dn] != OK )	return( res[elec][dn] );
				}
			}
			return OK;

	}	// InspElecSize( void ) end



//================================================
//	電極欠け判定
//================================================

int		InspElecEdge( void )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						elec, side;
		int						xmin, xmax;
		int						ymin, ymax;
		int						posi_res_x,posi_res_y;
		int						res;

		// View
			if( i_mode & INSPECT_STEP ) {
				R_DrawCls();
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );		// 原画の表示
				for( elec=DataLeft; elec<=DataRight; elec++ ) {
					// --- Top
					xmin = Elec[elec].CornerPosX[PosLup];	ymin = Elec[elec].CornerPosY[PosLup];
					xmax = Elec[elec].CornerPosX[PosRup];	ymax = Elec[elec].CornerPosY[PosRup];
					R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, SOLID_LINE );
					// --- Bottom
					xmin = Elec[elec].CornerPosX[PosLlo];	ymin = Elec[elec].CornerPosY[PosLlo];
					xmax = Elec[elec].CornerPosX[PosRlo];	ymax = Elec[elec].CornerPosY[PosRlo];
					R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, SOLID_LINE );
					// --- Left
					xmin = Elec[elec].CornerPosX[PosLup];	ymin = Elec[elec].CornerPosY[PosLup];
					xmax = Elec[elec].CornerPosX[PosLlo];	ymax = Elec[elec].CornerPosY[PosLlo];
					R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, SOLID_LINE );
					// --- Right
					xmin = Elec[elec].CornerPosX[PosRup];	ymin = Elec[elec].CornerPosY[PosRup];
					xmax = Elec[elec].CornerPosX[PosRlo];	ymax = Elec[elec].CornerPosY[PosRlo];
					R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, SOLID_LINE );
				}
				posi_res_x = 0;
				posi_res_y = 30;

				R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"[Check electrode corner]");
				//R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+2 ,"xsize L :%4dum (%3ddot) %4dum (%3ddot) (%3d-%3d)" ,xsize1M[0],xsize1[0],xsize2M[0],xsize2[0],InspParaM.ElecXmin[TWA_Spec].nXsize ,InspParaM.ElecXmax[TWA_Spec].nXsize );
				//R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+3 ,"ysize L :%4dum (%3ddot) %4dum (%3ddot) (%3d-%3d)" ,ysize1M[0],ysize1[0],ysize2M[0],ysize2[0],InspParaM.ElecYmin[TWA_Spec].nYsize ,InspParaM.ElecYmax[TWA_Spec].nYsize );
				//R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+4 ,"xsize R :%4dum (%3ddot) %4dum (%3ddot) (%3d-%3d)" ,xsize1M[1],xsize1[1],xsize2M[1],xsize2[1],InspParaM.ElecXmin[TWA_Spec].nXsize ,InspParaM.ElecXmax[TWA_Spec].nXsize );
				//R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+5 ,"ysize R :%4dum (%3ddot) %4dum (%3ddot) (%3d-%3d)" ,ysize1M[1],ysize1[1],ysize2M[1],xsize2[1],InspParaM.ElecYmin[TWA_Spec].nYsize ,InspParaM.ElecYmax[TWA_Spec].nYsize );

				while( !R_DrawGetCursor());
				R_DrawCls();
				while( R_DrawGetCursor());
				//DrawMessage( "Check electrode size", DRAW_GREEN, 2, 2 );
				//R_DrawCls();
			}

		// Ap. Kake size
			for( elec=DataLeft; elec<=DataRight; elec++ ) {
				for( side=0; side<4; side++ ) {
					res = JudgeElecEdge( elec, side );
					if( OK != res )	return(res);
/*
					switch( res ) {
						case OK:
							break;
						case NG_ELEC_EDGE_X:
							return NG_ELEC_EDGE_X;
							break;
						case NG_ELEC_EDGE_Y:
							return NG_ELEC_EDGE_Y;
							break;
						default:
							return ERROR;
							break;
					}
*/
				}
			}

		return OK;

	}	// InspElecEdge( void ) end


//================================================
//	電極回帰直線から電極エッジまでの寸法判定
//================================================

int		JudgeElecEdge( int elec, int side )
	{
		int						i_mode	= R_CS_GetInspectMode();
		int						judgeXsize, judgeYsize, judgeCode;
		int						judgeXsizeM,judgeYsizeM;
		int						cls_c;
		int						dns, dne, dn,masks,maske;
		int						fns, fne;
		int						stdx, stdy;
		int						lengthX, lengthY;
		int						lengthXmax, lengthYmax;
		int						lengthXM,lengthYM;
		int						xmin, xmax, ymin, ymax, rx, ry;
		int						fxmin, fxmax, fymin, fymax;
		int						posi_res_x,posi_res_y;
		int						res;
		int						c_mode;

		// Initial
			// --- Judge data
			//v2.20 if(!bIsLTR50){	// normal
			if( (!bIsLTR50) && (!bIsLTR18) ){	// normal
				switch( side ) {
					case PosYmin:
					case PosYmax:
						judgeXsize	= InspPara.ElecYedge[TWA_Spec].nXsize;
						judgeYsize	= InspPara.ElecYedge[TWA_Spec].nYsize;
						judgeCode	= InspPara.ElecYedge[TWA_Spec].nCode;
						break;
					case PosXmin:
						if( DataLeft==elec ) {
							judgeXsize	= InspPara.ChipXedge[TWA_Spec].nXsize;
							judgeYsize	= InspPara.ChipXedge[TWA_Spec].nYsize;
							judgeCode	= InspPara.ChipXedge[TWA_Spec].nCode;
						} else {
							judgeXsize	= InspPara.ElecXedge[TWA_Spec].nXsize;
							judgeYsize	= InspPara.ElecXedge[TWA_Spec].nYsize;
							judgeCode	= InspPara.ElecXedge[TWA_Spec].nCode;
						}
						break;
					case PosXmax:
						if( DataRight==elec ) {
							judgeXsize	= InspPara.ChipXedge[TWA_Spec].nXsize;
							judgeYsize	= InspPara.ChipXedge[TWA_Spec].nYsize;
							judgeCode	= InspPara.ChipXedge[TWA_Spec].nCode;
						} else {
							judgeXsize	= InspPara.ElecXedge[TWA_Spec].nXsize;
							judgeYsize	= InspPara.ElecXedge[TWA_Spec].nYsize;
							judgeCode	= InspPara.ElecXedge[TWA_Spec].nCode;
						}
						break;
				}
			} else {	//LTR
				switch( side ) {
					case PosYmin:
						if( DataUpper==elec ) {
							judgeXsize	= InspPara.ChipYedge[TWA_Spec].nXsize;
							judgeYsize	= InspPara.ChipYedge[TWA_Spec].nYsize;
							judgeCode	= InspPara.ChipYedge[TWA_Spec].nCode;
						} else {
							judgeXsize	= InspPara.ElecYedge[TWA_Spec].nXsize;
							judgeYsize	= InspPara.ElecYedge[TWA_Spec].nYsize;
							judgeCode	= InspPara.ElecYedge[TWA_Spec].nCode;
						}
						break;
					case PosYmax:
						if( DataLower==elec ) {
							judgeXsize	= InspPara.ChipYedge[TWA_Spec].nXsize;
							judgeYsize	= InspPara.ChipYedge[TWA_Spec].nYsize;
							judgeCode	= InspPara.ChipYedge[TWA_Spec].nCode;
						} else {
							judgeXsize	= InspPara.ElecYedge[TWA_Spec].nXsize;
							judgeYsize	= InspPara.ElecYedge[TWA_Spec].nYsize;
							judgeCode	= InspPara.ElecYedge[TWA_Spec].nCode;
						}
						break;
					case PosXmin:
					case PosXmax:
						judgeXsize	= InspPara.ElecXedge[TWA_Spec].nXsize;
						judgeYsize	= InspPara.ElecXedge[TWA_Spec].nYsize;
						judgeCode	= InspPara.ElecXedge[TWA_Spec].nCode;
						break;
				}
			}

			// --- Chip corner size
			if((side==PosYmin)||(side==PosYmax)) {
				cls_c = ChipCornerSize[DataXbit];
			} else {
				cls_c = ChipCornerSize[DataYbit];
			}

		// Chip edge kake 検査範囲指定
			switch( side ) {
				case PosXmin:	// --- Left
					//v2.20 if(!bIsLTR50){	// normal
					if( ((bIsLTR50)&&(0==(Option & BACKINSP_MODE))) || ((bIsLTR18)&&(0==(Option & BACKINSP_MODE)))  ){	// LTR
						if(elec == DataUpper){
							dns = Elec[elec].CornerPosY[PosLup];	// Ymin
							//180522 dne = dns+10;							// Ymax 凸形状
							dne = dns+20;							// Ymax 凸形状
						} else {
							dne = Elec[elec].CornerPosY[PosLlo];	// Ymax
							//180522 dns = dne-10;							// Ymin 凸形状
							dns = dne-20;							// Ymin 凸形状
						}
					} else {	// nomal
						dns = Elec[elec].CornerPosY[PosLup];	// Ymin
						dne = Elec[elec].CornerPosY[PosLlo];	// Ymax
					}
					break;
				case PosYmin:	// --- Top
					dns = Elec[elec].CornerPosX[PosLup];	// Xmin
					dne = Elec[elec].CornerPosX[PosRup];	// Xmax
					if(bIsMCR18){							// 凸形状
						if(elec == 0){
							if(ElecC12[elec].CornerPosX[PosRup] != 0xffff){
								dne = ElecC12[elec].CornerPosX[PosRup];
							}
						} else {
							if(ElecC12[elec].CornerPosX[PosLup] != 0xffff){
								dns = ElecC12[elec].CornerPosX[PosLup];
							}
						}
					}
					break;
				case PosXmax:	// --- Right
					//v2.20 if(!bIsLTR50){	// normal
					if( ((bIsLTR50)&&(0==(Option & BACKINSP_MODE))) || ((bIsLTR18)&&(0==(Option & BACKINSP_MODE)))  ){	// LTR
						if(elec == DataUpper){
							dns = Elec[elec].CornerPosY[PosRup];	// Ymin
							//180522 dne = dns+10;							// Ymax 凸形状
							dne = dns+20;							// Ymax 凸形状
						} else {
							dne = Elec[elec].CornerPosY[PosRlo];	// Ymax
							//180522 dns = dne-10;							// Ymin 凸形状
							dns = dne-20;							// Ymin 凸形状
						}
					} else {	// normal
						dns = Elec[elec].CornerPosY[PosRup];	// Ymin
						dne = Elec[elec].CornerPosY[PosRlo];	// Ymax
					}
					break;
				case PosYmax:	// --- Bottom
					dns = Elec[elec].CornerPosX[PosLlo];	// Xmin
					dne = Elec[elec].CornerPosX[PosRlo];	// Xmax
					if(bIsMCR18){							// 凸形状
						if(elec == 0){
							if(ElecC12[elec].CornerPosX[PosRlo] != 0xffff){
								dne = ElecC12[elec].CornerPosX[PosRlo];
							}
						} else {
							if(ElecC12[elec].CornerPosX[PosLlo] != 0xffff){
								dns = ElecC12[elec].CornerPosX[PosLlo];
							}
						}
					}
					break;
			}
			//v2.20 if(bIsLTR50){		//角の位置の補正をする前のcorner位置の情報を使用する
			if( (bIsLTR50) || (bIsLTR18) ){		//角の位置の補正をする前のcorner位置の情報を使用する
				masks=dns;
				maske=dne;
			}
			// --- コーナー部分の検査範囲から除外
				dns += cls_c;
				if(dns > 700)	return ERROR;
				// dns,dneの位置が無効データの場合は内寄りを選択する 20170529 tanaka
				while(ElecEdge[elec][side].dataX[dns] ==0xffff || ElecEdge[elec][side].dataY[dns] == 0xffff){
					dns++;
					if(dns > 700)	return ERROR;
				}
				dne -= cls_c;
				if(dne < 0)	return ERROR;
				while(ElecEdge[elec][side].dataX[dne] ==0xffff || ElecEdge[elec][side].dataY[dne] == 0xffff){
					dne--;
					if(dne < 0)	return ERROR;
				}
				if( dns >= dne )	return ERROR;

		// Judge  std. - edge
			res = OK;
			lengthX = lengthY = lengthXmax = lengthYmax = 0;
			fns = ERROR;
			for( dn=dns; dn<=dne; dn++ ) {
				rx = ElecEdge[elec][side].dataX[dn];
				ry = ElecEdge[elec][side].dataY[dn];
				if(rx == 0xffff || ry == 0xffff)    continue;   //20170204暫定 tanaka
				if((side==PosYmin)||(side==PosYmax)) {
					stdy = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-rx);
					//v2.20 if(bIsLTR50){
					if( (bIsLTR50) || (bIsLTR18) ){
						if(dn < (masks+17) || (dn > ((masks+maske)/2 -20) && ((masks+maske)/2 +15) > dn) ||  (maske - 17) < dn){
							if(elec==DataUpper	&& side==PosYmax ){
								stdy = ElecEdge[elec][PosYmin].MeanY-ElecEdge[elec][PosYmin].Rev*(ElecEdge[elec][PosYmin].MeanX-rx)+8;
							}
							if(elec==DataLower && side==PosYmin){
								stdy = ElecEdge[elec][PosYmax].MeanY-ElecEdge[elec][PosYmax].Rev*(ElecEdge[elec][PosYmax].MeanX-rx)-8;
							}
						}
					}

					lengthY = abs( stdy - ry );
					if(side==PosYmin) {
						if( stdy > ry )	lengthY = 0;
					} else {
						if( stdy < ry )	lengthY = 0;
					}
					if( lengthYmax < lengthY)	lengthYmax = lengthY;
					if( lengthY > judgeYsize ) {
						if( fns < 0 )	fns = dn;
						lengthX++;
						if( lengthXmax < lengthX)	lengthXmax = lengthX;
						if( lengthX > judgeXsize ) {
							fne = dn;
							res = judgeCode;
							break;
						}
					} else {
						lengthX = lengthY = 0;
						fns = ERROR;
					}
				} else {
					if(bIsMCR18){
						switch(elec){
							case DataLeft:
								if(side == PosXmax && ( ElecC1[elec].CornerPosY[0] != 0xffff && ry < ElecC1[elec].CornerPosY[0]  || ElecC1[elec].CornerPosY[1] != 0xffff && ElecC1[elec].CornerPosY[1] < ry)){
									stdx = ElecEdge[elec][5   ].MeanX-ElecEdge[elec][5   ].Rev*(ElecEdge[elec][5   ].MeanY-ry);
								} else {
									stdx = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ry);
								}
								break;
							case DataRight:
								if(side == PosXmin && ( ElecC1[elec].CornerPosY[2] != 0xffff && ry < ElecC1[elec].CornerPosY[2]  || ElecC1[elec].CornerPosY[3] != 0xffff && ElecC1[elec].CornerPosY[3] < ry)){
									stdx = ElecEdge[elec][4   ].MeanX-ElecEdge[elec][4   ].Rev*(ElecEdge[elec][4   ].MeanY-ry);
								} else {
									stdx = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ry);
								}
								break;
						}
					} else {
						stdx = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ry);
					}
					lengthX = abs( stdx - rx );
					if(side==PosXmin) {
						if( stdx > rx )	lengthX = 0;
					} else {
						if( stdx < rx )	lengthX = 0;
					}
					if( lengthXmax < lengthX)	lengthXmax = lengthX;
					if( lengthX > judgeXsize ) {
						if( fns < 0 )	fns = dn;
						lengthY++;
						if( lengthYmax < lengthY)	lengthYmax = lengthY;
						if( lengthY > judgeYsize ) {
							fne = dn;
							res = judgeCode;
							break;
						}
					} else {
						lengthX = lengthY = 0;
						fns = ERROR;
					}
				}
			}

			// --- draw
			if( i_mode & INSPECT_STEP ) {
				// --- std line
				if(bIsMCR18 && (elec == DataLeft || elec == DataRight)){
					switch(elec){
						case DataLeft:
							if(side == PosXmax){
								xmin = ElecC12[elec].CornerPosX[2];
								ymin = ElecC12[elec].CornerPosY[2];
								xmax = ElecC1[elec].CornerPosX[0];
								ymax = ElecC1[elec].CornerPosY[0];
								R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, SOLID_LINE );
								xmin = ElecC1[elec].CornerPosX[2];
								ymin = ElecC1[elec].CornerPosY[2];
								xmax = ElecC1[elec].CornerPosX[3];
								ymax = ElecC1[elec].CornerPosY[3];
								R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, SOLID_LINE );
								xmin = ElecC1[elec].CornerPosX[1];
								ymin = ElecC1[elec].CornerPosY[1];
								xmax = ElecC12[elec].CornerPosX[3];
								ymax = ElecC12[elec].CornerPosY[3];
								R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, SOLID_LINE );

							} else {
								xmin = ElecEdge[elec][side].dataX[dns];
								ymin = ElecEdge[elec][side].dataY[dns];
								xmax = ElecEdge[elec][side].dataX[dne];
								ymax = ElecEdge[elec][side].dataY[dne];

								xmin = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
								xmax = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
								R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, SOLID_LINE );
							}
							break;
						case DataRight:
							if(side == PosXmin){
								xmin = ElecC12[elec].CornerPosX[0];
								ymin = ElecC12[elec].CornerPosY[0];
								xmax = ElecC1[elec].CornerPosX[2];
								ymax = ElecC1[elec].CornerPosY[2];
								R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, SOLID_LINE );
								xmin = ElecC1[elec].CornerPosX[0];
								ymin = ElecC1[elec].CornerPosY[0];
								xmax = ElecC1[elec].CornerPosX[1];
								ymax = ElecC1[elec].CornerPosY[1];
								R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, SOLID_LINE );
								xmin = ElecC1[elec].CornerPosX[3];
								ymin = ElecC1[elec].CornerPosY[3];
								xmax = ElecC12[elec].CornerPosX[1];
								ymax = ElecC12[elec].CornerPosY[1];
								R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, SOLID_LINE );

							} else {
								xmin = ElecEdge[elec][side].dataX[dns];
								ymin = ElecEdge[elec][side].dataY[dns];
								xmax = ElecEdge[elec][side].dataX[dne];
								ymax = ElecEdge[elec][side].dataY[dne];
								xmin = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
								xmax = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
								R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, SOLID_LINE );
							}
							break;
					}
				} else {
					xmin = ElecEdge[elec][side].dataX[dns];
					ymin = ElecEdge[elec][side].dataY[dns];
					xmax = ElecEdge[elec][side].dataX[dne];
					ymax = ElecEdge[elec][side].dataY[dne];
					if((side==PosYmin)||(side==PosYmax)) {
						ymin = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
						ymax = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
					} else {
						xmin = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
						xmax = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
					}
					R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, SOLID_LINE );
				}


				switch( side ) {
					case PosXmin:	// --- Left
						xmin = Elec[elec].CornerPosX[PosLup];	ymin = Elec[elec].CornerPosY[PosLup];
						xmax = Elec[elec].CornerPosX[PosLlo];	ymax = Elec[elec].CornerPosY[PosLlo];
						break;
					case PosYmin:	// --- Top
						xmin = Elec[elec].CornerPosX[PosLup];	ymin = Elec[elec].CornerPosY[PosLup];
						xmax = Elec[elec].CornerPosX[PosRup];	ymax = Elec[elec].CornerPosY[PosRup];
						if(bIsMCR18){
							if(elec == 0){
								xmax = ElecC12[elec].CornerPosX[PosRup];
							} else {
								xmin = ElecC12[elec].CornerPosX[PosLup];
							}
						}
						break;
					case PosXmax:	// --- Right
						xmin = Elec[elec].CornerPosX[PosRup];	ymin = Elec[elec].CornerPosY[PosRup];
						xmax = Elec[elec].CornerPosX[PosRlo];	ymax = Elec[elec].CornerPosY[PosRlo];
						break;
					case PosYmax:	// --- Bottom
						xmin = Elec[elec].CornerPosX[PosLlo];	ymin = Elec[elec].CornerPosY[PosLlo];
						xmax = Elec[elec].CornerPosX[PosRlo];	ymax = Elec[elec].CornerPosY[PosRlo];
						if(bIsMCR18){
							if(elec == 0){
								xmax = ElecC12[elec].CornerPosX[PosRlo];
							} else {
								xmin = ElecC12[elec].CornerPosX[PosLlo];
							}
						}
						break;
				}
				R_DrawLine( DRAW_YELLOW, xmin, ymin, xmax, ymax, DRAW_NORMAL, SOLID_LINE );



				// --- fail pos.
				if( res != OK ) {
					xmin = fxmin = ElecEdge[elec][side].dataX[fns];
					ymin = fymin = ElecEdge[elec][side].dataY[fns];
					xmax = fxmax = ElecEdge[elec][side].dataX[fne];
					ymax = fymax = ElecEdge[elec][side].dataY[fne];
					if((side==PosYmin)||(side==PosYmax)) {
						ymin = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
						ymax = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
						if( side == PosYmin ) {
							fymin = ymin+judgeYsize;
							fymax = ymax+judgeYsize;
						} else {
							fymin = ymin-judgeYsize;
							fymax = ymax-judgeYsize;
						}
					} else {
						xmin = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
						xmax = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
						if( side == PosXmin ) {
							fxmin = xmin+judgeXsize;
							fxmax = xmax+judgeXsize;
						} else {
							fxmin = xmin-judgeXsize;
							fxmax = xmax-judgeXsize;
						}
					}
					R_DrawLine( DRAW_RED, fxmin, fymin, fxmax, fymax, DRAW_NORMAL, SOLID_LINE );
					sprintf( Comment1, "Fail" );
					c_mode = DRAW_RED;
				} else {
					sprintf( Comment1, "Pass" );
					c_mode = DRAW_GREEN;
				}
				// --- massage
				switch( side ) {
					case PosYmin:
					case PosYmax:
						judgeXsizeM	= InspParaM.ElecYedge[TWA_Spec].nXsize;
						judgeYsizeM	= InspParaM.ElecYedge[TWA_Spec].nYsize;
						break;
					case PosXmin:
						if( DataLeft==elec ) {
							judgeXsizeM	= InspParaM.ChipXedge[TWA_Spec].nXsize;
							judgeYsizeM	= InspParaM.ChipXedge[TWA_Spec].nYsize;
						} else {
							judgeXsizeM	= InspParaM.ElecXedge[TWA_Spec].nXsize;
							judgeYsizeM	= InspParaM.ElecXedge[TWA_Spec].nYsize;
						}
						break;
					case PosXmax:
						if( DataRight==elec ) {
							judgeXsizeM	= InspParaM.ChipXedge[TWA_Spec].nXsize;
							judgeYsizeM	= InspParaM.ChipXedge[TWA_Spec].nYsize;
						} else {
							judgeXsizeM	= InspParaM.ElecXedge[TWA_Spec].nXsize;
							judgeYsizeM	= InspParaM.ElecXedge[TWA_Spec].nYsize;
						}
						break;
				}

				lengthXM = (lengthXmax*SystemPara.nRateX+500)/1000;
				lengthYM = (lengthYmax*SystemPara.nRateY+500)/1000;
				posi_res_x = 0;
				posi_res_y = 30;

				R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"[Elec. edge Insp. result [%s]",Comment1);
				R_DrawPrintf(c_mode    , DISP_NORMAL, posi_res_x, posi_res_y+1 ,"position : Elec:%d Side:%d", elec, side);
				R_DrawPrintf(c_mode    , DISP_NORMAL, posi_res_x, posi_res_y+2 ,"Length X : %4dum (%3ddot) ( < %4dum) ", lengthXM,lengthXmax,judgeXsizeM);
				R_DrawPrintf(c_mode    , DISP_NORMAL, posi_res_x, posi_res_y+3 ,"Length Y : %4dum (%3ddot) ( < %4dum) ", lengthYM,lengthYmax,judgeYsizeM);

				while( !R_DrawGetCursor());
				R_DrawCls();
				while( R_DrawGetCursor());
			//	sprintf( Comment, "Elec. edge Insp. result [%s]\n position : Elec:%d Side:%d", Comment1, elec, side );
			//	DrawMessage( Comment, c_mode, 2, 2 );
			}

		// judge end
			return(res);

	}	// JudgeElecEdge( int side ) end


//================================================
//		電極検査
//			検査２値画像、検査範囲設定
//================================================

void	InspBinSetElectrode( void )
	{
		int						grayPn	= InspGrayPn;
		int						binPn	= InspBinPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						side, dn;
		int						xmin, xmax, xs, xe, xsize;
		int						ymin, ymax, ys, ye, ysize;
		int						ymin2,ymax2;
		int						chip_cx, chip_cy;
		int						offset_size1, offset_size2, offset_size7, offset_size8, offset_size10;
		int						cls_w, cls_c;
		int						bak_graph_page	= Graph_page;
		int						ix, iy, rx, ry, dy;
		int						sideX, sideY;
		int						elec;
		R_RECT					area;
        BIN_FILTER_PARAMETER	BinFilter;    // Filter condition

		// Initial
			BinClsPn( binPn, 0, 0, FxSize-1, FySize-1 );
//			offset_size1	= 2;
//			offset_size2	= 5;
			offset_size1	= 5;                        //GEE V1.02A
			if(bIsMCR01==0)		offset_size2	= 5;
			else				offset_size2	= 15;	//20170929 tanaka
			offset_size7	= 7;						//v2.11
			offset_size8	= 8;						//v2.11
			offset_size10	= 10;						//v2.11
		// Chip center
			rx = ry = 0;
			for( dn=0; dn<4; dn++ ) {
				rx += Elec[DataLR].CornerPosX[dn];
				ry += Elec[DataLR].CornerPosY[dn];
			}
			chip_cx = rx/4;
			chip_cy = ry/4;
		// Area
			area.left	= min( Elec[DataLR].CornerPosX[0], Elec[DataLR].CornerPosX[1] ) - 20;
				if( area.left < 0 )			area.left = 0;
			area.right	= max( Elec[DataLR].CornerPosX[2], Elec[DataLR].CornerPosX[3] ) + 20;
				if( area.right >= FxSize )	area.right = FxSize-1;
			area.top	= min( Elec[DataLR].CornerPosY[0], Elec[DataLR].CornerPosY[2] ) - 20;
				if( area.top < 0 )			area.top = 0;
			area.bottom	= max( Elec[DataLR].CornerPosY[1], Elec[DataLR].CornerPosY[3] ) + 20;
				if( area.bottom >= FySize )	area.bottom = FySize-1;

		// Electrode Gray --> Bin
			xsize = area.right-area.left+1;
			ysize = area.bottom-area.top+1;
			R_gray2bin( ElecInspLevel.nData, area.left, area.top, xsize, ysize, Gray_address[grayPn], Bin_address[binPn] );
		// noise clear
			BinFilter.nBinPn = binPn;
			BinFilter.nXmin = area.left;
			BinFilter.nXmax = area.right;
			BinFilter.nYmin = area.top;
			BinFilter.nYmax = area.bottom;
			//v.20 if(!bIsLTR50){
			if( (!bIsLTR50) && (!bIsLTR18) ){
				// x size 5
				FilterBinContrct( &BinFilter, 5, 1 );
				FilterBinExpand(  &BinFilter, 5, 1 );
				// y size 5
				FilterBinContrct( &BinFilter, 1, 5 );
				FilterBinExpand(  &BinFilter, 1, 5 );
				// x size 5
				FilterBinExpand(  &BinFilter, 5, 1 );
				FilterBinContrct( &BinFilter, 5, 1 );
				// y size 5
				FilterBinExpand(  &BinFilter, 1, 5 );
				FilterBinContrct( &BinFilter, 1, 5 );
			} else {
				// y size 5
				FilterBinContrct( &BinFilter, 1, 5 );
				FilterBinExpand(  &BinFilter, 1, 5 );
				// x size 5
				FilterBinContrct( &BinFilter, 5, 1 );
				FilterBinExpand(  &BinFilter, 5, 1 );
				// y size 5
				FilterBinExpand(  &BinFilter, 1, 5 );
				FilterBinContrct( &BinFilter, 1, 5 );
				// x size 5
				FilterBinExpand(  &BinFilter, 5, 1 );
				FilterBinContrct( &BinFilter, 5, 1 );
			}
			// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
				// --- comment
				sprintf( Comment, "Insp.Electrode0 BinP:%d (level:%d)", binPn, ElecInspLevel.nData );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
			}
		// Outside clear
			// Graph page set
				Graph_page = binPn;

				//v2.20 if(!bIsLTR50){
				if( (!bIsLTR50) && (!bIsLTR18) ){
				// upper side
					side = PosYmin;
					for( elec=DataLeft; elec<=DataRight; elec++ ) {
						if( elec == DataLeft ) {
							xmin = area.left;
							xmax = chip_cx;
						} else {
							xmin = chip_cx;
							xmax = area.right;
						}
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
						Elec[elec].Pos[side] = min( ys, ye );
						ys += offset_size1;
						ye += offset_size1;
						cls_w = abs(ys-ye)+10;
						for( dn=0; dn<cls_w; dn++ ) {
							R_line( xmin, ys, xmax, ye, GRAPH_DRAW, SOLID_LINE );
							ys--;
							ye--;
							if( ys < 0 ) ys = 0;
							if( ye < 0 ) ye = 0;
						}
					}
				// lower side
					side = PosYmax;
					for( elec=DataLeft; elec<=DataRight; elec++ ) {
						if( elec == DataLeft ) {
							xmin = area.left;
							xmax = chip_cx;
						} else {
							xmin = chip_cx;
							xmax = area.right;
						}
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
						Elec[elec].Pos[side] = max( ys, ye );
						ys -= offset_size1;
						ye -= offset_size1;
						cls_w = abs(ys-ye)+10;
						for( dn=0; dn<cls_w; dn++ ) {
							R_line( xmin, ys, xmax, ye, GRAPH_DRAW, SOLID_LINE );
							ys++;
							ye++;
							if( ys >= FySize ) ys = FySize-1;
							if( ye >= FySize ) ye = FySize-1;
						}
					}
				// left side
					side = PosXmin;
					ymin = area.top;
					ymax = area.bottom;
					for( elec=DataLeft; elec<=DataRight; elec++ ) {
						xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
						xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
						Elec[elec].Pos[side] = min( xs, xe );
						if( elec == DataLeft ) {
							xs += offset_size1;
							xe += offset_size1;
						} else {
							xs += offset_size2;
							xe += offset_size2;
						}
						cls_w = abs(xs-xe)+10;
						for( dn=0; dn<cls_w; dn++ ) {
							R_line( xs, ymin, xe, ymax, GRAPH_DRAW, SOLID_LINE );
							xs--;
							xe--;
							if( elec == DataLeft ) {
								if( xs < 0 ) xs = 0;
								if( xe < 0 ) xe = 0;
							} else {
								if( xs < chip_cx ) xs = chip_cx;
								if( xe < chip_cx ) xe = chip_cx;
							}
						}

						if(bIsMCR01 && elec ==DataLeft){   // 電極黒ずみマスク 20171004 tanaka
							ymin2 = (area.top+area.bottom)/2 - 45;
							ymax2 = (area.top+area.bottom)/2 + 45;
							xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin2);
							xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax2);
							if( elec == DataLeft ) {
								xs += offset_size1;
								xe += offset_size1;
							} else {
								xs += offset_size2;
								xe += offset_size2;
							}
							for( dn=0; dn<40; dn++ ) {
								R_line( xs, ymin2, xe, ymax2, GRAPH_DRAW, SOLID_LINE );
								xs++;
								xe++;
								if( elec == DataLeft ) {
									if( xs < 0 ) xs = 0;
									if( xe < 0 ) xe = 0;
								} else {
									if( xs < chip_cx ) xs = chip_cx;
									if( xe < chip_cx ) xe = chip_cx;
								}
							}
						}
					}
				// right side
					side = PosXmax;
					ymin = area.top;
					ymax = area.bottom;
					for( elec=DataLeft; elec<=DataRight; elec++ ) {
						xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
						xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
						Elec[elec].Pos[side] = max( xs, xe );
						if( elec == DataLeft ) {
							xs -= offset_size2;
							xe -= offset_size2;
						} else {
							xs -= offset_size1;
							xe -= offset_size1;
						}
						cls_w = abs(xs-xe)+10;
						for( dn=0; dn<cls_w; dn++ ) {
							R_line( xs, ymin, xe, ymax, GRAPH_DRAW, SOLID_LINE );
							xs++;
							xe++;
							if( elec == DataLeft ) {
								if( xs >= chip_cx ) xs = chip_cx;
								if( xe >= chip_cx ) xe = chip_cx;
							} else {
								if( xs >= FxSize ) xs = FxSize-1;
								if( xe >= FxSize ) xe = FxSize-1;
							}
						}
						if(bIsMCR01 && elec ==DataRight){   // 電極黒ずみマスク 20171004 tanaka
							ymin2 = (area.top+area.bottom)/2 - 45;
							ymax2 = (area.top+area.bottom)/2 + 45;
							xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin2);
							xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax2);
							if( elec == DataLeft ) {
								xs -= offset_size2;
								xe -= offset_size2;
							} else {
								xs -= offset_size1;
								xe -= offset_size1;
							}
							for( dn=0; dn<40; dn++ ) {
								R_line( xs, ymin2, xe, ymax2, GRAPH_DRAW, SOLID_LINE );
								xs--;
								xe--;
								if( elec == DataLeft ) {
									if( xs >= chip_cx ) xs = chip_cx;
									if( xe >= chip_cx ) xe = chip_cx;
								} else {
									if( xs >= FxSize ) xs = FxSize-1;
									if( xe >= FxSize ) xe = FxSize-1;
								}
							}
						}
					}

				// Corner
					cls_c = ChipCornerSize[DataXbit];
				// Corner left-upper
					elec = DataLeft;
					sideX = PosXmin;
					sideY = PosYmin;
					ix = Elec[elec].CornerPosX[0];
					iy = Elec[elec].CornerPosY[0];
					for( dn=0; dn<cls_c; dn++ ) {
						ix++;
						iy++;
						rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
						ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-rx);
						R_line( ix, ry, rx, iy, GRAPH_DRAW, SOLID_LINE );
					}
				// Corner left-lower
					elec = DataLeft;
					sideX = PosXmin;
					sideY = PosYmax;
					ix = Elec[elec].CornerPosX[1];
					iy = Elec[elec].CornerPosY[1];
					for( dn=0; dn<cls_c; dn++ ) {
						ix++;
						iy--;
						rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
						ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-rx);
						R_line( ix, ry, rx, iy, GRAPH_DRAW, SOLID_LINE );
					}
				// Corner right-upper
					elec = DataRight;
					sideX = PosXmax;
					sideY = PosYmin;
					ix = Elec[elec].CornerPosX[2];
					iy = Elec[elec].CornerPosY[2];
					for( dn=0; dn<cls_c; dn++ ) {
						ix--;
						iy++;
						rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
						ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-rx);
						R_line( ix, ry, rx, iy, GRAPH_DRAW, SOLID_LINE );
					}
				// Corner right-lower
					elec = DataRight;
					sideX = PosXmax;
					sideY = PosYmax;
					ix = Elec[elec].CornerPosX[3];
					iy = Elec[elec].CornerPosY[3];
					for( dn=0; dn<cls_c; dn++ ) {
						ix--;
						iy--;
						rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-iy);
						ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-rx);
						R_line( ix, ry, rx, iy, GRAPH_DRAW, SOLID_LINE );
					}

					if(bIsMCR18){
					// Corner left-upper
						elec  = DataLeft;
						sideX = PosXmax;
						if(ElecC1[elec].CornerPosY[2]!=0x0ffff){
							dy=ElecC1[elec].CornerPosY[2]-Elec[elec].CornerPosY[2];
							for( dn=0; dn<dy; dn++ ) {
								iy = ElecC12[elec].CornerPosY[2]+dn;
								ix = ElecEdge[elec][5    ].MeanX-ElecEdge[elec][5    ].Rev*(ElecEdge[elec][5    ].MeanY-iy);
								ry = Elec[elec].CornerPosY[2]+dn;
								rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-ry);
								R_line( ix, iy, rx, ry, GRAPH_DRAW, SOLID_LINE );
							}
						}
					// Corner left-lower
						elec = DataLeft;
						sideX = PosXmax;
						if(ElecC1[elec].CornerPosY[3]!=0x0ffff){
							dy=Elec[elec].CornerPosY[3]-ElecC1[elec].CornerPosY[3];
							for( dn=0; dn<dy; dn++ ) {
								iy = ElecC12[elec].CornerPosY[3]-dn;
								ix = ElecEdge[elec][5    ].MeanX-ElecEdge[elec][5    ].Rev*(ElecEdge[elec][5    ].MeanY-iy);
								ry = Elec[elec].CornerPosY[3]-dn;
								rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-ry);
								R_line( ix, iy, rx, ry, GRAPH_DRAW, SOLID_LINE );
							}
						}
					// Corner right-upper
						elec = DataRight;
						sideX = PosXmin;
						if(ElecC1[elec].CornerPosY[0]!=0x0ffff){
							dy=ElecC1[elec].CornerPosY[0]-Elec[elec].CornerPosY[0];
							for( dn=0; dn<dy; dn++ ) {
								iy = ElecC12[elec].CornerPosY[0]+dn;
								ix = ElecEdge[elec][4    ].MeanX-ElecEdge[elec][4    ].Rev*(ElecEdge[elec][4    ].MeanY-iy);
								ry = Elec[elec].CornerPosY[0]+dn;
								rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-ry);
								R_line( ix, iy, rx, ry, GRAPH_DRAW, SOLID_LINE );
							}
						}
					// Corner right-lower
						elec = DataRight;
						sideX = PosXmin;
						if(ElecC1[elec].CornerPosY[1]!=0x0ffff){
							dy=Elec[elec].CornerPosY[1]-ElecC1[elec].CornerPosY[1];
							for( dn=0; dn<dy; dn++ ) {
								iy = ElecC12[elec].CornerPosY[1]-dn;
								ix = ElecEdge[elec][4    ].MeanX-ElecEdge[elec][4    ].Rev*(ElecEdge[elec][4    ].MeanY-iy);
								ry = Elec[elec].CornerPosY[1]-dn;
								rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-ry);
								R_line( ix, iy, rx, ry, GRAPH_DRAW, SOLID_LINE );
							}
						}
					}
				// Graph page set
					Graph_page = bak_graph_page;

				// Draw Electrode bin pn
					if( i_mode & INSPECT_STEP ) {
						R_display_control( BIN_MEMORY | GRAY_MEMORY );
						BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
						R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
						for( elec=DataLeft; elec<=DataRight; elec++ ) {
							area.left	= Elec[elec].Pos[PosXmin];
							area.right	= Elec[elec].Pos[PosXmax];
							area.top	= Elec[elec].Pos[PosYmin];
							area.bottom	= Elec[elec].Pos[PosYmax];
							R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
						}
						sprintf( Comment, "Insp.Electrode1 BinP:%d (level:%d)", binPn, ElecInspLevel.nData );
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
						R_DrawCls();
					}
				} else {	//LTR
				// left side
					side = PosXmin;
					for( elec=DataUpper; elec<=DataLower; elec++ ) {
						if( elec == DataUpper ) {
							ymin = area.top;
							ymax = chip_cy;
						} else {
							ymin = chip_cy;
							ymax = area.bottom;
						}
						xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
						xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
						Elec[elec].Pos[side] = min( xs, xe );
						xs += offset_size1;
						xe += offset_size1;
						cls_w = abs(xs-xe)+10;
						for( dn=0; dn<cls_w; dn++ ) {
							R_line( xs, ymin, xe, ymax, GRAPH_DRAW, SOLID_LINE );
							xs--;
							xe--;
							if( xs < 0 ) xs = 0;
							if( xe < 0 ) xe = 0;
						}
					}
				// right side
					side = PosXmax;
					for( elec=DataUpper; elec<=DataLower; elec++ ) {
						if( elec == DataUpper ) {
							ymin = area.top;
							ymax = chip_cy;
						} else {
							ymin = chip_cy;
							ymax = area.bottom;
						}
						xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
						xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
						Elec[elec].Pos[side] = max( xs, xe );
						xs -= offset_size1;
						xe -= offset_size1;
						cls_w = abs(xs-xe)+10;
						for( dn=0; dn<cls_w; dn++ ) {
							R_line(  xs, ymin, xe, ymax, GRAPH_DRAW, SOLID_LINE );
							xs++;
							xe++;
							if( xs >= FxSize ) xs = FxSize-1;
							if( xe >= FxSize ) xe = FxSize-1;
						}
					}
				// upper side
					side = PosYmin;
					xmin = area.left;
					xmax = area.right;
					for( elec=DataUpper; elec<=DataLower; elec++ ) {
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
						Elec[elec].Pos[side] = min( ys, ye );
						if(0==(Option & BACKINSP_MODE)){					//v2.11
							if( elec == DataUpper ) {
								ys += offset_size1;
								ye += offset_size1;
							} else {
								ys += offset_size2;
								ye += offset_size2;
							}
							cls_w = abs(ys-ye)+10;
						}else{
							if( elec == DataUpper ) {
								ys += offset_size10;
								ye += offset_size10;
							} else {
								ys += offset_size10;
								ye += offset_size10;
							}
							cls_w = abs(ys-ye)+20;
						}
						for( dn=0; dn<cls_w; dn++ ) {
							R_line( xmin, ys, xmax, ye, GRAPH_DRAW, SOLID_LINE );
							ys--;
							ye--;
							if( elec == DataUpper ) {
								if( ys < 0 ) ys = 0;
								if( ye < 0 ) ye = 0;
							} else {
								if( ys < chip_cy ) ys = chip_cy;
								if( ye < chip_cy ) ye = chip_cy;
							}
						}
					}
				// Lower side
					side = PosYmax;
					xmin = area.left;
					xmax = area.right;
					for( elec=DataUpper; elec<=DataLower; elec++ ) {
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
						Elec[elec].Pos[side] = max( ys, ye );
						if(0==(Option & BACKINSP_MODE)){					//v2.11
							if( elec == DataUpper ) {
								ys -= offset_size2;
								ye -= offset_size2;
							} else {
								ys -= offset_size1;
								ye -= offset_size1;
							}
							cls_w = abs(ys-ye)+10;
						}else{
							if( elec == DataUpper ) {
								ys -= offset_size10;
								ye -= offset_size10;
							} else {
								ys -= offset_size10;
								ye -= offset_size10;
							}
							cls_w = abs(ys-ye)+20;
						}
						for( dn=0; dn<cls_w; dn++ ) {
							R_line(  xmin, ys,  xmax, ye, GRAPH_DRAW, SOLID_LINE );
							ys++;
							ye++;
							if( elec == DataUpper ) {
								if( ys >= chip_cy ) ys = chip_cy;
								if( ye >= chip_cy ) ye = chip_cy;
							} else {
								if( ys >= FySize ) ys = FySize-1;
								if( ye >= FySize ) ye = FySize-1;
							}
						}
					}
					{
						xs = (ElecPosRough[DataLR][PosXmin]+ElecPosRough[DataLR][PosXmax])/2-17;
						xe = xs+34;
						ys = ue.kaiki_ey+ue.kaiki_b*(xs-ue.kaiki_ex);
						ye = ue.kaiki_ey+ue.kaiki_b*(xe-ue.kaiki_ex);
						for( dn=0; dn<15; dn++ ) {
							R_line( xs, ys, xe, ye, GRAPH_DRAW, SOLID_LINE );
							ys--;
							ye--;
						}
					}
					{
						xs = (ElecPosRough[DataLR][PosXmin]+ElecPosRough[DataLR][PosXmax])/2-17;
						xe = xs+34;
						ys = shita.kaiki_ey+shita.kaiki_b*(xs-shita.kaiki_ex);
						ye = shita.kaiki_ey+shita.kaiki_b*(xe-shita.kaiki_ex);
						for( dn=0; dn<40; dn++ ) {
							R_line( xs, ys, xe, ye, GRAPH_DRAW, SOLID_LINE );
							ys++;
							ye++;
						}
					}

				// Corner
					cls_c = ChipCornerSize[DataYbit];
				// Corner left-upper
					elec = DataUpper;
					sideX = PosXmin;
					sideY = PosYmin;
					ix = Elec[elec].CornerPosX[0];
					iy = Elec[elec].CornerPosY[0];
					for( dn=0; dn<cls_c; dn++ ) {
						ix++;
						iy++;
						ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-ix);
						rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-ry);
						R_line( ix, ry, rx, iy, GRAPH_DRAW, SOLID_LINE );
					}
				// Corner right-upper
					elec = DataUpper;
					sideX = PosXmax;
					sideY = PosYmin;
					ix = Elec[elec].CornerPosX[2];
					iy = Elec[elec].CornerPosY[2];
					for( dn=0; dn<cls_c; dn++ ) {
						ix--;
						iy++;
						ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-ix);
						rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-ry);
						R_line( ix, ry, rx, iy, GRAPH_DRAW, SOLID_LINE );
					}
				// Corner left-lower
					elec = DataLower;
					sideX = PosXmin;
					sideY = PosYmax;
					ix = Elec[elec].CornerPosX[1];
					iy = Elec[elec].CornerPosY[1];
					for( dn=0; dn<cls_c; dn++ ) {
						ix++;
						iy--;
						ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-ix);
						rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-ry);
						R_line( ix, ry, rx, iy, GRAPH_DRAW, SOLID_LINE );
					}
				// Corner right-lower
					elec = DataLower;
					sideX = PosXmax;
					sideY = PosYmax;
					ix = Elec[elec].CornerPosX[3];
					iy = Elec[elec].CornerPosY[3];
					for( dn=0; dn<cls_c; dn++ ) {
						ix--;
						iy--;
						ry = ElecEdge[elec][sideY].MeanY-ElecEdge[elec][sideY].Rev*(ElecEdge[elec][sideY].MeanX-ix);
						rx = ElecEdge[elec][sideX].MeanX-ElecEdge[elec][sideX].Rev*(ElecEdge[elec][sideX].MeanY-ry);
						R_line( ix, ry, rx, iy, GRAPH_DRAW, SOLID_LINE );
					}

					// Graph page set
					Graph_page = bak_graph_page;

				// Draw Electrode bin pn
					if( i_mode & INSPECT_STEP ) {
						R_display_control( BIN_MEMORY | GRAY_MEMORY );
						BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
						R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
						for( elec=DataUpper; elec<=DataLower; elec++ ) {
							area.left	= Elec[elec].Pos[PosXmin];
							area.right	= Elec[elec].Pos[PosXmax];
							area.top	= Elec[elec].Pos[PosYmin];
							area.bottom	= Elec[elec].Pos[PosYmax];
							R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
						}
						sprintf( Comment, "Insp.Electrode1 BinP:%d (level:%d)", binPn, ElecInspLevel.nData );
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
						R_DrawCls();
					}
				}


	}	// InspBinSetElectrode( void ) end


//================================================
//		Kuro Chip Check 2012.03.28
//================================================

int	InspBinBl( void )
	{
		int						grayPn	= InspGrayPn;
		int						binPn	= InspBinPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						side, dn;
		int						xmin, xmax, xs, xe, xsize;
		int						ymin, ymax, ys, ye, ysize;
		int						chip_cx, chip_cy;
		int						ix, iy, rx, ry;
		int						sideX, sideY;
		int						elec;
		int 					n;
		int                     xl, yl, area_bl[8];
		int                     Kurolevel;
		int                     black_area[8], black_p[8];
		int                     color;
		R_RECT					area;
        R_RECT                  area_b[8];
static	int				C1_Insp_area[10][4];								// C1電極検査範囲[area No.][xs,ys,xe,ye]
static	int				C1_Insp_Result[10];
		int				x_offset = 10;
		int				y_offset_in = 4;
		int				y_offset_out = 4;
		int				x1, x2, x3, x4, x5;
		int				pn;

		// Initial
			BinClsPn( binPn, 0, 0, FxSize-1, FySize-1 );
		// Chip center
			rx = ry = 0;
			for( dn=0; dn<4; dn++ ) {
				rx += Elec[DataLR].CornerPosX[dn];		//same DataUL
				ry += Elec[DataLR].CornerPosY[dn];		//same DataUL
			}
			chip_cx = rx/4;
			chip_cy = ry/4;

			// Area
			area.left	= min( Elec[DataLR].CornerPosX[0], Elec[DataLR].CornerPosX[1] ) - 20;
				if( area.left < 0 )			area.left = 0;
			area.right	= max( Elec[DataLR].CornerPosX[2], Elec[DataLR].CornerPosX[3] ) + 20;
				if( area.right >= FxSize )	area.right = FxSize-1;
			area.top	= min( Elec[DataLR].CornerPosY[0], Elec[DataLR].CornerPosY[2] ) - 20;
				if( area.top < 0 )			area.top = 0;
			area.bottom	= max( Elec[DataLR].CornerPosY[1], Elec[DataLR].CornerPosY[3] ) + 20;
				if( area.bottom >= FySize )	area.bottom = FySize-1;

				//v2.20 if(!bIsLTR50){   //normal
				if( (!bIsLTR50) && (!bIsLTR18) ){   //normal
					// upper side
						side = PosYmin;
						for( elec=DataLeft; elec<=DataRight; elec++ ) {
							if( elec == DataLeft ) {
								xmin = area.left;
								xmax = chip_cx;
							} else {
								xmin = chip_cx;
								xmax = area.right;
							}
							ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
							ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
							Elec[elec].Pos[side] = max( ys, ye );
						}
					// lower side
						side = PosYmax;
						for( elec=DataLeft; elec<=DataRight; elec++ ) {
							if( elec == DataLeft ) {
								xmin = area.left;
								xmax = chip_cx;
							} else {
								xmin = chip_cx;
								xmax = area.right;
							}
							ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
							ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
							Elec[elec].Pos[side] = min( ys, ye );
						}
					// left side
						side = PosXmin;
						ymin = area.top;
						ymax = area.bottom;
						for( elec=DataLeft; elec<=DataRight; elec++ ) {
							xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
							xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
							Elec[elec].Pos[side] = max( xs, xe );
						}
					// right side
						side = PosXmax;
						ymin = area.top;
						ymax = area.bottom;
						for( elec=DataLeft; elec<=DataRight; elec++ ) {
							xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
							xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
							Elec[elec].Pos[side] = min( xs, xe );
						}

				// Draw Electrode bin pn
					area_b[0].left  	= Elec[0].Pos[PosXmin]+5;
					area_b[0].right 	= Elec[0].Pos[PosXmax]-5;
					area_b[0].top		= Elec[0].Pos[PosYmin]+5;
					area_b[0].bottom	= (Elec[0].Pos[PosYmax]+Elec[0].Pos[PosYmin])/2;

					area_b[1].left  	= Elec[0].Pos[PosXmin]+5;
					area_b[1].right 	= Elec[0].Pos[PosXmax]-5;
					area_b[1].top		= (Elec[0].Pos[PosYmax]+Elec[0].Pos[PosYmin])/2;
					area_b[1].bottom	= Elec[0].Pos[PosYmax]-5;

					area_b[2].left  	= Elec[1].Pos[PosXmin]+5;
					area_b[2].right 	= Elec[1].Pos[PosXmax]-5;
					area_b[2].top		= Elec[1].Pos[PosYmin]+5;
					area_b[2].bottom	= (Elec[1].Pos[PosYmax]+Elec[1].Pos[PosYmin])/2;

					area_b[3].left  	= Elec[1].Pos[PosXmin]+5;
					area_b[3].right 	= Elec[1].Pos[PosXmax]-5;
					area_b[3].top		= (Elec[1].Pos[PosYmax]+Elec[1].Pos[PosYmin])/2;
					area_b[3].bottom	= Elec[1].Pos[PosYmax]-5;

					if( i_mode & INSPECT_STEP ) {
						R_display_control( BIN_MEMORY | GRAY_MEMORY );
						BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
						R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
							for( n=0; n<4; n++ ){
								R_DrawBox( DRAW_GREEN, area_b[n], GRAPH_DRAW, SOLID_LINE );
							}
						sprintf( Comment, "Area Kuro Set" );
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
						R_DrawCls();
					}

				for( n=0; n<4; n++ ) {
					//２値化（グレープレーンから２値プレーンへ）
					if(area_b[n].left   < 0       )	area_b[n].left   = 0;
					if(area_b[n].right  > FxSize-1)	area_b[n].right  = FxSize-1;
					if(area_b[n].top    < 0       )	area_b[n].top    = 0;
					if(area_b[n].bottom > FySize-1)	area_b[n].bottom = FySize-1;
					xl = area_b[n].right - area_b[n].left + 1;
					yl = area_b[n].bottom - area_b[n].top + 1;
					area_bl[n] = xl * yl;
					if( area_bl[n] <= 0 ) return ERROR;
					Kurolevel = BlbinSet(area_b[n].left,area_b[n].top,area_b[n].right,area_b[n].bottom,grayPn);
					if(Kurolevel>=120)   Kurolevel = 120;
					if(Kurolevel==ERROR) Kurolevel = 140;
				   // if(Kurolevel>=170)   Kurolevel = 170;
				   // if(Kurolevel==ERROR) Kurolevel = 190;
					R_gray2bin( Kurolevel, area_b[n].left, area_b[n].top, xl, yl, Gray_address[grayPn], Bin_address[binPn] );
					black_area[n] = area_bl[n] - R_count_bin_area( binPn, area_b[n].left, area_b[n].top, area_b[n].right, area_b[n].bottom );
					if( area_bl[n] == 0 ) return ERROR;
					black_p[n] = black_area[n]*100/area_bl[n];

					if( i_mode & INSPECT_STEP ) {
						if( black_p[n] > InspPara.ElecBlack[TWA_Spec].nData1 ) color = DRAW_RED;
						else                                         color = DRAW_GREEN;
						R_move_bin_memory( binPn, BinP0, area_b[n].left, area_b[n].top, area_b[n].right, area_b[n].bottom );
						R_DrawBox( color, area_b[n], GRAPH_DRAW, SOLID_LINE );
						sprintf( Comment1, "Level: %d , Black Area[%d]: %d" ,Kurolevel, n, black_p[n] );
						sprintf( Comment2,"ElecBlackLimit %d", InspPara.ElecBlack[TWA_Spec].nData1 );
						sprintf( Comment, "%s\n%s", Comment1, Comment2 );
						DrawMessage( Comment, color, 2, 2 );
						R_DrawCls();
					}
				}
			} else {	//LTR
//2021.10.15<shio>  from ver9.02(TWA6903)///////////
// 変色チップ検査  KURO AREA setting 2-> 4area separate
			// Upper X position
				x1 = Elec[0].CornerPosX[PosXmin] + x_offset;
				x5 = Elec[0].CornerPosX[PosXmax] - x_offset;
				if( x1 >= x5 ) return ERROR;
				x3 = (x1+x5)/2;
				x2 = (x1+x3)/2;
				x4 = (x3+x5)/2;
				C1_Insp_area[0][PosXmin] = x1;
				C1_Insp_area[0][PosXmax] = C1_Insp_area[1][PosXmin] = x2;
				C1_Insp_area[1][PosXmax] = C1_Insp_area[2][PosXmin] = x3;
				C1_Insp_area[2][PosXmax] = C1_Insp_area[3][PosXmin] = x4;
				C1_Insp_area[3][PosXmax] = x5;
			// Upper Y min position
				for( pn=0; pn<4; pn++ ) {
				//	rx = (C1_Insp_area[pn][PosXmin]+C1_Insp_area[pn][PosXmax])/2;		//from ver9.02(TWA6903)
					rx = C1_Insp_area[pn][PosXmin];								// 2021.10.19<shio>
					ry = ElecEdge[0][PosYmin].MeanY-ElecEdge[0][PosYmin].Rev*(ElecEdge[0][PosYmin].MeanX-rx);
				//	ry = Ap_up.kaiki_ey-Ap_up.kaiki_b*(Ap_up.kaiki_ex-rx);
					C1_Insp_area[pn][PosYmin] = ry + y_offset_out;
				}
			// Upper Y max position
				for( pn=0; pn<4; pn++ ) {
				//	rx = (C1_Insp_area[pn][PosXmin]+C1_Insp_area[pn][PosXmax])/2;
					rx = C1_Insp_area[pn][PosXmax];
					ry = ElecEdge[0][PosYmax].MeanY-ElecEdge[0][PosYmax].Rev*(ElecEdge[0][PosYmax].MeanX-rx);
				//	ry = Ap_up.kaiki_ey-Ap_up.kaiki_b*(Ap_up.kaiki_ex-rx);
					C1_Insp_area[pn][PosYmax] = ry - y_offset_in;
				}
			// Lower X position
				x1 = Elec[1].CornerPosX[PosXmin]+x_offset;
				x5 = Elec[1].CornerPosX[PosXmax]-x_offset;
				if( x1 >= x5 ) return ERROR;
				x3 = (x1+x5)/2;
				x2 = (x1+x3)/2;
				x4 = (x3+x5)/2;
				C1_Insp_area[4][PosXmin] = x1;
				C1_Insp_area[4][PosXmax] = C1_Insp_area[5][PosXmin] = x2;
				C1_Insp_area[5][PosXmax] = C1_Insp_area[6][PosXmin] = x3;
				C1_Insp_area[6][PosXmax] = C1_Insp_area[7][PosXmin] = x4;
				C1_Insp_area[7][PosXmax] = x5;
			// Lower Y min position
				for( pn=4; pn<8; pn++ ) {
				//	rx = (C1_Insp_area[pn][PosXmin]+C1_Insp_area[pn][PosXmax])/2;		//from ver9.02(TWA6903)
					rx = C1_Insp_area[pn][PosXmin];								// 2021.10.19<shio>
					ry = ElecEdge[1][PosYmin].MeanY-ElecEdge[1][PosYmin].Rev*(ElecEdge[1][PosYmin].MeanX-rx);
				//	ry = Ap_up.kaiki_ey-Ap_up.kaiki_b*(Ap_up.kaiki_ex-rx);
					C1_Insp_area[pn][PosYmin] = ry + y_offset_out;
				}
			// Lower Y max position
				for( pn=4; pn<8; pn++ ) {
				//	rx = (C1_Insp_area[pn][PosXmin]+C1_Insp_area[pn][PosXmax])/2;
					rx = C1_Insp_area[pn][PosXmax];
					ry = ElecEdge[1][PosYmax].MeanY-ElecEdge[1][PosYmax].Rev*(ElecEdge[1][PosYmax].MeanX-rx);
				//	ry = Ap_up.kaiki_ey-Ap_up.kaiki_b*(Ap_up.kaiki_ex-rx);
					C1_Insp_area[pn][PosYmax] = ry - y_offset_in;
				}
			// Y position check
//				for( pn=0; pn<8; pn++ ) {
//					if( C1_Insp_area[pn][PosYmin] >= C1_Insp_area[pn][PosYmax] ) {
//						if( pn < 4 ) {
//							C1_Insp_area[pn][PosYmin] -= y_offset_out;
//							C1_Insp_area[pn][PosYmax] += y_offset_in;
//						} else {
//							C1_Insp_area[pn][PosYmin] -= y_offset_in;
//							C1_Insp_area[pn][PosYmax] += y_offset_out;
//						}
//						yl = (C1_Insp_area[pn][PosYmax]-C1_Insp_area[pn][PosYmin])/4;
//						C1_Insp_area[pn][PosYmin] += yl;
//						C1_Insp_area[pn][PosYmax] -= yl;
//					}
//				}
			// Draw Electrode bin pn
				for( pn=0; pn<8; pn++ ) {
					area_b[pn].left  	= C1_Insp_area[pn][PosXmin];
					area_b[pn].right 	= C1_Insp_area[pn][PosXmax];
					area_b[pn].top		= C1_Insp_area[pn][PosYmin];
					area_b[pn].bottom	= C1_Insp_area[pn][PosYmax];
					if(area_b[pn].top >= area_b[pn].bottom)	area_b[pn].bottom = area_b[pn].top + 5;
				}
///////////////////////////////////////////
/*2021.10.15<shio>comment out
					// left side
						side = PosXmin;
						for( elec=DataUpper; elec<=DataLower; elec++ ) {
							if( elec == DataUpper ) {
								ymin = area.top;
								ymax = chip_cy;
							} else {
								ymin = chip_cy;
								ymax = area.bottom;
							}
							xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
							xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
							Elec[elec].Pos[side] = max( xs, xe );
						}
					// right side
						side = PosXmax;
						for( elec=DataUpper; elec<=DataLower; elec++ ) {
							if( elec == DataUpper ) {
								ymin = area.top;
								ymax = chip_cy;
							} else {
								ymin = chip_cy;
								ymax = area.bottom;
							}
							xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
							xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
							Elec[elec].Pos[side] = min( xs, xe );
						}
					// upper side
						side = PosYmin;
						xmin = area.left;
						xmax = area.right;
						for( elec=DataUpper; elec<=DataLower; elec++ ) {
							ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
							ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
							Elec[elec].Pos[side] = max( ys, ye );
						}
					// lower side
						side = PosYmax;
						xmin = area.left;
						xmax = area.right;
						for( elec=DataUpper; elec<=DataLower; elec++ ) {
							ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
							ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
							Elec[elec].Pos[side] = min( ys, ye );
						}

				// Draw Electrode bin pn
					area_b[0].left  	= Elec[0].Pos[PosXmin]+5;
					area_b[0].right 	= (Elec[0].Pos[PosXmax]+Elec[0].Pos[PosXmin])/2;
					area_b[0].top		= Elec[0].Pos[PosYmin]+2;
					area_b[0].bottom	= Elec[0].Pos[PosYmax]-2;
					if(area_b[0].top >= area_b[0].bottom)	area_b[0].bottom = area_b[0].top + 5;

					area_b[1].left  	= (Elec[0].Pos[PosXmax]+Elec[0].Pos[PosXmin])/2;
					area_b[1].right 	= Elec[0].Pos[PosXmax]-5;
					area_b[1].top		= Elec[0].Pos[PosYmin]+2;
					area_b[1].bottom	= Elec[0].Pos[PosYmax]-2;
					if(area_b[1].top >= area_b[1].bottom)	area_b[1].bottom = area_b[1].top + 5;

					area_b[2].left  	= Elec[1].Pos[PosXmin]+5;
					area_b[2].right 	= (Elec[1].Pos[PosXmax]+Elec[1].Pos[PosXmin])/2;
					area_b[2].top		= Elec[1].Pos[PosYmin]+2;
					area_b[2].bottom	= Elec[1].Pos[PosYmax]-2;
					if(area_b[2].top >= area_b[2].bottom)	area_b[2].bottom = area_b[2].top + 5;

					area_b[3].left  	= (Elec[1].Pos[PosXmax]+Elec[1].Pos[PosXmin])/2;
					area_b[3].right 	= Elec[1].Pos[PosXmax]-5;
					area_b[3].top		= Elec[1].Pos[PosYmin]+2;
					area_b[3].bottom	= Elec[1].Pos[PosYmax]-2;
					if(area_b[3].top >= area_b[3].bottom)	area_b[3].bottom = area_b[3].top + 5;
*////////////////////////////////////////////////////////////

					if( i_mode & INSPECT_STEP ) {
						R_display_control( BIN_MEMORY | GRAY_MEMORY );
						BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
						R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
							for( n=0; n<8; n++ ){	   									// area 4-> 8 2021.10.19<shio>
								R_DrawBox( DRAW_GREEN, area_b[n], GRAPH_DRAW, SOLID_LINE );
							}
						sprintf( Comment, "Area Kuro Set" );
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
						R_DrawCls();
					}

				for( n=0; n<8; n++ ) {								// area 4-> 8 2021.10.19<shio>
					//２値化（グレープレーンから２値プレーンへ）
					if(area_b[n].left   < 0       )	area_b[n].left   = 0;
					if(area_b[n].right  > FxSize-1)	area_b[n].right  = FxSize-1;
					if(area_b[n].top    < 0       )	area_b[n].top    = 0;
					if(area_b[n].bottom > FySize-1)	area_b[n].bottom = FySize-1;
					xl = area_b[n].right - area_b[n].left + 1;
					yl = area_b[n].bottom - area_b[n].top + 1;
					area_bl[n] = xl * yl;
					if( area_bl[n] <= 0 ) return ERROR;
					Kurolevel = BlbinSet(area_b[n].left,area_b[n].top,area_b[n].right,area_b[n].bottom,grayPn);
					if(Kurolevel>=120)   Kurolevel = 120;
					if(Kurolevel==ERROR) Kurolevel = 140;
				   // if(Kurolevel>=170)   Kurolevel = 170;
				   // if(Kurolevel==ERROR) Kurolevel = 190;
					R_gray2bin( Kurolevel, area_b[n].left, area_b[n].top, xl, yl, Gray_address[grayPn], Bin_address[binPn] );
					black_area[n] = area_bl[n] - R_count_bin_area( binPn, area_b[n].left, area_b[n].top, area_b[n].right, area_b[n].bottom );
					if( area_bl[n] == 0 ) return ERROR;
					black_p[n] = black_area[n]*100/area_bl[n];

					if( i_mode & INSPECT_STEP ) {
						if( black_p[n] > InspPara.ElecBlack[TWA_Spec].nData1 ) color = DRAW_RED;
						else                                         color = DRAW_GREEN;
						R_move_bin_memory( binPn, BinP0, area_b[n].left, area_b[n].top, area_b[n].right, area_b[n].bottom );
						R_DrawBox( color, area_b[n], GRAPH_DRAW, SOLID_LINE );
						sprintf( Comment1, "Level: %d , Black Area[%d]: %d" ,Kurolevel, n, black_p[n] );
						sprintf( Comment2,"ElecBlackLimit %d", InspPara.ElecBlack[TWA_Spec].nData1 );
						sprintf( Comment, "%s\n%s", Comment1, Comment2 );
						DrawMessage( Comment, color, 2, 2 );
						R_DrawCls();
					}
				}
			}

			//Kuro Judge
			for( n=0; n<8; n++ ) {										// area 4-> 8 2021.10.19<shio>
				if( black_p[n] > InspPara.ElecBlack[TWA_Spec].nData1 ) return ERROR;
			}

			return OK;


	}	// InspBinBl( void ) end

//------------------------------------------------
//	BlbinSet
//	戻り値
//		-1 < 	正常終了
//		ERROR	メモリーERROR
//------------------------------------------------  kuro 2011.06.24

int		BlbinSet( int xs,int ys,int xe,int ye,int grayPn )
	{
		int						Bin;
		int						flag = 0;
		// Initialze
			if( OK == R_bgray_thresholding_open( THRESH_DEPTH_8BIT ))	flag = 1;
		// 明るさのヒストグラム
			if( ERROR == R_get_histgram( xs, ys, xe, ye, Gray_address[grayPn] ) )	return ERROR;
		// ２値化レベルの取得
			Bin = R_bgray_thresholding( Bright, 150, 200,THRESH_OTSU);
			if(flag)	R_bgray_thresholding_close();
			if( Bin < 0 )	return( ERROR );
		// ２値化レベル設定
		// result
			return Bin;

	}	// BinLevelSet( THRESHOLD_PARAMETER *pThresh ) end
//================================================
//		オーバーコート検査
//================================================

int		InspOvercoat( void )
	{
		int						binPn	= InspBinPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						xmin, xmax, xsize, rx, xl, xsizeM;
		int						ymin, ymax, ysize, ry, yl, ysizeM;
		int						side, elec;
		int						gap_x, gap_y;
		int						insp_mode;
		int						d_color;
		int						bn;
		int						res = OK;
		int						color;
		int                     OverCoat_Area[2];       //2011.09.12

		// Overcoat area
			//v2.20 if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0 && bIsLTR50==0){
			if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0 && bIsLTR50==0 && bIsLTR18==0){
				// --- Left upper
				Overcoat.CornerPosX[PosLup] = Elec[DataLeft].CornerPosX[2];
				Overcoat.CornerPosY[PosLup] = Elec[DataLeft].CornerPosY[2];
				// --- Left lower
				Overcoat.CornerPosX[PosLlo] = Elec[DataLeft].CornerPosX[3];
				Overcoat.CornerPosY[PosLlo] = Elec[DataLeft].CornerPosY[3];
				// --- Right upper
				Overcoat.CornerPosX[PosRup] = Elec[DataRight].CornerPosX[0];
				Overcoat.CornerPosY[PosRup] = Elec[DataRight].CornerPosY[0];
				// --- Right lower
				Overcoat.CornerPosX[PosRlo] = Elec[DataRight].CornerPosX[1];
				Overcoat.CornerPosY[PosRlo] = Elec[DataRight].CornerPosY[1];
			} else if( (bIsLTR50) || (bIsLTR18) ){
				// --- Left upper
				Overcoat.CornerPosX[PosLup] = Elec[DataUpper].CornerPosX[1];
				Overcoat.CornerPosY[PosLup] = Elec[DataUpper].CornerPosY[1];
				// --- Left lower
				Overcoat.CornerPosX[PosLlo] = Elec[DataLower].CornerPosX[0];
				Overcoat.CornerPosY[PosLlo] = Elec[DataLower].CornerPosY[0];
				// --- Right upper
				Overcoat.CornerPosX[PosRup] = Elec[DataUpper].CornerPosX[3];
				Overcoat.CornerPosY[PosRup] = Elec[DataUpper].CornerPosY[3];
				// --- Right lower
				Overcoat.CornerPosX[PosRlo] = Elec[DataLower].CornerPosX[2];
				Overcoat.CornerPosY[PosRlo] = Elec[DataLower].CornerPosY[2];
			}
			// --- position
			Overcoat.Pos[PosXmin] = min( Overcoat.CornerPosX[0], Overcoat.CornerPosX[1] );
			Overcoat.Pos[PosXmax] = max( Overcoat.CornerPosX[2], Overcoat.CornerPosX[3] );
			Overcoat.Pos[PosYmin] = min( Overcoat.CornerPosY[0], Overcoat.CornerPosY[2] );
			Overcoat.Pos[PosYmax] = max( Overcoat.CornerPosY[1], Overcoat.CornerPosY[3] );

		// Check Overcoat size
			// --- X size
			xsize = Overcoat.CornerPosX[PosRup] - Overcoat.CornerPosX[PosLup] + 1;
				if(( xsize < InspPara.G2Xmin[TWA_Spec].nXsize )||( xsize > InspPara.G2Xmax[TWA_Spec].nXsize ))	res = NG_G2_X_SIZE;
				if( i_mode & INSPECT_STEP ) {
					xsizeM = (xsize*SystemPara.nRateX+500)/1000;
					sprintf( Comment1, "Overcoat X top   :%3d[bit] %4d[um](%4d-%4d)", xsize, xsizeM, InspParaM.G2Xmin[TWA_Spec].nXsize, InspParaM.G2Xmax[TWA_Spec].nXsize );
				}
			xsize = Overcoat.CornerPosX[PosRlo] - Overcoat.CornerPosX[PosLlo] + 1;
				if(( xsize < InspPara.G2Xmin[TWA_Spec].nXsize )||( xsize > InspPara.G2Xmax[TWA_Spec].nXsize ))	res = NG_G2_X_SIZE;
				if( i_mode & INSPECT_STEP ) {
					xsizeM = (xsize*SystemPara.nRateX+500)/1000;
					sprintf( Comment2, "Overcoat X bottom:%3d[bit] %4d[um](%4d-%4d)", xsize, xsizeM, InspParaM.G2Xmin[TWA_Spec].nXsize, InspParaM.G2Xmax[TWA_Spec].nXsize );
					if( res == OK )	d_color = DRAW_GREEN;
					else			d_color = DRAW_RED;
					sprintf( Comment, "%s\n%s", Comment1, Comment2 );
					DrawMessage( Comment, d_color, 2, 2 );
				}
				if( res != OK )	return( res );
			// --- Y size
			ysize = Overcoat.CornerPosY[PosLlo] - Overcoat.CornerPosY[PosLup] + 1;
				if(( ysize < InspPara.G2Ymin[TWA_Spec].nYsize )||( ysize > InspPara.G2Ymax[TWA_Spec].nYsize ))	res = NG_G2_Y_SIZE;
				if( i_mode & INSPECT_STEP ) {
					ysizeM = (ysize*SystemPara.nRateY+500)/1000;
					sprintf( Comment1, "Overcoat Y left  :%3d[bit] %4d[um](%4d-%4d)", ysize, ysizeM, InspParaM.G2Ymin[TWA_Spec].nYsize, InspParaM.G2Ymax[TWA_Spec].nYsize );
				}
			ysize = Overcoat.CornerPosY[PosRlo] - Overcoat.CornerPosY[PosRup] + 1;
				if(( ysize < InspPara.G2Ymin[TWA_Spec].nYsize )||( ysize > InspPara.G2Ymax[TWA_Spec].nYsize ))	res = NG_G2_Y_SIZE;
				if( i_mode & INSPECT_STEP ) {
					ysizeM = (ysize*SystemPara.nRateY+500)/1000;
					sprintf( Comment2, "Overcoat Y right :%3d[bit] %4d[um](%4d-%4d)", ysize, ysizeM, InspParaM.G2Ymin[TWA_Spec].nYsize, InspParaM.G2Ymax[TWA_Spec].nYsize );
					if( res == OK )	d_color = DRAW_GREEN;
					else			d_color = DRAW_RED;
					sprintf( Comment, "%s\n%s", Comment1, Comment2 );
					DrawMessage( Comment, d_color, 2, 2 );
				}
				if( res != OK )	return( res );

		// Insp. level set
			if( OvercoatInspLevelSet() != OK )	return ERROR;
///			OcoatInspLevel.nData = OcoatInspLevel.nLower;

		// Insp. Bin page set
			InspBinSetOvercoat(); //9499-NOTE

		// Binary measure
			// Type check
				gap_x = InspOcOffsetSize[DataXbit] + 2;
				gap_y = InspOcOffsetSize[DataYbit] + 2;
			// 特徴量演算　演算方式指定
				R_set_meas_configration( &Mpx, BinMeasOcoat.nRun, BinMeasOcoat.nArea, BinMeasOcoat.nColor, BinMeasOcoat.nMode );
			// Measure area
				xmin = Overcoat.Pos[PosXmin];		ymin = Overcoat.Pos[PosYmin];
				xmax = Overcoat.Pos[PosXmax];		ymax = Overcoat.Pos[PosYmax],
				xsize = xmax-xmin+1;				ysize = ymax-ymin+1;
			// Measure
				if( R_measure( &Mpx, binPn, xmin, ymin, xsize, ysize, GROUP2 ) != OK ) {
					R_measure_close( &Mpx );
					R_measure_open( &Mpx, 2000 );
					DrawMessage( "ERROR !!\nR measure", DRAW_RED, 2, 2 );
					return ERROR;
				}
					OverCoat_Area[DataLeft] = OverCoat_Area[DataRight] = 0;              //2011.09.12
			// Inspection
				for( bn=1; bn<=Mpx.bn; bn++ ) {
					if( Mpx.color[bn] == ColorBlack )	continue;
					// Insp. mode initial
						insp_mode = 0;
					// blob Y pos. check
						// --- Y min
						side = PosYmin;
						rx = Mpx.center_x[bn];
						ry = ElecEdge[DataLR][side].MeanY-ElecEdge[DataLR][side].Rev*(ElecEdge[DataLR][side].MeanX-rx);
						yl = Mpx.jmin[bn]-ry;
						if( yl < gap_y )	insp_mode = 1;
						// --- Y max
						side = PosYmax;
						rx = Mpx.center_x[bn];
						ry = ElecEdge[DataLR][side].MeanY-ElecEdge[DataLR][side].Rev*(ElecEdge[DataLR][side].MeanX-rx);
						yl = ry-Mpx.jmax[bn];
						if( yl < gap_y )	insp_mode = 1;
						// --- X min
						elec = DataLeft;
						side = PosXmax;
						ry = Mpx.center_y[bn];
						rx = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ry);
						xl = Mpx.imin[bn]-rx;
						if( xl < gap_x )	insp_mode = 2;
						// --- X max
						elec = DataRight;
						side = PosXmin;
						ry = Mpx.center_y[bn];
						rx = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ry);
						xl = rx-Mpx.imax[bn];
						if( xl < gap_x )	insp_mode = 2;
					// Judge
						res = OK;
                        xsize = Mpx.imax[bn] - Mpx.imin[bn] + 1;
                        ysize = Mpx.jmax[bn] - Mpx.jmin[bn] + 1;
                        if( insp_mode != 0 )	ysize += InspOcOffsetSize[DataYbit];
                        switch( insp_mode ) {
							case 1:			// Overcoat edge X
                                if( ( xsize > InspPara.G2Xedge[TWA_Spec].nXsize )&&
                                    ( ysize > InspPara.G2Xedge[TWA_Spec].nYsize ))		res = InspPara.G2Xedge[TWA_Spec].nCode;
                                if( ( xsize > InspPara.OCoatXYsize[TWA_Spec].nXsize )&&
                                    ( ysize > InspPara.OCoatXYsize[TWA_Spec].nYsize ))	res = InspPara.OCoatXYsize[TWA_Spec].nCode;
								break;
							case 2:			// Overcoat edge Y
                                if( ( xsize > InspPara.G2Yedge[TWA_Spec].nXsize )&&
                                    ( ysize > InspPara.G2Yedge[TWA_Spec].nYsize ))		res = InspPara.G2Yedge[TWA_Spec].nCode;
                                if( ( xsize > InspPara.OCoatXYsize[TWA_Spec].nXsize )&&
                                    ( ysize > InspPara.OCoatXYsize[TWA_Spec].nYsize ))	res = InspPara.OCoatXYsize[TWA_Spec].nCode;
								break;
							case 0:			// Overcoat
							default:
                                if(  xsize > InspPara.OCoatXsize[TWA_Spec].nXsize )		res = InspPara.OCoatXsize[TWA_Spec].nCode;
                                if(  ysize > InspPara.OCoatYsize[TWA_Spec].nYsize )		res = InspPara.OCoatYsize[TWA_Spec].nCode;
                                if(( xsize > InspPara.OCoatXYsize[TWA_Spec].nXsize )&&
                                   ( ysize > InspPara.OCoatXYsize[TWA_Spec].nYsize ))	res = InspPara.OCoatXYsize[TWA_Spec].nCode;
								break;
						}
					// 2011.09.12
						//v2.20 if(!bIsLTR50){
						if( (!bIsLTR50) && (!bIsLTR18) ){
							if( Mpx.center_x[bn] < Center_C ) OverCoat_Area[DataLeft]  += Mpx.area[bn];
							if( Mpx.center_x[bn] > Center_C ) OverCoat_Area[DataRight] += Mpx.area[bn];
						} else {
							if( Mpx.center_y[bn] < Center_C ) OverCoat_Area[DataUpper] += Mpx.area[bn];
							if( Mpx.center_y[bn] > Center_C ) OverCoat_Area[DataLower] += Mpx.area[bn];
						}
					// Draw blob
						if( i_mode & INSPECT_STEP ) {
							// --- data
							xsize = Mpx.imax[bn] - Mpx.imin[bn] + 1;
							ysize = Mpx.jmax[bn] - Mpx.jmin[bn] + 1;
							if( insp_mode != 0 )	ysize += InspOcOffsetSize[DataYbit];
							// --- color
							if( insp_mode == 0 )	d_color = DRAW_GREEN;
							else					d_color = DRAW_YELLOW;
							if( res != OK )			d_color = DRAW_RED;
							// --- X line
							ry		= Mpx.center_y[bn];
							xmin	= Mpx.imin[bn];
							xmax	= Mpx.imax[bn];
							R_DrawLine( d_color, xmin, ry, xmax, ry, DRAW_NORMAL, SOLID_LINE );
							// --- Y line
							rx		= Mpx.center_x[bn];
							ymin	= Mpx.jmin[bn];
							ymax	= Mpx.jmax[bn];
							R_DrawLine( d_color, rx, ymin, rx, ymax, DRAW_NORMAL, SOLID_LINE );
							// --- massage
							xsizeM = (xsize*SystemPara.nRateX+500)/1000;
							ysizeM = (ysize*SystemPara.nRateY+500)/1000;

							color = DRAW_WHITE;
							if(res != OK)	color = DRAW_RED;
							R_DrawPrintf(DRAW_WHITE,DISP_NORMAL,0,30,"[Inspection Overcoat ]");
							R_DrawPrintf( color, DISP_NORMAL, 3, 31, "Measure X:%4d(bit) Y:%4d(bit)", xsize, ysize );
							R_DrawPrintf( color, DISP_NORMAL, 3, 32, "        X:%4d(um)  Y:%4d(um) ", xsizeM, ysizeM );
							R_DrawPrintf( color, DISP_NORMAL, 3, 33, "insp_mode: %d", insp_mode );
							R_DrawPrintf( color, DISP_NORMAL, 3, 34, "Ocoat   MaxLimit X:%4d(bit)  Y:%4d(bit) ", InspPara.OCoatXsize[TWA_Spec].nXsize, InspPara.OCoatYsize[TWA_Spec].nYsize );
							R_DrawPrintf( color, DISP_NORMAL, 3, 35, "G2Xedge MaxLimit X:%4d(bit)  Y:%4d(bit) ", InspPara.G2Xedge[TWA_Spec].nXsize, InspPara.G2Xedge[TWA_Spec].nYsize );
							R_DrawPrintf( color, DISP_NORMAL, 3, 36, "G2Yedge MaxLimit X:%4d(bit)  Y:%4d(bit) ", InspPara.G2Yedge[TWA_Spec].nXsize, InspPara.G2Yedge[TWA_Spec].nYsize );
							R_DrawPrintf( color, DISP_NORMAL, 3, 37, "OcoatXY MaxLimit X:%4d(bit)  Y:%4d(bit) ", InspPara.OCoatXYsize[TWA_Spec].nXsize, InspPara.OCoatXYsize[TWA_Spec].nYsize );
							R_DrawMouseDeYesno();
						}
					// result
						if( res != OK )	break;
				}

					//2011.09.12
						if( OverCoat_Area[DataLeft]  > (InspPara.OCoatXYsize[TWA_Spec].nXsize * InspPara.OCoatXYsize[TWA_Spec].nYsize)){
							// Draw blob
								if( i_mode & INSPECT_STEP ){
									//v2.20 if(!bIsLTR50)	sprintf( Comment, "OverCoat Area sum Left  Side:%4d(bit) > Std:%4d(bit)", OverCoat_Area[DataLeft],  (InspPara.OCoatXYsize[TWA_Spec].nXsize * InspPara.OCoatXYsize[TWA_Spec].nYsize));
									if((!bIsLTR50)&&(!bIsLTR18))	sprintf( Comment, "OverCoat Area sum Left  Side:%4d(bit) > Std:%4d(bit)", OverCoat_Area[DataLeft],  (InspPara.OCoatXYsize[TWA_Spec].nXsize * InspPara.OCoatXYsize[TWA_Spec].nYsize));
									else			sprintf( Comment, "OverCoat Area sum Upper Side:%4d(bit) > Std:%4d(bit)", OverCoat_Area[DataUpper], (InspPara.OCoatXYsize[TWA_Spec].nXsize * InspPara.OCoatXYsize[TWA_Spec].nYsize));
									DrawMessage( Comment, DRAW_RED, 2, 2 );
								}
									res = InspPara.OCoatXYsize[TWA_Spec].nCode;  //2011.09.12
						}
						if( OverCoat_Area[DataRight] > (InspPara.OCoatXYsize[TWA_Spec].nXsize * InspPara.OCoatXYsize[TWA_Spec].nYsize)){
							// Draw blob
								if( i_mode & INSPECT_STEP ){
									//v2.20 if(!bIsLTR50)	sprintf( Comment, "OverCoat Area sum Right Side:%4d(bit) > Std:%4d(bit)", OverCoat_Area[DataRight], (InspPara.OCoatXYsize[TWA_Spec].nXsize * InspPara.OCoatXYsize[TWA_Spec].nYsize));
									if((!bIsLTR50)&&(!bIsLTR18))	sprintf( Comment, "OverCoat Area sum Right Side:%4d(bit) > Std:%4d(bit)", OverCoat_Area[DataRight], (InspPara.OCoatXYsize[TWA_Spec].nXsize * InspPara.OCoatXYsize[TWA_Spec].nYsize));
									else			sprintf( Comment, "OverCoat Area sum Lower Side:%4d(bit) > Std:%4d(bit)", OverCoat_Area[DataLower], (InspPara.OCoatXYsize[TWA_Spec].nXsize * InspPara.OCoatXYsize[TWA_Spec].nYsize));
									DrawMessage( Comment, DRAW_RED, 2, 2 );
								}
									res = InspPara.OCoatXYsize[TWA_Spec].nCode;  //2011.09.12
						}
			// Inspection end
				return( res );

	}	// InspOvercoat( void ) end

//================================================
//		オーバーコート検査				v2.11
//================================================

int		InspOvercoatBlack( void )
	{
		int						binPn	= InspBinPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						xmin, xmax, xsize, rx, xl, xsizeM;
		int						ymin, ymax, ysize, ry, yl, ysizeM;
		int						side, elec;
		int						gap_x, gap_y;
		int						insp_mode;
		int						d_color;
		int						bn;
		int						res = OK;
		int						color;
		int                     OverCoat_Area[2];       //2011.09.12

		// Overcoat area
			//v2.20 if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0 && bIsLTR50==0){
			if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0 && bIsLTR50==0 && bIsLTR18==0){
				// --- Left upper
				Overcoat.CornerPosX[PosLup] = Elec[DataLeft].CornerPosX[2];
				Overcoat.CornerPosY[PosLup] = Elec[DataLeft].CornerPosY[2];
				// --- Left lower
				Overcoat.CornerPosX[PosLlo] = Elec[DataLeft].CornerPosX[3];
				Overcoat.CornerPosY[PosLlo] = Elec[DataLeft].CornerPosY[3];
				// --- Right upper
				Overcoat.CornerPosX[PosRup] = Elec[DataRight].CornerPosX[0];
				Overcoat.CornerPosY[PosRup] = Elec[DataRight].CornerPosY[0];
				// --- Right lower
				Overcoat.CornerPosX[PosRlo] = Elec[DataRight].CornerPosX[1];
				Overcoat.CornerPosY[PosRlo] = Elec[DataRight].CornerPosY[1];
			//v2.20 } else if(bIsLTR50){
			} else if( (bIsLTR50) || (bIsLTR18) ){
				// --- Left upper
				Overcoat.CornerPosX[PosLup] = Elec[DataUpper].CornerPosX[1];
				Overcoat.CornerPosY[PosLup] = Elec[DataUpper].CornerPosY[1];
				// --- Left lower
				Overcoat.CornerPosX[PosLlo] = Elec[DataLower].CornerPosX[0];
				Overcoat.CornerPosY[PosLlo] = Elec[DataLower].CornerPosY[0];
				// --- Right upper
				Overcoat.CornerPosX[PosRup] = Elec[DataUpper].CornerPosX[3];
				Overcoat.CornerPosY[PosRup] = Elec[DataUpper].CornerPosY[3];
				// --- Right lower
				Overcoat.CornerPosX[PosRlo] = Elec[DataLower].CornerPosX[2];
				Overcoat.CornerPosY[PosRlo] = Elec[DataLower].CornerPosY[2];
			}
			// --- position
			Overcoat.Pos[PosXmin] = min( Overcoat.CornerPosX[0], Overcoat.CornerPosX[1] );
			Overcoat.Pos[PosXmax] = max( Overcoat.CornerPosX[2], Overcoat.CornerPosX[3] );
			Overcoat.Pos[PosYmin] = min( Overcoat.CornerPosY[0], Overcoat.CornerPosY[2] );
			Overcoat.Pos[PosYmax] = max( Overcoat.CornerPosY[1], Overcoat.CornerPosY[3] );

		// Check Overcoat size
			// --- X size
			xsize = Overcoat.CornerPosX[PosRup] - Overcoat.CornerPosX[PosLup] + 1;
				if(( xsize < InspPara.G2Xmin[TWA_Spec].nXsize )||( xsize > InspPara.G2Xmax[TWA_Spec].nXsize ))	res = NG_G2_X_SIZE;
				if( i_mode & INSPECT_STEP ) {
					xsizeM = (xsize*SystemPara.nRateX+500)/1000;
					sprintf( Comment1, "Overcoat X top   :%3d[bit] %4d[um](%4d-%4d)", xsize, xsizeM, InspParaM.G2Xmin[TWA_Spec].nXsize, InspParaM.G2Xmax[TWA_Spec].nXsize );
				}
			xsize = Overcoat.CornerPosX[PosRlo] - Overcoat.CornerPosX[PosLlo] + 1;
				if(( xsize < InspPara.G2Xmin[TWA_Spec].nXsize )||( xsize > InspPara.G2Xmax[TWA_Spec].nXsize ))	res = NG_G2_X_SIZE;
				if( i_mode & INSPECT_STEP ) {
					xsizeM = (xsize*SystemPara.nRateX+500)/1000;
					sprintf( Comment2, "Overcoat X bottom:%3d[bit] %4d[um](%4d-%4d)", xsize, xsizeM, InspParaM.G2Xmin[TWA_Spec].nXsize, InspParaM.G2Xmax[TWA_Spec].nXsize );
					if( res == OK )	d_color = DRAW_GREEN;
					else			d_color = DRAW_RED;
					sprintf( Comment, "%s\n%s", Comment1, Comment2 );
					DrawMessage( Comment, d_color, 2, 2 );
				}
				if( res != OK )	return( res );
			// --- Y size
			ysize = Overcoat.CornerPosY[PosLlo] - Overcoat.CornerPosY[PosLup] + 1;
				if(( ysize < InspPara.G2Ymin[TWA_Spec].nYsize )||( ysize > InspPara.G2Ymax[TWA_Spec].nYsize ))	res = NG_G2_Y_SIZE;
				if( i_mode & INSPECT_STEP ) {
					ysizeM = (ysize*SystemPara.nRateY+500)/1000;
					sprintf( Comment1, "Overcoat Y left  :%3d[bit] %4d[um](%4d-%4d)", ysize, ysizeM, InspParaM.G2Ymin[TWA_Spec].nYsize, InspParaM.G2Ymax[TWA_Spec].nYsize );
				}
			ysize = Overcoat.CornerPosY[PosRlo] - Overcoat.CornerPosY[PosRup] + 1;
				if(( ysize < InspPara.G2Ymin[TWA_Spec].nYsize )||( ysize > InspPara.G2Ymax[TWA_Spec].nYsize ))	res = NG_G2_Y_SIZE;
				if( i_mode & INSPECT_STEP ) {
					ysizeM = (ysize*SystemPara.nRateY+500)/1000;
					sprintf( Comment2, "Overcoat Y right :%3d[bit] %4d[um](%4d-%4d)", ysize, ysizeM, InspParaM.G2Ymin[TWA_Spec].nYsize, InspParaM.G2Ymax[TWA_Spec].nYsize );
					if( res == OK )	d_color = DRAW_GREEN;
					else			d_color = DRAW_RED;
					sprintf( Comment, "%s\n%s", Comment1, Comment2 );
					DrawMessage( Comment, d_color, 2, 2 );
				}
				if( res != OK )	return( res );

		// Insp. level set
			if( OvercoatInspLevelSetBlack() != OK )	return ERROR;
///			OcoatInspLevel.nData = OcoatInspLevel.nLower;

		// Insp. Bin page set
			InspBinSetOvercoatBlack();

		// Binary measure
			// Type check
				gap_x = InspOcOffsetSizeBlack[DataXbit] + 2;
				gap_y = InspOcOffsetSizeBlack[DataYbit] + 2;
			// 特徴量演算　演算方式指定
				R_set_meas_configration( &Mpx, BinMeasOcoat.nRun, BinMeasOcoat.nArea, BinMeasOcoat.nColor, BinMeasOcoat.nMode );
			// Measure area
				xmin = Overcoat.Pos[PosXmin];		ymin = Overcoat.Pos[PosYmin];
				xmax = Overcoat.Pos[PosXmax];		ymax = Overcoat.Pos[PosYmax],
				xsize = xmax-xmin+1;				ysize = ymax-ymin+1;
			// Measure
				if( R_measure( &Mpx, binPn, xmin, ymin, xsize, ysize, GROUP2 ) != OK ) {
					R_measure_close( &Mpx );
					R_measure_open( &Mpx, 2000 );
					DrawMessage( "ERROR !!\nR measure", DRAW_RED, 2, 2 );
					return ERROR;
				}
					OverCoat_Area[DataLeft] = OverCoat_Area[DataRight] = 0;              //2011.09.12
			// Inspection
				for( bn=1; bn<=Mpx.bn; bn++ ) {
					if( Mpx.color[bn] == ColorWhite )	continue;
					// Insp. mode initial
						insp_mode = 0;
					// blob Y pos. check
						// --- Y min
						side = PosYmin;
						rx = Mpx.center_x[bn];
						ry = ElecEdge[DataLR][side].MeanY-ElecEdge[DataLR][side].Rev*(ElecEdge[DataLR][side].MeanX-rx);
						yl = Mpx.jmin[bn]-ry;
						if( yl < gap_y )	insp_mode = 1;
						// --- Y max
						side = PosYmax;
						rx = Mpx.center_x[bn];
						ry = ElecEdge[DataLR][side].MeanY-ElecEdge[DataLR][side].Rev*(ElecEdge[DataLR][side].MeanX-rx);
						yl = ry-Mpx.jmax[bn];
						if( yl < gap_y )	insp_mode = 1;
						// --- X min
						elec = DataLeft;
						side = PosXmax;
						ry = Mpx.center_y[bn];
						rx = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ry);
						xl = Mpx.imin[bn]-rx;
						if( xl < gap_x )	insp_mode = 2;
						// --- X max
						elec = DataRight;
						side = PosXmin;
						ry = Mpx.center_y[bn];
						rx = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ry);
						xl = rx-Mpx.imax[bn];
						if( xl < gap_x )	insp_mode = 2;
					// Judge
						res = OK;
                        xsize = Mpx.imax[bn] - Mpx.imin[bn] + 1;
                        ysize = Mpx.jmax[bn] - Mpx.jmin[bn] + 1;
                        if( insp_mode != 0 )	ysize += InspOcOffsetSizeBlack[DataYbit];
                        switch( insp_mode ) {
							case 1:			// Overcoat edge X
                                if( ( xsize > InspPara.G2Xedge[TWA_Spec].nXsize )&&
                                    ( ysize > InspPara.G2Xedge[TWA_Spec].nYsize ))		res = InspPara.G2Xedge[TWA_Spec].nCode;
                                if( ( xsize > InspPara.OCoatXYsize[TWA_Spec].nXsize )&&
                                    ( ysize > InspPara.OCoatXYsize[TWA_Spec].nYsize ))	res = InspPara.OCoatXYsize[TWA_Spec].nCode;
								break;
							case 2:			// Overcoat edge Y
                                if( ( xsize > InspPara.G2Yedge[TWA_Spec].nXsize )&&
                                    ( ysize > InspPara.G2Yedge[TWA_Spec].nYsize ))		res = InspPara.G2Yedge[TWA_Spec].nCode;
                                if( ( xsize > InspPara.OCoatXYsize[TWA_Spec].nXsize )&&
                                    ( ysize > InspPara.OCoatXYsize[TWA_Spec].nYsize ))	res = InspPara.OCoatXYsize[TWA_Spec].nCode;
								break;
							case 0:			// Overcoat
							default:
                                if(  xsize > InspPara.OCoatXsize[TWA_Spec].nXsize )		res = InspPara.OCoatXsize[TWA_Spec].nCode;
                                if(  ysize > InspPara.OCoatYsize[TWA_Spec].nYsize )		res = InspPara.OCoatYsize[TWA_Spec].nCode;
                                if(( xsize > InspPara.OCoatXYsize[TWA_Spec].nXsize )&&
                                   ( ysize > InspPara.OCoatXYsize[TWA_Spec].nYsize ))	res = InspPara.OCoatXYsize[TWA_Spec].nCode;
								break;
						}
					// 2011.09.12
						//v2.20 if(!bIsLTR50){
						if( (!bIsLTR50) && (!bIsLTR18) ){
							if( Mpx.center_x[bn] < Center_C ) OverCoat_Area[DataLeft]  += Mpx.area[bn];
							if( Mpx.center_x[bn] > Center_C ) OverCoat_Area[DataRight] += Mpx.area[bn];
						} else {
							if( Mpx.center_y[bn] < Center_C ) OverCoat_Area[DataUpper] += Mpx.area[bn];
							if( Mpx.center_y[bn] > Center_C ) OverCoat_Area[DataLower] += Mpx.area[bn];
						}
					// Draw blob
						if( i_mode & INSPECT_STEP ) {
							// --- data
							xsize = Mpx.imax[bn] - Mpx.imin[bn] + 1;
							ysize = Mpx.jmax[bn] - Mpx.jmin[bn] + 1;
							if( insp_mode != 0 )	ysize += InspOcOffsetSizeBlack[DataYbit];
							// --- color
							if( insp_mode == 0 )	d_color = DRAW_GREEN;
							else					d_color = DRAW_YELLOW;
							if( res != OK )			d_color = DRAW_RED;
							// --- X line
							ry		= Mpx.center_y[bn];
							xmin	= Mpx.imin[bn];
							xmax	= Mpx.imax[bn];
							R_DrawLine( d_color, xmin, ry, xmax, ry, DRAW_NORMAL, SOLID_LINE );
							// --- Y line
							rx		= Mpx.center_x[bn];
							ymin	= Mpx.jmin[bn];
							ymax	= Mpx.jmax[bn];
							R_DrawLine( d_color, rx, ymin, rx, ymax, DRAW_NORMAL, SOLID_LINE );
							// --- massage
							xsizeM = (xsize*SystemPara.nRateX+500)/1000;
							ysizeM = (ysize*SystemPara.nRateY+500)/1000;

							color = DRAW_WHITE;
							if(res != OK)	color = DRAW_RED;
							R_DrawPrintf(DRAW_WHITE,DISP_NORMAL,0,30,"[Inspection Overcoat Black]");
							R_DrawPrintf( color, DISP_NORMAL, 3, 31, "Measure X:%4d(bit) Y:%4d(bit)", xsize, ysize );
							R_DrawPrintf( color, DISP_NORMAL, 3, 32, "        X:%4d(um)  Y:%4d(um) ", xsizeM, ysizeM );
							R_DrawPrintf( color, DISP_NORMAL, 3, 33, "insp_mode: %d", insp_mode );
							R_DrawPrintf( color, DISP_NORMAL, 3, 34, "Ocoat   MaxLimit X:%4d(bit)  Y:%4d(bit) ", InspPara.OCoatXsize[TWA_Spec].nXsize, InspPara.OCoatYsize[TWA_Spec].nYsize );
							R_DrawPrintf( color, DISP_NORMAL, 3, 35, "G2Xedge MaxLimit X:%4d(bit)  Y:%4d(bit) ", InspPara.G2Xedge[TWA_Spec].nXsize, InspPara.G2Xedge[TWA_Spec].nYsize );
							R_DrawPrintf( color, DISP_NORMAL, 3, 36, "G2Yedge MaxLimit X:%4d(bit)  Y:%4d(bit) ", InspPara.G2Yedge[TWA_Spec].nXsize, InspPara.G2Yedge[TWA_Spec].nYsize );
							R_DrawPrintf( color, DISP_NORMAL, 3, 37, "OcoatXY MaxLimit X:%4d(bit)  Y:%4d(bit) ", InspPara.OCoatXYsize[TWA_Spec].nXsize, InspPara.OCoatXYsize[TWA_Spec].nYsize );
							R_DrawMouseDeYesno();
							R_DrawPrintf(DRAW_WHITE,DISP_NORMAL,0,30,"                              ");
							R_DrawPrintf( color, DISP_NORMAL, 3, 31, "                                                        ");
							R_DrawPrintf( color, DISP_NORMAL, 3, 32, "                                                        ");
							R_DrawPrintf( color, DISP_NORMAL, 3, 33, "                                                        ");
							R_DrawPrintf( color, DISP_NORMAL, 3, 34, "                                                        ");
							R_DrawPrintf( color, DISP_NORMAL, 3, 35, "                                                        ");
							R_DrawPrintf( color, DISP_NORMAL, 3, 36, "                                                        ");
							R_DrawPrintf( color, DISP_NORMAL, 3, 37, "                                                        ");
						}
					// result
						if( res != OK )	break;
				}

					//2011.09.12
						if( OverCoat_Area[DataLeft]  > (InspPara.OCoatXYsize[TWA_Spec].nXsize * InspPara.OCoatXYsize[TWA_Spec].nYsize)){
							// Draw blob
								if( i_mode & INSPECT_STEP ){
									//v2.20 if(!bIsLTR50)	sprintf( Comment, "OverCoat Area sum Left  Side:%4d(bit) > Std:%4d(bit)", OverCoat_Area[DataLeft],  (InspPara.OCoatXYsize[TWA_Spec].nXsize * InspPara.OCoatXYsize[TWA_Spec].nYsize));
									if((!bIsLTR50)&&(!bIsLTR18))	sprintf( Comment, "OverCoat Area sum Left  Side:%4d(bit) > Std:%4d(bit)", OverCoat_Area[DataLeft],  (InspPara.OCoatXYsize[TWA_Spec].nXsize * InspPara.OCoatXYsize[TWA_Spec].nYsize));
									else			sprintf( Comment, "OverCoat Area sum Upper Side:%4d(bit) > Std:%4d(bit)", OverCoat_Area[DataUpper], (InspPara.OCoatXYsize[TWA_Spec].nXsize * InspPara.OCoatXYsize[TWA_Spec].nYsize));
									DrawMessage( Comment, DRAW_RED, 2, 2 );
								}
									res = InspPara.OCoatXYsize[TWA_Spec].nCode;  //2011.09.12
						}
						if( OverCoat_Area[DataRight] > (InspPara.OCoatXYsize[TWA_Spec].nXsize * InspPara.OCoatXYsize[TWA_Spec].nYsize)){
							// Draw blob
								if( i_mode & INSPECT_STEP ){
									//v2.20 if(!bIsLTR50)	sprintf( Comment, "OverCoat Area sum Right Side:%4d(bit) > Std:%4d(bit)", OverCoat_Area[DataRight], (InspPara.OCoatXYsize[TWA_Spec].nXsize * InspPara.OCoatXYsize[TWA_Spec].nYsize));
									if((!bIsLTR50)&&(!bIsLTR18))	sprintf( Comment, "OverCoat Area sum Right Side:%4d(bit) > Std:%4d(bit)", OverCoat_Area[DataRight], (InspPara.OCoatXYsize[TWA_Spec].nXsize * InspPara.OCoatXYsize[TWA_Spec].nYsize));
									else			sprintf( Comment, "OverCoat Area sum Lower Side:%4d(bit) > Std:%4d(bit)", OverCoat_Area[DataLower], (InspPara.OCoatXYsize[TWA_Spec].nXsize * InspPara.OCoatXYsize[TWA_Spec].nYsize));
									DrawMessage( Comment, DRAW_RED, 2, 2 );
								}
									res = InspPara.OCoatXYsize[TWA_Spec].nCode;  //2011.09.12
						}
			// Inspection end
				return( res );

	}	// InspOvercoatBlack( void ) end


//================================================
//		オーバーコート検査検査レベル設定
//================================================

int		OvercoatInspLevelSet( void )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						thr;
//		int						offset_l = 20;
		int						xmin, xmax;
		int						ymin, ymax, ysize;
		int						i,j;
		R_RECT					mask;
		R_RECT					area;

		// Insp. bin level set
			// --- Gray memory
			InspOcLSetPara.nGrayPn = grayPn;
			// --- area
			//v2.20 if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0 && bIsLTR50==0){
			if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0 && bIsLTR50==0 && bIsLTR18==0){
				xmin	= max( max(	Elec[DataLeft].CornerPosX[PosLup]  , Elec[DataLeft].CornerPosX[PosLlo])
							,  max( Elec[DataLeft].CornerPosX[PosRup]  , Elec[DataLeft].CornerPosX[PosRlo])) + 5;
				xmax	= min( min(	Elec[DataRight].CornerPosX[PosLup] , Elec[DataRight].CornerPosX[PosLlo])
							,  min( Elec[DataRight].CornerPosX[PosRup] , Elec[DataRight].CornerPosX[PosRlo])) - 5;

				ymin	= max( max(	Elec[DataLeft].CornerPosY[PosLup]  , Elec[DataLeft].CornerPosY[PosRup])
							 , max(	Elec[DataRight].CornerPosY[PosLup] , Elec[DataRight].CornerPosY[PosRup])) + 5;
				ymax	= min( min(	Elec[DataLeft].CornerPosY[PosLlo] , Elec[DataLeft].CornerPosY[PosRlo])
							 , min(	Elec[DataRight].CornerPosY[PosLlo] , Elec[DataRight].CornerPosY[PosRlo])) - 5;
			//v2.20 } else if(bIsLTR50){
			} else if( (bIsLTR50) || (bIsLTR18) ){
				xmin	= max( max(	Elec[DataUpper].CornerPosX[PosLup] , Elec[DataUpper].CornerPosX[PosLlo])
							,  max( Elec[DataLower].CornerPosX[PosLup] , Elec[DataLower].CornerPosX[PosLlo])) + 5;
				xmax	= min( min(	Elec[DataUpper].CornerPosX[PosRup] , Elec[DataUpper].CornerPosX[PosRlo])
							,  min( Elec[DataLower].CornerPosX[PosRup] , Elec[DataLower].CornerPosX[PosRlo])) - 5;

				ymin	= max( max(	Elec[DataUpper].CornerPosY[PosLup] , Elec[DataUpper].CornerPosY[PosRup])
							 , max(	Elec[DataUpper].CornerPosY[PosLlo] , Elec[DataUpper].CornerPosY[PosRlo])) + 5;
				ymax	= min( min(	Elec[DataLower].CornerPosY[PosLup] , Elec[DataLower].CornerPosY[PosRup])
							 , min(	Elec[DataLower].CornerPosY[PosLlo] , Elec[DataLower].CornerPosY[PosRlo])) - 5;
			} else {
				for(i=0;i<80;i++){
					if(G2_data_u[i][0]==0xffff)	break;
				}
				if(i!=0)	i--;
				for(j=0;j<80;j++){
					if(G2_data_d[j][0]==0xffff)	break;
				}
				if(j!=0)	j--;
				xmin	= max(	G2_data_u[0][0]  , G2_data_d[0][0] ) +5;
				if(xmin > FxSize-1)   xmin =FxSize-1;
				xmax	= min(  G2_data_u[i][0]  , G2_data_d[j][0] ) -5;
				if(xmax < 0       )	  xmax =0;

				ymin	= max(	G2_data_u[0][1]  , G2_data_u[i][1] ) +5;
				if(ymin > FySize-1)   ymin =FySize-1;
				ymax	= min(  G2_data_d[0][1]  , G2_data_d[j][1] ) -5;
				if(ymax < 0       )	  ymax =0;
			}



			ysize = (ymax-ymin+1)/4;
			InspOcLSetPara.nXmin = xmin;
			InspOcLSetPara.nXmax = xmax;
		//	InspOcLSetPara.nYmin = ymin+ysize;
		//	InspOcLSetPara.nYmax = ymax-ysize;
			InspOcLSetPara.nYmin = ymin;
			InspOcLSetPara.nYmax = ymax;

/*******************
			InspOcLSetPara.nXmin = Overcoat.Pos[PosXmin]+offset_l;
			InspOcLSetPara.nXmax = Overcoat.Pos[PosXmax]-offset_l;
			InspOcLSetPara.nYmin = Overcoat.Pos[PosYmin]+offset_l;
			InspOcLSetPara.nYmax = Overcoat.Pos[PosYmax]-offset_l;
*******************/
			// ---
			InspOcLSetPara.nStartLevel	= 10;									// ヒストグラムの計算範囲の開始点(start>=0)
			InspOcLSetPara.nEndLevel	= 250;									// ヒストグラムの計算範囲の終了点
            InspOcLSetPara.nMethod		= THRESH_OTSU;							// しきい値選定の方法
			// --- level set
			//標印検査有り無し
			if( Ma_pos[0] < 0 || TWA_Mark_Insp != 0 || (TWA_Spec!=SpecJPW)&&(TWA_Insp_mode==Insp_Pinhole)){ 		//メッキ付着検査
				thr = BinLevelSet( &InspOcLSetPara );
			} else {
				mask.left   = Ma_pos[0]-4;
				mask.right  = Ma_pos[1]+4;
				mask.top    = Ma_pos[2]-4;
				mask.bottom = Ma_pos[3]+4;

				if(mask.left   <= InspOcLSetPara.nXmin)	InspOcLSetPara.nXmin=mask.left-1;
				if(mask.right  >= InspOcLSetPara.nXmax)	InspOcLSetPara.nXmax=mask.right+1;
				if(mask.top    <= InspOcLSetPara.nYmin)	InspOcLSetPara.nYmin=mask.top-1;
				if(mask.bottom >= InspOcLSetPara.nYmax)	InspOcLSetPara.nYmax=mask.bottom+1;
				thr = BinLevelSet_WithMask( &InspOcLSetPara , &mask );
			}
			// --- Check level
			if( thr < 0 ) {
				// --- comment
				if( i_mode & INSPECT_STEP ) {
					sprintf( Comment, " === ERROR === \n Setup of Insp. overcoat bin level was not completed. " );
					DrawMessage( Comment, DRAW_RED, 2, 2 );
				}
//				return ERROR;
				InspOcLSetPara.nLevel = OcoatInspLevel.nLower;
			}
			OcoatInspLevel.nData = InspOcLSetPara.nLevel + OcoatInspLevel.nOffset;
			if( OcoatInspLevel.nData < OcoatInspLevel.nLower )	OcoatInspLevel.nData = OcoatInspLevel.nLower;
			if( OcoatInspLevel.nData > OcoatInspLevel.nUpper )	OcoatInspLevel.nData = OcoatInspLevel.nUpper;
			// view
				if( i_mode & INSPECT_STEP ) {
					R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
					area.left	= InspOcLSetPara.nXmin;
					area.right	= InspOcLSetPara.nXmax;
					area.top	= InspOcLSetPara.nYmin;
					area.bottom	= InspOcLSetPara.nYmax;
					R_DrawBox( DRAW_YELLOW, area, GRAPH_DRAW, SOLID_LINE );
					sprintf( Comment, "Insp. level (Ocoat:%d)", OcoatInspLevel.nData );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					R_DrawCls();
				}

		// result
			return OK;

	}	// OvercoatInspLevelSet( void ) end

//================================================
//		オーバーコート検査検査レベル設定
//================================================

int		OvercoatInspLevelSetBlack( void )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						thr;
//		int						offset_l = 20;
		int						xmin, xmax;
		int						ymin, ymax, ysize;
		int						i,j;
		R_RECT					mask;
		R_RECT					area;

		// Insp. bin level set
			// --- Gray memory
			InspOcLSetPara.nGrayPn = grayPn;
			// --- area
			//v2.20 if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0 && bIsLTR50==0){
			if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0 && bIsLTR50==0 && bIsLTR18==0){
				xmin	= max( max(	Elec[DataLeft].CornerPosX[PosLup]  , Elec[DataLeft].CornerPosX[PosLlo])
							,  max( Elec[DataLeft].CornerPosX[PosRup]  , Elec[DataLeft].CornerPosX[PosRlo])) + 5;
				xmax	= min( min(	Elec[DataRight].CornerPosX[PosLup] , Elec[DataRight].CornerPosX[PosLlo])
							,  min( Elec[DataRight].CornerPosX[PosRup] , Elec[DataRight].CornerPosX[PosRlo])) - 5;

				ymin	= max( max(	Elec[DataLeft].CornerPosY[PosLup]  , Elec[DataLeft].CornerPosY[PosRup])
							 , max(	Elec[DataRight].CornerPosY[PosLup] , Elec[DataRight].CornerPosY[PosRup])) + 5;
				ymax	= min( min(	Elec[DataLeft].CornerPosY[PosLlo] , Elec[DataLeft].CornerPosY[PosRlo])
							 , min(	Elec[DataRight].CornerPosY[PosLlo] , Elec[DataRight].CornerPosY[PosRlo])) - 5;
			//v2.20 } else if(bIsLTR50){
			} else if( (bIsLTR50) || (bIsLTR18) ){
				xmin	= max( max(	Elec[DataUpper].CornerPosX[PosLup] , Elec[DataUpper].CornerPosX[PosLlo])
							,  max( Elec[DataLower].CornerPosX[PosLup] , Elec[DataLower].CornerPosX[PosLlo])) + 5;
				xmax	= min( min(	Elec[DataUpper].CornerPosX[PosRup] , Elec[DataUpper].CornerPosX[PosRlo])
							,  min( Elec[DataLower].CornerPosX[PosRup] , Elec[DataLower].CornerPosX[PosRlo])) - 5;

				ymin	= max( max(	Elec[DataUpper].CornerPosY[PosLup] , Elec[DataUpper].CornerPosY[PosRup])
							 , max(	Elec[DataUpper].CornerPosY[PosLlo] , Elec[DataUpper].CornerPosY[PosRlo])) + 5;
				ymax	= min( min(	Elec[DataLower].CornerPosY[PosLup] , Elec[DataLower].CornerPosY[PosRup])
							 , min(	Elec[DataLower].CornerPosY[PosLlo] , Elec[DataLower].CornerPosY[PosRlo])) - 5;
			} else {
				for(i=0;i<80;i++){
					if(G2_data_u[i][0]==0xffff)	break;
				}
				if(i!=0)	i--;
				for(j=0;j<80;j++){
					if(G2_data_d[j][0]==0xffff)	break;
				}
				if(j!=0)	j--;
				xmin	= max(	G2_data_u[0][0]  , G2_data_d[0][0] ) +5;
				if(xmin > FxSize-1)   xmin =FxSize-1;
				xmax	= min(  G2_data_u[i][0]  , G2_data_d[j][0] ) -5;
				if(xmax < 0       )	  xmax =0;

				ymin	= max(	G2_data_u[0][1]  , G2_data_u[i][1] ) +5;
				if(ymin > FySize-1)   ymin =FySize-1;
				ymax	= min(  G2_data_d[0][1]  , G2_data_d[j][1] ) -5;
				if(ymax < 0       )	  ymax =0;
			}



			ysize = (ymax-ymin+1)/4;
			InspOcLSetPara.nXmin = xmin;
			InspOcLSetPara.nXmax = xmax;
		//	InspOcLSetPara.nYmin = ymin+ysize;
		//	InspOcLSetPara.nYmax = ymax-ysize;
			InspOcLSetPara.nYmin = ymin;
			InspOcLSetPara.nYmax = ymax;

/*******************
			InspOcLSetPara.nXmin = Overcoat.Pos[PosXmin]+offset_l;
			InspOcLSetPara.nXmax = Overcoat.Pos[PosXmax]-offset_l;
			InspOcLSetPara.nYmin = Overcoat.Pos[PosYmin]+offset_l;
			InspOcLSetPara.nYmax = Overcoat.Pos[PosYmax]-offset_l;
*******************/
			// ---
			InspOcLSetPara.nStartLevel	= 10;									// ヒストグラムの計算範囲の開始点(start>=0)
			InspOcLSetPara.nEndLevel	= 250;									// ヒストグラムの計算範囲の終了点
			InspOcLSetPara.nMethod		= THRESH_OTSU;							// しきい値選定の方法
			// --- level set
			//標印検査有り無し
			if( Ma_pos[0] < 0 || TWA_Mark_Insp != 0 || (TWA_Spec!=SpecJPW)&&(TWA_Insp_mode==Insp_Pinhole)){ 		//メッキ付着検査
				thr = BinLevelSet( &InspOcLSetPara );
			} else {
				mask.left   = Ma_pos[0]-4;
				mask.right  = Ma_pos[1]+4;
				mask.top    = Ma_pos[2]-4;
				mask.bottom = Ma_pos[3]+4;

				if(mask.left   <= InspOcLSetPara.nXmin)	InspOcLSetPara.nXmin=mask.left-1;
				if(mask.right  >= InspOcLSetPara.nXmax)	InspOcLSetPara.nXmax=mask.right+1;
				if(mask.top    <= InspOcLSetPara.nYmin)	InspOcLSetPara.nYmin=mask.top-1;
				if(mask.bottom >= InspOcLSetPara.nYmax)	InspOcLSetPara.nYmax=mask.bottom+1;
				thr = BinLevelSet_WithMask( &InspOcLSetPara , &mask );
			}
			// --- Check level
			if( thr < 0 ) {
				// --- comment
				if( i_mode & INSPECT_STEP ) {
					sprintf( Comment, " === ERROR === \n Setup of Insp. overcoat bin level was not completed. " );
					DrawMessage( Comment, DRAW_RED, 2, 2 );
				}
//				return ERROR;
				InspOcLSetPara.nLevel = OcoatInspLevelBlack.nLower;
			}
			OcoatInspLevelBlack.nData = InspOcLSetPara.nLevel + OcoatInspLevelBlack.nOffset;
			if( OcoatInspLevelBlack.nData < OcoatInspLevelBlack.nLower )	OcoatInspLevelBlack.nData = OcoatInspLevelBlack.nLower;
			if( OcoatInspLevelBlack.nData > OcoatInspLevelBlack.nUpper )	OcoatInspLevelBlack.nData = OcoatInspLevelBlack.nUpper;
			// view
				if( i_mode & INSPECT_STEP ) {
					R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
					area.left	= InspOcLSetPara.nXmin;
					area.right	= InspOcLSetPara.nXmax;
					area.top	= InspOcLSetPara.nYmin;
					area.bottom	= InspOcLSetPara.nYmax;
					R_DrawBox( DRAW_YELLOW, area, GRAPH_DRAW, SOLID_LINE );
					sprintf( Comment, "Insp. level (OcoatBlack:%d)", OcoatInspLevelBlack.nData );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					R_DrawCls();
				}

		// result
			return OK;

	}	// OvercoatInspLevelSetBlack( void ) end


//================================================
//		オーバーコート検査
//			検査２値画像、検査範囲設定
//================================================

void	InspBinSetOvercoat( void )
	{
		int						grayPn	= InspGrayPn;
		int						binPn	= InspBinPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						side, dn;
		int						xmin, xmax, xsize, xs, xe;
		int						ymin, ymax, ysize, ys, ye;
        int                     xm;
        int						cls_w;
		int						bak_graph_page	= Graph_page;
		int						elec;
		R_RECT					area;
        BIN_FILTER_PARAMETER	BinFilter;    // Filter condition

		// Initialze
			BinClsPn( binPn, 0, 0, FxSize-1, FySize-1 );
			for( side=0; side<4; side++ ) {
				for( dn=0; dn<8; dn++ ) {
					InspOcOutside[side][dn]		= 0;
					InspOcOutsideCor[side][dn]	= 0;
				}
			}

//			InspOcOffsetXsize = 4;
//			InspOcOffsetYsize = 2;

		// Overcoat Gray --> Bin
		//	xmin	= 0;
		//	xmax	= IGuidePos[DataX];
		//	xsize	= xmax-xmin+1;
		//	ymin	= 0;
		//	ysize	= FySize;

			// 2017.08.21 白index部分を省く
			xmin	= IGuidePos[1];
			xmax	= IGuidePos[DataX];
			xsize	= xmax-xmin+1;
			ymin	= Index_pos[0];	// 0
			ymax    = Index_pos[1];	// FySize -1
			ysize	= ymax-ymin+1;

			R_gray2bin( OcoatInspLevel.nData, xmin, ymin, xsize, ysize, Gray_address[grayPn], Bin_address[binPn] );
			// Draw Overcoat bin pn
			// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
				// --- comment
				sprintf( Comment, "Insp.Overcoat BinP:%d (level:%d)", binPn, OcoatInspLevel.nData );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
			}

		//標印文字を消す。
			//v2.11 if(!bIsMCR01){
			if( (!bIsMCR01) && (0==(Option & BACKINSP_MODE)) ){
				if(Ma_pos[0] >= 0 || TWA_Mark_Insp == 0 && (TWA_Insp_level==Insp_Low)&&(TWA_Insp_mode!=Insp_Pinhole)) {
					BinFilter.nBinPn = binPn;
					BinFilter.nXmin = Ma_pos[0]-4;
					BinFilter.nXmax = Ma_pos[1]+4;
					BinFilter.nYmin = Ma_pos[2]-4;
					BinFilter.nYmax = Ma_pos[3]+4;
					if((bIsMCR03 || bIsMCR10 || bIsMCR18 ) && TWA_R_n > 1){
						//複数文字の場合は回転を考えて消す
						MaskMarkingMCR03(binPn);
					} else {
						BinClsPn( binPn, BinFilter.nXmin, BinFilter.nYmin, BinFilter.nXmax, BinFilter.nYmax ); //9499-NOTE
					}
					if( i_mode & INSPECT_STEP ) {
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
						sprintf( Comment, "Contrct Mask");
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
						BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
					}
				}
			}

        // noise clear
			BinFilter.nBinPn = binPn;
			BinFilter.nXmin = 0;
			BinFilter.nXmax = FxSize-1;
			BinFilter.nYmin = 10;
			BinFilter.nYmax = FySize-10;
			//
			FilterBinContrct( &BinFilter, G2_contrct_x, G2_contrct_y );
			FilterBinExpand(  &BinFilter, G2_expand_x,  G2_expand_y );
			FilterBinExpand(  &BinFilter, G2_expand_x,  G2_expand_y );
			FilterBinContrct( &BinFilter, G2_contrct_x, G2_contrct_y );

            if( i_mode & INSPECT_STEP ) {
                R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
                sprintf( Comment, "Noise clear");
                DrawMessage( Comment, DRAW_GREEN, 2, 2 );
                BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
            }

        // Outside clear
			// Graph page set
				Graph_page = binPn;

				//v2.20 if(!bIsLTR50){
				if( (!bIsLTR50) && (!bIsLTR18) ){
				// upper side
					elec = DataLR;
					side = PosYmin;
					xmin = Elec[elec].CornerPosX[PosLup];
					xmax = Elec[elec].CornerPosX[PosRup];
					if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18 ==0 && bIsSFR25 ==0){ // straight
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
					} else {		// separate
						ys = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-xmin);
						ye = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-xmax);
					}
					InspOcOutside[side][PosXmin+4] = xmin;
					InspOcOutside[side][PosXmax+4] = xmax;
					InspOcOutside[side][PosYmin+4] = ys;
					InspOcOutside[side][PosYmax+4] = ye;
		//			if(bIsESR25==0){ // straight
						ys += InspOcOffsetSize[DataYbit];
						ye += InspOcOffsetSize[DataYbit];
						cls_w = abs(ys-ye)+InspOcOffsetSize[DataYbit]+10;
		//			}
					InspOcOutside[side][PosXmin] = xmin;
					InspOcOutside[side][PosXmax] = xmax;
					InspOcOutside[side][PosYmin] = ys;
					InspOcOutside[side][PosYmax] = ye;
		//			if(bIsESR25!=0){
		//				ys += 10;   if(ys>FySize)   ys=FySize;
		//				ye += 10;   if(ye>FySize)   ye=FySize;
		//				cls_w = abs(ys-ye)+InspOcOffsetSize[DataYbit]+10+10;
		//			}
					for( dn=0; dn<cls_w; dn++ ) {
						R_line( xmin, ys, xmax, ye, GRAPH_ERASE, SOLID_LINE );
						ys--;
						ye--;
						if(( ys < 0 )||( ye < 0 ))	break;
					}

					if( i_mode & INSPECT_STEP ) {
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
						sprintf( Comment, "upper side clear");
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
						//  BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
					}


					// lower side
					elec = DataLR;
					side = PosYmax;
					xmin = Elec[elec].CornerPosX[PosLlo];
					xmax = Elec[elec].CornerPosX[PosRlo];
					if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
					} else {
						ys = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-xmin);
						ye = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-xmax);
					}
					InspOcOutside[side][PosXmin+4] = xmin;
					InspOcOutside[side][PosXmax+4] = xmax;
					InspOcOutside[side][PosYmin+4] = ys;
					InspOcOutside[side][PosYmax+4] = ye;
		//			if(bIsESR25==0){
						ys -= InspOcOffsetSize[DataYbit];
						ye -= InspOcOffsetSize[DataYbit];
						cls_w = abs(ys-ye)+InspOcOffsetSize[DataYbit]+10;
		//			}
					InspOcOutside[side][PosXmin] = xmin;
					InspOcOutside[side][PosXmax] = xmax;
					InspOcOutside[side][PosYmin] = ys;
					InspOcOutside[side][PosYmax] = ye;
		//			if(bIsESR25!=0){
		//				ys -= 10;   if(ys<0)   ys=0;
		//				ye -= 10;   if(ye<0)   ye=0;
		//				cls_w = abs(ys-ye)+InspOcOffsetSize[DataYbit]+10+10;
		//			}
					for( dn=0; dn<cls_w; dn++ ) {
						R_line( xmin, ys, xmax, ye, GRAPH_ERASE, SOLID_LINE );
						ys++;
						ye++;
						if(( ys >= FySize )||( ye >= FySize ))	break;
					}
					if( i_mode & INSPECT_STEP ) {
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
						sprintf( Comment, "lower side clear");
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					//   BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
					}

				// left side
					elec = DataLeft;
					side = PosXmax;
					ymin = Elec[elec].CornerPosY[PosLup]-10;
					ymax = Elec[elec].CornerPosY[PosLlo]+10;
					xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
					xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
					InspOcOutside[side][PosXmin+4] = xs;
					InspOcOutside[side][PosXmax+4] = xe;
					InspOcOutside[side][PosYmin+4] = ymin;
					InspOcOutside[side][PosYmax+4] = ymax;
	//				if(bIsESR25==0){
						xs += InspOcOffsetSize[DataXbit];
						xe += InspOcOffsetSize[DataXbit];
						cls_w = abs(xs-xe)+InspOcOffsetSize[DataXbit]+10;
	//				}
					InspOcOutside[side][PosXmin] = xs;
					InspOcOutside[side][PosXmax] = xe;
					InspOcOutside[side][PosYmin] = ymin;
					InspOcOutside[side][PosYmax] = ymax;
	//				if(bIsESR25!=0){
	//					xs+= 10;	if(xs > FxSize) xs=FxSize;
	//					xe+= 10;	if(xe > FxSize) xe=FxSize;
	//					cls_w = abs(xs-xe)+InspOcOffsetSize[DataXbit]+10+10;
	//				}
					for( dn=0; dn<cls_w; dn++ ) {
						R_line( xs-10, ymin, xe, ymax, GRAPH_ERASE, SOLID_LINE );
						xs--;
						xe--;
						if(( xs < 0 )||( xe < 0 ))	break;
					}
					if( i_mode & INSPECT_STEP ) {
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
                        sprintf( Comment, "left side clear 1");
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
						//  BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
					}

			   // right side
					elec = DataRight;
					side = PosXmin;
					ymin = Elec[elec].CornerPosY[PosRup]-10;
					ymax = Elec[elec].CornerPosY[PosRlo]+10;
					xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
					xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
					InspOcOutside[side][PosXmin+4] = xs;
					InspOcOutside[side][PosXmax+4] = xe;
					InspOcOutside[side][PosYmin+4] = ymin;
					InspOcOutside[side][PosYmax+4] = ymax;
					if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){
						xs -= InspOcOffsetSize[DataXbit];
						xe -= InspOcOffsetSize[DataXbit];
						cls_w = abs(xs-xe)+InspOcOffsetSize[DataXbit]+10;
					}
					InspOcOutside[side][PosXmin] = xs;
					InspOcOutside[side][PosXmax] = xe;
					InspOcOutside[side][PosYmin] = ymin;
					InspOcOutside[side][PosYmax] = ymax;
					if(bIsESR25!=0 || bIsMCR10!=0 || bIsMCR18!=0 || bIsSFR25!=0){
						xs-= 10;    if(xs < 0) xs=0;
						xe-= 10;    if(xe < 0) xe=0;
						cls_w = abs(xs-xe)+InspOcOffsetSize[DataXbit]+10+10;
					}
					for( dn=0; dn<cls_w; dn++ ) {
						R_line( xs, ymin, xe+10, ymax, GRAPH_ERASE, SOLID_LINE );
						xs++;
						xe++;
						if(( xs >= FxSize )||( xe >= FxSize ))	break;
					}

					if( i_mode & INSPECT_STEP ) {
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
						sprintf( Comment, "right side clear");
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
						//  BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
					}

				// upper left side
					xmin = Overcoat.CornerPosX[PosLup];
					xmax = xmin+InspOcOffsetCor[DataXbit];
					// --- position Y
					elec = DataLR;
					side = PosYmin;
					if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
					} else {
						ys = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-xmin);
						ye = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-xmax);
					}

					ys += InspOcOffsetCor[DataYbit];
					ye += InspOcOffsetCor[DataYbit];
					// --- position X
					elec = DataLeft;
					side = PosXmax;
					xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ys);
					xe = xs+InspOcOffsetCor[DataXbit];
					InspOcOutsideCor[0][PosXmin] = xs;
					InspOcOutsideCor[0][PosXmax] = xe;
					InspOcOutsideCor[0][PosYmin] = ys;
					InspOcOutsideCor[0][PosYmax] = ye;
					// --- clear
					cls_w = abs(ys-ye)+InspOcOffsetCor[DataYbit]+2;//10;
					for( dn=0; dn<cls_w; dn++ ) {
						xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ys);
						xe = xs+InspOcOffsetCor[DataXbit];
						R_line( xs, ys, xe, ye, GRAPH_ERASE, SOLID_LINE );
						ys--;
						ye--;
						if(( ys < 0 )||( ye < 0 ))	break;
					}
					InspOcOutsideCor[0][PosXmin+4] = xs;
					InspOcOutsideCor[0][PosXmax+4] = xe;
					InspOcOutsideCor[0][PosYmin+4] = ys;
					InspOcOutsideCor[0][PosYmax+4] = ye;
				// upper right side
					xmax = Overcoat.CornerPosX[PosRup];
					xmin = xmax-InspOcOffsetCor[DataXbit];
					// --- position Y
					elec = DataLR;
					side = PosYmin;
					if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
					} else {
						ys = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-xmin);
						ye = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-xmax);
					}
					ys += InspOcOffsetCor[DataYbit];
					ye += InspOcOffsetCor[DataYbit];
					// --- position X
					elec = DataRight;
					side = PosXmin;
					xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ye);
					xs = xe-InspOcOffsetCor[DataXbit];
					InspOcOutsideCor[1][PosXmin] = xs;
					InspOcOutsideCor[1][PosXmax] = xe;
					InspOcOutsideCor[1][PosYmin] = ys;
					InspOcOutsideCor[1][PosYmax] = ye;
					// --- clear
					cls_w = abs(ys-ye)+InspOcOffsetCor[DataYbit]+2;//10;
					for( dn=0; dn<cls_w; dn++ ) {
						xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ye);
						xs = xe-InspOcOffsetCor[DataXbit];
						R_line( xs, ys, xe, ye, GRAPH_ERASE, SOLID_LINE );
						ys--;
						ye--;
						if(( ys < 0 )||( ye < 0 ))	break;
					}
					InspOcOutsideCor[1][PosXmin+4] = xs;
					InspOcOutsideCor[1][PosXmax+4] = xe;
					InspOcOutsideCor[1][PosYmin+4] = ys;
					InspOcOutsideCor[1][PosYmax+4] = ye;
				// lower left side
					xmin = Overcoat.CornerPosX[PosLlo];
					xmax = xmin+InspOcOffsetCor[DataXbit];
					// --- position Y
					elec = DataLR;
					side = PosYmax;
					if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
					} else {
						ys = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-xmin);
						ye = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-xmax);
					}
					ys -= InspOcOffsetCor[DataYbit];
					ye -= InspOcOffsetCor[DataYbit];
					// --- position X
					elec = DataLeft;
					side = PosXmax;
					xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ys);
					xe = xs+InspOcOffsetCor[DataXbit];
					InspOcOutsideCor[2][PosXmin] = xs;
					InspOcOutsideCor[2][PosXmax] = xe;
					InspOcOutsideCor[2][PosYmin] = ys;
					InspOcOutsideCor[2][PosYmax] = ye;
					// --- clear
					cls_w = abs(ys-ye)+InspOcOffsetCor[DataYbit]+2;//10;
					for( dn=0; dn<cls_w; dn++ ) {
						xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ys);
						xe = xs+InspOcOffsetCor[DataXbit];
						R_line( xs, ys, xe, ye, GRAPH_ERASE, SOLID_LINE );
						ys++;
						ye++;
						if(( ys >= FySize )||( ye >= FySize ))	break;
					}
					InspOcOutsideCor[2][PosXmin+4] = xs;
					InspOcOutsideCor[2][PosXmax+4] = xe;
					InspOcOutsideCor[2][PosYmin+4] = ys;
					InspOcOutsideCor[2][PosYmax+4] = ye;
				// lower right side
					xmax = Overcoat.CornerPosX[PosRlo];
					xmin = xmax-InspOcOffsetCor[DataXbit];
					// --- position Y
					elec = DataLR;
					side = PosYmax;
					if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
					} else {
						ys = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-xmin);
						ye = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-xmax);
					}
					ys -= InspOcOffsetCor[DataYbit];
					ye -= InspOcOffsetCor[DataYbit];
					// --- position X
					elec = DataRight;
					side = PosXmin;
					xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ye);
					xs = xe-InspOcOffsetCor[DataXbit];
					InspOcOutsideCor[3][PosXmin] = xs;
					InspOcOutsideCor[3][PosXmax] = xe;
					InspOcOutsideCor[3][PosYmin] = ys;
					InspOcOutsideCor[3][PosYmax] = ye;
					// --- clear
					cls_w = abs(ys-ye)+InspOcOffsetCor[DataYbit]+2;//10;
					for( dn=0; dn<cls_w; dn++ ) {
						xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ye);
						xs = xe-InspOcOffsetCor[DataXbit];
						R_line( xs, ys, xe, ye, GRAPH_ERASE, SOLID_LINE );
						ys++;
						ye++;
						if(( ys >= FySize )||( ye >= FySize ))	break;
					}
					InspOcOutsideCor[3][PosXmin+4] = xs;
					InspOcOutsideCor[3][PosXmax+4] = xe;
					InspOcOutsideCor[3][PosYmin+4] = ys;
					InspOcOutsideCor[3][PosYmax+4] = ye;
				}
				 else { //9499-NOTE


					FilterBinContrct( &BinFilter, G2_contrct_x, G2_contrct_y );

				// left side
					elec = DataUL;
					side = PosXmin;
					ymin = Elec[elec].CornerPosY[PosLup];
					ymax = Elec[elec].CornerPosY[PosLlo];
					// separate
                    xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin); //11911
                    xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax); //11911
                    // xs = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-ymin);
                    // xe = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-ymax);

					InspOcOutside[side][PosXmin+4] = xs;
					InspOcOutside[side][PosXmax+4] = xe;
					InspOcOutside[side][PosYmin+4] = ymin;
					InspOcOutside[side][PosYmax+4] = ymax;

					xs += InspOcOffsetSize[DataXbit];
					xe += InspOcOffsetSize[DataXbit];
					cls_w = abs(xs-xe)+InspOcOffsetSize[DataXbit]+20; //9499 +10

					InspOcOutside[side][PosXmin] = xs;
					InspOcOutside[side][PosXmax] = xe;
					InspOcOutside[side][PosYmin] = ymin;
					InspOcOutside[side][PosYmax] = ymax;

					for( dn=0; dn<cls_w; dn++ ) {
						R_line( xs, ymin, xe, ymax, GRAPH_ERASE, SOLID_LINE );
						xs--;
						xe--;
						if(( xs < 0 )||( xe < 0 ))	break;
					}

					if( i_mode & INSPECT_STEP ) {
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
                        sprintf( Comment, "left side clear 2");
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					}

					// right side
					elec = DataUL;
					side = PosXmax;
					ymin = Elec[elec].CornerPosY[PosRup];
					ymax = Elec[elec].CornerPosY[PosRlo];
					xs = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-ymin);
					xe = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-ymax);

					InspOcOutside[side][PosXmin+4] = xs;
					InspOcOutside[side][PosXmax+4] = xe;
					InspOcOutside[side][PosYmin+4] = ymin;
					InspOcOutside[side][PosYmax+4] = ymax;

					xs -= InspOcOffsetSize[DataXbit];
					xe -= InspOcOffsetSize[DataXbit];
					cls_w = abs(xs-xe)+InspOcOffsetSize[DataXbit]+20; //9499 +10

					InspOcOutside[side][PosXmin] = xs;
					InspOcOutside[side][PosXmax] = xe;
					InspOcOutside[side][PosYmin] = ymin;
					InspOcOutside[side][PosYmax] = ymax;

					for( dn=0; dn<cls_w; dn++ ) {
						R_line( xs, ymin, xe, ymax, GRAPH_ERASE, SOLID_LINE );
						xs++;
						xe++;
						if(( xs >= FxSize )||( xe >= FxSize ))	break;
					}
					if( i_mode & INSPECT_STEP ) {
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
                        sprintf( Comment, "right side clear 2");
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					}

					// upper side
					elec = DataUpper;
					side = PosYmax;
					xmin = Elec[elec].CornerPosX[PosLlo]-10;
					xmax = Elec[elec].CornerPosX[PosRlo]+10;
					ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
					ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);

					InspOcOutside[side][PosXmin+4] = xmin;
					InspOcOutside[side][PosXmax+4] = xmax;
					InspOcOutside[side][PosYmin+4] = ys;
					InspOcOutside[side][PosYmax+4] = ye;

					ys += InspOcOffsetSize[DataYbit];
					ye += InspOcOffsetSize[DataYbit];
					cls_w = abs(ys-ye)+InspOcOffsetSize[DataYbit]+10;

					InspOcOutside[side][PosXmin] = xmin;
					InspOcOutside[side][PosXmax] = xmax;
					InspOcOutside[side][PosYmin] = ys;
					InspOcOutside[side][PosYmax] = ye;

					for( dn=0; dn<cls_w; dn++ ) {
						R_line( xmin, ys, xmax, ye, GRAPH_ERASE, SOLID_LINE );
						ys--;
						ye--;
						if(( ys < 0 )||( ye < 0 ))	break;
					}
					if( i_mode & INSPECT_STEP ) {
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
                        sprintf( Comment, "upper side clear 2");
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					}

				   // lower side
					elec = DataLower;
					side = PosYmin;
					xmin = Elec[elec].CornerPosX[PosLup]-10;
					xmax = Elec[elec].CornerPosX[PosRup]+10;
					ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
					ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);

					InspOcOutside[side][PosXmin+4] = xmin;
					InspOcOutside[side][PosXmax+4] = xmax;
					InspOcOutside[side][PosYmin+4] = ys;
					InspOcOutside[side][PosYmax+4] = ye;

					ys -= InspOcOffsetSize[DataYbit];
					ye -= InspOcOffsetSize[DataYbit];
					cls_w = abs(ys-ye)+InspOcOffsetSize[DataYbit]+10;

					InspOcOutside[side][PosXmin] = xmin;
					InspOcOutside[side][PosXmax] = xmax;
					InspOcOutside[side][PosYmin] = ys;
					InspOcOutside[side][PosYmax] = ye;

					for( dn=0; dn<cls_w; dn++ ) {
						R_line( xmin, ys, xmax, ye, GRAPH_ERASE, SOLID_LINE );
						ys++;
						ye++;
						if(( ys >= FySize )||( ye >= FySize ))	break;
					}

					if( i_mode & INSPECT_STEP ) {
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
                        sprintf( Comment, "lower side clear 2");
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					}

				// left upper side
					ymin = Overcoat.CornerPosY[PosLup];
					ymax = ymin+InspOcOffsetCor[DataYbit];
					// --- position X
					elec = DataUL;
					side = PosXmin;
					//xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
					//xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
					xs = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-ymin);
					xe = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-ymax);

					xs += InspOcOffsetCor[DataXbit];
					xe += InspOcOffsetCor[DataXbit];

					// --- position Y
					elec = DataUpper;
					side = PosYmax;
					ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xs);
					ye = ys+InspOcOffsetCor[DataYbit];
					InspOcOutsideCor[0][PosXmin] = xs;
					InspOcOutsideCor[0][PosXmax] = xe;
					InspOcOutsideCor[0][PosYmin] = ys;
					InspOcOutsideCor[0][PosYmax] = ye;

					// --- clear
					cls_w = abs(xs-xe)+InspOcOffsetCor[DataXbit]+2;//10;
					for( dn=0; dn<cls_w; dn++ ) {
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xs);
						ye = ys+InspOcOffsetCor[DataYbit];
						R_line( xs, ys, xe, ye, GRAPH_ERASE, SOLID_LINE );
						xs--;
						xe--;
						if(( xs < 0 )||( xe < 0 ))	break;
					}

					InspOcOutsideCor[0][PosXmin+4] = xs;
					InspOcOutsideCor[0][PosXmax+4] = xe;
					InspOcOutsideCor[0][PosYmin+4] = ys;
					InspOcOutsideCor[0][PosYmax+4] = ye;

				// left lower side
					ymax = Overcoat.CornerPosY[PosLlo];
					ymin = ymax-InspOcOffsetCor[DataYbit];
					// --- position X
					elec = DataUL;
					side = PosXmin;

					//xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
					//xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
					xs = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-ymin);
					xe = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-ymax);

					xs += InspOcOffsetCor[DataXbit];
					xe += InspOcOffsetCor[DataXbit];
					// --- position Y
					elec = DataLower;
					side = PosYmin;
					ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xe);
					ys = ye-InspOcOffsetCor[DataYbit];
					InspOcOutsideCor[2][PosXmin] = xs;
					InspOcOutsideCor[2][PosXmax] = xe;
					InspOcOutsideCor[2][PosYmin] = ys;
					InspOcOutsideCor[2][PosYmax] = ye;
					// --- clear
					cls_w = abs(xs-xe)+InspOcOffsetCor[DataXbit]+2;//10;
					for( dn=0; dn<cls_w; dn++ ) {
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xe);
						ys = ye-InspOcOffsetCor[DataYbit];
						R_line( xs, ys, xe, ye, GRAPH_ERASE, SOLID_LINE );
						xs--;
						xe--;
						if(( xs < 0 )||( xe < 0 ))	break;
					}
					InspOcOutsideCor[2][PosXmin+4] = xs;
					InspOcOutsideCor[2][PosXmax+4] = xe;
					InspOcOutsideCor[2][PosYmin+4] = ys;
					InspOcOutsideCor[2][PosYmax+4] = ye;
				// right upper side
					ymin = Overcoat.CornerPosY[PosRup];
					ymax = ymin+InspOcOffsetCor[DataYbit];
					// --- position X
					elec = DataUL;
					side = PosXmax;
					//xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
					//xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
					xs = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-ymin);
					xe = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-ymax);

					xs -= InspOcOffsetCor[DataXbit];
					xe -= InspOcOffsetCor[DataXbit];
					// --- position Y
					elec = DataUpper;
					side = PosYmax;
					ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xs);
					ye = ys+InspOcOffsetCor[DataYbit];
					InspOcOutsideCor[1][PosXmin] = xs;
					InspOcOutsideCor[1][PosXmax] = xe;
					InspOcOutsideCor[1][PosYmin] = ys;
					InspOcOutsideCor[1][PosYmax] = ye;
					// --- clear
					cls_w = abs(xs-xe)+InspOcOffsetCor[DataXbit]+2;//10;
					for( dn=0; dn<cls_w; dn++ ) {
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xs);
						ye = ys+InspOcOffsetCor[DataYbit];
						R_line( xs, ys, xe, ye, GRAPH_ERASE, SOLID_LINE );
						xs++;
						xe++;
						if(( xs >= FxSize )||( xe >= FxSize ))	break;
					}
					InspOcOutsideCor[1][PosXmin+4] = xs;
					InspOcOutsideCor[1][PosXmax+4] = xe;
					InspOcOutsideCor[1][PosYmin+4] = ys;
					InspOcOutsideCor[1][PosYmax+4] = ye;
				// right lower side
					ymax = Overcoat.CornerPosY[PosLlo];
					ymin = ymax-InspOcOffsetCor[DataYbit];
					// --- position X
					elec = DataUL;
					side = PosXmax;
					//xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
					//xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
					xs = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-ymin);
					xe = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-ymax);

					xs -= InspOcOffsetCor[DataXbit];
					xe -= InspOcOffsetCor[DataXbit];
					// --- position Y
					elec = DataLower;
					side = PosYmin;
					ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xe);
					ys = ye-InspOcOffsetCor[DataYbit];
					InspOcOutsideCor[3][PosXmin] = xs;
					InspOcOutsideCor[3][PosXmax] = xe;
					InspOcOutsideCor[3][PosYmin] = ys;
					InspOcOutsideCor[3][PosYmax] = ye;
					// --- clear
					cls_w = abs(xs-xe)+InspOcOffsetCor[DataXbit]+2;//10;
					for( dn=0; dn<cls_w; dn++ ) {
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xe);
						ys = ye-InspOcOffsetCor[DataYbit];
						R_line( xs, ys, xe, ye, GRAPH_ERASE, SOLID_LINE );
						xs++;
						xe++;
						if(( xs >= FxSize )||( xe >= FxSize ))	break;
					}
					InspOcOutsideCor[3][PosXmin+4] = xs;
					InspOcOutsideCor[3][PosXmax+4] = xe;
					InspOcOutsideCor[3][PosYmin+4] = ys;
					InspOcOutsideCor[3][PosYmax+4] = ye;

				}
			// Graph page set
				Graph_page = bak_graph_page;

			// Draw Overcoat bin pn
			// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
				// --- area( bin measure )
				area.left	= Overcoat.Pos[PosXmin];
				area.right	= Overcoat.Pos[PosXmax];
				area.top	= Overcoat.Pos[PosYmin];
				area.bottom	= Overcoat.Pos[PosYmax];
				R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
				// --- area
				for( side=0; side<4; side++ ) {
					xmin = InspOcOutside[side][PosXmin];
					xmax = InspOcOutside[side][PosXmax];
					ymin = InspOcOutside[side][PosYmin];
					ymax = InspOcOutside[side][PosYmax];
					R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, DASHED_LINE );
					xmin = InspOcOutside[side][PosXmin+4];
					xmax = InspOcOutside[side][PosXmax+4];
					ymin = InspOcOutside[side][PosYmin+4];
					ymax = InspOcOutside[side][PosYmax+4];
					R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, DASHED_LINE );
				}
				// --- corner
				for( side=0; side<4; side++ ) {
					// --- line Xs
					xs = InspOcOutsideCor[side][PosXmin];
					xe = InspOcOutsideCor[side][PosXmax];
					ys = InspOcOutsideCor[side][PosYmin];
					ye = InspOcOutsideCor[side][PosYmax];
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_NORMAL, DASHED_LINE );
					// --- line Xe
					xs = InspOcOutsideCor[side][PosXmin+4];
					xe = InspOcOutsideCor[side][PosXmax+4];
					ys = InspOcOutsideCor[side][PosYmin+4];
					ye = InspOcOutsideCor[side][PosYmax+4];
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_NORMAL, DASHED_LINE );
					// --- line Ys
					xs = InspOcOutsideCor[side][PosXmin];
					xe = InspOcOutsideCor[side][PosXmin+4];
					ys = InspOcOutsideCor[side][PosYmin];
					ye = InspOcOutsideCor[side][PosYmin+4];
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_NORMAL, DASHED_LINE );
					// --- line Ye
					xs = InspOcOutsideCor[side][PosXmax];
					xe = InspOcOutsideCor[side][PosXmax+4];
					ys = InspOcOutsideCor[side][PosYmax];
					ye = InspOcOutsideCor[side][PosYmax+4];
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_NORMAL, DASHED_LINE );
				}
				// --- comment
				sprintf( Comment, "Noise clear\nInsp.Overcoat BinP:%d (level:%d)", binPn, OcoatInspLevel.nData ); //9499-NOTE
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
				R_DrawCls();
			}

	}	// InspBinSetOvercoat( void ) end


//================================================
//		オーバーコート検査					v2.11
//			検査２値画像、検査範囲設定
//================================================

void	InspBinSetOvercoatBlack( void )
	{
		int						grayPn	= InspGrayPn;
		int						binPn	= InspBinPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						side, dn;
		int						xmin, xmax, xsize, xs, xe;
		int						ymin, ymax, ysize, ys, ye;
        int                     xm;
        int						cls_w;
		int						bak_graph_page	= Graph_page;
		int						elec;
		R_RECT					area;
        BIN_FILTER_PARAMETER	BinFilter;    // Filter condition

		// Initialze
			BinClsPn( binPn, 0, 0, FxSize-1, FySize-1 );
			for( side=0; side<4; side++ ) {
				for( dn=0; dn<8; dn++ ) {
					InspOcOutside[side][dn]		= 0;
					InspOcOutsideCor[side][dn]	= 0;
				}
			}

//			InspOcOffsetXsizeBlack = 4;
//			InspOcOffsetYsizeBlack = 2;

		// Overcoat Gray --> Bin
		//	xmin	= 0;
		//	xmax	= IGuidePos[DataX];
		//	xsize	= xmax-xmin+1;
		//	ymin	= 0;
		//	ysize	= FySize;

			// 2017.08.21 白index部分を省く
			xmin	= IGuidePos[1];
			xmax	= IGuidePos[DataX];
			xsize	= xmax-xmin+1;
			ymin	= Index_pos[0];	// 0
			ymax    = Index_pos[1];	// FySize -1
			ysize	= ymax-ymin+1;

			R_gray2bin( OcoatInspLevelBlack.nData, xmin, ymin, xsize, ysize, Gray_address[grayPn], Bin_address[binPn] );
			// Draw Overcoat bin pn
			// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
				// --- comment
				sprintf( Comment, "Insp.OvercoatBlack BinP:%d (level:%d)", binPn, OcoatInspLevelBlack.nData );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
			}

		//標印文字を消す。
			//v2.11 if(!bIsMCR01){
			if( (!bIsMCR01) && (0==(Option & BACKINSP_MODE)) ){
				if(Ma_pos[0] >= 0 || TWA_Mark_Insp == 0 && (TWA_Insp_level==Insp_Low)&&(TWA_Insp_mode!=Insp_Pinhole)) {
					BinFilter.nBinPn = binPn;
					BinFilter.nXmin = Ma_pos[0]-4;
					BinFilter.nXmax = Ma_pos[1]+4;
					BinFilter.nYmin = Ma_pos[2]-4;
					BinFilter.nYmax = Ma_pos[3]+4;
					if((bIsMCR03 || bIsMCR10 || bIsMCR18 ) && TWA_R_n > 1){
						//複数文字の場合は回転を考えて消す
						MaskMarkingMCR03(binPn);
					} else {
						BinClsPn( binPn, BinFilter.nXmin, BinFilter.nYmin, BinFilter.nXmax, BinFilter.nYmax );
					}
					if( i_mode & INSPECT_STEP ) {
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
						sprintf( Comment, "Contrct Mask For BlackInsp");
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
						BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
					}
				}
			}

        // noise clear
			BinFilter.nBinPn = binPn;
			BinFilter.nXmin = 0;
			BinFilter.nXmax = FxSize-1;
			BinFilter.nYmin = 10;
			BinFilter.nYmax = FySize-10;
			//
			FilterBinContrct( &BinFilter, G2_contrct_x, G2_contrct_y );
			FilterBinExpand(  &BinFilter, G2_expand_x,  G2_expand_y );
			FilterBinExpand(  &BinFilter, G2_expand_x,  G2_expand_y );
			FilterBinContrct( &BinFilter, G2_contrct_x, G2_contrct_y );

            if( i_mode & INSPECT_STEP ) {
                R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
                sprintf( Comment, "Noise clear For BlackInsp");
                DrawMessage( Comment, DRAW_GREEN, 2, 2 );
                BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
            }

        // Outside clear
			// Graph page set
				Graph_page = binPn;

				//v2.20 if(!bIsLTR50){
				if( (!bIsLTR50) && (!bIsLTR18) ){
				// upper side
					elec = DataLR;
					side = PosYmin;
					xmin = Elec[elec].CornerPosX[PosLup];
					xmax = Elec[elec].CornerPosX[PosRup];
					if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18 ==0 && bIsSFR25 ==0){ // straight
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
					} else {		// separate
						ys = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-xmin);
						ye = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-xmax);
					}
					InspOcOutside[side][PosXmin+4] = xmin;
					InspOcOutside[side][PosXmax+4] = xmax;
					InspOcOutside[side][PosYmin+4] = ys;
					InspOcOutside[side][PosYmax+4] = ye;
		//			if(bIsESR25==0){ // straight
						ys += InspOcOffsetSizeBlack[DataYbit];
						ye += InspOcOffsetSizeBlack[DataYbit];
						cls_w = abs(ys-ye)+InspOcOffsetSizeBlack[DataYbit]+10;
		//			}
					InspOcOutside[side][PosXmin] = xmin;
					InspOcOutside[side][PosXmax] = xmax;
					InspOcOutside[side][PosYmin] = ys;
					InspOcOutside[side][PosYmax] = ye;
		//			if(bIsESR25!=0){
		//				ys += 10;   if(ys>FySize)   ys=FySize;
		//				ye += 10;   if(ye>FySize)   ye=FySize;
		//				cls_w = abs(ys-ye)+InspOcOffsetSizeBlack[DataYbit]+10+10;
		//			}
					for( dn=0; dn<cls_w; dn++ ) {
						R_line( xmin, ys, xmax, ye, GRAPH_DRAW, SOLID_LINE );
						ys--;
						ye--;
						if(( ys < 0 )||( ye < 0 ))	break;
					}

					if( i_mode & INSPECT_STEP ) {
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
						sprintf( Comment, "upper side clear For BlackInsp");
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
						//  BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
					}


					// lower side
					elec = DataLR;
					side = PosYmax;
					xmin = Elec[elec].CornerPosX[PosLlo];
					xmax = Elec[elec].CornerPosX[PosRlo];
					if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
					} else {
						ys = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-xmin);
						ye = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-xmax);
					}
					InspOcOutside[side][PosXmin+4] = xmin;
					InspOcOutside[side][PosXmax+4] = xmax;
					InspOcOutside[side][PosYmin+4] = ys;
					InspOcOutside[side][PosYmax+4] = ye;
		//			if(bIsESR25==0){
						ys -= InspOcOffsetSizeBlack[DataYbit];
						ye -= InspOcOffsetSizeBlack[DataYbit];
						cls_w = abs(ys-ye)+InspOcOffsetSizeBlack[DataYbit]+10;
		//			}
					InspOcOutside[side][PosXmin] = xmin;
					InspOcOutside[side][PosXmax] = xmax;
					InspOcOutside[side][PosYmin] = ys;
					InspOcOutside[side][PosYmax] = ye;
		//			if(bIsESR25!=0){
		//				ys -= 10;   if(ys<0)   ys=0;
		//				ye -= 10;   if(ye<0)   ye=0;
		//				cls_w = abs(ys-ye)+InspOcOffsetSizeBlack[DataYbit]+10+10;
		//			}
					for( dn=0; dn<cls_w; dn++ ) {
						R_line( xmin, ys, xmax, ye, GRAPH_DRAW, SOLID_LINE );
						ys++;
						ye++;
						if(( ys >= FySize )||( ye >= FySize ))	break;
					}
					if( i_mode & INSPECT_STEP ) {
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
						sprintf( Comment, "lower side clear For BlackInsp");
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					//   BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
					}

				// left side
					elec = DataLeft;
					side = PosXmax;
					ymin = Elec[elec].CornerPosY[PosLup]-10;
					ymax = Elec[elec].CornerPosY[PosLlo]+10;
					xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
					xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
					InspOcOutside[side][PosXmin+4] = xs;
					InspOcOutside[side][PosXmax+4] = xe;
					InspOcOutside[side][PosYmin+4] = ymin;
					InspOcOutside[side][PosYmax+4] = ymax;
	//				if(bIsESR25==0){
						xs += InspOcOffsetSizeBlack[DataXbit];
						xe += InspOcOffsetSizeBlack[DataXbit];
						cls_w = abs(xs-xe)+InspOcOffsetSizeBlack[DataXbit]+10;
	//				}
					InspOcOutside[side][PosXmin] = xs;
					InspOcOutside[side][PosXmax] = xe;
					InspOcOutside[side][PosYmin] = ymin;
					InspOcOutside[side][PosYmax] = ymax;
	//				if(bIsESR25!=0){
	//					xs+= 10;	if(xs > FxSize) xs=FxSize;
	//					xe+= 10;	if(xe > FxSize) xe=FxSize;
	//					cls_w = abs(xs-xe)+InspOcOffsetSizeBlack[DataXbit]+10+10;
	//				}
					for( dn=0; dn<cls_w; dn++ ) {
						R_line( xs, ymin, xe, ymax, GRAPH_DRAW, SOLID_LINE );
						xs--;
						xe--;
						if(( xs < 0 )||( xe < 0 ))	break;
					}
					if( i_mode & INSPECT_STEP ) {
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
                        sprintf( Comment, "left side clear For BlackInsp1");
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
						//  BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
					}

			   // right side
					elec = DataRight;
					side = PosXmin;
					ymin = Elec[elec].CornerPosY[PosRup]-10;
					ymax = Elec[elec].CornerPosY[PosRlo]+10;
					xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
					xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
					InspOcOutside[side][PosXmin+4] = xs;
					InspOcOutside[side][PosXmax+4] = xe;
					InspOcOutside[side][PosYmin+4] = ymin;
					InspOcOutside[side][PosYmax+4] = ymax;
					if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){
						xs -= InspOcOffsetSizeBlack[DataXbit];
						xe -= InspOcOffsetSizeBlack[DataXbit];
						cls_w = abs(xs-xe)+InspOcOffsetSizeBlack[DataXbit]+10;
					}
					InspOcOutside[side][PosXmin] = xs;
					InspOcOutside[side][PosXmax] = xe;
					InspOcOutside[side][PosYmin] = ymin;
					InspOcOutside[side][PosYmax] = ymax;
					if(bIsESR25!=0 || bIsMCR10!=0 || bIsMCR18!=0 || bIsSFR25!=0){
						xs-= 10;    if(xs < 0) xs=0;
						xe-= 10;    if(xe < 0) xe=0;
						cls_w = abs(xs-xe)+InspOcOffsetSizeBlack[DataXbit]+10+10;
					}
					for( dn=0; dn<cls_w; dn++ ) {
						R_line( xs, ymin, xe, ymax, GRAPH_DRAW, SOLID_LINE );
						xs++;
						xe++;
						if(( xs >= FxSize )||( xe >= FxSize ))	break;
					}

					if( i_mode & INSPECT_STEP ) {
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
						sprintf( Comment, "right side clear For BlackInsp");
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
						//  BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
					}

				// upper left side
					xmin = Overcoat.CornerPosX[PosLup];
					xmax = xmin+InspOcOffsetCorBlack[DataXbit];
					// --- position Y
					elec = DataLR;
					side = PosYmin;
					if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
					} else {
						ys = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-xmin);
						ye = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-xmax);
					}

					ys += InspOcOffsetCorBlack[DataYbit];
					ye += InspOcOffsetCorBlack[DataYbit];
					// --- position X
					elec = DataLeft;
					side = PosXmax;
					xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ys);
					xe = xs+InspOcOffsetCorBlack[DataXbit];
					InspOcOutsideCor[0][PosXmin] = xs;
					InspOcOutsideCor[0][PosXmax] = xe;
					InspOcOutsideCor[0][PosYmin] = ys;
					InspOcOutsideCor[0][PosYmax] = ye;
					// --- clear
					cls_w = abs(ys-ye)+InspOcOffsetCorBlack[DataYbit]+2;//10;
					for( dn=0; dn<cls_w; dn++ ) {
						xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ys);
						xe = xs+InspOcOffsetCorBlack[DataXbit];
						R_line( xs, ys, xe, ye, GRAPH_DRAW, SOLID_LINE );
						ys--;
						ye--;
						if(( ys < 0 )||( ye < 0 ))	break;
					}
					InspOcOutsideCor[0][PosXmin+4] = xs;
					InspOcOutsideCor[0][PosXmax+4] = xe;
					InspOcOutsideCor[0][PosYmin+4] = ys;
					InspOcOutsideCor[0][PosYmax+4] = ye;
				// upper right side
					xmax = Overcoat.CornerPosX[PosRup];
					xmin = xmax-InspOcOffsetCorBlack[DataXbit];
					// --- position Y
					elec = DataLR;
					side = PosYmin;
					if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
					} else {
						ys = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-xmin);
						ye = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-xmax);
					}
					ys += InspOcOffsetCorBlack[DataYbit];
					ye += InspOcOffsetCorBlack[DataYbit];
					// --- position X
					elec = DataRight;
					side = PosXmin;
					xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ye);
					xs = xe-InspOcOffsetCorBlack[DataXbit];
					InspOcOutsideCor[1][PosXmin] = xs;
					InspOcOutsideCor[1][PosXmax] = xe;
					InspOcOutsideCor[1][PosYmin] = ys;
					InspOcOutsideCor[1][PosYmax] = ye;
					// --- clear
					cls_w = abs(ys-ye)+InspOcOffsetCorBlack[DataYbit]+2;//10;
					for( dn=0; dn<cls_w; dn++ ) {
						xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ye);
						xs = xe-InspOcOffsetCorBlack[DataXbit];
						R_line( xs, ys, xe, ye, GRAPH_DRAW, SOLID_LINE );
						ys--;
						ye--;
						if(( ys < 0 )||( ye < 0 ))	break;
					}
					InspOcOutsideCor[1][PosXmin+4] = xs;
					InspOcOutsideCor[1][PosXmax+4] = xe;
					InspOcOutsideCor[1][PosYmin+4] = ys;
					InspOcOutsideCor[1][PosYmax+4] = ye;
				// lower left side
					xmin = Overcoat.CornerPosX[PosLlo];
					xmax = xmin+InspOcOffsetCorBlack[DataXbit];
					// --- position Y
					elec = DataLR;
					side = PosYmax;
					if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
					} else {
						ys = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-xmin);
						ye = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-xmax);
					}
					ys -= InspOcOffsetCorBlack[DataYbit];
					ye -= InspOcOffsetCorBlack[DataYbit];
					// --- position X
					elec = DataLeft;
					side = PosXmax;
					xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ys);
					xe = xs+InspOcOffsetCorBlack[DataXbit];
					InspOcOutsideCor[2][PosXmin] = xs;
					InspOcOutsideCor[2][PosXmax] = xe;
					InspOcOutsideCor[2][PosYmin] = ys;
					InspOcOutsideCor[2][PosYmax] = ye;
					// --- clear
					cls_w = abs(ys-ye)+InspOcOffsetCorBlack[DataYbit]+2;//10;
					for( dn=0; dn<cls_w; dn++ ) {
						xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ys);
						xe = xs+InspOcOffsetCorBlack[DataXbit];
						R_line( xs, ys, xe, ye, GRAPH_DRAW, SOLID_LINE );
						ys++;
						ye++;
						if(( ys >= FySize )||( ye >= FySize ))	break;
					}
					InspOcOutsideCor[2][PosXmin+4] = xs;
					InspOcOutsideCor[2][PosXmax+4] = xe;
					InspOcOutsideCor[2][PosYmin+4] = ys;
					InspOcOutsideCor[2][PosYmax+4] = ye;
				// lower right side
					xmax = Overcoat.CornerPosX[PosRlo];
					xmin = xmax-InspOcOffsetCorBlack[DataXbit];
					// --- position Y
					elec = DataLR;
					side = PosYmax;
					if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
					} else {
						ys = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-xmin);
						ye = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-xmax);
					}
					ys -= InspOcOffsetCorBlack[DataYbit];
					ye -= InspOcOffsetCorBlack[DataYbit];
					// --- position X
					elec = DataRight;
					side = PosXmin;
					xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ye);
					xs = xe-InspOcOffsetCorBlack[DataXbit];
					InspOcOutsideCor[3][PosXmin] = xs;
					InspOcOutsideCor[3][PosXmax] = xe;
					InspOcOutsideCor[3][PosYmin] = ys;
					InspOcOutsideCor[3][PosYmax] = ye;
					// --- clear
					cls_w = abs(ys-ye)+InspOcOffsetCorBlack[DataYbit]+2;//10;
					for( dn=0; dn<cls_w; dn++ ) {
						xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ye);
						xs = xe-InspOcOffsetCorBlack[DataXbit];
						R_line( xs, ys, xe, ye, GRAPH_DRAW, SOLID_LINE );
						ys++;
						ye++;
						if(( ys >= FySize )||( ye >= FySize ))	break;
					}
					InspOcOutsideCor[3][PosXmin+4] = xs;
					InspOcOutsideCor[3][PosXmax+4] = xe;
					InspOcOutsideCor[3][PosYmin+4] = ys;
					InspOcOutsideCor[3][PosYmax+4] = ye;
				} else {


					FilterBinContrct( &BinFilter, G2_contrct_x, G2_contrct_y );

                // left side
                    elec = DataUL;
					side = PosXmin;
                    ymin = Elec[elec].CornerPosY[PosLup];
                    ymax = Elec[elec].CornerPosY[PosLlo];
					// separate
                    xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin); //11911
                    xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax); //11911
                   // xs = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-ymin); old
                    //xe = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-ymax); old


					InspOcOutside[side][PosXmin+4] = xs;
					InspOcOutside[side][PosXmax+4] = xe;
					InspOcOutside[side][PosYmin+4] = ymin;
					InspOcOutside[side][PosYmax+4] = ymax;

					xs += InspOcOffsetSizeBlack[DataXbit];
					xe += InspOcOffsetSizeBlack[DataXbit];
					cls_w = abs(xs-xe)+InspOcOffsetSizeBlack[DataXbit]+10;

					InspOcOutside[side][PosXmin] = xs;
					InspOcOutside[side][PosXmax] = xe;
					InspOcOutside[side][PosYmin] = ymin;
					InspOcOutside[side][PosYmax] = ymax;

					for( dn=0; dn<cls_w; dn++ ) {
						R_line( xs, ymin, xe, ymax, GRAPH_DRAW, SOLID_LINE );
						xs--;
						xe--;
						if(( xs < 0 )||( xe < 0 ))	break;
					}

					if( i_mode & INSPECT_STEP ) {
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
                        sprintf( Comment, "left side clear For BlackInsp2");
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					}

					// right side
					elec = DataUL;
					side = PosXmax;
					ymin = Elec[elec].CornerPosY[PosRup];
					ymax = Elec[elec].CornerPosY[PosRlo];
					xs = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-ymin);
					xe = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-ymax);

					InspOcOutside[side][PosXmin+4] = xs;
					InspOcOutside[side][PosXmax+4] = xe;
					InspOcOutside[side][PosYmin+4] = ymin;
					InspOcOutside[side][PosYmax+4] = ymax;

					xs -= InspOcOffsetSizeBlack[DataXbit];
					xe -= InspOcOffsetSizeBlack[DataXbit];
					cls_w = abs(xs-xe)+InspOcOffsetSizeBlack[DataXbit]+10;

					InspOcOutside[side][PosXmin] = xs;
					InspOcOutside[side][PosXmax] = xe;
					InspOcOutside[side][PosYmin] = ymin;
					InspOcOutside[side][PosYmax] = ymax;

					for( dn=0; dn<cls_w; dn++ ) {
						R_line( xs, ymin, xe, ymax, GRAPH_DRAW, SOLID_LINE );
						xs++;
						xe++;
						if(( xs >= FxSize )||( xe >= FxSize ))	break;
					}
					if( i_mode & INSPECT_STEP ) {
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
                        sprintf( Comment, "right side clear For BlackInsp2");
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					}

					// upper side
					elec = DataUpper;
					side = PosYmax;
					xmin = Elec[elec].CornerPosX[PosLlo]-10;
					xmax = Elec[elec].CornerPosX[PosRlo]+10;
					ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
					ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);

					InspOcOutside[side][PosXmin+4] = xmin;
					InspOcOutside[side][PosXmax+4] = xmax;
					InspOcOutside[side][PosYmin+4] = ys;
					InspOcOutside[side][PosYmax+4] = ye;

					ys += InspOcOffsetSizeBlack[DataYbit];
					ye += InspOcOffsetSizeBlack[DataYbit];
					cls_w = abs(ys-ye)+InspOcOffsetSizeBlack[DataYbit]+10;

					InspOcOutside[side][PosXmin] = xmin;
					InspOcOutside[side][PosXmax] = xmax;
					InspOcOutside[side][PosYmin] = ys;
					InspOcOutside[side][PosYmax] = ye;

					for( dn=0; dn<cls_w; dn++ ) {
						R_line( xmin, ys, xmax, ye, GRAPH_DRAW, SOLID_LINE );
						ys--;
						ye--;
						if(( ys < 0 )||( ye < 0 ))	break;
					}
					if( i_mode & INSPECT_STEP ) {
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
                        sprintf( Comment, "upper side clear For BlackInspInsp2");
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					}

				   // lower side
					elec = DataLower;
					side = PosYmin;
					xmin = Elec[elec].CornerPosX[PosLup]-10;
					xmax = Elec[elec].CornerPosX[PosRup]+10;
					ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
					ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);

					InspOcOutside[side][PosXmin+4] = xmin;
					InspOcOutside[side][PosXmax+4] = xmax;
					InspOcOutside[side][PosYmin+4] = ys;
					InspOcOutside[side][PosYmax+4] = ye;

					ys -= InspOcOffsetSizeBlack[DataYbit];
					ye -= InspOcOffsetSizeBlack[DataYbit];
					cls_w = abs(ys-ye)+InspOcOffsetSizeBlack[DataYbit]+10;

					InspOcOutside[side][PosXmin] = xmin;
					InspOcOutside[side][PosXmax] = xmax;
					InspOcOutside[side][PosYmin] = ys;
					InspOcOutside[side][PosYmax] = ye;

					for( dn=0; dn<cls_w; dn++ ) {
						R_line( xmin, ys, xmax, ye, GRAPH_DRAW, SOLID_LINE );
						ys++;
						ye++;
						if(( ys >= FySize )||( ye >= FySize ))	break;
					}

					if( i_mode & INSPECT_STEP ) {
						R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
                        sprintf( Comment, "lower side clear For BlackInsp2");
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					}

				// left upper side
					ymin = Overcoat.CornerPosY[PosLup];
					ymax = ymin+InspOcOffsetCorBlack[DataYbit];
					// --- position X
					elec = DataUL;
					side = PosXmin;
					//xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
					//xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
					xs = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-ymin);
					xe = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-ymax);

					xs += InspOcOffsetCorBlack[DataXbit];
					xe += InspOcOffsetCorBlack[DataXbit];

					// --- position Y
					elec = DataUpper;
					side = PosYmax;
					ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xs);
					ye = ys+InspOcOffsetCorBlack[DataYbit];
					InspOcOutsideCor[0][PosXmin] = xs;
					InspOcOutsideCor[0][PosXmax] = xe;
					InspOcOutsideCor[0][PosYmin] = ys;
					InspOcOutsideCor[0][PosYmax] = ye;

					// --- clear
					cls_w = abs(xs-xe)+InspOcOffsetCorBlack[DataXbit]+2;//10;
					for( dn=0; dn<cls_w; dn++ ) {
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xs);
						ye = ys+InspOcOffsetCorBlack[DataYbit];
						R_line( xs, ys, xe, ye, GRAPH_DRAW, SOLID_LINE );
						xs--;
						xe--;
						if(( xs < 0 )||( xe < 0 ))	break;
					}

					InspOcOutsideCor[0][PosXmin+4] = xs;
					InspOcOutsideCor[0][PosXmax+4] = xe;
					InspOcOutsideCor[0][PosYmin+4] = ys;
					InspOcOutsideCor[0][PosYmax+4] = ye;

				// left lower side
					ymax = Overcoat.CornerPosY[PosLlo];
					ymin = ymax-InspOcOffsetCorBlack[DataYbit];
					// --- position X
                    elec = DataUL;
					side = PosXmin;

					//xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
					//xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
					xs = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-ymin);
					xe = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-ymax);

					xs += InspOcOffsetCorBlack[DataXbit];
					xe += InspOcOffsetCorBlack[DataXbit];
					// --- position Y
					elec = DataLower;
					side = PosYmin;
					ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xe);
					ys = ye-InspOcOffsetCorBlack[DataYbit];
					InspOcOutsideCor[2][PosXmin] = xs;
					InspOcOutsideCor[2][PosXmax] = xe;
					InspOcOutsideCor[2][PosYmin] = ys;
					InspOcOutsideCor[2][PosYmax] = ye;
					// --- clear
					cls_w = abs(xs-xe)+InspOcOffsetCorBlack[DataXbit]+2;//10;
					for( dn=0; dn<cls_w; dn++ ) {
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xe);
						ys = ye-InspOcOffsetCorBlack[DataYbit];
						R_line( xs, ys, xe, ye, GRAPH_DRAW, SOLID_LINE );
						xs--;
						xe--;
						if(( xs < 0 )||( xe < 0 ))	break;
					}
					InspOcOutsideCor[2][PosXmin+4] = xs;
					InspOcOutsideCor[2][PosXmax+4] = xe;
					InspOcOutsideCor[2][PosYmin+4] = ys;
					InspOcOutsideCor[2][PosYmax+4] = ye;
				// right upper side
					ymin = Overcoat.CornerPosY[PosRup];
					ymax = ymin+InspOcOffsetCorBlack[DataYbit];
					// --- position X
					elec = DataUL;
					side = PosXmax;
					//xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
					//xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
					xs = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-ymin);
					xe = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-ymax);

					xs -= InspOcOffsetCorBlack[DataXbit];
					xe -= InspOcOffsetCorBlack[DataXbit];
					// --- position Y
					elec = DataUpper;
					side = PosYmax;
					ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xs);
					ye = ys+InspOcOffsetCorBlack[DataYbit];
					InspOcOutsideCor[1][PosXmin] = xs;
					InspOcOutsideCor[1][PosXmax] = xe;
					InspOcOutsideCor[1][PosYmin] = ys;
					InspOcOutsideCor[1][PosYmax] = ye;
					// --- clear
					cls_w = abs(xs-xe)+InspOcOffsetCorBlack[DataXbit]+2;//10;
					for( dn=0; dn<cls_w; dn++ ) {
						ys = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xs);
						ye = ys+InspOcOffsetCorBlack[DataYbit];
						R_line( xs, ys, xe, ye, GRAPH_DRAW, SOLID_LINE );
						xs++;
						xe++;
						if(( xs >= FxSize )||( xe >= FxSize ))	break;
					}
					InspOcOutsideCor[1][PosXmin+4] = xs;
					InspOcOutsideCor[1][PosXmax+4] = xe;
					InspOcOutsideCor[1][PosYmin+4] = ys;
					InspOcOutsideCor[1][PosYmax+4] = ye;
				// right lower side
					ymax = Overcoat.CornerPosY[PosLlo];
					ymin = ymax-InspOcOffsetCorBlack[DataYbit];
					// --- position X
					elec = DataUL;
					side = PosXmax;
					//xs = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymin);
					//xe = ElecEdge[elec][side].MeanX-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanY-ymax);
					xs = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-ymin);
					xe = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-ymax);

					xs -= InspOcOffsetCorBlack[DataXbit];
					xe -= InspOcOffsetCorBlack[DataXbit];
					// --- position Y
					elec = DataLower;
					side = PosYmin;
					ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xe);
					ys = ye-InspOcOffsetCorBlack[DataYbit];
					InspOcOutsideCor[3][PosXmin] = xs;
					InspOcOutsideCor[3][PosXmax] = xe;
					InspOcOutsideCor[3][PosYmin] = ys;
					InspOcOutsideCor[3][PosYmax] = ye;
					// --- clear
					cls_w = abs(xs-xe)+InspOcOffsetCorBlack[DataXbit]+2;//10;
					for( dn=0; dn<cls_w; dn++ ) {
						ye = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xe);
						ys = ye-InspOcOffsetCorBlack[DataYbit];
						R_line( xs, ys, xe, ye, GRAPH_DRAW, SOLID_LINE );
						xs++;
						xe++;
						if(( xs >= FxSize )||( xe >= FxSize ))	break;
					}
					InspOcOutsideCor[3][PosXmin+4] = xs;
					InspOcOutsideCor[3][PosXmax+4] = xe;
					InspOcOutsideCor[3][PosYmin+4] = ys;
					InspOcOutsideCor[3][PosYmax+4] = ye;

				}
			// Graph page set
				Graph_page = bak_graph_page;

			// Draw Overcoat bin pn
			// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
				// --- area( bin measure )
				area.left	= Overcoat.Pos[PosXmin];
				area.right	= Overcoat.Pos[PosXmax];
				area.top	= Overcoat.Pos[PosYmin];
				area.bottom	= Overcoat.Pos[PosYmax];
				R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
				// --- area
				for( side=0; side<4; side++ ) {
					xmin = InspOcOutside[side][PosXmin];
					xmax = InspOcOutside[side][PosXmax];
					ymin = InspOcOutside[side][PosYmin];
					ymax = InspOcOutside[side][PosYmax];
					R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, DASHED_LINE );
					xmin = InspOcOutside[side][PosXmin+4];
					xmax = InspOcOutside[side][PosXmax+4];
					ymin = InspOcOutside[side][PosYmin+4];
					ymax = InspOcOutside[side][PosYmax+4];
					R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, DASHED_LINE );
				}
				// --- corner
				for( side=0; side<4; side++ ) {
					// --- line Xs
					xs = InspOcOutsideCor[side][PosXmin];
					xe = InspOcOutsideCor[side][PosXmax];
					ys = InspOcOutsideCor[side][PosYmin];
					ye = InspOcOutsideCor[side][PosYmax];
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_NORMAL, DASHED_LINE );
					// --- line Xe
					xs = InspOcOutsideCor[side][PosXmin+4];
					xe = InspOcOutsideCor[side][PosXmax+4];
					ys = InspOcOutsideCor[side][PosYmin+4];
					ye = InspOcOutsideCor[side][PosYmax+4];
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_NORMAL, DASHED_LINE );
					// --- line Ys
					xs = InspOcOutsideCor[side][PosXmin];
					xe = InspOcOutsideCor[side][PosXmin+4];
					ys = InspOcOutsideCor[side][PosYmin];
					ye = InspOcOutsideCor[side][PosYmin+4];
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_NORMAL, DASHED_LINE );
					// --- line Ye
					xs = InspOcOutsideCor[side][PosXmax];
					xe = InspOcOutsideCor[side][PosXmax+4];
					ys = InspOcOutsideCor[side][PosYmax];
					ye = InspOcOutsideCor[side][PosYmax+4];
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_NORMAL, DASHED_LINE );
				}
				// --- comment
				sprintf( Comment, "Noise clear\nInsp.OvercoatBlack BinP:%d (level:%d)", binPn, OcoatInspLevelBlack.nData );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
				R_DrawCls();
			}

	}	// InspBinSetOvercoatBlack( void ) end


//================================================
//		メッキ付着検査（側面）
//================================================

int		InspPlating( void )
	{
		int						grayPn	= InspGrayPn;
		int						binPn	= InspBinPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						side;
		int						xmin, xmax, xsize, rx, xsizeM;
		int						ymin, ymax, ysize, ry, ysizeM;
		int						bn, d_color;
		int						res;
		R_RECT					area;

		// Insp. leve set
			if( PlatingInspLevelSet() != OK )	return ERROR;

		// Plating pos.
			//v2.20 if(!bIsLTR50){	// normal
			if( (!bIsLTR50) && (!bIsLTR18) ){	// normal
				// --- upper side
				side = 0;
				// --- Upper left
				InspPlatingPos[side][PosXmin]	= InspOcOutsideCor[0][PosXmax+4];
				InspPlatingPos[side][PosYmin]	= InspOcOutsideCor[0][PosYmax+4];
				InspPlatingPos[side][PosXmin+4]	= InspOcOutsideCor[0][PosXmin+4];
				InspPlatingPos[side][PosYmin+4]	= InspOcOutsideCor[0][PosYmin+4];
				// --- Upper right
				InspPlatingPos[side][PosXmax]	= InspOcOutsideCor[1][PosXmin+4];
				InspPlatingPos[side][PosYmax]	= InspOcOutsideCor[1][PosYmin+4];
				InspPlatingPos[side][PosXmax+4]	= InspOcOutsideCor[1][PosXmax+4];
				InspPlatingPos[side][PosYmax+4]	= InspOcOutsideCor[1][PosYmax+4];
				// --- lower side
				side = 1;
				// --- Lower left
				InspPlatingPos[side][PosXmin]	= InspOcOutsideCor[2][PosXmax+4];
				InspPlatingPos[side][PosYmin]	= InspOcOutsideCor[2][PosYmax+4];
				InspPlatingPos[side][PosXmin+4]	= InspOcOutsideCor[2][PosXmin+4];
				InspPlatingPos[side][PosYmin+4]	= InspOcOutsideCor[2][PosYmin+4];
				// --- Lower right
				InspPlatingPos[side][PosXmax]	= InspOcOutsideCor[3][PosXmin+4];
				InspPlatingPos[side][PosYmax]	= InspOcOutsideCor[3][PosYmin+4];
				InspPlatingPos[side][PosXmax+4]	= InspOcOutsideCor[3][PosXmax+4];
				InspPlatingPos[side][PosYmax+4]	= InspOcOutsideCor[3][PosYmax+4];
			} else {		// LTR
				// --- left side
				side = 0;
				// --- Left upper
				InspPlatingPos[side][PosXmin]	= InspOcOutsideCor[0][PosXmax+4];
				InspPlatingPos[side][PosYmin]	= InspOcOutsideCor[0][PosYmax+4];
				InspPlatingPos[side][PosXmin+4]	= InspOcOutsideCor[0][PosXmin+4];
				InspPlatingPos[side][PosYmin+4]	= InspOcOutsideCor[0][PosYmin+4];
				// --- Left lower
				InspPlatingPos[side][PosXmax]	= InspOcOutsideCor[2][PosXmin+4];
				InspPlatingPos[side][PosYmax]	= InspOcOutsideCor[2][PosYmin+4];
				InspPlatingPos[side][PosXmax+4]	= InspOcOutsideCor[2][PosXmax+4];
				InspPlatingPos[side][PosYmax+4]	= InspOcOutsideCor[2][PosYmax+4];
				// --- lower side
				side = 1;
				// --- Right upper
				InspPlatingPos[side][PosXmin]	= InspOcOutsideCor[1][PosXmax+4];
				InspPlatingPos[side][PosYmin]	= InspOcOutsideCor[1][PosYmax+4];
				InspPlatingPos[side][PosXmin+4]	= InspOcOutsideCor[1][PosXmin+4];
				InspPlatingPos[side][PosYmin+4]	= InspOcOutsideCor[1][PosYmin+4];
				// --- Right lower
				InspPlatingPos[side][PosXmax]	= InspOcOutsideCor[3][PosXmin+4];
				InspPlatingPos[side][PosYmax]	= InspOcOutsideCor[3][PosYmin+4];
				InspPlatingPos[side][PosXmax+4]	= InspOcOutsideCor[3][PosXmax+4];
				InspPlatingPos[side][PosYmax+4]	= InspOcOutsideCor[3][PosYmax+4];
			}
			// Insp. Bin page set
			InspBinSetPlating();
			// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
				for( side=0; side<2; side++ ) {
					xmin = InspPlatingPos[side][PosXmin];
					xmax = InspPlatingPos[side][PosXmax];
					ymin = InspPlatingPos[side][PosYmin];
					ymax = InspPlatingPos[side][PosYmax];
					if( side == 0 ) {
						ymin -= InspPlOffsetSize[DataYbit];
						ymax -= InspPlOffsetSize[DataYbit];
					} else {
						ymin -= InspPlOffsetSize[DataXbit];
						ymax -= InspPlOffsetSize[DataXbit];
					}
					R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, DASHED_LINE );
					ymin = InspPlatingPos[side][PosYmin]+InspPlOffsetSize[DataYbit];
					ymax = InspPlatingPos[side][PosYmax]+InspPlOffsetSize[DataYbit];
					if( side == 0 ) {
						ymin += InspPlOffsetSize[DataXbit];
						ymax += InspPlOffsetSize[DataXbit];
					} else {
						ymin += InspPlOffsetSize[DataYbit];
						ymax += InspPlOffsetSize[DataYbit];
					}
					R_DrawLine( DRAW_GREEN, xmin, ymin, xmax, ymax, DRAW_NORMAL, DASHED_LINE );
					// --- Insp. area
						area.left	= InspPlatingArea[side][PosXmin];
						area.right	= InspPlatingArea[side][PosXmax];
						area.top	= InspPlatingArea[side][PosYmin];
						area.bottom	= InspPlatingArea[side][PosYmax];
						R_DrawBox( DRAW_YELLOW, area, GRAPH_DRAW, SOLID_LINE );
				}
				// --- comment
				sprintf( Comment, "Inspe. Plating adhesion" );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				R_DrawCls();
			}

		// Binary measure
		// 特徴量演算　演算方式指定
			R_set_meas_configration( &Mpx, BinMeasPlating.nRun, BinMeasPlating.nArea, BinMeasPlating.nColor, BinMeasPlating.nMode );
		// Plating measure
			res = OK;
			for( side=0; side<2; side++ ) {
				// --- Measure area
				xmin = InspPlatingArea[side][PosXmin];
				xmax = InspPlatingArea[side][PosXmax];
				ymin = InspPlatingArea[side][PosYmin];
				ymax = InspPlatingArea[side][PosYmax];
				xsize = xmax-xmin+1;
				ysize = ymax-ymin+1;
				// --- Measure
				if( R_measure( &Mpx, binPn, xmin, ymin, xsize, ysize, GROUP2 ) != OK ) {
					R_measure_close( &Mpx );
					R_measure_open( &Mpx, 2000 );
					DrawMessage( "ERROR !!\nR measure", DRAW_RED, 2, 2 );
					return ERROR;
				}
				// Inspection
				for( bn=1; bn<=Mpx.bn; bn++ ) {
					if( Mpx.color[bn] == ColorBlack )	continue;
					// --- Judge
					if( Mpx.xdiff[bn] > InspPara.Plating[TWA_Spec].nXsize )	res = InspPara.Plating[TWA_Spec].nCode;
					if( Mpx.ydiff[bn] > InspPara.Plating[TWA_Spec].nYsize )	res = InspPara.Plating[TWA_Spec].nCode;
					// --- Draw blob
					if( i_mode & INSPECT_STEP ) {
						// --- data
						xsize = Mpx.imax[bn] - Mpx.imin[bn] + 1;
						ysize = Mpx.jmax[bn] - Mpx.jmin[bn] + 1;
						// --- color
						if( res != OK )	d_color = DRAW_RED;
						else			d_color = DRAW_YELLOW;
						// --- X line
						ry		= Mpx.center_y[bn];
						xmin	= Mpx.imin[bn];
						xmax	= Mpx.imax[bn];
						R_DrawLine( d_color, xmin, ry, xmax, ry, DRAW_NORMAL, SOLID_LINE );
						// --- Y line
						rx		= Mpx.center_x[bn];
						ymin	= Mpx.jmin[bn];
						ymax	= Mpx.jmax[bn];
						R_DrawLine( d_color, rx, ymin, rx, ymax, DRAW_NORMAL, SOLID_LINE );
						// --- massage
//						sprintf( Comment, "Measure X:%3d(bit) Y:%3d(bit)", xsize, ysize );
						xsizeM = (xsize*SystemPara.nRateX+500)/1000;
						ysizeM = (ysize*SystemPara.nRateY+500)/1000;
						sprintf( Comment1, "Measure X:%4d(bit) Y:%4d(bit)", xsize, ysize );
						sprintf( Comment2, "        X:%4d(um)  Y:%4d(um) ", xsizeM, ysizeM );
						sprintf( Comment, "%s\n%s", Comment1, Comment2 );
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					}
					// result
					if( res != OK )	return( res );
				}
			}

		// result
			return( res );

	}	// InspPlating( void ) end


//================================================
//		メッキ付着検査レベル設定
//================================================

int		PlatingInspLevelSet( void )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						side;
		int						thr[2];
		R_RECT					area;

		// Insp. bin level set
			// --- Gray memory
			InspPlLSetPara.nGrayPn = grayPn;
			// --- area
			side = 0;
			//v2.20 if(!bIsLTR50){		// normal
			if( (!bIsLTR50) && (!bIsLTR18) ){		// normal
				// --- Upper left
				InspPlatingArea[side][PosXmin] = InspOcOutsideCor[0][PosXmax+4];
				InspPlatingArea[side][PosYmin] = min(InspOcOutsideCor[0][PosYmax+4],InspOcOutsideCor[1][PosYmin+4]);
				InspPlatingArea[side][PosYmin] -= InspPlOffsetSize[DataYbit];
				// --- Upper right
				InspPlatingArea[side][PosXmax] = InspOcOutsideCor[1][PosXmin+4];
				InspPlatingArea[side][PosYmax] = max(InspOcOutsideCor[0][PosYmax+4],InspOcOutsideCor[1][PosYmin+4]);
				InspPlatingArea[side][PosYmax] += InspPlOffsetSize[DataXbit];
				// --- area
				side = 1;
				// --- Lower left
				InspPlatingArea[side][PosXmin] = InspOcOutsideCor[2][PosXmax+4];
				InspPlatingArea[side][PosYmin] = min(InspOcOutsideCor[2][PosYmax+4],InspOcOutsideCor[3][PosYmin+4]);
				InspPlatingArea[side][PosYmin] -= InspPlOffsetSize[DataXbit];
				// --- Lower right
				InspPlatingArea[side][PosXmax] = InspOcOutsideCor[3][PosXmin+4];
				InspPlatingArea[side][PosYmax] = max(InspOcOutsideCor[2][PosYmax+4],InspOcOutsideCor[3][PosYmin+4]);
				InspPlatingArea[side][PosYmax] += InspPlOffsetSize[DataYbit];
			} else {			// LTR
				// --- left upper
				InspPlatingArea[side][PosYmin] = InspOcOutsideCor[0][PosYmax+4];
				InspPlatingArea[side][PosXmin] = min(InspOcOutsideCor[0][PosXmax+4],InspOcOutsideCor[2][PosXmin+4]);
				InspPlatingArea[side][PosXmin] -= InspPlOffsetSize[DataXbit];
				// --- left lower
				InspPlatingArea[side][PosYmax] = InspOcOutsideCor[2][PosYmin+4];
				InspPlatingArea[side][PosXmax] = max(InspOcOutsideCor[0][PosXmax+4],InspOcOutsideCor[2][PosXmin+4]);
				InspPlatingArea[side][PosXmax] += InspPlOffsetSize[DataXbit];
				// --- area
				side = 1;
				// --- right upper
				InspPlatingArea[side][PosYmin] = InspOcOutsideCor[1][PosYmax+4];
				InspPlatingArea[side][PosXmin] = min(InspOcOutsideCor[1][PosXmax+4],InspOcOutsideCor[3][PosXmin+4]);
				InspPlatingArea[side][PosXmin] -= InspPlOffsetSize[DataXbit];
				// --- right lower
				InspPlatingArea[side][PosYmax] = InspOcOutsideCor[3][PosYmin+4];
				InspPlatingArea[side][PosXmax] = max(InspOcOutsideCor[1][PosXmax+4],InspOcOutsideCor[3][PosXmin+4]);
				InspPlatingArea[side][PosXmax] += InspPlOffsetSize[DataXbit];
			}

		// Insp. level set
			for( side=0; side<2; side++ ) {
				// --- area
				InspPlLSetPara.nXmin = InspPlatingArea[side][PosXmin];
				InspPlLSetPara.nXmax = InspPlatingArea[side][PosXmax];
				InspPlLSetPara.nYmin = InspPlatingArea[side][PosYmin];
				InspPlLSetPara.nYmax = InspPlatingArea[side][PosYmax];
				// ---
				InspPlLSetPara.nStartLevel	= OcoatInspLevel.nData/2;				// ヒストグラムの計算範囲の開始点(start>=0)
				InspPlLSetPara.nEndLevel	= 250;									// ヒストグラムの計算範囲の終了点
				InspPlLSetPara.nMethod		= THRESH_OTSU;							// しきい値選定の方法
				// --- level set
				thr[side] = BinLevelSet( &InspPlLSetPara );
				if( OK == thr[side] ) {
					thr[side] = InspPlLSetPara.nLevel;
					if( i_mode & INSPECT_STEP ) {
						sprintf( Comment, "Plating Insp.level(side:%d) %d", side, thr[side] );
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					}
				}
			}
			// --- Check level
				if(( 0 > thr[0] )&&( 0 > thr[1] )) {
					// --- comment
					if( i_mode & INSPECT_STEP ) {
						sprintf( Comment, " === ERROR === \n Setup of Insp. Plating bin level was not completed. " );
						DrawMessage( Comment, DRAW_RED, 2, 2 );
					}
					InspPlLSetPara.nLevel = PlatingInspLevel.nLower;
				} else if( 0 > thr[0] ) {
					InspPlLSetPara.nLevel = thr[1];
				} else if( 0 > thr[1] ) {
					InspPlLSetPara.nLevel = thr[0];
				} else {
					InspPlLSetPara.nLevel = min( thr[0], thr[1] );
				}
			// --- Insp. level
			PlatingInspLevel.nData = InspPlLSetPara.nLevel + PlatingInspLevel.nOffset;
			if( PlatingInspLevel.nData < PlatingInspLevel.nLower )	PlatingInspLevel.nData = PlatingInspLevel.nLower;
			if( PlatingInspLevel.nData > PlatingInspLevel.nUpper )	PlatingInspLevel.nData = PlatingInspLevel.nUpper;
			// --- view
				if( i_mode & INSPECT_STEP ) {
					R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
					for( side=0; side<2; side++ ) {
						area.left	= InspPlatingArea[side][PosXmin];
						area.right	= InspPlatingArea[side][PosXmax];
						area.top	= InspPlatingArea[side][PosYmin];
						area.bottom	= InspPlatingArea[side][PosYmax];
						R_DrawBox( DRAW_YELLOW, area, GRAPH_DRAW, SOLID_LINE );
					}
					sprintf( Comment, "Insp. level (Plating:%d)", PlatingInspLevel.nData );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					R_DrawCls();
				}

		// result
			return OK;

	}	// PlatingInspLevelSet( void ) end


//================================================
//		オーバーコート検査
//			検査２値画像、検査範囲設定
//================================================

void	InspBinSetPlating( void )
	{
		int						grayPn	= InspGrayPn;
		int						binPn	= InspBinPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						side, dn;
		int						xmin, xmax, xsize, xs, xe;
		int						ymin, ymax, ysize, ys, ye;
		int						cls_w;
		int						bak_graph_page	= Graph_page;
		int						cls_area[2][8];
		R_RECT					area;
        BIN_FILTER_PARAMETER	BinFilter;    // Filter condition

		// Initialze
			BinClsPn( binPn, 0, 0, FxSize-1, FySize-1 );

		// Plating Gray --> Bin
			for( side=0; side<2; side++ ) {
				xmin = InspPlatingArea[side][PosXmin];
				xmax = InspPlatingArea[side][PosXmax];
				ymin = InspPlatingArea[side][PosYmin];
				ymax = InspPlatingArea[side][PosYmax];
				if(xmin < 0       )	xmin = 0;
				if(xmax > FxSize-1) xmax = FxSize-1;
				if(ymin < 0       ) ymin = 0;
				if(ymax > FySize-1) ymax = FySize-1;
				xsize	= xmax-xmin+1;
				ysize	= ymax-ymin+1;
				R_gray2bin( PlatingInspLevel.nData, xmin, ymin, xsize, ysize, Gray_address[grayPn], Bin_address[binPn] );
			}
			// Draw Plating bin pn
			// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
				// --- comment
				sprintf( Comment, "Insp. Plating BinP:%d (level:%d)", binPn, PlatingInspLevel.nData );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
			}

		// Outside clear
			// Graph page set
				Graph_page = binPn;
			// upper side
				side = 0;
				xmin = InspPlatingPos[side][PosXmin+4];
				xmax = InspPlatingPos[side][PosXmax+4];
				ys = InspPlatingPos[side][PosYmin+4]-InspPlOffsetSize[DataYbit];
				ye = InspPlatingPos[side][PosYmax+4]-InspPlOffsetSize[DataYbit];
				cls_area[side][PosXmin] = xmin;
				cls_area[side][PosXmax] = xmax;
				cls_area[side][PosYmin] = ys;
				cls_area[side][PosYmax] = ye;
				// --- clear
				cls_w = abs(ys-ye)+InspPlOffsetSize[DataYbit]+10;
				for( dn=0; dn<cls_w; dn++ ) {
					R_line( xmin, ys, xmax, ye, GRAPH_ERASE, SOLID_LINE );
					ys--;
					ye--;
					if(( ys < 0 )||( ye < 0 ))	break;
				}
				cls_area[side][PosXmin+4] = xmin;
				cls_area[side][PosXmax+4] = xmax;
				cls_area[side][PosYmin+4] = ys;
				cls_area[side][PosYmax+4] = ye;
			// lower side
				side = 1;
				xmin = InspPlatingPos[side][PosXmin+4];
				xmax = InspPlatingPos[side][PosXmax+4];
				ys = InspPlatingPos[side][PosYmin+4]+InspPlOffsetSize[DataYbit];
				ye = InspPlatingPos[side][PosYmax+4]+InspPlOffsetSize[DataYbit];
				cls_area[side][PosXmin] = xmin;
				cls_area[side][PosXmax] = xmax;
				cls_area[side][PosYmin] = ys;
				cls_area[side][PosYmax] = ye;
				// --- clear
				cls_w = abs(ys-ye)+InspPlOffsetSize[DataYbit]+10;
				for( dn=0; dn<cls_w; dn++ ) {
					R_line( xmin, ys, xmax, ye, GRAPH_ERASE, SOLID_LINE );
					ys++;
					ye++;
					if(( ys >= FySize )||( ye >= FySize ))	break;
				}
				cls_area[side][PosXmin+4] = xmin;
				cls_area[side][PosXmax+4] = xmax;
				cls_area[side][PosYmin+4] = ys;
				cls_area[side][PosYmax+4] = ye;

			// Graph page set
				Graph_page = bak_graph_page;

		// noise clear
			BinFilter.nBinPn = binPn;
			for( side=0; side<2; side++ ) {
				BinFilter.nXmin = InspPlatingArea[side][PosXmin];
				BinFilter.nXmax = InspPlatingArea[side][PosXmax];
				BinFilter.nYmin = InspPlatingArea[side][PosYmin];
				BinFilter.nYmax = InspPlatingArea[side][PosYmax];
				// --- x size 5
				// --- y size 5
				FilterBinExpand(  &BinFilter, 5, 5 );
				FilterBinContrct( &BinFilter, 5, 5 );
			}
			// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
				// --- comment
				sprintf( Comment, "Noise clear \n Insp. Plating BinP:%d (level:%d)", binPn, PlatingInspLevel.nData );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
			}

			// Draw Plating bin pn
			// --- Viwe
			if( i_mode & INSPECT_STEP ) {
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
				// --- area( bin measure )
				area.left	= Overcoat.Pos[PosXmin];
				area.right	= Overcoat.Pos[PosXmax];
				area.top	= Overcoat.Pos[PosYmin];
				area.bottom	= Overcoat.Pos[PosYmax];
				R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
				// --- area
				for( side=0; side<2; side++ ) {
					// --- line Ys
					xs = cls_area[side][PosXmin];
					ys = cls_area[side][PosYmin];
					xe = cls_area[side][PosXmax];
					ye = cls_area[side][PosYmax];
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_NORMAL, DASHED_LINE );
					// --- line Ye
					xs = cls_area[side][PosXmin+4];
					ys = cls_area[side][PosYmin+4];
					xe = cls_area[side][PosXmax+4];
					ye = cls_area[side][PosYmax+4];
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_NORMAL, DASHED_LINE );
					// --- line Xs
					xs = cls_area[side][PosXmin];
					ys = cls_area[side][PosYmin];
					xe = cls_area[side][PosXmin+4];
					ye = cls_area[side][PosYmin+4];
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_NORMAL, DASHED_LINE );
					// --- line Xe
					xs = cls_area[side][PosXmax];
					ys = cls_area[side][PosYmax];
					xe = cls_area[side][PosXmax+4];
					ye = cls_area[side][PosYmax+4];
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_NORMAL, DASHED_LINE );
				}
				// --- comment
				sprintf( Comment, "Insp.Plating BinP:%d (level:%d)", binPn, PlatingInspLevel.nData );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
				R_DrawCls();
			}

	}	// InspBinSetPlating( void ) end


//------------------------------------------------
//	BinLevelSet
//	戻り値
//		-1 < 	正常終了
//		ERROR	メモリーERROR
//------------------------------------------------

int		BinLevelSet( THRESHOLD_PARAMETER *pThresh )
	{
		int						grayPn = pThresh->nGrayPn;
		int						thr;
		int						flag = 0;
		R_RECT					area;
		int						i_mode	= R_CS_GetInspectMode();

		// Initialze
			if( OK == R_bgray_thresholding_open( THRESH_DEPTH_8BIT ))	flag = 1;

		// 明るさのヒストグラム
			if( ERROR == R_get_histgram( pThresh->nXmin, pThresh->nYmin, pThresh->nXmax, pThresh->nYmax, Gray_address[grayPn] ) )	return ERROR;
		// ２値化レベルの取得
			thr = R_bgray_thresholding( Bright, pThresh->nStartLevel, pThresh->nEndLevel, pThresh->nMethod );
			if(flag)	R_bgray_thresholding_close();
			if( thr < 0 )	return( thr );
		// ２値化レベル設定
			pThresh->nLevel = thr;
			if( i_mode & INSPECT_STEP ) {
                R_rect_init_by_point(&area,pThresh->nXmin, pThresh->nYmin , pThresh->nXmax, pThresh->nYmax );
				R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, DOTTED_LINE );
				R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, 0, 31, "thr    : %d",thr);
				while( !R_DrawGetCursor());
				R_DrawCls();
				while( R_DrawGetCursor());
			}
		// result
			return OK;

	}	// BinLevelSet( THRESHOLD_PARAMETER *pThresh ) end

int		BinLevelSet_WithMask( THRESHOLD_PARAMETER *pThresh ,R_RECT *aMask)
	{
		int						grayPn = pThresh->nGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						sBright[256];
		int						i;
		int						thr;
		int						flag=0;
		R_RECT					area;

		// Initialze
			if( OK == R_bgray_thresholding_open( THRESH_DEPTH_8BIT ))	flag = 1;

		// 明るさのヒストグラム
			//if( ERROR == R_get_histgram( pThresh->nXmin, pThresh->nYmin, pThresh->nXmax, pThresh->nYmax, Gray_address[grayPn] ) )	return ERROR;
			if( ERROR == R_get_histgram( pThresh->nXmin, pThresh->nYmin , aMask->left-1 , pThresh->nYmax, Gray_address[grayPn] ) )	return ERROR;
			for(i=0;i<256;i++)	sBright[i]=Bright[i];
			if( ERROR == R_get_histgram( aMask->left   , pThresh->nYmin , aMask->right  , aMask->top-1  , Gray_address[grayPn] ) )	return ERROR;
			for(i=0;i<256;i++)	sBright[i]+=Bright[i];
			if( ERROR == R_get_histgram( aMask->right+1, pThresh->nYmin , pThresh->nXmax, pThresh->nYmax, Gray_address[grayPn] ) )	return ERROR;
			for(i=0;i<256;i++)	sBright[i]+=Bright[i];
			if( ERROR == R_get_histgram( aMask->left   , aMask->bottom+1 ,aMask->right  , pThresh->nYmax, Gray_address[grayPn] ) )	return ERROR;
			for(i=0;i<256;i++)	sBright[i]+=Bright[i];

			if( i_mode & INSPECT_STEP ) {
			//	BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
				R_rect_init_by_point(&area,pThresh->nXmin, pThresh->nYmin , aMask->left-1 , pThresh->nYmax);
				R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, DOTTED_LINE );
				R_rect_init_by_point(&area,aMask->left   , pThresh->nYmin , aMask->right  , aMask->top-1  );
				R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, DOTTED_LINE );
				R_rect_init_by_point(&area,aMask->right+1, pThresh->nYmin , pThresh->nXmax, pThresh->nYmax);
				R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, DOTTED_LINE );
				R_rect_init_by_point(&area,aMask->left   , aMask->bottom+1 ,aMask->right  , pThresh->nYmax);
				R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, DOTTED_LINE );

				while( !R_DrawGetCursor());
				R_DrawCls();
				while( R_DrawGetCursor());


			}




		// ２値化レベルの取得
			thr = R_bgray_thresholding( sBright, pThresh->nStartLevel, pThresh->nEndLevel, pThresh->nMethod );
			if(flag)	R_bgray_thresholding_close();
			if( thr < 0 )	return( thr );
		// ２値化レベル設定
			pThresh->nLevel = thr;

		// result
			return OK;

	}	// BinLevelSet( THRESHOLD_PARAMETER *pThresh ) end


/**********************************************************
	関数名：	ＳｐｅｃＳｅｔＢａｃｋＳｉｄｅ
	機  能：
	形  式：	int SpecSetBackSide( int nMenuNumber )
	引き数：
	戻り値：
	解  説：
	留意事項：
**********************************************************/

int		SpecSetBackSide( int nMenuNumber )
	{
		return OK;

	}	// SpecSetBackSide( int nMenuNumber ) end




/////////////////////////////////
//  Data_Lot_count 2011.01.29  //
/////////////////////////////////
/*
int		Data_Lot_count( void )
	{
		int						n, m, dn; //2012.03.14
		R_CS_COUNTER_DATA		*pCounter	= R_CS_GetCounterDataStruct(0);			// カウンタ構造体へのポインタ取得

		// Clear flag check
                if( Clear_flag == 1 ){
					for(n=0; n<20; n++){
						insp_sum1[n]=0;
						count_total[n]=0, count_R[n]=0, count_N[n]=0, count_L[n]=0, count_Re[n]=0;
						pass_sum[n] = 0;                                               // Count Go
						for (dn=0; dn<100; dn++){
			                count1[n][dn] = 0;
						}
					}
					Lot_num = 0;
				}
					Clear_flag = 0;

		// Counter
				if( Lot_num >= 0 ){
						n = Lot_num;
//					for( m=1; m<=9; m++){
//						if(( n-m ) == 0)  continue;
						insp_sum1[n] = R_CS_CounterGetTotalCount( pCounter )+R_CS_CounterGetNgCount( pCounter, MC_RESISTANCE );
						pass_sum[n] = R_CS_CounterGetGoCount( pCounter );                                 		 // Pass Count
						for( dn=0; dn<100; dn++ ) {
							if( 10 >= NgCode1[dn].nCode )	continue;
							if( 80 <  NgCode1[dn].nCode )	continue;
							count1[n][dn] = R_CS_CounterGetNgCount(pCounter, NgCode1[dn].nCode);                 // NG Mode Count
						}
							count_total[n] = R_CS_CounterGetNgTotalCount( pCounter );                            // NG Total Count
						// MC_RESISTANCE
							count_R[n] = R_CS_CounterGetNgCount(pCounter, MC_RESISTANCE);
						// MC_NINSHIKI
							count_N[n] = R_CS_CounterGetNgCount(pCounter, MC_NINSHIKI);
						// NG_LIGHTING
							count_L[n] = R_CS_CounterGetNgCount(pCounter, NG_LIGHTING);
						// NG_REVERSE
							count_Re[n]= R_CS_CounterGetNgCount(pCounter, NG_REVERSE);
//					}
				}

		// Counter
				if( Lot_num >= 0 ){
						n = Lot_num;
					for( m=1; m<=19; m++){
						if((n-m)<0) break;
//						insp_sum1[n] = R_CS_CounterGetTotalCount( pCounter )+R_CS_CounterGetNgCount( pCounter, MC_RESISTANCE )-insp_sum1[n-m];
						insp_sum1[n] = insp_sum1[n]-insp_sum1[n-m];
//					}
//					for( m=1; m<=9; m++){
//						if((n-m)<0) break;
//						pass_sum[n] = R_CS_CounterGetGoCount( pCounter )-pass_sum[n-m];                                 		 // Pass Count
						pass_sum[n] = pass_sum[n]-pass_sum[n-m];                                 		 // Pass Count
//					}
//					for( m=1; m<=9; m++){
//						if((n-m)<0) break;
						for( dn=0; dn<100; dn++ ) {
							if( 10 >= NgCode1[dn].nCode )	continue;
							if( 80 <  NgCode1[dn].nCode )	continue;
//							count1[n][dn] = R_CS_CounterGetNgCount(pCounter, NgCode1[dn].nCode)-count1[n-m][dn];                 // NG Mode Count
							count1[n][dn] = count1[n][dn]-count1[n-m][dn];                 // NG Mode Count
						}
//					}
//					for( m=1; m<=9; m++){
//						if((n-m)<0) break;
*/
/*							count_total[n] = R_CS_CounterGetNgTotalCount( pCounter )-count_total[n-m];                            // NG Total Count
						// MC_RESISTANCE
							count_R[n] = R_CS_CounterGetNgCount(pCounter, MC_RESISTANCE)-count_R[n-m];
						// MC_NINSHIKI
							count_N[n] = R_CS_CounterGetNgCount(pCounter, MC_NINSHIKI)-count_N[n-m];
						// NG_LIGHTING
							count_L[n] = R_CS_CounterGetNgCount(pCounter, NG_LIGHTING)-count_L[n-m];
						// NG_REVERSE
							count_Re[n]= R_CS_CounterGetNgCount(pCounter, NG_REVERSE)-count_Re[n-m];
*//*							count_total[n] = count_total[n]-count_total[n-m];                            // NG Total Count
						// MC_RESISTANCE
							count_R[n] = count_R[n]-count_R[n-m];
						// MC_NINSHIKI
							count_N[n] = count_N[n]-count_N[n-m];
						// NG_LIGHTING
							count_L[n] = count_L[n]-count_L[n-m];
						// NG_REVERSE
							count_Re[n]= count_Re[n]-count_Re[n-m];
					}
				}

		return OK;

	}	// Data_Lot_count( void ) end
*/
/************************************************************
	NG Limit Check   //2011.08.17
************************************************************/

int NG_Limit_Check( void )
{
		int n, nm;

		for(nm=0; nm<10; nm++){
			for(n=0; n<100; n++){
				 if( NG_Limit[1][n] == 0 ) continue;
				 if( count1[nm][n]  == 0 ) continue;
				 if( count_R[nm]    == 0 ) continue;
				 if( count_N[nm]    == 0 ) continue;
			     if(( count1[nm][n] > NG_Limit[1][n] )&&( NG_Limit_OnOff[n] != 0 )) {
						R_DrawPrintf( DRAW_RED, DISP_4, 2, 1, "ALARM : NG OCCURED!! %s", NgCode1[n].sMessEn);
						R_DrawPopupMessage(POPUP_WAIT_CLICK, "mouse click wait");
						R_DrawClsPosi( 0, 0, FxSize-1, FySize-1 );
						break;
				 }
				 if(n==81){
				     if(( count_R[nm] > NG_Limit[1][n] )&&( NG_Limit_OnOff[n] != 0 )) {
							R_DrawPrintf( DRAW_RED, DISP_4, 2, 1, "ALARM : NG OCCURED!! %s", NgCode1[n].sMessEn);
							R_DrawPopupMessage(POPUP_WAIT_CLICK, "mouse click wait");
							R_DrawClsPosi( 0, 0, FxSize-1, FySize-1 );
							break;
			 		 }
				 }
				 if(n==82){
				     if(( count_N[nm] > NG_Limit[1][n] )&&( NG_Limit_OnOff[n] != 0 )) {
							R_DrawPrintf( DRAW_RED, DISP_4, 2, 1, "ALARM : NG OCCURED!! %s", NgCode1[n].sMessEn);
							R_DrawPopupMessage(POPUP_WAIT_CLICK, "mouse click wait");
							R_DrawClsPosi( 0, 0, FxSize-1, FySize-1 );
							break;
					 }
				 }
			}
		}
		return OK;
}

/************************************************
	NGLimitDataLoadfile //2011.08.17
************************************************/

int		NGLimitDataLoadfile( void )
	{
		int					ans;
		int					dn;
		float				data;

		while( R_get_cursor() );

		for(dn=0; dn<100 ; dn++) NG_Limit_OnOff[dn] = 1;
		if( NULL!=(fin1=fopen( "LIMITNGI.csv", "r" ))) {		//NG Limit データファイルオープン
			data_read();
			dn = 0;
			//
			NG_Limit[0][dn] = data_read();					//Interval
			NG_Limit[1][dn] = 0;
			//NG項目別data
			for( dn=1; dn<100; dn++ ) {
				NG_Limit[0][dn] = data_read2();
			}
			fclose( fin1 );									//NG Limit データファイルクローズ

			for( dn=1; dn<100; dn++ ) {
				NG_Limit[1][dn] = 0;
				if( NG_Limit[0][dn] != 0 ) {
					data = NG_Limit[0][dn] / 10000.0;
					NG_Limit[1][dn] = NG_Limit[0][0] * data;
				}
			}
//			NG_Limit_can = 0;

			ans = OK;
		} else {
			R_display_control( BIN_MEMORY | GRAY_THROUGH );
            R_bin_cls( 0, 0, FxSize-1, FySize-1 );
			sprintf( Comment, "ERR. Read Out NG Limit." );		//NG Limit data 読み込みエラー
			DrawMessage( Comment, DRAW_RED, 2, 0 );
			fclose( fin1 );									//NG Limit データファイルクローズ

			ans = ERROR;
		}

		return(ans);
	}

/************************************************
	Data read
************************************************/

int		data_read( void )
	{
		char	s[50];
		fgets( s, sizeof s, fin1 );
			s[10] = 0;
		return( atoi(s) );
	}


/************************************************
	Data read 2
************************************************/

int		data_read2( void )
	{
		char				s[60];
		char				s1[10], s2[10];
		int					data;
		int					n;

		//Data format
		//0123456789
		//      1.00

		//data get
		fgets( s, sizeof s, fin1 );

		for( n=0; n<7; n++ ) s1[n] = s[n];
		s1[7] = 0;

		s2[0] = s[8];
		s2[1] = s[9];
		s2[2] = 0;

		data = atoi(s1) * 100 + atoi(s2);

		return( data );
	}

//------------------------------------------------
//	Cancle Poket sig.	(0x02)  //2012.04.19
//------------------------------------------------
/*
int		CanclePok_sig( void )
	{
		int						port;

		port = R_gpio_read(Drcy80Ch|CNa_IN_port_1);
		return( port & POK_SIG );

	}	// CheckInspStart( void ) end

/**********************************************************
	関数名：	ＰｋｇＰａｒａＳａｖｅＬｏｇ
	機  能：	パラメータ変更ログを保存する
	形  式：	void PkgParaSaveLog( void )
	引き数：
	戻り値：
	解  説：
	留意事項：
**********************************************************/
static void PkgParaSaveLog( void )
{
	char	ComMkdir[256];
	char	ComCpParameter[256];
	char	ComCpParameterDir[256];
	char	ParameterChangeDate[128];
	R_TIME_INFO RTimeInfo;

	//デバイスチェック実施日時の取得
	R_TimeInfoGetNow(&RTimeInfo);
	sprintf( ParameterChangeDate, "%4d-%02d-%02d_%02d%02d%02d",RTimeInfo.year,RTimeInfo.month,RTimeInfo.day,RTimeInfo.hour,RTimeInfo.minute,RTimeInfo.second );

	sprintf( ComMkdir,"mkdir ./Parameter/Log/%s",ParameterChangeDate );
	system( ComMkdir );
	sprintf( ComCpParameter,"cp TP-*.csv ./Parameter/Log/%s",ParameterChangeDate );
	system( ComCpParameter );
	//ver1.10 USBキーへの保存を追加
	//ＵＳＢへ同時アクセスするため、実機ではうまく動かず。
	/*if( OK == MountUSB()){
		sprintf( ComMkdir, "mkdir /mnt/usb/%s",MachineName );
		system( ComMkdir );
		sprintf( ComMkdir, "mkdir /mnt/usb/%s/Parameter",MachineName );
		system( ComMkdir );
		sprintf( ComMkdir, "mkdir /mnt/usb/%s/Parameter/Log",MachineName );
		system( ComMkdir );
		sprintf( ComCpParameterDir,"cp -R ./Parameter/Log/%s /mnt/usb/%s/Parameter/Log/", ParameterChangeDate, MachineName );
		system( ComCpParameterDir );
		UmountUSB();
	}*/
}


int twa_TimeDisp( TWA_TIME_RESULT *pPara )
{
	int offset = 11;


	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize+offset, FySize/Jph_ysize+2 , "[TIME]           :%8d[us]", (pPara->nTime[5]) );
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize+offset, FySize/Jph_ysize+3,  "OutJudge         :%8d[us]", (pPara->nTime[0] ) );
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize+offset, FySize/Jph_ysize+4,  "Capture>BusyOn   :%8d[us]", (pPara->nTime[1]-pPara->nTime[0]  ) );
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize+offset, FySize/Jph_ysize+5,  "WaitStartdown    :%8d[us]", (pPara->nTime[2]-pPara->nTime[1]  ) );
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize+offset, FySize/Jph_ysize+6,  "MoveSaveImage    :%8d[us]", (pPara->nTime[3]-pPara->nTime[2]  ) );
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize+offset, FySize/Jph_ysize+7,  "Inspection       :%8d[us]", (pPara->nTime[4]-pPara->nTime[3]  ) );
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize+offset, FySize/Jph_ysize+8,  "AfterInspection  :%8d[us]", (pPara->nTime[5]-pPara->nTime[4]  ) );


	/*
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize, FySize/Jph_ysize+7, "Display Image       :%8dmsec", (pPara->nTime[4]-pPara->nTime[3] + 500)/1000 );

	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize, FySize/Jph_ysize+3, "Capture   :%8dmsec", (pPara->nTime[2]-pPara->nTime[1] + 500)/1000 );
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize, FySize/Jph_ysize+4, "Inspect   :%8dmsec", (pPara->nTime[3]-pPara->nTime[2] + 500)/1000 );
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize, FySize/Jph_ysize+5, "Display   :%8dmsec", (pPara->nTime[4]-pPara->nTime[3] + 500)/1000 );
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize, FySize/Jph_ysize+6, "Etc       :%8dmsec", (pPara->nTime[5]-pPara->nTime[4] + 500)/1000 );
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize, FySize/Jph_ysize+7, "BusyOff   :%8dmsec", (pPara->nTime[6]-pPara->nTime[5] + 500)/1000 );
	*/
/*
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize, FySize/Jph_ysize  , "[TIME]    :%8dmsec", (pPara->nTime[6]+500)/1000 );
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize, FySize/Jph_ysize+1, "Start->Ls :%8dmsec", (pPara->nTime[0] + 500)/1000 );
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize, FySize/Jph_ysize+2, "Lock      :%8dmsec", (pPara->nTime[1]-pPara->nTime[0] + 500)/1000 );
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize, FySize/Jph_ysize+3, "Capture   :%8dmsec", (pPara->nTime[2]-pPara->nTime[1] + 500)/1000 );
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize, FySize/Jph_ysize+4, "Inspect   :%8dmsec", (pPara->nTime[3]-pPara->nTime[2] + 500)/1000 );
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize, FySize/Jph_ysize+5, "Display   :%8dmsec", (pPara->nTime[4]-pPara->nTime[3] + 500)/1000 );
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize, FySize/Jph_ysize+6, "Etc       :%8dmsec", (pPara->nTime[5]-pPara->nTime[4] + 500)/1000 );
	R_draw_printf(DRAW_WHITE, DRAW_NORMAL, FxSize/Jph_xsize, FySize/Jph_ysize+7, "BusyOff   :%8dmsec", (pPara->nTime[6]-pPara->nTime[5] + 500)/1000 );
*/
	return OK;
}

static void	FileInspSet(void)
{
        R_DrawWinbMenuFix( WINB_NON_CLOSE, &WinbSettingInspMenu );
}

/**********************************************************
	関数名：	ManualInspectFileAllReady
	機  能：	連続ファイルモード時の準備関数
	形  式：
	引き数：
	戻り値：
	解  説：
	留意事項：
**********************************************************/
int ManualInspectFileAllReady(int nMenuNumber){
	if(Option & ADMIN_MODE) R_pfile_sequential_set_menu( PFILE_SEQ_MENU3, "検査設定",     "InspectSetting", FileInspSet );

   return OK;
}
/*
int	GetBinG2Rough(int GrayPn)
{
	int flag = 0;
	int level,clevel;
	int						i_mode	= R_CS_GetInspectMode();
	R_RECT					area;

	//指定エリアのヒストグラムを取る。
	if(OK==R_bgray_thresholding_open(THRESH_DEPTH_8BIT))	flag = 1;

	R_get_histgram( (G2_pos[0]+G2_pos[1])/2-(G2_pos[1]-G2_pos[0])/4, G2_pos[2]-20, (G2_pos[0]+G2_pos[1])/2+(G2_pos[1]-G2_pos[0])/4, G2_pos[3]+20, Gray_address[GrayPn] );
	level = R_bgray_thresholding( Bright, 50, 255, THRESH_OTSU );
	clevel = level;
	if(clevel < 80)	clevel = 80;
	if(clevel > 120)	clevel = 120;

	if( i_mode & INSPECT_STEP ) {
		R_move_gray_memory( InspGrayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
		area.left	= (G2_pos[0]+G2_pos[1])/2-(G2_pos[1]-G2_pos[0])/4;
		area.right	= (G2_pos[0]+G2_pos[1])/2+(G2_pos[1]-G2_pos[0])/4;
		area.top	= G2_pos[2]-20;
		area.bottom	= G2_pos[3]+20;
		R_DrawBox( DRAW_YELLOW, area, GRAPH_DRAW, SOLID_LINE );
		sprintf( Comment, "Insp. level (Ocoat:%d(%d))", clevel,level );
		DrawMessage( Comment, DRAW_GREEN, 2, 2 );
		R_DrawCls();
	}
	if(flag)	R_bgray_thresholding_close();

	return level;
}
*/
//================================================
//	G2 edge search level set
//================================================

int		G2EsLevelSet( int side )
	{
		int						grayPn	= InspGrayPn;
		int						i_mode	= R_CS_GetInspectMode();
		int						thr;
		R_RECT					area;
		// Search bin level set
			// --- Gray memory
			G2EsLSetPara.nGrayPn = grayPn;
			// --- area
			// if(!bIsLTR50){
			if( (!bIsLTR50) && (!bIsLTR18) ){
				G2EsLSetPara.nXmin = (G2_pos[0]+G2_pos[1])/2-(G2_pos[1]-G2_pos[0])/4;
				G2EsLSetPara.nXmax = (G2_pos[0]+G2_pos[1])/2+(G2_pos[1]-G2_pos[0])/4;
				G2EsLSetPara.nYmin = G2_pos[2]-20;
				G2EsLSetPara.nYmax = G2_pos[3]+20;
			} else {
				if(side==0){
					G2EsLSetPara.nXmin = Ap_pos[0]-5;
					G2EsLSetPara.nXmax = G2EsLSetPara.nXmin+50;
					if(G2EsLSetPara.nXmax>FxSize-1)	G2EsLSetPara.nXmax=FxSize-1;
					G2EsLSetPara.nYmin = (G2_pos[2]+G2_pos[3])/2-(G2_pos[3]-G2_pos[2])/4;
					G2EsLSetPara.nYmax = (G2_pos[2]+G2_pos[3])/2+(G2_pos[3]-G2_pos[2])/4;
				} else {
					G2EsLSetPara.nXmax = Ap_pos[1]+5;
					G2EsLSetPara.nXmin = G2EsLSetPara.nXmax-50;
					if(G2EsLSetPara.nXmin < 0)	G2EsLSetPara.nXmin=0;
					G2EsLSetPara.nYmin = (G2_pos[2]+G2_pos[3])/2-(G2_pos[3]-G2_pos[2])/4;
					G2EsLSetPara.nYmax = (G2_pos[2]+G2_pos[3])/2+(G2_pos[3]-G2_pos[2])/4;
				}
			}
			// ---
			//v2.20 if(!bIsLTR50){
			if( (!bIsLTR50) && (!bIsLTR18) ){
				G2EsLSetPara.nStartLevel	= 10;// ChipEsLevel.nLower;			// ヒストグラムの計算範囲の開始点(start>=0)
				G2EsLSetPara.nEndLevel		= 250;									// ヒストグラムの計算範囲の終了点
			} else {
				G2EsLSetPara.nStartLevel	= 25;// ChipEsLevel.nLower;			// ヒストグラムの計算範囲の開始点(start>=0)
				G2EsLSetPara.nEndLevel		= 200;									// ヒストグラムの計算範囲の終了点
			}
			G2EsLSetPara.nMethod		= THRESH_OTSU;							// しきい値選定の方法
			// --- level set
			thr = BinLevelSet( &G2EsLSetPara );
			// --- Check level
			if( thr < 0 ) {
				// --- comment
				if( i_mode & INSPECT_STEP ) {
					sprintf( Comment, " === ERROR === \n Setup of G2 ES bin level was not completed. " );
					DrawMessage( Comment, DRAW_RED, 2, 2 );
				}
				return ERROR;
			}
			G2EsLevel.nData = G2EsLSetPara.nLevel + G2EsLevel.nOffset;
			if( G2EsLevel.nData < G2EsLevel.nLower )	G2EsLevel.nData = G2EsLevel.nLower;
			if( G2EsLevel.nData > G2EsLevel.nUpper )	G2EsLevel.nData = G2EsLevel.nUpper;

			if( i_mode & INSPECT_STEP ) {
				R_move_gray_memory( InspGrayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				area.left	= G2EsLSetPara.nXmin;
				area.right	= G2EsLSetPara.nXmax;
				area.top	= G2EsLSetPara.nYmin;
				area.bottom	= G2EsLSetPara.nYmax;
				R_DrawBox( DRAW_YELLOW, area, GRAPH_DRAW, SOLID_LINE );
				sprintf( Comment, "Insp. level (Ocoat:%d(%d))", G2EsLevel.nData,G2EsLSetPara.nLevel );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				R_DrawCls();
			}

		// result
			return OK;

	}	// G2EsLevelSet( void ) end

/**********************************************************
	関数名：	ＵｓｂＫｅｙＣｅｒｔｉｆｉｃａｔｉｏｎ
	機  能：	ＵＳＢメモリキー認証
	形  式：	static int UsbKeyCertification( char *UsbKeyInformation )
	引き数：	char *UsbKeyInformation
					認証成功した場合に、使用されたＵＳＢキーの情報を格納します。
					例）
					 -- CERTIFICATION PASS --
					S:  SerialNumber=07740D954E1B
					P:  Vendor=13fe ProdID=1d20 Rev= 1.00

	戻り値：	OK		認証ＯＫ
				ERROR	認証ＮＧ

	解  説：	ＵＳＢメモリの認証を行います。
				認証はＵＳＢメモリのＶｅｎｄｏｒＩＤ，ＰｒｏｄｕｃｔＩＤ，ＳｅｒｉａｌＮｕｍｂｅｒを用います。
				登録されたＵＳＢメモリが接続されているかどうかを確認し、接続されていればＯＫ，されていなければＥＲＲＯＲを返します。

	留意事項：	あらかじめＵＳＢメモリキーの登録ファイルの作成が必要です。

				登録するＵＳＢメモリは
					VendorID
					ProductID
					SerialNumber
				が正しく設定されているものを利用してください。
				（中にはSerialNumber=0123456789ABCDEF などときちんと設定していないメーカもあります）
				VendorID等の情報の取得はusbviewというソフトを使用して確認出来ます。

				登録ファイルは
				＜SerialNumberの登録ファイル＞
				usbserial.dat
				S:  SerialNumber=07740D954E1B
				S:  SerialNumber=07740D954E1C
				S:  SerialNumber=07740D954E1D
				＜VendorID, ProductIDの登録ファイル＞
				usbvendor.dat
				P:  Vendor=13fe ProdID=1d20 Rev= 1.00

				９３０１ＬｉｘｕｘマシンでＵＳＢ情報の取得方法
				ターミナル上で以下のコマンドを入力
		Fedora11>cat /proc/bus/usb/devices | grep Serial
		UBUNTU	>cat /sys/kernel/debug/usb/devices | grep Serial
				⇒接続中のSerialNumberの情報が表\示
				S:  SerialNumber=0000:00:1d.2
				S:  SerialNumber=0000:00:1d.1
				S:  SerialNumber=0000:00:1d.0
				S:  SerialNumber=0000:00:1a.2
				S:  SerialNumber=0000:00:1a.1
				S:  SerialNumber=0000:00:1a.0
				S:  SerialNumber=0000:00:1d.7
				S:  SerialNumber=07740D954E1B ->この行をコピーし、usbserial.datに張り付ける
				S:  SerialNumber=0000:00:1a.7

		Fedora11>cat /proc/bus/usb/devices | grep Vendor
		UBUNTU	>cat /sys/kernel/debug/usb/devices | grep Vendor
				P:  Vendor=1d6b ProdID=0001 Rev= 2.06
				P:  Vendor=1d6b ProdID=0001 Rev= 2.06
				P:  Vendor=1d6b ProdID=0001 Rev= 2.06
				P:  Vendor=046d ProdID=c408 Rev=14.00
				P:  Vendor=1d6b ProdID=0001 Rev= 2.06
				P:  Vendor=1d6b ProdID=0001 Rev= 2.06
				P:  Vendor=1d6b ProdID=0001 Rev= 2.06
				P:  Vendor=1d6b ProdID=0002 Rev= 2.06
				P:  Vendor=13fe ProdID=1d20 Rev= 1.00 ->この行をコピーし、usbserial.datに張り付ける
				P:  Vendor=1d6b ProdID=0002 Rev= 2.06
		Fedora11>cat /proc/bus/usb/devices | grep Vendor
		UBUNTU	>cat /sys/kernel/debug/usb/devices | grep Vendor
				⇒接続中のSerialNumberの情報が表\示

**********************************************************/							// 2017.06.13
/*
#define USBKEY_MAX 10
//int UsbKeyCertification( char *UsbKeyInformation, int SChange )
static int UsbKeyCertification_rist( USB_KEY_INFO *UsbKeyInfo )

{
	int		i;
	FILE	*fpListVendor;
	FILE	*fpListSerial;
	FILE	*fpTmpVendor;
	FILE	*fpTmpSerial;
	char	ListSerial[USBKEY_MAX][128];
	char	ListVendor[USBKEY_MAX][128];
//	char	ComVendor[] = "cat /proc/bus/usb/devices | grep Vendor > usbvendor.txt";                    //For Fedora11
//	char	ComSerial[] = "cat /proc/bus/usb/devices | grep Serial > usbserial.txt";
    char	ComVendor[] = "cat /sys/kernel/debug/usb/devices | grep Vendor > usbvendor.txt";            //For Fedora19
	char	ComSerial[] = "cat /sys/kernel/debug/usb/devices | grep Serial > usbserial.txt";
	char    ComMount[]    = "mount -t vfat /dev/sdb /home/ipg/user/surface/usb";
	char    ComMount1[]   = "mount -t vfat /dev/sdb1 /home/ipg/user/surface/usb";
	char    ComMount2[]   = "mount -t vfat /dev/sdb2 /home/ipg/user/surface/usb";
	char    ComMount3[]   = "mount -t vfat /dev/sdb3 /home/ipg/user/surface/usb";
	char    ComMount4[]   = "mount -t vfat /dev/sdb4 /home/ipg/user/surface/usb";
	char    ComMount5[]   = "mount -t vfat /dev/sdb5 /home/ipg/user/surface/usb";
	char    ComMount6[]   = "mount -t vfat /dev/sdc  /home/ipg/user/surface/usb";
	char    ComMount7[]   = "mount -t vfat /dev/sdc1 /home/ipg/user/surface/usb";

	char    ComUnMount[]  = "umount usb";

	int		list_no;
	int		list_count;
	char	buffer[128];
	int		FlagSerial = 0;
	int		FlagVendor = 0;
	char	InfoSerial[128];
	char	InfoVendor[128];
//	int 	UsbKeySerialID;
//	char	UsbKeyInformation[1024];
//	int		UsbKeySerial;
//	int 	UsbKeyVendor;

	system( ComMount );
 	system( ComMount1 );
	system( ComMount2 );
	system( ComMount3 );
	system( ComMount4 );
	system( ComMount5 );
	system( ComMount6 );
	system( ComMount7 );

//    return OK;

	//USB Serial Numberの認証
	if( NULL == (fpListSerial=fopen("./usb/usbserial.dat","rt")) ){
		R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (can not open usbserial.dat)" );
		system( ComUnMount );
		return ERROR;
	}
	else{
		list_no = 0;
		while(NULL != fgets(buffer, 128, fpListSerial )){
			if( list_no > USBKEY_MAX ){
				fclose( fpListSerial );
				//登録Listが多すぎる場合はエラー
				R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (too many list of usbserial.dat)" );
				fclose( fpListSerial );
				system( ComUnMount );
				return ERROR;
			}
			if( 1>=strlen(buffer) ){
				//登録Listがゼロの場合はエラー
				if( 0 == list_no ){
					R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (no list of usbserial.dat)" );
					fclose( fpListSerial );
					return ERROR;
				}
			}

			if( 10 < strlen(buffer) ){
				buffer[strlen(buffer)-1] = '\0';
				printf("buffer[%s]\n",buffer);				//行末の改行コードの削除
				strcpy( ListSerial[list_no], buffer );
				printf("ListSerial[%d]:[%s]\n",list_no,ListSerial[list_no] );
				list_no++;
			}
		}
		fclose( fpListSerial );
		list_count = list_no;

		//Listの表\示
		printf("--- USB KEY LIST ---\n");
		for( i=0; i<list_count; i++){
			printf("LIST SERIAL[%2d]:[%s]\n",i, ListSerial[i] );
		}

		//接続されているＵＳＢ情報の取得⇒usbserial.txtへ
		system ( ComSerial );

		if( NULL == (fpTmpSerial=fopen("usbserial.txt","rt")) ){
			R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (can not open usbserial.txt)" );
			system( ComUnMount );
			return ERROR;
		}
		else{
			list_no = 0;
			while(NULL != fgets(buffer, 128, fpTmpSerial )){
				buffer[strlen(buffer)-1] = '\0';			printf("buffer[%s]\n",buffer);				//行末の改行コードの削除
				for( i=0; i<list_count; i++){
					if( 0==strcmp( buffer, ListSerial[i] ) ){ //success certification of usb key. //list_no //<z2>
						//R_draw_popup_message( DRAW_GREEN, DISP_4, POPUP_WAIT_CLICK, "-- CERTIFICATION PASS --\nUSB KEY[%s]\nLIST   [%s]",buffer,ListSerial[list_no] );
						//UsbKeySerial = atoi(ListSerial[i]); //<z2>
						//UsbKeySerialID = i; //<z2>
						fclose( fpTmpSerial );
						strcpy( InfoSerial, buffer );
						FlagSerial = 1;
						goto SERIAL_SUCCESS;
						//return OK;
					}
				}
			}
			fclose( fpTmpSerial );
			R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- CERTIFICATION FAIL!! --\n KEY IS NOT FOUND" );
			system( ComUnMount );
			return ERROR;
		}
	}
SERIAL_SUCCESS:

	//USB VendorID, ProductIDの認証
	if( NULL == (fpListVendor=fopen("./usb/usbvendor.dat","rt")) ){
		R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (can not open usbvendor.dat)" );
		system( ComUnMount );
		return ERROR;
	}
	else{
		list_no = 0;
		while(NULL != fgets(buffer, 128, fpListVendor )){
			if( list_no > USBKEY_MAX ){
				fclose( fpListVendor );
				//登録Listが多すぎる場合はエラー
				R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (too many list of usbvendor.dat)" );
				fclose( fpListVendor );
				system( ComUnMount );
				return ERROR;
			}
			if( 1>=strlen(buffer) ){
				//登録Listがゼロの場合はエラー
				if( 0 == list_no ){
					R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (no list of usbvendor.dat)" );
					fclose( fpListVendor );
					system( ComUnMount );
					return ERROR;
				}
			}

			if( 10 < strlen(buffer) ){
				buffer[strlen(buffer)-1] = '\0';
				printf("buffer[%s]\n",buffer);				//行末の改行コードの削除
				strcpy( ListVendor[list_no], buffer );
				printf("ListVendor[%d]:[%s]\n",list_no,ListVendor[list_no] );
				list_no++;
			}
		}
		fclose( fpListVendor );
		list_count = list_no;

		//Listの表\示
		printf("--- USB KEY LIST ---\n");
		for( i=0; i<list_count; i++){
			printf("LIST VENDOR [%2d]:[%s]\n",i, ListVendor[i] );
		}

		//接続されているＵＳＢ情報の取得⇒usbserial.txtへ
		system ( ComVendor );

		if( NULL == (fpTmpVendor=fopen("usbvendor.txt","rt")) ){
			R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (can not open usbvendor.txt)" );
			system( ComUnMount );
			return ERROR;
		}
		else{
			list_no = 0;
			while(NULL != fgets(buffer, 128, fpTmpVendor )){
				buffer[strlen(buffer)-1] = '\0';			printf("buffer[%s]\n",buffer);				//行末の改行コードの削除
				for( i=0; i<list_count; i++){
					if( 0==strcmp( buffer, ListVendor[i] ) ){ //success certification of usb key. //list_no //<z2>
						//R_draw_popup_message( DRAW_GREEN, DISP_4, POPUP_WAIT_CLICK, "-- CERTIFICATION PASS --\nUSB KEY[%s]\nLIST   [%s]",buffer,ListVendor[list_no] );
					//	UsbKeyVendor = ListVendor[i]; //<z2>
					//	sscanf(ListVendor[i],"%x",&UsbKeyVendor ) ; //<z2>
						fclose( fpTmpVendor );
						strcpy( InfoVendor, buffer );
						FlagVendor = 1;
						goto VENDOR_SUCCESS;
						//return OK;
					}
				}
			}
			fclose( fpTmpVendor );
			R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- CERTIFICATION FAIL!! --\n KEY IS NOT FOUND" );
			system( ComUnMount );
			return ERROR;
		}
	}
VENDOR_SUCCESS:
*/
/*	R_draw_popup_message( DRAW_GREEN, DISP_4, POPUP_WAIT_CLICK, "-- CERTIFICATION PASS --\n"
																"Vendor[%s]\n"
																"Serial[%s]\n"
														 		,InfoVendor,InfoSerial );
*/
/*	//sprintf( UsbKeyInformation," -- CERTIFICATION PASS --\n");
//	R_TimeInfoGetNow(&RTimeInfo);
//	sprintf( UsbKeyInfo->Date, "%4d-%02d-%02d_%02d%02d%02d",RTimeInfo.year,RTimeInfo.month,RTimeInfo.day,RTimeInfo.hour,RTimeInfo.minute,RTimeInfo.second );
	strcpy ( UsbKeyInfo->Vendor, InfoVendor );
	strcpy ( UsbKeyInfo->Serial, InfoSerial );
	R_draw_popup_message( DRAW_GREEN, DISP_4, POPUP_WAIT_CLICK, " -- CERTIFICATION PASS --\n"
																"%s\n"
																"%s\n"
																"%s"
																,UsbKeyInfo->Date, UsbKeyInfo->Vendor, UsbKeyInfo->Serial );

	system( ComUnMount );
	return OK;
}
*/


// 指定位置のビットをセットorリセットする関数。nBit:位置(0-)、nFlag:1=off を指定
// IN-CY-122カスタムビット対応	2016/04/09	komatsu
static int SetBit(int nBit, int nFlag)
{
	int data = 0x01;
	int flag;
	int port;
	int bit;

	if(CommunicationDeviceShareId == 999){
		port = DrCy83Ch | (CNa_OUT_port_1 + (CommunicationDeviceStartBit + nBit - 1) / 8);
		bit = (CommunicationDeviceStartBit + nBit - 1) % 8;
	}else{
		port = GpioOutPort;
		bit = nBit - 1;
	}

	data = data << bit;
	flag = nFlag&0x01;

	{
		if(flag==1)		R_gpio_off(port, data);
		else			R_gpio_on(port, data);
	}

	return OK;
}
static void OutputBusyOffNwt(void)
{
	SetBit( 1 , 1);
}

static void OutputBusyOnNwt(void)
{
	SetBit( 1, 0);
}

static void OutputLockOffNwt(void)
{
	SetBit( 2, 1);
}

static void OutputLockOnNwt(void)
{
	SetBit( 2, 0);
}

static void OutputJudgeNgNwt(void)
{
	SetBit( 3, 1);
}

static void OutputJudgeGoNwt(void)
{
	SetBit( 3, 0);
}

static void OutputBinReverseNwt(void)
{
	SetBit( 5, 1);
	SetBit( 6, 0);
	SetBit( 7, 0);
}

static void OutputBinSystemNwt(void)
{
	SetBit( 5, 1);
	SetBit( 6, 1);
	SetBit( 7, 1);
}

static void OutputBinNormalNwt(void)
{
	SetBit( 5, 0);
	SetBit( 6, 0);
	SetBit( 7, 0);
}

static void OutputNoworkOffNwt(void)
{
	SetBit( 8, 0);
}

static void OutputNoworkOnNwt(void)
{
	SetBit( 8, 1);
}

static long long int InputByte(void)
{
	if(999 == CommunicationDeviceShareId){
		long long int data = 0ll;
		int i;
		int start,end;

		start = CommunicationDeviceStartBit / 8;
		end = (CommunicationDeviceStartBit + CommunicationDeviceUseLen - 1) / 8;
		for(i = end; i >= start; i--){
			data <<= 8;
			data |= (R_gpio_read(DrCy83Ch | (CNa_IN_port_1 + i))) & 0xFF;
		}

		data >>= (CommunicationDeviceStartBit % 8);

		return data;
	}
	return R_gpio_read(GpioInPort) & 0xFFFF;
}

int		CheckInspStartNwt( void )
	{
		int						port;

		port = InputByte();
		return( port & NWT_INSP_START );

	}	// CheckInspStart( void ) end


// 指定位置のビットを得る関数
static int nMaxBitNumber = 64;
static long long int GetBit(long long int nData, int nBit, int nSize)
{
	int i;
	long long int data;
	long long int mask = 0x01ll;

	if((nBit<0)||nMaxBitNumber<nBit)			return -1;		// ビット数エラー
	if((nSize<0)||nMaxBitNumber<nSize)			return -1;		// サイズエラー
	if((nSize==0)||(nBit==0))					return -2;		// 片方が0の場合は使用しない

	data = (long long int)((nData >> (nBit-1)) & 0xffffffffffffffff);

	for(i=1; i<nSize; i++){
		mask = mask << 1;
		mask += 0x01;
	}

	return (mask & data);
}

// スタートビットを得る関数。nData内のSystemPara.tInputPara.nStartBitStartビットを得る
static long long int GetStartBit(long long int nData)
{
	return GetBit(nData, 16, 1);
}
// リトライビットを得る関数。
//static long long int GetRetryStartBit(long long int nData)
//{
//	return GetBit(nData, SystemPara.tInputPara.nRetryStartBitStart, 1);
//}

// コレット番号を得る関数。nData内のSystemPara.nColletNumberBitStartからSystemPara.nColletNumberBitSize分を得る。
static long long int GetColletNumber(long long int nData)
{
	return GetBit(nData, 1, 8);		// 制御からもらうコレット番号は０から、認識で使用する番号は１から
}
// ワークの状態の確認。NormalWork or NGWork or NoWork
static long long int GetWorkState(long long int nData)
{
	return GetBit(nData, 14, 2);
}

// カウンタクリアフラグの確認
static long long int GetCounterClear(long long int nData)
{
	return GetBit(nData, 1, 16);
}
// ロットスタートフラグの確認
//static long long int GetLotStart(long long int nData)
//{
//	return GetBit(nData, SystemPara.tInputPara.nLotStartBitStart, SystemPara.tInputPara.nLotStartBitSize);
//}

static int GetOutputStatus(void)
{
	int port,bit,mask;
	unsigned int ret;

	if(CommunicationDeviceShareId == 999){		// 2016.05.30 10bit出力に対応 by tanaka
		port = DrCy83Ch | (CNa_OUT_port_1 + (CommunicationDeviceStartBit) / 8);
		bit = CommunicationDeviceStartBit  % 8;
		mask = (1 << CommunicationDeviceUseLen)-1;
		ret = ( ((R_gpio_read_output_status(port+1) << 8) + R_gpio_read_output_status(port)) >> bit);
		return ((ret ^ mask));
	}else{
		return (~R_gpio_read_output_status(GpioOutPort)) & 0xFF;
	}
}

/**********************************************************
	関数名：	ＤｒＸ１２４ＣｏｎｄｉｔｉｏｎＳｅｔ
	機  能：	DR-X-124条件設定
	形  式：	static int DrX124ConditionSet(void)
	引き数：
	戻り値：
	解  説：
	留意事項：
**********************************************************/
static int DrX124ConditionSet(void)
{
	int nPatternNumber;
	int nLedChNumber;
//	int nBoardNumber;
	int nTotalBoard;
	int nTotalPattern;

	if(LedControllMode == LED_CONTROLL_DRX124){		// 1枚の場合
		nTotalBoard			= 1;
		nTotalPattern		= _Drx124_Setting->nPatternNumber;
	}else{
		printf("DrX124CondisitonSet() ERROR     LedControllMode is wrong!!\n");
		return ERROR;
	}

	// 点灯パターン数を設定。通信終了まで待つ。
	if(OK != R_drx124_set_turn(HandleDrx124, nTotalPattern, 1)){
		printf("R_drx124_set_turn Error!    number %d\n", 0);
		return ERROR;
	}

	// 全CHを点灯させる
	if(OK != R_drx124_set_pattern(HandleDrx124, 1, _DRX124_CH_0 | _DRX124_CH_1 | _DRX124_CH_2 | _DRX124_CH_3 | _DRX124_CH_4 | _DRX124_CH_5)){
		printf("R_drx124_set_pattern Error!    number %d\n", 0);
		return ERROR;
	}

	// 点灯時間を設定する
	if(OK != R_drx124_set_time(HandleDrx124, 1, SystemPara.nShutterSpeed)){
		printf("R_drx124_set_time Error!    number %d\n", 0);
		return ERROR;
	}

	// 電流値を設定する
	for(nLedChNumber=0 ; nLedChNumber<USER_DRX124_CH_NUMBER ; nLedChNumber++){
		if(OK != R_drx124_set_current(HandleDrx124, nLedChNumber, 1, LedCurrent[nLedChNumber])){
			printf("R_drx124_set_current Error!    number %d\n", 0);
			return ERROR;
		}
	}

	// パターン0はテンポラリなので、点灯時間と電流値は設定しないが、点灯パターンだけは設定する
	if(OK != R_drx124_set_pattern(HandleDrx124, 0, _DRX124_CH_0 | _DRX124_CH_1 | _DRX124_CH_2 | _DRX124_CH_3 | _DRX124_CH_4 | _DRX124_CH_5)){
		printf("R_drx124_set_pattern Error!\n");
		return ERROR;
	}

	return OK;
}

/**********************************************************
	関数名：	ＤｒＸ１２４ＲｅｓｅｔＴｕｒｎ
	機  能：	DR-X-124のターン数をリセットする
	形  式：	static int DrX124ResetTurn(void)
	引き数：
	戻り値：
	解  説：
	留意事項：
**********************************************************/
static int DrX124ResetTurn(void)
{
	int res_light = OK;
	int i;

	if(Option&NO_DRX124)	return OK;

	// 点灯パターンをリセットする
	if(LedControllMode == LED_CONTROLL_DRX124){
		res_light = R_drx124_set_turn(HandleDrx124, 1, 1);
		if(OK != res_light){		// DR-X-124との通信に失敗した場合は再オープンして、通信しなおす
			int res_restart;
			res_restart = R_drx124_restart(HandleDrx124);
			if(OK == res_restart){
				res_light = R_drx124_set_turn(HandleDrx124, 1, 1);		// 再オープンに成功したら通信する
			}
		}
	}

	return res_light;
}


/**********************************************************
	関数名：	ＣａｐｔｕｒｅＬｅｄＡｄｊｕｓｔＣａｐ０
	機  能：	LED調整用　画像取り込み
	形  式：	static void CaptureLedAdjustCap0(int nMode)
	引き数：	int nMode
	戻り値：
	解  説：
	留意事項：	取り込み番号0専用
**********************************************************/
static void CaptureLedAdjustCap0(int nMode)
{
	int nCameraNumber = R_CS_GetCurrentCameraNumber();
	int nCaptureNumber = 0;

	nMode = nMode;	// 引数を未使用のときにでるワーニング対策

//	if(SystemPara.nExternalLightMode){
//		if(nCameraNumber==SystemPara.nExternalLightUsingCameraNumber && nCaptureNumber==SystemPara.nExternalLightUsingCaptureNumber) ExternalLedOn();
//	}

	R_CS_Capture(nCameraNumber, nCaptureNumber, GV_NO_VRAM);

	// 画像転送待ち、表示は終了処理のCaptureLedAdjustEndCap0に移動		2010/08/24 by sassa
	//R_CS_CaptureWait(nCameraNumber);

	//R_CS_CaptureDisplayImage(nCameraNumber, nCaptureNumber);		// 取り込んだ画像をモニターに表示
	//R_CS_CaptureChangeDisplayId();									// 画像ボードのIDをモニターに出力している番号に切り替える
}

/**********************************************************
	関数名：	ＣａｐｔｕｒｅＬｅｄＡｄｊｕｓｔＥｎｄＣａｐ０
	機  能：	LED調整用　画像取り込みの終了処理
	形  式：	static void CaptureLedAdjustEndCap0(int nMode)
	引き数：	int nMode
	戻り値：
	解  説：
	留意事項：	取り込み番号0専用
**********************************************************/
static void CaptureLedAdjustEndCap0(int nMode)
{
	int nCameraNumber = R_CS_GetCurrentCameraNumber();
	int nCaptureNumber = 0;

	nMode = nMode;	// 引数を未使用のときにでるワーニング対策

//	if(nCameraNumber==SystemPara.nExternalLightUsingCameraNumber && nCaptureNumber==SystemPara.nExternalLightUsingCaptureNumber) ExternalLedOff();

	R_CS_CaptureWait(nCameraNumber);
	R_CS_CaptureChangeDisplayId();									// 画像ボードのIDをモニターに出力している番号に切り替える
	// 画像表示
	// そのまま表示
	R_move_gray_memory(  MANUAL_GRAYADDRESS, 0, 0, 0, FxSize-1, FySize-1 );
}

/**********************************************************
	関数名：	ＭｅｎｕＬｉｇｈｔＳｅｔ
	機  能：	照明調整
	形  式：	static int MenuLightSet(int nMenuNumber)
	引き数：	int nCaptureNumber
	戻り値：
	解  説：
	留意事項：
**********************************************************/
static int MenuLightSet(int nMenuNumber)
{
	int nCameraNumber = R_CS_GetCurrentCameraNumber();
	int res;
	int i;

	nMenuNumber = nMenuNumber;	// 引数を未使用のときにでるワーニング対策

	R_DrawCls();
	// 照明調整モードで場合分けする		2012/03/29 by sassa
	if((LedControllMode == LED_CONTROLL_DR_CY_80)){
		// 画像取り込みページ番号を設定
//		R_LedAdjustInitiatorPageSet( MANUAL_GRAYADDRESS );
//		res = R_LedAdjust( DrCy83Ch, _LedAdjustParameter);
//		if( YES == res )	R_datafile_save(SystemFileData,  SystemFileName);			// 機台データファイル保存
		res = TWA_LED_set(MANUAL_GRAYADDRESS,TP_Bright_SetLevel,TP_Bright_SetPercent,&LedPara);
	}else {
		MenuLightSet_DrX124_Single();
	}

//	if(ON == SystemPara.nLedCurrentEachMode){
//		CopyCurrent_System_to_Package();		// 機台データの照明電流値を、機種データにコピーする
//		R_datafile_save(PackageFileData, SystemPara.sPackageFileName);		// 機種データファイル保存
//	}

	return OK;
}


/**********************************************************
	関数名：	ＩｎｃｒｅｍｅｎｔＶａｌｕｅ
	機  能：
	形  式：	static void IncrementValue(int *value, int step, int max)
	引き数：
	戻り値：
	解  説：
	留意事項：
**********************************************************/
void IncrementValue(int *value, int step, int max)
{
	(*value)+=step;
	if(*value>max)			*value = max;
}

/**********************************************************
	関数名：	ＤｅｃｒｅｍｅｎｔＶａｌｕｅ
	機  能：
	形  式：	static void DecrementValue(int *value, int step, int min)
	引き数：
	戻り値：
	解  説：
	留意事項：
**********************************************************/
void DecrementValue(int *value, int step, int min)
{
	(*value)-=step;
	if(*value<min)			*value = min;
}

/**********************************************************
	関数名：	ＲｏｔａｔｅＶａｌｕｅ
	機  能：
	形  式：	static void RotateValue(int *value, int min, int max)
	引き数：
	戻り値：
	解  説：
	留意事項：
**********************************************************/
void RotateValue(int *value, int min, int max)
{
	(*value)++;
	if(*value>max)			*value = min;
}

/**********************************************************
	関数名：
	機  能：
	形  式：
	引き数：
	戻り値：
	解  説：
	留意事項：
**********************************************************/
#define USER_DRAW_WINDOW_NUMBER			0					// R_DrawWindow系関数の引数int win(ウィンドウ番号)に入れる値
static int MenuLightSet_DrX124_Single(void)
{
	static int nTempTimer[USER_DRX124_PATTERN];

	static struct winb_para_tm tWinbmParameter[] = {
		{	"点灯時間 パターン1[usec]"		,"LedTimer Pattern1[usec]"		,&nTempTimer[0]		,10000	,10		,10		},
		{	"点灯時間 パターン2[usec]"		,"LedTimer Pattern2[usec]"		,&nTempTimer[1]		,10000	,10		,10		},
		{	"点灯時間 パターン3[usec]"		,"LedTimer Pattern3[usec]"		,&nTempTimer[2]		,10000	,10		,10		},
		{	"点灯時間 パターン4[usec]"		,"LedTimer Pattern4[usec]"		,&nTempTimer[3]		,10000	,10		,10		},
		{	"点灯時間 パターン5[usec]"		,"LedTimer Pattern5[usec]"		,&nTempTimer[4]		,10000	,10		,10		},
		{	"点灯時間 パターン6[usec]"		,"LedTimer Pattern6[usec]"		,&nTempTimer[5]		,10000	,10		,10		},
		{	NULL	},
	};

	static struct button_t btn_set[]={
		{"‐", 96,  1},{"＋",100,  1},	//  1, 2
		{"‐", 96,  3},{"＋",100,  3},	//  3, 4
		{"‐", 96,  5},{"＋",100,  5},	//  5, 6
		{"‐", 96,  7},{"＋",100,  7},	//  7, 8
		{"‐", 96,  9},{"＋",100,  9},	//  9,10
		{"‐", 96, 11},{"＋",100, 11},	// 11,12

		{"Change"		, 96	, 15},	// 13
		{"Change"		, 96	, 17},	// 14

		{"Area1"		, 96	, 21},	// 15
		{"Area2"		,102	, 21},	// 16
		{"Area3"		,108	, 21},	// 17
		{"Area4"		,114	, 21},	// 18

		{"Parameter"	, 96	, 23},	// 19
		{"Execute"		, 96	, 25},	// 20
		{"Cancel"		, 96	, 27},	// 21

	#ifdef USE_DRX124_DIALOG
		{"Viewer"		, 84	, 21},	// 22
	#endif

		{NULL, 0, 0}
	};

	R_RECT	tCheckArea[BRIGHTNESS_CHECK_AREA_NUMBER];	// 濃度チェックエリア
	int nCurrentStep[] = {100, 1000, 10000};		// 電流調整ステップ値の配列[uA]
	int nStepNum = 0;								// 現在のステップ番号(配列nCurrentStep[]の何番目を選択しているかを表す)
	int nCurrentMax = 500000;						// 電流最大値[uA]
	int nFlagLoop = ON;								// このフラグがOFFになったらループから抜ける
	int nFlagSave = OFF;							// このフラグがONなら、設定ファイルに保存する
	R_POINT tDispPosi;								//
	int nBoardNumber = 0;							// DR-X-124は1枚だけ使用する

	int nPatternNumber = 0;
	int nCameraNumber;
	int nCaptureNumber;

	int nBackupCurrent[USER_DRX124_CH_NUMBER];
	int nBackupTimer;

	int i,j;

	for (i = 0; i < BRIGHTNESS_CHECK_AREA_NUMBER; i++) {	// 濃度チェックエリア参照	2014.07.22 by araki
		tCheckArea[i]	= rectLedCheckArea[i];
	}

	if(Option&NO_DRX124)	return OK;

	// 点灯パターン数を0に設定すると、パターン0のみを繰り返すようになる
	// 通常はパターン1以上を使うので、パターン0にデータを書き込んでテンポラリとして使用する
	if(OK != R_drx124_set_turn(HandleDrx124, 0, 1)){
		printf("R_drx124_set_turn Error!\n");
		return OK;
	}

	// パラメータの値をバックアップする
	//nBackupTimer = nLedTimerDrX124;
	for(j=0 ; j<USER_DRX124_CH_NUMBER ; j++){
		nBackupCurrent[j] = LedCurrent[j];
	}

	// 表示をクリア
	R_DrawCls();

	// 表示位置をVGA、XGAで変える
	if(FxSize<=640){
		// カメラ解像度がVGA以下の場合
		tDispPosi.x = 0;
		tDispPosi.y = 35;
	}else{
		tDispPosi.x = 0;
		tDispPosi.y = 0;
	}

	while(ON == nFlagLoop){
		nCameraNumber  = _Drx124_Setting->nCameraNo[nPatternNumber];
		nCaptureNumber = _Drx124_Setting->nCaptureNo[nPatternNumber];

		// パターン0にデータを書き込んでテンポラリとして使用する
		{
			int nTurn = 0;

			// 点灯時間を設定する
			if(OK != R_drx124_set_time(HandleDrx124, nTurn, SystemPara.nShutterSpeed)){		//nLedTimerDrX124)){
				printf("R_drx124_set_time Error!\n");
			}

			// 電流値を設定する
			for(j=0 ; j<USER_DRX124_CH_NUMBER ; j++){
				if(OK != R_drx124_set_current(HandleDrx124, j, nTurn, LedCurrent[j])){
					printf("R_drx124_set_current Error!\n");
				}
			}
		}

		// 画像取り込み
		R_CS_Capture(nCameraNumber, nCaptureNumber, GV_NO_VRAM);
		R_CS_CaptureWait(nCameraNumber);
		R_CS_CaptureChangeDisplayId();									// 画像ボードのIDをモニターに出力している番号に切り替える
		// そのまま表示
		R_move_gray_memory(  MANUAL_GRAYADDRESS, 0, 0, 0, FxSize-1, FySize-1 );

		// 表示
		{
			int posi_x = 81;
			int posi_y = 0;

			R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, posi_x, posi_y, "CH  Current");
			for(j=0 ; j<USER_DRX124_CH_NUMBER ; j++){
				R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, posi_x, posi_y+1+j*2, " %1d %6duA", j+1, LedCurrent[j] );
			}
			R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, posi_x, posi_y +15, "Pattern : %2d", 1 );
			R_DrawPrintf( DRAW_WHITE, DISP_NORMAL, posi_x, posi_y +17, "Step : %5d", nCurrentStep[nStepNum] );
			R_DrawPrintf( DRAW_RED, DISP_NORMAL, tDispPosi.x, tDispPosi.y,  "Camera %1d - Capture %1d", nCameraNumber, nCaptureNumber );
			R_DrawPrintf( DRAW_RED, DISP_NORMAL, tDispPosi.x, tDispPosi.y+1,"                    " );		// 2013/09/30 by sassa
//			R_DrawPrintf( DRAW_RED, DISP_NORMAL, tDispPosi.x, tDispPosi.y+1,"[%s]", _InspectionName[nCameraNumber] );		// 2013/09/30 by sassa
			// 濃度測定＆表示
			{
				int nColor[BRIGHTNESS_CHECK_AREA_NUMBER] = {DRAW_GREEN, DRAW_RED, DRAW_BLUE, DRAW_MAGENTA};
				int nAverage[BRIGHTNESS_CHECK_AREA_NUMBER];

				for(i=0;i<BRIGHTNESS_CHECK_AREA_NUMBER;i++){
					nAverage[i] = R_get_histgram(tCheckArea[i].left, tCheckArea[i].top, tCheckArea[i].right, tCheckArea[i].bottom, Gray_address[MANUAL_GRAYADDRESS]);
					R_DrawWindowBox(USER_DRAW_WINDOW_NUMBER, nColor[i], tCheckArea[i], DRAW_NORMAL, DOTTED_LINE);
					R_DrawPrintf(nColor[i], DISP_NORMAL, tDispPosi.x, tDispPosi.y+3+i, "Area%1d: %3d", i+1, nAverage[i]);
				}
			}
		}

		switch(R_DrawButtonSetAndGet(DRAW_WHITE,DISP_NORMAL,btn_set,NON_CLOSE_BUTTON|CONTINUOUS_PUSH|NON_WAIT_PUSH|NON_CURSOR)){
			case 0:						// キャンセル
				break;
			case  1:	DecrementValue(&LedCurrent[0] ,nCurrentStep[nStepNum] ,0);				break;
			case  2:	IncrementValue(&LedCurrent[0] ,nCurrentStep[nStepNum] ,nCurrentMax);	break;
			case  3:	DecrementValue(&LedCurrent[1] ,nCurrentStep[nStepNum] ,0);				break;
			case  4:	IncrementValue(&LedCurrent[1] ,nCurrentStep[nStepNum] ,nCurrentMax);	break;
			case  5:	DecrementValue(&LedCurrent[2] ,nCurrentStep[nStepNum] ,0);				break;
			case  6:	IncrementValue(&LedCurrent[2] ,nCurrentStep[nStepNum] ,nCurrentMax);	break;
			case  7:	DecrementValue(&LedCurrent[3] ,nCurrentStep[nStepNum] ,0);				break;
			case  8:	IncrementValue(&LedCurrent[3] ,nCurrentStep[nStepNum] ,nCurrentMax);	break;
			case  9:	DecrementValue(&LedCurrent[4] ,nCurrentStep[nStepNum] ,0);				break;
			case 10:	IncrementValue(&LedCurrent[4] ,nCurrentStep[nStepNum] ,nCurrentMax);	break;
			case 11:	DecrementValue(&LedCurrent[5] ,nCurrentStep[nStepNum] ,0);				break;
			case 12:	IncrementValue(&LedCurrent[5] ,nCurrentStep[nStepNum] ,nCurrentMax);	break;

			case 13:
				RotateValue(&nPatternNumber, 0, _Drx124_Setting->nPatternNumber-1);
				while( R_DrawGetCursor());
				break;
			case 14:
				RotateValue(&nStepNum, 0, sizeof(nCurrentStep)/sizeof(int)-1);
				while( R_DrawGetCursor());
				break;

			case 15:
				R_DrawButtonClose(DRAW_WHITE,DISP_NORMAL);
				R_DrawCls();
				while( R_DrawGetCursor());
				GetRect(&tCheckArea[0],  FxSize-1, FySize-1, 0, 0 );
				break;
			case 16:
				R_DrawButtonClose(DRAW_WHITE,DISP_NORMAL);
				R_DrawCls();
				while( R_DrawGetCursor());
				GetRect(&tCheckArea[1],  FxSize-1, FySize-1, 0, 0 );
				break;
			case 17:
				R_DrawButtonClose(DRAW_WHITE,DISP_NORMAL);
				R_DrawCls();
				while( R_DrawGetCursor());
				GetRect(&tCheckArea[2],  FxSize-1, FySize-1, 0, 0 );
				break;
			case 18:
				R_DrawButtonClose(DRAW_WHITE,DISP_NORMAL);
				R_DrawCls();
				while( R_DrawGetCursor());
				GetRect(&tCheckArea[3],  FxSize-1, FySize-1, 0, 0 );
				break;

			case 19:
				R_DrawButtonClose(DRAW_WHITE,DISP_NORMAL);
				R_DrawCls();
				while( R_DrawGetCursor());
		//		if(ON == nLedTimerAdjustMode){		// 2012/12/03 by sassa
		//			for(i=0 ; i<_Drx124_Setting->nPatternNumber ; i++)	nTempTimer[i] = nLedTimerDrX124[i];
		//			R_DrawWinbParameterWithMess( tWinbmParameter, "パラメータ設定", "Prameter Setting");
		//			for(i=0 ; i<_Drx124_Setting->nPatternNumber ; i++)	nLedTimerDrX124[i] = nTempTimer[i];
		//		}
				break;
			case 20:
				// 設定内容を保存する
				nFlagLoop = OFF;
				nFlagSave = ON;
				break;
			case 21:
			case ERROR:					// 設定異常
				nFlagLoop = OFF;
				break;
		#ifdef USE_DRX124_DIALOG
			case 22:
				{	// 2014.07.15 by araki
					int	handle = R_adjust_drx124_open(HandleDrx124);
					if (0 <= handle) {
						for (i = 0; i < 6; i++) {
							R_adjust_drx124_set_channel_title(handle, i, sLedChannelTitleDrx124[i]);
							R_adjust_drx124_set_channel_description(handle, i, sLedChannelDescriptionDrx124[i]);
							R_adjust_drx124_set_current_limit(handle, i, nLedCurrentLimitDrX124[i], 0);
						}

						for (i = 0; i < _Drx124_Setting->nPatternNumber; i++) {
							struct R_ADJUST_DRX124_PATTERN*	pattern;
							pattern = R_adjust_drx124_add_pattern(handle, "Title",
								&LedCurrent[0],
								&LedCurrent[1],
								&LedCurrent[2],
								&LedCurrent[3],
								&LedCurrent[4],
								&LedCurrent[5]
							);
							{
								int	camera_number	= _Drx124_Setting->nCameraNo[i];
								int	capture_number	= _Drx124_Setting->nCaptureNo[i];
								R_CS_CAMERA*	camera = &R_CS_CameraPara[camera_number];
								pattern->video_card		= camera->nBoardId;
								pattern->initiator_page	= camera->nInitiatorPage[capture_number];
								pattern->camera_gain	= NULL;
								pattern->shutter_speed	= &camera->nShutter[capture_number];
								pattern->turn_on_period	= &SystemPara.nShutterSpeed;	//nLedTimerDrX124[i];
								{
									const char*	fname[USER_CAPTURE_NUMBER] = {
										{	"ReferenceImages/LightAdjustment/00000AA.GMD", "ReferenceImages/LightAdjustment/00000AB.GMD", "ReferenceImages/LightAdjustment/00000AC.GMD", "ReferenceImages/LightAdjustment/00000AD.GMD"	},
										{	"ReferenceImages/LightAdjustment/00000BA.GMD", "ReferenceImages/LightAdjustment/00000BB.GMD", "ReferenceImages/LightAdjustment/00000BC.GMD", "ReferenceImages/LightAdjustment/00000BD.GMD"	},
										{	"ReferenceImages/LightAdjustment/00000CA.GMD", "ReferenceImages/LightAdjustment/00000CB.GMD"	}
									};
									pattern->file_name	= fname[camera_number][capture_number];
								}
								pattern->check_area_num = 4;
								pattern->check_area_target = nLedCheckAreaTarget[i];
							}
						}

						R_adjust_drx124_add_check_area(handle, &tCheckArea[0]);
						R_adjust_drx124_add_check_area(handle, &tCheckArea[1]);
						R_adjust_drx124_add_check_area(handle, &tCheckArea[2]);
						R_adjust_drx124_add_check_area(handle, &tCheckArea[3]);

						R_adjust_drx124_set_cycle_time(handle, &SystemPara.nCycleTimeOnLedAdjustmentGui_msec);

						R_adjust_drx124_exec(handle);
					}
					R_adjust_drx124_close(handle);
				}
				break;
		#endif
		}

		R_time_delay(30000);
	}

	R_DrawButtonClose(DRAW_WHITE,DISP_NORMAL);	// ループから抜けた時点ではボタンをクローズしていないので、ここでクローズする
	while(R_DrawGetCursor());
	R_DrawCls();

	if(nFlagSave){
		for (i = 0; i < BRIGHTNESS_CHECK_AREA_NUMBER; i++) {	// 濃度チェックエリア保存	2014.07.22 by araki
			rectLedCheckArea[i]	= tCheckArea[i];
		}
		R_datafile_save(SystemFileData,  SystemFileName);			// 機台データファイル保存
	}else{
		// パラメータの値を元に戻す
	//	nLedTimerDrX124 = nBackupTimer;
		for(j=0 ; j<USER_DRX124_CH_NUMBER ; j++){
			LedCurrent[j] = nBackupCurrent[j];
		}
	}

	// 点灯パターン数を再設定する
	if(OK != R_drx124_set_turn(HandleDrx124, _Drx124_Setting->nPatternNumber, 1)){
		printf("R_drx124_set_turn Error!\n");
	}

	// 変更した値を設定する
	for(i=0 ; i<_Drx124_Setting->nPatternNumber ; i++){
		// 点灯時間を設定する
		if(OK != R_drx124_set_time(HandleDrx124, i+1, SystemPara.nShutterSpeed)){	//nLedTimerDrX124)){
			printf("R_drx124_set_time Error!\n");
		}

		// 電流値を設定する
		for(j=0 ; j<USER_DRX124_CH_NUMBER ; j++){
			if(OK != R_drx124_set_current(HandleDrx124, j, 1, LedCurrent[j])){
				printf("R_drx124_set_current Error!\n");
			}
		}
	}

	return OK;
}

/**********************************************************
	関数名：	ＧｅｔＲｅｃｔ
	機  能：
	形  式：	static void GetRect(R_RECT *area,  int xmax, int ymax, int xmin, int ymin )
	引き数：
	戻り値：
	解  説：
	留意事項：
**********************************************************/
static void GetRect(R_RECT *area,  int xmax, int ymax, int xmin, int ymin )
{
	int xw,yw;

	xw = area->right  - area->left + 1;
	yw = area->bottom - area->top  + 1;
	R_DrawGetBoxArea(&(area->left), &(area->top), &xw, &yw, xmax, ymax, xmin, ymin);
	area->right  = area->left + xw - 1;
	area->bottom = area->top  + yw - 1;
}


/**********************************************************
	関数名：
	機  能：
	形  式：
	引き数：
	戻り値：
	解  説：
	留意事項：
**********************************************************/
static int UserTimeDelay(int nTimer)
{
	RTIME rt;
	int time;

	R_time_start(&rt);
	for(;;){
		time = R_time_read(&rt);
		if(nTimer < time)	break;
	}
	return OK;

//	return R_time_delay(nTimer);
}

/**********************************************************
	関数名：	user_getch
	機  能：	コンソールからの入力を取得する
	形  式：	static int user_getch(void)
	引き数：	なし
	戻り値：	取得した文字(ASCI)
				取得失敗した場合は0を返します
	解  説：	コンソールから1文字入力されるまで待ち、入力された文字を戻り値として返します。
				また、入力された文字は画面に表示されません。

				※ANCIの標準関数にgetchar()がありますが、これを使うと1文字入力しただけでは読み込まれません。
				　改行した(Enterを押した)時点でコンソールから入力があったと判断して読み込みます。
				　これはコンソールの設定が行単位で入力するモード(カノニカルモード)になっているためなので、
				　設定を変更してからコンソールの入力を読み込めば、1文字入力した時点で取得できます。
	留意事項：	ファンクションキー、カーソルキーなどの入力には対応していません。
**********************************************************/
static int user_getch(void)
{
	struct termios oldt, newt;
	int ch = 0;

	// コンソールの設定を取得して、一時退避させる
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;

	// コンソールの設定を変更して、Enterを押さなくても1文字入力した時点で読み込むようにする
	newt.c_lflag &= ~( ICANON | ECHO );			// 非カノニカルモード、エコー無しに設定
	newt.c_cc[VTIME] = 0;						// タイムアウト無しに設定
	newt.c_cc[VMIN] = 1;						// 最小文字数を1文字に設定
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );

	// 1文字入力されるまで待つ(入力文字はchに格納される)
	read(STDIN_FILENO, &ch, 1);

	// コンソールの設定を元に戻す
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );

//	printf("key = %x\n", ch);
	return ch;
}

/**********************************************************
	関数名：	user_kbhit
	機  能：	コンソールからの入力があるかをチェックする
	形  式：	static int user_kbhit(void)
	引き数：	なし
	戻り値：	値		意味
				0		入力無し
				0以外	入力有り
	解  説：	コンソールからの入力があるかをチェックします。
				入力がある場合には、getch()で入力を取得できます。
	留意事項：
**********************************************************/
static int user_kbhit(void)
{
	struct termios oldt, newt;
	fd_set rfds;
	struct timeval tv;
	int retval;

	// コンソールの設定を取得して、一時退避させる
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;

	// コンソールの設定を変更して、Enterを押さなくても1文字入力した時点で読み込むようにする
	newt.c_lflag &= ~( ICANON );				// 非カノニカルモードに設定
	newt.c_cc[VTIME] = 0;						// タイムアウト無しに設定
	newt.c_cc[VMIN] = 1;						// 最小文字数を1文字に設定
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );

	// select関数の引数を設定する
	FD_ZERO( &rfds );					// 構造体初期化
	FD_SET( STDIN_FILENO, &rfds );		// コンソールを構造体に設定する
	tv.tv_sec = 0;						// タイムアウトは0秒に設定(チェックしたらすぐに終了する)
	tv.tv_usec = 0;

	// コンソールからの入力があるかチェック
	retval = select( 1, &rfds, NULL, NULL, &tv );

	// コンソールの設定を元に戻す
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );

	if( retval )	return 1;
	else			return 0;
}

static int AutoTypeChangeLanUser(int command){

	switch(command){
		case COMMUNICATION_COMMAND_LOT_START:
			OutputBusyOff();
			OutputLockOff();
			strcpy(QrCode,R_CS_Lan_GetQr(R_CS_Lan_GetHandle()));
			// 切り替え実行
			TypeChange_ByQrCode(QrCode, &LotData);
			// 他のプロセスにデータ送信する		2013/05/25 by sassa
		//	SendQrCodeToOtherProcess();

		//9499- not functional


			OutputBusyOn();
			OutputLockOn();



			break;
		case COMMUNICATION_COMMAND_LOT_END:
			break;
		case ERROR2:
			break;
	}


	return OK;
}

/**********************************************************
	関数名：	ＴｙｐｅＣｈａｎｇｅ＿ＢｙＱｒＣｏｄｅ
	機  能：	QRコードを元に、
	形  式：	static int TypeChange_ByQrCode(char *sQrCode, LOT_DATA *pLotData)
	引き数：
	戻り値：
	解  説：
	留意事項：
**********************************************************/
static int TypeChange_ByQrCode(char *sQrCode, LOT_DATA *pLotData)
{
	char sPackageName[64];
	char sPackageFileName[64];
	char sMarkName[64];
	char sMarkFileName[64];
	char sTapingDirName[64];
	char sDirValue[64];
	int dn;

//	switch (SystemPara.nAutoChangeMode) {
//		case QRCODE_DEFAULT:
			// 機種名
			strcpy(pLotData->sPackageName,R_CS_Lan_GetType(R_CS_Lan_GetHandle()));
			strcpy(sPackageName,   pLotData->sPackageName);

			// ロット番号
			strcpy(pLotData->sLotNumber,  R_CS_Lan_GetLotNo(R_CS_Lan_GetHandle()));

			// Machine Setting Code
			for( dn=0; dn<20; dn++ ) {
				 SpecDataBak[dn]	= SpecData[dn];				// 前のバーコードデーターを保存
				 SpecData[dn]		= 0;
			}
			for(dn=0;dn<7;dn++)	SpecData[dn]=sQrCode[108+dn];
			SpecData[7] = '0';
			for(dn=0;dn<6;dn++)	SpecData[dn+8]=sQrCode[115+dn];
			SpecSet(1);


	{
		int posi_x = 0;
		int posi_y = 2;

		R_draw_printf(DRAW_RED, DISP_NORMAL, posi_x, posi_y++, "<<Type change complete!!>>");
		R_draw_printf(DRAW_RED, DISP_NORMAL, posi_x, posi_y++, "LotNumber:     **%s**",          pLotData->sLotNumber);
		R_draw_printf(DRAW_RED, DISP_NORMAL, posi_x, posi_y++, "PackageName:   **%s**",          pLotData->sPackageName);
		R_draw_printf(DRAW_RED, DISP_NORMAL, posi_x, posi_y++, "MarkCharacter: **%s**",          pLotData->sMarkCharacter);
		R_draw_printf(DRAW_RED, DISP_NORMAL, posi_x, posi_y++, "MarkNumber:    %2d,%2d,%2d,%2d", pLotData->nMarkNumber[0], pLotData->nMarkNumber[1], pLotData->nMarkNumber[2], pLotData->nMarkNumber[3]);
		R_draw_printf(DRAW_RED, DISP_NORMAL, posi_x, posi_y++, "TapingDirName: **%s**",          pLotData->sTapingDirName);
	}

	return OK;
}
static int UsbKeyCertification_rist( USB_KEY_INFO *UsbKeyInfo )

{
	int		i;
	FILE	*fpListVendor;
	FILE	*fpListSerial;
	FILE	*fpTmpVendor;
	FILE	*fpTmpSerial;
	char	ListSerial[USBKEY_MAX][128];
	char	ListVendor[USBKEY_MAX][128];
//	char	ComVendor[] = "cat /proc/bus/usb/devices | grep Vendor > usbvendor.txt";                    //For Fedora11
//	char	ComSerial[] = "cat /proc/bus/usb/devices | grep Serial > usbserial.txt";
    char	ComVendor[] = "cat /sys/kernel/debug/usb/devices | grep Vendor > usbvendor.txt";            //For Fedora19
	char	ComSerial[] = "cat /sys/kernel/debug/usb/devices | grep Serial > usbserial.txt";
	char    ComMount[]    = "mount -t vfat /dev/sdb /home/ipg/user/surface/usb";
	char    ComMount1[]   = "mount -t vfat /dev/sdb1 /home/ipg/user/surface/usb";
	char    ComMount2[]   = "mount -t vfat /dev/sdb2 /home/ipg/user/surface/usb";
	char    ComMount3[]   = "mount -t vfat /dev/sdb3 /home/ipg/user/surface/usb";
	char    ComMount4[]   = "mount -t vfat /dev/sdb4 /home/ipg/user/surface/usb";
	char    ComMount5[]   = "mount -t vfat /dev/sdb5 /home/ipg/user/surface/usb";
	char    ComMount6[]   = "mount -t vfat /dev/sdc  /home/ipg/user/surface/usb";
	char    ComMount7[]   = "mount -t vfat /dev/sdc1 /home/ipg/user/surface/usb";

	char    ComUnMount[]  = "umount usb";

	int		list_no;
	int		list_count;
	char	buffer[128];
	int		FlagSerial = 0;
	int		FlagVendor = 0;
	char	InfoSerial[128];
	char	InfoVendor[128];
//	int 	UsbKeySerialID;
//	char	UsbKeyInformation[1024];
//	int		UsbKeySerial;
//	int 	UsbKeyVendor;

	system( ComMount );
	system( ComMount1 );
	system( ComMount2 );
	system( ComMount3 );
	system( ComMount4 );
	system( ComMount5 );
	system( ComMount6 );
	system( ComMount7 );

//    return OK;

	//USB Serial Numberの認証
	if( NULL == (fpListSerial=fopen("./usb/usbserial.dat","rt")) ){
		R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (can not open usbserial.dat)" );
		system( ComUnMount );
		return ERROR;
	}
	else{
		list_no = 0;
		while(NULL != fgets(buffer, 128, fpListSerial )){
			if( list_no > USBKEY_MAX ){
				fclose( fpListSerial );
				//登録Listが多すぎる場合はエラー
				R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (too many list of usbserial.dat)" );
				fclose( fpListSerial );
				system( ComUnMount );
				return ERROR;
			}
			if( 1>=strlen(buffer) ){
				//登録Listがゼロの場合はエラー
				if( 0 == list_no ){
					R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (no list of usbserial.dat)" );
					fclose( fpListSerial );
					return ERROR;
				}
			}

			if( 10 < strlen(buffer) ){
				buffer[strlen(buffer)-1] = '\0';
				printf("buffer[%s]\n",buffer);				//行末の改行コードの削除
				strcpy( ListSerial[list_no], buffer );
				printf("ListSerial[%d]:[%s]\n",list_no,ListSerial[list_no] );
				list_no++;
			}
		}
		fclose( fpListSerial );
		list_count = list_no;

		//Listの表\示
		printf("--- USB KEY LIST ---\n");
		for( i=0; i<list_count; i++){
			printf("LIST SERIAL[%2d]:[%s]\n",i, ListSerial[i] );
		}

		//接続されているＵＳＢ情報の取得⇒usbserial.txtへ
		system ( ComSerial );

		if( NULL == (fpTmpSerial=fopen("usbserial.txt","rt")) ){
			R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (can not open usbserial.txt)" );
			system( ComUnMount );
			return ERROR;
		}
		else{
			list_no = 0;
			while(NULL != fgets(buffer, 128, fpTmpSerial )){
				buffer[strlen(buffer)-1] = '\0';			printf("buffer[%s]\n",buffer);				//行末の改行コードの削除
				for( i=0; i<list_count; i++){
					if( 0==strcmp( buffer, ListSerial[i] ) ){ //success certification of usb key. //list_no //<z2>
						//R_draw_popup_message( DRAW_GREEN, DISP_4, POPUP_WAIT_CLICK, "-- CERTIFICATION PASS --\nUSB KEY[%s]\nLIST   [%s]",buffer,ListSerial[list_no] );
						//UsbKeySerial = atoi(ListSerial[i]); //<z2>
						//UsbKeySerialID = i; //<z2>
						fclose( fpTmpSerial );
						strcpy( InfoSerial, buffer );
						FlagSerial = 1;
						goto SERIAL_SUCCESS;
						//return OK;
					}
				}
			}
			fclose( fpTmpSerial );
			R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- CERTIFICATION FAIL!! --\n KEY IS NOT FOUND" );
			system( ComUnMount );
			return ERROR;
		}
	}
SERIAL_SUCCESS:

	//USB VendorID, ProductIDの認証
	if( NULL == (fpListVendor=fopen("./usb/usbvendor.dat","rt")) ){
		R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (can not open usbvendor.dat)" );
		system( ComUnMount );
		return ERROR;
	}
	else{
		list_no = 0;
		while(NULL != fgets(buffer, 128, fpListVendor )){
			if( list_no > USBKEY_MAX ){
				fclose( fpListVendor );
				//登録Listが多すぎる場合はエラー
				R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (too many list of usbvendor.dat)" );
				fclose( fpListVendor );
				system( ComUnMount );
				return ERROR;
			}
			if( 1>=strlen(buffer) ){
				//登録Listがゼロの場合はエラー
				if( 0 == list_no ){
					R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (no list of usbvendor.dat)" );
					fclose( fpListVendor );
					system( ComUnMount );
					return ERROR;
				}
			}

			if( 10 < strlen(buffer) ){
				buffer[strlen(buffer)-1] = '\0';
				printf("buffer[%s]\n",buffer);				//行末の改行コードの削除
				strcpy( ListVendor[list_no], buffer );
				printf("ListVendor[%d]:[%s]\n",list_no,ListVendor[list_no] );
				list_no++;
			}
		}
		fclose( fpListVendor );
		list_count = list_no;

		//Listの表\示
		printf("--- USB KEY LIST ---\n");
		for( i=0; i<list_count; i++){
			printf("LIST VENDOR [%2d]:[%s]\n",i, ListVendor[i] );
		}

		//接続されているＵＳＢ情報の取得⇒usbserial.txtへ
		system ( ComVendor );

		if( NULL == (fpTmpVendor=fopen("usbvendor.txt","rt")) ){
			R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (can not open usbvendor.txt)" );
			system( ComUnMount );
			return ERROR;
		}
		else{
			list_no = 0;
			while(NULL != fgets(buffer, 128, fpTmpVendor )){
				buffer[strlen(buffer)-1] = '\0';			printf("buffer[%s]\n",buffer);				//行末の改行コードの削除
				for( i=0; i<list_count; i++){
					if( 0==strcmp( buffer, ListVendor[i] ) ){ //success certification of usb key. //list_no //<z2>
						//R_draw_popup_message( DRAW_GREEN, DISP_4, POPUP_WAIT_CLICK, "-- CERTIFICATION PASS --\nUSB KEY[%s]\nLIST   [%s]",buffer,ListVendor[list_no] );
					//	UsbKeyVendor = ListVendor[i]; //<z2>
					//	sscanf(ListVendor[i],"%x",&UsbKeyVendor ) ; //<z2>
						fclose( fpTmpVendor );
						strcpy( InfoVendor, buffer );
						FlagVendor = 1;
						goto VENDOR_SUCCESS;
						//return OK;
					}
				}
			}
			fclose( fpTmpVendor );
			R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- CERTIFICATION FAIL!! --\n KEY IS NOT FOUND" );
			system( ComUnMount );
			return ERROR;
		}
	}
VENDOR_SUCCESS:

/*	R_draw_popup_message( DRAW_GREEN, DISP_4, POPUP_WAIT_CLICK, "-- CERTIFICATION PASS --\n"
																"Vendor[%s]\n"
																"Serial[%s]\n"
														 		,InfoVendor,InfoSerial );
*/
	//sprintf( UsbKeyInformation," -- CERTIFICATION PASS --\n");
//	R_TimeInfoGetNow(&RTimeInfo);
//	sprintf( UsbKeyInfo->Date, "%4d-%02d-%02d_%02d%02d%02d",RTimeInfo.year,RTimeInfo.month,RTimeInfo.day,RTimeInfo.hour,RTimeInfo.minute,RTimeInfo.second );
	strcpy ( UsbKeyInfo->Vendor, InfoVendor );
	strcpy ( UsbKeyInfo->Serial, InfoSerial );
	R_draw_popup_message( DRAW_GREEN, DISP_4, POPUP_WAIT_CLICK, " -- CERTIFICATION PASS --\n"
																"%s\n"
																"%s\n"
																"%s"
																,UsbKeyInfo->Date, UsbKeyInfo->Vendor, UsbKeyInfo->Serial );

	system( ComUnMount );
	return OK;
}
