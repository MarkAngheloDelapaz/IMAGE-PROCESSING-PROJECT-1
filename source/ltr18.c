#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "R_stdlib.h"
#include "R_video.h"
#include "R_filter.h"
#include "R_bgray.h"
#include "R_binrec.h"
#include "R_meas.h"
#include "R_menu.h"
#include "R_dgraph.h"
#include "R_trans.h"
#include "R_math.h"
#include "R_edge.h"
#include "R_cursor.h"
#include "R_cs.h"
#include "R_cdraw.h"
#include "tmcr00i.h"
#include "tp_sub.h"
#include "LTR18_Ma.h"
#include "mcr03.h"

extern  MACHINE_PARAMETER	SystemPara;
extern  INSPECT_PARAMETER	InspPara, InspParaM;

// --- Chip 傾き ---
extern  int        		ChipAtan;       				// Chip 角度×１０の値（アークタンジェント）
// --- Inspection Inage(Gray)
extern int				GetGrayPn;                  	// Get capture		gray memory
// --- Inspection Inage(Bin)
extern int				InspBinPn;					// Inspection		bin memory
extern Regression_def   ElecEdge[3][8];
// Electrode and Overcoat data
extern Overcoat_data	Elec[3];

//仕様 ( 0:通常 1:表裏 2:ピンホール検査 3:パイロット )
extern  int				TWA_Insp_mode;                        				//仕様 ( 0:通常 1:表裏 2:ピンホール検査 3:パイロット )
extern  int				TWA_Spec;                         					//製品仕様( 0:Normal 1:NP3 2:Jpw)
extern  int				TWA_Insp_level;             						//検査モード（ 0:通常 1:詳細 ）
extern  int				TWA_Mark_Insp;          							//標印検査

extern  int             G2_pos[4];											//Ｇ２位置検出データ( lex, rix, upy, loy )
extern  int             Ap_pos[6];											//外形位置検出データ( lex, rix, leuy, lely, riuy, rily )


//標印検査
extern	int				Mark_Xseg[50][5];									//文字切り出し結果 X[ judge, xs, xe, level, 誤差 ]
extern	int				Mark_Yseg[50][5];									//文字切り出し結果 Y[ judge, ys, ye, level, 誤差 ]
extern	int				Mark_zure[4];//		= { 0,0,0,0 };					//標印ズレdata
extern	int				Mark_area_xl;										//標印文字範囲( X size )
extern	int				Mark_area_yl;										//標印文字範囲( Y size )
extern 	int				Mark_Slevel_s[5][3];								//標印文字有り無し判断レベル( min, center, max )
extern	int				Mark_Slevel_r[5][3];								//標印文字有り無し判断レベル( min, center, max )
extern	int				Mark_div_x;											//標印文字分割数 X
extern	int				Mark_div_y;											//標印文字分割数 Y
extern	int				Mark_area_x[125][4][4];								//標印文字範囲X ([θ][桁][x1,x2,x3,x4])
extern	int				Mark_area_y[125][4][4];								//標印文字範囲Y ([θ][桁][y1,y2,y3,y4])
extern	int				Mark_space_x[125][5][10];							//標印文字間隔X ([θ][桁+1][x1,x2,,..])
extern	int				Mark_space_y[125][5][10];							//標印文字間隔Y ([θ][桁+1][y1,y2,,..])
extern	int				Mark_size_ls[5][10][2];									//標印文字Size L White
extern	int				Mark_size_lr[5][10][2];									//標印文字Size L White
extern	int				Mark_rx[125][4][30];								//標印検査ポイントX([θ][桁][Xdata])
extern	int				Mark_ry[125][4][30];								//標印検査ポイントY([θ][桁][Ydata])
extern	int				Mark_Judge_st[5][NumMarkChar][5];							//標印文字判定結果[桁][文字][一致,Check,定点]
extern	int				Mark_Judge_re[5][NumMarkChar][5];							//標印文字判定結果[桁][文字][一致,Check,定点]
extern	int				Mark_L_set[5];//		= { 0,0,0,0,0 };				//標印詳細検査設定（0:検査無し 1:詳細検査）
extern	int				Mark_Ans_st[5];
extern	int				Mark_Ans_re[5];
extern	int				Mark_J100_st[5];
extern	int				Mark_J100_re[5];
//static	int				MarkChoice_st[5][NumMarkChar];								// 標印選択回数
//static	int				MarkChoice_re[5][NumMarkChar];								// 標印選択回数
//static	int				MarkChoiceMax_st[5][2];								// 標印選択 max[character][data]
//static	int				MarkChoiceMax_re[5][2];								// 標印選択 max[character][data]
//static	char			MarkCharacter[13][2]= { "0","1","2","3","4",
//                                                "5","6","7","8","9","R","L" };	// 標印表示文字

extern	int				Mark_Insp_mode;	//		= 0x0000;						//標印詳細検査設定bit（1桁目 0x01, 2桁目 0x02, 3桁目0x04, 4桁目0x08）
//static int	Mark_pos_le;
//static int 	Mark_pos_ri;
//static int	Diff_mark_seg;
extern	 int	mark_zure1;
extern	 int	mark_zure2;
extern	 int	mark_zure3;
extern	 int	mark_zure4;

//標印
extern char				MarkChar[10];

static  int             TWA_R_data[6];										//設定抵抗値（数値）

static	int				TWA_mark_area_p[8];									//標印文字範囲
//static	int				TWA_Judge_Ma		= ERROR;						//判定結果

//static	int				Bin_Ma_l_ini[4][2]	= {{ 30, 40},{ 30, 40},{ 30, 40},{ 20, 60}};	//標印検査時の２値化レベル設定範囲
static  int				Mark_temp[8];
static	int				Bin_Ma_l_ini[5][2]	= {{ 40, 65},{ 40, 65},{ 40, 65},{ 40, 65},{ 40, 65}}; // {{ 30, 40},{ 30, 40},{ 30, 40},{ 30, 40},{ 30, 40}};	//標印検査時の２値化レベル設定範囲 9499
static	int				Bin_Ma_l[5]			= { 50, 50, 50, 60,60}; // { 35, 35, 35, 40,40}				//標印検査時の２値化レベル 9499

extern  int				Bin_Mz_l[5];										//C1 mezumari Binary level			//<z3>
extern  int             ElecPosRough[3][4];									// 電極位置検出データ[side][pos]

extern  MEASDATA Mpx;
extern  BIN_MEASURE_PARAMETER	BinMeasChip;
#define	TWA_Mark_co		24
#define	TWA_Mark_ch1	20
#define	TWA_Mark_oX1	16
#define	TWA_Mark_oY1	12
#define	TWA_Mark_ch2	8
#define	TWA_Mark_oX2	4
#define	TWA_Mark_oY2	0

static void		TWA_G2_level( int grayPn, int *level, int *l_lo, int *l_up );
static int		TWA_segmentation( int deg10, int level, int n_seg, int offset_x, int offset_y );
int		TWA_Mark_point_10_25( int deg10, int ox, int oy, int ol );
static int		TWA_Mark_Judge( void );
static int      TWA_Mark_level( int deg10, int ox, int oy );
static int		TWA_Mark_More( void );
static void     TWA_Mark_error( int nc );
static void     TWA_mark_position_set( void );

void	TWA_gray2bin( int level, int xs, int ys, int xl, int yl, int gn, int bn );
//9499 START
static	int				MarkChoice_st[5][11];								// 標印選択回数
static	int				MarkChoice_re[5][11];								// 標印選択回数
static	int				MarkChoiceMax_st[5][2];								// 標印選択 max[character][data]
static	int				MarkChoiceMax_re[5][2];								// 標印選択 max[character][data]
static	char			MarkCharacter[12][2]= { "0","1","2","3","4",
		 												"5","6","7","8","9","R" };	// 標印表示文字
static int				MarkChoice_Limit = 10;	//9499							//Mark choice times Limit
unsigned char			CommentM[15][50];										// Comment Buffer(Mark)
//9499 END

//<---<K1>
static int		xsm = 0; 
static int		ysm = 0; 
static int		xem = 0; 
static int		yem = 0; 

static int 	xs_mb[4];
static int 	ys_mb[4];
static int 	xe_mb[4];
static int 	ye_mb[4];

static int      more_lvl= 0;
static int		Judge_Mark = 76;							// <z4>

//static	unsigned int	Judge_M_More[2] = {250, 50};						// 標印漏れ出し（にじみ）判定値[data] //<K1>
//static	unsigned int	Judge_M_More[2] = {750, 50};						// 標印漏れ出し（にじみ）判定値[data] //<K1>

#define KURO			0
#define SHIRO			1

//フィルター
static	int				G2_contrct_x		= 3;							//Ｇ２縮退Ｘサイズ
static	int				G2_contrct_y		= 3;							//Ｇ２縮退Ｙサイズ
static	int				G2_expand_x			= 3;							//Ｇ２膨張Ｘサイズ
static	int				G2_expand_y			= 3;							//Ｇ２膨張Ｙサイズ

static	int				Ap_contrct_x		= 3;							//外形縮退Ｘサイズ
static	int				Ap_contrct_y		= 3;							//外形縮退Ｙサイズ
static	int				Ap_expand_x			= 3;							//外形膨張Ｘサイズ
static	int				Ap_expand_y			= 3;							//外形膨張Ｙサイズ

#define TWA_ES_BLACK	0
#define TWA_ES_WHITE	1

//static	unsigned int	ES_G2_xp			= 8;							//Ｇ２検査幅
static	unsigned int	ES_G2_yp			= 8;							//Ｇ２検査幅
extern	unsigned int	G2_data_r[80][2];									//Ｇ２エッジ検査データ（Ｘ，Ｙ）上
extern	unsigned int	G2_data_l[80][2];									//Ｇ２エッジ検査データ（Ｘ，Ｙ）下
extern	unsigned int	G2_data_u[80][2];									//Ｇ２エッジ検査データ（Ｘ，Ｙ）右
extern	unsigned int	G2_data_d[80][2];									//Ｇ２エッジ検査データ（Ｘ，Ｙ）左
extern	unsigned int	ES_Ap_xp;											//外形検査幅
extern	unsigned int	ES_Ap_yp;											//外形検査幅
extern	unsigned int	Ap_data_u[80][2];									//外形検査データ（Ｘ，Ｙ）上
extern	unsigned int	Ap_data_d[80][2];									//外形検査データ（Ｘ，Ｙ）下
extern	unsigned int	Ap_data_r[80][2];									//外形検査データ（Ｘ，Ｙ）右
extern	unsigned int	Ap_data_l[80][2];									//外形検査データ（Ｘ，Ｙ）左
extern	int				Ap_ue_ey;											//外形検査上 Ｙデータ平均値
extern	int				Ap_shita_ey;										//外形検査下 Ｙデータ平均値
extern	int				Ap_r_ex;											//外形検査右 Ｘデータ平均値
extern	int				Ap_l_ex;											//外形検査左 Ｘデータ平均値

//回帰係数設定用
extern KAIKI		ue, shita, migi, hidari, w, l, Ap_up, Ap_lo, Ap_ri, Ap_le, Ap_w, Ap_l;
//平均値・標準偏差
extern MEANSD		up_y, lo_y, le_x, ri_x, Ap_up_y, Ap_lo_y, Ap_le_x, Ap_ri_x;

extern int g2level;

// Filter C2 Insp. //9499
static  int				TWA_SurfLoC2			= 20;	//10;
static  int				TWA_SmoothingXsizeC2	= 10;	//5;
static  int				TWA_SmoothingYsizeC2	= 10;	//5;                 //GEE 3 to  5
static  int				TWA_CoefficientC2		= 10;
static  int				Bin_C2YsizeLevel		= 60; //9499 110

static	int				Corner_pos[10];										// 外形角位置データ( le_xu, le_yu, le_xl, le_yl, ri_xu, ri_yu, ri_xl, ri_yl )
static	unsigned int  	FreqData[1000];
static	unsigned int	C2_data_ul[480][2];
static	unsigned int	C2_data_ur[480][2];
static	unsigned int	C2_data_ll[480][2];
static	unsigned int	C2_data_lr[480][2];

// Filter C1 Insp.
static	int				C1_level			= 220;

static	int				Pl_contrct_x		= 3;							//メッキ付着縮退Ｘサイズ
static	int				Pl_contrct_y		= 3;							//メッキ付着縮退Ｙサイズ
static	int				Pl_expand_x			= 3;							//メッキ付着膨張Ｘサイズ
static	int				Pl_expand_y			= 3;							//メッキ付着膨張Ｙサイズ
static	int				Bin_C2_Loffset		= 110;	//110 JHONG

// Mezumari Inspection.
static	int				TWA_SurfLoMzO			= 10; 			//<z3>
static	int				TWA_SmoothingXsizeMzO	= 7; 		//<z3>
static	int				TWA_SmoothingYsizeMzO	= 3; 		//<z3>
static	int				TWA_CoefficientMzO		= 10; 	//<z3>
static	int				Bin_C2XsizeLevelMz		= 85;		//<z3>

extern  int             IGuidePos[2];										// Index guide position
extern	int				Index_pos[6];										//インデックステーブル　エッジ位置( uy, dy, ly )
int		ElecEsLevelSet( void );
extern  BIN_LEVEL_PARAMETER		IGuideEsLevel;
/************************************************
	標印検査
************************************************/

int		TWA_kensa_l18( void )
	{
		int				deg10;
		int				i, nc;
		int				level, l_lo, l_ce, l_up;
		int				ans;
		int				hx, hy, hf, hfx, hfy, ox, oy;
		int				hl_x, hl_y;
		int				off_px, off_py;
		int				n_seg, n_x, n_y;
		int				x_seg, y_seg;
		int				seg_offset_x, seg_offset_y;
		int				offset_Mal[3] = { 0, 5, -5 };
		int				binPn	= InspBinPn;
		int				grayPn  = InspGrayPn;
//		int				i_mode	= R_CS_GetInspectMode();
		int				nm;
		char            CommentM[15][50];					// Comment Buffer(Mark)
		int				res_yesno;							// v2.20

//***** 9499 START
		int				res, res_st, res_re;
	  int				i_mode	= R_CS_GetInspectMode();
		int posi_res_x,posi_res_y;
//***** 9499 END


		//There is marking inspection, no  //標印検査有り無し
			if( TWA_Mark_Insp != 0 )	return OK;

		//marking character inspection  // 標印文字検査
			if( TWA_Insp_mode == Insp_Pinhole )	return OK;		//Plating adhesion inspection //メッキ付着検査

		//marking inclination calculation  // 標印傾き計算
			deg10 = TWA_katamuki();
			i = (300+deg10)/5;
			if((i<0)||(i>120)) return ERROR;
			
//***** 9499 START Initialize value
			for( nc=0; nc<TWA_R_n; nc++ ) {
				for( nm=0; nm<11; nm++ ) {
					MarkChoice_st[nc][nm] = 0;
					MarkChoice_re[nc][nm] = 0;
				}
				MarkChoiceMax_st[nc][0] = ERROR;
				MarkChoiceMax_st[nc][1] = 0;
				MarkChoiceMax_re[nc][0] = ERROR;
				MarkChoiceMax_re[nc][1] = 0;
			}
//***** 9499 END	

		//Mark Level
			ans = ERROR;
			level = 0;
			TWA_G2_level( grayPn, &l_ce, &l_lo, &l_up );

		//A character is brought down.  //文字切り出し
			seg_offset_x = seg_offset_y = 0;
			while(1) {
				for( n_seg=0; n_seg<50; n_seg++ ) {
					Mark_Xseg[n_seg][0] = Mark_Xseg[n_seg][1] = Mark_Xseg[n_seg][2] = Mark_Xseg[n_seg][3] = Mark_Xseg[n_seg][4] = ERROR;
					Mark_Yseg[n_seg][0] = Mark_Yseg[n_seg][1] = Mark_Yseg[n_seg][2] = Mark_Yseg[n_seg][3] = Mark_Yseg[n_seg][4] = ERROR;
				}
				n_seg = 0;
				n_x = n_y = ERROR;
				x_seg = y_seg = 100;
				
				for( level=l_lo; level<=l_up; level+=5, n_seg++ ) {
					TWA_segmentation( deg10, level, n_seg, seg_offset_x, seg_offset_y );
					if( Mark_Xseg[n_seg][0] == OK ) {
						if( x_seg > Mark_Xseg[n_seg][4]) {
							x_seg = Mark_Xseg[n_seg][4];
							n_x = n_seg;
						}
					}
					if( Mark_Yseg[n_seg][0] == OK ) {
						if( y_seg > Mark_Yseg[n_seg][4]) {
							y_seg = Mark_Yseg[n_seg][4];
							n_y = n_seg;
						}
					}
				}
				
				if((n_x!=ERROR)&&(n_y!=ERROR)) {
					Ma_pos[0] = Mark_Xseg[n_x][1];
					Ma_pos[1] = Mark_Xseg[n_x][2];
					Ma_pos[2] = Mark_Yseg[n_y][1];
					Ma_pos[3] = Mark_Yseg[n_y][2];
					break;	// segmentation OK;
				} else {
					if( TWA_Insp_mode != Insp_Pinhole ) {
						if( n_y == ERROR ) {
							seg_offset_y += 5;
							if( seg_offset_y > 10 ) return ERROR2;
						} else if( n_x == ERROR ) {
							seg_offset_x += 5;
							if( seg_offset_x > 10 ) return ERROR2;
						}
					} else {
						return(OK);
					}
				}
			}
	

		//Character level  //文字レベル
			ox = oy = 0;
			TWA_Mark_level( deg10, ox, oy );
	
			for( nc=0; nc<TWA_R_n; nc++ ) {
				Mark_Ans_st[nc] = ERROR;
				Mark_Ans_re[nc] = ERROR;
				Mark_J100_st[nc] = ERROR;
				Mark_J100_re[nc] = ERROR;
			}
//start 
/******************************************

			hl_x = abs((Ma_pos[1]-Ma_pos[0])-Mark_area_xl);
			if( hl_x <= 0 ) hl_x = 0;
			else			hl_x /= 2;
			hl_x += 2;
			if( Mark_Insp_mode == 0 ) {
				if( hl_x < 4 ) hl_x = 4;
			}
			
			hl_y = abs((Ma_pos[3]-Ma_pos[2])-Mark_area_yl);
			if( hl_y <= 0 ) hl_y = 0;
			else			hl_y /= 2;
			hl_y += 2;
			if( Mark_Insp_mode == 0 ) {
				if( hl_y < 4 ) hl_y = 4;
			}
******************************************/
//end

			hl_x = 4;	//; //6 4<0002>
			hl_y = 4;	//; //2 4<0002>
//<0002>
			off_px = off_py = 1; //1 - 2 //9499

			ans = ERROR;
			hy = 0;
			for( hfy=1; hy<hl_y; hfy++ ) {
				hf = hfy%2;
				if( hf == 0 ) {
					oy = hy;
				} else {
					oy = 0 - hy;
					hy += off_py;
				}
				hx = 0;
				for( hfx=1; hx<hl_x; hfx++ ) {
					hf = hfx%2;
					if( hf == 0 ) {
						ox = hx;
					} else {
						ox = 0 - hx;
						hx += off_px;
					}
					for( nm=0; nm<3; nm++ ) {
						//標印データ読み取り
						if( TWA_Mark_point_10_25( deg10, ox, oy, offset_Mal[nm] ) == OK ) {
							//文字判定
							if( TWA_Mark_Judge() == OK ) {
								ans = OK;
							//9499	break;
							}
							
							//9499-START
							if( i_mode & INSPECT_STEP ) {
								res_yesno=R_DrawMouseDeYesno();					// v2.20
								if(NO==res_yesno) {
									R_DrawCls();
									goto TWA_kensa_END;
								}
							} //9499-END
							
						}
//						for( nc=0; nc<TWA_R_n; nc++ ) {
//							Mark_J100_st[nc] = ERROR;
//							Mark_J100_re[nc] = ERROR;
//						}
					}
					if( ans == OK ) {
				//9499		break;
					}
				}
/***** 9499 START	
				if( ans == OK ) {
					Ma_pos[0] += ox;
					Ma_pos[1] += ox;
					Ma_pos[2] += oy;
					Ma_pos[3] += oy;
					break;
				}
9499 END ******/
				
			}
			
//***** 9499 START Mark Choice

		// Check Mark choice data
			for( nc=0; nc<TWA_R_n; nc++ ) {
				for( nm=0; nm<11; nm++ ) {
					if( MarkChoice_st[nc][nm] > MarkChoiceMax_st[nc][1] ) {
						MarkChoiceMax_st[nc][0] = nm;
						MarkChoiceMax_st[nc][1] = MarkChoice_st[nc][nm];
					}
					if( MarkChoice_re[nc][nm] > MarkChoiceMax_re[nc][1] ) {
						MarkChoiceMax_re[nc][0] = nm;
						MarkChoiceMax_re[nc][1] = MarkChoice_re[nc][nm];
					}
				}
			}
			// --- view
//9499	if( i_mode & INSPECT_STEP ){ //<z17> to display mark matching result
if( nDispMode1 == 1){ //9499
				R_display_control( BIN_MEMORY | GRAY_MEMORY );
				R_bin_cls( 0, 0, FxSize-1, FySize-1 );
				R_move_gray_memory( InspGrayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				// --- Title
				posi_res_x = 0;
				posi_res_y = 30;

				R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"Marking[%s] X:%d, Y:%d", MarkChar,hl_x,hl_y);
				posi_res_y ++;
				R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"Matching Limit: > %d is GOOD", MarkChoice_Limit);

				posi_res_x = 0;
				posi_res_y ++;

				for( nm=0; nm<11; nm++ ) {
					R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"[%s]|", MarkCharacter[nm]);
					posi_res_y++;
				}

				R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"STR: ");
				posi_res_y = posi_res_y + 1;
				R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"REV: ");

				// --- Character data
				for( nc=0; nc<TWA_R_n; nc++ ) {

					// --- Character data
					posi_res_x = (nc * 10) + 5;
					if(nc == 0) posi_res_x = 5;
					posi_res_y = 32;

					for( nm=0; nm<11; nm++ ) {
						R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"(%2d)(%2d)|", MarkChoice_st[nc][nm], MarkChoice_re[TWA_R_n-1-nc][nm]);
						posi_res_y++;
					}


					// --- Max Character data(st)
					nm = MarkChoiceMax_st[nc][0];

					if( nm < 0 )	R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x+3, posi_res_y   ,"%s(%2d): ", "-", 0 );
					else R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x+3, posi_res_y   ,"%s(%2d): ", MarkCharacter[nm], MarkChoice_st[nc][nm]);
					posi_res_y++;


					nm = MarkChoiceMax_re[TWA_R_n-1-nc][0];
					if( nm < 0 )	R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x+3, posi_res_y   ,"%s(%2d): ", "-", 0 );
					else R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x+3, posi_res_y   ,"%s(%2d): ", MarkCharacter[nm], MarkChoice_re[TWA_R_n-1-nc][nm]);
					posi_res_y++;


				}

			//WaitMouseClick();
				BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
//9499	
}
//2021.09.30<shio>  Target charactor(TWA_R_data[nc]) OK times
//   MarkChoice_st[nc][nm] > MarkChoice_Limit is OK
            res_st = res_re  =  OK; 
			for( nc=0; nc<TWA_R_n; nc++ ) {		
				nm = TWA_R_data[nc];
				if( MarkChoice_st[nc][nm] <	MarkChoice_Limit ) {
					res_st = ERROR;
					break;
				}
			}	
			// --- Judge Max Choice(re)
			for( nc=0; nc<TWA_R_n; nc++ ) {
				nm = TWA_R_data[nc];
				if( MarkChoice_re[nc][nm] <	MarkChoice_Limit )	{
					res_re = ERROR;
					break;
				}
			}
/* 2021.09.30<shio> comment out below judgement		// Judge // MIA
			// --- Judge Max Choice(st)
			for( nc=0; nc<TWA_R_n; nc++ ) {
				if( MarkChoiceMax_st[nc][0] != TWA_R_data[nc] )	break;
				if( MarkChoiceMax_st[nc][1] < MarkChoice_Limit )				break;
			}
			if( TWA_R_n == nc )	res_st = OK;
			else				res_st = ERROR;
			// --- Judge Max Choice(re)
			for( nc=0; nc<TWA_R_n; nc++ ) {
				if( MarkChoiceMax_re[nc][0] != TWA_R_data[nc] ) break;
				if( MarkChoiceMax_re[nc][1] < MarkChoice_Limit )				break;
			}
			if( TWA_R_n == nc )	res_re = OK;
			else				res_re = ERROR;
		//20190110 shio
		
			for( nc=0; nc<TWA_R_n; nc++ ) {
				nm = MarkChoiceMax_st[nc][0];
				if( MarkChoice_st[nc][nm]  < MarkChoice_Limit ){
				res_st = ERROR;
				 break;
				}
			}
			for( nc=0; nc<TWA_R_n; nc++ ) {
				nm = MarkChoiceMax_re[nc][0];
				if( MarkChoice_re[nc][nm]  < MarkChoice_Limit ){
				res_re = ERROR;
				 break;
				}

			}
*********************************************/

			if(( res_st != OK )&&( res_re != OK ))	return ERROR5;	//20190110 shio
//***** 9499 END			
			
TWA_kensa_END:
			if( ans != OK ) return(ERROR4);

		return OK;
	
	}	// TWA_kensa_m END
	
//------------------------------------------------
//	Mark level
//------------------------------------------------

static void	TWA_G2_level( int grayPn, int *level, int *l_lo, int *l_up )
	{
		int					xs, xe, ys, ye;
		int					cl, clmin, clcen, clmax;
		int					n;
		int					bright_n;
		int					i_mode	= R_CS_GetInspectMode();
		R_RECT				area;
	
		//検査範囲設定
	//	xs = G2_pos[0] + 10;
	//	xe = G2_pos[1] - 10;
		ys = G2_pos[2] + 10;
		ye = G2_pos[3] - 10;
		xs	= ElecPosRough[DataUpper][PosXmin]+100;
		xe	= ElecPosRough[DataUpper][PosXmax]-100;
	
		//指定エリアのヒストグラムを取る。（１ドットおきに間引く）
			R_get_step_histgram( xs, ys, xe, ye, Gray_address[grayPn] );
			bright_n = 0;
			for( n=1; n<=255; n++ ) bright_n += Bright[n];
			bright_n = bright_n/5 + 1;
			//標印文字有り無し判断レベル
				//min level
				cl = 0;
				for( n=1; n<255; n++ ) {
					cl += Bright[n];
					if( cl > bright_n ) break;
				}
				clmin = n;
				if( clmin < Bin_Ma_l_ini[TWA_Spec][0] ) clmin = Bin_Ma_l_ini[TWA_Spec][0];
//				if( clmin < 10 ) clmin = 10;	//最低レベル 10
				//max level
				cl = 0;
				for( n=255; n>0; n-- ) {
					cl += Bright[n];
					if( cl > bright_n ) break;
				}
				clmax = n;
				if( clmin > clmax ) clmax = clmin;
				//level
				cl = (clmax-clmin)/10 + 1;
				clcen = (clmin+clmax)/2;		//center level
//				clmin = clcen - cl;				//min level
				*l_lo = clcen - cl;				//min level
//				clmax = clcen + cl;				//max level
				*l_up = clcen + cl;				//max level
				if( *l_lo < Bin_Ma_l_ini[TWA_Spec][0] ) *l_lo = Bin_Ma_l_ini[TWA_Spec][0];
//				if( *l_lo < 10 ) *l_lo = 10;
				*level = clcen; //9499
//				*l_lo = clmin;
//				*l_up = clmax;
			//Manual
			if( i_mode & INSPECT_STEP ) {
				R_display_control( BIN_MEMORY | GRAY_MEMORY );
				R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				//TWA_pset_histgram( 10, 50, Bright) ;
				WaitMouseClick();
				TWA_gray2bin( clcen, xs, ys, xe-xs+1, ye-ys+1, grayPn, BinP0 );
//				R_gray2bin( clcen, xs, ys, xe-xs+1, ye-ys+1, Gray_address[2], Bin_address[0] );
				area.left	= xs;
				area.right	= xe;
				area.top	= ys;
				area.bottom	= ye;
				R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );	
				printf( "Mark Seg.Bright min:%d  cen:%d  max:%d  n:%d\n", clmin, clcen, clmax, bright_n );
				printf( "Mark Seg.Level  min:%d  cen:%d  max:%d\n", *l_lo, clcen, *l_up );
				WaitMouseClick();
				R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
			}

	}	// TWA_G2_level( int grayPn, int *level, int *l_lo, int *l_up ) end


/*********************************************************************************
	関数名：	ＴＷＡ＿ｓｅｇｍｅｎｔａｔｉｏｎ（ int deg10 ）
	機　能：　	文字切り出し
	形　式：	int TWA_segmentation( int deg10 )
	引き数：	deg10
				文字角度データ(10倍)
	戻り値：	値			定　数			意　味
				0			OK				正常に文字切り出しが完了しました。
				-1			ERROR			異常終了しました。文字の切り出しが出来なかった。
	解　説：	標印文字の切り出しを行う。
	留意事項：	
*********************************************************************************/

static int		TWA_segmentation( int deg10, int level, int n_seg, int offset_x, int offset_y )
{
		int				xs, xe, ys, ye, xl, yl, xs1, xe1;
		int				xn, yn;
		int				xp[640], yp[480];		// 20170614
		int				n, n1;
		unsigned char	*sp_ad;
		int				l;
		int				x0, y0, x, y;
		int				div_n;
		int				i;
		int				slx, slx2, sly, snx, sny;
		int				x1, y1, x2, y2;
	
		int				xp_av, xp_n, xp_max;
		int				yp_av, yp_n, yp_max;
	
		int				ans, judge_l;

		R_RECT			area;
		int				i_mode	= R_CS_GetInspectMode();
		int				grayPn	= InspGrayPn;
		int				binPn	= InspBinPn;

		if( TWA_R_n == 3 ) {
			slx2 = 3;
			sly = 3;
			snx = 3;
			sny = 3;
		} else {
			slx2 = 3;
			sly = 3;
			snx = 3;
			sny = 3;
		}

		if( i_mode & INSPECT_STEP ) {
			R_display_control( BIN_MEMORY | GRAY_MEMORY );
			R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
			R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
		}

		//標印傾き確認
		i = (300+deg10)/5;
		if((i<0)||(i>120)) return(ERROR);

		for( n=0; n<640; n++ ) xp[n] = 0;
		for( n=0; n<480; n++ ) yp[n] = 0;

		//検査範囲設定
		ys = G2_pos[2] + 10 + offset_y;			// 5 ->10  2021.10.15<shio> same as Ver9.02
		ye = G2_pos[3] - 10 - offset_y;			// 5 ->10  
		//xs = ElecPosRough[DataLeft][2]+10;
		//xe = ElecPosRough[DataRight][0]-10;
		xs = G2_pos[0] + 50;
		xe = G2_pos[1] - 50;

		//Y
		//Y level
		xs1 = xe1 = 0;
		for( n=n1=0; n<=n_seg; n++ ) {
			if( Mark_Xseg[n][0] == OK ) {
				xs1 += Mark_Xseg[n][1];
				xe1 += Mark_Xseg[n][2];
				n1++;
			}
		}
		if( n1 != 0 ) {
			xs1 /= n1;
			xe1 /= n1;
		} else {
			//xs1	= ElecPosRough[DataLeft][2]+30;
			//xe1	= ElecPosRough[DataRight][0]-30;
			xs1 = G2_pos[0] + 65;			//15 -> 65 2021.10.25<shio> same as Ver9.02
			xe1 = G2_pos[1] - 65;			//15 -> 65 2021.10.25<shio>	
		}

		for( y=ys, yn=0; y<=ye; y+=2, yn++ ) {
			sp_ad = Gray_address[grayPn]+MxSize*y;
			for( x=xs1; x<=xe1; x+=4 ) {
				l = *(sp_ad+x);
				if( l > level ) yp[yn]++;			//一定レベル以下は、カウントしない
			}
		}

		yp_av = yp_n = yp_max = 0;
		for( n=0; n<yn; n++ ) {
			if( yp[n] != 0 ) {
				yp_max = max( yp_max, yp[n] );
				yp_av += yp[n];
				yp_n++;
				//Test
				if( i_mode & INSPECT_STEP ) {
					y1 = (ys+n*2-1);
					x1 = G2_pos[1] - yp[n];
					x2 = G2_pos[1];
					R_line( x1, y1, x2, y1, GRAPH_DRAW, SOLID_LINE );
				}
			}
		}
		if( yp_n != 0 ) {
			yp_av/=yp_n;
			sly = yp_av/2;
			//Test
			if( i_mode & INSPECT_STEP ) {
				y1 = ys;
				y2 = (ys+yn*2-1);
				x1 = G2_pos[1] - sly;
				R_line( x1, y1, x1, y2, GRAPH_DRAW, SOLID_LINE );
			}
		}

		//Yup
		if( Mark_zure[2] == 0 ) {
			for( n=0; n<yn; n++ ) {
				for( n1=0; n1<sny; n1++ ) {
					if( yp[n+n1] < sly ) break;
				}
				if( n1 == sny ) {
					Mark_Yseg[n_seg][1] = (ys+n*2-1);
					break;
				}
			}
			if( n >= yn ) Mark_Yseg[n_seg][1] = ERROR;
		} else {
			Mark_Yseg[n_seg][1] = Mark_zure[2];
		}

		//Ylo
		if( Mark_zure[3] == 0 ) {
			for( n=yn-1; n>=0; n-- ) {
				for( n1=0; n1<sny; n1++ ) {
					if( yp[n-n1] < sly ) break;
				}
				if( n1 == sny ) {
					Mark_Yseg[n_seg][2] = (ys+n*2+1);
					break;
				}
			}
			if( n < 0 ) Mark_Yseg[n_seg][2] = ERROR;
		} else {
			Mark_Yseg[n_seg][2] = Mark_zure[3];
		}

		//Y Test
		if( i_mode & INSPECT_STEP ) {
			if( Mark_Yseg[n_seg][1]!=ERROR ) {
				ys = Mark_Yseg[n_seg][1];
				R_line( xs1, ys, xe1, ys, GRAPH_DRAW, SOLID_LINE );
			}
			if( Mark_Yseg[n_seg][2]!=ERROR ) {
				ye = Mark_Yseg[n_seg][2];
				R_line( xs1, ye, xe1, ye, GRAPH_DRAW, SOLID_LINE );
			}
			DrawMessage( "Mark segmentation Y position", DRAW_GREEN, 2, 2 );
		}

		//Y judge
		ans = OK;
		if( Mark_Yseg[n_seg][1] < G2_pos[2] ) ans = ERROR;
		if( Mark_Yseg[n_seg][2] > G2_pos[3] ) ans = ERROR;

		if( ans == OK ) {
			yl = Mark_Yseg[n_seg][2] - Mark_Yseg[n_seg][1];
			y = abs( yl - Mark_area_yl )*100/Mark_area_yl;
			if( y < 30 ) {
				Mark_Yseg[n_seg][0] = OK;
				Mark_Yseg[n_seg][3] = level;
				Mark_Yseg[n_seg][4] = y;
			}
		}

		//X
		//X level
		ys = ye = 0;
		for( n=n1=0; n<=n_seg; n++ ) {
			if((Mark_Yseg[n][1]!=ERROR)&&(Mark_Yseg[n][2]!=ERROR)) {
				ys += Mark_Yseg[n][1];
				ye += Mark_Yseg[n][2];
				n1++;
			}
		}

		if( n1 != 0 ) {
			ys /= n1;
			ye /= n1;
			//v2.20 xs = G2_pos[0]+16+offset_x;
			//v2.20 xe = G2_pos[1]-16-offset_x;
			xs = G2_pos[0]+40+offset_x;
			xe = G2_pos[1]-40-offset_x;
			for( x=xs, xn=0; x<=xe; x+=4, xn++ ) {
				for( y=ys; y<=ye; y++ ) {
					sp_ad = Gray_address[grayPn]+MxSize*y;
					l = *(sp_ad+x);
					if( l > level ) xp[xn]++;		//一定レベル以下は、カウントしない
				}
			}
		} else {
			return(ERROR);
		}

		xp_av = xp_n = xp_max = 0;
		for( n=0; n<xn; n++ ) {
			if( xp[n] > 0 ) {
				xp_max = max( xp_max, xp[n] );
				xp_av += xp[n];
				xp_n++;
				//Test
				if( i_mode & INSPECT_STEP ) {
					x1 = xs+n*4;
					y1 = G2_pos[3] - xp[n];
					y2 = G2_pos[3];
					R_line( x1, y1, x1, y2, GRAPH_DRAW, SOLID_LINE );
				}
			}
		}
		if( xp_n != 0 ) {
			xp_av/=xp_n;
			slx = xp_av/3; //<0002>
			if( MarkChar[0] == '7' )	slx = 2;
			//Test
			if( i_mode & INSPECT_STEP ) {
				x1 = xs;
				x2 = xs+xn*4;
				y1 = G2_pos[3] - slx;
				R_line( x1, y1, x2, y1, GRAPH_DRAW, SOLID_LINE );
			}
		} else {
			slx = slx2;
		}

	//Xle
		for( n=0; n<xn; n++ ) {
			for( n1=0; n1<snx; n1++ ) {
				if( xp[n+n1] < slx ) break;
			}
			if( n1 == snx ) {
				Mark_Xseg[n_seg][1] = xs+n*4;
				break;
			}
		}
		if( n >= xn ) Mark_Xseg[n_seg][1] = ERROR;

		if( Mark_Xseg[n_seg][1] != ERROR ) {
			for( ; n>0; n-- ) {
				if( xp[n] < slx2 ) break;
			}
			n++;
			Mark_Xseg[n_seg][1] = xs+n*4;
		}

	//Xri
		for( n=xn-1; n>=0; n-- ) {
			for( n1=0; n1<snx; n1++ ) {
				if( xp[n-n1] < slx ) break;
			}
			if( n1 == snx ) {
				Mark_Xseg[n_seg][2] = xs+n*4;
					break;
			}
		}
		if( n < 0 ) Mark_Xseg[n_seg][2] = ERROR;

		if( Mark_Xseg[n_seg][2] != ERROR ) {
			for( ; n<xn; n++ ) {
				if( xp[n] < slx2 ) break;
			}
			n--;
			Mark_Xseg[n_seg][2] = xs+n*4;
		}

		if( i_mode & INSPECT_STEP ) {
			ys = Mark_Yseg[n_seg][1];
			ye = Mark_Yseg[n_seg][2];
			if( Mark_Xseg[n_seg][1]!=ERROR ) {
				xs = Mark_Xseg[n_seg][1];
				R_line( xs, ys, xs, ye, GRAPH_DRAW, SOLID_LINE );
			}
			if( Mark_Xseg[n_seg][2]!=ERROR ) {
				xe = Mark_Xseg[n_seg][2];
				R_line( xe, ys, xe, ye, GRAPH_DRAW, SOLID_LINE );
			}
			DrawMessage( "Mark segmentation X position", DRAW_GREEN, 2, 2 );
		}

		if( Mark_Xseg[n_seg][1] == ERROR ) return(ERROR);
		if( Mark_Xseg[n_seg][2] == ERROR ) return(ERROR);

		x0 = (Mark_Xseg[n_seg][1]+Mark_Xseg[n_seg][2])/2;
		y0 = (Mark_Yseg[n_seg][1]+Mark_Yseg[n_seg][2])/2;

		if( i_mode & INSPECT_STEP ) {
			R_display_control( BIN_MEMORY | GRAY_MEMORY );
			R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
			R_move_gray_memory( grayPn, GrayP0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
			xs = Mark_Xseg[n_seg][1];
			xe = Mark_Xseg[n_seg][2];
			ys = Mark_Yseg[n_seg][1];
			ye = Mark_Yseg[n_seg][2];
			area.left	= xs;
			area.right	= xe;
			area.top	= ys;
			area.bottom	= ye;
			R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
			WaitMouseClick();

			//標印傾き確認
				i = (300+deg10)/5;
			//文字エリア
			for( n=0; n<TWA_R_n; n++ ) {
				xs = x0 + Mark_area_x[i][n][0];
				xe = x0 + Mark_area_x[i][n][3];
				ys = y0 + Mark_area_y[i][n][0];
				ye = y0 + Mark_area_y[i][n][2];
				area.left	= xs;
				area.right	= xe;
				area.top	= ys;
				area.bottom	= ye;
				R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
			}
			WaitMouseClick();
		}

		//X judge
		ans = OK;
		if( Mark_Xseg[n_seg][1] < G2_pos[0] ) ans = ERROR;
		if( Mark_Xseg[n_seg][2] > G2_pos[1] ) ans = ERROR;

		if( ans == OK ) {
			xl = Mark_Xseg[n_seg][2] - Mark_Xseg[n_seg][1];
			if( xl > Mark_area_xl ) {
				judge_l = 15;
			} else {
				judge_l = 20;
			}
			x = abs( xl - Mark_area_xl )*100/Mark_area_xl;
			if( x < judge_l ) {
				Mark_Xseg[n_seg][0] = OK;
				Mark_Xseg[n_seg][3] = OK;
				Mark_Xseg[n_seg][4] = x;
			}
		}

		if((Mark_Xseg[n_seg][0]!=OK)||(Mark_Yseg[n_seg][0]!=OK)) return ERROR;

		return OK;

}	//TWA_segmentation End


static int	TWA_Mark_level( int deg10, int ox, int oy )
	{
		int					x0, y0;
		int					i, cn, n;
		int					xs, xe, ys, ye;
		int					cl, clmin, clmax;
		int					bright_n;
		int					mark_left,mark_right;       //
		int					mark_top ,mark_bottom;      //
		int					left_length ,right_length;
		int					left_lengthM,right_lengthM;
		int					top_length ,bottom_length;
		int					top_lengthM,bottom_lengthM;
		R_RECT				area;
		int					i_mode	= R_CS_GetInspectMode();
		int					grayPn	= InspGrayPn;

		//上下左右ズレ
			mark_zure1 = ERROR;
			mark_zure2 = ERROR;
			mark_zure3 = ERROR;
			mark_zure4 = ERROR;

		//Manual
			if( i_mode & INSPECT_STEP ) {
				R_display_control( BIN_MEMORY | GRAY_MEMORY );
				R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
			}

		//標印傾き確認
			i = (300+deg10)/5;
		//文字エリア中心
			x0 = (Ma_pos[0]+Ma_pos[1])/2;
			y0 = (Ma_pos[2]+Ma_pos[3])/2;
			x0 += ox;
			y0 += oy;

		//1文字毎にレベル設定
			for( cn=0; cn<TWA_R_n; cn++ ) {
				//検査範囲設定
					xs = x0 + max( Mark_area_x[i][cn][0], Mark_area_x[i][cn][2] );
					xe = x0 + min( Mark_area_x[i][cn][1], Mark_area_x[i][cn][3] );
					ys = y0 + max( Mark_area_y[i][cn][0], Mark_area_y[i][cn][1] );
					ye = y0 + min( Mark_area_y[i][cn][2], Mark_area_y[i][cn][3] );
				//指定エリアのヒストグラムを取る。
					R_get_histgram( xs, ys, xe, ye, Gray_address[grayPn] );
					bright_n = 0;
					for( n=1; n<=255; n++ ) bright_n += Bright[n];
					bright_n = bright_n/5 + 1;
				//標印文字有り無し判断レベル
					//min level
					cl = 0;
					for( n=1; n<255; n++ ) {
						cl += Bright[n];
						if( cl > bright_n ) break;
					}
					clmin = n;
					if( clmin < Bin_Ma_l_ini[TWA_Spec][0] ) clmin = Bin_Ma_l_ini[TWA_Spec][0];
//					if( clmin < 10 ) clmin = 10;	//最低レベル 10
					//max level
					cl = 0;
					for( n=255; n>0; n-- ) {
						cl += Bright[n];
						if( cl > bright_n ) break;
					}
					clmax = n;
					cl = (Bin_Ma_l_ini[TWA_Spec][0]+Bin_Ma_l_ini[TWA_Spec][1])/2;
					if( clmax < cl ) clmax = cl;
//					if( clmax < Bin_Ma_l_ini[TWA_Spec][0] ) clmax = Bin_Ma_l_ini[TWA_Spec][0];
					//level
					cl = (clmax-clmin)/10 + 1;
//					if((TWA_Spec==SpecJPW)&&(cl<=4)) return(ERROR);
					Mark_Slevel_s[cn][1] = (clmin+clmax)/2;						//center level
					Mark_Slevel_s[cn][0] = Mark_Slevel_s[cn][1] - cl - 10;		//min level
					Mark_Slevel_s[cn][2] = Mark_Slevel_s[cn][1] + cl;			//max level
					if( Mark_Slevel_s[cn][0] < 10 ) Mark_Slevel_s[cn][0] = 10;
				//Manual
					if( i_mode & INSPECT_STEP ) {
						TWA_gray2bin( Mark_Slevel_s[cn][1], xs, ys, xe-xs+1, ye-ys+1, grayPn, 0 );
//						R_gray2bin( Mark_Slevel_s[cn][1], xs, ys, xe-xs+1, ye-ys+1, Gray_address[GrayP2], Bin_address[BinP0] );
						R_box( xs, ys, xe, ye, GRAPH_REVERSE, SOLID_LINE );
						printf( "Mark %d Bright Lo:%d  Hi:%d  n:%d\n", cn, clmin, clmax, bright_n );
						printf( "Mark %d Level min:%d  max:%d\n", cn, Mark_Slevel_s[cn][0], Mark_Slevel_s[cn][2] );
					}
			}
			if( i_mode & INSPECT_STEP ) {
				WaitMouseClick();
				R_bin_cls(0,0,FxSize-1,FySize-1);
			}

		//1文字毎にレベル設定（１８０°反転）
			for( cn=0; cn<TWA_R_n; cn++ ) {
				//検査範囲設定
					xe = 0 - max( Mark_area_x[i][cn][0], Mark_area_x[i][cn][2] );
					xs = 0 - min( Mark_area_x[i][cn][1], Mark_area_x[i][cn][3] );
					ye = 0 - max( Mark_area_y[i][cn][0], Mark_area_y[i][cn][1] );
					ys = 0 - min( Mark_area_y[i][cn][2], Mark_area_y[i][cn][3] );
					xs += x0;
					xe += x0;
					ys += y0;
					ye += y0;
				//指定エリアのヒストグラムを取る。
					R_get_histgram( xs, ys, xe, ye, Gray_address[grayPn] );
					bright_n = 0;
					for( n=1; n<=255; n++ ) bright_n += Bright[n];
					bright_n = bright_n/5 + 1;
				//標印文字有り無し判断レベル
					//min level
					cl = 0;
					for( n=1; n<255; n++ ) {
						cl += Bright[n];
						if( cl > bright_n ) break;
					}
					clmin = n;
					if( clmin < Bin_Ma_l_ini[TWA_Spec][0] ) clmin = Bin_Ma_l_ini[TWA_Spec][0];
//					if( clmin < 10 ) clmin = 10;	//最低レベル 10
					//max level
					cl = 0;
					for( n=255; n>0; n-- ) {
						cl += Bright[n];
						if( cl > bright_n ) break;
					}
					clmax = n;
					cl = (Bin_Ma_l_ini[TWA_Spec][0]+Bin_Ma_l_ini[TWA_Spec][1])/2;
					if( clmax < cl ) clmax = cl;
//					if( clmax < Bin_Ma_l_ini[TWA_Spec][0] ) clmax = Bin_Ma_l_ini[TWA_Spec][0];
					//level
					cl = (clmax-clmin)/10 + 1;
//					if((TWA_Spec==SpecJPW)&&(cl<=4)) return(ERROR);
					Mark_Slevel_r[cn][1] = (clmin+clmax)/2;						//center level
					Mark_Slevel_r[cn][0] = Mark_Slevel_r[cn][1] - cl - 10;		//min level
					Mark_Slevel_r[cn][2] = Mark_Slevel_r[cn][1] + cl;			//max level
					if( Mark_Slevel_r[cn][0] < 10 ) Mark_Slevel_r[cn][0] = 10;
				//Manual
					if( i_mode & INSPECT_STEP ) {
						TWA_gray2bin( Mark_Slevel_r[cn][1], xs, ys, xe-xs+1, ye-ys+1, grayPn, 0 );
						R_box( xs, ys, xe, ye, GRAPH_REVERSE, SOLID_LINE );
						printf( "Mark R %d Bright Lo:%d  Hi:%d  n:%d\n", cn, clmin, clmax, bright_n );
						printf( "Mark R %d Level min:%d  max:%d\n", cn, Mark_Slevel_r[cn][0], Mark_Slevel_r[cn][2] );
					}
			}
	
			if( i_mode & INSPECT_STEP ) {
				WaitMouseClick();
			}
///////////////////////////////////////////////
//             marking zure                  //
///////////////////////////////////////////////

			mark_left  = x0 + max( Mark_area_x[i][        0][0], Mark_area_x[i][        0][2] );
			mark_right = x0 + min( Mark_area_x[i][TWA_R_n-1][1], Mark_area_x[i][TWA_R_n-1][3] );
			mark_top   = y0 + min( Mark_area_y[i][        0][0], Mark_area_y[i][TWA_R_n-1][1] );
			mark_bottom= y0 + max( Mark_area_y[i][        0][2], Mark_area_y[i][TWA_R_n-1][3] );
			left_length  = mark_left - G2_pos[0] ;
			right_length = G2_pos[1] - mark_right;
			top_length   = mark_top - G2_pos[2];
			bottom_length= G2_pos[3] -mark_bottom;

			if( left_length  >  InspPara.MarkShift[TWA_Spec].nData1 )   mark_zure1 = OK;
			if( right_length >  InspPara.MarkShift[TWA_Spec].nData1 )   mark_zure2 = OK;
			if( top_length    >  InspPara.MarkShift[TWA_Spec].nData2 )   mark_zure3 = OK;
			if( bottom_length >  InspPara.MarkShift[TWA_Spec].nData2 )   mark_zure4 = OK;

			if(( TWA_R_n > 1 )) {                                   //2011.04.19
				if( i_mode & INSPECT_STEP ) {
					TWA_bin_cls( GrayP0, 0, 0, FxSize-1, FySize-1 );
					R_line( mark_left,  ((G2_pos[2]+G2_pos[3])/2)-5, mark_left,  ((G2_pos[2]+G2_pos[3])/2)+5, GRAPH_DRAW, SOLID_LINE );
					R_line( G2_pos[0],  ((G2_pos[2]+G2_pos[3])/2)-5, G2_pos[0],  ((G2_pos[2]+G2_pos[3])/2)+5, GRAPH_DRAW, SOLID_LINE );
					R_line( G2_pos[0],  ((G2_pos[2]+G2_pos[3])/2),   mark_left,  ((G2_pos[2]+G2_pos[3])/2),   GRAPH_DRAW, SOLID_LINE );
					left_lengthM=(left_length * SystemPara.nRateX + 500)/1000;
					sprintf( Comment, "check left :%4dum (%3ddot) ( > %4dum)",left_lengthM,left_length,InspPara.MarkShift[TWA_Spec].nData1 );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );

					R_line( mark_right, ((G2_pos[2]+G2_pos[3])/2)-5, mark_right, ((G2_pos[2]+G2_pos[3])/2)+5, GRAPH_DRAW, SOLID_LINE );
					R_line( G2_pos[1],  ((G2_pos[2]+G2_pos[3])/2)-5, G2_pos[1],  ((G2_pos[2]+G2_pos[3])/2)+5, GRAPH_DRAW, SOLID_LINE );
					R_line( G2_pos[1],  ((G2_pos[2]+G2_pos[3])/2),   mark_right, ((G2_pos[2]+G2_pos[3])/2),   GRAPH_DRAW, SOLID_LINE );
					right_lengthM=(right_length*SystemPara.nRateX + 500)/1000;
					sprintf( Comment, "check right :%4dum (%3ddot) ( > %4dum)",right_lengthM,right_length,InspPara.MarkShift[TWA_Spec].nData1 );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );

					top_lengthM   =(top_length    * SystemPara.nRateX + 500)/1000;
					bottom_lengthM=(bottom_length * SystemPara.nRateX + 500)/1000;
					sprintf( Comment, "check top :%4dum (%3ddot) bottom :%4dum (%3ddot)  ( > %4dum)",top_lengthM,top_length,bottom_lengthM,bottom_length,InspPara.MarkShift[TWA_Spec].nData2 );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				}
				if((mark_zure1 !=OK) || (mark_zure2 !=OK))	return ERROR;
				if((mark_zure3 !=OK) || (mark_zure4 !=OK))	return ERROR;
			}

			///////////////////////////////////////////////
			///////////////////////////////////////////////
			if(  i_mode & INSPECT_STEP  ) {
				WaitMouseClick();
			}

		return(OK);
	}


/*********************************************************************************
	関数名：	ＴＷＡ＿Ｍａｒｋ＿Ｊｕｄｇｅ（ void ）
	機　能：　	文字判定
	形　式：	int TWA_Mark_Judge( void )
	引き数：	なし
	戻り値：	値		定　数		意　味
				0		OK			正常
				-1		ERROR		異常終了しました。
	解　説：	文字の判定を行う。
	留意事項：	
*********************************************************************************/

static int		TWA_Mark_Judge( void )
	{
		int				c1st, c1re, c1sum;
		int				c2st_t, c2re_t, c2sum_t;
		int				c2st_w, c2re_w, c2sum_w;
		int				c2st_b, c2re_b, c2sum_b;
		int				nc, nm, nx, ny, nd, ncr;
		int				pt[10];
		int				max_st[5][2], max_re[5][2];
		int				judge;

		int				so_st[5][12][4], so_re[5][12][4];
		int				j, i, l0, l1, l2, l3;
		int				jm;

		int				jm101st, jm101re, n_jm101;
		int				insp_st, insp_re;

		int				insp_mode;
		int				i_mode	= R_CS_GetInspectMode();
		int				grayPn	= InspGrayPn;

			if( TWA_Insp_mode != Insp_Pinhole ) judge = 60;
			else								judge = 80; //

			//文字計測値初期化
				insp_mode = Mark_Insp_mode;
				for( nc=0; nc<4; nc++ ) {
					if((insp_mode & 0x01) == 0 ) {
						Mark_L_set[nc] = 0;
					} else {
						Mark_L_set[nc] = 1;
					}
					insp_mode >>= 1;
				}
			//検査文字指定
				if( TWA_R_n == 3 ) {
					if( MarkChar[1] == 'R' ) Mark_L_set[2] = 1;
					else                     Mark_L_set[1] = 1;
				} else {
					if( MarkChar[1] == 'R' ) {
						Mark_L_set[2] = 1;
						Mark_L_set[3] = 1;
					} else if( MarkChar[2] == 'R' ) {
						Mark_L_set[1] = 1;
						Mark_L_set[3] = 1;
					} else {
						Mark_L_set[1] = 1;
						Mark_L_set[2] = 1;
					}
				}

			for( nc=0; nc<TWA_R_n; nc++ ) {
                for( nm=0; nm<12; nm++ ) {
					c1st = c1re = c1sum = 0;
					c2st_t = c2re_t = c2sum_t = 0;
					c2st_w = c2re_w = c2sum_w = 0;
					c2st_b = c2re_b = c2sum_b = 0;
			
					for( ny=0; ny<Mark_div_y; ny++ ) {
						for( nx=0; nx<Mark_div_x; nx++ ) {
							nd = ny*Mark_div_x + nx;
							pt[0] = nc;												//桁
							pt[1] = (Mark_Check[nm][nd]>>TWA_Mark_co ) & 0x0f;	//Color
							pt[3] = nx;												//Check 1
							pt[4] = ny;												//Check 1
							//Check 1
							pt[2] = (Mark_Check[nm][nd]>>TWA_Mark_ch1) & 0x0f;	//Check1
							pt[5] = (Mark_Check[nm][nd]>>TWA_Mark_oX1) & 0x0f;	//or X1
							pt[6] = (Mark_Check[nm][nd]>>TWA_Mark_oY1) & 0x0f;	//or Y1
							if( pt[2] != 0 ) TWA_Mark_div_check( pt, &c1st, &c1re, &c1sum );
							//Check 2
							pt[2] = (Mark_Check[nm][nd]>>TWA_Mark_ch2) & 0x0f;	//Check2
							pt[5] = (Mark_Check[nm][nd]>>TWA_Mark_oX2) & 0x0f;	//or X2
							pt[6] = Mark_Check[nm][nd] & 0x0f;					//or Y2
							if( pt[2] != 0 ) {
								if( pt[1] == 0 ) {
									TWA_Mark_div_check( pt, &c2st_b, &c2re_b, &c2sum_b );
								} else {
									TWA_Mark_div_check( pt, &c2st_w, &c2re_w, &c2sum_w );
								}
							}
						}
				}

					c2st_t  = c2st_w  + c2st_b;
					c2re_t  = c2re_w  + c2re_b;
					c2sum_t = c2sum_w + c2sum_b;
					Mark_Judge_st[nc][nm][0] = c2st_t*100/c2sum_t;
					Mark_Judge_re[nc][nm][0] = c2re_t*100/c2sum_t;
					Mark_Judge_st[nc][nm][1] = OK;
					Mark_Judge_re[nc][nm][1] = OK;
					Mark_Judge_st[nc][nm][2] = c1st*100/c1sum;
					Mark_Judge_re[nc][nm][2] = c1re*100/c1sum;
					Mark_Judge_st[nc][nm][3] = c2st_w*100/c2sum_w;
					Mark_Judge_re[nc][nm][3] = c2re_w*100/c2sum_w;
					Mark_Judge_st[nc][nm][4] = c2st_b*100/c2sum_b;
					Mark_Judge_re[nc][nm][4] = c2re_b*100/c2sum_b;
				}
				TWA_Mark_error( nc );
			}

			for( nc=0; nc<TWA_R_n; nc++ ) {
                for( nm=0; nm<12; nm++ ) {
					if( Mark_Judge_st[nc][nm][0] == 100 ) {
						if( Mark_Judge_st[nc][nm][1] == OK ) {
							Mark_J100_st[nc] = nm;
						}
					}
					if( Mark_Judge_re[nc][nm][0] == 100 ) {
						if( Mark_Judge_re[nc][nm][1] == OK ) {
							Mark_J100_re[nc] = nm;
						}
					}
				}
			}


				for( nc=0; nc<TWA_R_n; nc++ ) {
					max_st[nc][0] = max_re[nc][0] = 0;
					max_st[nc][1] = max_re[nc][1] = ERROR;

					if( Mark_J100_st[nc] != ERROR ) {
						max_st[nc][0] = 101;
						max_st[nc][1] = Mark_J100_st[nc];
					}
					if( Mark_J100_re[nc] != ERROR ) {
						max_re[nc][0] = 101;
						max_re[nc][1] = Mark_J100_re[nc];
					}

                    for( nm=0; nm<12; nm++ ) {
						so_st[nc][nm][0] = nm;
						so_st[nc][nm][1] = Mark_Judge_st[nc][nm][0];
						so_st[nc][nm][2] = Mark_Judge_st[nc][nm][1];
						so_st[nc][nm][3] = Mark_Judge_st[nc][nm][2];
						so_re[nc][nm][0] = nm;
						so_re[nc][nm][1] = Mark_Judge_re[nc][nm][0];
						so_re[nc][nm][2] = Mark_Judge_re[nc][nm][1];
						so_re[nc][nm][3] = Mark_Judge_re[nc][nm][2];
						if( Mark_Judge_st[nc][nm][1] == OK ) {
							if( max_st[nc][0] < Mark_Judge_st[nc][nm][0] ) {
								max_st[nc][0] = Mark_Judge_st[nc][nm][0];
								max_st[nc][1] = nm;
							}
						}
						if( Mark_Judge_re[nc][nm][1] == OK ) {
							if( max_re[nc][0] < Mark_Judge_re[nc][nm][0] ) {
								max_re[nc][0] = Mark_Judge_re[nc][nm][0];
								max_re[nc][1] = nm;
							}
						}
					}
				}
						
				//***** 9499 START
				// Mark choice // MIA
					for( nc=0; nc<TWA_R_n; nc++ ) {
						// --- check matching rate
						if( max_st[nc][0] > judge ) {
							nm = max_st[nc][1];
							MarkChoice_st[nc][nm]++;
						}
						// --- check matching rate
						if( max_re[nc][0] > judge ) {
							nm = max_re[nc][1];
							MarkChoice_re[nc][nm]++;
						}
					} // MIA
//***** 9499 END

				for( nc=0; nc<TWA_R_n; nc++ ) {
                    for( i=0; i<11; i++ ) {
                        for( j=i+1; j<=11; j++ ) {
							if( so_st[nc][j][1] > so_st[nc][i][1] ) {
								l0 = so_st[nc][j][0];
								l1 = so_st[nc][j][1];
								l2 = so_st[nc][j][2];
								l3 = so_st[nc][j][3];
								so_st[nc][j][0] = so_st[nc][i][0];
								so_st[nc][j][1] = so_st[nc][i][1];
								so_st[nc][j][2] = so_st[nc][i][2];
								so_st[nc][j][3] = so_st[nc][i][3];
								so_st[nc][i][0] = l0;
								so_st[nc][i][1] = l1;
								so_st[nc][i][2] = l2;
								so_st[nc][i][3] = l3;
							}
						}
					}
                    for( i=0; i<11; i++ ) {
                        for( j=i+1; j<=11; j++ ) {
							if( so_re[nc][j][1] > so_re[nc][i][1] ) {
								l0 = so_re[nc][j][0];
								l1 = so_re[nc][j][1];
								l2 = so_re[nc][j][2];
								l3 = so_re[nc][j][3];
								so_re[nc][j][0] = so_re[nc][i][0];
								so_re[nc][j][1] = so_re[nc][i][1];
								so_re[nc][j][2] = so_re[nc][i][2];
								so_re[nc][j][3] = so_re[nc][i][3];
								so_re[nc][i][0] = l0;
								so_re[nc][i][1] = l1;
								so_re[nc][i][2] = l2;
								so_re[nc][i][3] = l3;
							}
						}
					}
				}

//#ifdef	Mark_DEBUG

            if( i_mode & INSPECT_STEP ) {
                int posi_res_x,posi_res_y;
                R_display_control( BIN_MEMORY | GRAY_MEMORY );
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
				for( nc=0; nc<TWA_R_n; nc++ ) {
					if( TWA_Spec != SpecJPW ) {
						posi_res_x = nc * 31;
						posi_res_y = 30;
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"Rst : %d                          ", nc );
						R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+1 ,"%2d:%3d(%2d)%2d %2d:%3d(%2d)%2d",
									so_st[nc][0][0], so_st[nc][0][1], so_st[nc][0][2], so_st[nc][0][3],
									so_st[nc][1][0], so_st[nc][1][1], so_st[nc][1][2], so_st[nc][1][3]);
						R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+2 ,"%2d:%3d(%2d)%2d %2d:%3d(%2d)%2d",
									so_st[nc][2][0], so_st[nc][2][1], so_st[nc][2][2], so_st[nc][2][3],
									so_st[nc][3][0], so_st[nc][3][1], so_st[nc][3][2], so_st[nc][3][3]);
						R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+3 ,"%2d:%3d(%2d)%2d %2d:%3d(%2d)%2d",
									so_st[nc][4][0], so_st[nc][4][1], so_st[nc][4][2], so_st[nc][4][3],
									so_st[nc][5][0], so_st[nc][5][1], so_st[nc][5][2], so_st[nc][5][3]);
						R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+4 ,"%2d:%3d(%2d)%2d %2d:%3d(%2d)%2d",
									so_st[nc][6][0], so_st[nc][6][1], so_st[nc][6][2], so_st[nc][6][3],
									so_st[nc][7][0], so_st[nc][7][1], so_st[nc][7][2], so_st[nc][7][3]);
						R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+5 ,"%2d:%3d(%2d)%2d %2d:%3d(%2d)%2d",
									so_st[nc][8][0], so_st[nc][8][1], so_st[nc][8][2], so_st[nc][8][3],
									so_st[nc][9][0], so_st[nc][9][1], so_st[nc][9][2], so_st[nc][9][3]);
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+6 ,"%2d:%3d(%2d)%2d %2d:%3d(%2d)%2d",
                                    so_st[nc][10][0], so_st[nc][10][1], so_st[nc][10][2], so_st[nc][10][3],
                                    so_st[nc][11][0], so_st[nc][11][1], so_st[nc][11][2], so_st[nc][11][3]);
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+7 ,"max:%2d(%3d)",
									max_st[nc][1], max_st[nc][0] );


						ncr = TWA_R_n - nc - 1;
						posi_res_x = ncr * 31;
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+8 ,"Rre : %d",ncr);
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+9 ,"%2d:%3d(%2d)%2d %2d:%3d(%2d)%2d",
									so_re[ncr][0][0], so_re[ncr][0][1], so_re[ncr][0][2], so_re[ncr][0][3],
									so_re[ncr][1][0], so_re[ncr][1][1], so_re[ncr][1][2], so_re[ncr][1][3]);
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+10 ,"%2d:%3d(%2d)%2d %2d:%3d(%2d)%2d",
									so_re[ncr][2][0], so_re[ncr][2][1], so_re[ncr][2][2], so_re[ncr][2][3],
									so_re[ncr][3][0], so_re[ncr][3][1], so_re[ncr][3][2], so_re[ncr][3][3]);
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+11 ,"%2d:%3d(%2d)%2d %2d:%3d(%2d)%2d",
									so_re[ncr][4][0], so_re[ncr][4][1], so_re[ncr][4][2], so_re[ncr][4][3],
									so_re[ncr][5][0], so_re[ncr][5][1], so_re[ncr][5][2], so_re[ncr][5][3]);
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+12 ,"%2d:%3d(%2d)%2d %2d:%3d(%2d)%2d",
									so_re[ncr][6][0], so_re[ncr][6][1], so_re[ncr][6][2], so_re[ncr][6][3],
									so_re[ncr][7][0], so_re[ncr][7][1], so_re[ncr][7][2], so_re[ncr][7][3]);
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+13 ,"%2d:%3d(%2d)%2d %2d:%3d(%2d)%2d",
									so_re[ncr][8][0], so_re[ncr][8][1], so_re[ncr][8][2], so_re[ncr][8][3],
									so_re[ncr][9][0], so_re[ncr][9][1], so_re[ncr][9][2], so_re[ncr][9][3]);
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+14 ,"%2d:%3d(%2d)%2d %2d:%3d(%2d)%2d",
                                    so_re[ncr][10][0], so_re[ncr][10][1], so_re[ncr][10][2], so_re[ncr][10][3],
                                    so_re[ncr][11][0], so_re[ncr][11][1], so_re[ncr][11][2], so_re[ncr][11][3]);
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+15 ,"max:%2d(%3d)",
									max_re[ncr][1], max_re[ncr][0] );

					}
				}
				WaitMouseClick();
				R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );

			}
//#endif

			jm101st = jm101re = n_jm101 = 0;
			if( TWA_Spec != SpecJPW ) {
				for( nc=0; nc<TWA_R_n; nc++ ) {
					if( Mark_L_set[nc] != 0 ) {
						if( Mark_J100_st[nc] != ERROR ) jm101st++;
						if( Mark_J100_re[nc] != ERROR ) jm101re++;
						n_jm101++;
					}
				}
			}

			if((Mark_Insp_mode==0)&&(TWA_Insp_level==Insp_Low)) {
				insp_st = insp_re = OK;
			} else {
				if((jm101re!=n_jm101)||(jm101st==n_jm101)) {
					insp_st = OK;
				} else {
					insp_st = ERROR;
				}
				if((jm101st!=n_jm101)||(jm101re==n_jm101)) {
					insp_re = OK;
				} else {
					insp_re = ERROR;
				}
			}

			if( insp_st == OK ) {
				for( nc=0; nc<TWA_R_n; nc++ ) {
					jm = TWA_R_data[nc];
					if( Mark_L_set[nc] != 0 ) {
						if( TWA_R_data[nc] != max_st[nc][1] ) break;
						if( max_st[nc][0] < Judge_Mark ) break; //<z4>
						if( Mark_Judge_st[nc][jm][3] < Judge_Mark ) break; //<z4>
					} else {
						if( TWA_R_data[nc] != max_st[nc][1] ) break;
						if( max_st[nc][0] < Judge_Mark ) break; //<z4>
					}
					Mark_Ans_st[nc] = OK;
				}
				if( nc == TWA_R_n ) return(OK);
			}

			if( insp_re == OK ) {
				for( nc=0; nc<TWA_R_n; nc++ ) {
					jm = TWA_R_data[nc];
					if( Mark_L_set[nc] != 0 ) {
						if( TWA_R_data[nc] != max_re[nc][1] ) break;
						if( max_re[nc][0] < Judge_Mark ) break; //<z4>
						if( Mark_Judge_re[nc][jm][3] < Judge_Mark ) break; //<z4>
					} else {
						if( TWA_R_data[nc] != max_re[nc][1] ) break;
						if( max_re[nc][0] < Judge_Mark ) break; //<z4>
					}
					Mark_Ans_re[nc] = OK;
				}
				if( nc == TWA_R_n ) return OK;
			}

		return ERROR;

	}	// TWA_Mark_Judge( void ) end

void	TWA_Mark_error( int nc )
	{
		int				nst, nre;
		int				ds1, ds2, dr1, dr2;
		int				nm;
		int				pt[55];
		int				rp = 0;

				for( nm=0; nm<11; nm++ ) {
					Mark_Judge_st[nc][nm][1] = OK;
					Mark_Judge_re[nc][nm][1] = OK;
				}

				//0
					nm = 0;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;	//Bcack
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst!=0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre!=0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2;		pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 4 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 4 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2;		pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 4 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 4 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y-1;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y-1;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y-2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds2, &dr2 );
					if((ds1==0)&&(ds2==1)) Mark_Judge_st[nc][nm][1] = ERROR;
					if((dr1==0)&&(dr2==1)) Mark_Judge_re[nc][nm][1] = ERROR;

//9499-START
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;	
					pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;	
					pt[rp++] = 0;	pt[rp++] =  Mark_div_y-1;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] =  Mark_div_y-1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] =  Mark_div_y-1;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;	
					pt[rp++] = 1;	pt[rp++] =  0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] =  Mark_div_y-1;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst!=2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre!=2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;	
					pt[rp++] = 1;	pt[rp++] =  1;						pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] =  Mark_div_y-2;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst!=2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre!=2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;	
					pt[rp++] = 0;	pt[rp++] =  0;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] =  1;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] =  2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;	
					pt[rp++] = 0;	pt[rp++] =  4;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] =  5;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] =  6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;	
					pt[rp++] = 2;	pt[rp++] =  0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] =  1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] =  2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;	
					pt[rp++] = 2;	pt[rp++] =  4;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] =  5;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] =  6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;	
					pt[rp++] = 0;	pt[rp++] =  2;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] =  3;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] =  4;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;	
					pt[rp++] = 2;	pt[rp++] =  2;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] =  3;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] =  4;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
//9499-END

					//0 End
				//1
					nm = 1;
/*******************
					//Mark L Size
					ds1 = dr1 = FxSize;	ds2 = dr2 = 0;
					for( n=1; n<5; n++ ) {
						ds3 = (Mark_size_ls[nc][n][0] + Mark_size_ls[nc][n+1][0])/2;
						ds1 = min( ds1, ds3 );
						ds2 = max( ds2, ds3 );
						dr3 = (Mark_size_lr[nc][n][0] + Mark_size_lr[nc][n+1][0])/2;
						dr1 = min( dr1, dr3 );
						dr2 = max( dr2, dr3 );
					}
					ds3 = ds2-ds1;
					dr3 = dr2-dr1;
					if( ds3 >= 5 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( dr3 >= 5 ) Mark_Judge_re[nc][nm][1] = ERROR;

					ds1 = dr1 = FxSize;	ds2 = dr2 = 0;
					for( n=1; n<5; n++ ) {
						ds3 = (Mark_size_ls[nc][n][1] + Mark_size_ls[nc][n+1][1])/2;
						ds1 = min( ds1, ds3 );
						ds2 = max( ds2, ds3 );
						dr3 = (Mark_size_lr[nc][n][1] + Mark_size_lr[nc][n+1][1])/2;
						dr1 = min( dr1, dr3 );
						dr2 = max( dr2, dr3 );
					}
					ds3 = ds2-ds1;
					dr3 = dr2-dr1;
					if( ds3 >= 5 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( dr3 >= 5 ) Mark_Judge_re[nc][nm][1] = ERROR;
*******************/

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst <= 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre <= 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst <= 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre <= 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst > 3 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre > 3 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst <= 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre <= 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst > 3 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre > 3 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

//9499-MARKING START
					rp = 0;
			 		pt[rp++] = nc;	pt[rp++] = 1;
			 		pt[rp++] = 0;	pt[rp++] = Mark_div_y-1;	pt[rp++] = 1;
			 		pt[rp++] = 1;	pt[rp++] = Mark_div_y-1;	pt[rp++] = 1;
			 		pt[rp++] = 2;	pt[rp++] = Mark_div_y-1;	pt[rp++] = 1;
			 		pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
			 		TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;				
					pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst <= 4 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre <= 4 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
				
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
//9499-MARKING END
					//1 End

				//2
					nm = 2;
					rp = 0;
					ds2 = dr2 = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					if( ds1 > 0 ) ds2++;
					if( dr1 > 0 ) dr2++;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					if( ds1 > 0 ) ds2++;
					if( dr1 > 0 ) dr2++;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					if( ds1 > 0 ) ds2++;
					if( dr1 > 0 ) dr2++;
					if( ds2 < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( dr2 < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					ds2 = dr2 = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y-1;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y-2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					if( ds1 > 0 ) ds2++;
					if( dr1 > 0 ) dr2++;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y-1;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y-2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					if( ds1 > 0 ) ds2++;
					if( dr1 > 0 ) dr2++;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y-1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y-2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					if( ds1 > 0 ) ds2++;
					if( dr1 > 0 ) dr2++;
					if( ds2 < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( dr2 < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
//9499-MARKING START
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
	
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 3 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 3 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 3 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 3 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 0;					
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 8 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 8 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;					
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 1;					
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

//9499-MARKING
					//2 End
				//3
					nm = 3;
					rp = 0;
					ds2 = dr2 = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y-1;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y-2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					if( ds1 > 0 ) ds2++;
					if( dr1 > 0 ) dr2++;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y-1;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y-2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					if( ds1 > 0 ) ds2++;
					if( dr1 > 0 ) dr2++;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y-1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y-2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					if( ds1 > 0 ) ds2++;
					if( dr1 > 0 ) dr2++;
					if( ds2 < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( dr2 < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					if((ds1==0)&&(ds2!=3)) Mark_Judge_st[nc][nm][1] = ERROR;
					if((dr1==0)&&(dr2!=3)) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds2, &dr2 );
					if((ds1==0)&&(ds2<5)) Mark_Judge_st[nc][nm][1] = ERROR;
					if((dr1==0)&&(dr2<5)) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst <= 1 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre <= 1 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst <= 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre <= 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					rp = 0;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst <= 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre <= 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					rp = 0;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst <= 1 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre <= 1 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst <= 1 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre <= 1 ) Mark_Judge_re[nc][nm][1] = ERROR;

//9499-MARKING START
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

//9499-MARKING END

					//3 End
				//4
					nm = 4;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
//9499-MARKING START
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
						
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
						
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;
						pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
						pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
																	
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;
						pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
						pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 1;
						pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;
						pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre == 0) Mark_Judge_re[nc][nm][1] = ERROR;

//9499-MARKING END
					//4 End
				//5
					nm = 5;
					rp = 0;
					ds2 = dr2 = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					if( ds1 > 0 ) ds2++;
					if( dr1 > 0 ) dr2++;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					if( ds1 > 0 ) ds2++;
					if( dr1 > 0 ) dr2++;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					if( ds1 > 0 ) ds2++;
					if( dr1 > 0 ) dr2++;
					if( ds2 < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( dr2 < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					ds2 = dr2 = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y-1;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y-2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					if( ds1 > 0 ) ds2++;
					if( dr1 > 0 ) dr2++;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y-1;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y-2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					if( ds1 > 0 ) ds2++;
					if( dr1 > 0 ) dr2++;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y-1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y-2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					if( ds1 > 0 ) ds2++;
					if( dr1 > 0 ) dr2++;
					if( ds2 < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( dr2 < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst <= 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre <= 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					rp = 0;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

//9499-MARKING START
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 1;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
						
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;
						pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
						pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
			
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;
						pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
						pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
						
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;
						pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
						pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
	
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;
						pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
						
//9499-MARKING END

					//5 End
				//6
					nm = 6;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;	
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
//9499-MARKING START

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
				
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 0;				
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 1;				
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 0;				
					pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 0;	
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 3 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 3 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
//2021.06.09<shio>
					rp = 0;												//same as "5"
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
//9499-MARKING END
					//6 End
				//7
					nm = 7;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds2, &dr2 );
					if((ds1<2)&&(ds2<2)) Mark_Judge_st[nc][nm][1] = ERROR;
					if((dr1<2)&&(dr2<2)) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

//9499-MARKING START
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 3 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 3 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					
					
//9499-MARING END

					//7 End
				//8
					nm = 8;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

//9499-MARKING START
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
	
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
//2021.06.09<shio>
					rp = 0;													//same as "6"
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					rp = 0;													//same as "9"
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					rp = 0;													//same as "2"
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
//9499-MARKING END

					//8 End
				//9
					nm = 9;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
			
//9499-MARKING START
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
			
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;	
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
//2021.06.09<shio>
					rp = 0;													//same as "3"
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					rp = 0;													//same as "5"
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
//9499-MARKING END			
			
					//9 End
					
				//R
					nm = 10;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
//9499-MARKING START
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
		
		
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 1 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 1 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
//9499-MARKING END


					//R End

	}

/*******************************************************************************
	関数名：	ＴＷＡ＿ｇｅｔ＿ｃｏｌｏｒ＿ａｒｅａ
	機　能：	範囲内の白／黒を数える
	形　式：	void TWA_get_color_area( int page, int pt[], int *jw, int *jb )
	引き数：	page		バイナリプレーン番号
				pt[0]		計測範囲中心座標 xc
				pt[1]		計測範囲中心座標 yc
				pt[2]		白判定level
				pt[3]		黒判定level
				jb			黒画素数格納ポインタ
				jw			白画素数格納ポインタ
	戻り値：	ありません。結果は指定ポインタに格納されます。
********************************************************************************/
/*
static void	TWA_get_color_area( int page, int pt[], int *jw, int *jb )
	{
		int				xs, ys, xe, ye;
		int				lw, lb;
		int				rx, ry, rl;
		unsigned char	*ad;
		int				nw, nb, nt, nj;

			xs = xe = pt[0];
			ys = ye = pt[1];
			lw = pt[2];
			lb = pt[3];

			if( TWA_R_n == 3 ) {
				xs -= 5;
				xe += 4;
				ys -= 5;
				ye += 4;
			} else {
				xs -= 5;
				xe += 4;
				ys -= 5;
				ye += 4;
			}

			nw = nb = nt = 0;
			for( ry=ys; ry<=ye; ry++ ) {
				ad = Gray_address[page]+ry*MxSize;
				for( rx=xs; rx<=xe; rx++ ) {
					rl = *(ad+rx);
					if( rl >= lw ) nw++;
					if( rl <= lb ) nb++;
					nt++;
				}
			}
			rx = pt[0];
			for( ry=ys; ry<=ye; ry++ ) {
				ad = Gray_address[page]+ry*MxSize;
				rl = *(ad+rx);
				if( rl >= lw ) nw++;
				if( rl <= lb ) nb++;
//				nt++;
			}
			ry = pt[1];
			ad = Gray_address[page]+ry*MxSize;
			for( rx=xs; rx<=xe; rx++ ) {
				rl = *(ad+rx);
				if( rl >= lw ) nw++;
				if( rl <= lb ) nb++;
//				nt++;
			}

			nj = nt/2;
			if( nw >= nj ) *jw = 1;
			else           *jw = 0;
			if( nb <= nj ) *jb = 1;
			else           *jb = 0;

	}
*/
/************************************************
	抵抗値設定
************************************************/

void TWA_mark_set_l18( void )
	{
		int						n;
		int						area;

		TWA_R_data[0]=TWA_R_data[1]=TWA_R_data[2]=TWA_R_data[3]=0;

		for( n=0; n<4; n++ ) {
			Comment[0] = MarkChar[n]; Comment[1] = 0;
			if( MarkChar[n] == ' ' ) break;
			if( MarkChar[n] == 'R' ) TWA_R_data[n] = 10;
            else if( MarkChar[n] == 'L' ) TWA_R_data[n] = 11;
            else TWA_R_data[n] = atoi( Comment );
		}

		TWA_R_n = n;
		area = 0;
//		TWA_ver_title_set();

		TWA_mark_position_set();
}

/************************************************
	標印検査ポイント設定
************************************************/

static void	TWA_mark_position_set( void )
	{
	int						markDigitPosX[4];
	int						ln, dn;
	int						xmin, xmax;
	int						ymin, ymax;
	int						deg;
	
	
		int			n, n1, n2, i, deg10;	//, area;
		int			x, y;
//		int			offset_x[4] = {0,0,0,0};
//		int			l[4] = {0,0,0,0};
//		int			w = 0;
		int			moji;
		double		zoomx;
		double		zoomy;
		int			i_mode	= R_CS_GetInspectMode();

		int				x1, x2, x3, x4, x5;
		int				div_n;

		// Initial
		TWA_point_cls();

		zoomx = (double)1000/(double)SystemPara.nRateX;
		zoomy = (double)1000/(double)SystemPara.nRateY;

		// Mark digit position and size set
		if( TWA_R_n == 3 ) {
			Mark_area_xl = MarkAreaSize3[DataX]*zoomx;
			Mark_area_yl = MarkAreaSize3[DataY]*zoomy;
			for( dn=0; dn<4; dn++ )	markDigitPosX[dn] = MarkDigitPosX3[dn];
		} else {
			Mark_area_xl = MarkAreaSize4[DataX]*zoomx;
			Mark_area_yl = MarkAreaSize4[DataY]*zoomy;
			for( dn=0; dn<4; dn++ )	markDigitPosX[dn] = MarkDigitPosX4[dn];
		}

		//標印文字分割数
			Mark_div_x = 3;
			Mark_div_y = 7;

		// Mark Area data set
		//文字エリア
			deg = 60;
			for( ln=0; ln<TWA_R_n; ln++ ) {
				// X
					xmin = (markDigitPosX[ln]-MarkLetterSize[DataX]/2)*zoomx;
					xmax = (markDigitPosX[ln]+MarkLetterSize[DataX]/2)*zoomx;
						Mark_area_x[deg][ln][0] = Mark_area_x[deg][ln][2] = xmin; 		// x1, x3
						Mark_area_x[deg][ln][3] = Mark_area_x[deg][ln][1] = xmax;		// x2, x4
				// Y
					ymin = (0-MarkLetterSize[DataY]/2)*zoomy;
					ymax = (MarkLetterSize[DataY]/2)*zoomy;
						Mark_area_y[deg][ln][0] = Mark_area_y[deg][ln][1] = ymin;		// y1, y2
						Mark_area_y[deg][ln][2] = Mark_area_y[deg][ln][3] = ymax;		// y3, y4
			}
	
			// 文字エリアデータ回転
				for( deg10=-300, i=0; deg10<=300; deg10+=5, i++ ) {
					for( n=0; n<TWA_R_n; n++ ) {
						for( n1=0; n1<4; n1++ ) {
							// 検査座標設定
							x = Mark_area_x[60][n][n1];
							y = Mark_area_y[60][n][n1];
							if( deg10!=0 ) {
								TWA_kaiten( &x, &y, deg10 );
								Mark_area_x[i][n][n1] = x;
								Mark_area_y[i][n][n1] = y;
							}
						}
					}
				}


		//文字検査データ
			i = 60;
			div_n = Mark_div_x * Mark_div_y;
			for( n=0; n<TWA_R_n; n++ ) {
				moji = TWA_R_data[n];
				for( n1=0; n1<div_n; n1++ ) {
					Mark_rx[i][n][n1] = ( Mark_point[moji][n1][0]+markDigitPosX[n] ) * zoomx;
					Mark_ry[i][n][n1] = Mark_point[moji][n1][1] * zoomy;
				}
			}
			//文字データ回転
			for( deg10=-300, i=0; deg10<=300; deg10+=5, i++ ) {
				for( n=0; n<TWA_R_n; n++ ) {
					for( n1=0; n1<div_n; n1++ ) {
						//検査座標設定
						x = Mark_rx[60][n][n1];
						y = Mark_ry[60][n][n1];
						if( deg10!=0 ) {
							TWA_kaiten( &x, &y, deg10 );
							Mark_rx[i][n][n1] = x;
							Mark_ry[i][n][n1] = y;
						}
					}
				}
			}

		//文字間データ
			i = 60;
			div_n = Mark_div_x * Mark_div_y;
			switch( TWA_R_n ) {
				case 3:
					x = (Mark_area_x[i][1][0]-Mark_area_x[i][0][1])/2;
					x1 = Mark_area_x[i][0][0] - x;
					x2 = Mark_area_x[i][0][1] + x;
					x = (Mark_area_x[i][2][0]-Mark_area_x[i][1][1])/2;
					x3 = Mark_area_x[i][1][1] + x;
					x4 = Mark_area_x[i][2][1] + x;
					for( n=0; n<Mark_div_y; n++ ) {
						Mark_space_x[i][0][n] = x1;
						Mark_space_x[i][1][n] = x2;
						Mark_space_x[i][2][n] = x3;
						Mark_space_x[i][3][n] = x4;
					}
					for( n=0; n<4; n++ ) {
						for( n1=0; n1<Mark_div_y; n1++ ) {
							n2 = n1*Mark_div_x;
							Mark_space_y[i][n][n1] = Mark_ry[i][0][n2];
						}
					}
					break;
				case 4:
					x = (Mark_area_x[i][1][0]-Mark_area_x[i][0][1])/2;
					x1 = Mark_area_x[i][0][0] - x;
					x2 = Mark_area_x[i][0][1] + x;
					x = (Mark_area_x[i][2][0]-Mark_area_x[i][1][1])/2;
					x3 = Mark_area_x[i][1][1] + x;
					x = (Mark_area_x[i][3][0]-Mark_area_x[i][2][1])/2;
					x4 = Mark_area_x[i][2][1] + x;
					x5 = Mark_area_x[i][3][1] + x;
					for( n=0; n<Mark_div_y; n++ ) {
						Mark_space_x[i][0][n] = x1;
						Mark_space_x[i][1][n] = x2;
						Mark_space_x[i][2][n] = x3;
						Mark_space_x[i][3][n] = x4;
						Mark_space_x[i][4][n] = x5;
					}
					for( n=0; n<5; n++ ) {
						for( n1=0; n1<Mark_div_y; n1++ ) {
							n2 = n1*Mark_div_x;
							Mark_space_y[i][n][n1] = Mark_ry[i][0][n2];
						}
					}
					break;
			}
			//文字間データ回転
			for( deg10=-300, i=0; deg10<=300; deg10+=5, i++ ) {
				for( n=0; n<(TWA_R_n+1); n++ ) {
					for( n1=0; n1<Mark_div_y; n1++ ) {
						x = Mark_space_x[60][n][n1];
						y = Mark_space_y[60][n][n1];
						if( deg10!=0 ) {
							TWA_kaiten( &x, &y, deg10 );
							Mark_space_x[i][n][n1] = x;
							Mark_space_y[i][n][n1] = y;
						}
					}
				}
			}
			//標印文字判定結果[桁][文字][一致,Check,定点]
			for( n=0; n<5; n++ ) {
                for( n1=0; n1<NumMarkChar; n1++ ) {
					Mark_Judge_st[n][n1][0] = 0;
					Mark_Judge_st[n][n1][1] = ERROR;
					Mark_Judge_st[n][n1][2] = 0;
					Mark_Judge_st[n][n1][3] = 0;
					Mark_Judge_st[n][n1][4] = 0;
					Mark_Judge_re[n][n1][0] = 0;
					Mark_Judge_re[n][n1][1] = ERROR;
					Mark_Judge_re[n][n1][2] = 0;
					Mark_Judge_re[n][n1][3] = 0;
					Mark_Judge_re[n][n1][4] = 0;
				}
			}
	}


//外形検査部

//================================================
//	電極確認
//================================================

static int		TWA_C1_Check( int grayPn, int binPn )
{
	int						xmin, xmax;
	int						ymin, ymax;
	int						t_count, count;
	int						ans;
	int						i_mode	= R_CS_GetInspectMode();
	int						xsize,ysize;
	// Gray to Bin
	xsize = IGuidePos[DataX]-IGuidePos[1]+1;
	ysize = Index_pos[1]-Index_pos[0]+1;
	BinClsPn( binPn, 0, 0, FxSize-1, FySize-1 );
	//２値化（グレープレーンから２値プレーンへ）
	R_gray2bin( IGuideEsLevel.nData, IGuidePos[1], Index_pos[0], xsize, ysize, Gray_address[grayPn], Bin_address[binPn] );
	
	//２値化（グレープレーンから２値プレーンへ）
//	R_gray2bin16( Bin_Chip_l, 0, 0, FxSize, FySize, Gray_address[grayPn], Bin_address[binPn] );
	if( i_mode & INSPECT_STEP ) 	R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
	
	// upper pos.
	ymin = Ap_pos[4];
	ymax = ymin + 20;
	xmax = Index_pos[0] - 20;
	xmin = xmax - (4900/12) + 40;	//LTR_size_x[1] 
	if( xmin < 20 ) xmin = 20;
	t_count = (xmax-xmin+1)*(ymax-ymin+1);
	count = R_count_bin_area( binPn, xmin, ymin, xmax, ymax );
	ans = count*100/t_count;
	// --- view
	if( i_mode & INSPECT_STEP ) {
		R_box( xmin, ymin, xmax, ymax, GRAPH_REVERSE, DASHED_LINE );
		sprintf( Comment, "Upper side C1 = %4d", ans );
		DrawMessage(  Comment, DRAW_GREEN, 2, 2 );
	}
	// --- Judge
	if( ans < 100 )	return ERROR;
	
	// lower pos.
	ymax = Ap_pos[1];
	ymin = ymax - 40;
	xmax = Index_pos[0] - 20;
	xmin = xmax - (4900/12) + 40;	//LTR_size_x[1]
	if( xmin < 20 ) xmin = 20;
	t_count = (xmax-xmin+1)*(ymax-ymin+1);
	count = R_count_bin_area( binPn, xmin, ymin, xmax, ymax );
	ans = count*100/t_count;
	// --- view
	if( i_mode & INSPECT_STEP ) {
		R_box( xmin, ymin, xmax, ymax, GRAPH_REVERSE, DASHED_LINE );
		sprintf( Comment, "Upper side C1 = %4d", ans );
		DrawMessage(  Comment, DRAW_GREEN, 2, 2 );
	}
	// --- Judge
	if( ans < 100 )	return ERROR;
	
	// Judge
	return OK;
	
}	// TWA_C1_Check( void ) end


//================================================
//	外形エッジサーチ Y position
//================================================
/* **********************************************************************************************************
int		LTR_SearchApYpos_l18( int grayPn, int binPn )
{
	int						pt[10];
	int						xmin, xcen, xmax;
	int						ymin, ymax, ry, ysize;
	int						t_count, count;
	int						ans;
	int						xsize;
	int						i_mode	= R_CS_GetInspectMode();
	int						judgeYmin = InspPara.ChipYmin[TWA_Spec].nYsize* 8/10;

	// Search bin level set
		if( ElecEsLevelSet() != OK )	return ERROR;
	// Gray to Bin
		xsize = IGuidePos[DataX]-IGuidePos[1]+1;
		ysize = Index_pos[1]-Index_pos[0]+1;
		BinClsPn( binPn, 0, 0, FxSize-1, FySize-1 );
		//２値化（グレープレーンから２値プレーンへ）
		R_gray2bin( IGuideEsLevel.nData, IGuidePos[1], Index_pos[0], xsize, ysize, Gray_address[grayPn], Bin_address[binPn] );
	//検査エリア外消去
	//	BinClsPn( binPn, 0, 0, FxSize-1, IndexPos[PosYmin]+1 );
	//	BinClsPn( binPn, 0, IndexPos[PosYmax]-1, FxSize-1, FySize-1 );
	//	BinClsPn( binPn, 0, IndexPos[PosYmin]+1, IndexPos[PosXmin], IndexPos[PosYmax]+1 );
	//	BinClsPn( binPn, IndexPos[PosXmax], IndexPos[PosYmin]-1, FxSize-1, IndexPos[PosYmax]+1 );
	//ノイズ除去処理
		pt[0]=0;	pt[1]=0;	pt[2]=FxSize-1;	pt[3]=FySize-1;
		TWA_contrct2( binPn, Ap_contrct_x, Ap_contrct_y, pt );
		TWA_expand2( binPn, Ap_expand_x, Ap_expand_y, pt );
		TWA_expand2( binPn, Ap_expand_x, Ap_expand_y, pt );
		TWA_contrct2( binPn, Ap_contrct_x, Ap_contrct_y, pt );
		if( i_mode & INSPECT_STEP ) {
			R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
			R_line( IGuidePos[1], Index_pos[0], IGuidePos[0], Index_pos[0], GRAPH_REVERSE, SOLID_LINE );
			R_line( IGuidePos[1], Index_pos[1], IGuidePos[0], Index_pos[1], GRAPH_REVERSE, SOLID_LINE );
			R_line( IGuidePos[1], Index_pos[0], IGuidePos[1], Index_pos[1], GRAPH_REVERSE, SOLID_LINE );
			R_line( IGuidePos[0], Index_pos[0], IGuidePos[0], Index_pos[1], GRAPH_REVERSE, SOLID_LINE );
			WaitMouseClick();
		}
	
	// initial
		xmin = 20;
		xmax = IGuidePos[0]-20;
		xcen = (xmin+xmax)/2;
		t_count = (xcen-xmin+1)*2;
	
	// Pos. Y min
	ymin = 20;
	ymax = FySize/2;
	// Search
	for( ry=ymin; ry<ymax; ry++ ) {
		count = R_count_bin_area( binPn, xmin, ry, xcen, ry+1 );
		ans = count*100/t_count;
		if( ans > 70 ) break;
		count = R_count_bin_area( binPn, xcen, ry, xmax, ry+1 );
		ans = count*100/t_count;
		if( ans > 70 ) break;
	}
	if( ry >= ymax )	ry = 0xffff;
	Ap_pos[4] = ry;
	
	// Pos. Y max
	ymax = FySize-20;
	ymin = FySize/2;
	// Search
	for( ry=ymax; ry>ymin; ry-- ) {
		count = R_count_bin_area( binPn, xmin, ry-1, xcen, ry );
		ans = count*100/t_count;
		if( ans > 70 ) break;
		count = R_count_bin_area( binPn, xcen, ry-1, xmax, ry );
		ans = count*100/t_count;
		if( ans > 70 ) break;
	}
	if( ry <= ymin )	ry = 0xffff;
	Ap_pos[5] = ry;
	
	// Check pos. data
	// --- NO chip
	if((Ap_pos[4]==0xffff)&&(Ap_pos[5]==0xffff)) return ERROR;
	// --- can not insp.
	if((Ap_pos[4]==0xffff)||(Ap_pos[5]==0xffff)) return ERROR3;
	
	// Check elec
	if( TWA_C1_Check( grayPn, binPn ) != OK ) return ERROR;
	
	//サーチ位置表示
	if( i_mode & INSPECT_STEP ) {
		BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
		R_line( 0, Index_pos[0], FxSize-1, Index_pos[0], GRAPH_REVERSE, DASHED_LINE );
		R_line( 0, Index_pos[1], FxSize-1, Index_pos[1], GRAPH_REVERSE, DASHED_LINE );
		xmax = IGuidePos[0] - 10;
		xmin = xmax - (4900/12) + 20;  //LTR_size_x[1]
		R_line( xmin, Ap_pos[4], xmax, Ap_pos[4], GRAPH_DRAW, SOLID_LINE );
		R_line( xmin, Ap_pos[5], xmax, Ap_pos[5], GRAPH_DRAW, SOLID_LINE );
		DrawMessage( "search a position of Chip(rough)",DRAW_GREEN, 2, 2 );
	}
	
	// Check Y size
	ysize = Ap_pos[5] - Ap_pos[4];
	if( ysize < judgeYmin )	return ERROR2;
	
	return OK;
	
}	// LTR_SearchApYpos_18( int binPn ) end
*************************************************************************************************************/


/************************************************
	外形エッジサーチ（左）
************************************************/

int		TWA_Ap_search_le_l18( int binPn ,int mode)
	{
		int					ye, xs, xl, xe;
		int					G2_ys, G2_ye;
		int					rx, ry, rl;
		int					n;
		int					sum_x, sum_n;
		int					i_mode	= R_CS_GetInspectMode();
		R_RECT				area;

		if( i_mode & INSPECT_STEP ) {
			 R_move_bin_memory( binPn, 0, 0, 0, FxSize-1, FySize-1 );
		}

		for( n=0; n<80; n++ ) {
			Ap_data_l[n][0] = 0xffff;
			Ap_data_l[n][1] = 0xffff;
		}
		//サーチ条件設定
			ye = Ap_pos[5]-ES_Ap_yp;									//サーチ終了座標

		//外形上認識開始点の設定
			for( ry=0; ry<Ap_pos[4]; ry+=ES_Ap_yp );
			ry+=ES_Ap_yp;
		//G2 Area
			G2_ys = G2_data_l[0][1];// - 10;
			for( n=0; n<80; n++ ) if( G2_data_l[n][1] == 0xffff ) break;
			n--;
			G2_ye = G2_data_l[n][1];// + 10;
			xl = (G2_pos[1]-G2_pos[0])/5;

		//外形エッジサーチ
			sum_x = sum_n = 0;
			for( n=0; n<79; n++, ry+=ES_Ap_yp ) {
				if( ry >= ye ) break;
				xs = rx = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-ry);		//検査スタートＹ座標

				if(mode != 0){
					xe = ElecEdge[DataUL][PosXmin].MeanX-ElecEdge[DataUL][PosXmin].Rev*(ElecEdge[DataUL][PosXmin].MeanY-ry);
				}
				if( R_pstate( rx, ry, binPn ) == KURO ) {		//認識開始点の色判別
					if((ry>=G2_ys)&&(ry<=G2_ye)) {
						//Ｇ２～外
						if(mode == 0){
							if(rx < xl)	rl = rx;
							else		rl = xl;
						} else {
							rl = rx-xe+2;
						}
						if( TWA_es_bin_search( binPn, &rx, &ry, 180, rl, TWA_ES_WHITE ) == ERROR ) {
							if(mode == 0)   rx = xs;
							else            rx = xe;
							Ap_data_l[n][0] = rx;								//外形座標データ保存
							Ap_data_l[n][1] = ry;								//外形座標データ保存
							sum_x += rx;
							sum_n++;
							continue;
						}
					} else {
						//外～Ｇ２
						rl = xl;
						if( TWA_es_bin_search( binPn, &rx, &ry, 0, rl, TWA_ES_WHITE ) == ERROR ) {
							Ap_data_l[n][0] = rx;								//外形座標データ保存
							Ap_data_l[n][1] = ry;								//外形座標データ保存
							sum_x += rx;
							sum_n++;
							continue;
						}
					}
				}
				//Ｇ２～外
				if(mode == 0){
					if(rx < xl-2)	rl = rx;
					else			rl = xl-2;
				} else {
					rl = rx-xe+2;
					if(rl < 30) rl = 30;
					if(rx-rl < 0)  rl=rx;
				}

				if( TWA_es_bin_search( binPn, &rx, &ry, 180, rl, TWA_ES_BLACK ) != ERROR ){
					Ap_data_l[n][0] = rx;								//外形座標データ保存
					Ap_data_l[n][1] = ry;								//外形座標データ保存
					sum_x += rx;
					sum_n++;
				} else {
						Ap_data_l[n][0] = rx;								//外形座標データ保存
						Ap_data_l[n][1] = ElecPosRough[DataUL][PosXmin];	//外形がうまく取れない場合はラフ位置決めの値を当てはめる 20170612 tanaka
						sum_x += xe;
						sum_n++;
				}
			}
			for( ; n<80; n++ ) {
				Ap_data_l[n][0] = 0xffff;
				Ap_data_l[n][1] = 0xffff;
			}

			if( i_mode & INSPECT_STEP ) {
				for( n=0; n<80; n++ ) {
					if( Ap_data_l[n][0] != 0xffff ) {
						rx = Ap_data_l[n][0];
						ry = Ap_data_l[n][1];
						R_box( rx-1, ry-1, rx+1, ry+1, GRAPH_REVERSE, SOLID_LINE );
					}
				}
				WaitMouseClick();
			}

			if( sum_n == 0 ) rx = 0;
			else			 rx = sum_x/sum_n;

		return(rx);

	}


/************************************************
	外形エッジサーチ（右）
************************************************/

int		TWA_Ap_search_ri_l18( int binPn ,int mode)
	{
		int					ye, xs, xe, xl;
		int					G2_ys, G2_ye;
		int					rx, ry, rl;
		int					n;
		int					sum_x, sum_n;
		int					i_mode	= R_CS_GetInspectMode();

		for( n=0; n<80; n++ ) {
			Ap_data_r[n][0] = 0xffff;
			Ap_data_r[n][1] = 0xffff;
		}

		//サーチ条件設定
			ye = Ap_pos[5]-ES_Ap_yp;									//サーチ終了座標
			xe = ElecPosRough[DataUL][PosXmax]+3;					// mode==0のときはこの値
			if(xe > FxSize-1)	xe==FxSize-1;
		//外形上認識開始点の設定
			for( ry=0; ry<Ap_pos[4]; ry+=ES_Ap_yp );
			ry+=ES_Ap_yp;

		//G2 Area
			G2_ys = G2_data_r[0][1] - 10;
			for( n=0; n<80; n++ ) if( G2_data_r[n][1] == 0xffff ) break;
			n--;
			G2_ye = G2_data_r[n][1] + 10;
			xl = (G2_pos[1]-G2_pos[0])/5;

		//外形エッジサーチ
			sum_x = sum_n = 0;
			for( n=0; n<79; n++, ry+=ES_Ap_yp ) {
				if( ry >= ye ) break;
				xs = rx = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-ry);	//検査スタートＸ座標
				xe = (ElecPosRough[DataUpper][PosXmax]+ElecPosRough[DataLower][PosXmax])/2-migi.kaiki_b*(migi.kaiki_ey-ry);
				if(mode != 0){
					xe = ElecEdge[DataUL][PosXmax].MeanX-ElecEdge[DataUL][PosXmax].Rev*(ElecEdge[DataUL][PosXmax].MeanY-ry);
				}

				if( R_pstate( rx, ry, binPn ) == KURO ) {		//認識開始点の色判別
					if((ry>=G2_ys)&&(ry<=G2_ye)) {
					//Ｇ２～外
						rl = xe -rx+2;
						if( TWA_es_bin_search( binPn, &rx, &ry, 0, rl, TWA_ES_WHITE ) == ERROR ) {
							rx = xe;
							Ap_data_r[n][0] = rx;								//外形座標データ保存
							Ap_data_r[n][1] = ry;								//外形座標データ保存
							sum_x += rx;
							sum_n++;
							continue;
						}
					} else {
						//外～Ｇ２
						rl = xl;
						if( TWA_es_bin_search( binPn, &rx, &ry, 180, rl, TWA_ES_WHITE ) == ERROR ) {
							Ap_data_r[n][0] = rx;								//外形座標データ保存
							Ap_data_r[n][1] = ry;								//外形座標データ保存
							sum_x += rx;
							sum_n++;
							continue;
						}
					}
				}
				//Ｇ２～外
				rl = xe - rx+2;
				if(rl < 30) rl = 30;
				if(rx+rl > FxSize-1)  rl=FxSize-1 - rx;
				if( TWA_es_bin_search( binPn, &rx, &ry, 0, rl, TWA_ES_BLACK ) != ERROR ){
					Ap_data_r[n][0] = rx;								//外形座標データ保存
					Ap_data_r[n][1] = ry;								//外形座標データ保存
					sum_x += rx;
					sum_n++;
				} else {
					Ap_data_r[n][0] = rx;								//外形座標データ保存
					Ap_data_r[n][1] = ElecPosRough[DataUL][PosXmax];	//外形がうまく取れない場合はラフ位置決めの値を当てはめる 20170612 tanaka
					sum_x += xe;
					sum_n++;
				}
			}
			for( ; n<80; n++ ) {
				Ap_data_r[n][0] = 0xffff;
				Ap_data_r[n][1] = 0xffff;
			}

			if( i_mode & INSPECT_STEP ) {
				for( n=0; n<80; n++ ) {
					if( Ap_data_r[n][0] != 0xffff ) {
						rx = Ap_data_r[n][0];
						ry = Ap_data_r[n][1];
						R_box( rx-1, ry-1, rx+1, ry+1, GRAPH_REVERSE, SOLID_LINE );
					}
				}
				WaitMouseClick();
			}

			if( sum_n == 0 ) rx= 0;
			else			 rx= sum_x/sum_n;

		return(rx);

	}

/************************************************
	回帰係数（外形）
************************************************/

static int		KAIKI_keisuu_Ap( int mode )
	{
		int				d, n, dn, ds, de;
		int				G2_xs, G2_xe, rx;
		int				os_l = 1;
		float			sum, sumx, sumy;
		float			ex, ey;
		int				i, up_sd, lo_sd;

	//右
		n = sumx = sumy = 0;
		for( d=os_l; n<80; d++, n++ ) {
			if( Ap_data_r[d+os_l][0] == 0xffff ) break;
			sumx += Ap_data_r[d][0];
			sumy += Ap_data_r[d][1];
		}
		if( n == 0 ) return ERROR;
		dn = d;
		ex = sumx/n;		//平均Ｘ
		ey = sumy/n;		//平均Ｙ
		sum = sumy = 0;
		for( d=os_l; d<dn; d++ ) {
			sumy += pow((Ap_data_r[d][1]-ey),2);
			sum  += ( Ap_data_r[d][0]-ex )*( Ap_data_r[d][1]-ey);
		}
		if( sumy == 0 ) return ERROR;
		Ap_ri.kaiki_ex = ex;
		Ap_ri.kaiki_ey = ey;
		Ap_ri.kaiki_b  = sum/sumy;
	//左
		n = sumx = sumy = 0;
		for( d=os_l; n<80; d++, n++ ) {
			if( Ap_data_l[d+os_l][0] == 0xffff ) break;
			sumx += Ap_data_l[d][0];
			sumy += Ap_data_l[d][1];
		}
		if( n == 0 ) return ERROR;
		dn = d;
		ex = sumx/n;		//平均Ｘ
		ey = sumy/n;		//平均Ｙ
		sum = sumy = 0;
		for( d=os_l; d<dn; d++ ) {
			sumy += pow((Ap_data_l[d][1]-ey),2);
			sum  += ( Ap_data_l[d][0]-ex )*( Ap_data_l[d][1]-ey);
		}
		if( sumy == 0 ) return ERROR;
		Ap_le.kaiki_ex = ex;
		Ap_le.kaiki_ey = ey;
		Ap_le.kaiki_b  = sum/sumy;
	//左右平均
		Ap_l.kaiki_ex = ( Ap_ri.kaiki_ex + Ap_le.kaiki_ex )/2;
		Ap_l.kaiki_ey = ( Ap_ri.kaiki_ey + Ap_le.kaiki_ey )/2;
		Ap_l.kaiki_b  = ( Ap_ri.kaiki_b  + Ap_le.kaiki_b )/2;
		return OK;

	}	// KAIKI_keisuu_Ap( int mode ) end

/************************************************
	平均値・標準偏差
************************************************/

static int		TWA_meansd_Ap( void )
	{
		int					d, n, ds, de;
		int					G2_ys, G2_ye, ry;
		float				i, s1, s2;
		int					i_mode	= R_CS_GetInspectMode();

	//左
		//G2 Area
			G2_ys = G2_data_l[0][1];
			for( n=0; n<80; n++ ) if( G2_data_l[n][1] == 0xffff ) break;
			n--;
			G2_ye = G2_data_l[n][1];

		for( d=0; d<80; d++ ) {
			ry = Ap_data_l[d][1];
			if( ry == 0xffff ) return(ERROR);
			if( ry > G2_ys ) {
				ds = d;
				break;
			}
		}
		for( ; d<80; d++ ) {
			ry = Ap_data_l[d][1];
			if( ry == 0xffff ) return(ERROR);
			if( ry > G2_ye ) {
				d--;
				de = d;
				break;
			}
		}

		n=s1=s2=0;
		for( d=ds; d<=de; d++ ) {
			i = Ap_data_l[d][0];
			i -= Ap_le.kaiki_ex-Ap_le.kaiki_b*(Ap_le.kaiki_ey-Ap_data_l[d][1]);
			i -= s1;
			n++;
			s1 += i/n;
			s2 += ( n - 1 ) * i * i / n;
		}
		if( n > 1 ) {
			s2 = sqrt(s2/(n-1));
		} else {
			s2 = 0;
		}
		Ap_le_x.mean = s1;
		Ap_le_x.sd = s2;

	//右
		//G2 Area
			G2_ys = G2_data_r[0][1];
			for( n=0; n<80; n++ ) if( G2_data_r[n][1] == 0xffff ) break;
			n--;
			G2_ye = G2_data_r[n][1];

		for( d=0; d<80; d++ ) {
			ry = Ap_data_r[d][1];
			if( ry == 0xffff ) return(ERROR);
			if( ry > G2_ys ) {
				ds = d;
				break;
			}
		}
		for( ; d<80; d++ ) {
			ry = Ap_data_r[d][1];
			if( ry == 0xffff ) return(ERROR);
			if( ry > G2_ye ) {
				d--;
				de = d;
				break;
			}
		}

		n=s1=s2=0;
		for( d=ds; d<=de; d++ ) {
			i = Ap_data_r[d][0];
			i -= Ap_ri.kaiki_ex-Ap_ri.kaiki_b*(Ap_ri.kaiki_ey-Ap_data_r[d][1]);
			i -= s1;
			n++;
			s1 += i/n;
			s2 += ( n - 1 ) * i * i / n;
		}
		if( n > 1 ) {
			s2 = sqrt(s2/(n-1));
		} else {
			s2 = 0;
		}
		Ap_ri_x.mean = s1;
		Ap_ri_x.sd = s2;

		if( i_mode & INSPECT_STEP ) {
			printf( "Upper Xbar=%g  SD=%g\n", Ap_le_x.mean, Ap_le_x.sd );
			printf( "Lower Xbar=%g  SD=%g\n", Ap_ri_x.mean, Ap_ri_x.sd );
		}

		return(OK);

	}


/************************************************
	外形認識
************************************************/

static int		TWA_kensa_Ap( int binPn ,int mode)
	{
		int					w[80], w_n, w_av, w_sub;
		int					w_min, w_max;
		int					l;
		int					n, n1;
		int					judge_wn = 3;
		int					i_mode	= R_CS_GetInspectMode();

		if( i_mode & INSPECT_STEP ) {
			 R_move_bin_memory( binPn, 0, 0, 0, FxSize-1, FySize-1 );
		}

		Ap_l_ex = TWA_Ap_search_le_l18(binPn,mode);							//外形左検査
		Ap_r_ex = TWA_Ap_search_ri_l18(binPn,mode);							//外形右検査

		//回帰係数計算
			if( KAIKI_keisuu_Ap(0) != OK ) return(ERROR3);
		//平均値・標準偏差
			if( TWA_meansd_Ap() != OK ) return(ERROR3);
		//回帰係数計算
			if( KAIKI_keisuu_Ap(1) != OK ) return(ERROR3);

		return(OK);
	}


/************************************************
	外形認識
************************************************/

int		TWA_Insp_ApLTR_l18( int mode )
	{
		int				grayPn	= InspGrayPn;
		int				binPn	= InspBinPn;
		int				level = g2level-10;		//Bin_Ap_l[TWA_Spec];
		int				l_mode = 0;
		int				l_p = 4;
		int				ans;
		int				pt[4];

		if( (1==bIsLTR50)&&(0!=(Option & BACKINSP_MODE)) || (1==bIsLTR18)&&(0!=(Option & BACKINSP_MODE)) ){			//v2.11 v2.20
			level = g2level-70;
		}else{
			level = g2level-10;
		}
		//２値化（グレープレーンから２値プレーンへ）
			R_gray2bin16( level, 0, 0, TWA_X_size, TWA_Y_size, Gray_address[grayPn], Bin_address[binPn] );
		//ノイズ除去処理
			pt[0]=0;	pt[1]=0;	pt[2]=TWA_X_size-1;	pt[3]=TWA_Y_size-2;
			TWA_expand2( binPn, Ap_expand_x, Ap_expand_y, pt );
			TWA_contrct2( binPn, Ap_contrct_x, Ap_contrct_y, pt );

		//外形検査
		ans = TWA_kensa_Ap(binPn,mode);
		return(ans);
	}


//Ｇ２検査部

/************************************************
	Ｇ２エッジサーチ（左右）
************************************************/
/*
static int		TWA_G2_search_rl( void )
	{
		int				p = 2;
		int				count, t_count, ans;
		int				xs, xe, ys, ye;
		int				rl;
		int				i_mode	= R_CS_GetInspectMode();

		//サーチ条件設定
//			ys = (Index_pos[0]+Index_pos[1])/2-20;
//			ye = (Index_pos[0]+Index_pos[1])/2+20;
			ys = (ElecPosRough[DataLeft][PosYmin]+ElecPosRough[DataLeft][PosYmax])/2;
			ye = ys + 40;
			ys = ys - 40;
			t_count = 2*(ye-ys+1);
			if( t_count <= 0 ) return(ERROR);                            //GEE Program Hang
			rl = FxSize/2;
		//左
			xs = ElecPosRough[DataLeft][PosXmin]+40;
			for( ; xs<rl; xs+=4 ) {
				xe = xs+1;
				count = R_count_bin_area( p, xs, ys, xe, ye );
				ans = count*100/t_count;
				if( ans<40 ) break;
			}
			if( xs >= rl ) return(ERROR);
			G2_pos[0] = xs;
		//右
			xe = ElecPosRough[DataRight][PosXmax]-40;
			for( ; xe>rl; xe-=4 ) {
				xs = xe-1;
				count = R_count_bin_area( p, xs, ys, xe, ye );
				ans = count*100/t_count;
				if( ans<40 ) break;
			}
			if( xe <= rl ) return(ERROR);
			G2_pos[1] = xe;

		//サーチ位置表示
			if( i_mode & INSPECT_STEP ) {
				xs = G2_pos[0];
				xe = G2_pos[1];
				ys = ElecPosRough[DataLR][PosYmin];
				ye = ElecPosRough[DataLR][PosYmax];
				R_line( xs, ys, xs, ye, GRAPH_REVERSE, SOLID_LINE );
				R_line( xe, ys, xe, ye, GRAPH_REVERSE, SOLID_LINE );
			}

		return OK;

	}	// TWA_G2_search_rl( void ) end
*/

static int slice = 500;
static int level = 10;
static int average = 1;
static int noise = 1;
static int degreeL = 180;
static int degreeR = 0;


/************************************************
	Ｇ２エッジサーチ（左）
************************************************/
static int		TWA_G2_search_gray_le( int binPn, int grayPn, int mode )
{
		int					xs, ys, xe, ye;
		int					rx, ry, rl, rx2;
		int					rc;
		int					xl, yl;
		int					x_inc;							//傾き*1000000
		int					n,i;
		int					xo;
		int					i_mode	= R_CS_GetInspectMode();
		R_RECT				area;
		//R_clsd();
		xo = 0;
		if( mode == 0 ) {
			xo = 45;			//       2017.10.30  tanaka
		}

		//傾き計算
		ys = (Ap_pos[4]+G2_pos[2])/2;
		ye = (Ap_pos[5]+G2_pos[3])/2;
		yl = ye-ys;
		if( yl <= 0 ) return(ERROR);                //GEE Program hang
		xl = Ap_pos[2]-Ap_pos[0];
		x_inc = xl*10000000/yl;

		//サーチ条件設定
		ye = G2_pos[3]-4;							//サーチ終了座標
		xl = (Ap_pos[1]-Ap_pos[0])/2;				//サーチ距離 W/2

		//Ｇ２上認識開始点の設定
		ry = ((G2_pos[2]+7)/ES_G2_yp) * ES_G2_yp;		// 8dot 刻みの位置に整列
		ry+=ES_G2_yp;

		for( n=0; n<80; n++ ) {
			G2_data_l[n][0] = 0xffff;
			G2_data_l[n][1] = 0xffff;
		}

		//Ｇ２エッジサーチ
		for( n=0; n<79; n++, ry+=ES_G2_yp ) {
			if( ry >= ye ) break;
			xs = (ry-ys)*x_inc/10000000 + Ap_pos[0];
			rx = xs;

			for(i=0;i<6;i++){
				//R_DrawDispCross( DRAW_RED, rx+i, ry, 2, SOLID_LINE);
				if( R_pstate( rx+i, ry, binPn ) != KURO )	break;		//認識開始点の色判別
			}
			if(i>=6){													//外形から6dot(72um)入っても白（セラミック）が見つからない場合は異常
					G2_data_l[n][0] = rx;								//Ｇ２座標データ保存
					G2_data_l[n][1] = ry;								//Ｇ２座標データ保存
					continue;
			}
			rx += i;													//セラミックが見つかったところから、G2がある位置を探す

			xs = rx;
			xl = 40;
			//外～Ｇ２
			R_es_set_page( binPn );			//	Bin画面binPnページを処理対象とする。
			R_es_set_degree( 0 );			//	認識方向をright 0°方向に設定する。
			R_es_set_length( xl );			//	サーチ距離をxlとする。
			R_es_set_noise( 1 );			//	5画素以下の黒（又は白）は無視する。
			R_es_set_start( rx, ry );		//	認識開始点の設定
			rc = ES_WHITE;					//  w -> b
			if( R_es_bin_search( &rx, &ry, &rc ) != OK ){
				rx=xs;
			}

			G2_data_l[n][0] = rx;								//Ｇ２座標データ保存
			G2_data_l[n][1] = ry;								//Ｇ２座標データ保存
		}

		if( i_mode & INSPECT_STEP ) {
			for( n=0; n<80; n++ ) {
				if( G2_data_l[n][0] == 0xffff ) break;
				rx = G2_data_l[n][0];
				ry = G2_data_l[n][1];
				R_DrawDispCross( DRAW_GREEN, rx, ry, 2, SOLID_LINE);
			}
		}
	return( OK );
}


/************************************************
	Ｇ２エッジサーチ（右）
************************************************/

static int		TWA_G2_search_gray_ri( int binPn, int grayPn, int mode )
{
		int					xs, ys, xe, ye;
		int					rx, ry, rl, rx2;
		int					rc;
		int					xl, yl;
		int					x_inc;							//傾き*1000000
		int					n,i;
		int					xo;
		int					i_mode	= R_CS_GetInspectMode();
		R_RECT				area;

		xo = 0;
		if( mode == 0 ) {
			xo = 45;			// 4->8  2017.03.21  tanaka
		}

		//傾き計算
		ys = (Ap_pos[4]+G2_pos[2])/2;
		ye = (Ap_pos[5]+G2_pos[3])/2;
		yl = ye-ys;
		if( yl <= 0 ) return(ERROR);                //GEE Program hang
		xl = Ap_pos[3]-Ap_pos[1];
		x_inc = xl*10000000/yl;

		//サーチ条件設定
		ye = G2_pos[3]-4;							//サーチ終了座標
		xl = (Ap_pos[1]-Ap_pos[0])/2;			//サーチ距離 W/2

		//Ｇ２上認識開始点の設定
		for( ry=0; ry<G2_pos[2]; ry+=ES_G2_yp );
		ry+=ES_G2_yp;

		for( n=0; n<80; n++ ) {
			G2_data_r[n][0] = 0xffff;
			G2_data_r[n][1] = 0xffff;
		}

		//Ｇ２エッジサーチ
		for( n=0; n<79; n++, ry+=ES_G2_yp ) {
			if( ry >= ye ) break;
			xs = (ry-ys)*x_inc/10000000 + Ap_pos[1];
			rx = xs;

			for(i=0;i<6;i++){
				if( R_pstate( rx-i, ry, binPn ) != KURO )	break;		//認識開始点の色判別
			}
			if(i>=6){													//外形から6dot(72um)入っても白（セラミック）が見つからない場合は異常
					G2_data_r[n][0] = rx;								//Ｇ２座標データ保存
					G2_data_r[n][1] = ry;								//Ｇ２座標データ保存
					continue;
			}
			rx -= i;													//セラミックが見つかったところから、G2がある位置を探す
			xs = rx;
			xl = 40;

			//外～Ｇ２
			R_es_set_page( binPn );			//	Bin画面binPnページを処理対象とする。
			R_es_set_degree( 180 );			//	認識方向をleft 180°方向に設定する。
			R_es_set_length( xl );			//	サーチ距離をrlとする。
			R_es_set_noise( 1 );			//	5画素以下の黒（又は白）は無視する。
			R_es_set_start( rx, ry );		//	認識開始点の設定
			rc = ES_WHITE;					//  w -> b
			if( R_es_bin_search( &rx, &ry, &rc ) != OK ){
				rx=xs;
			}

			G2_data_r[n][0] = rx;								//Ｇ２座標データ保存
			G2_data_r[n][1] = ry;								//Ｇ２座標データ保存
		}

		if( i_mode & INSPECT_STEP ) {
			for( n=0; n<80; n++ ) {
				if( G2_data_r[n][0] == 0xffff ) break;
				rx = G2_data_r[n][0];
				ry = G2_data_r[n][1];
				R_DrawDispCross( DRAW_GREEN, rx, ry, 2, SOLID_LINE);
			}
		}
	return( OK );
}

/************************************************
	Ｇ２エッジサーチ（上）
************************************************/

static int		TWA_G2_search_up( int binPn )
	{
		int					xs, xe, rx, ry, rl;
		int					rc;
		int					x_sum, x_n;
		int					n;
		int					haba_x = 8;					//Ｇ２検査幅
		int					i_mode	= R_CS_GetInspectMode();

		R_es_set_page( binPn );			//	バイナリ画面binPnページを処理対象とする。

		//Ｇ２左認識開始点の設定
			x_sum = x_n = 0;
			for( n=3; n<8; n++ ) {
				if( G2_data_l[n][0] == 0xffff ) break;
				x_sum += G2_data_l[n][0];
				x_n++;
			}
			if( x_n == 0 ) return(ERROR);
			xs = x_sum/x_n+10;

			x_sum = x_n = 0;
			for( n=3; n<8; n++ ) {
				if( G2_data_r[n][0] == 0xffff ) break;
				x_sum += G2_data_r[n][0];
				x_n++;
			}
			if( x_n == 0 ) return(ERROR);
			xe = x_sum/x_n-10;

		//Ｇ２エッジサーチ
			rx = xs;
			for( n=0; n<79; n++, rx+=haba_x ) {
				if( rx >= xe ) break;
				ry = G2_pos[2];
				rl = G2_pos[2] - Ap_pos[2] - 5;		//20 ->5 2021.10.22<shio>
				if( R_pstate( rx, ry, binPn ) != KURO ) {		//認識開始点の色判別
					//外～Ｇ２
					if( TWA_es_bin_search( binPn, &rx, &ry, 90, 50, TWA_ES_BLACK ) == ERROR ) ry = G2_pos[2] + 100;	//return(ERROR);
				} else {
					//Ｇ２～外
					if( TWA_es_bin_search( binPn, &rx, &ry, 270, rl, TWA_ES_WHITE ) == ERROR ) ry = G2_pos[2];	//return(ERROR);
				}
				G2_data_u[n][0] = rx;								//Ｇ２座標データ保存
				G2_data_u[n][1] = ry;								//Ｇ２座標データ保存
			}
			for( ; n<80; n++ ) {
				G2_data_u[n][0] = 0xffff;
				G2_data_u[n][1] = 0xffff;
			}

		if( i_mode & INSPECT_STEP ) {
			R_RECT area;
			for( n=0; n<80; n++ ) {
				if( G2_data_u[n][0] == 0xffff ) break;
				rx = G2_data_u[n][0];
				ry = G2_data_u[n][1];
				area.left = rx -1;
				area.right = rx +1;
				area.top = ry -1;
				area.bottom = ry +1;
				R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE);
			}
		}
		return(OK);
	}


/************************************************
	Ｇ２エッジサーチ（下）
************************************************/

static int		TWA_G2_search_lo( int binPn )
	{
		int					xs, xe, rx, ry, rl;
		int					rc;
		int					x_sum, x_n;
		int					n;
		int					haba_x = 8;					//Ｇ２検査幅
		int					i_mode	= R_CS_GetInspectMode();

		R_es_set_page( binPn );			//	バイナリ画面binPnページを処理対象とする。

		//Ｇ２右認識開始点の設定
			x_sum = x_n = 0;
			for( n=0; n<80; n++ ) if( G2_data_l[n][0] == 0xffff ) break;
			if( (0==(Option & BACKINSP_MODE)) ){		//v2.11
				n-=5;
				if( n < 5 ) return(ERROR);
			}else{
				n-=1;
				if( n < 1 ) return(ERROR);
			}
			for( ; n<80; n++ ) {
				if( G2_data_l[n][0] == 0xffff ) break;
				x_sum += G2_data_l[n][0];
				x_n++;
			}
			if( x_n == 0 ) return(ERROR);
			xs = x_sum/x_n+10;

			x_sum = x_n = 0;
			for( n=0; n<80; n++ ) if( G2_data_r[n][0] == 0xffff ) break;
			if( (0==(Option & BACKINSP_MODE)) ){		//v2.11
				n-=5;
				if( n < 5 ) return(ERROR);
			}else{
				n-=1;
				if( n < 1 ) return(ERROR);
			}
			for( ; n<80; n++ ) {
				if( G2_data_r[n][0] == 0xffff ) break;
				x_sum += G2_data_r[n][0];
				x_n++;
			}
			if( x_n == 0 ) return(ERROR);
			xe = x_sum/x_n-10;

		//Ｇ２エッジサーチ
			rx = xs;
			for( n=0; n<79; n++, rx+=haba_x ) {
				if( rx >= xe ) break;
				ry = G2_pos[3];
				rl = Ap_pos[3] - G2_pos[3] - 5;				//20 -> 5 2021.10.22<shio>
				if( R_pstate( rx, ry, binPn ) != KURO ) {		//認識開始点の色判別
					//外～Ｇ２
					if( TWA_es_bin_search( binPn, &rx, &ry, 270, 50, TWA_ES_BLACK ) == ERROR ) ry = G2_pos[3] - 50;	// return(ERROR);
				} else {
					//Ｇ２～外
					if( TWA_es_bin_search( binPn, &rx, &ry, 90, rl, TWA_ES_WHITE ) == ERROR ) ry = G2_pos[3];	//return(ERROR);
				}
				G2_data_d[n][0] = rx;								//Ｇ２座標データ保存
				G2_data_d[n][1] = ry;								//Ｇ２座標データ保存
			}
			for( ; n<80; n++ ) {
				G2_data_d[n][0] = 0xffff;
				G2_data_d[n][1] = 0xffff;
			}

		if( i_mode & INSPECT_STEP ) {
			R_RECT area;
			for( n=0; n<80; n++ ) {
				if( G2_data_d[n][0] == 0xffff ) break;
				rx = G2_data_d[n][0];
				ry = G2_data_d[n][1];
				area.left = rx -1;
				area.right = rx +1;
				area.top = ry -1;
				area.bottom = ry +1;
				R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE);
			}
		}
		return(OK);
	}

/************************************************
	Ｇ２　エッジ検出
************************************************/

int		TWA_G2_ES_searchLTR_l18( int mode )
	{
		int					grayPn	= InspGrayPn;
		int					binPn	= InspBinPn;
		int					i_mode	= R_CS_GetInspectMode();
		int					pt[10];

		if( i_mode & INSPECT_STEP ){
			R_display_control( BIN_MEMORY | GRAY_MEMORY );
			R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1);
		}

		//ノイズ除去処理
/*
		// left
				pt[0] = 0;					pt[1] = 0;
				pt[2] = FxSize-1;			pt[3] = FySize-1;
				TWA_contrct2( binPn, 3, 1, pt );
				TWA_contrct2( binPn, 3, 1, pt );
				TWA_expand2(  binPn, 3,  1,  pt );
				TWA_expand2(  binPn, 3,  1,  pt );
*/
		if( i_mode & INSPECT_STEP ){
//		{
//R_DrawCls();
			R_move_bin_memory( binPn, 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
		}

	//Ｇ２左
		if( TWA_G2_search_gray_le( binPn, grayPn, mode ) != OK ) return(ERROR);
	//Ｇ２右
		if( TWA_G2_search_gray_ri( binPn, grayPn, mode ) != OK ) return(ERROR);

		if( mode != 0 ) return(OK);

	//Ｇ２上
		if( TWA_G2_search_up(binPn) != OK ) return(ERROR);
	//Ｇ２下
		if( TWA_G2_search_lo(binPn) != OK ) return(ERROR);

		if( i_mode & INSPECT_STEP ){
			WaitMouseClick();
		}
		return(OK);

	}

/************************************************
	電極幅検査
************************************************/

int		TWA_JudgeC2SizeY_l18( int bin_pn )
	{
		int						xs, xe;
		int						ys, ye;
		int						rx, ry, rl, rc;
		int						m;
		int						G2_y;
		int						d, dn;
		int						c2l, C2fail, C2fail_max;
		int						judge_c2n;
		int						cx, cy;
		int						offset_ly = 2;
		int						offset_ry = 2;
		int						offset_ys = 20;        	// -3  nok for adjust up corner
		int						offset_ux =  6;			//2  JHONG
		int						offset_lx =  4;       	//2   nok for adj.down corner
		int						size_x = 40;
		int						judge_ul, judge_ll, judge_ur, judge_lr;
		int						n=0;
		int						countul=0;
		int						countur=0;
		int						countll=0;
		int						countlr=0;
		int						countxul=0;
		int						countxur=0;
		int						countxll=0;
		int						countxlr=0;
		int						ry_uple=0;
		int						ry_upri=0;
		int						ry_lole=0;
		int						ry_lori=0;
		int						FreqData_uple_max=0;
		int						FreqData_upri_max=0;
		int						FreqData_lole_max=0;
		int						FreqData_lori_max=0;
		int						ys_uple=0;
		int						ys_upri=0;
		int						ys_lole=0;
		int						ys_lori=0;
		int						xs_uple[200];
		int						xs_upri[200];
		int						yys_uple[200];
		int						yys_upri[200];

		int						xs_lole[200];
		int						xs_lori[200];
		int						yys_lole[200];
		int						yys_lori[200];

		int						C2_up_le_size=0;
		int						C2_up_ri_size=0;
		int						C2_lo_le_size=0;
		int						C2_lo_ri_size=0;


		int	up_le[4];				//xs, ys, xe, ye
		int	up_ri[4];
		int	lo_le[4];
		int	lo_ri[4];
		int						i_mode	= R_CS_GetInspectMode();

		// LTR50L
		offset_ux =  0;
		offset_lx =  0;
		size_x    = 20;

		Corner_pos[0]=Elec[DataUL].CornerPosX[0];	Corner_pos[1]=Elec[DataUL].CornerPosY[0];
		Corner_pos[2]=Elec[DataUL].CornerPosX[2];	Corner_pos[3]=Elec[DataUL].CornerPosY[2];
		Corner_pos[4]=Elec[DataUL].CornerPosX[1];	Corner_pos[5]=Elec[DataUL].CornerPosY[1];
		Corner_pos[6]=Elec[DataUL].CornerPosX[3];	Corner_pos[7]=Elec[DataUL].CornerPosY[3];

		R_es_set_page( bin_pn );
		// Judge data
			judge_c2n = InspPara.ElecC2Xmin[TWA_Spec].nXsize;
		// initial

		// 検査範囲設定
		//上左 area
				// ye
				up_le[1] = Corner_pos[1] - offset_ly;
				// ys
				G2_y = dn = 0;
				for( d=2; d<80; d++ ) {
					if( G2_data_u[d][1] == 0xffff ) return ERROR;
					G2_y += G2_data_u[d][1];
					dn++;
					if( dn >= 5 ) break;
				}
				if( dn == 0 ) return ERROR;
				up_le[3] = G2_y / dn + offset_ys;		//ys
				// xs
				up_le[0] = Corner_pos[0] + offset_ux;
				// xe
				up_le[2] = up_le[0] + size_x;

		//上右 area
			// ye
				up_ri[1] = Corner_pos[3] - offset_ry;
				// ys
				G2_y = dn = 0;
				for( d=0; d<80; d++ ) {
					if( G2_data_u[d][1] == 0xffff ) break;
				}
				d-=3;
				for( ; d>0; d-- ) {
					G2_y += G2_data_u[d][1];
					dn++;
					if( dn >= 5 ) break;
				}
				if( dn == 0 ) return ERROR;
				up_ri[3] = G2_y / dn + offset_ys;		//ys
				// xs
				up_ri[2] = Corner_pos[2] - offset_lx;
				// xe
				up_ri[0] = up_ri[2] - size_x;	//ye

		//下左 area
				// ye
				lo_le[3] = Corner_pos[5] + offset_ly;
				// ys
				G2_y = dn = 0;
				for( d=2; d<80; d++ ) {
					if( G2_data_d[d][1] == 0xffff ) return ERROR;
					G2_y += G2_data_d[d][1];
					dn++;
					if( dn >= 5 ) break;
				}
				if( dn == 0 ) return ERROR;
				lo_le[1] = G2_y / dn - offset_ys;		//ys
				// xs
				lo_le[0] = Corner_pos[4] + offset_ux;
				// xe
				lo_le[2] = lo_le[0] + size_x;

		//下右 area
				// ye
				lo_ri[3] = Corner_pos[7] + offset_ry;
				// ys
				G2_y = dn = 0;
				for( d=0; d<80; d++ ) {
					if( G2_data_d[d][1] == 0xffff ) break;
				}
				d-=3;
				for( ; d>0; d-- ) {
					G2_y += G2_data_d[d][1];
					dn++;
					if( dn >= 5 ) break;
				}
				if( dn == 0 ) return ERROR;
				lo_ri[1] = G2_y / dn - offset_ys;		//ys
				// xs
				lo_ri[2] = Corner_pos[6] - offset_lx;
				// xe
				lo_ri[0] = lo_ri[2] - size_x;

			if( i_mode & INSPECT_STEP ) {
				R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
				R_move_bin_memory( bin_pn, BinP0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
				R_box( up_le[0], up_le[1], up_le[2], up_le[3], GRAPH_REVERSE, SOLID_LINE );
				R_box( up_ri[0], up_ri[1], up_ri[2], up_ri[3], GRAPH_REVERSE, SOLID_LINE );
				R_box( lo_le[0], lo_le[1], lo_le[2], lo_le[3], GRAPH_REVERSE, SOLID_LINE );
				R_box( lo_ri[0], lo_ri[1], lo_ri[2], lo_ri[3], GRAPH_REVERSE, SOLID_LINE );
				WaitMouseClick();
			}
		//検査範囲設定 end

		// Insp.
			// 左上 insp.
				xs = up_le[0];
				xe = up_le[2];
				ys = up_le[3];
				c2l = C2fail = C2fail_max = n = 0;
				for( rx=xs+4; rx<=xe-4; rx++,n++ ) {						// nok
					ry = ys;
					ye = up_le[1];
					rl = ys-ye+1;
					R_es_set_degree( 270 );			//	認識方向をup 270°方向に設定する。
					R_es_set_length( rl );			//	サーチ距離をrlとする。
					R_es_set_noise( 2 );			//	5画素以下の黒（又は白）は無視する。
					R_es_set_start( rx, ry );		//	認識開始点の設定
					rc = ES_BLACK;					//  black -> white
					if( R_es_bin_search( &rx, &ry, &rc ) != OK ) ry=ye;
			ry-=6; //フィルタでシフトする分を補正
						C2_data_ul[n][0] = rx;
						C2_data_ul[n][1] = ry;
						countul=n;
				}
				for(n=0; n<1000; n++){
					FreqData[n] = 0;
				}
				ys = up_le[1];
				ye = up_le[3];
				for(ys=up_le[1]; ys<ye; ys++){
					for(n=0; n<=countul; n++){
						ry_uple=C2_data_ul[n][1];
						if(abs(ys-ry_uple+1)<4)	FreqData[ys]++;
					}
				}

				for(ys=up_le[1];ys<ye;ys++){
					FreqData_uple_max=max(FreqData[ys],FreqData_uple_max);
				}
				for(ys=up_le[1];ys<ye;ys++){
					if(FreqData[ys] == FreqData_uple_max) ys_uple = ys;
				}
				for(m=0; m<200; m++){
					yys_uple[m] = 0;
				}
				m=0;
				for(n=0;n<countul;n++){
					if(abs(ys_uple - C2_data_ul[n][1])<=4) {
						yys_uple[m] = C2_data_ul[n][1];
						xs_uple[m] = C2_data_ul[n][0];
						m++;

					}else{
						continue;
					}
					countxul=m;
				}
				if(countxul<=0){        // 2017.04.09 tanaka
					C2fail_max = judge_c2n+1;
				} else {
					C2_up_le_size = ys_uple -up_le[1];
					for(m=0;m<=countxul-1;m++){
						rx=xs_uple[m];
						ry=yys_uple[m];
						ye =up_le[1];
						c2l= C2_up_le_size;

						if( c2l < InspPara.ElecC2Xmin[TWA_Spec].nYsize ) {
							C2fail++;
							if( i_mode & INSPECT_STEP ) {
								R_line( rx, ye-1, rx, ry, GRAPH_REVERSE, SOLID_LINE );
							}
						} else {
							C2fail = 0;
							if( i_mode & INSPECT_STEP ) {
								R_pset( rx, ry, GRAPH_REVERSE );
								R_pset( rx, ye, GRAPH_REVERSE );
							}
						}
						C2fail_max = max( C2fail, C2fail_max );
					}
				}
				if( i_mode & INSPECT_STEP ) {
					sprintf( Comment, "fail:%3d", C2fail_max );
					cy = 0;		cx = up_le[0]/16-3;	if(cx < 0) cx=0;
					R_chrdisp( cx, cy, Comment );
				}
				if( C2fail_max > judge_c2n )	judge_ul = ERROR;
				else							judge_ul = OK;

			//上右 insp.
				xs = up_ri[0];
				xe = up_ri[2];
				ys = up_ri[3];
				c2l = C2fail = C2fail_max = n= 0;
				for( rx=xs+4; rx<=xe-4; rx++ ,n++) {
					ry = ys;
					ye = up_ri[1];
					rl = ys-ye+1;
					R_es_set_degree( 270 );			//	認識方向をup 270°方向に設定する。
					R_es_set_length( rl );			//	サーチ距離をrlとする。
					R_es_set_noise( 2 );			//	5画素以下の黒（又は白）は無視する。
					R_es_set_start( rx, ry );		//	認識開始点の設定
					rc = ES_BLACK;					//  black -> white
					if( R_es_bin_search( &rx, &ry, &rc ) != OK ) ry=ye;
					ry-=6; //フィルタでシフトする分を補正
						C2_data_ur[n][0] = rx;
						C2_data_ur[n][1] = ry;
						countur=n;
				}
				for(n=0; n<1000; n++){
					FreqData[n] = 0;
				}
				ys = up_ri[1];
				ye = up_ri[3];
				for(ys=up_ri[1]; ys<ye; ys++){
					for(n=0; n<=countur; n++){
						ry_upri=C2_data_ur[n][1];
						if(abs(ys-ry_upri+1)<4)	FreqData[ys]++;
					}
				}

				for(ys=up_ri[1];ys<ye;ys++){
					FreqData_upri_max=max(FreqData[ys],FreqData_upri_max);
				}
				for(ys=up_ri[1];ys<ye;ys++){
					if(FreqData[ys] == FreqData_upri_max) ys_upri = ys;
				}
				for(m=0; m<200; m++){
					yys_upri[m] = 0;
				}
				m=0;
				for(n=0;n<countur;n++){
					if(abs(ys_upri - C2_data_ur[n][1])<=4) {
						yys_upri[m] = C2_data_ur[n][1];
						xs_upri[m] = C2_data_ur[n][0];
						m++;

					}else{
						continue;
					}
					countxur=m;
				}
				if(countxur<=0){        // 2017.04.09 tanaka
					C2fail_max = judge_c2n+1;
				} else {
					C2_up_ri_size = ys_upri-up_ri[1];
					for(m=0;m<=countxur-1;m++){
						ry=yys_upri[m];
						rx=xs_upri[m];
						ye =up_ri[1];
						c2l= C2_up_ri_size;

						if( c2l <  InspPara.ElecC2Xmin[TWA_Spec].nYsize ) {
							C2fail++;
							if( i_mode & INSPECT_STEP ) {
								R_line( rx, ye-1, rx, ry, GRAPH_REVERSE, SOLID_LINE );
							}
						} else {
							C2fail = 0;
							if( i_mode & INSPECT_STEP ) {
								R_pset( rx, ry, GRAPH_REVERSE );
								R_pset( rx, ye, GRAPH_REVERSE );
							}
						}
						C2fail_max = max( C2fail, C2fail_max );
					}
				}
				if( i_mode & INSPECT_STEP ) {
					sprintf( Comment, "fail:%3d", C2fail_max );
					cy = 0;		cx = up_ri[0]/16+3;
					R_chrdisp( cx, cy, Comment );
				}
				if( C2fail_max > judge_c2n )	judge_ur = ERROR;
				else							judge_ur = OK;

			//左下
				xs = lo_le[0];
				xe = lo_le[2];
				ys = lo_le[1];
				c2l = C2fail = C2fail_max = n = 0;
				for( rx=xs+4; rx<=xe-4; rx++ ,n++) {
					ry = ys;
					ye = lo_le[3];
					rl = ye-ys+1;
					R_es_set_degree( 90 );			//	認識方向をdown 90°方向に設定する。
					R_es_set_length( rl );			//	サーチ距離をrlとする。
					R_es_set_noise( 2 );			//	5画素以下の黒（又は白）は無視する。
					R_es_set_start( rx, ry );		//	認識開始点の設定
					rc = ES_BLACK;					//  black -> white
					if( R_es_bin_search( &rx, &ry, &rc ) != OK ) ry=ye;
					ry+=6; //フィルタでシフトする分を補正
						C2_data_ll[n][0] = rx;
						C2_data_ll[n][1] = ry;
						countll=n;
					}
				for(n=0; n<1000; n++){
					FreqData[n] = 0;
				}
				ys = lo_le[3];
				ye = lo_le[1];
				for(ys=lo_le[3]; ys>lo_le[1]; ys--){
					for(n=0; n<=countll; n++){
						ry_lole=C2_data_ll[n][1];
						if(abs(ys-ry_lole+1)<4)	FreqData[ys]++;
					}
				}
				for(ys=lo_le[3];ys>lo_le[1];ys--){
					FreqData_lole_max=max(FreqData[ys],FreqData_lole_max);
				}
				for(ys=lo_le[3];ys>lo_le[1];ys--){
					if(FreqData[ys] == FreqData_lole_max) ys_lole = ys;
				}
				for(m=0; m<200; m++){
					yys_lole[m] = 0;
				}
				m=0;
				for(n=0;n<countll;n++){
					if(abs(ys_lole - C2_data_ll[n][1])<=4) {
						yys_lole[m] = C2_data_ll[n][1];
						xs_lole[m] = C2_data_ll[n][0];
						m++;

					}else{
						continue;
					}
					countxll=m;
				}
				if(countxll<=0){        // 2017.04.09 tanaka
					C2fail_max = judge_c2n+1;
				} else {
					C2_lo_le_size = lo_le[3]- ys_lole;
					for(m=0;m<=countxll-1;m++){
						ry=yys_lole[m];
						rx=xs_lole[m];
						ye =lo_le[3];
						c2l= C2_lo_le_size;

						if( c2l <  InspPara.ElecC2Xmin[TWA_Spec].nYsize ) {
							C2fail++;
							if( i_mode & INSPECT_STEP ) {
								R_line( rx, ry, rx, ye, GRAPH_REVERSE, SOLID_LINE );
							}
						} else {
							C2fail = 0;
							if( i_mode & INSPECT_STEP ) {
								R_pset( rx, ry, GRAPH_REVERSE );
								R_pset( rx, ye, GRAPH_REVERSE );
							}
						}
						C2fail_max = max( C2fail, C2fail_max );
					}
				}
				if( i_mode & INSPECT_STEP ) {
					sprintf( Comment, "fail:%3d", C2fail_max );
					cy = 25;		cx = lo_le[0]/16-3;	if(cy < 0) cy=0;
					R_chrdisp( cx-1, cy, Comment );
				}
				if( C2fail_max > judge_c2n )	judge_ll = ERROR;
				else							judge_ll = OK;

			//右下
				xs = lo_ri[0];
				xe = lo_ri[2];
				ys = lo_ri[1];
				c2l = C2fail = C2fail_max = n = 0;
				for( rx=xs+4; rx<=xe-4; rx++,n++ ) {
					ry = ys;
					ye = lo_ri[3];
					rl = ye-ys+1;
					R_es_set_degree( 90 );			//	認識方向をdown 90°方向に設定する。
					R_es_set_length( rl );			//	サーチ距離をrlとする。
					R_es_set_noise( 2 );			//	5画素以下の黒（又は白）は無視する。
					R_es_set_start( rx, ry );		//	認識開始点の設定
					rc = ES_BLACK;					//  black -> white
					if( R_es_bin_search( &rx, &ry, &rc ) != OK ) ry=ye;
					ry+=6; //フィルタでシフトする分を補正
						C2_data_lr[n][0] = rx;
						C2_data_lr[n][1] = ry;
						countlr=n;
					}
				for(n=0; n<1000; n++){
					FreqData[n] = 0;
				}
				ys = lo_ri[3];
				ye = lo_ri[1];
				for(ys=lo_ri[3]; ys>lo_ri[1]; ys--){
					for(n=0; n<=countlr; n++){
						ry_lori=C2_data_lr[n][1];
						if(abs(ys-ry_lori+1)<4)	FreqData[ys]++;
					}
				}
				for(ys=lo_ri[3];ys>lo_ri[1];ys--){
					FreqData_lori_max=max(FreqData[ys],FreqData_lori_max);
				}
				for(ys=lo_ri[3];ys>lo_ri[1];ys--){
					if(FreqData[ys] == FreqData_lori_max) ys_lori = ys;
				}
				for(m=0; m<200; m++){
					yys_lori[m] = 0;
				}
				m=0;
				for(n=0;n<countlr;n++){
					if(abs(ys_lori - C2_data_lr[n][1])<=4) {
						yys_lori[m] = C2_data_lr[n][0];
						xs_lori[m] = C2_data_lr[n][0];

						m++;

					}else{
						continue;
					}
					countxlr=m;
				}
				if(countxlr<=0){        // 2017.04.09 tanaka
					C2fail_max = judge_c2n+1;
				} else {
					C2_lo_ri_size = lo_ri[3]- ys_lori;
					for(m=0;m<=countxlr-1;m++){
						ry=yys_lori[m];
						rx=xs_lori[m];
						ye =lo_ri[3];
						c2l= C2_lo_ri_size;

						if( c2l <  InspPara.ElecC2Xmin[TWA_Spec].nYsize ) {
							C2fail++;
							if( i_mode & INSPECT_STEP ) {
								R_line( rx, ry, rx, ye, GRAPH_REVERSE, SOLID_LINE );
							}
						} else {
							C2fail = 0;
							if( i_mode & INSPECT_STEP ) {
								R_pset( rx, ry, GRAPH_REVERSE );
								R_pset( rx, ye, GRAPH_REVERSE );
							}
						}
						C2fail_max = max( C2fail, C2fail_max );
					}
				}

				if( i_mode & INSPECT_STEP ) {
					sprintf( Comment, "fail:%3d", C2fail_max );
					cy = 25;		cx = lo_ri[0]/16+3;
					R_chrdisp( cx-1, cy, Comment );
				}
				if( C2fail_max > judge_c2n )	judge_lr = ERROR;
				else							judge_lr = OK;

				if( i_mode & INSPECT_STEP ) {
					R_DrawPrintf(judge_ul != OK? DRAW_RED:DRAW_WHITE , DISP_NORMAL, 0, 31, "C2_up_le_size = %5d\n ",C2_up_le_size );
					R_DrawPrintf(judge_ur != OK? DRAW_RED:DRAW_WHITE , DISP_NORMAL, 0, 32, "C2_up_ri_size = %5d\n ",C2_up_ri_size );
					R_DrawPrintf(judge_ll != OK? DRAW_RED:DRAW_WHITE , DISP_NORMAL, 0, 33, "C2_lo_le_size = %5d\n ",C2_lo_le_size );
					R_DrawPrintf(judge_lr != OK? DRAW_RED:DRAW_WHITE , DISP_NORMAL, 0, 34, "C2_lo_ri_size = %5d\n ",C2_lo_ri_size );
					WaitMouseClick();
				}
			//	 TWA_Judge_G2_furyo();
			// Judge C2 l width
				if(( judge_ul != OK )||( judge_ll != OK )) return ERROR;
				if(( judge_ur != OK )||( judge_lr != OK )) return ERROR;

		return OK;

	}	// TWA_JudgeC2SizeY_l18( int bin_pn ) end


/*********************************************************************************

	TWA_InspC2Ysize_l18( void )

*********************************************************************************/

int		TWA_InspC2Ysize_l18( void )
	{
		int						grayPn	= InspGrayPn;
		int						binPn	= InspBinPn;
		int						up_xs, up_xe, up_xl, up_ys, up_ye, up_yl;
		int						lo_xs, lo_xe, lo_xl, lo_ys, lo_ye, lo_yl;
		int						yl_limit = 5;
		int						yu_limit = TWA_Y_size - 5;
		int						xl_limit = min(Elec[DataUL].CornerPosX[0],Elec[DataUL].CornerPosX[1])-50;
		int						xu_limit = max(Elec[DataUL].CornerPosX[2],Elec[DataUL].CornerPosY[3])+50;
		int						level;
		int						pt[10];
		int						i_mode	= R_CS_GetInspectMode();

		//LTR50L
		TWA_CoefficientC2 = 8;
		TWA_SurfLoC2      = 150;

		// 外形ポジション代入
		Ap_pos[0]=Elec[DataUL].CornerPosX[0];	//Left Upper
		Ap_pos[1]=Elec[DataUL].CornerPosX[1];	//Left Lower
		Ap_pos[2]=Elec[DataUL].CornerPosX[2];	//Right Upper
		Ap_pos[3]=Elec[DataUL].CornerPosX[3];	//Right Lower
		// initial
			TWA_bin_cls( binPn, 0, 0, FX_SIZE-1, FY_SIZE-1 );
			TWA_gray_fill( 0, 0, FX_SIZE-1, FY_SIZE-1, 0, BASE3_GRAYADDRESS );

		// 平滑化
			// left
				up_xs = Ap_pos[0] - 20;			if( up_xs < xl_limit ) up_xs = xl_limit;
				up_xe = Ap_pos[2] + 20;			if( up_xe > xu_limit ) up_xe = xu_limit;
				up_ys = Ap_pos[4] - 20;			if( up_ys < yl_limit ) up_ys = yl_limit;
				up_ye = G2_pos[2] + 20;			if( up_ye > yu_limit ) up_ye = yu_limit;
				pt[0] = TWA_SmoothingXsizeC2;	pt[1] = TWA_SmoothingYsizeC2;
				pt[2] = up_xs;					pt[3] = up_ys;
				pt[4] = up_xe;					pt[5] = up_ye;
				TWA_smoothing( grayPn, BASE3_GRAYADDRESS, pt );
			// right
				lo_xs = Ap_pos[1] - 20;			if( lo_xs < xl_limit ) lo_xs = xl_limit;
				lo_xe = Ap_pos[3] + 20;			if( lo_xe > xu_limit ) lo_xe = xu_limit;
				lo_ys = G2_pos[3] - 20;			if( lo_ys < yl_limit ) lo_ys = yl_limit;
				lo_ye = Ap_pos[5] + 20;			if( lo_ye > yu_limit ) lo_ye = yu_limit;
				pt[0] = TWA_SmoothingXsizeC2;	pt[1] = TWA_SmoothingYsizeC2;
				pt[2] = lo_xs;					pt[3] = lo_ys;
				pt[4] = lo_xe;					pt[5] = lo_ye;
				TWA_smoothing( grayPn, BASE3_GRAYADDRESS, pt );
			// view
				if( i_mode & INSPECT_STEP ) {
					R_gray_fill( 0, 0, FX_SIZE-1, FY_SIZE-1, 0 );
					TWA_bin_cls( 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					R_move_gray_memory( BASE3_GRAYADDRESS, GrayP0, 0, 0, TWA_X_size-1, TWA_Y_size-1 );
					WaitMouseClick();
				}

		// サーフボードフィルター
			// left
				pt[0] = up_xs;							pt[1] = up_ys;
				pt[2] = up_xe;							pt[3] = up_ye;
				pt[4] = TWA_CoefficientC2;								// level up 倍率
				pt[5] = 0;	// 1;										// 均最大値 : 1
				TWA_surf_board( grayPn, BASE3_GRAYADDRESS, BASE3_GRAYADDRESS, pt, TWA_SurfLoC2 );
			// right
				pt[0] = lo_xs;							pt[1] = lo_ys;
				pt[2] = lo_xe;							pt[3] = lo_ye;
				pt[4] = TWA_CoefficientC2;								// level up 倍率
				pt[5] = 0;	// 1;										// 均最大値 : 1
				TWA_surf_board( grayPn, BASE3_GRAYADDRESS, BASE3_GRAYADDRESS, pt, TWA_SurfLoC2 );
			// view
				if( i_mode & INSPECT_STEP ) {
					R_gray_fill( 0, 0, FX_SIZE-1, FY_SIZE-1, 0 );
					TWA_bin_cls( 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					R_move_gray_memory( BASE3_GRAYADDRESS, GrayP0, 0, 0, TWA_X_size-1, TWA_Y_size-1 );
					WaitMouseClick();
				}

		// White or
			// left
				TWA_Insp_white_or( grayPn, BASE3_GRAYADDRESS, up_xs, up_ys, up_xe, up_ye, C1_level );
			// right
				TWA_Insp_white_or( grayPn, BASE3_GRAYADDRESS, lo_xs, lo_ys, lo_xe, lo_ye, C1_level );
			// view
				if( i_mode & INSPECT_STEP ) {
					R_gray_fill( 0, 0, FX_SIZE-1, FY_SIZE-1, 0 );
					TWA_bin_cls( 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					R_move_gray_memory( BASE3_GRAYADDRESS, GrayP0, 0, 0, TWA_X_size-1, TWA_Y_size-1 );
					WaitMouseClick();
				}

		// Gray to Bin
			// left
				up_xl = up_xe-up_xs+1;		up_yl = up_ye-up_ys+1;
				level = Bin_C2YsizeLevel;
				R_gray2bin( level, up_xs, up_ys, up_xl, up_yl, Gray_address[BASE3_GRAYADDRESS], Bin_address[binPn] );
			// right
				lo_xl = lo_xe-lo_xs+1;		lo_yl = lo_ye-lo_ys+1;
				level = Bin_C2YsizeLevel;
				R_gray2bin( level, lo_xs, lo_ys, lo_xl, lo_yl, Gray_address[BASE3_GRAYADDRESS], Bin_address[binPn] );
			// view
				if( i_mode & INSPECT_STEP ) {
					R_display_control( BIN_MEMORY | GRAY_MEMORY );
					R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
				//	R_move_gray_memory( grayPn, 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					R_move_bin_memory(  binPn, 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					WaitMouseClick();
				}

		//ノイズ除去処理
			// left
				pt[0] = up_xs;					pt[1] = up_ys;
				pt[2] = up_xe;					pt[3] = up_ye;
				TWA_expand2(  binPn, Pl_expand_x,  Pl_expand_y,  pt );
				TWA_contrct2( binPn, Pl_contrct_x, Pl_contrct_y, pt );
				TWA_contrct2( binPn, Pl_contrct_x, Pl_contrct_y, pt );
				TWA_expand2(  binPn, Pl_expand_x,  Pl_expand_y,  pt );

				TWA_contrct2( binPn, 5, 1, pt );                      //GEE 1 to 3
				TWA_expand2(  binPn, 3, 1, pt );                      //GEE 1 to 3
				TWA_expand2(  binPn, 3, 1, pt );                      //GEE 1 to 3
				TWA_contrct2( binPn, 5, 1, pt );                      //GEE 1 to 3

			// right
				pt[0] = lo_xs;					pt[1] = lo_ys;
				pt[2] = lo_xe;					pt[3] = lo_ye;
				TWA_expand2(  binPn, Pl_expand_x,  Pl_expand_y,  pt );
				TWA_contrct2( binPn, Pl_contrct_x, Pl_contrct_y, pt );
				TWA_contrct2( binPn, Pl_contrct_x, Pl_contrct_y, pt );
				TWA_expand2(  binPn, Pl_expand_x,  Pl_expand_y,  pt );

				TWA_contrct2( binPn, 5, 1, pt );                      //GEE 1 to 3
				TWA_expand2(  binPn, 3, 1, pt );                      //GEE 1 to 3
				TWA_expand2(  binPn, 3, 1, pt );                      //GEE 1 to 3
				TWA_contrct2( binPn, 5, 1, pt );                      //GEE 1 to 3

			// view
				if( i_mode & INSPECT_STEP ) {
					R_display_control( BIN_MEMORY | GRAY_MEMORY );
					R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
				//	R_move_gray_memory( grayPn, 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					R_move_bin_memory(  binPn, 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					WaitMouseClick();
				}

			// Judge C2 X size
				if( TWA_JudgeC2SizeY_l18( binPn ) != OK ) return ERROR;

		return OK;

	}	// TWA_InspC2Ysize_l18( void ) end

/************************************************
	C1 Mezumari Left <0001>
*************************************************/

static int		TWA_Judge_Mzleft( void )
	{
		int						grayPn	= InspGrayPn;
		int						binPn	= InspBinPn;
		int						xs, xe, ys, ye;
		int						xl, yl, xel;
		int						area, area_sum;
		int						x_offset_out = 40;
		int						x_offset_in  = 8;   //2 <K5> 4 -> 3
		int						y_offset	 = 48;  //24 <z3>32
		int						xsize, ysize; 		//, lsize;
		int						ans, bn;
		int						judge_area; 		//, judge_l;
		int						i_mode	= R_CS_GetInspectMode();

		// 外形ポジション代入
		Ap_pos[2]=Elec[DataLR].CornerPosY[0];	//Left Upper
		Ap_pos[3]=Elec[DataLR].CornerPosY[1];	//Left Lower

		// Judge data
			 //judge_area		= Judge_C1_Mz[2] * Judge_C1_Mz[3] * 2;
			 judge_area		= InspPara.ElecC1Mezumari[TWA_Spec].nXsize * InspPara.ElecC1Mezumari[TWA_Spec].nYsize * 2;
//			 judge_l		= Judge_C1_Mz[2] + Judge_C1_Mz[3];

		// Inspection set-up
			// left ys, yc, ye position
				ys = Ap_pos[2]+y_offset;
				if( ys == 0 ) return(ERROR); //<0001> hang up
				ye = Ap_pos[3]-y_offset;
				if( ye == 0 ) return(ERROR); //<0001> hang up
				if( ys >= ye ) return ERROR;
			// left xs position

			//	xs = Ap_pos[0] + x_offset_out; <K5>
					xs = G2_pos[0] - x_offset_out +10; //<K5> +8

			// left xe position

			//	xel = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-ye); <K5>
				xel = G2_pos[0]; //<K5>

				xe = xel - x_offset_in;
		 if( xe == 0 ) return(ERROR); //<0001> hang up

				//２値化（グレープレーンから２値プレーンへ）
				xl = xe - xs + 1;
				yl = ye - ys + 1;
				area = abs(xl * yl);
				if( area == 0 ) return(ERROR); //<0001> hang up

				R_gray2bin16( Bin_Mz_l[TWA_Spec], xs, ys, xl, yl, Gray_address[grayPn], Bin_address[binPn] ); //<K5>

				// Insp. Area and
			// view
				if( i_mode & INSPECT_STEP ) {
//					R_display_control( BIN_MEMORY | GRAY_MEMORY );
					R_bin_cls( 0, 0, FxSize-1, FySize-1 );
					R_move_bin_memory(  binPn, 0, xs, ys, xe, ye );
					R_box( xs, ys, xe, ye, GRAPH_REVERSE, SOLID_LINE );
					DrawMessage( "Insp. [C1 Mezuleft]", DRAW_GREEN, 2, 2 );
				}
			// 2 value measurement
				R_set_meas_configration( &Mpx, 0, 5, RM_BLACK|RM_WHITE, RM_AREA );
				if( R_measure_fast( &Mpx, binPn, xs, ys, xl, yl, 0, 3 ) != OK ) {
					R_measure_close( &Mpx );
					R_measure_open( &Mpx, 2000 );
					return ERROR;
				}
			// Judge
				ans = OK;
				area_sum = 0;
				for( bn=1; bn<=Mpx.bn; bn++ ) {
					xsize = Mpx.imax[bn] - Mpx.imin[bn] + 1;
					if( xsize == 0 ) return(ERROR); //<0001> hang up
					ysize = Mpx.jmax[bn] - Mpx.jmin[bn] + 1;
					if( ysize == 0 ) return(ERROR); //<0001> hang up
					if( Mpx.area[bn] > 20 ) area_sum += Mpx.area[bn];

					if( i_mode & INSPECT_STEP ) {
						// x line
						ys = Mpx.center_y[bn];
						xs = Mpx.imin[bn];		xe = Mpx.imax[bn];
						R_line( xs, ys, xe, ys, GRAPH_REVERSE, SOLID_LINE );
						// y line
						xs = Mpx.center_x[bn];
						ys = Mpx.jmin[bn];		ye = Mpx.jmax[bn];
						R_line( xs, ys, xs, ye, GRAPH_REVERSE, SOLID_LINE );
						sprintf( Comment1, "Xsize:%3d(bit)  Ysize:%3d(bit)", xsize, ysize );
						sprintf( Comment2, "%s\n Area :%5d(bit)", Comment1,Mpx.area[bn] );
						DrawMessage( Comment2, DRAW_GREEN, 2, 2 );
					}
					if( xsize >= InspPara.ElecC1Mezumari[TWA_Spec].nXsize ) {
					  if( xsize == 0 ) return(ERROR); //<0001> hang up
					  if( ysize >= InspPara.ElecC1Mezumari[TWA_Spec].nYsize ) {
						 if( ysize == 0 ) return(ERROR); //<0001> hang up
							ans = ERROR;
							break;
						}
					}
/* //<0001>
					lsize = xsize + ysize;
//					printf( "lsizeL:%d\n", lsize );	//<0001>
					if( lsize >= judge_l ) {
						ans = ERROR;
						break;
					}
*///<0001>

//<0001>
					if( area_sum > judge_area ) {
						if( area_sum == 0 ) return(ERROR); //<0001> hang up
						ans = ERROR;
						if( i_mode & INSPECT_STEP ) {
							sprintf( Comment, "                 " );
							sprintf( Comment2, "Area :%5d(bit)", area_sum );
							DrawMessage( Comment2, DRAW_GREEN, 2, 2 );
						}
						break;
					}
//<0001>

				}

			return ans;

	}	// TWA_Judge_Mzleft( int binPn )




/************************************************
	C1 Mezumari Right <0001>
*************************************************/

static int		TWA_Judge_Mzright( void )
	{
		int						grayPn	= InspGrayPn;
		int						binPn	= InspBinPn;
		int						xs, xe, ys, ye;
		int						xl, yl, xsl;
		int						area, area_sum;
		int						x_offset_out = 40;
		int						x_offset_in  = 8;   //2 //<K5> 4 -> 3
		int						y_offset	 = 48;  //24 //<z3>32
		int						xsize, ysize;		//, lsize;
		int						ans, bn;
		int						judge_area; 	    //, judge_l;
		int						i_mode	= R_CS_GetInspectMode();

		// 外形ポジション代入
		Ap_pos[4]=Elec[DataLR].CornerPosY[2];	//Right Upper
		Ap_pos[5]=Elec[DataLR].CornerPosY[3];	//Right Lower


		// Judge data
			 //judge_area		= Judge_C1_Mz[2] * Judge_C1_Mz[3] * 2;
			 judge_area		= InspPara.ElecC1Mezumari[TWA_Spec].nXsize * InspPara.ElecC1Mezumari[TWA_Spec].nYsize * 2;
//			 judge_l		= Judge_C1_Mz[2] + Judge_C1_Mz[3];

		// Inspection set-up
			// right ys, yc, ye position
				ys = Ap_pos[4]+y_offset;
				if( ys == 0 ) return(ERROR); //<0001> hang up
				ye = Ap_pos[5]-y_offset;
				if( ye == 0 ) return(ERROR); //<0001> hang up
				if( ys >= ye ) return ERROR;
			// right xs position

			// xsl = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-ys); <K5>
				xsl = G2_pos[1]; //9499 <K5>
				xs = xsl + x_offset_in;
			// right xe position

				//xe = Ap_pos[1] - x_offset_out; //<K5>
				 xe = G2_pos[1] + x_offset_out - 10; //<K5> -8

				//２値化（グレープレーンから２値プレーンへ）
				xl = xe - xs + 1;
				yl = ye - ys + 1;
				area = abs(xl * yl);
				if( area == 0 ) return(ERROR); //<0001> hang up

				R_gray2bin16( Bin_Mz_l[TWA_Spec], xs, ys, xl, yl, Gray_address[grayPn], Bin_address[binPn] ); // <K5>

				// Insp. Area and
			// view
				if( i_mode & INSPECT_STEP ) {
//					R_display_control( BIN_MEMORY | GRAY_MEMORY );
					R_bin_cls( 0, 0, FxSize-1, FySize-1 );
					R_move_bin_memory(  binPn, 0, xs, ys, xe, ye );
					R_box( xs, ys, xe, ye, GRAPH_REVERSE, SOLID_LINE );
					DrawMessage( "Insp. [C1 MezuRight]", DRAW_GREEN, 2, 2 );
				}
			// 2 value measurement
				R_set_meas_configration( &Mpx, 0, 5, RM_BLACK|RM_WHITE, RM_AREA );
				if( R_measure_fast( &Mpx, binPn, xs, ys, xl, yl, 0, 3 ) != OK ) {
					R_measure_close( &Mpx );
					R_measure_open( &Mpx, 2000 );
					return ERROR;
				}
			// Judge
				ans = OK;
				area_sum = 0;
				for( bn=1; bn<=Mpx.bn; bn++ ) {
					xsize = Mpx.imax[bn] - Mpx.imin[bn] + 1;
					if( xsize == 0 ) return(ERROR); //<0001> hang up
					ysize = Mpx.jmax[bn] - Mpx.jmin[bn] + 1;
					if( ysize == 0 ) return(ERROR); //<0001> hnag up
					if( Mpx.area[bn] > 20 ) area_sum += Mpx.area[bn];

					if( i_mode & INSPECT_STEP ) {
						// x line
						ys = Mpx.center_y[bn];
						xs = Mpx.imin[bn];		xe = Mpx.imax[bn];
						R_line( xs, ys, xe, ys, GRAPH_REVERSE, SOLID_LINE );
						// y line
						xs = Mpx.center_x[bn];
						ys = Mpx.jmin[bn];		ye = Mpx.jmax[bn];
						R_line( xs, ys, xs, ye, GRAPH_REVERSE, SOLID_LINE );
						sprintf( Comment1, "Xsize:%3d(bit)  Ysize:%3d(bit)", xsize, ysize );
						sprintf( Comment2, "%s,Area :%5d(bit)", Comment1,Mpx.area[bn] );
						DrawMessage( Comment2, DRAW_GREEN, 2, 2 );
					}
					if( xsize >= InspPara.ElecC1Mezumari[TWA_Spec].nXsize ) {
					  if( xsize == 0 ) return(ERROR); //<0001> hang up
					  if( ysize >= InspPara.ElecC1Mezumari[TWA_Spec].nYsize ) {
						 if( ysize == 0 ) return(ERROR); //<0001> hang up
							ans = ERROR;
							break;
						}
					}
/* //<0001>
					lsize = xsize + ysize;
//					printf( "lsizeL:%d\n", lsize );	//<0001>
					if( lsize >= judge_l ) {
						ans = ERROR;
						break;
					}
 */ //<0001>

 //<0001>
					if( area_sum > judge_area ) {
					  if( area_sum == 0 ) return(ERROR); //<0001> hang up
						ans = ERROR;
						if( i_mode & INSPECT_STEP ) {
							sprintf( Comment, "                 " );
							sprintf( Comment2, "Area :%5d(bit)", area_sum );
							DrawMessage( Comment2, DRAW_GREEN,2, 2 );
						}
						break;
					}
 //<0001>

				}

			return ans;

	}	// TWA_Judge_Mzright( int binPn )




	// TWA_Judge_Mz( void ) end

//-------------------<z3>-------------------//

/************************************************
	C1 Mezumari <z3>
************************************************/
/* ********************************************************************************
int		TWA_Judge_MzLTR_l18( void )
	{

		//C1 Mezumari（Left）
			if( TWA_Judge_Mzleft() != OK )	return ERROR;
		//C1 Mezumari（Right）
			if( TWA_Judge_Mzright() != OK )	return ERROR;

		return OK;

	}	// TWA_Judge_Mz( void ) end
********************************************************************************* */

/************************************************
	C1 Mezumari Filtering <z1>
************************************************/

/*
static int		TWA_Mezu_Area( void )
	{
		int						grayPn	= InspGrayPn;
		int						binPn	= InspBinPn;
		int						le_xs, le_xe, le_xl, le_ys, le_ye, le_yl;
		int						ri_xs, ri_xe, ri_xl, ri_ys, ri_ye, ri_yl;
		int						xl_limit = 5;
		int						xu_limit = TWA_X_size - 5;
		int						yl_limit = min(Elec[DataLR].CornerPosY[0],Elec[DataLR].CornerPosY[2])-50;
		int						yu_limit = max(Elec[DataLR].CornerPosY[1],Elec[DataLR].CornerPosY[3])+50;
		int						level;
		int						pt[10];
		int						i_mode	= R_CS_GetInspectMode();

		// 外形ポジション代入
		Ap_pos[2]=Elec[DataLR].CornerPosY[0];	//Left Upper
		Ap_pos[3]=Elec[DataLR].CornerPosY[1];	//Left Lower
		Ap_pos[4]=Elec[DataLR].CornerPosY[2];	//Right Upper
		Ap_pos[5]=Elec[DataLR].CornerPosY[3];	//Right Lower

		// initial
			BinClsPn( binPn, 0, 0, FX_SIZE-1, FY_SIZE-1 );
			GrayClsPn( BASE3_GRAYADDRESS, 0, 0, FX_SIZE-1, FY_SIZE-1 );

		// It becomes smooth.     //平滑化
			// left
				le_xs = Ap_pos[0] - 20;			if( le_xs < xl_limit ) le_xs = xl_limit;
//				le_xe = G2_pos[0] + 10;			if( le_xe > xu_limit ) le_xe = xu_limit;
				le_xe = G2_pos[0];				if( le_xe > xu_limit ) le_xe = xu_limit;	//
				le_ys = Ap_pos[2] - 20;			if( le_ys < yl_limit ) le_ys = yl_limit;	//
				le_ye = Ap_pos[3] + 20;			if( le_ye > yu_limit ) le_ye = yu_limit;	//
				pt[0] = TWA_SmoothingXsizeMzO;	pt[1] = TWA_SmoothingYsizeMzO;
				pt[2] = le_xs;					pt[3] = le_ys;
				pt[4] = le_xe;					pt[5] = le_ye;
				TWA_smoothing( InspGrayPn, BASE3_GRAYADDRESS, pt );
			// right
//				ri_xs = G2_pos[1] - 10;			if( ri_xs < xl_limit ) ri_xs = xl_limit;
				ri_xs = G2_pos[1];				if( ri_xs < xl_limit ) ri_xs = xl_limit;	//
				ri_xe = Ap_pos[1] + 20;			if( ri_xe > xu_limit ) ri_xe = xu_limit;	//
				ri_ys = Ap_pos[4] - 20;			if( ri_ys < yl_limit ) ri_ys = yl_limit;	//
				ri_ye = Ap_pos[5] + 20;			if( ri_ye > yu_limit ) ri_ye = yu_limit;	//
				pt[0] = TWA_SmoothingXsizeMzO;	pt[1] = TWA_SmoothingYsizeMzO;
				pt[2] = ri_xs;					pt[3] = ri_ys;
				pt[4] = ri_xe;					pt[5] = ri_ye;
				TWA_smoothing( InspGrayPn, BASE3_GRAYADDRESS, pt );
			// view
				if( i_mode & INSPECT_STEP ) {
					GrayClsPn( GrayP0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					BinClsPn( binPn, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					R_move_gray_memory( grayPn, GrayP0, 0, 0, TWA_X_size-1, TWA_Y_size-1 );
					WaitMouseClick();
				}

		// Surfboard filter      //サーフボードフィルター
			// left
				pt[0] = le_xs;							pt[1] = le_ys;
				pt[2] = le_xe;							pt[3] = le_ye;
				pt[4] = TWA_CoefficientMzO;								// level up Magnification   //level up 倍率
				pt[5] = 0;	// 1;										// 均 maximum value : 1     //均最大値 : 1
				TWA_surf_board( grayPn, BASE3_GRAYADDRESS, BASE3_GRAYADDRESS, pt, TWA_SurfLoMzO );
			// right
				pt[0] = ri_xs;							pt[1] = ri_ys;
				pt[2] = ri_xe;							pt[3] = ri_ye;
				pt[4] = TWA_CoefficientMzO;								// level up 倍率
				pt[5] = 0;	// 1;										// 均最大値 : 1
				TWA_surf_board( grayPn, BASE3_GRAYADDRESS, BASE3_GRAYADDRESS, pt, TWA_SurfLoMzO );
			// view
				if( i_mode & INSPECT_STEP ) {
					GrayClsPn( GrayP0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					BinClsPn( binPn, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					R_move_gray_memory( BASE3_GRAYADDRESS, GrayP0, 0, 0, TWA_X_size-1, TWA_Y_size-1 );
					WaitMouseClick();
				}

		// Gray to Bin
			// left
				le_xl = le_xe-le_xs+1;		le_yl = le_ye-le_ys+1;
				level = Bin_C2XsizeLevelMz;
				R_gray2bin( level, le_xs, le_ys, le_xl, le_yl, Gray_address[BASE3_GRAYADDRESS], Bin_address[binPn] );
			// right
				ri_xl = ri_xe-ri_xs+1;		ri_yl = ri_ye-ri_ys+1;
				level = Bin_C2XsizeLevelMz;
				R_gray2bin( level, ri_xs, ri_ys, ri_xl, ri_yl, Gray_address[BASE3_GRAYADDRESS], Bin_address[binPn] );
			// view
				if( i_mode & INSPECT_STEP ) {
					R_display_control( BIN_MEMORY | GRAY_MEMORY );
					R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
					R_move_gray_memory( grayPn, GrayP0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					R_move_bin_memory(  binPn, BinP0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					WaitMouseClick();
				}

		//ノイズ除去処理
			// left
				pt[0] = le_xs;					pt[1] = le_ys;
				pt[2] = le_xe;					pt[3] = le_ye;
				TWA_expand2(  binPn, Pl_expand_x,  Pl_expand_y,  pt );
				TWA_contrct2( binPn, Pl_contrct_x, Pl_contrct_y, pt );
				TWA_contrct2( binPn, Pl_contrct_x, Pl_contrct_y, pt );
				TWA_expand2(  binPn, Pl_expand_x,  Pl_expand_y,  pt );
///x*
				TWA_expand2(  binPn, 1, Pl_expand_y,  pt );
				TWA_contrct2( binPn, 1, Pl_contrct_y, pt );
				TWA_contrct2( binPn, 1, Pl_contrct_y, pt );
				TWA_expand2(  binPn, 1, Pl_expand_y,  pt );
// /x*
			// right
				pt[0] = ri_xs;					pt[1] = ri_ys;
				pt[2] = ri_xe;					pt[3] = ri_ye;
				TWA_expand2(  binPn, Pl_expand_x,  Pl_expand_y,  pt );
				TWA_contrct2( binPn, Pl_contrct_x, Pl_contrct_y, pt );
				TWA_contrct2( binPn, Pl_contrct_x, Pl_contrct_y, pt );
				TWA_expand2(  binPn, Pl_expand_x,  Pl_expand_y,  pt );
///x*
				TWA_expand2(  binPn, 1, Pl_expand_y,  pt );
				TWA_contrct2( binPn, 1, Pl_contrct_y, pt );
				TWA_contrct2( binPn, 1, Pl_contrct_y, pt );
				TWA_expand2(  binPn, 1, Pl_expand_y,  pt );
// /x*
			// view
				if( i_mode & INSPECT_STEP ) {
					R_display_control( BIN_MEMORY | GRAY_MEMORY );
					R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
					R_move_gray_memory( grayPn, GrayP0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					R_move_bin_memory(  binPn, BinP0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					WaitMouseClick();
				}

			// Judge C1 Mezumari	<z3>
		if( TWA_Judge_Mz() != OK ) return( 17 );	//<z3>

		return OK;

	}	// TWA_Omotte_edge( void ) end
*/

//================================================
//		電極間検査
//================================================
static	unsigned int	Judge_ElSpace[4]={200,1000,17,83};									// 電極間サイズ[Xum,Yum,Xbit,Ybit]

int		LTR_Judge_C1Space_l18( void )
	{
		int						grayPn	= InspGrayPn;
		int						binPn	= InspBinPn;
		int						level;
		int						limitXmin = ElecPosRough[DataUL][PosXmin]+1;	///5;
		int						limitXmax = ElecPosRough[DataUL][PosXmax]-1;
		int						leXmin, leXmax, leYmin, leYmax;
		int						riXmin, riXmax, riYmin, riYmax;
		int						sizeX, sizeY;
		int						rx;
		int						iy, ry, rl;
		int						yl, ylmax, leYspace, riYspace;
		int						res;
		int						pt[10];
		int						i_mode	= R_CS_GetInspectMode();

		// initial
			BinClsPn( binPn, 0, 0, FxSize-1, FySize-1 );
		// Insp. level
///			level = (Bin_G2_l[TWA_Spec]+C1_level)/2;
			level = C1_level+10;
			if( level > 250 )	level = 250;
		// Insp. area
			// --- left X
			leXmin = min( Elec[0].CornerPosX[PosLup], Elec[1].CornerPosX[PosLlo] )-5;
			if( leXmin < limitXmin )	leXmin = limitXmin;
			leXmax = max( Elec[0].CornerPosX[PosLup], Elec[1].CornerPosX[PosLlo] )+Judge_ElSpace[DataXbit];
			// --- right X
			riXmax = max( Elec[0].CornerPosX[PosRup], Elec[1].CornerPosX[PosRlo] )+5;
			if( riXmax > limitXmax )	riXmax = limitXmax;
			riXmin = min( Elec[0].CornerPosX[PosRup], Elec[1].CornerPosX[PosRlo] )-Judge_ElSpace[DataXbit];
			// --- left and right Y
			leYmin =  Elec[0].CornerPosY[PosLup];
			leYmax =  Elec[1].CornerPosY[PosLlo];
			riYmin =  Elec[0].CornerPosY[PosRup];
			riYmax =  Elec[1].CornerPosY[PosRlo];

		// Gray to Bin
			// left
			sizeX = leXmax-leXmin+1;
			sizeY = leYmax-leYmin+1;
			R_gray2bin( level, leXmin, leYmin, sizeX, sizeY, Gray_address[grayPn], Bin_address[binPn] );
			// right
			sizeX = riXmax-riXmin+1;
			sizeY = riYmax-riYmin+1;
			R_gray2bin( level, riXmin, riYmin, sizeX, sizeY, Gray_address[grayPn], Bin_address[binPn] );
			// view
				if( i_mode & INSPECT_STEP ) {
					R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
					R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
					R_box( leXmin, leYmin, leXmax, leYmax, GRAPH_DRAW, SOLID_LINE );
					R_box( riXmin, riYmin, riXmax, riYmax, GRAPH_DRAW, SOLID_LINE );
                    sprintf( Comment, "Gray to Bin Insp. bin level:%d", level );
                    DrawMessage(  Comment, DRAW_YELLOW, 2, 2 );
				}

		// ノイズ除去処理
			// left
				pt[PosXmin] = leXmin;		pt[PosYmin] = leYmin;
				pt[PosXmax] = leXmax;		pt[PosYmax] = leYmax;
				TWA_expand2(  binPn, 1, Pl_expand_y,  pt );
				TWA_contrct2( binPn, 3, Pl_contrct_y, pt );
				TWA_contrct2( binPn, 1, Pl_contrct_y, pt );
				TWA_expand2(  binPn, 3, Pl_expand_y,  pt );
			// right
				pt[PosXmin] = riXmin;		pt[PosYmin] = riYmin;
				pt[PosXmax] = riXmax;		pt[PosYmax] = riYmax;
				TWA_expand2(  binPn, 1, Pl_expand_y,  pt );
				TWA_contrct2( binPn, 3, Pl_contrct_y, pt );
				TWA_contrct2( binPn, 1, Pl_contrct_y, pt );
				TWA_expand2(  binPn, 3, Pl_expand_y,  pt );
			// view
				if( i_mode & INSPECT_STEP ) {
					R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
					R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
					R_box( leXmin, leYmin, leXmax, leYmax, GRAPH_DRAW, SOLID_LINE );
					R_box( riXmin, riYmin, riXmax, riYmax, GRAPH_DRAW, SOLID_LINE );
                    DrawMessage( "Elec. space insp.", DRAW_YELLOW, 2, 2 );
				}

		// Check elec. space
			// --- left
			yl = 0;
			ylmax = 0;
			for( iy=leYmin; iy<=leYmax; iy++ ) {
				ry = iy;
				rx = leXmin;
				rl = leXmax-leXmin+1;
				if( TWA_es_bin_search( binPn, &rx, &ry, 0, rl, TWA_ES_WHITE ) != OK )	yl++;
				else																	yl=0;
				ylmax = max( ylmax, yl );
			}
			leYspace = ylmax;
			// --- right
			yl = 0;
			ylmax = 0;
			for( iy=riYmin; iy<=riYmax; iy++ ) {
				ry = iy;
				rx = riXmin;
				rl = riXmax-riXmin+1;
				if( TWA_es_bin_search( binPn, &rx, &ry, 0, rl, TWA_ES_WHITE ) != OK )	yl++;
				else																	yl=0;
				ylmax = max( ylmax, yl );
			}
			riYspace = ylmax;
			// view
				if( i_mode & INSPECT_STEP ) {
					R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
					R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
					R_box( leXmin, leYmin, leXmax, leYmax, GRAPH_DRAW, SOLID_LINE );
					R_box( riXmin, riYmin, riXmax, riYmax, GRAPH_DRAW, SOLID_LINE );
					sprintf( Comment1, "Left  size:%3dbit(%3d)", leYspace, Judge_ElSpace[DataYbit] );
                    sprintf( Comment2, "%s Right size:%3dbit(%3d)", Comment1, riYspace, Judge_ElSpace[DataYbit] );
                    DrawMessage( Comment2, DRAW_YELLOW, 2, 2 );
				}

		// Judge
			if( leYspace < Judge_ElSpace[DataYbit] )		res = ERROR;
			else if( riYspace < Judge_ElSpace[DataYbit] )	res = ERROR;
			else											res = OK;

			return( res );

	}	// LTR_Judge_C1Space_l18( void ) end

/************************************************
	外形からＧ２までの寸法判定
	Ｇ２回帰直線からＧ２までの寸法判定
************************************************/
extern int				G2StdEdgeLe[2];										// Ｇ２カケ判定基準エッジデータ
extern int				G2StdEdgeRi[2];										// Ｇ２カケ判定基準エッジデータ

int		G2_Judge_w_SDlLTR_l18( void )
	{
		int						n, ns, ne;
		int						judge_sd, judge_sd_n;
		int						bad_n;
		int						rx, ry, ix, std_x;
		int						xs, xe, ys, ye, G2_u, G2_l;
		int						i_mode	= R_CS_GetInspectMode();

//		judge_sd   = InspPara.G2YSdl[TWA_Spec].nXsize;				//Judge_G2W_SDtoG2[TWA_Spec][3]
//		judge_sd_n = InspPara.G2YSdl[TWA_Spec].nYsize / ES_G2_yp;	//Judge_G2W_SDtoG2[TWA_Spec][2]
		judge_sd   = InspPara.OCoatXsize[TWA_Spec].nXsize;				//Judge_OCoatXsize[]TWA_Spec][0]
		judge_sd_n = InspPara.OCoatXsize[TWA_Spec].nYsize / ES_G2_yp;	//Judge_G2W_SDtoG2[TWA_Spec][1]
		if( judge_sd_n <= 0 ) judge_sd_n = 1;

		G2_u = Elec[0].CornerPosY[1];
		G2_l = Elec[1].CornerPosY[0];
		// G2 Left
			// data n
				ns = 0;
				for( n=0; n<80; n++ ) {
					if( G2_data_l[n][0] == 0xffff ) break; //9499
					if((G2_u < G2_data_l[n][1])&& ns == 0 ){		//2021.10.22<shio>
//						ns = 1;
						ns = n;
					}
					if( ns != 0 &&(G2_l > G2_data_l[n][1])){
						ne = n-1;
					}
				}
				if( ns >= ne ) return ERROR;
				if( i_mode & INSPECT_STEP ) {
					R_DrawCls();
					ys = G2_data_l[ns][1];
					ye = G2_data_l[ne][1];
					xs = G2StdEdgeLe[0]-hidari.kaiki_b*(G2StdEdgeLe[1]-ys);
					xe = G2StdEdgeLe[0]-hidari.kaiki_b*(G2StdEdgeLe[1]-ye);
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_REVERSE, SOLID_LINE );
					R_mouse_de_yesno();
					xs = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-ys);
					xe = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-ye);
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_REVERSE, DOTTED_LINE );
					R_mouse_de_yesno();
				}

			// Judge  SD - G2
				bad_n = 0;
				for( n=ns; n<ne; n++ ) {
					rx = G2_data_l[n][0];
					ry = G2_data_l[n][1];
//					std_y = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-rx);
					std_x = G2StdEdgeLe[0]-hidari.kaiki_b*(G2StdEdgeLe[1]-ry);
					ix = rx - std_x;
					if( ix < 0 ) ix = 0;
					if( ix > judge_sd ) {
						bad_n++;
						if( i_mode & INSPECT_STEP ) {
							R_DrawLine( DRAW_GREEN, std_x, ry, rx, ry, DRAW_REVERSE, SOLID_LINE );
						}
						if( bad_n >= judge_sd_n ) {
							// disp
								if( i_mode & INSPECT_STEP ) {
									R_DrawLine( DRAW_RED, std_x, ry, rx, ry, DRAW_REVERSE, SOLID_LINE );
									R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30   ,"[G2_Judge_w_SDl Le]");
									R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30+2 ,"ix=%d    > judge_sd=%d"   ,ix,judge_sd);
									R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30+3 ,"bad_n=%d > judge_sd_n=%d" ,bad_n,judge_sd_n);
									R_mouse_de_yesno();
									R_DrawCls();
								}
							return ERROR2;
						}
					} else {
						bad_n = 0;
					}
				}

		// G2 Right
		G2_u = Elec[0].CornerPosY[3];
		G2_l = Elec[1].CornerPosY[2];
			// data n
				for( n=0; n<80; n++ ) {
					if( G2_data_r[n][0] == 0xffff ) break;
					if((G2_u < G2_data_l[n][1])&& ns == 0 ){				//2021.10.22<shio>
//						ns = 1;
						ns = n;
					}
					if( ns != 0 &&(G2_l > G2_data_l[n][1])){
						ne = n-1;
					}
				}
				if( ns >= ne ) return ERROR;

				if( i_mode & INSPECT_STEP ) {
					ys = G2_data_r[ns][1];
					ye = G2_data_r[ne][1];
					xs = G2StdEdgeRi[0]-migi.kaiki_b*(G2StdEdgeRi[1]-ys);
					xe = G2StdEdgeRi[0]-migi.kaiki_b*(G2StdEdgeRi[1]-ye);
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_REVERSE, SOLID_LINE );
					xs = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-ys);
					xe = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-ye);
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_REVERSE, DOTTED_LINE );
				}
			// Judge  SD - G2
				bad_n = 0;
				for( n=ns; n<ne; n++ ) {
					rx = G2_data_r[n][0];
					ry = G2_data_r[n][1];
//					std_y = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-rx);
					std_x = G2StdEdgeRi[0]-migi.kaiki_b*(G2StdEdgeRi[1]-ry);
					ix = std_x - rx;
					if( ix < 0 ) ix = 0;
					if( ix > judge_sd ) {
						bad_n++;
						// disp
						if( i_mode & INSPECT_STEP ) {
							R_DrawLine( DRAW_GREEN, rx, ry, std_x, ry, DRAW_REVERSE, SOLID_LINE );
						}
						if( bad_n >= judge_sd_n ) {
							if( i_mode & INSPECT_STEP ) {
								R_DrawLine( DRAW_RED, rx, ry, std_x, ry, DRAW_REVERSE, SOLID_LINE );
								R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30   ,"[G2_Judge_w_SDl Ri]");
								R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30+2 ,"ix=%d    > judge_sd=%d"   ,ix,judge_sd);
								R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30+3 ,"bad_n=%d > judge_sd_n=%d" ,bad_n,judge_sd_n);
								R_mouse_de_yesno();
								R_DrawCls();
							}
							return ERROR3;
						}
					} else {
						bad_n = 0;
					}
				}
		if( i_mode & INSPECT_STEP ) {
			R_mouse_de_yesno();
			R_DrawCls();
		}

		// judge end

		return OK;

	}	// G2_Judge_w_SDl( void ) end

