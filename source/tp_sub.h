/******************************************************************************

	ＴＰ専用の関数

	system name		ＴＰ専用関数
	program name	"TP_SUB.C"

	履歴
	history
	<version>		<date>			<name>			<comment>
	Ver. X.01		2008-02-14		M.Takahashi		new


******************************************************************************/


// --- define ---

typedef struct{
	int		nRateX;						// Ｘ方向分解能(1000倍値)
	int		nRateY;						// Ｙ方向分解能(1000倍値)
	int		nShutterSpeed;				// シャッタスピード(us)
	int		nCaptureTimer;				// 取り込み前タイマ(us)
	int		nCameraName;				// カメラ名	2017.07.11  tanaka BASLER acA2000-340km-TRIMVGA suport
} MACHINE_PARAMETER;



typedef struct{
	char	Date[128];
	char	Vendor[128];
	char	Serial[128];
}USB_KEY_INFO;

USB_KEY_INFO UsbKeyInfo;			//2017.06.13

//オプションコード
#define NO_DRCY80		0x0001
#define NO_INCY83		0x0002
#define NO_DRCY84		0x0004
#define NO_INCY76		0x0008			//2010.01.07 by saito
#define NO_INCY119		0x0080			//<z1> 2021.06.10<shio>
#define NO_USBKEY		0x0010			//2017.04.14 by tanaka
#define IO_INCY122		0x0020			//2017.07.11 by tanaka for NWT
#define NO_DRX124		0x0040			//2017/07/11 by tanaka
#define DEBUG_MODE		0x1000
#define BACKINSP_MODE	0x2000			//裏面検査モード
#define MECHA_CHECK		0x4000			//業者調整用モード（検査後半を処理しない）
#define ADMIN_MODE		0x8000
#define REPI            0x0100          // REPI barcode

// I/O
#define START			0x01			// input
#define LOT_END			0x02			// "
#define ALARM_RESET		0x04			// "
#define JUDGE			0x01			// "  <GO:low, NG:high>
#define BUSY			0x02			// output
#define LOCK			0x04			// "
#define ALARM			0x08			// "  <active high>
#define SAVEBUSY		0x10			// "

#define BUSY_TR			0x01			// output
#define JUDGE_TR		0x04			// "  <GO:low, NG:high>

#define SPEC_SET		0x10			// "
// I/O
#define INSP_START		0x01				// Insp. start
#define POK_SIG		    0x02				// Cancle Poket  //2012.04.19
#define	RESISTANCE_NG	0x01				// 抵抗値％外 count sig.
#define	NINSHIKI_NG		0x02				// 外観検査NG count sig.
#define	PILOT_MODE		0x04				// Pilot mode sig.
#define	JUDGE_READ		0x08				// Judge data read sig.
#define	SPEC_SET		0x10				// Spec set sig.

#define NumMarkChar     12

//NGコード
enum	NG_CODE {
//												//	コード1は予約済みなので使用禁止(TWL.Hで宣言済み)
					// Insp.
//						NG_NO_WORK		= 1,	//  1: ワークなし
						NG_CANT_INSP	= 2,	//  2: Can't Insp.
						NG_INDEX_POS,			//  3: Index pos. Error
						NG_TAPE_POS,			//  4: Tape pos. Error
						NG_REVERSE,				//  5: Chip Reverse
						NG_FOCUS,				//  6: Focus Error
					// G2 Insp.
						NG_G2_X_SIZE	= 11,	// 11: G2 X size
						NG_G2_Y_SIZE,			// 12: G2 Y size
						NG_G2_KAKE_X,			// 13: G2 Kake X
						NG_G2_KAKE_Y,			// 14: G2 Kake Y
						NG_G2_POS_X,			// 15: G2 position X Error
						NG_G2_POS_Y,			// 16: G2 position Y Error
						NG_OCOAT_X,				// 17: Overcoat kake X
						NG_OCOAT_Y,				// 18: Overcoat kake Y
						NG_OCOAT_XY,			// 19: Overcoat kake Y
					// Chip(Appearence) Insp.
						NG_CHIP_X_SIZE	= 21,	// 21: CHIP X size
						NG_CHIP_Y_SIZE,			// 22: CHIP Y size
						NG_CHIP_KAKE_X,			// 23: CHIP Kake X
						NG_CHIP_KAKE_Y,			// 24: CHIP Kake Y
						NG_CHIP_CORNER,			// 25: CHIP Kake corner
					// Plating adhesion
						NG_PL_ADHESION	= 31,	// 31: Plating adhesion Error
					// Marking Insp.
						NG_MARKING		= 41,	// 41: Marking Error
						NG_MARK_SEGMENT,		// 42: Mark segment Error
					// Electrode Insp.
						NG_ELEC_X_SIZE	= 51,	// 51: Elec. X size
						NG_ELEC_Y_SIZE,			// 52: Elec. Y size
						NG_ELEC_EDGE_X,			// 53: Elec. edge Kake
						NG_ELEC_EDGE_Y,			// 54: Elec. edge Kake
						NG_ELEC_KAKE,			// 55: Elec. Kake
						NG_ELEC_BLACK,			// 56: Elec. black Error
						NG_SIDE_ELEC,			// 57: Side Elec. X size
						NG_GLASS_IBUTSU,		// 58: Glass IBUTSU
					// M/C NG box
						MC_RESISTANCE	= 81,	// 81: M/C NG box Resistance
                        MC_NINSHIKI 			// 82: M/C NG box Ninshiki
	};
//コード98,99は予約済みなので使用禁止(TWL.Hで宣言済み)
//#define NG_LIGHTING			97		//照明エラー
//#define NG_CAPTURE			98		//画像取り込みエラー
//#define NG_INSP				99		//認識エラー

#define COMMAND_BUFF	256				//コマンド送受信バッファサイズ


#define GPIO_DSW				_BOARD_RDSW_0							// 照明基板DSW番号
#define OutputBusyOn()			R_gpio_on( GPIO_DSW|CNa_OUT_port_1, BUSY)
#define OutputBusyOff()			R_gpio_off(GPIO_DSW|CNa_OUT_port_1, BUSY)
#define OutputLockOn()			R_gpio_on( GPIO_DSW|CNa_OUT_port_1, LOCK)
#define OutputLockOff()			R_gpio_off(GPIO_DSW|CNa_OUT_port_1, LOCK)
#define OutputJudgeGo()			R_gpio_on( GPIO_DSW|CNa_OUT_port_1, JUDGE)
#define OutputJudgeNg()			R_gpio_off(GPIO_DSW|CNa_OUT_port_1, JUDGE)
#define OutputAlarmOn()			R_gpio_on( GPIO_DSW|CNa_OUT_port_1, ALARM)
#define OutputAlarmOff()		R_gpio_off(GPIO_DSW|CNa_OUT_port_1, ALARM)
#define OutputSaveBusyOn()		R_gpio_on( GPIO_DSW|CNa_OUT_port_1, SAVEBUSY)
#define OutputSaveBusyOff()		R_gpio_off(GPIO_DSW|CNa_OUT_port_1, SAVEBUSY)

#define OutputJudgeGoTR()		R_gpio_on( GPIO_DSW|CNa_OUT_port_2, JUDGE_TR)
#define OutputJudgeNgTR()		R_gpio_off(GPIO_DSW|CNa_OUT_port_2, JUDGE_TR)
#define OutputBusyOnTR()		R_gpio_on( GPIO_DSW|CNa_OUT_port_2, BUSY_TR)
#define OutputBusyOffTR()		R_gpio_off(GPIO_DSW|CNa_OUT_port_2, BUSY_TR)

#define OutputPilotOn()			R_gpio_on( GPIO_DSW|CNa_OUT_port_2, 0x01)
#define OutputPilotOff()		R_gpio_off(GPIO_DSW|CNa_OUT_port_2, 0x01)
#define OutputNGAlarmOn()		R_gpio_on( GPIO_DSW|CNa_OUT_port_2, 0x02)
#define OutputNGAlarmOff()		R_gpio_off(GPIO_DSW|CNa_OUT_port_2, 0x02)
#define OutputGLModeOn()		R_gpio_on( GPIO_DSW|CNa_OUT_port_2, 0x04)
#define OutputGLModeOff()		R_gpio_off(GPIO_DSW|CNa_OUT_port_2, 0x04)
#define OutputReverseOn()		R_gpio_on( GPIO_DSW|CNa_OUT_port_2, 0x10)
#define OutputReverseOff()		R_gpio_off(GPIO_DSW|CNa_OUT_port_2, 0x10)
#define OutputNoworkOn76()		R_gpio_on( GPIO_DSW|CNa_OUT_port_2, 0x20)		//<shio>
#define OutputNoworkOff76()		R_gpio_off(GPIO_DSW|CNa_OUT_port_2, 0x20)		//<shio>

#define OutputNoworkOn119()		R_gpio_on( Incy119Ch|CNa_OUT_port_1, 0x04) //<z1>
#define OutputNoworkOff119()	R_gpio_off(Incy119Ch|CNa_OUT_port_1, 0x04) //<z1>


// --- Product name
extern char			ProductName[32];				// 製品名
// --- Inspection name
extern char			InspectionName[32];				// 検査名
// --- Program version ---
extern char			ProgramVer[32];					// Program Version
//機台No.
extern char			MachineName[32];				// 機台名




// --- Bin level set ---
typedef struct{
	int		nGrayPn;						// 処理Gray memory No.
	int		nXmin;							// 計測Area X min
	int		nYmin;							// 計測Area Y min
	int		nXmax;							// 計測Area X max
	int		nYmax;							// 計測Area Y max
	int		nStartLevel;					// ヒストグラムの計算範囲の開始点(start>=0)
	int		nEndLevel;						// ヒストグラムの計算範囲の終了点
	int		nMethod;						// しきい値選定の方法
	int		nLevel;							// ２値化レベル
}	THRESHOLD_PARAMETER;

// --- Gray to Bin level ---
typedef struct{
	int		nData;							// Bin level data
	int		nOffset;						// Bin level offset
	int		nLower;							// Bin level lower limit
	int		nUpper;							// Bin level upper limit
}	BIN_LEVEL_PARAMETER;

// --- Edge search set ---
typedef struct{
	int		nGrayPn;						// 処理Gray memory No.
	int		nXmin;							// 計測Area X min
	int		nYmin;							// 計測Area Y min
	int		nXmax;							// 計測Area X max
	int		nYmax;							// 計測Area Y max
	int		nLvelLo;						// 明暗の差の最低値
	int		nAv;							// 平均化回数
	int		nNoise;							// ノイズ幅
	int		nSlice;							// スライスレベル
	int		nDeg;							// 検出方向
	int		nWidth;							// 検出幅
	int		nContrast;						// エッジの種類（暗い→明るい：ES_BLACK、明るい→暗い:ES_WHITE）
	int		nPos[700][3];					// 検出エッジデータ( X,Y,temp )
	int		nFreq;							// 最頻値
}	EDGE_SEARCH_PARAMETER;

// --- Bin Filter condition ---
typedef struct{
	int		nBinPn;							// 処理bin memory No.
	int		nXmin;							// 処理範囲 X min
	int		nYmin;							// 処理範囲 Y min
	int		nXmax;							// 処理範囲 X max
	int		nYmax;							// 処理範囲 Y max
}	BIN_FILTER_PARAMETER;

// --- ２値計測条件（ 特徴量演算　演算方式指定 ） ---
typedef struct{
	int		nMode;							// 演算モード( 0:RM_NORMAL 通常演算  1:RM_RUN 特定ランサイズ無視演算  2:RM_AREA 特定面積以下無視演算 )
	int		nRun;							// 編集対象ランサイズ
	int		nArea;							// 無視対象面積値
	int		nColor;							// 無視対象色
}	BIN_MEASURE_PARAMETER;

// --- 標印検査 ---
typedef struct{
	int		marking_xsize;					// 標印横サイズ
	int		marking_ysize;					// 標印縦サイズ
} INSPECT_MARK_PARAMETER;

// --- グレーサーチ用パラメータ構造体 ---
typedef struct {
	int		pat_no;							// パターン名称
	int		middle_lower;
	int		last_lower;
	int		complex;
	int		resolution;
	R_BOX	search_window;					// サーチエリア
} GS_PARAMETER;


enum	GrayPlaneNo {
						GrayP0		= 0,		// GRAY_PLANE0	グレー０ページ（表示メモリー）
						GrayP1,
						GrayP2,
						GrayP3,
						GrayP4,
						GrayP5,
						GrayP6,
						GrayP7,
						GrayP8,
                        GrayP9
};

enum	BinPlaneNo {
						BinP0		= 0,		// BIN_PLANE0	バイナリ０ページ（表示メモリー）
						BinP1,
						BinP2,
						BinP3,
						BinP4,
						BinP5,
						BinP6,
						BinP7,
						BinP8,
                        BinP9
};

#define TWA_TAPE		0
#define TWA_INDEX		1

#define ImageCamera		0
#define ImageMemory		1
#define ImageFail		2
#define	ImageFile		3

// Class
#define CLASS_D			 0				// D級
#define CLASS_F			 1				// F級
#define CLASS_J			 2				// J級

#define	TWA_Scallop		0
#define	TWA_Rectangle	1

#define	PosXmin			0
#define	PosYmin			1
#define	PosXmax			2
#define	PosYmax			3

#define	DataXum			0
#define	DataYum			1
#define	DataXbit		2
#define	DataYbit		3

#define	DataX			0
#define	DataY			1

#define ColorBlack		0
#define ColorWhite		1

#define	DataLeft		0
#define	DataRight		1
#define	DataLR			2
#define DataUpper		0
#define DataLower       1
#define DataUL			2

#define	PosLup			0
#define	PosLlo			1
#define	PosRup			2
#define	PosRlo			3

#define	PilotFlag		Pilot_mode

#define	SpecGlass		0
#define	SpecNormal		0
#define	SpecResin		1
#define	SpecJPW			2
#define	SpecSpG			3
#define	SpecGlassSp		3
#define	SpecMCL			3
#define	SpecSpJ			4
#define	SpecResinSp		4
//#define	SpecJPWSp		5

#define	TWA_G2			1

#define	Insp_Normal		0
#define	Insp_Hyouri		1
#define	Insp_Pinhole	2
#define	Insp_Pilot		3

#define	Insp_Low		0
#define	Insp_High		1
#define	Insp_High2		2

// 回帰係数・平均値・標準偏差
typedef struct	{
    int		dataX[700];														// Chip edge search data X
    int		dataY[700];														// Chip edge search data Y
    int		dataR[700];														// Chip edge data result
//	double	Mean;															// 平均値
//	double	SD;																// 標準偏差
    double	MeanX;															// Ｘデータ平均値
    double	MeanY;															// Ｙデータ平均値
    double	Rev;															// 回帰係数
    int		Result;															// 計算結果
} Regression_def;//		ChipEdge[4], ChipXside, ChipYside, ElecEdge[3][8];


typedef struct	{
    int		BlobNo;												// blob No.( bin measure )
    int		CornerPosX[4];										// X pos. corner(pos.)
    int		CornerPosY[4];										// Y pos. corner(pos.)
    int		Pos[4];												// position
}	Overcoat_data;

typedef struct	{
	float	kaiki_ex;														//Ｘデータ平均値
	float	kaiki_ey;														//Ｙデータ平均値
	float	kaiki_b;														//回帰係数
} KAIKI;

typedef struct {
	float	mean;															//平均値
	float	sd;																//標準偏差
	float	mean2;															//平均値（2sd以下）
	float	sd2;															//標準偏差（2sd以下）
} MEANSD;

extern int				Bin_level;
extern int				Channel;
extern int				Vmode;
extern int				TWA_X_size;
extern int				TWA_Y_size;
extern int				TWA_trigger_mode;
extern double			CCD_x;
extern int				Pilot_mode;											//パイロットモード
extern int				Incy76Ch;											// IN-CY-76 DSW data
extern int				Incy119Ch;										// IN-CY-119 DSW data //<z1>
extern int				Drcy80Ch;											// DR-CY-80 DSW data
extern int              Option;

//仕様 ( 0:通常 1:表裏 2:ピンホール検査 3:パイロット )
extern int				TWA_Insp_mode;                                      //仕様 ( 0:通常 1:表裏 2:ピンホール検査 3:パイロット )
extern int				TWA_Spec;                                           //製品仕様( 0:Normal 1:NP3 2:Jpw)
extern int				TWA_Insp_level;                                     //検査モード（ 0:通常 1:詳細 ）
extern int				TWA_Mark_Insp;                                      //標印検査

extern int              bIsMCR01;                                           // MCR01検査
extern int              bIsMCR03;                                           // MCR03検査
extern int              bIsMCR10;                                           // MCR10検査
extern int              bIsMCR18;                                           // MCR18検査
extern int              bIsESR25;                                           // ESR25検査
extern int              bIsSFR25;                                           // SFR25検査
extern int				bIsLTR50;											// LTR50検査
extern int				bIsLTR18;											// LTR18検査									//v2.20
extern int				bIsG2Straight;										// G2樹脂品（外形と接触するもの）
extern int				bIsG2Separate;										// G2ガラス品と樹脂で個別品

extern char				ProductName[32];									// 製品名
extern char				MachineName[32];									// 機台No.

extern          char	Comment[2048];										// Comment Buffer
extern          char	Comment1[200];										// Comment Buffer
extern          char	Comment2[200];										// Comment Buffer


// 仕様設定データ
//char					ProdSpecData[20];									// 仕様設定Data
//static	char			ProdSpecDataBak[20];								// 仕様設定Data backup
//static	int				ProdSpecSet			= ERROR;						// 仕様設定


void	TWA_Zoom_set( int level, int zoom_std );
void	TWA_Focus_set( int bin_l, int Judge_F );
void	TWA_rs_cle( void );
void	TWA_kaiten( int *x, int *y, int deg );

void	TWA_es_initial( int page, int level, int av, int noise );
int		TWA_es_search( int deg, int *xs, int *ys, int l, int contrast );
void	TWA_manu_video_in( int level );
int		TWA_ID_check( void );
int		DrawMessage( char mes[], int color, int mode, int pos );
void	DrawMessageOff( void );
void	BinClsPn( int binPn, int xs, int ys, int xe, int ye );
void	GrayClsPn( int grayPn, int xs, int ys, int xe, int ye );

void	FilterBinContrct( BIN_FILTER_PARAMETER *pFilter, int xsize, int ysize );
void	FilterBinExpand( BIN_FILTER_PARAMETER *pFilter, int xsize, int ysize );
int		SetsubiDataLoadfile( void );
void	SetsubiDataCheck( void );
void	SetCounter( void );
void	SetCounterTape( void );

int		CheckInspStart( void );
int		CheckInspStartNwt( void );
int		CheckResistanceNG( void );
int		CheckNinshikiNG( void );
int		CheckPilotMode( void );
int		CheckJudgeRead( void );
int		CheckSpecSet( void );
//int		CheckPilotSet( void );

void    Print1( void );
int     CanclePok_sig( void );       //2012.04.19
void	WaitMouseClick( void );
void	SetsubiDataPrintBusy( int mode );
int		SetsubiDataPrint( void );
void	SetsubiDataReadBusy( int mode );
void	SetsubiDataRead( int dn );
int		SetsubiDataWrite( void );
int		NinshikiDataWrite( void );
void TWA_mark_set_m03( void );
void TWA_mark_set_m10( void );
void TWA_mark_set_e25( void );


typedef struct{
    int 	nTime[20];
    RTIME	tTime;
    char	cComment[20][30];
}TWA_TIME_RESULT;

extern TWA_TIME_RESULT twaTime[2];

#ifdef __DOS__
    extern struct	dosdate_t		Lot_StartDate;										// ロット開始 Date
    extern struct	dostime_t		Lot_StartTime;										// ロット開始 Time
#elif __GNUC__
    extern time_t					ctLotStart;
    extern struct	tm				Lot_StartDateTime;									// ロット開始 Date,Time
#endif



void twa_TimeStart( TWA_TIME_RESULT *pPara );
void twa_TimeRead( TWA_TIME_RESULT *pPara, int nNumber );
int twa_TimeDisp( TWA_TIME_RESULT *pPara );
int UsbKeyCertification_rist( USB_KEY_INFO *UsbKeyInfo );     //2017.06.13
int UsbKeyCertification( char *UsbKeyInformation, int SChange );
void	TWA_expand2( int mem_no, int x_size, int y_size, int pt[] );
void	TWA_contrct2( int mem_no, int x_size, int y_size, int pt[] );
//int		TWA_es_bin_search_n( int p, int *rx, int *ry, int deg, int l, int flag, int nm );
void	TWA_open_switch( char *str[], int cn );
void	TWA_close_switch( int cn );

void	TWA_gray2bin( int level, int xs, int ys, int xl, int yl, int gn, int bn );
void	TWA_bin_cls( int p, int x1, int y1, int x2, int y2 );

