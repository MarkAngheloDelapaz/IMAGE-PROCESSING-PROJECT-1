/*********************************************************************************
	system name		チップ外観認識  ＴＷＡ専用の関数
	program name	"TWA_SUB.C"
	date			2009-01-19
	programer		高橋　昌也
    corp.			ローム福岡（株）
					ＭＣＲ製造部 生産技術課 生産技術２係

	履歴
	history
	<version>		<date>			<name>			<comment>
					2008-02-14		M.Takahashi		new
					2008-07-04		M.Takahashi		SetsubiDataPrint(void) 不具合修正
													SetsubiDataWrite(void) 不具合修正
					2008-10-18		M.Takahashi		誤字修正

					2009-01-19		A.Jeerasak		addition couter "PASS" of surface ninshiki for easy to keep data.
					2010-05-18		M.Tanaka		Linux対応
					2012.03.14      A.Jeerasak      Print Out Semi Lot NG count
					2012.04.19      A.Jeerasak      Cancle Poket Signal
					2017.06.12  	P.Pichet		Pilot print good chip
*********************************************************************************/


#include <stdio.h>			// Standard I/O functions
#include <stdlib.h>			// Standard Library functions
#include <malloc.h>			// Memory allocation functions
#include <string.h>
#ifdef __DOS__
	#include <conio.h>			// Console and Port I/O functions
	#include <dos.h>
	#include <io.h>
	#include <graph.h>			// Graphics functions
#elif __GNUC__
	#include "R_conio.h"
#endif

#include <time.h>
#include <fcntl.h>

#include "R_stdio.h"		// Ｉ／Ｏ　関係の宣言および関数
#include "R_stdlib.h"		// メモリー関係のライブラリー
#include "R_graph.h"
#include "R_dgraph.h"
#include "R_meas.h"			// ２値画像処理ライブラリ用ヘッダ
#include "R_math.h"
#include "R_filter.h"
#include "R_cursor.h"		// マウスライブラリー（ｃｕｒｓｏｒ．ｃ）用ヘッダファイル
							// 画面操作ライブラリ（Ｒ＿ＭＥＮＵ．Ｃ）用ヘッダファイル
#include <math.h>			// Math functions

#include "R_bgray.h"		// 画像計測ライブラリーのヘッダーファイル
#include "R_time.h"
#include "R_cursor.h"
#include "R_video.h"
#include "R_menu.h"			// 画面操作ライブラリ（Ｒ＿ＭＥＮＵ．Ｃ）用ヘッダファイル
#include "R_file.h"
#include "R_binrec.h"		// ２値認識用ヘッダファイル
#include "R_incy54.h"		// ＩＮーＣＹー５４／６２（汎用ディジタル入出力インターフェース）ライブラリー

#include "R_edge.h"
#include "R_bitmap.h"		// BITMAP FILE関係の設定
#include "R_dmenu.h"

// common library
#include "R_cs.h"
#include "R_cdraw.h"
#include "R_led.h"

#include "tp_sub.h"

#define TWA_NG			0
#define TWA_GO			1

#define KURO			0
#define SHIRO			1
#define GRAY			2

#define trig_h			0
#define trig_l			EX_IO_NO4	// shutter camera trigger signal

#define TWA_IN_CY_62	0
#define TWA_IN_CY_67	1

#define TWA_TAPE		0
#define TWA_INDEX		1

#define ImageCamera		0
#define ImageMemory		1
#define ImageFail		2
#define	ImageFile		3

#define TWA_ES_p1		0x0100
#define TWA_ES_p2		0x0200


#define	TWA_Scallop		0
#define	TWA_Rectangle	1

//static	unsigned char	TWA_out_GO   = 0x10;			//Ｉ／０出力データ 1:Low(GO)
//static	unsigned char	TWA_out_BUSY = 0x20;			//Ｉ／０出力データ 1:Low(BUSY)
//static	unsigned char	TWA_out_LOCK = 0x40;			//Ｉ／０出力データ 1:Low(LOCK)
//static	unsigned char	TWA_out_data;					//Ｉ／Ｏ出力データ

#ifdef	JAPANESE
struct message_t position1 = { 30, 1, "計測始点を指定して下さい。" };
struct message_t position2 = { 30, 1, "計測終点を指定して下さい。" };
#else
struct message_t position1 = { 30, 1, "Set Starting Point to Measure" };
struct message_t position2 = { 30, 1, "Set End Point to Measure" };
#endif
//struct message_t TWA_mes = { 14, 1, "function" };

#define	PilotFlag		Pilot_mode

// --- Product name
char			ProductName[32]		= "MCRXX";					// 製品名
// --- Inspection name
char			InspectionName[32]	= "TP-Surface";				// 検査名
// --- Program version ---
char			ProgramVer[32]		= "1.02";					// Program Version
//機台No.
char			MachineName[32]		= "  -00";					// 機台名

int				CameraType;
double			CCD_x;
int				Pilot_mode;											//パイロットモード
int				Incy76Ch;											// IN-CY-76 DSW data
int				Incy119Ch;										// IN-CY-119 DSW data //<z1>
int				Drcy80Ch;											// DR-CY-93 DSW data
int				Drcy83Ch;											// DR-CY-83 DSW data
int				ImageMode			= ImageCamera;					// 画像 mode
int             Option;

//仕様 ( 0:通常 1:表裏 2:ピンホール検査 3:パイロット )
        int				TWA_Insp_mode       = Insp_Normal;  				//仕様 ( 0:通常 1:表裏 2:ピンホール検査 3:パイロット )
        int				TWA_Spec            = SpecNormal;  					//製品仕様( 0:Normal 1:NP3 2:Jpw)
        int				TWA_Insp_level		= Insp_Low;						//検査モード（ 0:通常 1:詳細 ）
        int				TWA_Mark_Insp       = 0;							//標印検査

// --- NGコード ---
// ＮＧコード・コメント・クラスを宣言
R_CS_COUNTER_CLASS NgClass[] = {
    {	COUNTER_CLASS1		,"ワーク無し"		,"No Work",			},
	{	COUNTER_CLASS7		,"Reverse"			,"Reverse",			},
	{	COUNTER_CLASS2		,"G2 Fail"			,"G2 Fail",			},
	{	COUNTER_CLASS3		,"Chip Fail"		,"Chip Fail",		},
	{	COUNTER_CLASS4		,"PL Fail"			,"PL Fail",			},
	{	COUNTER_CLASS5		,"Mark Fail"		,"Mark Fail",		},
	{	COUNTER_CLASS6		,"Elec Fail"		,"Elec Fail",		},
	{	COUNTER_CLASS9		,"認識異常"			,"System",			},
	{	COUNTER_CLASS18		,"[Resistance]"		,"[Resistance]",	},
	{	COUNTER_CLASS19		,"[Ninshiki]"		,"[Ninshiki]",		},
	{	0					,NULL				,NULL				},
};
R_CS_COUNTER_CLASS NgClassTape[] = {
    {	COUNTER_CLASS1		,"ワーク無し"		,"No Work",			},
	{	COUNTER_CLASS2		,"G2 Fail"			,"G2 Fail",			},
	{	COUNTER_CLASS3		,"Chip Fail"		,"Chip Fail",		},
	{	COUNTER_CLASS4		,"PL Fail"			,"PL Fail",			},
	{	COUNTER_CLASS6		,"Elec Fail"		,"Elec Fail",		},
	{	COUNTER_CLASS9		,"認識異常"			,"System",			},
	{	0					,NULL				,NULL				},
};

R_CS_COUNTER_NG_CODE NgCode[] = {
	// Insp. Error
	{ NG_NO_WORK,				"NO Work   ",			"no work",					COUNTER_CLASS1 | COUNTER_NO_INC	},	// 1
	{ NG_CANT_INSP,				"Can'tInsp.",			"Can'n Insp.",				COUNTER_CLASS1 | COUNTER_NO_INC	},	// 2
	{ NG_INDEX_POS,				"Index Pos.",			"Index pos. Error",			COUNTER_CLASS1 | COUNTER_NO_INC	},	// 3
	{ NG_TAPE_POS,				"Tape Pos. ",			"Tape pos. Error",			COUNTER_CLASS1 | COUNTER_NO_INC	},	// 4
//	{ NG_REVERSE,				"Reverse   ",			"Chip reverse",				COUNTER_CLASS1 | COUNTER_NO_INC	},	// 5
	{ NG_FOCUS,					"Focus     ",			"Focus Error",				COUNTER_CLASS1 | COUNTER_NO_INC	},	// 6
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
	// Chip Reverse
	{ NG_REVERSE,				"Reverse   ",			"Chip reverse",				COUNTER_CLASS7 | COUNTER_NO_INC	},	// 5
	// M/C NG box
	{ MC_RESISTANCE,			"M/C NG [R]",			"M/C NG [Resistance]",		COUNTER_CLASS18 | COUNTER_NO_INC	},	// 81
	{ MC_NINSHIKI,				"M/C NG [N]",			"M/C NG [Ninshiki]",		COUNTER_CLASS19 | COUNTER_NO_INC	},	// 82

	{ 0,						NULL,					NULL,						0				},
};
R_CS_COUNTER_NG_CODE NgCodeTape[] = {
	// Insp. Error
	{ NG_NO_WORK,				"NO Work   ",			"no work",					COUNTER_CLASS1 | COUNTER_NO_INC	},	// 1
	{ NG_CANT_INSP,				"Can'tInsp.",			"Can'n Insp.",				COUNTER_CLASS1 | COUNTER_NO_INC	},	// 2
	{ NG_INDEX_POS,				"Index Pos.",			"Index pos. Error",			COUNTER_CLASS1 | COUNTER_NO_INC	},	// 3
	{ NG_TAPE_POS,				"Tape Pos. ",			"Tape pos. Error",			COUNTER_CLASS1 | COUNTER_NO_INC	},	// 4
	{ NG_REVERSE,				"Reverse   ",			"Chip reverse",				COUNTER_CLASS1 | COUNTER_NO_INC	},	// 5
	{ NG_FOCUS,					"Focus     ",			"Focus Error",				COUNTER_CLASS1 | COUNTER_NO_INC	},	// 6
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
	// Electrode Insp.
	{ NG_ELEC_X_SIZE,			"Elec.Xsize",			"Elec. X size",				COUNTER_CLASS6	},					// 51
	{ NG_ELEC_Y_SIZE,			"Elec.Ysize",			"Elec. Y size",				COUNTER_CLASS6	},					// 52
	{ NG_ELEC_EDGE_X,			"Elec.edgeX",			"Elec. edge X",				COUNTER_CLASS6	},					// 53
	{ NG_ELEC_EDGE_Y,			"Elec.edgeY",			"Elec. edge Y",				COUNTER_CLASS6	},					// 54
	{ NG_ELEC_KAKE,				"Elec.kake ",			"Elec. Kake",				COUNTER_CLASS6	},					// 55
	{ NG_ELEC_BLACK,			"Elec.Black",			"Elec. Black",				COUNTER_CLASS6	},					// 56
	{ NG_SIDE_ELEC,				"Side Elec.",			"Side Elec. X size",		COUNTER_CLASS6	},					// 57

	{ 0,						NULL,					NULL,						0				},
};

// Message
struct message_t	Message =	{ 100, 5,
									{	"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
										"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
										"CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
										"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD",
										"EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"}};

#ifdef __DOS__
	int __far critical_error_handler( unsigned deverr, unsigned errcode, unsigned far *devhdr );
#endif




// spec data
char					SpecData[20];										// バーコードデータ

// 設備データ & Meassage
struct	SetsubiData {
	int					data;												// Machine data
	int					print_no;											// プリンタ出力順番
	int					count;												// Total count( 0:NO, 1:YES )
	char				massage[30];										// Massage
}		Setsubi[300];
int						Std_rpm				= 0;							// 設備基準回転数(rmp)

#ifdef __DOS__
	struct	dosdate_t		Lot_StartDate;										// ロット開始 Date
	struct	dostime_t		Lot_StartTime;										// ロット開始 Time
	struct	dosdate_t		Lot_EndDate;										// ロット終了 Date
	struct	dostime_t		Lot_EndTime;										// ロット終了 Time
#elif __GNUC__
	time_t					ctLotStart;
	time_t					ctLotEnd;
	struct	tm				Lot_StartDateTime;									// ロット開始 Date,Time
	struct	tm				Lot_EndDateTime;									// ロット終了 Date,Time
#endif


//2012.03.14
int						insp_sum1[20];
int						count_total[20], count_R[20], count_N[20], count_L[20], count_Re[20];
int						pass_sum[20];                                           // Count Go
int                     count1[20][100];
/*********************************************************************************
	関数名：	ＴＷＡ＿Ｚｏｏｍ＿ｓｅｔ（ ｖｏｉｄ ）
	機　能：　	カメラレンズ倍率調整
	形　式：	void TWA_Zoom_set( void )
	引き数：	なし
	戻り値：	なし
	解　説：	カメラレンズの倍率を調整する。
	留意事項：	なし
*********************************************************************************/

void	TWA_Zoom_set( int level, int zoom_std )
	{
		double			l_b, b_l;
		double			lm;
		int				n, n1;
		int				x0 = FX_SIZE/2;
		int				y0 = FY_SIZE/2;
		int				stx1, stx2, sty1, sty2, stl;
		int				x1, x2, y1, y2, ly;
		int				rx1, rx2, ry1, ry2, l;
		char			work[30];

		while( R_get_cursor() != NO_TOUCH );

		//2値画面消去
			R_display_control( BIN_MEMORY | GRAY_THROUGH );
			R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
		//画像取込み
			R_display_control( BIN_MEMORY | GRAY_MEMORY );
			TWA_manu_video_in( level );

			l_b = (246*CCD_x)/6600;
			b_l = 6600/246/CCD_x;

			stx1 = x0 - 100;
			stx2 = x0 + 100;
			sty1 = y0 - ((zoom_std * l_b)/2);
			sty2 = y0 + ((zoom_std * l_b)/2);
			stl = sty2 - sty1;

		while( R_get_cursor() == NO_TOUCH ) {
			//画像取込み
			TWA_manu_video_in( level );
			R_move_gray_memory( 0, 1, 0, 0, FX_SIZE-1, FY_SIZE-1 );

			//エッジ測定
			y1 = y2 = ly = 0;
			l = FY_SIZE/2;
			TWA_es_initial( 1, 30, 1, 5 );
			for( n=n1=0; n<5; n++ ) {
				rx1 = rx2 = FX_SIZE/2-20+10*n;
				ry1 = ry2 = FY_SIZE/2;
				if( TWA_es_search( 270, &rx1, &ry1, l, ES_WHITE ) != OK ) ry1 = 0;
				if( TWA_es_search(  90, &rx2, &ry2, l, ES_WHITE ) != OK ) ry2 = 0;
				if(( ry1!=0 )&&( ry2!=0 )) {
					y1 += ry1;
					y2 += ry2;
					ly += (ry2-ry1);
					n1++;
				}
			}
			if( n1 != 0 ) {
				y1 = y1/1000/n1;
				y2 = y2/1000/n1;
				ly = ly/1000/n1;
			}

			R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );

			sprintf( work, "Zoom=%5.2f", CCD_x );
			R_chrdisp(  1, 1, work );

//			lm = lx * b_l;
			lm = ly * 6600/246/CCD_x;
			sprintf( work, "Measur=%5.3fmm", lm/1000 );
			R_chrdisp(  1, 3, work );

			//Judge
				if( abs( lm - zoom_std )<50 ) strcpy( work, "Pass" );
				else                          strcpy( work, "Fail" );
//				R_chrdisp_modify( 50, 1, work, DISP_V2 ) ;
				R_chrdisp(  30, 1, work );

			x1 = x0 - 30;
			x2 = x0 + 30;
			R_line( x1, y1, x2, y1, GRAPH_REVERSE, SOLID_LINE );
			R_line( x1, y2, x2, y2, GRAPH_REVERSE, SOLID_LINE );

			R_line( stx1, sty1, stx2, sty1, GRAPH_REVERSE, DASHED_LINE );
			R_line( stx1, sty2, stx2, sty2, GRAPH_REVERSE, DASHED_LINE );

			R_time_delay( 100000 );
		}
		while( R_get_cursor() != NO_TOUCH );
		R_display_control( BIN_MEMORY | GRAY_THROUGH );
		R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );

	}


/*********************************************************************************
	関数名：	ＴＷＡ＿Ｆｏｃｕｓ＿ｓｅｔ（ ｖｏｉｄ ）
	機　能：　	カメラレンズ焦点調整
	形　式：	void TWA_Focus_set( void )
	引き数：	なし
	戻り値：	なし
	解　説：	カメラレンズの焦点を調整する。
	留意事項：	なし
*********************************************************************************/

void	TWA_Focus_set( int bin_l, int Judge_F )
	{
		int				n, n1;
		int				l, es_n;
		int				rx1, rx2, ry1, ry2;
		int				x1, x2, y1, y2, ly;
		char			work[50];
		unsigned char	*ad;
		int				le[240], level, l_max;

		while( R_get_cursor() != NO_TOUCH );

		//2値画面消去
			R_display_control( BIN_MEMORY | GRAY_THROUGH );
			R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
		//画像取込み
			R_display_control( BIN_MEMORY | GRAY_MEMORY );
			TWA_manu_video_in( bin_l );

		R_es_set_debug( OFF );

		while( R_get_cursor() == NO_TOUCH ) {
			//画像取込み
			R_get_video( ANY_FEILD, GRAY_PLANE );
//			TWA_manu_video_in( bin_l );
			R_move_gray_memory( 0, 1, 0, 0, FX_SIZE-1, FY_SIZE-1 );
			R_defferential( 1, 2, Y_DIRECTION, 0, 0, FX_SIZE-1, FY_SIZE-1 );

//			R_max_filter( 3, 2, 0, 0, FX_SIZE-1, FY_SIZE-1 );

//			R_roberts( 1, 2, 0, 0, FX_SIZE-1, FY_SIZE-1 );


			R_move_gray_memory( 2, 0, FX_SIZE/2, 0, FX_SIZE-1, FY_SIZE-1 );
			R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );

			//ボケ計測
			y1 = y2 = ly= 0;
			l = FY_SIZE/2;
			TWA_es_initial( 2, 30, 1, 1 );
			for( n=es_n=0; n<5; n++ ) {
				rx1 = rx2 = FX_SIZE/2-20+10*n;
				ry1 = ry2 = FY_SIZE/2-50;
				if( TWA_es_search( 90, &rx1, &ry1, l, ES_BLACK ) != OK ) ry1 = 0;
				if( TWA_es_search( 90, &rx2, &ry2, l, ES_WHITE ) != OK ) ry2 = 0;
				if(( ry1!=0 )&&( ry2!=0 )) {
					y1 += ry1;
					y2 += ry2;
					ly += abs(ry2-ry1);
					es_n++;
				}
			}
			if( es_n != 0 ) {
				y1 = y1/1000/es_n;
				y2 = y2/1000/es_n;
				ly = ly/es_n;
			}

			sprintf( work, "Focus=%d", ly );
			R_chrdisp( 2, 2, work );

			x1 = FX_SIZE/2-10;
			x2 = FX_SIZE/2+10;

			if( es_n != 0 ) {
				R_line( x1, y1, x2, y1, GRAPH_DRAW, SOLID_LINE );
				R_line( x1, y2, x2, y2, GRAPH_DRAW, SOLID_LINE );
			}

			l_max = 0;
			if(( y1!=0 )&&( y2!=0 )) {
				y1 -= 10;
				y2 += 10;
				for( n=0; y1<y2; y1+=2, n++ ) {
					ad = Gray_address[1]+y1*512+256;
					le[n] = *ad + *(ad+1) + *(ad+512) + *(ad+512+1);
				}
				n1 = n-2;
				for( n=1; n<n1; n++ ) {
					level = abs(le[n-1]-le[n])/4 + abs(le[n]-le[n+1])/4;
					level *= level;
					l_max = max( l_max, level );
				}
			}
			sprintf( work, "Level=%d", l_max );
			R_chrdisp( 2, 3, work );

			//Judge
				if(( es_n!=0 )&&( ly<Judge_F )) strcpy( work, "Pass" );
				else                       strcpy( work, "Fail" );
//				R_chrdisp_modify( 50, 1, work, DISP_V2 ) ;
				R_chrdisp(  30, 1, work );

			R_time_delay( 500000 );
		}

		R_es_set_debug( OFF );

		while( R_get_cursor() != NO_TOUCH );
		R_display_control( BIN_MEMORY | GRAY_THROUGH );
		R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );

	}


/********************************************************************
	関数名：	ＴＷＡ＿ｒｓ＿ｃｌｅ（）
    機　能：　	ＲＳ２３２Ｃクリアー
	形　式：	#include "r_stdio.h"
				void TWA_rs_cle( void );
	引き数：	なし
	戻り値：	なし
	解　説：
	留意事項：	特にありません。
************************************************************************/
void TWA_rs_cle( )
{
	R_open_sio( SIO_CHANNEL1, RATE9600BPS, BIT_8, PARITY_NONE, STOP_2, FLOW_SOFT );
	while( R_getsio_rcvbuff( ) ) R_getsio( );
	R_close_sio( );
}


/************************************************
	座標回転計算
************************************************/

void	TWA_kaiten( int *x, int *y, int deg )
	{

		double			l, x1, y1, x2, y2;
	    double			sin, cos;
	    int				sindata, cosdata;
		int				atan10;

		x1 = *x;
		y1 = *y;

		l = hypot( x1, y1 );

		x2 = *x;
		y2 = *y;
		atan10 = R_atan( x2, y2 );
		if( x2 == 0 ) {
			if( y2>0 ) atan10 =  900;
			if( y2<0 ) atan10 = 2700;
		}

		if( x2<0 && y2>=0 ) atan10 += 1800;
		if( x2<0 && y2<0 )  atan10 += 1800;
		if( x2>=0 && y2<0 ) atan10 += 3600;

		atan10 += deg;

	    cosdata = R_cosfunc( atan10 );
	    cos = (double)cosdata / (double)0x8000;
	    sindata = R_sinfunc( atan10 );
	    sin = (double)sindata / (double)0x8000;

//		*x = l*cos;
//		*y = l*sin;

		x1 = l*cos;
		y1 = l*sin;

		if( x1 > 0 ) x1++;
		if( x1 < 0 ) x1--;
		if( y1 > 0 ) y1++;
		if( y1 < 0 ) y1--;

		*x = x1;
		*y = y1;

	}

#ifdef __DOS__
	int __far critical_error_handler( unsigned deverr,
	                                  unsigned errcode,
	                                  unsigned far *devhdr )
	  {
	    cprintf( "Critical error: " );
	    cprintf( "deverr=%4.4X errcode=%d\r\n",
	             deverr, errcode );
	    cprintf( "devhdr = %Fp\r\n", devhdr );
	    return( _HARDERR_FAIL );
	  }
#endif
/*
void __far error_handler( )
{
	_hardresume( _HARDERR_FAIL );
}
*/



/************************************************
	Ｇ２エッジ検出初期設定
************************************************/

void	TWA_es_initial( int page, int level, int av, int noise )
	{
        R_es_set_page( page );			//処理対象のページを指定します。
		R_es_set_level( level );		//設定未満の階調変化はエッジとみなさない。
		R_es_set_average( av );			//片側１画素、計３画素の平均を取る。
		R_es_set_noise( noise );		//ノイズとみなす幅を設定する。
		R_es_set_slice( 500 );			//最大値と最小値の真ん中でエッジと判断する。
	}


/************************************************
	Ｇ２エッジ検出
************************************************/

int		TWA_es_search( int deg, int *xs, int *ys, int l, int contrast )
	{
		int		rx, ry, rc;

		R_es_set_degree( deg );					//認識方向を設定する。→：０°↓：９０°←：１８０°↑：２７０°
		rc = contrast;							//どのエッジ（暗い→明るい：ES_BLACK、明るい→暗い:ES_WHITE）を捜すか指定する。
        R_es_set_length( l );					//サーチ距離を設定する。
		R_es_set_start( *xs, *ys );				//認識開始点の設定

		if( R_es_search( &rx, &ry, &rc ) != OK ) return(-1);
//		*xs = rx/1000;
//		*ys = ry/1000;
		*xs = rx;
		*ys = ry;

		return(0);
	}


/************************************************
	画面取り込み
************************************************/

void	TWA_manu_video_in( int level )
	{
		int				mode;

		if( FySize != 180 ) mode = 1;
		else                    mode = 2;

		R_change_camera_mode(STANDBY | CameraType);		// trigger stand_by
		R_start_clk( );
		if( ImageMode == ImageCamera ) {
//			TWA_get_video( level, mode );
		} else {
			R_display_control( BIN_MEMORY | GRAY_MEMORY );
			R_move_gray_memory( 3, 0, 0, 0, FxSize-1, FySize-1 );
			R_gray2bin16( level, 0, 0, FxSize, FySize, Gray_address[3], Bin_address[0] );
		}
		printf( "Screen taking time：%6d usec\n" , R_read_clk() );
		R_change_camera_mode( NORMAL|CameraType );	// trigger normal
	}


/*********************************************************************************
	関数名：	ＴＷＡ＿ＩＤ＿ｃｈｅｃｋ
	機　能：	ＩＤ　ファイルを確認する
	形　式：	int TWA_ID_check( void )
	引き数：	なし
	戻り値：	値		定　数		意　味
				0		OK			ＩＤファイル有り
				-1		ERROR		ＩＤファイルなし
	解　説：	認識検査条件の設定制限する。
*********************************************************************************/

int		TWA_ID_check( void )
	{
		FILE					*fp;
		char					work[50];

		// file open
			fp = fopen( "TWA_ID.DAT", "r" );		// ID file open
			if( fp == NULL ) {
				// mouse check
					while( R_get_cursor() != NO_TOUCH );

#ifdef	JAPANESE
                strcpy( work, "このメニュー操作は、禁止されています" );
				R_chrdisp_center( 10, work );
				strcpy( work, "マウスをクリックして下さい" );
				R_chrdisp_center( 12, work );
#else
				strcpy( work, "This menu operation is prohibited." );
				R_chrdisp_center( 10, work );
				strcpy( work, "Please click a mouse" );
				R_chrdisp_center( 12, work );
#endif
				// マウスクリック待ち
					while( NO_TOUCH == R_get_cursor() );
					while( NO_TOUCH != R_get_cursor() );
					strcpy( work, "                                      " );
					R_chrdisp_center( 10, work );
					R_chrdisp_center( 12, work );

				return ERROR;
			}
			fclose( fp );

		return OK;
	}


/***************************************************
        メッセージの描画
    機能	ウィンドウ上にメッセージを描画します。
	引き数
		char	mes[]	文字

		int		color
		表示色の指定
		値			定　数			意　味
		────────────────────────
		0x00000007	DRAW_WHITE		白色で表示を行う。
		0x00000002	DRAW_RED		赤色で表示を行う。
		0x00000004	DRAW_GREEN		緑色で表示を行う。
		0x00000006	DRAW_YELLOW	黄色で表示を行う。

		int		mode
		表示の時間
		値	意　味
		────────────────────────
		0:	1秒描画後消去
		1:	0.5秒描画後消去
		2:	描画→クリック待ち→消去
		3:	YES/NOクリック待ち→消去
		4:	描画→YES/NOクリック待ち→消去
		5:	描画のみ

		int		pos
		表示位置
		値	意　味
		────────────────────────
		0:	画面中央
		1:	画面上限
		2:	画面下限

	Return		:	modeが3,4の場合のみYES,NO
	注意事項	:	modeが5のみDrawMessageOff()で消去して
					ください。
***************************************************/

int		DrawMessage( char mes[], int color, int mode, int pos )
	{
		int						dn, cn, cl, clmax;
		int						xn, yn;
		int						ans = YES;
		int						nWidthX	= FxSize;
		int						nWidthY	= FySize;
		int						font_x, font_y;
		// color check
			switch( color ) {
				case DRAW_WHITE:
				case DRAW_RED:
				case DRAW_GREEN:
				case DRAW_YELLOW:
					break;
				default:
					color = DRAW_GREEN;
					break;
			}
#ifdef __DOS__
		// 文字配列
			xn = 0;
			yn = 0;
			cn = strlen( mes );
			clmax = 30;
			for( dn=0; dn<=cn; dn++ ) {
				Message.str[yn][xn] = mes[dn];
				if((mes[dn]=='\n')||(mes[dn]==0)) {
					Message.str[yn][xn] = 0;
					clmax = max( clmax, (strlen(Message.str[yn])+4) );
					yn++;
					xn=0;
				} else {
					xn++;
				}
			}
		// マウスクリックの表記
			switch( mode ) {
				case 2:
					strcpy( Message.str[yn], "Please Click!!" );
					yn++;
					break;
				case 3:
				case 4:
					strcpy( Message.str[yn], "Left: [ YES ] Right: [ NO ]" );
					yn++;
					break;
			}
			Message.ylen = yn;
			Message.xlen = clmax;
			cl = clmax;

		// フォントサイズの取得
			font_x = R_draw_consult_font_x( DISP_NORMAL );
			font_y = R_draw_consult_font_y( DISP_NORMAL );

        // メッセージの表示
		switch( pos ) {
			case 0:
				R_draw_win_message( color, DISP_NORMAL, (nWidthX/font_x-cl)/2, (nWidthY/font_y-2)/2, &Message );
				break;
			case 1:
				R_draw_win_message( color, DISP_NORMAL, (nWidthX/font_x-cl)/2, 0, &Message );
				break;
			case 2:
				R_draw_win_message( color, DISP_NORMAL, (nWidthX/font_x-cl)/2, nWidthY/font_y-5, &Message );
				break;
			default:
				R_draw_win_message( color, DISP_NORMAL, (nWidthX/font_x-cl)/2, (nWidthY/font_y-2)/2, &Message );
				break;
		}
		switch( mode ){
            case 0:		// 通常モード（ 1.0秒間表示 )
				R_time_delay( 1000000 );
				break;
            case 1:		// 高速モード（ 0.5秒間表示 )
			case 5:
				R_time_delay( 500000 );
				break;
			case 2:		// クリック待ち
				R_mouse_de_yesno();
				break;
			case 3:		// クリック結果待ち
			case 4:
				ans = R_mouse_de_yesno();
				break;
		}
		if( mode != 5 ){
			R_draw_close_window();
		}
#elif __GNUC__
		switch( mode ) {
			case 0:
				R_draw_popup_message( color, DISP_NORMAL, 1000000, mes);
				break;
			case 1:
			case 5:
				R_draw_popup_message( color, DISP_NORMAL, 500000, mes);
				break;
			case 2:
				R_draw_popup_message( color, DISP_NORMAL, POPUP_WAIT_CLICK, mes);
				break;
			case 3:
			case 4:
				ans = R_draw_popup_message( color, DISP_NORMAL, POPUP_WAIT_CLICK, mes);
				break;
		}
#endif
		return( ans );

	}	// DrawMessage( char mes0[], char mes1[], int mode, int pos )


/***************************************************
            メッセージ消去
	機能	DrawMessage( .. ,5 )の場合のウィンドウを消去
	Entry :	-----
	Return: -----
***************************************************/

void	DrawMessageOff( void )
	{

		R_draw_close_window();

	}	// DrawMessageOff( void ) end


//==========================================================================
//	関数名：	ＢｉｎＣｌｓＰｎ
//	機　能：	２値画面消去
//	形　式：	int BinClsPn( int binPn, int xs, int ys, int xe, int ye )
//	引き数：	ｂｉｎＰｎは、バイナリプレーン番号
//				ｘｓは、消去四角形の頂点１のｘ座標です。
//				ｙｓは、消去四角形の頂点１のｙ座標です。
//				ｘｅは、消去四角形の頂点２のｘ座標です。
//				ｙｅは、消去四角形の頂点２のｙ座標です。
//	戻り値：	ありません。
//	留意事項：	特にありません。
//==========================================================================

void	BinClsPn( int binPn, int xs, int ys, int xe, int ye )
	{
		unsigned char			*bak_address;

		bak_address = Bin_address[0];
		Bin_address[0] = Bin_address[binPn];

		R_bin_cls( xs, ys, xe, ye );

		Bin_address[0] = bak_address;

	}	// BinClsPn( int binPn, int xs, int ys, int xe, int ye ) end


//==========================================================================
//	関数名：	ＧｒａｙＣｌｓＰｎ
//	機　能：	グレー画面消去
//	形　式：	int GrayClsPn( int GrayPn, int xs, int ys, int xe, int ye )
//	引き数：	ＧｒａｙＰｎは、グレープレーン番号
//				ｘｓは、消去四角形の頂点１のｘ座標です。
//				ｙｓは、消去四角形の頂点１のｙ座標です。
//				ｘｅは、消去四角形の頂点２のｘ座標です。
//				ｙｅは、消去四角形の頂点２のｙ座標です。
//	戻り値：	ありません。
//	留意事項：	特にありません。
//==========================================================================

void	GrayClsPn( int grayPn, int xs, int ys, int xe, int ye )
	{
		int						rx, ry;
		unsigned char			*bak_address;

		// position check

        if( xs > xe ) {
				rx = xs;
				xs = xe;
				xe = rx;
			}
			if( ys > ye ) {
				ry = ys;
				ys = ye;
				ye = ry;
			}
		// Gray memory No. set
			bak_address		= Gray_address[0];
			Gray_address[0]	= Gray_address[grayPn];
		// Gray image clear
			R_gray_fill( xs, ys, xe, ye, 0 );
		// Gray memory No. reset
			Gray_address[0] = bak_address;

	}	// GrayClsPn( int grayPn, int xs, int ys, int xe, int ye ) end


//================================================
//	縮退フィルター
//================================================

void	FilterBinContrct( BIN_FILTER_PARAMETER *pFilter, int xsize, int ysize )
	{
		int						rx, ry;

		// area check
			if( pFilter->nXmin > pFilter->nXmax ) {
				rx = pFilter->nXmin;
				pFilter->nXmin = pFilter->nXmax;
				pFilter->nXmax = rx;
			}
			if( pFilter->nYmin > pFilter->nYmax ) {
				ry = pFilter->nYmin;
				pFilter->nYmin = pFilter->nYmax;
				pFilter->nYmax = ry;
			}
			if( pFilter->nXmin < 0 )		return;
			if( pFilter->nXmax >= FxSize )	return;
			if( pFilter->nYmin < 0 )		return;
			if( pFilter->nYmax >= FySize )	return;

		// Y filter
		switch( ysize ) {
			case 2:
				R_contrct_window( FILTER_12, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 3:
				R_contrct_window( FILTER_13, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 4:
				R_contrct_window( FILTER_12, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_contrct_window( FILTER_13, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 5:
				R_contrct_window( FILTER_15, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 6:
				R_contrct_window( FILTER_12, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_contrct_window( FILTER_15, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 7:
				R_contrct_window( FILTER_13, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_contrct_window( FILTER_15, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 8:
				R_contrct_window( FILTER_12, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_contrct_window( FILTER_13, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_contrct_window( FILTER_15, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 9:
				R_contrct_window( FILTER_15, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_contrct_window( FILTER_15, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			}

		// X filter
		switch( xsize ) {
			case 2:
				R_contrct_window( FILTER_21, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 3:
				R_contrct_window( FILTER_31, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 4:
				R_contrct_window( FILTER_21, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_contrct_window( FILTER_31, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 5:
				R_contrct_window( FILTER_51, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 6:
				R_contrct_window( FILTER_21, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_contrct_window( FILTER_51, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 7:
				R_contrct_window( FILTER_31, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_contrct_window( FILTER_51, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 8:
				R_contrct_window( FILTER_21, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_contrct_window( FILTER_31, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_contrct_window( FILTER_51, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 9:
				R_contrct_window( FILTER_51, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_contrct_window( FILTER_51, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			}

	}	// FilterBinContrct( BIN_FILTER_PARAMETER *pFilter, int xsize, int ysize ) end


//================================================
//	膨張フィルター
//================================================

void	FilterBinExpand( BIN_FILTER_PARAMETER *pFilter, int xsize, int ysize )
	{
		int						rx, ry;

		// area check
			if( pFilter->nXmin > pFilter->nXmax ) {
				rx = pFilter->nXmin;
				pFilter->nXmin = pFilter->nXmax;
				pFilter->nXmax = rx;
			}
			if( pFilter->nYmin > pFilter->nYmax ) {
				ry = pFilter->nYmin;
				pFilter->nYmin = pFilter->nYmax;
				pFilter->nYmax = ry;
			}
			if( pFilter->nXmin < 0 )		return;
			if( pFilter->nXmax >= FxSize )	return;
			if( pFilter->nYmin < 0 )		return;
			if( pFilter->nYmax >= FySize )	return;

		// Y filter
		switch( ysize ) {
			case 2:
				R_expand_window( FILTER_12, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 3:
				R_expand_window( FILTER_13, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 4:
				R_expand_window( FILTER_12, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_expand_window( FILTER_13, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 5:
				R_expand_window( FILTER_15, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 6:
				R_expand_window( FILTER_12, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_expand_window( FILTER_15, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 7:
				R_expand_window( FILTER_13, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_expand_window( FILTER_15, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 8:
				R_expand_window( FILTER_12, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_expand_window( FILTER_13, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_expand_window( FILTER_15, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 9:
				R_expand_window( FILTER_15, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_expand_window( FILTER_15, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			}

		// X filter
		switch( xsize ) {
			case 2:
				R_expand_window( FILTER_21, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 3:
				R_expand_window( FILTER_31, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 4:
				R_expand_window( FILTER_21, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_expand_window( FILTER_31, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 5:
				R_expand_window( FILTER_51, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 6:
				R_expand_window( FILTER_21, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_expand_window( FILTER_51, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 7:
				R_expand_window( FILTER_31, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_expand_window( FILTER_51, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 8:
				R_expand_window( FILTER_21, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_expand_window( FILTER_31, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_expand_window( FILTER_51, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			case 9:
				R_expand_window( FILTER_51, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				R_expand_window( FILTER_51, pFilter->nBinPn, pFilter->nXmin, pFilter->nYmin, pFilter->nXmax, pFilter->nYmax );
				break;
			}

	}	// FilterBinExpand( BIN_FILTER_PARAMETER *pFilter, int xsize, int ysize ) end


/************************************************
	Setsubi Data Load file
************************************************/

int		SetsubiDataLoadfile( void )
	{
		int						i_mode = R_CS_GetInspectMode();
		int						n, ret;
		int						dn, pn, cn;
		char					str[128], label[50];
		FILE					*fp;

		// Initial
			for( n=0; n<260; n++ ) Setsubi[n].massage[0] = 0;
			for( n=0; n<260; n++ ) Setsubi[n].data = 0;

		// file open
			fp = fopen( "Setsubi.dat", "r" );		// 設備 file open
			if( fp == NULL ) {
				sprintf( Comment, "File open Error !!\n[ SETSUBI.DAT ]" );
				DrawMessage( Comment, DRAW_RED, 2, 0 );
				fclose( fp );
				return ERROR;
			}

		// data read
			if( fgets( str, sizeof str, fp ) == NULL ) {
				sprintf( Comment, "Error !!  No data\n[ SETSUBI.DAT ]" );
				DrawMessage( Comment, DRAW_RED, 2, 0 );
				fclose( fp );
				return ERROR;
			}
			for( n=0; n<300; n++ ) {
				if( fgets( str, sizeof str, fp ) == NULL ) break;
                ret = sscanf( str, "%d, %d, %d, %s\n", &dn, &pn, &cn, label );
				if( ret == 4 ) {
					if(( dn > 0 )&&( dn < 260 )) {
						Setsubi[dn].print_no	= pn;
						Setsubi[dn].count		= cn;
						strcpy( Setsubi[dn].massage, label );
					}
				}
			}

		// file close
			fclose( fp );

		// data change '_' -> ' '
			for( n=0; n<260; n++ ) {
				if( Setsubi[n].massage[0] == 0 ) continue;
				for( dn=0; dn<30; dn++ ) {
					if( Setsubi[n].massage[dn] == 0 ) break;
					if( Setsubi[n].massage[dn] == '_' ) Setsubi[n].massage[dn] = ' ';
				}
			}

			if( i_mode & INSPECT_STEP ) {
				for( n=0; n<260; n++ ) {
					if( Setsubi[n].massage[0] == 0 ) continue;
					printf( "Data No.%2d : %s\n", n, Setsubi[n].massage );
					if( n%10 == 0 ) {
						WaitMouseClick();
					}
				}
				WaitMouseClick();
			}

		return OK;

	}	// SetsubiDataLoadfile( void ) end


/************************************************
	Setsubi Data Check
************************************************/

void	SetsubiDataCheck( void )
	{
		int						dn;
		RTIME					waitTime;

		// I/O board check
		//<z1>			if( Incy76Ch == ERROR ) return;
			if( Incy119Ch == ERROR ) return; //<z1>

		// Data No. check
			if(((Option & NO_INCY119)!=0)&&((Option & NO_INCY76)==0)){		//<shio>
				dn = 0x0ff & ~(R_gpio_read( Incy76Ch | CNa_IN_port_1 ));
			}else{
				dn = 0x0ff & ~(R_gpio_read( Incy119Ch | CNa_IN_port_1 )); //<z1>
			}
			if( dn == 0 ) return;

		// Time waite
			R_time_delay( 1000 );		// 1.0msec

			if(((Option & NO_INCY119)!=0)&&((Option & NO_INCY76)==0)){		//<shio>
				dn = 0x0ff & ~(R_gpio_read( Incy76Ch | CNa_IN_port_1 ));
			}else{
				dn = 0x0ff & ~(R_gpio_read( Incy119Ch | CNa_IN_port_1 )); //<z1>
			}
			switch( dn ) {
				case 0x000:
					break;
				case 0x0ff:
					// Print Busy ON
						SetsubiDataPrintBusy( ON );
						// Print
							SetsubiDataPrint();
						// Data write
							SetsubiDataWrite();
						// Data No.0 wait
							R_time_start( &waitTime );
							while( dn != 0 ) {
							if(((Option & NO_INCY119)!=0)&&((Option & NO_INCY76)==0)){		//<shio>
								dn = 0x0ff & ~(R_gpio_read( Incy76Ch | CNa_IN_port_1 ));
							}else{
								dn = 0x0ff & ~(R_gpio_read( Incy119Ch | CNa_IN_port_1 )); //<z1>
							}
							if( 5000000 < R_time_read(&waitTime) ) {
									sprintf( Comment, "ERROR. Print Time-out" );
									R_chrdisp( 3, 12, Comment );
									WaitMouseClick();
									sprintf( Comment, "                     " );
									R_chrdisp( 3, 12, Comment );
									break;
								}
							}
					// Print Busy OFF
						SetsubiDataPrintBusy( OFF );
					break;
				default:
					// Read Busy ON
						SetsubiDataReadBusy( ON );
						// Data read
							SetsubiDataRead( dn );
						// Data No.0 wait
							R_time_start( &waitTime );
							while( dn != 0 ) {
								if(((Option & NO_INCY119)!=0)&&((Option & NO_INCY76)==0)){		//<shio>
									dn = 0x0ff & ~(R_gpio_read( Incy76Ch | CNa_IN_port_1 ));
								}else{
									dn = 0x0ff & ~(R_gpio_read( Incy119Ch | CNa_IN_port_1 )); //<z1>
								}
								if( 5000000 < R_time_read(&waitTime) ) {
									sprintf( Comment, "ERROR. Data read Time-out" );
									R_chrdisp( 3, 12, Comment );
									WaitMouseClick();
									sprintf( Comment, "                         " );
									R_chrdisp( 3, 12, Comment );
									break;
								}
							}
					// Read Busy OFF
						SetsubiDataReadBusy( OFF );
					break;
			}

		// Data No. check

	}


////////////////////////////////////////////////////////////
//              Print test                                //
////////////////////////////////////////////////////////////

void	Print1( void )
	{
		int						dn;

		// 設備 Data Test
			for( dn=0; dn<260; dn++ ) {
				if( Setsubi[dn].massage[0] == 0 )	continue;
				 Setsubi[dn].data = dn;
			}
		// Print out test
			SetsubiDataPrint();
		// Data write test
		//	SetsubiDataWrite();
		// 設備 Data Initial
			for( dn=0; dn<260; dn++ ) Setsubi[dn].data = 0;

	}


/************************************************
	Setsubi Data Read
************************************************/

void	SetsubiDataRead( int dn )
	{
		int						data_l, data_h;

		// Data No. check
			if( dn != 0 ) {
				// Data read
				if(((Option & NO_INCY119)!=0)&&((Option & NO_INCY76)==0)){		//<shio>
					data_l = ~(R_gpio_read( Incy76Ch | CNa_IN_port_2 ));
					data_h = ~(R_gpio_read( Incy76Ch | CNa_IN_port_3 ));
				}else{
					data_l = ~(R_gpio_read( Incy119Ch | CNa_IN_port_2 )); //<z1>
					data_h = ~(R_gpio_read( Incy119Ch | CNa_IN_port_3 ));	//<z1>
				}
					data_l &= 0x0ff;
					data_h &= 0x0ff;
					data_h <<= 8;
					Setsubi[dn].data = (data_h | data_l);

//				sprintf( Comment, "%-17s:%6d\n", Setsubi[dn].massage, Setsubi[dn].data );
//				R_chrdisp( 5, 10, Comment );

			}

		// Time waite
			R_time_delay( 1000 );		// 1.0msec

	}	// SetsubiDataRead( void ) end


/************************************************
	Setsubi Data Read
************************************************/
#ifdef __DOS__
int		SetsubiDataPrint( void )
	{
		int						dn, sn, cn;
		int						alarm_sum, alarm_lf;
		int						andon;
		int						insp_sum[2], ng_sum;
		int						sort[260];
		int						count;
		int						pass;                                               // Count Go
		double					dp_pass;                                            // Count Go
		double					dp, mtbf, mttr;
		FILE					*fp;
		R_CS_COUNTER_DATA		*pCounter	= R_CS_GetCounterDataStruct(0);			// カウンタ構造体へのポインタ取得
		int						ng_num		= R_CS_CounterGetNgNum(pCounter);		// ＮＧ種別数の取得
		char 					*name;

		// initial
			for( sn=0; sn<260; sn++ ) sort[sn] = 0;

		// file open
			fp = fopen( "prn", "w" );
			if( fp == NULL ) {
				strcpy( Comment, "Printer open Error !!" );
				R_chrdisp( 1, 1, Comment );
				fclose( fp );
				return ERROR;
			}

		// ロット終了 Date & Time
#ifdef __DOS__
			_dos_getdate(&Lot_EndDate);		// 現在の日付を取得
			_dos_gettime(&Lot_EndTime);		// 現在の時間を取得
#elif __GNUC__
			ctLotEnd = time(NULL);
			memcpy(&Lot_EndDateTime , localtime(&ctLotEnd) ,sizeof(Lot_EndDateTime));;
#endif
		// Type & 機台No.
			fprintf( fp, "%s (%drpm) %s\n", ProductName, Std_rpm, MachineName );
		// Lot Data
			fprintf( fp, "Lot Data\n" );
			fprintf( fp, "  %s\n", SpecData );

		// Lot Start Date & Time
			fprintf( fp, "Lot Start\n" );
#ifdef __DOS__
			fprintf( fp, "  date:%04d-%02d-%02d\n", Lot_StartDate.year, Lot_StartDate.month, Lot_StartDate.day );
			fprintf( fp, "  time:%02d:%02d:%02d\n", Lot_StartTime.hour, Lot_StartTime.minute, Lot_StartTime.second );
#elif __GNUC__
			fprintf( fp, "  date:%04d-%02d-%02d\n", Lot_StartDateTime.tm_year+1900, Lot_StartDateTime.tm_mon+1, Lot_StartDateTime.tm_mday );
			fprintf( fp, "  time:%02d:%02d:%02d\n", Lot_StartDateTime.tm_hour     , Lot_StartDateTime.tm_min, Lot_StartDateTime.tm_sec );
#endif
		// Lot End Date & Time
			fprintf( fp, "Lot End\n" );
#ifdef __DOS__
			fprintf( fp, "  date:%04d-%02d-%02d\n", Lot_EndDate.year, Lot_EndDate.month, Lot_EndDate.day );
			fprintf( fp, "  time:%02d:%02d:%02d\n", Lot_EndTime.hour, Lot_EndTime.minute, Lot_EndTime.second );
#elif __GNUC__
			fprintf( fp, "  date:%04d-%02d-%02d\n", Lot_EndDateTime.tm_year+1900, Lot_EndDateTime.tm_mon+1, Lot_EndDateTime.tm_mday );
			fprintf( fp, "  time:%02d:%02d:%02d\n", Lot_EndDateTime.tm_hour     , Lot_EndDateTime.tm_min, Lot_EndDateTime.tm_sec );
#endif
		// LF
			fprintf( fp, "\n" );

		// Print No. sort
			for( dn=0; dn<260; dn++ ) {
				sn = Setsubi[dn].print_no;
				if( sn >= 260 ) sn = 259;
				sort[sn] = dn;
			}
		// Alarm & Andon count
			alarm_sum = 0;
			andon = 0;
			for( sn=0; sn<260; sn++ ) {
				dn = sort[sn];
				if( Setsubi[dn].massage[0] == 0 )	continue;
				if( Setsubi[dn].data == 0 )			continue;
				if( Setsubi[dn].count > 0 )			alarm_sum += Setsubi[dn].data;
				if( Setsubi[dn].massage[0] == '*' )	andon += Setsubi[dn].data;
			}
		// MTBF
			if( Setsubi[1].data == 0 ) {
				mtbf = 0.0;
			} else {
				if( alarm_sum <= 1 ) {
					mtbf = Setsubi[1].data;
				} else {
					mtbf = (double)( Setsubi[1].data ) / (double)( alarm_sum );
				}
			}
		// MTTR
			if( Setsubi[3].data == 0 ) {
				mttr = 0.0;
			} else {
				if( alarm_sum <= 1 ) {
					mttr = Setsubi[3].data;
				} else {
					mttr = (double)( Setsubi[3].data ) / (double)( alarm_sum );
				}
			}

		// Data print
			alarm_lf = 0;
		// Machine data print out
			for( sn=0; sn<260; sn++ ) {
				dn = sort[sn];
				if( Setsubi[dn].massage[0] == 0 )	continue;
				if( Setsubi[dn].data == 0 )			continue;
				if( Setsubi[dn].count < 0 )			continue;
				if(( Setsubi[dn].count > 0 )&&( alarm_lf == 0 )) {
					alarm_lf++;
					fprintf( fp, "\n" );
				}
				fprintf( fp, "%-17s:%6d\n", Setsubi[dn].massage, Setsubi[dn].data );
				if( dn == 1 ) {
					fprintf( fp, "%-17s:%6.1f\n", " MTBF", mtbf );
					fprintf( fp, "%-17s:%6.1f\n", " MTTR", mttr );
				}
			}
				fprintf( fp, "%-17s:%6d\n", "Alarm Total", alarm_sum );
		// LF
			fprintf( fp, "\n" );

		// Counter
			for( cn=0; cn<2; cn++ ) {
				insp_sum[cn] = R_CS_CounterGetTotalCount( pCounter )+R_CS_CounterGetNgCount( pCounter, MC_RESISTANCE );
				if( cn > 0) {
					if( insp_sum[cn] == insp_sum[cn-1] )	continue;
				}
				pass = R_CS_CounterGetGoCount( pCounter );                                 		 // Count Print Out
				if(insp_sum[cn] == 0)	dp_pass = 0;                                               // Count Print Out
				else					dp_pass = pass*100.0/insp_sum[cn];                         // Count Print Out
				ng_sum = 0;
				// count data
					fprintf( fp, "Index Ninshiki\n" );
					fprintf( fp, "  Counter( %d )\n", cn+1 );
					fprintf( fp, "%10s:%6s(%5s)\n", "item", "pcs", "%" );
					if( pass > 0 ) fprintf( fp, "%10s:%6d(%5.2f)\n", "Pass", pass, dp_pass );
//					for( dn=11; dn<=80; dn++ ) {
					for( dn=0; dn<ng_num; dn++ ) {
						if( 10 >= NgCode[dn].nCode )	continue;
						if( 80 <  NgCode[dn].nCode )	continue;
						count = R_CS_CounterGetNgCount(pCounter, NgCode[dn].nCode);
						if( count <= 0 )	continue;
                        // --- NGが発生したコードのみカウンタ表示
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
///						char *name = R_CS_CounterGetCodeName(pCounter, NgCode[dn].nCode, COUNTER_NAME_JP);
						name = R_CS_CounterGetCodeName(pCounter, NgCode[dn].nCode, COUNTER_NAME_JP);
						if(name != NULL){
							fprintf( fp, "%s:%6d(%5.2f)\n", name, count, dp );
							fprintf( fp, "%s:%6d(%5.2f)\n", name, count, dp );
						}
					}
				// NG total
					count = R_CS_CounterGetNgTotalCount( pCounter );
					if( insp_sum[cn] == 0 )	dp = 0.0;
					else					dp = count*100.0/insp_sum[cn];
					fprintf( fp, "%s:%6d(%5.2f)\n", "NG Total  ", count, dp );
				// LF
					fprintf( fp, "\n" );
				// MC_RESISTANCE
					dn = MC_RESISTANCE;
					count = R_CS_CounterGetNgCount(pCounter, dn);
					if( count > 0 ) {
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
///						char *name = R_CS_CounterGetCodeName(pCounter, NgCode[dn].nCode, COUNTER_NAME_JP);
						name = R_CS_CounterGetCodeName(pCounter, dn, COUNTER_NAME_JP);
						fprintf( fp, "%s:%6d(%5.2f)\n", name, count, dp );
					}
				// MC_NINSHIKI
					dn = MC_NINSHIKI;
					count = R_CS_CounterGetNgCount(pCounter, dn);
					if( count > 0 ) {
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
///						char *name = R_CS_CounterGetCodeName(pCounter, NgCode[dn].nCode, COUNTER_NAME_JP);
						name = R_CS_CounterGetCodeName(pCounter, dn, COUNTER_NAME_JP);
						fprintf( fp, "%s:%6d(%5.2f)\n", name, count, dp );
					}
				// NG_LIGHTING
					dn = NG_LIGHTING;
					count = R_CS_CounterGetNgCount(pCounter, dn);
					if( count > 0 ) {
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
///						char *name = R_CS_CounterGetCodeName(pCounter, NgCode[dn].nCode, COUNTER_NAME_JP);
						name = R_CS_CounterGetCodeName(pCounter, dn, COUNTER_NAME_JP);
						fprintf( fp, "%s:%6d(%5.2f)\n", name, count, dp );
					}
				// NG_REVERSE
					dn = NG_REVERSE;
					count = R_CS_CounterGetNgCount(pCounter, dn);
					if( count > 0 ) {
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
///						char *name = R_CS_CounterGetCodeName(pCounter, NgCode[dn].nCode, COUNTER_NAME_JP);
						name = R_CS_CounterGetCodeName(pCounter, dn, COUNTER_NAME_JP);
						fprintf( fp, "%s:%6d(%5.2f)\n", name, count, dp );
					}
			}

		// Andon check
			if( andon > 0 ) {
				fprintf( fp, "\n\n" );		// LF 2
				fprintf( fp, "%-17s:%6d\n", "** ANDON LOT **", andon );
			}

		// LF 3
			fprintf( fp, "\n\n\n" );

		// file close
			fclose( fp );

		return OK;

	}	// SetsubiDataPrint( void ) end

#elif __GNUC__
int		SetsubiDataPrint( void )
	{
		int						dn, sn, cn;
		int						alarm_sum, alarm_lf;
		int						andon;
		int						insp_sum[2], ng_sum;
		int						sort[260];
		int						count;
		int						pass;                                               // Count Go
		double					dp_pass;                                            // Count Go
		double					dp, mtbf, mttr;
		R_CS_COUNTER_DATA		*pCounter	= R_CS_GetCounterDataStruct(0);			// カウンタ構造体へのポインタ取得
		int						ng_num		= R_CS_CounterGetNgNum(pCounter);		// ＮＧ種別数の取得
		char 					*name;
		int						hSio;
		char					pbuf[256];
		RTIME					waitTime;
		int                     nm;

		// initial
			for( sn=0; sn<260; sn++ ) sort[sn] = 0;

		// file open
			if( (hSio=R_sio2_open( SIO_CHANNEL(5), RATE4800BPS, BIT_8, PARITY_NONE, STOP_1, FLOW_HARD )) <= 0 ) {
				strcpy( Comment, "Printer open Error !!" );
				R_chrdisp( 1, 1, Comment );
				return ERROR;
			}

		// ロット終了 Date & Time
			ctLotEnd = time(NULL);
			memcpy(&Lot_EndDateTime , localtime(&ctLotEnd) ,sizeof(Lot_EndDateTime));;
		// Type & 機台No.
			sprintf( pbuf, "%s (%drpm) %s\r\n", ProductName, Std_rpm, MachineName );
			R_sio2_puts( hSio, pbuf);
		// Lot Data
			sprintf( pbuf, "Lot Data\r\n" );
			R_sio2_puts( hSio, pbuf);
			sprintf( pbuf, "  %s\r\n", SpecData );
			R_sio2_puts( hSio, pbuf);

		// Lot Start Date & Time
			sprintf( pbuf, "Lot Start\r\n" );
			R_sio2_puts( hSio, pbuf);
			sprintf( pbuf, "  date:%04d-%02d-%02d\r\n", Lot_StartDateTime.tm_year+1900, Lot_StartDateTime.tm_mon+1, Lot_StartDateTime.tm_mday );
			R_sio2_puts( hSio, pbuf);
			sprintf( pbuf, "  time:%02d:%02d:%02d\r\n", Lot_StartDateTime.tm_hour, Lot_StartDateTime.tm_min, Lot_StartDateTime.tm_sec );
			R_sio2_puts( hSio, pbuf);
		// Lot End Date & Time
			sprintf( pbuf, "Lot End\r\n" );
			R_sio2_puts( hSio, pbuf);
			sprintf( pbuf, "  date:%04d-%02d-%02d\r\n", Lot_EndDateTime.tm_year+1900, Lot_EndDateTime.tm_mon+1, Lot_EndDateTime.tm_mday );
			R_sio2_puts( hSio, pbuf);
			sprintf( pbuf, "  time:%02d:%02d:%02d\r\n", Lot_EndDateTime.tm_hour     , Lot_EndDateTime.tm_min, Lot_EndDateTime.tm_sec );
			R_sio2_puts( hSio, pbuf);
		// LF
			sprintf( pbuf, "\r\n" );
			R_sio2_puts( hSio, pbuf);

		// Print No. sort
			for( dn=0; dn<260; dn++ ) {
				sn = Setsubi[dn].print_no;
				if( sn >= 260 ) sn = 259;
				sort[sn] = dn;
			}
		// Alarm & Andon count
			alarm_sum = 0;
			andon = 0;
			for( sn=0; sn<260; sn++ ) {
				dn = sort[sn];
				if( dn == 240 )						continue;
				if( Setsubi[dn].massage[0] == 0 )	continue;
				if( Setsubi[dn].data == 0 )			continue;
				if( Setsubi[dn].count > 0 )			alarm_sum += Setsubi[dn].data;
				if( Setsubi[dn].massage[0] == '*' )	andon += Setsubi[dn].data;
			}
		// MTBF
			if( Setsubi[1].data == 0 ) {
				mtbf = 0.0;
			} else {
				if( alarm_sum <= 1 ) {
					mtbf = Setsubi[1].data;
				} else {
					mtbf = (double)( Setsubi[1].data ) / (double)( alarm_sum );
				}
			}
		// MTTR
			if( Setsubi[3].data == 0 ) {
				mttr = 0.0;
			} else {
				if( alarm_sum <= 1 ) {
					mttr = Setsubi[3].data;
				} else {
					mttr = (double)( Setsubi[3].data ) / (double)( alarm_sum );
				}
			}

		// Data print
			alarm_lf = 0;
		// Machine data print out
			for( sn=0; sn<260; sn++ ) {
				dn = sort[sn];
				if( dn == 240 )						continue;
				if( Setsubi[dn].massage[0] == 0 )	continue;
				if( Setsubi[dn].data == 0 )			continue;
				if( Setsubi[dn].count < 0 )			continue;
				if(( Setsubi[dn].count > 0 )&&( alarm_lf == 0 )) {
					alarm_lf++;
					sprintf( pbuf, "\r\n" );
					R_sio2_puts( hSio, pbuf);
				}
				sprintf( pbuf, "%-17s:%6d\r\n", Setsubi[dn].massage, Setsubi[dn].data );
				R_sio2_puts( hSio, pbuf);
				if( dn == 1 ) {
					sprintf( pbuf, "%-17s:%6.1f\r\n", " MTBF", mtbf );
					R_sio2_puts( hSio, pbuf);
					sprintf( pbuf, "%-17s:%6.1f\r\n", " MTTR", mttr );
					R_sio2_puts( hSio, pbuf);
				}
			}
				sprintf( pbuf, "%-17s:%6d\r\n", "Alarm Total", alarm_sum );
				R_sio2_puts( hSio, pbuf);
		// LF
			sprintf( pbuf, "\r\n" );
			R_sio2_puts( hSio, pbuf);

		// Counter
			for( cn=0; cn<2; cn++ ) {
				insp_sum[cn] = R_CS_CounterGetTotalCount( pCounter )+R_CS_CounterGetNgCount( pCounter, MC_RESISTANCE );
				if( cn > 0) {
					if( insp_sum[cn] == insp_sum[cn-1] )	continue;
				}
				pass = R_CS_CounterGetGoCount( pCounter );                                 		   // Count Print Out
				if(insp_sum[cn] == 0)	dp_pass = 0;                                               // Count Print Out
				else					dp_pass = pass*100.0/insp_sum[cn];                         // Count Print Out
				ng_sum = 0;
				// count data
					sprintf( pbuf, "Index Ninshiki\r\n" );
					R_sio2_puts( hSio, pbuf);
					sprintf( pbuf, "  Counter( %d )\r\n", cn+1 );
					R_sio2_puts( hSio, pbuf);
					sprintf( pbuf, "%10s:%6s(%5s)\r\n", "item", "pcs", "%" );
					R_sio2_puts( hSio, pbuf);
					if( pass > 0 ){
						sprintf( pbuf, "%10s:%6d(%5.2f)\r\n", "Pass", pass, dp_pass );
						R_sio2_puts( hSio, pbuf);
					}
//					for( dn=11; dn<=80; dn++ ) {
					for( dn=0; dn<ng_num; dn++ ) {
						if( 10 >= NgCode[dn].nCode )	continue;
						if( 80 <  NgCode[dn].nCode )	continue;
						count = R_CS_CounterGetNgCount(pCounter, NgCode[dn].nCode);
						if( count <= 0 )	continue;
                        // --- NGが発生したコードのみカウンタ表示
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
///						char *name = R_CS_CounterGetCodeName(pCounter, NgCode[dn].nCode, COUNTER_NAME_JP);
						name = R_CS_CounterGetCodeName(pCounter, NgCode[dn].nCode, COUNTER_NAME_JP);
						if(name != NULL){
							sprintf( pbuf, "%s:%6d(%5.2f)\r\n", name, count, dp );
							R_sio2_puts( hSio, pbuf);
						}
					}
				// NG total
					count = R_CS_CounterGetNgTotalCount( pCounter );
					if( insp_sum[cn] == 0 )	dp = 0.0;
					else					dp = count*100.0/insp_sum[cn];
					sprintf( pbuf, "%s:%6d(%5.2f)\r\n", "NG Total  ", count, dp );
					R_sio2_puts( hSio, pbuf);
				// LF
					sprintf( pbuf, "\r\n" );
					R_sio2_puts( hSio, pbuf);

				// Pilot Good Print   2017.06.12
					if( Pilot_mode == ON ){
						sprintf( pbuf, "<Pilot Count>\n");
						R_sio2_puts( hSio, pbuf);
						sprintf( pbuf, "%s:%6d\n", Setsubi[240].massage, Setsubi[240].data );
						R_sio2_puts( hSio, pbuf);
					}
				// MC_RESISTANCE
					dn = MC_RESISTANCE;
					count = R_CS_CounterGetNgCount(pCounter, dn);
					if( count > 0 ) {
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
						name = R_CS_CounterGetCodeName(pCounter, dn, COUNTER_NAME_JP);
						sprintf( pbuf, "%s:%6d(%5.2f)\r\n", name, count, dp );
						R_sio2_puts( hSio, pbuf);
					}
				// MC_NINSHIKI
					dn = MC_NINSHIKI;
					count = R_CS_CounterGetNgCount(pCounter, dn);
					if( count > 0 ) {
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
						name = R_CS_CounterGetCodeName(pCounter, dn, COUNTER_NAME_JP);
						sprintf( pbuf, "%s:%6d(%5.2f)\r\n", name, count, dp );
						R_sio2_puts( hSio, pbuf);
					}
				// NG_LIGHTING
					dn = NG_LIGHTING;
					count = R_CS_CounterGetNgCount(pCounter, dn);
					if( count > 0 ) {
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
						name = R_CS_CounterGetCodeName(pCounter, dn, COUNTER_NAME_JP);
						sprintf( pbuf, "%s:%6d(%5.2f)\r\n", name, count, dp );
						R_sio2_puts( hSio, pbuf);
					}
				// NG_REVERSE
					dn = NG_REVERSE;
					count = R_CS_CounterGetNgCount(pCounter, dn);
					if( count > 0 ) {
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
						name = R_CS_CounterGetCodeName(pCounter, dn, COUNTER_NAME_JP);
						sprintf( pbuf, "%s:%6d(%5.2f)\r\n", name, count, dp );
						R_sio2_puts( hSio, pbuf);
					}
			}

			//2012.03.14
			 for( nm=0; nm<20; nm++ ){
					if(insp_sum1[nm] == 0) continue;
					dp_pass = pass_sum[nm]*100.0/insp_sum1[nm];                         // Count Print Out
				// count data
					sprintf( pbuf, "Semi Lot[%d] \r\n",nm+1 );
					R_sio2_puts( hSio, pbuf);
					sprintf( pbuf, "%10s:%6s(%5s)\r\n", "item", "pcs", "%" );
					R_sio2_puts( hSio, pbuf);
					if( pass_sum[nm] > 0 ){
						sprintf( pbuf, "%10s:%6d(%5.2f)\r\n", "Pass", pass_sum[nm], dp_pass );
						R_sio2_puts( hSio, pbuf);
					}
					for( dn=0; dn<ng_num; dn++ ) {
						if( 10 >= NgCode[dn].nCode )	continue;
						if( 80 <  NgCode[dn].nCode )	continue;
						if( count1[nm][dn] <= 0 )		continue;
                        // --- NGが発生したコードのみカウンタ表示
						dp = count1[nm][dn]*100.0/insp_sum1[nm];
						name = R_CS_CounterGetCodeName(pCounter, NgCode[dn-1].nCode, COUNTER_NAME_JP);
						if(name != NULL){
							sprintf( pbuf, "%s:%6d(%5.2f)\r\n", name, count1[nm][dn], dp );
							R_sio2_puts( hSio, pbuf);
						}
					}
				// NG total
					dp = count_total[nm]*100.0/insp_sum1[nm];
					sprintf( pbuf, "%s:%6d(%5.2f)\r\n", "NG Total  ", count_total[nm], dp );
					R_sio2_puts( hSio, pbuf);
				// NG Resistance
					dp = count_R[nm]*100.0/insp_sum1[nm];
					sprintf( pbuf, "%s:%6d(%5.2f)\r\n", "M/C NG [R]", count_R[nm], dp );
					R_sio2_puts( hSio, pbuf);
				// NG Ninshiki
					dp = count_N[nm]*100.0/insp_sum1[nm];
					sprintf( pbuf, "%s:%6d(%5.2f)\r\n", "M/C NG [N]", count_N[nm], dp );
					R_sio2_puts( hSio, pbuf);
				// NG Reverse
					dp = count_Re[nm]*100.0/insp_sum1[nm];
					sprintf( pbuf, "%s:%6d(%5.2f)\r\n", "Reverse   ", count_Re[nm], dp );
					R_sio2_puts( hSio, pbuf);
				// LF
					sprintf( pbuf, "\r\n" );
					R_sio2_puts( hSio, pbuf);

			}

		// Andon check
			if( andon > 0 ) {
				sprintf( pbuf, "\r\n\r\n" );		// LF 2
				R_sio2_puts( hSio, pbuf);
				sprintf( pbuf, "%-17s:%6d\r\n", "** ANDON LOT **", andon );
				R_sio2_puts( hSio, pbuf);
			}

		// LF 3
			sprintf( pbuf, "\r\n\r\n\r\n" );
			R_sio2_puts( hSio, pbuf);

			//出力完了待ち　-> 待たずにcloseすると通信が止まる
			R_time_start( &waitTime );
			while(R_sio2_get_sndbuff( hSio ) !=0 ){
				if( 30000000 < R_time_read(&waitTime) ) break;
			}
			R_time_delay(500000);

		// file close
			R_sio2_close( hSio );

		return OK;

	}	// SetsubiDataPrint( void ) end
#endif


/************************************************
	Ninshiki Data Write
************************************************/
int		NinshikiDataWrite( void )
	{
		int						dn, sn, cn;
		int						alarm_sum, alarm_lf;
		int						andon;
		int						insp_sum[2], ng_sum;
		int						sort[260];
		int						count;
		int						pass;                                               // Count Go
		double					dp_pass;                                            // Count Go
		double					dp, mtbf, mttr;
		R_CS_COUNTER_DATA		*pCounter	= R_CS_GetCounterDataStruct(0);			// カウンタ構造体へのポインタ取得
		int						ng_num		= R_CS_CounterGetNgNum(pCounter);		// ＮＧ種別数の取得
		char 					*name;
///		int						hSio;
		char					pbuf[256];
///		RTIME					waitTime;
		int                     nm;
		FILE					*fp;

		// initial
////			for( sn=0; sn<260; sn++ ) sort[sn] = 0;

		// file open
/*
			if( (hSio=R_sio2_open( SIO_CHANNEL(5), RATE4800BPS, BIT_8, PARITY_NONE, STOP_1, FLOW_HARD )) <= 0 ) {
				strcpy( Comment, "Printer open Error !!" );
				R_chrdisp( 1, 1, Comment );
				return ERROR;
			}

		// ロット終了 Date & Time
			ctLotEnd = time(NULL);
			memcpy(&Lot_EndDateTime , localtime(&ctLotEnd) ,sizeof(Lot_EndDateTime));;
		// Type & 機台No.
			sprintf( pbuf, "%s (%drpm) %s\r\n", ProductName, Std_rpm, MachineName );
			R_sio2_puts( hSio, pbuf);
		// Lot Data
			sprintf( pbuf, "Lot Data\r\n" );
			R_sio2_puts( hSio, pbuf);
			sprintf( pbuf, "  %s\r\n", SpecData );
			R_sio2_puts( hSio, pbuf);

		// Lot Start Date & Time
			sprintf( pbuf, "Lot Start\r\n" );
			R_sio2_puts( hSio, pbuf);
			sprintf( pbuf, "  date:%04d-%02d-%02d\r\n", Lot_StartDateTime.tm_year+1900, Lot_StartDateTime.tm_mon+1, Lot_StartDateTime.tm_mday );
			R_sio2_puts( hSio, pbuf);
			sprintf( pbuf, "  time:%02d:%02d:%02d\r\n", Lot_StartDateTime.tm_hour, Lot_StartDateTime.tm_min, Lot_StartDateTime.tm_sec );
			R_sio2_puts( hSio, pbuf);
		// Lot End Date & Time
			sprintf( pbuf, "Lot End\r\n" );
			R_sio2_puts( hSio, pbuf);
			sprintf( pbuf, "  date:%04d-%02d-%02d\r\n", Lot_EndDateTime.tm_year+1900, Lot_EndDateTime.tm_mon+1, Lot_EndDateTime.tm_mday );
			R_sio2_puts( hSio, pbuf);
			sprintf( pbuf, "  time:%02d:%02d:%02d\r\n", Lot_EndDateTime.tm_hour     , Lot_EndDateTime.tm_min, Lot_EndDateTime.tm_sec );
			R_sio2_puts( hSio, pbuf);
		// LF
			sprintf( pbuf, "\r\n" );
			R_sio2_puts( hSio, pbuf);

		// Print No. sort
			for( dn=0; dn<260; dn++ ) {
				sn = Setsubi[dn].print_no;
				if( sn >= 260 ) sn = 259;
				sort[sn] = dn;
			}
		// Alarm & Andon count
			alarm_sum = 0;
			andon = 0;
			for( sn=0; sn<260; sn++ ) {
				dn = sort[sn];
				if( dn == 240 )						continue;
				if( Setsubi[dn].massage[0] == 0 )	continue;
				if( Setsubi[dn].data == 0 )			continue;
				if( Setsubi[dn].count > 0 )			alarm_sum += Setsubi[dn].data;
				if( Setsubi[dn].massage[0] == '*' )	andon += Setsubi[dn].data;
			}
		// MTBF
			if( Setsubi[1].data == 0 ) {
				mtbf = 0.0;
			} else {
				if( alarm_sum <= 1 ) {
					mtbf = Setsubi[1].data;
				} else {
					mtbf = (double)( Setsubi[1].data ) / (double)( alarm_sum );
				}
			}
		// MTTR
			if( Setsubi[3].data == 0 ) {
				mttr = 0.0;
			} else {
				if( alarm_sum <= 1 ) {
					mttr = Setsubi[3].data;
				} else {
					mttr = (double)( Setsubi[3].data ) / (double)( alarm_sum );
				}
			}

		// Data print
			alarm_lf = 0;
		// Machine data print out
			for( sn=0; sn<260; sn++ ) {
				dn = sort[sn];
				if( dn == 240 )						continue;
				if( Setsubi[dn].massage[0] == 0 )	continue;
				if( Setsubi[dn].data == 0 )			continue;
				if( Setsubi[dn].count < 0 )			continue;
				if(( Setsubi[dn].count > 0 )&&( alarm_lf == 0 )) {
					alarm_lf++;
					sprintf( pbuf, "\r\n" );
					R_sio2_puts( hSio, pbuf);
				}
				sprintf( pbuf, "%-17s:%6d\r\n", Setsubi[dn].massage, Setsubi[dn].data );
				R_sio2_puts( hSio, pbuf);
				if( dn == 1 ) {
					sprintf( pbuf, "%-17s:%6.1f\r\n", " MTBF", mtbf );
					R_sio2_puts( hSio, pbuf);
					sprintf( pbuf, "%-17s:%6.1f\r\n", " MTTR", mttr );
					R_sio2_puts( hSio, pbuf);
				}
			}
				sprintf( pbuf, "%-17s:%6d\r\n", "Alarm Total", alarm_sum );
				R_sio2_puts( hSio, pbuf);
		// LF
			sprintf( pbuf, "\r\n" );
			R_sio2_puts( hSio, pbuf);
*/
		if((Option & BACKINSP_MODE) ==0)			fp=fopen("surface.txt","w");
		else										fp=fopen("back.txt","w");
		if(fp == NULL)	return ERROR;
		// Counter
			for( cn=0; cn<2; cn++ ) {
				insp_sum[cn] = R_CS_CounterGetTotalCount( pCounter )+R_CS_CounterGetNgCount( pCounter, MC_RESISTANCE );
				if( cn > 0) {
					if( insp_sum[cn] == insp_sum[cn-1] )	continue;
				}
				pass = R_CS_CounterGetGoCount( pCounter );                                 		   // Count Print Out
				if(insp_sum[cn] == 0)	dp_pass = 0;                                               // Count Print Out
				else					dp_pass = pass*100.0/insp_sum[cn];                         // Count Print Out
				ng_sum = 0;
				// count data
					if((Option & BACKINSP_MODE) ==0)			sprintf( pbuf, "Index Ninshiki\r\n" );
					else										sprintf( pbuf, "Back Ninshiki\r\n" );
					fprintf( fp, "%s", pbuf);
					sprintf( pbuf, "  Counter( %d )\r\n", cn+1 );
					fprintf( fp, "%s", pbuf);
					sprintf( pbuf, "%10s:%6s(%5s)\r\n", "item", "pcs", "%" );
					fprintf( fp, "%s", pbuf);
					if( pass > 0 ){
						sprintf( pbuf, "%10s:%6d(%5.2f)\r\n", "Pass", pass, dp_pass );
						fprintf( fp, "%s", pbuf);
					}
//					for( dn=11; dn<=80; dn++ ) {
					for( dn=0; dn<ng_num; dn++ ) {
						if( 10 >= NgCode[dn].nCode )	continue;
						if( 80 <  NgCode[dn].nCode )	continue;
						count = R_CS_CounterGetNgCount(pCounter, NgCode[dn].nCode);
						if( count <= 0 )	continue;
						// --- NGが発生したコードのみカウンタ表示
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
///						char *name = R_CS_CounterGetCodeName(pCounter, NgCode[dn].nCode, COUNTER_NAME_JP);
						name = R_CS_CounterGetCodeName(pCounter, NgCode[dn].nCode, COUNTER_NAME_JP);
						if(name != NULL){
							sprintf( pbuf, "%s:%6d(%5.2f)\r\n", name, count, dp );
							fprintf( fp, "%s", pbuf);
						}
					}
				// NG total
					count = R_CS_CounterGetNgTotalCount( pCounter );
					if( insp_sum[cn] == 0 )	dp = 0.0;
					else					dp = count*100.0/insp_sum[cn];
					sprintf( pbuf, "%s:%6d(%5.2f)\r\n", "NG Total  ", count, dp );
					fprintf( fp, "%s", pbuf);
				// LF
					sprintf( pbuf, "\r\n" );
					fprintf( fp, "%s", pbuf);

				// Pilot Good Print   2017.06.12
		//			if( Pilot_mode == ON ){
		//				sprintf( pbuf, "<Pilot Count>\n");
		//				fprintf( fp, "%s", pbuf);
		//				sprintf( pbuf, "%s:%6d\n", Setsubi[240].massage, Setsubi[240].data );
		//				fprintf( fp, "%s", pbuf);
		//			}
/*
				// MC_RESISTANCE
					dn = MC_RESISTANCE;
					count = R_CS_CounterGetNgCount(pCounter, dn);
					if( count > 0 ) {
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
						name = R_CS_CounterGetCodeName(pCounter, dn, COUNTER_NAME_JP);
						sprintf( pbuf, "%s:%6d(%5.2f)\r\n", name, count, dp );
						R_sio2_puts( hSio, pbuf);
					}
				// MC_NINSHIKI
					dn = MC_NINSHIKI;
					count = R_CS_CounterGetNgCount(pCounter, dn);
					if( count > 0 ) {
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
						name = R_CS_CounterGetCodeName(pCounter, dn, COUNTER_NAME_JP);
						sprintf( pbuf, "%s:%6d(%5.2f)\r\n", name, count, dp );
						R_sio2_puts( hSio, pbuf);
					}
*/
				// NG_LIGHTING
					dn = NG_LIGHTING;
					count = R_CS_CounterGetNgCount(pCounter, dn);
					if( count > 0 ) {
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
						name = R_CS_CounterGetCodeName(pCounter, dn, COUNTER_NAME_JP);
						sprintf( pbuf, "%s:%6d(%5.2f)\r\n", name, count, dp );
						fprintf( fp, "%s", pbuf);
					}
				// NG_REVERSE
					dn = NG_REVERSE;
					count = R_CS_CounterGetNgCount(pCounter, dn);
					if( count > 0 ) {
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
						name = R_CS_CounterGetCodeName(pCounter, dn, COUNTER_NAME_JP);
						sprintf( pbuf, "%s:%6d(%5.2f)\r\n", name, count, dp );
						fprintf( fp, "%s", pbuf);
					}
			}

			fclose(fp);
/*
			//2012.03.14
			 for( nm=0; nm<20; nm++ ){
					if(insp_sum1[nm] == 0) continue;
					dp_pass = pass_sum[nm]*100.0/insp_sum1[nm];                         // Count Print Out
				// count data
					sprintf( pbuf, "Semi Lot[%d] \r\n",nm+1 );
					R_sio2_puts( hSio, pbuf);
					sprintf( pbuf, "%10s:%6s(%5s)\r\n", "item", "pcs", "%" );
					R_sio2_puts( hSio, pbuf);
					if( pass_sum[nm] > 0 ){
						sprintf( pbuf, "%10s:%6d(%5.2f)\r\n", "Pass", pass_sum[nm], dp_pass );
						R_sio2_puts( hSio, pbuf);
					}
					for( dn=0; dn<ng_num; dn++ ) {
						if( 10 >= NgCode[dn].nCode )	continue;
						if( 80 <  NgCode[dn].nCode )	continue;
						if( count1[nm][dn] <= 0 )		continue;
						// --- NGが発生したコードのみカウンタ表示
						dp = count1[nm][dn]*100.0/insp_sum1[nm];
						name = R_CS_CounterGetCodeName(pCounter, NgCode[dn-1].nCode, COUNTER_NAME_JP);
						if(name != NULL){
							sprintf( pbuf, "%s:%6d(%5.2f)\r\n", name, count1[nm][dn], dp );
							R_sio2_puts( hSio, pbuf);
						}
					}
				// NG total
					dp = count_total[nm]*100.0/insp_sum1[nm];
					sprintf( pbuf, "%s:%6d(%5.2f)\r\n", "NG Total  ", count_total[nm], dp );
					R_sio2_puts( hSio, pbuf);
				// NG Resistance
					dp = count_R[nm]*100.0/insp_sum1[nm];
					sprintf( pbuf, "%s:%6d(%5.2f)\r\n", "M/C NG [R]", count_R[nm], dp );
					R_sio2_puts( hSio, pbuf);
				// NG Ninshiki
					dp = count_N[nm]*100.0/insp_sum1[nm];
					sprintf( pbuf, "%s:%6d(%5.2f)\r\n", "M/C NG [N]", count_N[nm], dp );
					R_sio2_puts( hSio, pbuf);
				// NG Reverse
					dp = count_Re[nm]*100.0/insp_sum1[nm];
					sprintf( pbuf, "%s:%6d(%5.2f)\r\n", "Reverse   ", count_Re[nm], dp );
					R_sio2_puts( hSio, pbuf);
				// LF
					sprintf( pbuf, "\r\n" );
					R_sio2_puts( hSio, pbuf);

			}

		// Andon check
			if( andon > 0 ) {
				sprintf( pbuf, "\r\n\r\n" );		// LF 2
				R_sio2_puts( hSio, pbuf);
				sprintf( pbuf, "%-17s:%6d\r\n", "** ANDON LOT **", andon );
				R_sio2_puts( hSio, pbuf);
			}

		// LF 3
			sprintf( pbuf, "\r\n\r\n\r\n" );
			R_sio2_puts( hSio, pbuf);

			//出力完了待ち　-> 待たずにcloseすると通信が止まる
			R_time_start( &waitTime );
			while(R_sio2_get_sndbuff( hSio ) !=0 ){
				if( 30000000 < R_time_read(&waitTime) ) break;
			}
			R_time_delay(500000);

		// file close
			R_sio2_close( hSio );
*/
		return OK;

	}	// NinshikiDataWrite( void ) end



/**********************************************************************************
	Type date and time write
**********************************************************************************/

int		SetsubiDataWrite( void )
	{
		int						dn, sn, cn;
		int						alarm_sum, alarm_lf;
		int						andon;
		int						insp_sum[2], ng_sum;
		int						sort[260];
		int						count;
		int						pass;                                               // Count Go
		double					dp_pass;                                            // Count Go
		double					dp, mtbf, mttr;
		char					fname[20];
		FILE					*fin;
		R_CS_COUNTER_DATA		*pCounter	= R_CS_GetCounterDataStruct(0);			// カウンタ構造体へのポインタ取得
		int						ng_num		= R_CS_CounterGetNgNum(pCounter);		// ＮＧ種別数の取得
		char 					*name;

		// initial
			for( sn=0; sn<260; sn++ ) sort[sn] = 0;

		// file name
#ifdef __DOS__
			sprintf( fname, "%04d%02d%02d.csv", Lot_StartDate.year, Lot_StartDate.month, Lot_StartDate.day );
#elif __GNUC__
			sprintf( fname, "%04d%02d%02d.csv", Lot_StartDateTime.tm_year+1900, Lot_StartDateTime.tm_mon+1, Lot_StartDateTime.tm_mday );
#endif
		// file open
			if((fin = fopen( fname, "a" )) == NULL){
				fclose( fin );
				return ERROR;
			}

		// ロット終了 Date & Time
#ifdef __DOS__
			_dos_getdate(&Lot_EndDate);		// 現在の日付を取得
			_dos_gettime(&Lot_EndTime);		// 現在の時間を取得
#elif __GNUC__
			ctLotEnd = time(NULL);
			memcpy(&Lot_EndDateTime , localtime(&ctLotEnd) ,sizeof(Lot_EndDateTime));
#endif


		// Type & 機台No.
			fprintf( fin, "%s (%drpm) %s\n", ProductName, Std_rpm, MachineName );
		// Lot Data
			fprintf( fin, "Lot Data\n" );
			fprintf( fin, "  %s\n", SpecData );

		// Lot Start Date & Time
			fprintf( fin, "Lot Start\n" );
#ifdef __DOS__
			fprintf( fin, "  date:%04d-%02d-%02d\n", Lot_StartDate.year, Lot_StartDate.month, Lot_StartDate.day );
			fprintf( fin, "  time:%02d:%02d:%02d\n", Lot_StartTime.hour, Lot_StartTime.minute, Lot_StartTime.second );
#elif __GNUC__
			fprintf( fin, "  date:%04d-%02d-%02d\n", Lot_StartDateTime.tm_year+1900, Lot_StartDateTime.tm_mon+1, Lot_StartDateTime.tm_mday );
			fprintf( fin, "  time:%02d:%02d:%02d\n", Lot_StartDateTime.tm_hour     , Lot_StartDateTime.tm_min, Lot_StartDateTime.tm_sec );
#endif
		// Lot End Date & Time
			fprintf( fin, "Lot End\n" );
#ifdef __DOS__
			fprintf( fin, "  date:%04d-%02d-%02d\n", Lot_EndDate.year, Lot_EndDate.month, Lot_EndDate.day );
			fprintf( fin, "  time:%02d:%02d:%02d\n", Lot_EndTime.hour, Lot_EndTime.minute, Lot_EndTime.second );
#elif __GNUC__
			fprintf( fin, "  date:%04d-%02d-%02d\n", Lot_EndDateTime.tm_year+1900, Lot_EndDateTime.tm_mon+1, Lot_EndDateTime.tm_mday );
			fprintf( fin, "  time:%02d:%02d:%02d\n", Lot_EndDateTime.tm_hour     , Lot_EndDateTime.tm_min, Lot_EndDateTime.tm_sec );
#endif
		// LF
			fprintf( fin, "\n" );

		// Data print
			alarm_sum = 0;
			andon = 0;
		// Print No. sort
			for( dn=0; dn<260; dn++ ) {
				sn = Setsubi[dn].print_no;
				if( sn >= 260 ) sn = 259;
				sort[sn] = dn;
			}
		// Alarm & Andon count
			alarm_sum = 0;
			andon = 0;
			for( sn=0; sn<260; sn++ ) {
				dn = sort[sn];
				if( Setsubi[dn].massage[0] == 0 )	continue;
				if( Setsubi[dn].data == 0 )			continue;
				if( Setsubi[dn].count > 0 )			alarm_sum += Setsubi[dn].data;
				if( Setsubi[dn].massage[0] == '*' )	andon += Setsubi[dn].data;
			}
		// MTBF
			if( Setsubi[1].data == 0 ) {
				mtbf = 0.0;
			} else {
				if( alarm_sum <= 1 ) {
					mtbf = Setsubi[1].data;
				} else {
					mtbf = (double)( Setsubi[1].data ) / (double)( alarm_sum );
				}
			}
		// MTTR
			if( Setsubi[3].data == 0 ) {
				mttr = 0.0;
			} else {
				if( alarm_sum <= 1 ) {
					mttr = Setsubi[3].data;
				} else {
					mttr = (double)( Setsubi[3].data ) / (double)( alarm_sum );
				}
			}

		// Data write
			alarm_lf = 0;
		// Machine data print out
			for( sn=0; sn<260; sn++ ) {
				dn = sort[sn];
				if( Setsubi[dn].massage[0] == 0 )	continue;
//				if( Setsubi[dn].data == 0 )			continue;
				if(( Setsubi[dn].count > 0 )&&( alarm_lf == 0 )) {
					alarm_lf++;
					fprintf( fin, "\n" );
				}
				fprintf( fin, "%-17s:%6d\n", Setsubi[dn].massage, Setsubi[dn].data );
				if( dn == 1 ) {
					fprintf( fin, "%-17s,%6.1f\n", " MTBF", mtbf );
					fprintf( fin, "%-17s,%6.1f\n", " MTTR", mttr );
				}
			}
				fprintf( fin, "%-17s,%6d\n", "Alarm Total", alarm_sum );
		// LF
			fprintf( fin, "\n" );

		// Counter
			for( cn=0; cn<2; cn++ ) {
				insp_sum[cn] = R_CS_CounterGetTotalCount( pCounter )+R_CS_CounterGetNgCount( pCounter, MC_RESISTANCE );
				if( cn > 0) {
					if( insp_sum[cn] == insp_sum[cn-1] )	continue;
				}
				pass = R_CS_CounterGetGoCount( pCounter );                                 		 // Count Print Out
				if(insp_sum[cn] == 0)	dp_pass = 0;                                               // Count Print Out
				else					dp_pass = pass*100.0/insp_sum[cn];                         // Count Print Out
				ng_sum = 0;
				// count data
					fprintf( fin, "Index Ninshiki\n" );
					fprintf( fin, "  Counter( %d )\n", cn+1 );
					fprintf( fin, "%10s,%6s,%5s\n", "item", "pcs", "%" );
					if( pass > 0 ) fprintf( fin, "%10s:%6d(%5.2f)\n", "Pass", pass, dp_pass );
///					for( dn=11; dn<=80; dn++ ) {
					for( dn=0; dn<ng_num; dn++ ) {
						if( 10 >= NgCode[dn].nCode )	continue;
						if( 80 <  NgCode[dn].nCode )	continue;
						count = R_CS_CounterGetNgCount(pCounter, NgCode[dn].nCode);
						if( count <= 0 )	continue;
                        // --- NGが発生したコードのみカウンタ表示
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
///						char *name = R_CS_CounterGetCodeName(pCounter, NgCode[dn].nCode, COUNTER_NAME_JP);
						name = R_CS_CounterGetCodeName(pCounter, NgCode[dn].nCode, COUNTER_NAME_JP);
						if(name != NULL){
							fprintf( fin, "%s,%6d,%5.2f\n", name, count, dp );
						}
					}
				// NG total
					count = R_CS_CounterGetNgTotalCount( pCounter );
					if( insp_sum[cn] == 0 )	dp = 0.0;
					else					dp = count*100.0/insp_sum[cn];
					fprintf( fin, "%s:%6d,%5.2f\n", "NG Total  ", count, dp );
				// LF
					fprintf( fin, "\n" );

				// Pilot Good Print   2017.06.12
					if( Pilot_mode == ON ){
						fprintf( fin, "<Pilot Count>\n");
						fprintf( fin, "%s:%6d\n", Setsubi[240].massage, Setsubi[240].data );
					}
				// MC_RESISTANCE
					dn = MC_RESISTANCE;
					count = R_CS_CounterGetNgCount(pCounter, dn);
					if( count > 0 ) {
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
						name = R_CS_CounterGetCodeName(pCounter, dn, COUNTER_NAME_JP);
						fprintf( fin, "%s:%6d(%5.2f)\r\n", name, count, dp );
					}
				// MC_NINSHIKI
					dn = MC_NINSHIKI;
					count = R_CS_CounterGetNgCount(pCounter, dn);
					if( count > 0 ) {
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
						name = R_CS_CounterGetCodeName(pCounter, dn, COUNTER_NAME_JP);
						fprintf( fin, "%s:%6d(%5.2f)\r\n", name, count, dp );
					}
				// NG_LIGHTING
					dn = NG_LIGHTING;
					count = R_CS_CounterGetNgCount(pCounter, dn);
					if( count > 0 ) {
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
						name = R_CS_CounterGetCodeName(pCounter, dn, COUNTER_NAME_JP);
						fprintf( fin, "%s:%6d(%5.2f)\r\n", name, count, dp );
					}
				// NG_REVERSE
					dn = NG_REVERSE;
					count = R_CS_CounterGetNgCount(pCounter, dn);
					if( count > 0 ) {
						if( insp_sum[cn] == 0 )	dp = 0.0;
						else					dp = count*100.0/insp_sum[cn];
						name = R_CS_CounterGetCodeName(pCounter, dn, COUNTER_NAME_JP);
						fprintf( fin, "%s,%6d,%5.2f\n", name, count, dp );
					}
			}

		// Andon check
			if( andon > 0 ) {
				fprintf( fin, "\n\n" );		// LF 2
				fprintf( fin, "%-17s,%6d\n", "** ANDON LOT **", andon );
			} else {
				fprintf( fin, "\n\n\n" );	// LF 3
			}

		// LF 3
			fprintf( fin, "\n\n\n" );

		// file close
			fclose( fin );

		return OK;

	}	// SetsubiDataWrite( void ) end


/************************************************
	Setsubi Data Read Busy
************************************************/

void	SetsubiDataReadBusy( int mode )
	{
		if(((Option & NO_INCY119)!=0)&&((Option & NO_INCY76)==0)){				//<shio>
			if( mode == ON )	R_gpio_on(  Incy76Ch|CNa_OUT_port_1, 0x01 );
			else				R_gpio_off( Incy76Ch|CNa_OUT_port_1, 0x01 );
		}else{
			if( mode == ON )	R_gpio_on(  Incy119Ch|CNa_OUT_port_1, 0x01 ); //<z1>
			else				R_gpio_off( Incy119Ch|CNa_OUT_port_1, 0x01 ); //<z1>
		}
	}	// SetsubiDataReadBusy( int mode ) end


/************************************************
	Setsubi Data Print Busy
************************************************/

void	SetsubiDataPrintBusy( int mode )
	{
		if(((Option & NO_INCY119)!=0)&&((Option & NO_INCY76)==0)){				//<shio>
			if( mode == ON )	R_gpio_on(  Incy76Ch|CNa_OUT_port_1, 0x02 );
			else				R_gpio_off( Incy76Ch|CNa_OUT_port_1, 0x02 );
		}else{
			if( mode == ON )	R_gpio_on(  Incy119Ch|CNa_OUT_port_1, 0x02 ); //<z1>
			else				R_gpio_off( Incy119Ch|CNa_OUT_port_1, 0x02 ); //<z1>
		}
	}	// SetsubiDataPrintBusy( int mode ) end


//------------------------------------------------
//	Set Counter
//------------------------------------------------

void	SetCounter( void )
	{
		// NGカウンタ設定
			R_CS_SetCounterNgCode(NgCode);
			R_CS_SetCounterNgClass(NgClass);
	}


//------------------------------------------------
//	Set Counter Tape
//------------------------------------------------

void	SetCounterTape( void )
	{
		// NGカウンタ設定
			R_CS_SetCounterNgCode(NgCodeTape);
			R_CS_SetCounterNgClass(NgClassTape);
	}


//------------------------------------------------
//	start sig.	(0x01)
//------------------------------------------------

int		CheckInspStart( void )
	{
		int						port;

		port = R_gpio_read(Drcy80Ch|CNa_IN_port_1);
		return( port & INSP_START );

	}	// CheckInspStart( void ) end

//------------------------------------------------
//	Cancle Poket sig.	(0x02)  //2012.04.19
//------------------------------------------------

int		CanclePok_sig( void )
	{
		int						port;

		port = R_gpio_read(Drcy80Ch|CNa_IN_port_1);
		return( port & POK_SIG );

	}	// CheckInspStart( void ) end


//------------------------------------------------
//	抵抗値％外 count sig.	(0x01)
//------------------------------------------------

int		CheckResistanceNG( void )
	{
		int						port;

		port = R_gpio_read(Drcy80Ch|CNa_IN_port_2);
		return( port & RESISTANCE_NG );

	}	// CheckRetryStart( void ) end


//------------------------------------------------
//	外観検査NG count sig.	(0x02)
//------------------------------------------------

int		CheckNinshikiNG( void )
	{
		int						port;

		port = R_gpio_read(Drcy80Ch|CNa_IN_port_2);
		return( port & NINSHIKI_NG );

	}	// CheckRetryStart( void ) end


//------------------------------------------------
//	Pilot mode sig.			(0x04)
//------------------------------------------------

int		CheckPilotMode( void )
	{
		int						port;

		port = R_gpio_read(Drcy80Ch|CNa_IN_port_2);
		return( port & PILOT_MODE );

	}	// CheckRetryStart( void ) end


//------------------------------------------------
//	Check Judge read sig.	(0x08)
//------------------------------------------------

int		CheckJudgeRead( void )
	{
		int						port;

		port = R_gpio_read(Drcy80Ch|CNa_IN_port_2);
		return( port & JUDGE_READ );

	}	// CheckRetryStart( void ) end


//------------------------------------------------
//	Check Spec set sig.	(0x10)
//------------------------------------------------

int		CheckSpecSet( void )
	{
		int						port;

		port = R_gpio_read(Drcy80Ch|CNa_IN_port_2);
		return( port & SPEC_SET );

	}	// CheckRetryStart( void ) end


//------------------------------------------------
//	Check Pilot sig.	(0x04)
//------------------------------------------------
/*
int		CheckPilotset( void )
	{
		int						port;

		port = R_gpio_read(Drcy80Ch|CNa_IN_port_2);
		return( port & PILOT_MODE );

	}	// CheckRetryStart( void ) end

*/

void twa_TimeStart( TWA_TIME_RESULT *pPara ){					 R_time_start( &pPara->tTime);				}
void twa_TimeRead( TWA_TIME_RESULT *pPara, int nNumber ){		pPara->nTime[nNumber] = R_time_read ( &pPara->tTime);		}

/**********************************************************************************
	Login date and time write		//<z1> START
**********************************************************************************/
int		Login_data_write( int mode, int SChange )
	{
		int						mn, fn;
		static char		fname[] = "key_log.csv";
		char buff[100];
		FILE *fin;
		time_t nTime;

		//ファイルオープン
			if((fin = fopen( fname, "a" )) == NULL){
//				strcpy( Comment, "File open Error !!" );
//				R_chrdisp( 1, 1, Comment );
				fclose( fin );

				return(ERROR);
			}

				nTime = time(NULL);
				strftime(buff, 100, "Date: %Y:%m:%d Time: %H:%M:%S ", localtime(&nTime));
				fprintf( fin, buff);


			switch( mode ) {
				case 0:		//PM
					fprintf( fin, ": PM (BM)");
					break;
				case 1:		//PM
					fprintf( fin, ": PM (EPM)");
					break;
				case 2:		//PM Manager
					fprintf( fin, ": PM In-Charge");
					break;
				case 3:		//Instru
					fprintf( fin, ": Instru(BM)");
					break;
				case 4:		//Instru
					fprintf( fin, ": Instru(Improvement)");
					break;
				case 5:		//PM Back up
					fprintf( fin, ": PM(BACK UP)");
					break;
				default:	//ERROR
					fprintf( fin, " : ERROR\n" );
					break;
			}
			switch( SChange ) {
				case 0:
					fprintf( fin, " : Inspection Parameter\n");
					break;
				case 1:
					fprintf( fin, " : Presence Chip Check\n");
					break;
				case 2:
					fprintf( fin, " : Chip Stand Check\n");
					break;
				case 3:
					fprintf( fin, " : Chip Positioning Check\n");
					break;
				case 4:
					fprintf( fin, " : Presence Elec Check\n");
					break;
				case 5:
					fprintf( fin, " : Binary Coarse Positioning\n");
					break;
				case 6:
					fprintf( fin, " : Chipping Detection\n");
					break;
				case 7:
					fprintf( fin, " : Elec Peel Detection\n");
					break;
				case 8:
					fprintf( fin, " : Elec Defect Detection\n");
					break;
				case 9:
					fprintf( fin, " : Elec Defect Detection 2\n");
					break;
				case 10:
					fprintf( fin, " : Elec Defect Detection 3\n");
					break;
				case 11:
					fprintf( fin, " : Elec Defect Detection 4\n");
					break;
				case 12:
					fprintf( fin, " : Dust Black Detection\n");
					break;
				case 13:
					fprintf( fin, " : Dust Ceramic Detection\n");
					break;
				case 14:
					fprintf( fin, " : Between Elec Detection\n");
					break;
				case 15:
					fprintf( fin, " : System Setting\n");
					break;
				case 16:
					fprintf( fin, " : Light Setting\n");
					break;
				default:	//ERROR
					fprintf( fin, " : ERROR\n" );
					break;
			}
//<s2>			fprintf( fin, "  USB Serial: %s  Vendor : %s\n", UsbKeySerial, UsbKeyVendor);
			fclose( fin );

			return(OK);
}

//<--- <z2>
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

**********************************************************/
#define USBKEY_MAX 10

int UsbKeyCertification( char *UsbKeyInformation, int SChange )
{
	int		i;
	FILE	*fpListVendor;
	FILE	*fpListSerial;
	FILE	*fpTmpVendor;
	FILE	*fpTmpSerial;
	char	ListSerial[USBKEY_MAX][128];
	char	ListVendor[USBKEY_MAX][128];
//	char	ComVendor[] = "cat /proc/bus/usb/devices | grep Vendor > usbvendor.txt";
//	char	ComSerial[] = "cat /proc/bus/usb/devices | grep Serial > usbserial.txt";
	char	ComVendor[] = "cat /sys/kernel/debug/usb/devices | grep Vendor > usbvendor.txt";
	char	ComSerial[] = "cat /sys/kernel/debug/usb/devices | grep Serial > usbserial.txt";

	int		list_no;
	int		list_count;
	char	buffer[128];
	int		FlagSerial = 0;
	int		FlagVendor = 0;
	char	InfoSerial[128];
	char	InfoVendor[128];
	int 	UsbKeySerialID; //<z2>

//	char	UsbKeyInformation[1024];
	int		UsbKeySerial;
	int 	UsbKeyVendor;

	//USB Serial Numberの認証
	if( NULL == (fpListSerial=fopen("usbserial.dat","rt")) ){
		R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (can not open usbserial.dat)" );
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
			return ERROR;
		}
		else{
			list_no = 0;
			while(NULL != fgets(buffer, 128, fpTmpSerial )){
				buffer[strlen(buffer)-1] = '\0';			printf("buffer[%s]\n",buffer);				//行末の改行コードの削除
				for( i=0; i<list_count; i++){
					if( 0==strcmp( buffer, ListSerial[i] ) ){ //success certification of usb key. //list_no //<z2>
						//R_draw_popup_message( DRAW_GREEN, DISP_4, POPUP_WAIT_CLICK, "-- CERTIFICATION PASS --\nUSB KEY[%s]\nLIST   [%s]",buffer,ListSerial[list_no] );
						UsbKeySerial = atoi(ListSerial[i]); //<z2>
						UsbKeySerialID = i; //<z2>
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
			return ERROR;
		}
	}
SERIAL_SUCCESS:

	//USB VendorID, ProductIDの認証
	if( NULL == (fpListVendor=fopen("usbvendor.dat","rt")) ){
		R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (can not open usbvendor.dat)" );
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
				return ERROR;
			}
			if( 1>=strlen(buffer) ){
				//登録Listがゼロの場合はエラー
				if( 0 == list_no ){
					R_draw_popup_message( DRAW_RED, DISP_4, POPUP_WAIT_CLICK, "-- ERROR!! --\nCan not read USB key information!!\n (no list of usbvendor.dat)" );
					fclose( fpListVendor );
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
						sscanf(ListVendor[i],"%x",&UsbKeyVendor ) ; //<z2>
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
			return ERROR;
		}
	}
VENDOR_SUCCESS:

/*	R_draw_popup_message( DRAW_GREEN, DISP_4, POPUP_WAIT_CLICK, "-- CERTIFICATION PASS --\n"
																"Vendor[%s]\n"
																"Serial[%s]\n"
																,InfoVendor,InfoSerial );
*/
	Login_data_write(UsbKeySerialID, SChange); //<z2>
	sprintf( UsbKeyInformation," -- CERTIFICATION PASS --\n");
/*<---<z2>
	strcat ( UsbKeyInformation, InfoVendor );
	strcat ( UsbKeyInformation, "\n" );
	strcat ( UsbKeyInformation, InfoSerial );
	R_draw_popup_message( DRAW_GREEN, DISP_4, POPUP_WAIT_CLICK, UsbKeyInformation );
	strcat ( UsbKeyInformation, "\n" );
*--->*/
	R_draw_popup_message( DRAW_GREEN, DISP_4, POPUP_WAIT_CLICK, "-- CERTIFICATION PASS --" ); //<z2>

	return OK;
}
//<z2> --->

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
int UsbKeyCertification_rist( USB_KEY_INFO *UsbKeyInfo )

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

	char    ComMountb[]    = "mount -t vfat /dev/sdb /home/ipg/user/back/usb";
	char    ComMount1b[]   = "mount -t vfat /dev/sdb1 /home/ipg/user/back/usb";
	char    ComMount2b[]   = "mount -t vfat /dev/sdb2 /home/ipg/user/back/usb";
	char    ComMount3b[]   = "mount -t vfat /dev/sdb3 /home/ipg/user/back/usb";
	char    ComMount4b[]   = "mount -t vfat /dev/sdb4 /home/ipg/user/back/usb";
	char    ComMount5b[]   = "mount -t vfat /dev/sdb5 /home/ipg/user/back/usb";
	char    ComMount6b[]   = "mount -t vfat /dev/sdc  /home/ipg/user/back/usb";
	char    ComMount7b[]   = "mount -t vfat /dev/sdc1 /home/ipg/user/back/usb";


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

	if(!(Option & BACKINSP_MODE)){
		system( ComMount );
		system( ComMount1 );
		system( ComMount2 );
		system( ComMount3 );
		system( ComMount4 );
		system( ComMount5 );
		system( ComMount6 );
		system( ComMount7 );
	} else {
		system( ComMountb );
		system( ComMount1b );
		system( ComMount2b );
		system( ComMount3b );
		system( ComMount4b );
		system( ComMount5b );
		system( ComMount6b );
		system( ComMount7b );
	}
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




/************************************************
	縮退フィルター
************************************************/

void	TWA_contrct2( int mem_no, int x_size, int y_size, int pt[] )
	{
		int						pt_r;

		// area check
			if( pt[0] > pt[2] ) {
				pt_r = pt[0];
				pt[0] = pt[2];
				pt[2] = pt_r;
			}
			if( pt[1] > pt[3] ) {
				pt_r = pt[1];
				pt[1] = pt[3];
				pt[3] = pt_r;
			}
			if( (pt[0]<0) || (pt[2]>(FX_SIZE-1)) ) x_size = y_size = 0;
			if( (pt[1]<0) || (pt[3]>(FY_SIZE-1)) ) x_size = y_size = 0;
			if( pt[0] != 0 ) {
				if( pt[0]%32 == 0 ) pt[0]-=1;
			}

		switch( y_size )
			{
			case 2:
				R_contrct_window( FILTER_12, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 3:
				R_contrct_window( FILTER_13, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 4:
				R_contrct_window( FILTER_12, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_contrct_window( FILTER_13, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 5:
				R_contrct_window( FILTER_15, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 6:
				R_contrct_window( FILTER_12, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_contrct_window( FILTER_15, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 7:
				R_contrct_window( FILTER_13, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_contrct_window( FILTER_15, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 8:
				R_contrct_window( FILTER_12, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_contrct_window( FILTER_13, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_contrct_window( FILTER_15, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 9:
				R_contrct_window( FILTER_15, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_contrct_window( FILTER_15, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			}

		switch( x_size )
			{
			case 2:
				R_contrct_window( FILTER_21, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 3:
				R_contrct_window( FILTER_31, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 4:
				R_contrct_window( FILTER_21, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_contrct_window( FILTER_31, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 5:
				R_contrct_window( FILTER_51, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 6:
				R_contrct_window( FILTER_21, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_contrct_window( FILTER_51, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 7:
				R_contrct_window( FILTER_31, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_contrct_window( FILTER_51, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 8:
				R_contrct_window( FILTER_21, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_contrct_window( FILTER_31, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_contrct_window( FILTER_51, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 9:
				R_contrct_window( FILTER_51, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_contrct_window( FILTER_51, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			}
	}


/************************************************
	膨張フィルター
************************************************/

void	TWA_expand2( int mem_no, int x_size, int y_size, int pt[] )
	{
		int						pt_r;

		// area check
			if( pt[0] > pt[2] ) {
				pt_r = pt[0];
				pt[0] = pt[2];
				pt[2] = pt_r;
			}
			if( pt[1] > pt[3] ) {
				pt_r = pt[1];
				pt[1] = pt[3];
				pt[3] = pt_r;
			}
			if( (pt[0]<0) || (pt[2]>(FX_SIZE-1)) ) x_size = y_size = 0;
			if( (pt[1]<0) || (pt[3]>(FY_SIZE-1)) ) x_size = y_size = 0;
			if( pt[0] != 0 ) {
				if( pt[0]%32 == 0 ) pt[0]-=1;
			}

		switch( y_size )
			{
			case 2:
				R_expand_window( FILTER_12, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 3:
				R_expand_window( FILTER_13, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 4:
				R_expand_window( FILTER_12, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_expand_window( FILTER_13, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 5:
				R_expand_window( FILTER_15, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 6:
				R_expand_window( FILTER_12, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_expand_window( FILTER_15, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 7:
				R_expand_window( FILTER_13, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_expand_window( FILTER_15, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 8:
				R_expand_window( FILTER_12, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_expand_window( FILTER_13, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_expand_window( FILTER_15, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 9:
				R_expand_window( FILTER_15, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_expand_window( FILTER_15, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			}

		switch( x_size )
			{
			case 2:
				R_expand_window( FILTER_21, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 3:
				R_expand_window( FILTER_31, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 4:
				R_expand_window( FILTER_21, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_expand_window( FILTER_31, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 5:
				R_expand_window( FILTER_51, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 6:
				R_expand_window( FILTER_21, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_expand_window( FILTER_51, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 7:
				R_expand_window( FILTER_31, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_expand_window( FILTER_51, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 8:
				R_expand_window( FILTER_21, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_expand_window( FILTER_31, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_expand_window( FILTER_51, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			case 9:
				R_expand_window( FILTER_51, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				R_expand_window( FILTER_51, mem_no, pt[0], pt[1], pt[2], pt[3] ) ;
				break;
			}
	}
///////////////////////////////////////////////////////////////////////////////
/*
int		TWA_es_bin_search_n( int p, int *rx, int *ry, int deg, int l, int flag, int nm )
	{
		auto			 div_t bit;
		unsigned long	*ad;
		unsigned long	i = 0x00000001;
		int				x = *rx;
		int				y = *ry;
		int				color = 0;
		int				es_p = 0;
		int				y_p = 1;
		int				ad_p = MxSize/32;
		int             n; //GEE

		color = flag & 0x0ff;
		es_p = (flag>>8)<<8;

		if( es_p == TWA_ES_p2 ) {
			y_p *= 2;
			ad_p *= 2;
		}

		if( l <= 0 ) return( ERROR );						//サーチ距離異常
		bit = div( x, 32 );
		ad = (unsigned long *)Bin_address[p] + y*(MxSize/32) + bit.quot;
		if( bit.rem != 0 ) i <<= bit.rem;
		if( color == KURO ) {
			switch( deg ) {
				case 90:	//認識方向　下
						n=0;                                    //GEE
					for( ; l>0; l-=y_p, y+=y_p, ad+=ad_p ) {
//						if( (*ad & i) == KURO ) break;
						if( (*ad & i) == KURO ) {
							n++;
							if( n == nm ) {
								y -= y_p*n;
								break;
							}
						}
					}
					break;
				case 270:	//認識方向　上
						n=0;                                    //GEE
					for( ; l>0; l-=y_p, y-=y_p, ad-=ad_p ) {
//						if( (*ad & i) == KURO ) break;
						if( (*ad & i) == KURO ) {               //GEE
							n++;
							if( n == nm ) {
								y += y_p*n;
								break;
							}
						}                                       //GEE
					}
					break;
				case 180:	//認識方向　左
						n=0;                                    //GEE
					for( ; l>0; l--, x--, i>>=1 ) {
						if( i == 0x00000000 ) {
							ad--;
							i = 0x80000000;
						}
//						if( (*ad & i) == KURO ) break;
						if( (*ad & i) == KURO ) {                //GEE
							n++;
							if( n == nm ) {
								x += n;
								break;
							}
						}                                       //GEE
					}
					break;
				case 0:		//認識方向　右
						n=0;                                    //GEE
					for( ; l>0; l--, x++, i<<=1 ) {
						if( i == 0x00000000 ) {
							ad++;
							i = 0x00000001;
						}
//						if( (*ad & i) == KURO ) break;
						if( (*ad & i) == KURO ) {                //GEE
							n++;
							if( n == nm ) {
								x -= n;
								break;
							}
						}                                       //GEE
					}
					break;
				}
		}
		else {
			switch( deg ) {
				case 90:	//認識方向　下
						n=0;                                    //GEE
					for( ; l>0; l-=y_p, y+=y_p, ad+=ad_p ) {
//						if( (*ad & i) != KURO ) break;
						if( (*ad & i) != KURO ) {
							n++;
							if( n == nm ) {
								y -= y_p*n;
								break;
							}
						}
					}
					break;
				case 270:	//認識方向　上
						n=0;                                    //GEE
					for( ; l>0; l-=y_p, y-=y_p, ad-=ad_p ) {
//						if( (*ad & i) != KURO ) break;
						if( (*ad & i) != KURO ) {               //GEE
							n++;
							if( n == nm ) {
								y += y_p*n;
								break;
							}
						}                                       //GEE
					}
					break;
				case 180:	//認識方向　左
						n=0;                                    //GEE
					for( ; l>0; l--, x--, i>>=1 ) {
						if( i == 0x00000000 ) {
							ad--;
							i = 0x80000000;
						}
//						if( (*ad & i) != KURO ) break;
						if( (*ad & i) != KURO ) {                //GEE
							n++;
							if( n == nm ) {
								x += n;
								break;
							}
						}                                       //GEE
					}
					break;
				case 0:		//認識方向　右
						n=0;                                    //GEE
					for( ; l>0; l--, x++, i<<=1 ) {
						if( i == 0x00000000 ) {
							ad++;
							i = 0x00000001;
						}
//						if( (*ad & i) != KURO ) break;
						if( (*ad & i) != KURO ) {                //GEE
							n++;
							if( n == nm ) {
								x -= n;
								break;
							}
						}                                       //GEE
					}
					break;
				}
		}
		*rx = x;
		*ry = y;
		if( l <= 0 ) return(ERROR);						//認識範囲オーバー
		return(OK);
	}
*/

////////////////////////////////
////////////////////////////////

void	WaitMouseClick( void )
	{
		R_DrawPopupMessage(POPUP_WAIT_CLICK, "mouse click wait");
		R_DrawClsPosi( 0, 0, FxSize-1, FySize-1 );
	}

/**********************************************************************************
	関数名：	ＴＷＡ＿ｇｅｔ＿ｖｉｄｅｏ＿ｏｎｃｅ
	機　能：	グレー画像を一度取り込む
	形　式：	void TWA_get_video_once( void );
	引き数：	なし
	戻り値：	なし
	解　説：	グレー画像を一度取り込みます。
	留意事項：	処理結果は、Judge_videoに入ります。
**********************************************************************************/

void	TWA_get_video_once( R_LED_PARA *pLedPara )
	{

		// マウス割り込み禁止 PS2
	//		R_stop_mouse_function( 0 );
		// LED照明 ON
			R_LedOn(pLedPara);			// 照明点灯
		// Auto get video
			R_CS_Capture(0, 0, 0);
		// LED照明 OFF
			R_LedOff(pLedPara);			// 照明消灯
		// Wait get video end
			R_CS_CaptureWait(0);

			R_time_delay(50*1000);

	}

/*********************************************************************************
	関数名：	ＴＷＡ＿Ｌｉｇｈｔ＿ｓｅｔ（ ｖｏｉｄ ）
	機　能：　	LED照度調整
	形　式：	void TWA_LED_set( void )
	引き数：	なし
	戻り値：	なし
	解　説：	照度と２値レベルの調整する。
	留意事項：	なし
*********************************************************************************/
static struct message_t func_mes = {14, 1, "function"};

int 	TWA_LED_set( int page, int level ,int Judge_LED_White, R_LED_PARA *pLedPara)
	{
		int					*ledcurrent;
		int					Current_bk;
		int					xs, xe, ys, ye;
		int					t_count, w_count, b_count;
		int					bright_r;
		int					mode = 1;	// ( normal:0, white:1 )
		int					loop;
		int					posi_res_x;
		int					posi_res_y;
		double				w_p, b_p;
		char				*func[] = { "-1000", "-100", "+100", "+1000", "N/W" };
		RTIME					waitTime;

		posi_res_x = 0;
		posi_res_y = 30;

		ledcurrent=pLedPara->item[0].data;
		Current_bk=*ledcurrent;
		while( R_get_cursor() != NO_TOUCH );

		//測定エリア設定
			xs = FxSize/2-100 - 20;
			xe = xs + 200 - 1;
			ys = FySize/2-100;
			ye = ys + 200 - 1;
			t_count = (xe-xs+1)*(ye-ys+1);

		//2値画面消去
			R_display_control( BIN_MEMORY | GRAY_MEMORY );
			R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
			R_move_bin_memory( 0, 1, 0, 0, FX_SIZE-1, FY_SIZE-1 );

		while( 1 ){
			//LED ADC Control

			if( *ledcurrent < 0     ) *ledcurrent = 0;
			if( *ledcurrent > 100000) *ledcurrent = 100000;
			R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+2 ,"Current : %5d uA", *ledcurrent );

			// get video
				TWA_get_video_once(pLedPara);
				R_move_gray_memory( page, GrayP0, 0, 0, FxSize-1, FySize-1 );

			//２値化（グレープレーンから２値プレーンへ）
				R_gray2bin16( level, 0, 0, FxSize, FySize, Gray_address[page], Bin_address[BinP1] );
				if( mode == 0 ) {
					R_bin_cls( 0, 0, FxSize-1, FySize-1 );
				} else {
					R_move_bin_memory( BinP1, 0, 0, 0, FxSize-1, FySize-1 );
				}
				R_box( xs, ys, xe, ye, GRAPH_REVERSE, SOLID_LINE );

			//指定エリアのヒストグラムを取る。
				bright_r = R_get_histgram( xs, ys, xe, ye, Gray_address[page] );
				R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+3 ,"Bright  : %5d", bright_r );
			//白Bitカウント
				w_count = R_count_bin_area( BinP1, xs, ys, xe, ye );
				b_count = t_count - w_count;
				w_p = w_count*100.0/t_count;
				b_p = b_count*100.0/t_count;
				if( mode != 0 ) {
					if( w_p >= Judge_LED_White  && w_p < 100) {
						R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+4 ,"White   : %5d(%5.1f) Pass", w_count, w_p );
					} else {
						R_DrawPrintf(DRAW_RED, DISP_NORMAL  , posi_res_x, posi_res_y+4 ,"White   : %5d(%5.1f) Fail", w_count, w_p );
					}
				} else {
					R_DrawPrintf(DRAW_RED, DISP_NORMAL, posi_res_x, posi_res_y+4 ,"                     ");
				}

				TWA_open_switch( func, 5 );
				R_cursor_on( );

				R_time_start( &waitTime );
				loop=0;
				while( loop==0 ) {
					if(500000 < R_time_read(&waitTime )){
						TWA_close_switch( 5 );
						R_cursor_off( );

						loop = -1;
					}
					switch(R_check_scr_switch()){
						case CANCEL:
							//	*ledcurrent = Current_bk;
							TWA_close_switch( 5 );
							while( R_check_scr_switch( ) );
							R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
							R_display_control( BIN_MEMORY | GRAY_MEMORY );
							return OK;
							break;
						case  EXECUTE:
							switch( ScrSw_select ) {
						case 0:
							break;
						case 1:
							*ledcurrent -= 1000;
							break;
						case 2:
							*ledcurrent -= 100;
							break;
						case 3:
							*ledcurrent += 100;
							break;
						case 4:
							*ledcurrent += 1000;
							break;
						case 5:
							mode ^= 0x0001;
							break;
					}
					while( R_check_scr_switch( ) );
					TWA_close_switch( 5 );
					R_cursor_off( );
				//	loop=-1;
				}
			}
		}

	}

/************************************************************
        ２値化（グレープレーンから２値プレーンへ）
*************************************************************/

void	TWA_gray2bin( int level, int xs, int ys, int xl, int yl, int gn, int bn )
    {
        int					rx;

        // xs check
            rx = xs % 4;
            if( rx != 0 ) {
                xs-=rx;
                xl+=rx;
            }
            if( xs < 0 ) xs = 0;

            R_gray2bin( level, xs, ys, xl, yl, Gray_address[gn], Bin_address[bn] );

    }
/*********************************************************************************
    関数名：	ＴＷＡ＿ｂｉｎ＿ｃｌｓ
    機　能：	２値画面消去
    形　式：	int TWA_bin_cls( int p, int x1, int y1, int x2, int y2 )
    引き数：	ｐは、バイナリプレーン番号
                ｘ１は、消去四角形の頂点１のｘ座標です。
                ｙ１は、消去四角形の頂点１のｙ座標です。
                ｘ２は、消去四角形の頂点２のｘ座標です。
                ｙ２は、消去四角形の頂点２のｙ座標です。
    戻り値：	ありません。
    留意事項：	特にありません。
*********************************************************************************/

void	TWA_bin_cls( int p, int x1, int y1, int x2, int y2 )
    {
    int		a, b, i, j, xt, yt;
    unsigned long	p1 = 0xffffffff, p2 = 0xffffffff;
    unsigned char	*bin_adr;
    if( x2 < x1 || y2 < y1 ) return;

    if( x1 > x2 ) {
        xt = x1;
        x1 = x2;
        x2 = xt;
    }
    if( y1 > y2 ) {
        yt = y1;
        y1 = y2;
        y2 = yt;
    }

    a = x1/32  ;
    b = x2/32;
    p1 >>= 32-x1%32;
    p2 <<= x2%32+1;
    if( x1%32 == 0 ) p1 = 0x00000000;
    if( x2%32 == 31 ) p2 = 0x00000000;

    bin_adr = Bin_address[p];

    if( a == b ) for( i = y1; i <= y2; i++ ) *( ( unsigned long * )( bin_adr+i*BIN_BX+a*4 ) ) &= p1 | p2;
    else {
        for( i = y1; i <= y2; i++ ) {
            *( ( unsigned long * )( bin_adr+i*BIN_BX+a*4 ) ) &= p1;
            *( ( unsigned long * )( bin_adr+i*BIN_BX+b*4 ) ) &= p2;
            for( j = a+1; j < b; j++ ) *( ( long * )( bin_adr+i*BIN_BX+j*4 ) ) = 0L;
        }
    }
}
