#include <stdlib.h>
#include <math.h>
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
#include "R_cs.h"
#include "R_cdraw.h"
#include "R_cursor.h"
#include "R_robust.h"

#include "tmcr00i.h"
#include "tp_sub.h"
#include "mcr03.h"
#include "mark03x6.h"

#define Mark_Debug_M03  0

extern  MACHINE_PARAMETER	SystemPara;
extern  INSPECT_PARAMETER	InspPara, InspParaM;



// --- Chip 傾き ---
extern  int        		ChipAtan;       				// Chip 角度×１０の値（アークタンジェント）
// --- Inspection Inage(Gray)
extern int				GetGrayPn;                  	// Get capture		gray memory
extern int				InspGray3Pn;					// Inspection		gray memory
extern int				InspGray4Pn;					// Inspection		gray memory
// --- Inspection Inage(Bin)
extern int				InspBinPn;						// Inspection		bin memory
extern Regression_def  ElecEdge[3][8];
// Electrode and Overcoat data
extern Overcoat_data	Elec[3];

//仕様 ( 0:通常 1:表裏 2:ピンホール検査 3:パイロット )
extern  int				TWA_Insp_mode;                        				//仕様 ( 0:通常 1:表裏 2:ピンホール検査 3:パイロット )
extern  int				TWA_Spec;                         					//製品仕様( 0:Normal 1:NP3 2:Jpw)
extern  int				TWA_Insp_level;             						//検査モード（ 0:通常 1:詳細 ）
extern  int				TWA_Mark_Insp;          							//標印検査

        int             G2_pos[4];											//Ｇ２位置検出データ( lex, rix, upy, loy )
        int             Ap_pos[6];											//外形位置検出データ( lex, rix, leuy, lely, riuy, rily )
static	int         	max_st[5][2], max_re[5][2];

//標印検査
extern	int				Mark_Xseg[50][5];									//文字切り出し結果 X[ judge, xs, xe, level, 誤差 ]
extern	int				Mark_Yseg[50][5];									//文字切り出し結果 Y[ judge, ys, ye, level, 誤差 ]
extern	int				Mark_zure[4];//		= { 0,0,0,0 };					//標印ズレdata
extern	int				Mark_area_xl;										//標印文字範囲( X size )
extern	int				Mark_area_yl;										//標印文字範囲( Y size )
static	int				Mark_Slevel[5][3];									//標印文字有り無し判断レベル( min, center, max )
extern	int				Mark_div_x;											//標印文字分割数 X
extern	int				Mark_div_y;											//標印文字分割数 Y
extern	int				Mark_div_ws[4][50];									//標印文字分割Data(Bin正方向)White
static	int				Mark_div_ws1[5][50];								//標印文字分割Data(Bin正方向)White
extern	int				Mark_div_wr[4][50];									//標印文字分割Data(Bin逆方向)White
extern	int				Mark_div_bs[4][50];									//標印文字分割Data(Bin正方向)Black
static	int				Mark_div_bs1[5][50];								//標印文字分割Data(Bin正方向)Black
extern	int				Mark_div_br[4][50];									//標印文字分割Data(Bin逆方向)Black
extern	int				Mark_area_x[125][4][4];								//標印文字範囲X ([θ][桁][x1,x2,x3,x4])
extern	int				Mark_area_y[125][4][4];								//標印文字範囲Y ([θ][桁][y1,y2,y3,y4])
extern	int				Mark_space_x[125][5][10];							//標印文字間隔X ([θ][桁+1][x1,x2,,..])
extern	int				Mark_space_y[125][5][10];							//標印文字間隔Y ([θ][桁+1][y1,y2,,..])
extern	int				Mark_size_ls[5][10][2];								//標印文字Size L White
extern	int				Mark_size_lr[5][10][2];								//標印文字Size L White
//static	int				Mark_space_l[5][10];							//標印文字間隔Data(gray)
extern	int				Mark_rx[125][4][30];								//標印検査ポイントX([θ][桁][Xdata])
extern	int				Mark_ry[125][4][30];								//標印検査ポイントY([θ][桁][Ydata])
static	int				Mark_rx_J[50];										//標印検査ポイントX([Xdata])JPW
static	int				Mark_ry_J[50];										//標印検査ポイントY([Ydata])JPW
extern	int				Mark_Judge_st[5][NumMarkChar][5];							//標印文字判定結果[桁][文字][一致,Check,定点]
extern	int				Mark_Judge_re[5][NumMarkChar][5];							//標印文字判定結果[桁][文字][一致,Check,定点]
static	int				Nijimi_Judge[5];									//標印文字判定結果[桁][文字][一致,Check,定点]
extern	int				Mark_L_set[5];//= { 0,0,0,0,0 };					//標印詳細検査設定（0:検査無し 1:詳細検査）
extern	int				Mark_Ans_st[5];
extern	int				Mark_Ans_re[5];
extern	int				Mark_J100_st[5];
extern	int				Mark_J100_re[5];
extern	int				Mark_Insp_mode; //= 0x0000;							//標印詳細検査設定bit（1桁目 0x01, 2桁目 0x02, 3桁目0x04, 4桁目0x08）
static  int				Mark_pos_le;
static  int				Mark_pos_ri;
static  int				Diff_mark_seg;
extern  int				mark_zure1;	//= ERROR;
extern  int				mark_zure2;
extern  int				mark_zure3;
extern  int				mark_zure4;

static int				so_st[5][12][4], so_re[5][12][4];

//標印
extern char				MarkChar[10];

extern  int				TWA_R_data[6];										//設定抵抗値（数値）
extern  int				TWA_R_n;                    						//抵抗値文字数 F/J

extern	int				TWA_mark_area_p[8];									//標印文字範囲
//static int			TWA_Judge_Ma		= ERROR;						//判定結果
#define	Insp_Pinhole	2

#define TWA_Ma			3

static  int             Mark_temp[6];
static	int				Bin_Ma_l_ini[5][2]			= {{ 40, 50},{ 45, 75},{ 65, 75},{ 45, 75},{ 45, 75}};	//標印検査時の２値化レベル設定範囲
static	int				Bin_Ma_l_ini_rist[5][2]		= {{ 70, 90},{ 75, 95},{ 75, 95},{ 75, 95},{ 75, 95}};	//標印検査時の２値化レベル設定範囲
static	int				Bin_Ap_l[5]			= { 115, 115, 115, 115, 115};		//外形検査時の２値化レベル
static	int				Bin_Ap_l_ini[5][2]	= {{ 110, 120},{ 110, 120},{ 110, 120},{ 110, 120},{ 110, 120}};	// 外形検査時の２値化レベル設定範囲
//Auto bin level														//jig add search auto bin level for search sigmentation
static  int				Low_level_resin			= 10;							// Test
static  int				High_level_resin		= 240;							// Test
static  int				Low_level_glass			= 10;							// Test
static  int				High_level_glass		= 240;							// Test
static  int             check_st[11];
static  int             check_re[11];

extern  char			SpecData[20];										// バーコードデータ
extern	int				Index_pos[6];										//インデックステーブル　エッジ位置( uy, dy, ly )
        int				ChipBlobNo;                                     	// Chip blob No.( bin measure )

extern  int             Center_C;                                           //GEE V1.02A
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

static int	TWA_G2_level( int *level, int *l_lo, int *l_up );
static int	TWA_segmentation( int deg10, int level, int n_seg, int offset_x, int offset_y );
static int	TWA_Mark_level( int deg10 );
static int	TWA_Mark_point( int deg10, int ox, int oy, int ol, int nc  );
static int	TWA_Mark_Judge( int nc );
void	TWA_gray2bin( int level, int xs, int ys, int xl, int yl, int gn, int bn );
static void	TWA_get_color_area( int page, int pt[], int *jw, int *jb, int nc );
static void	TWA_get_color_area_nijimi( int page, int pt[], int *jw, int *jb, int nc );
static void	TWA_Mark_error( int nc );
static void	TWA_Mark_J_error( int nc );
static void	TWA_Nijimi_error( int nc );
static void	TWA_mark_position_set( void );
int		ChipEsLevelSet( void );
void	ChipGray2Bin( void );
int		Regression( int elec, int side );
void	TWA_Mark_Size( int page, int pt[], int *ls, int *le );
void	TWA_Mark_div_check( int pt[], int *nst, int *nre, int *sum );
static void  Mark_point_disp(int deg10,int nc,int ox, int oy);

#define KURO			0
#define SHIRO			1

#define TWA_ES_BLACK	0
#define TWA_ES_WHITE	1

#define TWA_ES_p1		0x0100
#define TWA_ES_p2		0x0200

static	unsigned int	ES_G2_xp			= 8;							//Ｇ２検査幅
//static	unsigned int	ES_G2_yp			= 4;							//Ｇ２検査幅
static	unsigned int	APG2_data_u[80];									//AP Pos to Ｇ２エッジ検査データ（Ｘ，Ｙ）上      2011.04.19
static	unsigned int	APG2_data_d[80];									//AP Pos to Ｇ２エッジ検査データ（Ｘ，Ｙ）下      2011.04.19
		unsigned int	G2_data_u[80][2];									//Ｇ２エッジ検査データ（Ｘ，Ｙ）上
		unsigned int	G2_data_d[80][2];									//Ｇ２エッジ検査データ（Ｘ，Ｙ）下
		unsigned int	G2_data_r[80][2];									//Ｇ２エッジ検査データ（Ｘ，Ｙ）右
		unsigned int	G2_data_l[80][2];									//Ｇ２エッジ検査データ（Ｘ，Ｙ）左

		unsigned int	ES_Ap_xp			= 8;							//外形検査幅
		unsigned int	ES_Ap_yp			= 8;							//外形検査幅
		unsigned int	Ap_data_u[80][2];									//外形検査データ（Ｘ，Ｙ）上
		unsigned int	Ap_data_d[80][2];									//外形検査データ（Ｘ，Ｙ）下
		unsigned int	Ap_data_r[80][2];									//外形検査データ（Ｘ，Ｙ）右
		unsigned int	Ap_data_l[80][2];									//外形検査データ（Ｘ，Ｙ）左
		int				Ap_ue_ey;											//外形検査上 Ｙデータ平均値
		int				Ap_shita_ey;										//外形検査下 Ｙデータ平均値
		int				Ap_r_ex;											//外形検査右 Ｘデータ平均値
		int				Ap_l_ex;											//外形検査左 Ｘデータ平均値

static	int				Ap_contrct_x		= 3;							//外形縮退Ｘサイズ
static	int				Ap_contrct_y		= 3;							//外形縮退Ｙサイズ
static	int				Ap_expand_x			= 3;							//外形膨張Ｘサイズ
static	int				Ap_expand_y			= 3;							//外形膨張Ｙサイズ



//回帰係数設定用
KAIKI		ue, shita, migi, hidari, w, l, Ap_up, Ap_lo, Ap_ri, Ap_le, Ap_w, Ap_l;
//平均値・標準偏差
MEANSD		up_y, lo_y, le_x, ri_x, Ap_up_y, Ap_lo_y, Ap_le_x, Ap_ri_x;


// Filter C2 Insp.
        int				TWA_SurfLoC2			= 20;	//10;
        int				TWA_SmoothingXsizeC2	= 10;	//5;
        int				TWA_SmoothingYsizeC2	= 10;	//5;                 //GEE 3 to  5
        int				TWA_CoefficientC2		= 10;
        int				Bin_C2XsizeLevel		= 110;

static	int				Corner_pos[10];										// 外形角位置データ( le_xu, le_yu, le_xl, le_yl, ri_xu, ri_yu, ri_xl, ri_yl )
static	unsigned int  	FreqData[1000];
static	unsigned int	C2_data_lu[640][2];
static	unsigned int	C2_data_lo[640][2];
static	unsigned int	C2_data_ru[640][2];
static	unsigned int	C2_data_rl[640][2];

static	int				Pl_contrct_x		= 3;							//メッキ付着縮退Ｘサイズ
static	int				Pl_contrct_y		= 3;							//メッキ付着縮退Ｙサイズ
static	int				Pl_expand_x			= 3;							//メッキ付着膨張Ｘサイズ
static	int				Pl_expand_y			= 3;							//メッキ付着膨張Ｙサイズ
static	int				Bin_C2_Loffset		= 110;	//110 JHONG
// Filter C1 Insp.
static	int				C1_level			= 220;

		int				Bin_Mz_l[5]			= { 50, 108, 50, 50, 50};					//C1 mezumari Binary level			//<z3>
// Mezumari Inspection.
static	int				TWA_SurfLoMzO			= 10; 			//<z3>
static	int				TWA_SmoothingXsizeMzO	= 7; 		//<z3>
static	int				TWA_SmoothingYsizeMzO	= 3; 		//<z3>
static	int				TWA_CoefficientMzO		= 10; 	//<z3>
static	int				Bin_C2XsizeLevelMz		= 85;		//<z3>
extern  int				mox[4],moy[4];	//標印のオフセットメモ用


static	int				G2StdEdgeUp[2];										// Ｇ２カケ判定基準エッジデータ
static	int				G2StdEdgeLo[2];										// Ｇ２カケ判定基準エッジデータ
		int				G2StdEdgeLe[2];										// Ｇ２カケ判定基準エッジデータ
		int				G2StdEdgeRi[2];										// Ｇ２カケ判定基準エッジデータ
static	int				G2StdOffsetX[2]		= { 200, 27 };						// Ｇ２ダレ除外サイズ
static	int				G2StdOffsetY[2]		= {   0,  0 };						// Ｇ２ダレ除外サイズ
//static	unsigned int	Judge_Zure_Wmin[4] = {450, 10, 61,  1};		//Ｇ２Ｌ方向ズレ最小値[um,bit]
//static	unsigned int	Judge_Zure_Wmax[4] = {450,300, 61, 40};		//Ｇ２Ｌ方向ズレ最小値[um,bit]
//static	unsigned int	Judge_Zure_Lmin[4] = {150,450, 20, 61};		//Ｇ２Ｌ方向ズレ最小値[um,bit]
//static	unsigned int	Judge_G2W_SDtoG2[4][4]
//										   ={{150, 66, 20,  9},
//											 {150, 66, 20,  9},
//											 {150, 66, 20,  9},
//											 {150, 66, 20,  9}};	// W 回帰直線 - G2 data[TWA_Spec][um,bit]
//static	unsigned int	Judge_G2W_APtoG2[4][4]
//                                           ={{250, 95, 34, 13},
//                                             {250, 95, 34, 13},
//                                             {250, 95, 34, 13},
//                                             {250, 95, 34, 13}};	// W Ap data -- G2 data[TWA_Spec][um,bit]
extern int g2level;

// --- ロバスト推定条件構造体
extern ROBUST_CONDITION		RobustCond;

extern R_POINT					*Src_point;
extern R_POINT					*Src_point_adr;
extern R_POINT					*Dst_point;
extern R_POINT					*Dst_point_adr;
extern int						RobustDegWidth;
extern int						RobustDegStep;


/************************************************
	標印検査
************************************************/

int		TWA_kensa_m03( void )
	{
		int				deg10;
		int				i, nc;
		int				level, l_lo, l_ce, l_up;
		int				ans[3];
		int				hx, hy, hf, hfx, hfy, ox, oy;
		int				hl_x, hl_y;
		int				off_px, off_py;
		int	 			n_seg, n_x, n_y;
		int				x_seg, y_seg;
		int				seg_offset_x, seg_offset_y;
//		int	 			offset_Mal[3] = { 0, 5, -5 };
//		int				offset_Mal[5] = { 0, 5, -5, 8, -8 };                      //GEE 8.93
		int				offset_Mal[7] = { 0, 5, -5, 8, -8, 10, -10 };                      //GEE 8.93
		int				nm;
		int             ans0_st, ans1_st, ans2_st;
		int             ans0_re, ans1_re, ans2_re;
//		int             St[3], Re[3];
		R_RECT					area;
		int						i_mode	= R_CS_GetInspectMode();

		//標印検査有り無し
			if( TWA_Mark_Insp != 0 ) return(OK);

		//標印文字検査
			if((TWA_Spec!=SpecJPW)&&(TWA_Insp_mode==Insp_Pinhole)) return(OK);		//メッキ付着検査

		//標印傾き計算
			deg10 = TWA_katamuki();
			i = (300+deg10)/5;
			if((i<0)||(i>120)) return(ERROR);
		//Mark Level
			ans[0] = ans[1] = ans[2] = ERROR;
			level = 0;
//			if(TWA_G2_level( &l_ce, &l_lo, &l_up ) != OK)	return(ERROR5);
			TWA_G2_level( &l_ce, &l_lo, &l_up );
		//文字切り出し
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
					if( i_mode & INSPECT_STEP ) {
						sprintf(Comment,"level = %d",level);
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					}
					TWA_segmentation( deg10, level, n_seg, seg_offset_x, seg_offset_y );
					if( Mark_Xseg[n_seg][0] == OK ) {
		//				if( x_seg > Mark_Xseg[n_seg][4]) {
		//					x_seg = Mark_Xseg[n_seg][4];
		//					n_x = n_seg;
						if( TWA_Spec == SpecResin ) {
							if((Mark_pos_le == OK) || (Mark_pos_ri == OK)) {  		//Jig search auto segmentation
								n_x = n_seg;
							}
						}
						else{
							if((Mark_pos_le == OK) && (Mark_pos_ri == OK)) {  		//Jig search auto segmentation
								n_x = n_seg;
				   			}	
						}
				  	}
				 	if( Mark_Yseg[n_seg][0] == OK ) {
				 		if( y_seg > Mark_Yseg[n_seg][4]) {
				 			y_seg = Mark_Yseg[n_seg][4];
				 			n_y = n_seg;
				 		}
				 	}
			
/*			 		if( i_mode & INSPECT_STEP ) {
					sprintf(Comment,"level = %3d,%3d,%3d",level,l_lo,l_up);
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );	
					sprintf(Comment,"Xseg/Yseg/pos_le/pos_ri/diff = %3d,%3d,%3d,%3d,%3d",Mark_Xseg[n_seg][0], Mark_Yseg[n_seg][0],Mark_pos_le,Mark_pos_ri,Diff_mark_seg);
		 			DrawMessage( Comment, DRAW_GREEN, 2, 3 );
					sprintf(Comment,"|| = %3d",(Mark_pos_le,Mark_pos_ri||Diff_mark_seg));
		 			DrawMessage( Comment, DRAW_GREEN, 2, 4 );
					sprintf(Comment,"sum = %3d",(( Mark_Xseg[n_seg][0] == OK )&&( Mark_Yseg[n_seg][0] == OK )&&((Mark_pos_le == OK)||(Mark_pos_ri == OK ))&& (Diff_mark_seg == OK)));
		 			DrawMessage( Comment, DRAW_GREEN, 2, 5 );
					R_DrawCls();
		 			}
*/
				if( TWA_Spec == SpecResin ) {
					
			 	  	if(( Mark_Xseg[n_seg][0] == OK )&&( Mark_Yseg[n_seg][0] == OK )&&((Mark_pos_le == OK)||(Mark_pos_ri == OK ))&& (Diff_mark_seg == OK)){
					 break;
					}
				}
				else{
					if(( Mark_Xseg[n_seg][0] == OK )&&( Mark_Yseg[n_seg][0] == OK )&&(Mark_pos_le == OK)&&(Mark_pos_ri == OK )&& (Diff_mark_seg == OK)){
					 break;
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

		//文字レベル
				ox = oy = 0;
			if( TWA_Mark_level( deg10 ) != OK ) {
				if( TWA_Insp_mode != Insp_Pinhole ) {
					return(ERROR3);
				} else {
					return(OK);
				}
			}

			for( nc=0; nc<TWA_R_n; nc++ ) {
				Mark_Ans_st[nc] = ERROR;
				Mark_Ans_re[nc] = ERROR;
				Mark_J100_st[nc] = ERROR;
				Mark_J100_re[nc] = ERROR;
			}

//			if( TWA_Spec != SpecJPW )
			if( TWA_R_n > 1 ) {
			//	hl_x = abs((Ma_pos[1]-Ma_pos[0])-Mark_area_xl);
				hl_x =    ((Ma_pos[1]-Ma_pos[0])-Mark_area_xl);
				if( hl_x <= 0 ) hl_x = 0;
				else			hl_x /= 2;
				hl_x += 2;
				if( Mark_Insp_mode == 0 ) {
					if( hl_x < 4 ) hl_x = 4;
				}

			//	hl_y = abs((Ma_pos[3]-Ma_pos[2])-Mark_area_yl);
				hl_y =    ((Ma_pos[3]-Ma_pos[2])-Mark_area_yl);
				if( hl_y <= 0 ) hl_y = 0;
				else			hl_y /= 2;
				hl_y += 2;
				if( Mark_Insp_mode == 0 ) {
					if( hl_y < 4 ) hl_y = 4;
				}

				off_px = off_py = 2;
			} else {
				hl_x = abs((Ma_pos[1]-Ma_pos[0])-Mark_area_xl);
				if( hl_x <= 0 ) hl_x = 0;
				else			hl_x /= 2;
				hl_x += 3;
				hl_y = abs((Ma_pos[3]-Ma_pos[2])-Mark_area_yl);
				if( hl_y <= 0 ) hl_y = 0;
				else			hl_y /= 2;
				hl_y += 3;
				off_px = off_py = 3;
			}

			for( nm=0; nm<11; nm++ ) {
				check_st[nm] = check_re[nm] = 0;
			}

			ans[0] = ans[1] = ans[2] = ERROR;
//			for(nc=0; nc<TWA_R_n; nc++){
//				max_st[nc][0] = max_re[nc][0] = 0;
//				max_st[nc][1] = max_re[nc][1] = ERROR;
//			}
//			for( nm=0; nm<5; nm++ ) {                                                //GEE 8.93
			for( nm=0; nm<7; nm++ ) {                                                //GEE 8.93

				ans[0] = ans[1] = ans[2] = ERROR;
//				St[0] = St[1] = St[2] = 0;
//	 			Re[0] = Re[1] = Re[2] = 0;

///////////////////////////////
//        filter             //
///////////////////////////////
		 	if( TWA_Spec == SpecNormal ) {											
        //		R_move_gray_memory( 2, 4, 0, 0, FxSize-1, FySize-1 );							//JIG^^ Addition min filter
        //		R_move_gray_memory( 1, 5, 0, 0, FxSize-1, FySize-1 );							//JIG^^ Addition min filter
		//		R_min_filter(2, 1 , Ma_pos[0]-5,Ma_pos[2]-5,Ma_pos[1]+5,Ma_pos[3]+5);
                GrayClsPn(BASE3_GRAYADDRESS,0, 0, FxSize-1, FySize-1);
		//		R_min_filter(2,4 , Ma_pos[0]-5,Ma_pos[2]-5,Ma_pos[1]+5,Ma_pos[3]+5);
				R_min_filter(InspGrayPn, InspGray3Pn , Ma_pos[0]-5,Ma_pos[2]-5,Ma_pos[1]+5,Ma_pos[3]+5);
				if( i_mode & INSPECT_STEP ) {
                    R_gray_fill( 0, 0, FxSize-1, FySize-1, GrayP0 );
                    R_bin_cls( 0, 0, FxSize-1, FySize-1 );
                    R_move_gray_memory( InspGray3Pn,GrayP0, 0, 0, FxSize-1, FySize-1 );
	 				sprintf(Comment,"min filter");
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					R_DrawCls();
				}
		//		R_max_filter(1, 2 , Ma_pos[0]-5,Ma_pos[2]-5,Ma_pos[1]+5,Ma_pos[3]+5);					//JIG^^ Addition max filter
                GrayClsPn(BASE4_GRAYADDRESS,0, 0, FxSize-1, FySize-1);
	 			R_max_filter(InspGray3Pn,InspGray4Pn , Ma_pos[0]-5,Ma_pos[2]-5,Ma_pos[1]+5,Ma_pos[3]+5);					//JIG^^ Addition max filter
	  	 		if( i_mode & INSPECT_STEP ) {
                    R_gray_fill( 0, 0, FxSize-1, FySize-1,GrayP0 );
                    R_bin_cls( 0, 0, FxSize-1, FySize-1 );
                    R_move_gray_memory( InspGray4Pn,GrayP0, 0, 0, FxSize-1, FySize-1 );
	 				sprintf(Comment,"max filter");
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					sprintf( Comment,"TWA_R_data[0]=%d\nTWA_R_data[1]=%d\nTWA_R_data[2]=%d",TWA_R_data[0],TWA_R_data[1],TWA_R_data[2]);
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					R_DrawCls();
				}
			}
			else{
        //		R_move_gray_memory( 2, 4, 0, 0, FxSize-1, FySize-1 );							//JIG^^ Addition min filter
        //		R_move_gray_memory( 1, 5, 0, 0, FxSize-1, FySize-1 );
	 	//		R_min_filter(2, 1 , Ma_pos[0]-5,Ma_pos[2]-5,Ma_pos[1]+5,Ma_pos[3]+5);
                GrayClsPn(BASE3_GRAYADDRESS,0, 0, FxSize-1, FySize-1);
	  	 		R_min_filter(InspGrayPn, InspGray3Pn, Ma_pos[0]-5,Ma_pos[2]-5,Ma_pos[1]+5,Ma_pos[3]+5);
                if( i_mode & INSPECT_STEP ) {
                    R_gray_fill( 0, 0, FxSize-1, FySize-1, GrayP0 );
                    R_bin_cls( 0, 0, FxSize-1, FySize-1 );
                    R_move_gray_memory( InspGray3Pn,GrayP0, 0, 0, FxSize-1, FySize-1 );
	   				sprintf(Comment,"min filter");
		 			DrawMessage( Comment, DRAW_GREEN, 2, 2 );
	 				R_DrawCls();
				}
		//		R_max_filter(1, 2 , Ma_pos[0]-5,Ma_pos[2]-5,Ma_pos[1]+5,Ma_pos[3]+5);					//JIG^^ Addition max filter
                GrayClsPn(BASE4_GRAYADDRESS,0, 0, FxSize-1, FySize-1);
				R_max_filter(InspGray3Pn,InspGray4Pn , Ma_pos[0]-5,Ma_pos[2]-5,Ma_pos[1]+5,Ma_pos[3]+5);					//JIG^^ Addition max filter
                if( i_mode & INSPECT_STEP ) {
                    R_gray_fill( 0, 0, FxSize-1, FySize-1, GrayP0 );
                    R_bin_cls( 0, 0, FxSize-1, FySize-1 );
                    R_move_gray_memory(InspGray4Pn,GrayP0, 0, 0, FxSize-1, FySize-1 );
					sprintf(Comment,"max filter");
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					R_DrawCls();
				}
			}
			
            if( i_mode & INSPECT_STEP ) {
                R_move_gray_memory(InspGrayPn,GrayP0, 0, 0, FxSize-1, FySize-1 );
            }

///////////////////////////////////////
//         Digit 0                   //
///////////////////////////////////////
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
						max_st[0][0] = max_re[0][0] = 0;
						max_st[0][1] = max_re[0][1] = ERROR;
				 		Mark_J100_st[0] = ERROR;
						Mark_J100_re[0] = ERROR;
						ans0_st = ans0_re = ERROR;
						//標印データ読み取り
						if( TWA_Mark_point( deg10, ox, oy, offset_Mal[nm], 0 ) == OK ) {
							//文字判定
							if( TWA_Mark_Judge(0) == OK ) {
								ans[0] = OK;
								break;
							}
							if((Mark_Ans_st[0] == OK)&&(ans[0] == OK)) {ans0_st = OK;    ans0_re = ERROR;}
							if((Mark_Ans_re[0] == OK)&&(ans[0] == OK)) {ans0_st = ERROR; ans0_re = OK;}
						}
					}
						if( ans[0] == OK ) break;
				}

            if( i_mode & INSPECT_STEP )  Mark_point_disp(deg10, 0, ox, oy);

///////////////////////////////////////
//         Digit 1                   //
///////////////////////////////////////
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
						max_st[1][0] = max_re[1][0] = 0;
						max_st[1][1] = max_re[1][1] = ERROR;
						Mark_J100_st[1] = ERROR;
						Mark_J100_re[1] = ERROR;
						ans1_st = ans1_re = ERROR;
                        //標印データ読み取り
						if( TWA_Mark_point( deg10, ox, oy, offset_Mal[nm], 1 ) == OK ) {
							//文字判定
							if( TWA_Mark_Judge(1) == OK ) {
								ans[1] = OK;
								break;
							}
							if((Mark_Ans_st[1] == OK)&&(ans[1] == OK)) {ans1_st = OK;    ans1_re = ERROR;}
							if((Mark_Ans_re[1] == OK)&&(ans[1] == OK)) {ans1_st = ERROR; ans1_re = OK;}
						}
					}
						if( ans[1] == OK ) break;
				}
                if( i_mode & INSPECT_STEP )  Mark_point_disp(deg10, 1, ox, oy);
/*
                        int i,xs,xe,ys,ye,nd;
                    int x0,y0,div_n,x,y;
                    int nc = 1;
                    R_RECT area;
                    //標印傾き確認
                    i = (300+deg10)/5;
                    //文字エリア中心
                    x0 = (Ma_pos[0]+Ma_pos[1])/2;
                    y0 = (Ma_pos[2]+Ma_pos[3])/2;
                    x0 += ox;
                    y0 += oy;
                    //文字エリア
                    xs = x0 + Mark_area_x[i][nc][0];
                    xe = x0 + Mark_area_x[i][nc][3];
                    ys = y0 + Mark_area_y[i][nc][0];
                    ye = y0 + Mark_area_y[i][nc][2];
                    area.left	= xs;
                    area.right	= xe;
                    area.top	= ys;
                    area.bottom	= ye;
                    R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );

                    div_n = Mark_div_x * Mark_div_y;
                    for( nd=0; nd<div_n; nd++ ) {
                        //検査座標設定
                        if( TWA_R_n == 1 ) {	//ＪＰＷ検査データ
                            x = Mark_rx_J[nd];
                            y = Mark_ry_J[nd];
                        } else {
                            x = Mark_rx[i][nc][nd];
                            y = Mark_ry[i][nc][nd];
                        }
                        x += x0;
                        y += y0;
                        xs = x - 5;
                        xe = x + 4;
                        ys = y - 5;
                        ye = y + 4;
                        if( Mark_div_ws[nc][nd] == 1 ) {
                            area.left	= xs;
                            area.right	= xe;
                            area.top	= ys;
                            area.bottom	= ye;
                            R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
                        }
                        if( Mark_div_bs[nc][nd] == 0 ) {
                            area.left	= x;
                            area.right	= x;
                            area.top	= ys;
                            area.bottom	= ye;
                            R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
                            area.left	= xs;
                            area.right	= xe;
                            area.top	= y;
                            area.bottom	= y;
                            R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
                        }
                    }
                    for( nd=0; nd<Mark_div_y; nd++ ) {
                    //検査座標設定
                        if( TWA_R_n == 1 ) {	//ＪＰＷ検査データ
                            x = Mark_space_x[i][nc][nd];
                            y = Mark_space_y[i][nc][nd];
                        } else {
                            x = Mark_space_x[i][nc][nd];
                            y = Mark_space_y[i][nc][nd];
                        }
                        x += x0;
                        y += y0;
                        xs = x - 5;
                        xe = x + 4;
                        ys = y - 5;
                        ye = y + 4;
                        if( Mark_div_ws1[nc][nd] == 1 ) {
                            area.left	= xs;
                            area.right	= xe;
                            area.top	= ys;
                            area.bottom	= ye;
                            R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
                        }
                        if( Mark_div_bs1[nc][nd] == 0 ) {
                            R_line( x, ys, x, ye, GRAPH_DRAW, SOLID_LINE );
                            R_line( xs, y, xe, y, GRAPH_DRAW, SOLID_LINE );
                        }
                    }
                }
*/

///////////////////////////////////////
//         Digit 2                   //
///////////////////////////////////////
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
						max_st[2][0] = max_re[2][0] = 0;
						max_st[2][1] = max_re[2][1] = ERROR;
						Mark_J100_st[2] = ERROR;
						Mark_J100_re[2] = ERROR;
						ans2_st = ans2_re = ERROR;
                        //標印データ読み取り
						if( TWA_Mark_point( deg10, ox, oy, offset_Mal[nm], 2 ) == OK ) {
							//文字判定
							if( TWA_Mark_Judge(2) == OK ) {
								ans[2] = OK;
								break;
							}
					 		if((Mark_Ans_st[2] == OK)&&(ans[2] == OK)) {ans2_st = OK;    ans2_re = ERROR;}
							if((Mark_Ans_re[2] == OK)&&(ans[2] == OK)) {ans2_st = ERROR; ans2_re = OK;}
						}
					}
						if( ans[2] == OK ) break;
				}
				if( i_mode & INSPECT_STEP )  Mark_point_disp(deg10, 2, ox, oy);

//#ifdef Mark_DEBUG_M03

                if( i_mode & INSPECT_STEP ) {
                    int posi_res_x,posi_res_y,ncs,ncr;
                    int Nijimi;
                    R_display_control( BIN_MEMORY | GRAY_MEMORY );
               //     R_bin_cls( 0, 0, FxSize-1, FySize-1 );
               //     R_move_gray_memory(grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );

                    Nijimi = ERROR;
                    if( TWA_R_n > 1 ) {
                        for( ncs=0; ncs<TWA_R_n; ncs++ ) {
                            if((Nijimi_Judge[ncs] == OK))  Nijimi = OK;
                            if( TWA_Spec != SpecJPW ) {
                                posi_res_x = ncs * 31;
                                posi_res_y = 30;
                                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"Rst : %d  Nijimi : %2d",ncs,Nijimi);
                                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+1 ,"%2d:%3d(%2d)%3d %2d:%3d(%2d)%3d",
                                                            so_st[ncs][0][0], so_st[ncs][0][1], so_st[ncs][0][2], so_st[ncs][0][3],
                                                            so_st[ncs][1][0], so_st[ncs][1][1], so_st[ncs][1][2], so_st[ncs][1][3]);
                                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+2 ,"%2d:%3d(%2d)%3d %2d:%3d(%2d)%3d",
                                                            so_st[ncs][2][0], so_st[ncs][2][1], so_st[ncs][2][2], so_st[ncs][2][3],
                                                            so_st[ncs][3][0], so_st[ncs][3][1], so_st[ncs][3][2], so_st[ncs][3][3]);
                                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+3 ,"%2d:%3d(%2d)%3d %2d:%3d(%2d)%3d",
                                                            so_st[ncs][4][0], so_st[ncs][4][1], so_st[ncs][4][2], so_st[ncs][4][3],
                                                            so_st[ncs][5][0], so_st[ncs][5][1], so_st[ncs][5][2], so_st[ncs][5][3]);
                                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+4 ,"%2d:%3d(%2d)%3d %2d:%3d(%2d)%3d",
                                                            so_st[ncs][6][0], so_st[ncs][6][1], so_st[ncs][6][2], so_st[ncs][6][3],
                                                            so_st[ncs][7][0], so_st[ncs][7][1], so_st[ncs][7][2], so_st[ncs][7][3]);
                                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+5 ,"%2d:%3d(%2d)%3d %2d:%3d(%2d)%3d",
                                                            so_st[ncs][8][0], so_st[ncs][8][1], so_st[ncs][8][2], so_st[ncs][8][3],
                                                            so_st[ncs][9][0], so_st[ncs][9][1], so_st[ncs][9][2], so_st[ncs][9][3]);
                                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+6 ,"%2d:%3d(%2d)%3d max:%2d(%3d)",
                                                            so_st[ncs][10][0], so_st[ncs][10][1], so_st[ncs][10][2], so_st[ncs][10][3],
                                                            max_st[ncs][1], max_st[ncs][0] );


                                ncr = TWA_R_n - ncs - 1;
                                posi_res_x = ncr * 31;
                                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+7 ,"Rre : %d  Nijimi : %2d",ncr,Nijimi);
                                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+8 ,"%2d:%3d(%2d)%3d %2d:%3d(%2d)%3d",
                                                            so_re[ncr][0][0], so_re[ncr][0][1], so_re[ncr][0][2], so_re[ncr][0][3],
                                                            so_re[ncr][1][0], so_re[ncr][1][1], so_re[ncr][1][2], so_re[ncr][1][3]);
                                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+9 ,"%2d:%3d(%2d)%3d %2d:%3d(%2d)%3d",
                                                            so_re[ncr][2][0], so_re[ncr][2][1], so_re[ncr][2][2], so_re[ncr][2][3],
                                                            so_re[ncr][3][0], so_re[ncr][3][1], so_re[ncr][3][2], so_re[ncr][3][3]);
                                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+10 ,"%2d:%3d(%2d)%3d %2d:%3d(%2d)%3d",
                                                            so_re[ncr][4][0], so_re[ncr][4][1], so_re[ncr][4][2], so_re[ncr][4][3],
                                                            so_re[ncr][5][0], so_re[ncr][5][1], so_re[ncr][5][2], so_re[ncr][5][3]);
                                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+11 ,"%2d:%3d(%2d)%3d %2d:%3d(%2d)%3d",
                                                            so_re[ncr][6][0], so_re[ncr][6][1], so_re[ncr][6][2], so_re[ncr][6][3],
                                                            so_re[ncr][7][0], so_re[ncr][7][1], so_re[ncr][7][2], so_re[ncr][7][3]);
                                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+12 ,"%2d:%3d(%2d)%3d %2d:%3d(%2d)%3d",
                                                            so_re[ncr][8][0], so_re[ncr][8][1], so_re[ncr][8][2], so_re[ncr][8][3],
                                                            so_re[ncr][9][0], so_re[ncr][9][1], so_re[ncr][9][2], so_re[ncr][9][3]);
                                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+13 ,"%2d:%3d(%2d)%3d max:%2d(%3d)",
                                                            so_re[ncr][10][0],so_re[ncr][10][1],so_re[ncr][10][2],so_re[ncr][10][3],
                                                            max_re[ncr][1], max_re[ncr][0] );
                            }
                        }
                        WaitMouseClick();
                        R_DrawCls();
                    } else {
                        sprintf( Comment, "0:%3d(%2d)%2d       Nijimi : %2d", Mark_Judge_st[nc][0][0], Mark_Judge_st[nc][0][1], Mark_Judge_st[nc][0][2], Nijimi );
                        DrawMessage( Comment, DRAW_GREEN, 2, 2);
                        R_DrawCls();
                    }
                }
            //#endif


        //		R_move_gray_memory( 4, 2, 0, 0, FxSize-1, FySize-1 );
        //		R_move_gray_memory( 5, 1, 0, 0, FxSize-1, FySize-1 );

				if(TWA_R_n  == 1){
					if(( Mark_Ans_st[0] == OK )){
						if((ans[0] == OK )){
							if((TWA_R_data[0] == max_st[0][1])){
								return OK;
							}
						}
					}
					if(( Mark_Ans_re[0] == OK )){
						if((ans[0]==OK)){
							if((TWA_R_data[2] == max_re[0][1])){
								return OK;
							}
						}
					}
				}
			if(( Mark_Ans_st[0] == OK )&&( Mark_Ans_st[1] == OK )&&( Mark_Ans_st[2] == OK )){
				if((ans[0] == OK )&&(ans[1] == OK)&&(ans[2]==OK)){
					if((TWA_R_data[0] == max_st[0][1])&&(TWA_R_data[1] == max_st[1][1])&&(TWA_R_data[2] == max_st[2][1])){
						return OK;
					}
				}
			}
			if(( Mark_Ans_re[0] == OK )&&( Mark_Ans_re[1] == OK )&&( Mark_Ans_re[2] == OK )){
				if((ans[0]==OK)&&(ans[1]==OK)&&(ans[2]==OK)){
					if((TWA_R_data[2] == max_re[0][1])&&(TWA_R_data[1] == max_re[1][1])&&(TWA_R_data[0] == max_re[2][1])){
						return OK;
					}
				}
			}
//			}
			}
		 		if( i_mode & INSPECT_STEP ) {
			 		sprintf( Comment, "mark0:%2d  mark1:%2d  mark2:%2d", Mark_Ans_st[0],Mark_Ans_st[1],Mark_Ans_st[2] );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					sprintf( Comment, "markr0:%2d markr1:%2d markr2:%2d", Mark_Ans_re[0],Mark_Ans_re[1],Mark_Ans_re[2] );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					sprintf( Comment, "ansst0:%2d ansst1:%2d ansst2:%2d", ans0_st, ans1_st, ans2_st );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					sprintf( Comment, "ansre0:%2d ansre1:%2d ansre2:%2d", ans0_re, ans1_re, ans2_re );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					sprintf( Comment, "ans0:%2d   ans1:%2d   ans2:%2d", ans[0], ans[1], ans[2] );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					R_DrawCls();
				}


		if( i_mode & INSPECT_STEP ) {
            R_bin_cls( 0, 0, FxSize-1, FySize-1 );
			if(ans[0] != OK) {
				area.left	= Mark_temp[0];
			 	area.right	= Mark_temp[1];
			 	area.top	= Mark_Yseg[0][1];
			 	area.bottom	= Mark_Yseg[0][2];
			 	R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
			}
			if(ans[1] != OK){
				area.left	= Mark_temp[2];
			 	area.right	= Mark_temp[3];
			 	area.top	= Mark_Yseg[0][1];
			 	area.bottom	= Mark_Yseg[0][2];
			 	R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
			}
			if(ans[2] != OK){
				area.left	= Mark_temp[4];
			 	area.right	= Mark_temp[5];
			 	area.top	= Mark_Yseg[0][1];
			 	area.bottom	= Mark_Yseg[0][2];
			 	R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
			}
				sprintf( Comment, "NG marking position");
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );


//			if((ans[0] == OK)&&(ans0_st == OK)&&((Mark_Ans_st[0]+Mark_Ans_st[1]+Mark_Ans_st[2])<0)) R_box(Mark_temp[0], Mark_Yseg[0][1], Mark_temp[1], Mark_Yseg[0][2],GRAPH_REVERSE, DASHED_LINE );
			if((ans[0] == OK)&&(ans0_st == OK)&&((Mark_Ans_st[0]+Mark_Ans_st[1]+Mark_Ans_st[2])<0));
				area.left	= Mark_temp[0];
			 	area.right	= Mark_temp[1];
			 	area.top	= Mark_Yseg[0][1];
			 	area.bottom	= Mark_Yseg[0][2];
			 	R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
//			if((ans[0] == OK)&&(ans0_re == OK)&&((Mark_Ans_re[0]+Mark_Ans_re[1]+Mark_Ans_re[2])<0)) R_box(Mark_temp[0], Mark_Yseg[0][1], Mark_temp[1], Mark_Yseg[0][2],GRAPH_REVERSE, DASHED_LINE );
			if((ans[0] == OK)&&(ans0_re == OK)&&((Mark_Ans_re[0]+Mark_Ans_re[1]+Mark_Ans_re[2])<0));
				area.left	= Mark_temp[0];
			 	area.right	= Mark_temp[1];
			 	area.top	= Mark_Yseg[0][1];
			 	area.bottom	= Mark_Yseg[0][2];
			 	R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
//			if((ans[1] == OK)&&(ans1_st == OK)&&((Mark_Ans_st[0]+Mark_Ans_st[1]+Mark_Ans_st[2])<0)) R_box(Mark_temp[2], Mark_Yseg[0][1], Mark_temp[3], Mark_Yseg[0][2],GRAPH_REVERSE, DASHED_LINE );
			if((ans[1] == OK)&&(ans1_st == OK)&&((Mark_Ans_st[0]+Mark_Ans_st[1]+Mark_Ans_st[2])<0));
				area.left	= Mark_temp[2];
			 	area.right	= Mark_temp[3];
			 	area.top	= Mark_Yseg[0][1];
			 	area.bottom	= Mark_Yseg[0][2];
			 	R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
//			if((ans[1] == OK)&&(ans1_re == OK)&&((Mark_Ans_re[0]+Mark_Ans_re[1]+Mark_Ans_re[2])<0)) R_box(Mark_temp[2], Mark_Yseg[0][1], Mark_temp[3], Mark_Yseg[0][2],GRAPH_REVERSE, DASHED_LINE );
			if((ans[1] == OK)&&(ans1_re == OK)&&((Mark_Ans_re[0]+Mark_Ans_re[1]+Mark_Ans_re[2])<0));
				area.left	= Mark_temp[2];
			 	area.right	= Mark_temp[3];
			 	area.top	= Mark_Yseg[0][1];
			 	area.bottom	= Mark_Yseg[0][2];
			 	R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
//			if((ans[2] == OK)&&(ans2_st == OK)&&((Mark_Ans_st[0]+Mark_Ans_st[1]+Mark_Ans_st[2])<0)) R_box(Mark_temp[4], Mark_Yseg[0][1], Mark_temp[5], Mark_Yseg[0][2],GRAPH_REVERSE, DASHED_LINE );
			if((ans[2] == OK)&&(ans2_st == OK)&&((Mark_Ans_st[0]+Mark_Ans_st[1]+Mark_Ans_st[2])<0));
//			if((ans[2] == OK)&&(ans2_re == OK)&&((Mark_Ans_re[0]+Mark_Ans_re[1]+Mark_Ans_re[2])<0)) R_box(Mark_temp[4], Mark_Yseg[0][1], Mark_temp[5], Mark_Yseg[0][2],GRAPH_REVERSE, DASHED_LINE );
			if((ans[2] == OK)&&(ans2_re == OK)&&((Mark_Ans_re[0]+Mark_Ans_re[1]+Mark_Ans_re[2])<0));
				area.left	= Mark_temp[4];
			 	area.right	= Mark_temp[5];
			 	area.top	= Mark_Yseg[0][1];
			 	area.bottom	= Mark_Yseg[0][2];
			 	R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );

				sprintf( Comment, "NG marking" );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );

		}
		return(ERROR);

	}	// TWA_kensa_m END


static void  Mark_point_disp(int deg10,int nc,int ox, int oy)
{
    int i,xs,xe,ys,ye,nd;
    int x0,y0,div_n,x,y;
    R_RECT area;
    //標印傾き確認
    i = (300+deg10)/5;
    //文字エリア中心
    x0 = (Ma_pos[0]+Ma_pos[1])/2;
    y0 = (Ma_pos[2]+Ma_pos[3])/2;
    x0 += ox;
    y0 += oy;
    //文字エリア
    xs = x0 + Mark_area_x[i][nc][0];
    xe = x0 + Mark_area_x[i][nc][3];
    ys = y0 + Mark_area_y[i][nc][0];
    ye = y0 + Mark_area_y[i][nc][2];
    area.left	= xs;
    area.right	= xe;
    area.top	= ys;
    area.bottom	= ye;
    R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );

    div_n = Mark_div_x * Mark_div_y;
    for( nd=0; nd<div_n; nd++ ) {
        //検査座標設定
        if( TWA_R_n == 1 ) {	//ＪＰＷ検査データ
            x = Mark_rx_J[nd];
            y = Mark_ry_J[nd];
        } else {
            x = Mark_rx[i][nc][nd];
            y = Mark_ry[i][nc][nd];
        }
        x += x0;
        y += y0;
        xs = x - 5;
        xe = x + 4;
        ys = y - 5;
        ye = y + 4;
        if( Mark_div_ws[nc][nd] == 1 ) {
            area.left	= xs;
            area.right	= xe;
            area.top	= ys;
            area.bottom	= ye;
            R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
        }
        if( Mark_div_bs[nc][nd] == 0 ) {
            area.left	= x;
            area.right	= x;
            area.top	= ys;
            area.bottom	= ye;
            R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
            area.left	= xs;
            area.right	= xe;
            area.top	= y;
            area.bottom	= y;
            R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
        }
    }
    for( nd=0; nd<Mark_div_y; nd++ ) {
    //検査座標設定
        if( TWA_R_n == 1 ) {	//ＪＰＷ検査データ
            x = Mark_space_x[i][nc][nd];
            y = Mark_space_y[i][nc][nd];
        } else {
            x = Mark_space_x[i][nc][nd];
            y = Mark_space_y[i][nc][nd];
        }
        x += x0;
        y += y0;
        xs = x - 5;
        xe = x + 4;
        ys = y - 5;
        ye = y + 4;
        if( Mark_div_ws1[nc][nd] == 1 ) {
            area.left	= xs;
            area.right	= xe;
            area.top	= ys;
            area.bottom	= ye;
            R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
        }
        if( Mark_div_bs1[nc][nd] == 0 ) {
            R_line( x, ys, x, ye, GRAPH_DRAW, SOLID_LINE );
            R_line( xs, y, xe, y, GRAPH_DRAW, SOLID_LINE );
        }
    }
}


/************************************************
	Ｇ２θ計算
************************************************/

int		TWA_katamuki( void )
	{
		int			x, y, deg10;

		x = 100;
//		y = w.kaiki_b * x;					//////////   check again
		y =10;
//		deg10 = R_atan( x, y );
		deg10 = ChipAtan;
		return( deg10 );
	}

/*********************************************************************************
*********************************************************************************/

static int	TWA_G2_level( int *level, int *l_lo, int *l_up )
	{
		int					xs, xe, ys, ye;
		int					cl, clmin, clcen, clmax;
		int					n;
		int					bright_n; 
		int					t_count, w_count, b_count;
		double				w_p, b_p;
		int 				ans=OK;
		int 		 		clbase;
		R_RECT				area;
		int					i_mode	= R_CS_GetInspectMode();
		int					grayPn	= InspGrayPn;
		int					binPn	= InspBinPn;
		int 				r_get;
		int 				elec =0;
		//検査範囲設定
//i_mode|=INSPECT_STEP;
//			xs = G2_pos[0] + 10;
//			xe = G2_pos[1] - 10;
//			ys = G2_pos[2]/2 + 5;
//			ye = G2_pos[3]/2 - 5;
            xs	= ElecPosRough[DataLeft][PosXmax]+20;
            xe	= ElecPosRough[DataRight][PosXmin]-20;
			ys	= ElecEdge[elec][PosYmin].MeanY-ElecEdge[elec][PosYmin].Rev*(ElecEdge[elec][PosYmin].MeanX-xs)+10;
			ye	= ElecEdge[elec][PosYmax].MeanY-ElecEdge[elec][PosYmax].Rev*(ElecEdge[elec][PosYmax].MeanX-xe)-10;

			if( i_mode & INSPECT_STEP ) {
				// --- area
				area.left	= xs;
				area.right	= xe;
				area.top	= ys;
				area.bottom	= ye;
				R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
				// --- comment
				sprintf( Comment, "G2 AREA" );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				R_DrawCls();
			}

		if( TWA_Insp_mode != Insp_Pinhole ) {                                   //GEE 8.93P
		//Mark Chcek
			TWA_gray2bin( Bin_Ma_l_ini[TWA_Spec][0], xs, ys, xe-xs+1, ((ye-ys)*2)+1, grayPn, binPn );
			w_count = R_count_bin_area( binPn, xs, ys*2, xe, ye*2 );					//check again
			t_count = (xe-xs+1)*(((ye-ys)*2)+1);
			if( t_count == 0 ) return(ERROR);
			b_count = t_count - w_count;
			w_p = w_count*100.0/t_count;
			b_p = b_count*100.0/t_count;
		}

		if( ERROR == R_bgray_thresholding_open( THRESH_DEPTH_8BIT )) return ERROR;					//JIG addition search light auto binary
		//指定エリアのヒストグラムを取る。
		r_get = R_get_histgram( xs, ys, xe, ye, Gray_address[grayPn] );
		if(TWA_Spec == SpecResin){			
		 	clbase = R_bgray_thresholding( Bright, Low_level_resin, High_level_resin, THRESH_OTSU );		//JIG addition search light auto binary
		} else {
			clbase = R_bgray_thresholding( Bright, Low_level_glass, High_level_glass, THRESH_OTSU );
		}
		R_bgray_thresholding_close();
			bright_n = 0;
			for( n=1; n<=255; n++ )	bright_n += Bright[n];
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
//				if( clmin < 20 ) clmin = 20;	//最低レベル 10
				//max level
				cl = 0;
				for( n=255; n>0; n-- ) {
					cl += Bright[n];
					if( cl > bright_n ) break;
				}
				clmax = n;
				if( clmin > clmax ) clmax = clmin;
				//level
				cl = (clmax-clmin)/5 + 1;
				clcen = (clmin+clmax)/2;			//center level				
				clmin = clbase - cl;				//min level				//change clcen to clbase
				*l_lo = clbase - (cl*4);			//min level     JIG^^			//change clcen to clbase
				if( *l_lo < Bin_Ma_l_ini[TWA_Spec][0] ) *l_lo = Bin_Ma_l_ini[TWA_Spec][0];
                *l_up = clbase + cl;				//max level				//change clcen to clbase
                if(*l_up < *l_lo+5) *l_up= *l_lo+5;
                *level = clbase;									//change clcen to clbase

				if( i_mode & INSPECT_STEP ) {
					sprintf( Comment, "r_get=%3d clbase=%3d",r_get,clbase );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					R_DrawCls();
				}

		 	//Manual
	 		if( i_mode & INSPECT_STEP ) {
				R_display_control( BIN_MEMORY | GRAY_MEMORY );
                R_bin_cls( 0, 0, FxSize-1, FySize-1 );
                R_move_gray_memory( InspGrayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				sprintf( Comment, "R MOVE GRAYpn to GRAY0" );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				area.left	= xs;
				area.right	= xe;
				area.top	= ys;
				area.bottom	= ye;
				R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
				sprintf( Comment, "box gray to bin \n xs=%2d,ys=%2d  xe=%2d,ye=%2d",xs,ys,xe,ye );
	 			DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				TWA_gray2bin( clbase, xs, ys, xe, ye,grayPn, binPn );
			 	R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				R_move_bin_memory( binPn, BinP0, xs, ys, xe, ye);
				sprintf( Comment, "xs=%2d,ys=%2d  xe=%2d,ye=%2d",xs,ys,xe,ye );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				sprintf(Comment,"clbase = %3d \n clcen = %3d",clbase,clcen);
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				sprintf(Comment, "Mark Seg.Bright min:%d  cen:%d  max:%d  n:%d\nMark Seg.Level  min:%d  cen:%d  max:%d\n ", clmin, clcen, clmax, bright_n, *l_lo, clcen, *l_up);
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
	 			R_DrawCls();


				printf( "Mark Seg.Bright min:%d  cen:%d  max:%d  n:%d\n", clmin, clcen, clmax, bright_n );
				printf( "Mark Seg.Level  min:%d  cen:%d  max:%d\n", *l_lo, clcen, *l_up );
			
			}
				return ans;
	}
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
        int				xp[640], yp[480];
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
		int          	nm, chk_mark, chk_error, dn;
		int             ans_seg;
		int             offset, offset_l;
        int             mark_le_diff,mark_ri_diff,width_seg;
        R_RECT			area;
		int				i_mode	= R_CS_GetInspectMode();
        int				grayPn	= InspGrayPn;
        int				binPn	= InspBinPn;
		int				elec = 0;

        if( TWA_R_n > 1 ) {
            if( TWA_Spec == SpecResin ) {
            //ＮＰーIII検査データ　３桁標印
				slx2 = 2;		//<-3 2017.04.17 tanaka
                sly = 5;
                snx = 5;
                sny = 5;
            } else {
            //通常品検査データ
                slx2 = 3;
                sly = 3;
                snx = 8;
                sny = 3;
            }
        } else {
            slx2 = 3;
            sly = 3;
            snx = 8;
            sny = 3;
        }

		//標印傾き確認
        i = (300+deg10)/5;
        if((i<0)||(i>120)) return(ERROR);

		for( n=0; n<640; n++ ) xp[n] = 0;
        for( n=0; n<480; n++ ) yp[n] = 0;

		//検査範囲設定
        xs		= ElecPosRough[DataLeft][2]+10;
        xe		= ElecPosRough[DataRight][0]-10;
		ys		= ElecEdge[elec][PosYmin].MeanY-ElecEdge[elec][PosYmin].Rev*(ElecEdge[elec][PosYmin].MeanX-xs)+10;
		ye 		= ElecEdge[elec][PosYmax].MeanY-ElecEdge[elec][PosYmax].Rev*(ElecEdge[elec][PosYmax].MeanX-xe)-10;
//		ys		= ElecEdge[elec][PosYmin].MeanY-ElecEdge[elec][PosYmin].Rev*(ElecEdge[elec][PosYmin].MeanX-xs)+20;
//		ye 		= ElecEdge[elec][PosYmax].MeanY-ElecEdge[elec][PosYmax].Rev*(ElecEdge[elec][PosYmax].MeanX-xe)-20;
		G2_pos[2]	= ys;
        G2_pos[3]	= ye;

        if( i_mode & INSPECT_STEP ) {
            area.left	= xs;
            area.right	= xe;
            area.top	= ys;
            area.bottom	= ye;
            R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
            sprintf( Comment, "inspec range setting" );
            DrawMessage( Comment, DRAW_GREEN, 2, 2 );
            R_DrawCls();
        }

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
            xs1	= ElecPosRough[DataLeft][2]+30;
            xe1	= ElecPosRough[DataRight][0]-30;
        }

        if( i_mode & INSPECT_STEP ) {
            sprintf( Comment, "Y level = %d",level );
            DrawMessage( Comment, DRAW_GREEN, 2, 2 );
        }

        for( y=ys, yn=0; y<=ye; y+=2, yn++ ) {
            sp_ad = Gray_address[grayPn]+MxSize*y;
            for( x=xs1; x<=xe1; x+=4 ) {
                l = *(sp_ad+x);
                if( l > level ){
                    yp[yn]++;			//一定レベル以下は、カウントしない
                }
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
                    y1 = (ys+n*2-2);
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
                y2 = (ys+yn*2);
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
         //   sprintf( Comment, "Mark Y position" );
         //   DrawMessage( Comment, DRAW_GREEN, 2, 2 );
        }

        //Y judge
        ans = OK;
        if( Mark_Yseg[n_seg][1] < G2_pos[2] ) ans = ERROR;
        if( Mark_Yseg[n_seg][2] > G2_pos[3] ) ans = ERROR;

        yl = Mark_Yseg[n_seg][2] - Mark_Yseg[n_seg][1];
        if( ans == OK ) {
            if( Mark_area_yl == 0 ) return(ERROR);                       //GEE Program hang
            y = abs( yl - Mark_area_yl )*100/Mark_area_yl;
            if( y < 30 ) {                  // 想定の30%以内 20170128
                Mark_Yseg[n_seg][0] = OK;
                Mark_Yseg[n_seg][3] = level;
                Mark_Yseg[n_seg][4] = y;
            }
        }

  //      if( i_mode & INSPECT_STEP ) {
  //          sprintf( Comment, "ans=%d\nok=%d error=%d",ans,OK,ERROR );
  //          DrawMessage( Comment, DRAW_GREEN, 2, 2 );
  //      }


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
            xs = G2_pos[0]+16+offset_x;     // 8x2 20170128
            xe = G2_pos[1]-16-offset_x;     // 8x2 20170128

            for( x=xs, xn=0; x<=xe; x+=4, xn++ ) {
                for( y=ys; y<=ye; y+=2 ) {
                    sp_ad = Gray_address[grayPn]+MxSize*y;
                    l = *(sp_ad+x);
                    if( l >  level ) xp[xn]++;		//一定レベル以下は、カウントしない
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
            if( TWA_R_n == 1 ) slx = xp_av*2/4;
            if( TWA_R_n > 1 )  slx = xp_av/6;
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

        ans_seg = ERROR;
        if(ans_seg == ERROR) {
        //Segmentation X axis 2010.08.04
            chk_mark = chk_error = nm = 0;
            Mark_temp[0] = Mark_temp[1] = Mark_temp[2] = Mark_temp[3] = Mark_temp[4] = Mark_temp[5] = ERROR;
            // サーチ最初から文字レベルがある場合読み飛ばす
            int z;
            for( n=nm; n<xn; n++ ) {
                for( n1=0; n1<2; n1++ ) {
                    z=xp[n+n1];
                    if( z > slx || z == 0) break;
                }
                if( n1 == snx || z == 0 ) {
                    nm = n;
                    break;
                }
            }
            if(n == xn) return ERROR;  //文字レベルの途切れが無い場合異常

            for(;;){
                for( n=nm; n<xn; n++ ) {
                    for( n1=0; n1<snx; n1++ ) {
                        if( xp[n+n1] < slx ) break;
                    }
                    if( n1 == snx ) {
                        Mark_temp[chk_mark] = xs+n*4;
                        nm = n;
                        break;
                    }
                }
                Mark_Xseg[n_seg][1] = Mark_temp[chk_mark];

                for( n=nm+1; n<xn; n++ ) {
                    if(TWA_R_n == 1) { if( xp[n] <= (slx/2) ){ nm = n; break;}}
					else             { if( xp[n] <= (slx*3)/4 && xp[n+1] <= (slx*3)/4){ nm = n; break;}}
                }
                Mark_temp[chk_mark+1] = xs+n*4;

                if(TWA_R_n == 1) offset = 110;                              // 55x2 20170128
                if((TWA_R_n > 1)&&(TWA_Spec == SpecNormal))  offset = 60;   // 30x2 20170128
                if((TWA_R_n > 1)&&(TWA_Spec == SpecResin))   offset = 70;   // 35x2 20170128
                if(TWA_Spec==SpecNormal) offset_l = 18;                     //  9x2 20170128
                if(TWA_Spec==SpecResin)  offset_l = 26;                     // 13x2 20170128
                if(((Mark_temp[chk_mark+1]-Mark_temp[chk_mark] ) > offset_l )&&((Mark_temp[chk_mark+1]-Mark_temp[chk_mark] ) < offset )){
                    chk_mark +=2;
                }else{
                    Mark_temp[chk_mark+1] = Mark_temp[chk_mark] = ERROR;
                    chk_error ++;
                }
                if( chk_error >= 10 ) break;
                if(( TWA_R_n > 1 )&&( chk_mark == 6 )) break;
                if(( TWA_R_n == 1)&&( chk_mark == 2 )) break;
            }
            if( TWA_R_n > 1 ) {
                Mark_Xseg[n_seg][1] = Mark_temp[0];
                Mark_Xseg[n_seg][2] = Mark_temp[5];
            }else{
                Mark_Xseg[n_seg][1] = Mark_temp[0];
                Mark_Xseg[n_seg][2] = Mark_temp[1];
            }

            if( TWA_R_n > 1 ) {
                for(n=0; n<6; n++){
                    if( Mark_temp[n] == ERROR ) {ans_seg = ERROR; break;}
                    if(n==5) ans_seg = OK;
                }
            }else{
                for(n=0; n<2; n++){
                    if( Mark_temp[n] == ERROR ) {ans_seg = ERROR; break;}
                    if(n==1) ans_seg = OK;
                }
            }
        }
  //      if( i_mode & INSPECT_STEP ) {
  //         sprintf( Comment , "Mark_temp[1]-[0] %3d ", Mark_temp[1]-Mark_temp[0] );
  //          DrawMessage( Comment, DRAW_GREEN, 2, 2 );
  //          sprintf( Comment , "Mark_temp[3]-[2] %3d ", Mark_temp[3]-Mark_temp[2] );
  //          DrawMessage( Comment, DRAW_GREEN, 2, 3 );
  //          sprintf( Comment , "Mark_temp[5]-[4] %3d ", Mark_temp[5]-Mark_temp[4] );
  //          DrawMessage( Comment, DRAW_GREEN, 2, 4 );
  //      }
        if( ans_seg == OK ){
            if( TWA_R_n > 1 ) {
                if(TWA_Spec == SpecNormal){
                    if(abs((Mark_temp[1]-Mark_temp[0])-(Mark_temp[3]-Mark_temp[2])) >= 16 ) return ERROR;   // 8x2 20170128tanaka
                    if(abs((Mark_temp[3]-Mark_temp[2])-(Mark_temp[5]-Mark_temp[4])) >= 16 ) return ERROR;   // 8x2 20170128tanaka
                }
            }else{
                if(abs(Mark_temp[1]-Mark_temp[0]) > 110 ) return ERROR;  // 55x2 20170128tanaka
                if(abs(Mark_temp[1]-Mark_temp[0]) < 30 ) return ERROR;   // 15x2 20170128tanaka
            }
        }
        if( ans_seg == ERROR ){

            //Xle
            for( n=0; n<xn; n++ ) {
                for( n1=0; n1<snx; n1++ ) {
					if(n1==0)	if( xp[n+n1] < slx ) break;	//2017.04.17 tanaka 細線対策
					else		if( xp[n+n1] < slx2) break;
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
					if(n1==0)	if( xp[n-n1] < slx ) break;		// 2017.04.17 tanaka
					else		if( xp[n-n1] < slx2) break;
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
        }

        if( i_mode & INSPECT_STEP ) {
            ys = Mark_Yseg[n_seg][1];
            ye = Mark_Yseg[n_seg][2];
            if( Mark_Xseg[n_seg][1]!=ERROR ) {
                xs = Mark_Xseg[n_seg][1];
                R_line( xs, ys, xs, ye, GRAPH_DRAW, SOLID_LINE );
             //   sprintf(Comment, "mark pos1" );
             //   DrawMessage( Comment, DRAW_GREEN, 2, 2 );
            }
            if( Mark_Xseg[n_seg][2]!=ERROR ) {
                xe = Mark_Xseg[n_seg][2];
                R_line( xe, ys, xe, ye, GRAPH_DRAW, SOLID_LINE );
             //   sprintf(Comment, "mark pos2" );
             //   DrawMessage( Comment, DRAW_GREEN, 2, 2 );
            }
        }


		/***************************************			
		*********test mark position*************			by JIG
		***************************************/
        mark_le_diff = mark_ri_diff = 0;
        mark_le_diff = mark_ri_diff = ERROR;
 
        mark_le_diff = abs( Mark_Xseg[n_seg][1]-G2_pos[0]);
        mark_ri_diff = abs(G2_pos[1]- Mark_Xseg[n_seg][2]);

        if( i_mode & INSPECT_STEP ) {
            sprintf(Comment, "mark_le_diff = %d \n mark_ri_diff =%d",mark_le_diff,mark_ri_diff );
            DrawMessage( Comment, DRAW_GREEN, 2, 2 );
        }

        if( TWA_Spec == SpecResin ){
            if(mark_le_diff >= 16){  //26 ){				//13x2 check again
                Mark_pos_le = OK;
            } else {
                Mark_pos_le = ERROR;
            }
 					
            if(mark_ri_diff >= 16){ // 26 ){				//13x2 check again
                Mark_pos_ri = OK;
            } else {
                Mark_pos_ri = ERROR;
            }
 					
        } else{
            if(mark_le_diff >= 30 ){				//15x2 check again
                Mark_pos_le = OK;
            } else {
                Mark_pos_le = ERROR;
            }
 					
            if(mark_ri_diff >= 30 ){				//15x2 check again
                Mark_pos_ri = OK;
            } else {
                Mark_pos_ri = ERROR;
            }
        }

/////////////////////////////////////////////
//////////CHECK WIDTH OF X SEGMENT////////////			BY JIG
///////////////////////////////////////////

        Diff_mark_seg = ERROR ;
        width_seg = 0 ;
        if( TWA_Spec == SpecResin ){
            width_seg = abs( Mark_Xseg[n_seg][2]-Mark_Xseg[n_seg][1] );
			if(( width_seg >= 150)&&( width_seg <= 174)) {					//75x2 84x2=168 -> 174 check again
                Diff_mark_seg = OK;
            } else {
                Diff_mark_seg = ERROR;
            }
        } else {
            width_seg = abs( Mark_Xseg[n_seg][2]-Mark_Xseg[n_seg][1] );
            if( i_mode & INSPECT_STEP ) {
                sprintf(Comment, "width_seg = %d",width_seg );
                DrawMessage( Comment, DRAW_GREEN, 2, 2 );
            }

            if(( width_seg >= 120)&&( width_seg <= 134)) {					//60x2 67x2check again
                Diff_mark_seg = OK;
            } else {
                Diff_mark_seg = ERROR;
            }
        }
 
/////////////END CHECK WIDTH  SEGMENT/////////////////

        if( Mark_Xseg[n_seg][1] == ERROR ) return(ERROR);
        if( Mark_Xseg[n_seg][2] == ERROR ) return(ERROR);

        x0 = (Mark_Xseg[n_seg][1]+Mark_Xseg[n_seg][2])/2;
        y0 = (Mark_Yseg[n_seg][1]+Mark_Yseg[n_seg][2])/2;

        if( i_mode & INSPECT_STEP ) {
            R_display_control( BIN_MEMORY | GRAY_MEMORY );
            R_bin_cls( 0, 0, FxSize-1, FySize-1 );
            R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
            xs = Mark_Xseg[n_seg][1];
            xe = Mark_Xseg[n_seg][2];
            ys = Mark_Yseg[n_seg][1];
            ye = Mark_Yseg[n_seg][2];
            area.left	= xs;
            area.right	= xe;
            area.top	= ys;
            area.bottom	= ye;
            R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
            sprintf(Comment,"box segment");
            DrawMessage( Comment, DRAW_GREEN, 2, 2 );
            R_DrawCls();
			
            R_bin_cls( 0, 0, FxSize-1, FySize-1 );

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
            //    sprintf(Comment,"x0 = %2d y0 = %2d \n xs= %2d xe = %2d ys = %2d ye = %2d",x0,y0,xs,xe,ys,ye);
            //    DrawMessage( Comment, DRAW_GREEN, 2, 2 );

            }
            //文字検査データ
            div_n = Mark_div_x * Mark_div_y;
            for( n=0; n<TWA_R_n; n++ ) {
                for( n1=0; n1<div_n; n1++ ) {
                    //検査座標設定
                    if( TWA_R_n == 1 ) {	//ＪＰＷ検査データ
                        x = Mark_rx_J[n1];
                        y = Mark_ry_J[n1];
                    } else {
                        x = Mark_rx[i][n][n1];
                        y = Mark_ry[i][n][n1];
                    }
                    x += x0;
                    y += y0;
					xs = x - 5;
					xe = x + 4;
					ys = y - 5;
					ye = y + 4;
                    area.left	= xs;
                    area.right	= xe;
                    area.top	= ys;
                    area.bottom	= ye;
                    R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
                }
            }
          //  sprintf(Comment,"in box");
          //  DrawMessage( Comment, DRAW_GREEN, 2, 2 );

/////////////////////////////////////////////
//               GEE test                  //
/////////////////////////////////////////////
		//文字間検査データ
            i = 60;
            for( n=1; n<(TWA_R_n); n++ ) {
                for( n1=0; n1<Mark_div_y; n1++ ) {
                    x = Mark_space_x[i][n][n1] + x0;
                    y = Mark_space_y[i][n][n1] + y0;

					xs = x - 5;
					xe = x + 4;
					ys = y - 5;
					ye = y + 4;
                    R_line( xs, y, xe, y, GRAPH_DRAW, SOLID_LINE );
                    R_line( x, ys, x, ye, GRAPH_DRAW, SOLID_LINE );
                }
            }
            sprintf(Comment,"nijimi check");
            DrawMessage( Comment, DRAW_GREEN, 2, 2 );
            R_DrawCls();

/////////////////////////////////////////////
/////////////////////////////////////////////
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
            if( TWA_R_n == 1 ) judge_l = 30;
            if( Mark_area_xl == 0 ) return(ERROR);                       //GEE Program hang

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

//////////////////////////////////////////////////

int		TWA_Mark_level( int deg10 )
	{
		int					x0, y0;
		int					i, cn, n;
		int					xs, xe, ys, ye;
		int					cl, clmin, clmax;
		int					bright_n;
		int					mark_left,mark_right;       //
		int					mark_top ,mark_bottom;      //
		int                 mn, ans_seg;
		int					left_length ,right_length;
		int					left_lengthM,right_lengthM;
		int					top_length ,bottom_length;
		int					top_lengthM,bottom_lengthM;
		R_RECT				area;
		int					i_mode	= R_CS_GetInspectMode();
		int						grayPn	= InspGrayPn;

			mark_zure1 = ERROR;              //GEE
			mark_zure2 = ERROR;              //GEE
			mark_zure3 = ERROR;
			mark_zure4 = ERROR;
		//Manual
            if( i_mode & INSPECT_STEP ) {
                R_display_control( BIN_MEMORY | GRAY_MEMORY );
                R_bin_cls( 0, 0, FxSize-1, FySize-1 );
                R_move_gray_memory(grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
			}

		//標印傾き確認
			i = (300+deg10)/5;
		//文字エリア中心
			x0 = (Ma_pos[0]+Ma_pos[1])/2;
			y0 = (Ma_pos[2]+Ma_pos[3])/2;

			for(mn=0; mn<6; mn++){
				if( Mark_temp[mn] == ERROR ) {ans_seg = ERROR; break;}
                //	if( i_mode & INSPECT_STEP ) {
                //		sprintf( Comment, "mark pos xs xn ys yn  = %3d %3d %3d %3d",Ma_pos[0],Ma_pos[1],Ma_pos[2],Ma_pos[3] );
                //		DrawMessage( Comment, DRAW_GREEN, 2, 3 );
                //		R_DrawCls();
                //	}
			}

		//1文字毎にレベル設定
			for( cn=0,mn=0; cn<TWA_R_n; cn++,mn+=2 ) {
				//検査範囲設定
					xs = x0 + max( Mark_area_x[i][cn][0], Mark_area_x[i][cn][2] );          //GEE
					xe = x0 + min( Mark_area_x[i][cn][1], Mark_area_x[i][cn][3] );          //GEE
					ys = y0 + max( Mark_area_y[i][cn][0], Mark_area_y[i][cn][1] );          //GEE
					ye = y0 + min( Mark_area_y[i][cn][2], Mark_area_y[i][cn][3] );          //GEE

                if( i_mode & INSPECT_STEP ) {
                        sprintf( Comment, "what = %3d,%3d,%3d",Mark_area_x[i][cn][0],Mark_area_x[i][cn][2],x0 );
						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
						R_DrawCls();
				}
					if(ans_seg == ERROR){
						Mark_temp[mn]   = xs;
						Mark_temp[mn+1] = xe; 
					}
				//指定エリアのヒストグラムを取る。（１ドットおきに間引く）
//					bright_av = R_get_step_histgram( xs, ys, xe, ye, Gray_address[2] );
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
//					if( clmin < 20 ) clmin = 20;	//最低レベル 10
					//max level
					cl = 0;
					for( n=255; n>0; n-- ) {
						cl += Bright[n];
						if( cl > bright_n ) break;
					}
					clmax = n;
					cl = (Bin_Ma_l_ini[TWA_Spec][0]+Bin_Ma_l_ini[TWA_Spec][1])/2;
					if( clmax < cl ) clmax = cl;
					//level
					cl = (clmax-clmin)/10 + 1;
					if((TWA_Spec==SpecJPW)&&(cl<4)) return ERROR;
					Mark_Slevel[cn][1] = (clmin+clmax)/2;					//center level
					Mark_Slevel[cn][0] = Mark_Slevel[cn][1] - cl - 12;		//min level  GEE 10 to 12
					Mark_Slevel[cn][2] = Mark_Slevel[cn][1] + cl;			//max level
					if( Mark_Slevel[cn][0] < 10 ) Mark_Slevel[cn][0] = 10;
				//Manual
                    if( i_mode & INSPECT_STEP ) {
                        TWA_gray2bin( Mark_Slevel[cn][1], xs, ys, xe-xs+1, ye-ys+1, grayPn, 0 );
//						R_gray2bin( Mark_Slevel[cn][1], xs, ys, xe-xs+1, ye-ys+1, Gray_address[2], Bin_address[0] );
//						R_box( xs, ys, xe, ye, GRAPH_REVERSE, SOLID_LINE );
						area.left	= xs;
					 	area.right	= xe;
					 	area.top	= ys;
					 	area.bottom	= ye;
					 	R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
						R_DrawCls();
						printf( "Mark %d Bright Lo:%d  Hi:%d  n:%d\n", cn, clmin, clmax, bright_n );
						printf( "Mark %d Level min:%d  max:%d\n", cn, Mark_Slevel[cn][0], Mark_Slevel[cn][2] );
					}
			}

///////////////////////////////////////////////
//             GEE marking zure              //
///////////////////////////////////////////////

					mark_left  = x0 + max( Mark_area_x[i][0][0], Mark_area_x[i][0][2] );          //GEE
					mark_right = x0 + min( Mark_area_x[i][2][1], Mark_area_x[i][2][3] );          //GEE
					mark_top   = y0 + min( Mark_area_y[i][0][0], Mark_area_y[i][2][1] );
					mark_bottom= y0 + max( Mark_area_y[i][0][2], Mark_area_y[i][2][3] );
					left_length  = mark_left - G2_pos[0] ;
					right_length = G2_pos[1] - mark_right;
					top_length   = mark_top - (Elec[DataLR].CornerPosY[0]+Elec[DataLR].CornerPosY[2])/2;
					bottom_length= (Elec[DataLR].CornerPosY[1]+Elec[DataLR].CornerPosY[3])/2 -mark_bottom;

					if( left_length  >  InspPara.MarkShift[TWA_Spec].nData1 )   mark_zure1 = OK;                         //GEE
					if( right_length >  InspPara.MarkShift[TWA_Spec].nData1 )   mark_zure2 = OK;                         //GEE
					if( top_length    >  InspPara.MarkShift[TWA_Spec].nData2 )   mark_zure3 = OK;                         //GEE
					if( bottom_length >  InspPara.MarkShift[TWA_Spec].nData2 )   mark_zure4 = OK;                         //GEE
//printf("marklr,%d,%d\n",left_length,right_length);
//					if(( TWA_Insp_mode != Insp_Pinhole )||( TWA_Spec != SpecJPW )) {                                   //2011.04.19
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

//                                 if((mark_left-7) < (G2_pos[0]))  return ERROR;				                       	   //GEE
//                                 if((mark_right+7) > (G2_pos[1])) return ERROR;              				           //GEE
							if((mark_zure1 !=OK) || (mark_zure2 !=OK))	return ERROR;
							if((mark_zure3 !=OK) || (mark_zure4 !=OK))	return ERROR;
					}

///////////////////////////////////////////////
///////////////////////////////////////////////


		return(OK);
	}
/*********************************************************************************
	関数名：	ＴＷＡ＿Ｍａｒｋ＿ｐｏｉｎｔ（ int deg10, int ox, int oy, int ol )
    機　能：　	標印データ読み取り
	形　式：	void	TWA_Mark_point( int deg10, int ox, int oy, int ol )
	引き数：	int		deg10       文字角度データ(10倍)
			int		ox
			int		oy
			int		ol
			int		nc		文字位置
	戻り値：	値			定　数			意　味
				0			OK				正常に文字切り出しが完了しました。
				-1			ERROR			異常終了しました。文字の切り出しが出来なかった。
	解　説：	標印文字の切り出しを行う。
	留意事項：	
*********************************************************************************/

static int		TWA_Mark_point( int deg10, int ox, int oy, int ol, int nc  )
{
        int				x0, y0;
		int				xs, xe, ys, ye;
		int				div_n;
		int				ncr, nd, ndr, i;
		int				x, y;
		int				nx, ny;
		int				pt[8];
		int				jw, jb;
		int				ls, le;
		int				ans;
		int             mn;
        R_RECT			area;
		int				i_mode	= R_CS_GetInspectMode();
        int				grayPn	= InspGrayPn;
		int				binPn	= InspBinPn;

        //標印傾き確認
			i = (300+deg10)/5;

		//文字エリア中心
			x0 = (Ma_pos[0]+Ma_pos[1])/2;
			y0 = (Ma_pos[2]+Ma_pos[3])/2;
			x0 += ox;
			y0 += oy;

//	 		if( i_mode & INSPECT_STEP ) {
//                //文字エリア
//                xs = x0 + Mark_area_x[i][nc][0];
//                xe = x0 + Mark_area_x[i][nc][3];
//                ys = y0 + Mark_area_y[i][nc][0];
//                ye = y0 + Mark_area_y[i][nc][2];
//                area.left	= xs;
//                area.right	= xe;
//                area.top	= ys;
//                area.bottom	= ye;
//                R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
//			}

			xs = G2_pos[0]-5;
			xe = G2_pos[1]+5;
			ys = G2_pos[2]-5;
			ye = G2_pos[3]+5;

            ans = OK;
            for( nd=0; nd<4; nd++ ) {
                x = x0 + Mark_area_x[i][nc][nd];
                y = y0 + Mark_area_y[i][nc][nd];
                if((x<xs)||(x>xe)) ans = ERROR;
                if((y<ys)||(y>ye)) ans = ERROR;
            }

            if( ans != OK ) return(ERROR);

//            if( i_mode & INSPECT_STEP  && nc == 0) {
//				R_display_control( BIN_MEMORY | GRAY_MEMORY );
//                R_bin_cls( 0, 0, FxSize-1, FySize-1 );
//                R_move_gray_memory(grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
//			}

			if(nc==0) mn = 0;
			if(nc==1) mn = 2;
			if(nc==2) mn = 4;
        //文字データ読み取り

            div_n = Mark_div_x * Mark_div_y;
            for( ny=0; ny<Mark_div_y; ny++ ) {
                for( nx=0; nx<Mark_div_x; nx++ ) {
                    nd = Mark_div_x*ny+nx;
                //検査座標設定
                       if( TWA_R_n == 1 ) {	//ＪＰＷ検査データ
                        x = Mark_rx_J[nd];
                        y = Mark_ry_J[nd];
                    } else {
                        x = Mark_rx[i][nc][nd];
                        y = Mark_ry[i][nc][nd];
                    }
                    pt[0] = x + x0;							//X center
				 //   if(pt[0]-3 < Mark_temp[mn])   pt[0] = Mark_temp[mn]+3;
				 //   if(pt[0]+3 > Mark_temp[mn+1]) pt[0] = Mark_temp[mn+1]-3;

                    pt[1] = y + y0;							//Y center
                    pt[2] = Mark_Slevel[nc][0] + ol;		//White level
                    if( pt[2] < 10 ) pt[2] = 10;
                    pt[3] = Mark_Slevel[nc][2] + ol;		//Black level
                    if( pt[3] < 10 ) pt[3] = 10;
                //範囲内の白／黒を数える
                    TWA_get_color_area(InspGray4Pn, pt, &jw, &jb, nc );	//change plan 2 to 5
                    Mark_div_ws[nc][nd] = jw;
                    Mark_div_bs[nc][nd] = jb;
/*
                    if( i_mode & INSPECT_STEP ) {
                //検査座標設定
                        if( TWA_R_n == 1 ) {	//ＪＰＷ検査データ
                            x = Mark_rx_J[nd];
                            y = Mark_ry_J[nd];
                        } else {
                            x = Mark_rx[i][nc][nd];
                            y = Mark_ry[i][nc][nd];
                        }
                        x += x0;
                        y += y0;
				   //     if(x-3 < Mark_temp[mn])   x = Mark_temp[mn]+3;
				   //     if(x+3 > Mark_temp[mn+1]) x = Mark_temp[mn+1]-3;
						xs = x - 5;
						xe = x + 4;
						ys = y - 5;
						ye = y + 4;
                        if( Mark_div_ws[nc][nd] == 1 ) {
                            area.left	= xs;
                            area.right	= xe;
                            area.top	= ys;
                            area.bottom	= ye;
                            R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
                        }
                        if( Mark_div_bs[nc][nd] == 0 ) {
                            area.left	= x;
                            area.right	= x;
                            area.top	= ys;
                            area.bottom	= ye;
                            R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
                            area.left	= xs;
                            area.right	= xe;
                            area.top	= y;
                            area.bottom	= y;
                            R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
                        }
                    }
*/
                }
            //標印Size
				if( TWA_R_n == 1 ) {
					mox[0] = x0;
					moy[0] = y0;
					nd = Mark_div_x*ny;
                    pt[0] = Mark_rx_J[nd] + x0;
                    pt[1] = Mark_ry_J[nd] + y0;
                    nd = Mark_div_x*(ny+1)-1;
                    pt[2] = Mark_rx_J[nd] + x0;
                    pt[3] = Mark_ry_J[nd] + y0;
                } else {
					mox[nc] = x0;
					moy[nc] = y0;
					nd = Mark_div_x*ny;
                    pt[0] = Mark_rx[i][nc][nd] + x0;
                    pt[1] = Mark_ry[i][nc][nd] + y0;
                    nd = Mark_div_x*(ny+1)-1;
                    pt[2] = Mark_rx[i][nc][nd] + x0;
                    pt[3] = Mark_ry[i][nc][nd] + y0;
                }
                pt[4] = Mark_Slevel[nc][0] + ol;		//White level
				TWA_Mark_Size( grayPn, pt, &ls, &le );
				Mark_size_ls[nc][ny][0] = ls;
                Mark_size_ls[nc][ny][1] = le;
                if( i_mode & INSPECT_STEP ) {
                    printf( "Mark Size %d-%d:%d-%d\n", nc, ny, ls, le );
                }
            }
 //           if( i_mode & INSPECT_STEP ) {
 //               sprintf(Comment,"MARKING");
 //               DrawMessage( Comment, DRAW_GREEN, 2, 2 );
 //               R_DrawCls();
 //           }

///////////////////////////////////////////////
//                 GEE nijimi               //
///////////////////////////////////////////////
            if(nc!=0){
            //Out side mark detect
                div_n = Mark_div_x * Mark_div_y;
				for( ny=0; ny<Mark_div_y; ny++ ) {
                //検査座標設定
                    if( TWA_R_n == 1 ) {	//ＪＰＷ検査データ
                        x = Mark_rx_J[nd];
                        y = Mark_ry_J[nd];
                    } else {
                        x = Mark_space_x[i][nc][ny];
                        y = Mark_space_y[i][nc][ny];
                    }
                    pt[0] = x + x0;							//X center
                    pt[1] = y + y0;							//Y center
                    pt[2] = Mark_Slevel[nc][0] + ol;		//White level
                    if( pt[2] < 10 ) pt[2] = 10;
                    pt[3] = Mark_Slevel[nc][2] + ol;		//Black level
                    if( pt[3] < 10 ) pt[3] = 10;
                    if( nc == 3 ) {
                        pt[2] = Mark_Slevel[2][0] + ol;                 //White level
                        if( pt[2] < 10 ) pt[2] = 10;
						pt[3] = Mark_Slevel[2][2] + ol;	         	//Black level
						if( pt[3] < 10 ) pt[3] = 10;
					}
                //範囲内の白／黒を数える
                    TWA_get_color_area_nijimi( InspGray4Pn, pt, &jw, &jb, nc );			//Jig change page 2to5
                    Mark_div_ws1[nc][ny] = jw;
                    Mark_div_bs1[nc][ny] = jb;

/*                    if( i_mode & INSPECT_STEP ) {
                    //検査座標設定
                        if( TWA_R_n == 1 ) {	//ＪＰＷ検査データ
                            x = Mark_space_x[i][nc][ny];
                            y = Mark_space_y[i][nc][ny];
                        } else {
                            x = Mark_space_x[i][nc][ny];
                            y = Mark_space_y[i][nc][ny];
                        }
                        x += x0;
                        y += y0;
						xs = x - 5;
						xe = x + 4;
						ys = y - 5;
						ye = y + 4;
                        if( Mark_div_ws1[nc][ny] == 1 ) {
                            area.left	= xs;
                            area.right	= xe;
                            area.top	= ys;
                            area.bottom	= ye;
                            R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
                        }
                        if( Mark_div_bs1[nc][ny] == 0 ) {
                            R_line( x, ys, x, ye, GRAPH_DRAW, SOLID_LINE );
                            R_line( xs, y, xe, y, GRAPH_DRAW, SOLID_LINE );
                        }
                    }
*/
                }
//		 		if( i_mode & INSPECT_STEP ) {
//					sprintf(Comment," NIJIMI ");
//					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
//					R_DrawCls();
//				}
			}

/////////////////////////////////////////
/////////////////////////////////////////
		//標印文字分割Data(Bin逆方向)
			div_n = Mark_div_x * Mark_div_y;
			for( nc=0, ncr=0; nc<TWA_R_n; nc++, ncr++ ) {
				for( nd=0, ndr=(div_n-1); nd<div_n; nd++, ndr-- ) {
					Mark_div_wr[ncr][ndr] = Mark_div_ws[nc][nd];
					Mark_div_br[ncr][ndr] = Mark_div_bs[nc][nd];
				}
				for( nd=0, ndr=(Mark_div_y-1); nd<Mark_div_y; nd++, ndr-- ) {
					Mark_size_lr[ncr][ndr][0] = Mark_size_ls[nc][nd][1];
					Mark_size_lr[ncr][ndr][1] = Mark_size_ls[nc][nd][0];
				}
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

static int		TWA_Mark_Judge( int nc )
	{
		int				c1st, c1re, c1sum;
		int				c2st_t, c2re_t, c2sum_t;
		int				c2st_w, c2re_w, c2sum_w;
		int				c2st_b, c2re_b, c2sum_b;
		int				nm, nx, ny, nd, ncr;
		int				pt[10];
		int				judge;

		int				j, i, l0, l1, l2, l3;
		int				jm;

		int				jm101st, jm101re, n_jm101;
		int				insp_st, insp_re;

		int				insp_mode;
		int				ncm;
		int				ans_st, ans_re;
		int				i_mode	= R_CS_GetInspectMode();
		int				grayPn	= InspGrayPn;
		int				Nijimi;

			if( TWA_Insp_mode != Insp_Pinhole ) judge = InspParaM.MarkLevel[TWA_Spec].nData1;      // 60 GEE 60 to 80
			else								judge = InspParaM.MarkLevel[TWA_Spec].nData2;      // 80 GEE 80 to 95

			//文字計測値初期化
				insp_mode = Mark_Insp_mode;
					if((insp_mode & 0x01) == 0 ) {
						Mark_L_set[nc] = 0;
					} else {
						Mark_L_set[nc] = 1;
					}
					insp_mode >>= 1;
			//検査文字指定
				switch( TWA_R_n ) {
					case 1:
						Mark_L_set[0] = 1;
						if( TWA_Insp_mode != Insp_Pinhole ) judge = InspParaM.MarkLevel[TWA_Spec].nData1; //80;          //GEE 80 to 95
						else								judge = InspParaM.MarkLevel[TWA_Spec].nData2;	//80;          //GEE 80 to 95
						break;
					case 3:
						if( MarkChar[1] == 'R' ) Mark_L_set[2] = 1;
						else                     Mark_L_set[1] = 1;
						break;
					case 4:
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
						break;
				}

			for( ncm=0; ncm<(TWA_R_n+1); ncm++ ) {     //GEE iniitial data
				Nijimi_Judge[ncm] = OK;
			}

			if(nc != 3){
				for( nm=0; nm<11; nm++ ) {
					c1st = c1re = c1sum = 0;
					c2st_t = c2re_t = c2sum_t = 0;
					c2st_w = c2re_w = c2sum_w = 0;
					c2st_b = c2re_b = c2sum_b = 0;
					if( TWA_R_n == 1 ) {	//ＪＰＷ検査データ
						for( ny=0; ny<Mark_div_y; ny++ ) {
							for( nx=0; nx<Mark_div_x; nx++ ) {
								nd = ny*Mark_div_x + nx;
								pt[0] = nc;							//桁
								pt[3] = nx;							//X1
								pt[4] = ny;							//Y1
								if( TWA_Insp_mode == Insp_Pinhole ) {
									pt[1] = 0;						//Color
									//Check 1
									pt[2] = 1;						//Check1
									pt[5] = 0x0f;					//or X1
									pt[6] = 0x0f;					//or Y1
									if( pt[2] != 0 ) TWA_Mark_div_check( pt, &c1st, &c1re, &c1sum );
									//Check 2
									pt[2] = 1;						//Check2
									pt[5] = 0x0f;					//or X2
									pt[6] = 0x0f;					//or Y2
									if( pt[2] != 0 ) {
										if( pt[1] == 0 ) {
											TWA_Mark_div_check( pt, &c2st_b, &c2re_b, &c2sum_b );
										} else {
											TWA_Mark_div_check( pt, &c2st_w, &c2re_w, &c2sum_w );
										}
									}
								} else {
									pt[1] = (Mark_Check_0[nd]>>TWA_Mark_co ) & 0x0f;	//Color
									//Check 1
									pt[2] = (Mark_Check_0[nd]>>TWA_Mark_ch1) & 0x0f;	//Check1
									pt[5] = (Mark_Check_0[nd]>>TWA_Mark_oX1) & 0x0f;	//or X1
									pt[6] = (Mark_Check_0[nd]>>TWA_Mark_oY1) & 0x0f;	//or Y1
									if( pt[2] != 0 ) TWA_Mark_div_check( pt, &c1st, &c1re, &c1sum );
									//Check 2
									pt[2] = (Mark_Check_0[nd]>>TWA_Mark_ch2) & 0x0f;	//Check2
									pt[5] = (Mark_Check_0[nd]>>TWA_Mark_oX2) & 0x0f;	//or X2
									pt[6] = Mark_Check_0[nd] & 0x0f;					//or Y2
									if( pt[2] != 0 ) {
										if( pt[1] == 0 ) {
											TWA_Mark_div_check( pt, &c2st_b, &c2re_b, &c2sum_b );
										} else {
											TWA_Mark_div_check( pt, &c2st_w, &c2re_w, &c2sum_w );
										}
									}
								}
							}
						}
					} else {
						if( TWA_Spec == SpecResin ) {
							//ＮＰーIII検査データ　３桁標印
							for( ny=0; ny<Mark_div_y; ny++ ) {
								for( nx=0; nx<Mark_div_x; nx++ ) {
									nd = ny*Mark_div_x + nx;
									pt[0] = nc;												//桁
									pt[1] = (Mark_Check_P3[nm][nd]>>TWA_Mark_co ) & 0x0f;	//Color
									pt[3] = nx;												//Check X
									pt[4] = ny;												//Check Y
									//Check 1
									pt[2] = (Mark_Check_P3[nm][nd]>>TWA_Mark_ch1) & 0x0f;	//Check1
									pt[5] = (Mark_Check_P3[nm][nd]>>TWA_Mark_oX1) & 0x0f;	//or X1
									pt[6] = (Mark_Check_P3[nm][nd]>>TWA_Mark_oY1) & 0x0f;	//or Y1
									if( pt[2] != 0 ) TWA_Mark_div_check( pt, &c1st, &c1re, &c1sum );
									//Check 2
									pt[2] = (Mark_Check_P3[nm][nd]>>TWA_Mark_ch2) & 0x0f;	//Check2
									pt[5] = (Mark_Check_P3[nm][nd]>>TWA_Mark_oX2) & 0x0f;	//or X2
									pt[6] = Mark_Check_P3[nm][nd] & 0x0f;					//or Y2
									if( pt[2] != 0 ) {
										if( pt[1] == 0 ) {
											TWA_Mark_div_check( pt, &c2st_b, &c2re_b, &c2sum_b );
										} else {
											TWA_Mark_div_check( pt, &c2st_w, &c2re_w, &c2sum_w );
										}
									}
								}
							}
						} else {
							//通常品検査データ
							for( ny=0; ny<Mark_div_y; ny++ ) {
								for( nx=0; nx<Mark_div_x; nx++ ) {
									nd = ny*Mark_div_x + nx;
									pt[0] = nc;												//桁
									pt[1] = (Mark_Check_St[nm][nd]>>TWA_Mark_co ) & 0x0f;	//Color
									pt[3] = nx;												//Check 1
									pt[4] = ny;												//Check 1
									//Check 1
									pt[2] = (Mark_Check_St[nm][nd]>>TWA_Mark_ch1) & 0x0f;	//Check1
									pt[5] = (Mark_Check_St[nm][nd]>>TWA_Mark_oX1) & 0x0f;	//or X1
									pt[6] = (Mark_Check_St[nm][nd]>>TWA_Mark_oY1) & 0x0f;	//or Y1
									if( pt[2] != 0 ) TWA_Mark_div_check( pt, &c1st, &c1re, &c1sum );
									//Check 2
									pt[2] = (Mark_Check_St[nm][nd]>>TWA_Mark_ch2) & 0x0f;	//Check2
									pt[5] = (Mark_Check_St[nm][nd]>>TWA_Mark_oX2) & 0x0f;	//or X2
									pt[6] = Mark_Check_St[nm][nd] & 0x0f;					//or Y2
									if( pt[2] != 0 ) {
										if( pt[1] == 0 ) {
											TWA_Mark_div_check( pt, &c2st_b, &c2re_b, &c2sum_b );
										} else {
											TWA_Mark_div_check( pt, &c2st_w, &c2re_w, &c2sum_w );
										}
									}
								}
							}
						}
					}
					c2st_t  = c2st_w  + c2st_b;
					c2re_t  = c2re_w  + c2re_b;
					c2sum_t = c2sum_w + c2sum_b;
					//if( c2sum_t == 0 ) c2sum_t = 1;
					//if( c1sum == 0 )   c1sum   = 1;
					//if( c2sum_w == 0 ) c2sum_w = 1;
					//if( c2sum_b == 0 ) c2sum_b = 1;
					if( c2sum_t <= 0 ) break;                               //GEE Program hang
					if( c1sum <= 0 )   break;                               //GEE Program hang
					if( c2sum_w <= 0 ) break;                               //GEE Program hang
					if( c2sum_b <= 0 ) break;                               //GEE Program hang
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
					if( Mark_Judge_st[nc][nm][3] < judge ) Mark_Judge_st[nc][nm][1] = ERROR;           //GEE comment
					if( Mark_Judge_re[nc][nm][3] < judge ) Mark_Judge_re[nc][nm][1] = ERROR;           //GEE comment
					if( Mark_Judge_st[nc][nm][4] < judge ) Mark_Judge_st[nc][nm][1] = ERROR;           //GEE comment
					if( Mark_Judge_re[nc][nm][4] < judge ) Mark_Judge_re[nc][nm][1] = ERROR;           //GEE comment
			 		if( i_mode & INSPECT_STEP ) {
						printf( "Mst:%d-%d  Judge W:%d  B:%d\n", nc, nm,           		   //GEE comment
										Mark_Judge_st[nc][nm][3],                  //GEE comment
										Mark_Judge_st[nc][nm][4] );                //GEE comment
						printf( "Mre:%d-%d  Judge W:%d  B:%d\n", nc, nm,                           //GEE comment
										Mark_Judge_re[nc][nm][3],                  //GEE comment
										Mark_Judge_re[nc][nm][4] );                //GEE comment
					}
					if( TWA_R_n == 1 ) break;	//ＪＰＷ検査データ
				}
				if( TWA_R_n != 1 ) {
					TWA_Mark_error( nc );
				} else {
					TWA_Mark_J_error( nc );
				}
					if( nc != 0 )  TWA_Nijimi_error( nc );             //GEE nijimi check
			}

				for( nm=0; nm<11; nm++ ) {
					if( Mark_Judge_st[nc][nm][0] == 100 ) {
						if( (Mark_Judge_st[nc][nm][1] == OK) && ( Nijimi_Judge[nc] == OK) ) {
							Mark_J100_st[nc] = nm;
						}
					}
					if( Mark_Judge_re[nc][nm][0] == 100 ) {
						if( (Mark_Judge_re[nc][nm][1] == OK) && (Nijimi_Judge[nc] == OK) ) {
							Mark_J100_re[nc] = nm;
						}
					}
				}

					if( Mark_J100_st[nc] != ERROR ) {
						max_st[nc][0] = 101;
						max_st[nc][1] = Mark_J100_st[nc];
					}
					if( Mark_J100_re[nc] != ERROR ) {
						max_re[nc][0] = 101;
						max_re[nc][1] = Mark_J100_re[nc];
					}

					for( nm=0; nm<11; nm++ ) {
						so_st[nc][nm][0] = nm;
						so_st[nc][nm][1] = Mark_Judge_st[nc][nm][0];
						so_st[nc][nm][2] = Mark_Judge_st[nc][nm][1];
						so_st[nc][nm][3] = Mark_Judge_st[nc][nm][2];
						so_re[nc][nm][0] = nm;
						so_re[nc][nm][1] = Mark_Judge_re[nc][nm][0];
						so_re[nc][nm][2] = Mark_Judge_re[nc][nm][1];
						so_re[nc][nm][3] = Mark_Judge_re[nc][nm][2];
						if( (Mark_Judge_st[nc][nm][1] == OK) && ( Nijimi_Judge[nc] == OK) ) {
							if( max_st[nc][0] < Mark_Judge_st[nc][nm][0] ) {
								max_st[nc][0] = Mark_Judge_st[nc][nm][0];
								max_st[nc][1] = nm;
							}
							if( max_st[nc][0] <= Mark_Judge_st[nc][nm][0] ) {
								if(TWA_R_data[nc] == nm ) {
									max_st[nc][0] = Mark_Judge_st[nc][nm][0];
									max_st[nc][1] = nm;
								}
							}
						}
						if( (Mark_Judge_re[nc][nm][1] == OK) && ( Nijimi_Judge[nc] == OK) ) {
							if( max_re[nc][0] < Mark_Judge_re[nc][nm][0] ) {
								max_re[nc][0] = Mark_Judge_re[nc][nm][0];
								max_re[nc][1] = nm;
							}
							if( max_re[nc][0] <= Mark_Judge_re[nc][nm][0] ) {
								if(TWA_R_data[nc] == nm ) {
									max_re[nc][0] = Mark_Judge_re[nc][nm][0];
									max_re[nc][1] = nm;
								}
							}
						}
					}


					for( i=0; i<10; i++ ) {
						for( j=i+1; j<=10; j++ ) {
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
					for( i=0; i<10; i++ ) {
						for( j=i+1; j<=10; j++ ) {
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
/*
#ifdef Mark_DEBUG_M03

		 		if( i_mode & INSPECT_STEP ) {
				R_display_control( BIN_MEMORY | GRAY_MEMORY );
                R_bin_cls( 0, 0, FxSize-1, FySize-1 );
                R_move_gray_memory(grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
//				for( nc=0; nc<TWA_R_n; nc++ ) {

					Nijimi = ERROR;
//				    if((Nijimi_Judge[nc] == OK) && (Nijimi_Judge[nc+1] == OK))  Nijimi = OK;
				    if((Nijimi_Judge[nc] == OK))  Nijimi = OK;
					if( TWA_R_n > 1 ) {
						sprintf( Comment, "Rst : %d       Nijimi : %2d \n %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d \n %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d \n %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d \n %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d max:%2d(%3d)",
							                nc , Nijimi,
									so_st[nc][0][0], so_st[nc][0][1], so_st[nc][0][2], so_st[nc][0][3],
									so_st[nc][1][0], so_st[nc][1][1], so_st[nc][1][2], so_st[nc][1][3],
									so_st[nc][2][0], so_st[nc][2][1], so_st[nc][2][2], so_st[nc][2][3],
									so_st[nc][3][0], so_st[nc][3][1], so_st[nc][3][2], so_st[nc][3][3],
									so_st[nc][4][0], so_st[nc][4][1], so_st[nc][4][2], so_st[nc][4][3],
									so_st[nc][5][0], so_st[nc][5][1], so_st[nc][5][2], so_st[nc][5][3],
									so_st[nc][6][0], so_st[nc][6][1], so_st[nc][6][2], so_st[nc][6][3],
									so_st[nc][7][0], so_st[nc][7][1], so_st[nc][7][2], so_st[nc][7][3],
									so_st[nc][8][0], so_st[nc][8][1], so_st[nc][8][2], so_st[nc][8][3],
									so_st[nc][9][0], so_st[nc][9][1], so_st[nc][9][2], so_st[nc][9][3],
									so_st[nc][10][0], so_st[nc][10][1], so_st[nc][10][2], so_st[nc][10][3],
									max_st[nc][1], max_st[nc][0] );
						DrawMessage( Comment, DRAW_GREEN, 2, 2);
						R_DrawCls();

						ncr = nc;
						sprintf( Comment, "Rre : %d       Nijimi : %2d \n %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d \n %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d \n %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d \n %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d max:%2d(%3d)",
									nc,Nijimi,
									so_re[ncr][0][0], so_re[ncr][0][1], so_re[ncr][0][2], so_re[ncr][0][3],
									so_re[ncr][1][0], so_re[ncr][1][1], so_re[ncr][1][2], so_re[ncr][1][3],
									so_re[ncr][2][0], so_re[ncr][2][1], so_re[ncr][2][2], so_re[ncr][2][3],
									so_re[ncr][3][0], so_re[ncr][3][1], so_re[ncr][3][2], so_re[ncr][3][3],
									so_re[ncr][4][0], so_re[ncr][4][1], so_re[ncr][4][2], so_re[ncr][4][3],
									so_re[ncr][5][0], so_re[ncr][5][1], so_re[ncr][5][2], so_re[ncr][5][3],
									so_re[ncr][6][0], so_re[ncr][6][1], so_re[ncr][6][2], so_re[ncr][6][3],
									so_re[ncr][7][0], so_re[ncr][7][1], so_re[ncr][7][2], so_re[ncr][7][3],
								 	so_re[ncr][8][0], so_re[ncr][8][1], so_re[ncr][8][2], so_re[ncr][8][3],
									so_re[ncr][9][0], so_re[ncr][9][1], so_re[ncr][9][2], so_re[ncr][9][3],
									so_re[ncr][10][0], so_re[ncr][10][1], so_re[ncr][10][2], so_re[ncr][10][3],
									max_re[ncr][1], max_re[ncr][0] );
						DrawMessage( Comment, DRAW_GREEN, 2, 2);
						R_DrawCls();

					} else {
						sprintf( Comment, "0:%3d(%2d)%2d       Nijimi : %2d", Mark_Judge_st[nc][0][0], Mark_Judge_st[nc][0][1], Mark_Judge_st[nc][0][2], Nijimi );
						DrawMessage( Comment, DRAW_GREEN, 2, 2);
						R_DrawCls();
					}
//				}
			}
#endif
*/
			jm101st = jm101re = n_jm101 = 0;
			if( TWA_R_n > 1 ) {
					if( Mark_L_set[nc] != 0 ) {
						if( Mark_J100_st[nc] != ERROR ) jm101st++;
						if( Mark_J100_re[nc] != ERROR ) jm101re++;
						n_jm101++;
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

			ans_st = ans_re = OK;
			if( insp_st == OK ) {
					jm = TWA_R_data[nc]; 
					if( Mark_L_set[nc] != 0 ) {
						if( TWA_R_data[nc] != max_st[nc][1] )  ans_st = ERROR;
						if( max_st[nc][0] < judge )            ans_st = ERROR;
						if( Mark_Judge_st[nc][jm][3] < judge ) ans_st = ERROR;
						if( Mark_Judge_st[nc][jm][4] < judge ) ans_st = ERROR;
						
					} else {
						if( TWA_R_data[nc] != max_st[nc][1] )  ans_st = ERROR;
						if( max_st[nc][0] < judge ) 			ans_st = ERROR;
					}
				if( ans_st == OK ) {
					Mark_Ans_st[nc] = OK;
				}
			}

			if( insp_re == OK ) {
					jm = TWA_R_data[TWA_R_n-nc-1];
					if( Mark_L_set[nc] != 0 ) {
						if( TWA_R_data[TWA_R_n-nc-1] != max_re[nc][1])	ans_re = ERROR;
						if( max_re[nc][0] < judge )                     ans_re = ERROR;
						if( Mark_Judge_re[nc][jm][3] < judge )			ans_re = ERROR;
						if( Mark_Judge_re[nc][jm][4] < judge )			ans_re = ERROR;
					} else {
						if( TWA_R_data[TWA_R_n-nc-1] != max_re[nc][1] ) ans_re = ERROR;
						if( max_re[nc][0] < judge )						ans_re = ERROR;
					}
				if( ans_re == OK ) {
					Mark_Ans_re[nc] = OK;
				}
			}
		if((ans_st == OK)||(ans_re == OK )) return OK;
		return(ERROR);

	}

/*******************************************************************************
void	TWA_Mark_div_check( int pt[], int *nst, int *nre, int *sum )
********************************************************************************/

void	TWA_Mark_div_check( int pt[], int *nst, int *nre, int *sum )
	{
		int				cn;
		int				nx, ny;
		int				nd, ns, nr, nt;
		int				co;
		int				bst1, bst2, wst1, wst2;
		int				bre1, bre2, wre1, wre2;
		int				mode;

			cn = pt[0];
			co = pt[1];

			mode = pt[2];
			ns = *nst;
			nr = *nre;
			nt = *sum;

			nx = pt[3];
			ny = pt[4];

			nd = ny*Mark_div_x + nx;
			bst1 = Mark_div_bs[cn][nd];
			bre1 = Mark_div_br[cn][nd];
			wst1 = Mark_div_ws[cn][nd];
			wre1 = Mark_div_wr[cn][nd];

			nx = pt[5];
			ny = pt[6];
			bst2 = bre2 = -1;
			wst2 = wre2 = -1;

			if( nx != 0x0f ) {
				nd = ny*Mark_div_x + nx;
				bst2 = Mark_div_bs[cn][nd];
				bre2 = Mark_div_br[cn][nd];
				wst2 = Mark_div_ws[cn][nd];
				wre2 = Mark_div_wr[cn][nd];
			}

			switch( mode ) {
				case 1:
					if( co == 0 ) {
						if((bst1==co)||(bst2==co)) ns++;
						if((bre1==co)||(bre2==co)) nr++;
					} else {
						if((wst1==co)||(wst2==co)) ns++;
						if((wre1==co)||(wre2==co)) nr++;
					}
					break;
				case 2:
					if( co != 0 ) {
						if((bst1==co)||(bst2==co)) ns++;
						if((bre1==co)||(bre2==co)) nr++;
					} else {
						if((wst1==co)||(wst2==co)) ns++;
						if((wre1==co)||(wre2==co)) nr++;
					}
					break;
			}


			nt++;

			*nst = ns;
			*nre = nr;
			*sum = nt;

	}


void	TWA_Mark_error( int nc )
	{
		int				nst, nre;
		int				ds1, ds2, ds3, dr1, dr2, dr3;
		int				nm;
		int				pt[55];
		int				rp = 0;
		int				n;

				for( nm=0; nm<11; nm++ ) {
					Mark_Judge_st[nc][nm][1] = OK;
					Mark_Judge_re[nc][nm][1] = OK;
				}

				//0
				   if( TWA_Spec == SpecResin ) {
					nm = 0;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;	//Bcack
//					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 0;     //nd: 7    chk:black
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 0;     //nd:10    chk:black
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 0;     //nd:13    chk:black
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 0;     //nd:16    chk:black
//					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 0;     //nd:19    chk:black
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
			if((Option & REPI)!=0){
					if( nst!=3 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre!=3 ) Mark_Judge_re[nc][nm][1] = ERROR;
			} else {
					if( nst<2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre<2 ) Mark_Judge_re[nc][nm][1] = ERROR;
			}
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;	//Bcack
//					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 1;     //nd: 7   chk:white
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 1;     //nd:10   chk:white
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 1;     //nd:13   chk:white
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 1;     //nd:16   chk:white
//					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 1;     //nd:19   chk:white
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst>=1 ) Mark_Judge_st[nc][nm][1] = ERROR;            //GEE  2  to  1
					if( nre>=1 ) Mark_Judge_re[nc][nm][1] = ERROR;            //GEE  2  to  1

			if((Option & REPI)!=0){
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2-3;	pt[rp++] = 0;     //nd:3    chk:black
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 0;     //nd:6    chk:black
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 0;     //nd:9    chk:black
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 0;     //nd:12   chk:black
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 0;     //nd:15   chk:black
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 0;     //nd:18   chk:black
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2+3;	pt[rp++] = 0;     //nd:21   chk:black
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2-3;	pt[rp++] = 0;     //nd:5    chk:black
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 0;     //nd:8    chk:black
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 0;     //nd:11   chk:black
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 0;     //nd:14   chk:black
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 0;     //nd:17   chk:black
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 0;     //nd:20   chk:black
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2+3;	pt[rp++] = 0;     //nd:23   chk:black
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst>=6 ) Mark_Judge_st[nc][nm][1] = ERROR;          //GEE 2 to 7
					if( nre>=6 ) Mark_Judge_re[nc][nm][1] = ERROR;          //GEE 2 to 7
			}

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2-3;	pt[rp++] = 1;     //nd:3    chk:white
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 1;     //nd:6    chk:white
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 1;     //nd:9    chk:white
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 1;     //nd:12   chk:white
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 1;     //nd:15   chk:white
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 1;     //nd:18   chk:white
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2+3;	pt[rp++] = 1;     //nd:21   chk:white
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
			if((Option & REPI)!=0){
					if( nst<6 ) Mark_Judge_st[nc][nm][1] = ERROR;           //GEE 2 to 7
					if( nre<6 ) Mark_Judge_re[nc][nm][1] = ERROR;           //GEE 2 to 7
			} else {
					if( nst<=3 ) Mark_Judge_st[nc][nm][1] = ERROR;           //jig 6 to 3
					if( nre<=3 ) Mark_Judge_re[nc][nm][1] = ERROR;           //jig 6 to 6
			}

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2-3;	pt[rp++] = 1;     //nd:5    chk:white
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 1;     //nd:8    chk:white
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 1;     //nd:11   chk:white
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 1;     //nd:14   chk:white
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 1;     //nd:17   chk:white
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 1;     //nd:20   chk:white
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2+3;	pt[rp++] = 1;     //nd:23   chk:white
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
			if((Option & REPI)!=0){
					if( nst<6 ) Mark_Judge_st[nc][nm][1] = ERROR;          //GEE 2 to 7
					if( nre<6 ) Mark_Judge_re[nc][nm][1] = ERROR;          //GEE 2 to 7
			} else {
					if( nst<=3 ) Mark_Judge_st[nc][nm][1] = ERROR;          //jig 6 to 3
					if( nre<=3 ) Mark_Judge_re[nc][nm][1] = ERROR;          //jig 6 to 3
			}
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;	                  // GEE add inspection conner
					pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;     //nd:0    chk:white
					pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;     //nd:0    chk:white
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;     //nd:2    chk:white
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst<2 ) Mark_Judge_st[nc][nm][1] = ERROR;            //GEE
					if( nre<2 ) Mark_Judge_re[nc][nm][1] = ERROR;            //GEE

					pt[rp++] = 24;	pt[rp++] = 0;	pt[rp++] = 1;     //nd:24   chk:white
					pt[rp++] = 25;	pt[rp++] = 0;	pt[rp++] = 1;     //nd:24   chk:white
					pt[rp++] = 26;	pt[rp++] = 0;	pt[rp++] = 1;     //nd:26   chk:white
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst<2 ) Mark_Judge_st[nc][nm][1] = ERROR;            //GEE
					if( nre<2 ) Mark_Judge_re[nc][nm][1] = ERROR;            //GEE
					}else{

					nm = 0;
				if((Option & REPI) == 0){
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 0;	//Bcack
						pt[rp++] = 1;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 0;
						pt[rp++] = 1;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst!=2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre!=2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					
						nm = 0;
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 0;	//Bcack
						pt[rp++] = 1;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 0;
						pt[rp++] = 1;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst!=2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre!=2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
			 		pt[rp++] = nc;	pt[rp++] = 1;	//Bcack
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 1;
			 		pt[rp++] = 1;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst>=2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre>=2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;	//Bcack
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst==1 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre==1 ) Mark_Judge_re[nc][nm][1] = ERROR;

				} else {
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;	//Bcack
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst!=3 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre!=3 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;	//Bcack
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst>=2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre>=2 ) Mark_Judge_re[nc][nm][1] = ERROR;
				}
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst<=2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre<=2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst<=2 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre<=2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					if((Option & REPI) == 0){
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;	                  //GEE add inspection conner Up
						pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;     //nd:0    chk:white
						pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;     //nd:1    chk:white
						pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;     //nd:2    chk:white
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst<2 ) Mark_Judge_st[nc][nm][1] = ERROR;            //GEE
						if( nre<2 ) Mark_Judge_re[nc][nm][1] = ERROR;            //GEE

						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;	                  //GEE add inspection conner Lo
						pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;     //nd:18   chk:white
						pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;     //nd:19   chk:white
						pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;     //nd:20   chk:white
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst<2 ) Mark_Judge_st[nc][nm][1] = ERROR;            //GEE
						if( nre<2 ) Mark_Judge_re[nc][nm][1] = ERROR;            //GEE
	
					}
				}

					//0 End
				//1
					nm = 1;
					if( TWA_Spec == SpecResin ) {
						if((Option & REPI) != 0){
							//NP--III検査データ　３桁標印
								rp = 0;
								pt[rp++] = nc;	pt[rp++] = 1;
								pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 0;         //nd:2    chk:black
								pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 0;         //nd:5    chk:black
								pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 0;         //nd:8    chk:black
								pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;

								TWA_Mark_error_check( pt, &nst, &nre );
								if( nst <= 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
								if( nre <= 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
						}
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;         //nd:1    chk:white
							pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;         //nd:4    chk:white
							pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 1;         //nd:7    chk:white
							pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;         //nd:10   chk:white
							pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;         //nd:13   chk:white
							pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 1;         //nd:16   chk:white
							pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;         //nd:18   chk:white
							pt[rp++] = 1;	pt[rp++] = 7;	pt[rp++] = 1;         //nd:21   chk:white
							pt[rp++] = 1;	pt[rp++] = 8;	pt[rp++] = 1;         //nd:24   chk:white
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
						if((Option & REPI) != 0){
							if( nst <= 7 ) Mark_Judge_st[nc][nm][1] = ERROR;      //GEE 4 to 6
							if( nre <= 7 ) Mark_Judge_re[nc][nm][1] = ERROR;      //GEE 4 to 6
						} else {
							if( nst <= 6 ) Mark_Judge_st[nc][nm][1] = ERROR;      //GEE 4 to 6
							if( nre <= 6 ) Mark_Judge_re[nc][nm][1] = ERROR;      //GEE 4 to 6
						}
					} else {
						//通常品検査デ--タ
							//Mark L Size
							ds1 = dr1 = FX_SIZE;	ds2 = dr2 = 0;
							for( n=2; n<=5; n++ ) {
								ds1 = min( ds1, Mark_size_ls[nc][n][0] );
								dr1 = min( dr1, Mark_size_lr[nc][n][0] );
								ds2 = max( ds2, Mark_size_ls[nc][n][0] );
								dr2 = max( dr2, Mark_size_lr[nc][n][0] );
							}
							ds3 = ds2-ds1;
							dr3 = dr2-dr1;
						if((Option & REPI) != 0){
							if( ds3 >= 4 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( dr3 >= 4 ) Mark_Judge_re[nc][nm][1] = ERROR;
						} else {
							if(( ds3 >= 4 )&&( nc == 1 )) Mark_Judge_st[nc][nm][1] = ERROR;
							if(( dr3 >= 4 )&&( nc == 1 )) Mark_Judge_re[nc][nm][1] = ERROR;
						}

							ds1 = dr1 = FX_SIZE;	ds2 = dr2 = 0;
							for( n=1; n<=4; n++ ) {
								ds1 = min( ds1, Mark_size_ls[nc][n][1] );
								dr1 = min( dr1, Mark_size_lr[nc][n][1] );
								ds2 = max( ds2, Mark_size_ls[nc][n][1] );
								dr2 = max( dr2, Mark_size_lr[nc][n][1] );
							}
							ds3 = ds2-ds1;
							dr3 = dr2-dr1;
						if((Option & REPI) != 0){
							if( ds3 >= 4 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( dr3 >= 4 ) Mark_Judge_re[nc][nm][1] = ERROR;
						} else {
							if(( ds3 >= 4 )&&( nc == 1 )) Mark_Judge_st[nc][nm][1] = ERROR;
							if(( dr3 >= 4 )&&( nc == 1 )) Mark_Judge_re[nc][nm][1] = ERROR;
						}
						if((Option & REPI) != 0){
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
						} else {
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
							if( nst <= 4 ) Mark_Judge_st[nc][nm][1] = ERROR;			//JIG change from 2 to 4
							if( nre <= 4 ) Mark_Judge_re[nc][nm][1] = ERROR;			//JIG change from 2 to 4

							rp = 0;                                                          //2010.07.30      //JIG cut off
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
					 		if( nst > 2 ) Mark_Judge_st[nc][nm][1] = ERROR;				//JIG 
					 		if( nre > 2 ) Mark_Judge_re[nc][nm][1] = ERROR;				//JIG
					
					
							rp = 0;                                                          //2010.07.30
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

						}
					}

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

					if( TWA_Spec == SpecResin ) {
						//NP--III検査データ　３桁標印
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 0;
							pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
						if((Option & REPI) == 0){
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 8;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 8;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
						}
					} else {
						//通常品検査デ--タ
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst <= 1 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre <= 1 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 0;
							pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 0;
							pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst <= 1 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre <= 1 ) Mark_Judge_re[nc][nm][1] = ERROR;

							//Mark L Size
							ds1 = dr1 = FX_SIZE;	ds2 = dr2 = 0;
							for( n=1; n<=5; n++ ) {
								ds1 = min( ds1, Mark_size_ls[nc][n][0] );
								dr1 = min( dr1, Mark_size_lr[nc][n][0] );
								ds2 = max( ds2, Mark_size_ls[nc][n][0] );
								dr2 = max( dr2, Mark_size_lr[nc][n][0] );
							}
							ds3 = ds2-ds1;
							dr3 = dr2-dr1;
						if((Option & REPI)!=0){
							if( ds3 <= 3 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( dr3 <= 3 ) Mark_Judge_re[nc][nm][1] = ERROR;
						} else {
							if(( ds3 <= 3 )&&( nc == 1 )) Mark_Judge_st[nc][nm][1] = ERROR;
							if(( dr3 <= 3 )&&( nc == 1 )) Mark_Judge_re[nc][nm][1] = ERROR;
						}
							ds1 = dr1 = FX_SIZE;	ds2 = dr2 = 0;
							for( n=1; n<=5; n++ ) {
								ds1 = min( ds1, Mark_size_ls[nc][n][1] );
								dr1 = min( dr1, Mark_size_lr[nc][n][1] );
								ds2 = max( ds2, Mark_size_ls[nc][n][1] );
								dr2 = max( dr2, Mark_size_lr[nc][n][1] );
							}
							ds3 = ds2-ds1;
							dr3 = dr2-dr1;
						if((Option & REPI)!=0){
							if( ds3 <= 3 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( dr3 <= 3 ) Mark_Judge_re[nc][nm][1] = ERROR;
						} else {
							if(( ds3 <= 3 )&&( nc == 1 )) Mark_Judge_st[nc][nm][1] = ERROR;
							if(( dr3 <= 3 )&&( nc == 1 )) Mark_Judge_re[nc][nm][1] = ERROR;
						}
					}
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
					if( ds2 < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;          //GEE 3 to 2
					if( dr2 < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;          //GEE 3 to 2

					if( TWA_Spec == SpecResin ) {
						//ＮＰ-III検査デ--タ　３桁標印
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &ds1, &dr1 );
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &ds2, &dr2 );
							if((ds1==0)&&(ds2<9)) Mark_Judge_st[nc][nm][1] = ERROR;
							if((dr1==0)&&(dr2<9)) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst > 1 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre > 1 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst > 1 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre > 1 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;                                                 //GEE
							pt[rp++] = nc;	pt[rp++] = 1;                           //GEE
							pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;           //GEE
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;          //GEE
							TWA_Mark_error_check( pt, &nst, &nre );                 //GEE
							if( nst != 1 ) Mark_Judge_st[nc][nm][1] = ERROR;        //GEE
							if( nre != 1 ) Mark_Judge_re[nc][nm][1] = ERROR;        //GEE

							rp = 0;                                                 //GEE
							pt[rp++] = nc;	pt[rp++] = 1;                           //GEE
							pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;           //GEE
							pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;           //GEE
							pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 1;           //GEE
							pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;           //GEE
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;          //GEE
							TWA_Mark_error_check( pt, &nst, &nre );                 //GEE
							if( nst >= 3 ) Mark_Judge_st[nc][nm][1] = ERROR;        //GEE
							if( nre >= 3 ) Mark_Judge_re[nc][nm][1] = ERROR;        //GEE

							rp = 0;                                                 //GEE
							pt[rp++] = nc;	pt[rp++] = 1;                           //GEE
							pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 1;           //GEE
							pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;           //GEE
							pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 1;           //GEE
							pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;           //GEE
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;          //GEE
							TWA_Mark_error_check( pt, &nst, &nre );                 //GEE
							if( nst >= 3 ) Mark_Judge_st[nc][nm][1] = ERROR;        //GEE
							if( nre >= 3 ) Mark_Judge_re[nc][nm][1] = ERROR;        //GEE

						if((Option & REPI)==0){
							rp = 0;                                                 //GEE
							pt[rp++] = nc;	pt[rp++] = 1;                           //GEE
							pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 1;           //GEE
							pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;           //GEE
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;          //GEE
							TWA_Mark_error_check( pt, &nst, &nre );                 //GEE
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;        //GEE
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;        //GEE
						}
					} else {
						//通常品検査データ
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
							pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst <= 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre <= 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

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
						if((Option & REPI)!=0){
//99.02.08
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst <= 1 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre <= 1 ) Mark_Judge_re[nc][nm][1] = ERROR;
						} else {
//99.02.08
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst <= 1 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre <= 1 ) Mark_Judge_re[nc][nm][1] = ERROR;
//2010.08.16
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
//2010.08.16
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
						}
							rp = 0;

							//Mark L Size
							ds1 = dr1 = FX_SIZE;	ds2 = dr2 = 0;
							for( n=0; n<=6; n++ ) {
								ds1 = min( ds1, Mark_size_ls[nc][n][0] );
								dr1 = min( dr1, Mark_size_lr[nc][n][0] );
								ds2 = max( ds2, Mark_size_ls[nc][n][0] );
								dr2 = max( dr2, Mark_size_lr[nc][n][0] );
							}
							ds3 = ds2-ds1;
							dr3 = dr2-dr1;
						if((Option & REPI)!=0){
							if( ds3 <= 4 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( dr3 <= 4 ) Mark_Judge_re[nc][nm][1] = ERROR;
						}else{
							if(( ds3 <= 4 )&&( nc == 1 )) Mark_Judge_st[nc][nm][1] = ERROR;
							if(( dr3 <= 4 )&&( nc == 1 )) Mark_Judge_re[nc][nm][1] = ERROR;
						}
							ds1 = dr1 = FX_SIZE;	ds2 = dr2 = 0;
							for( n=1; n<=4; n++ ) {
								ds1 = min( ds1, Mark_size_ls[nc][n][1] );
								dr1 = min( dr1, Mark_size_lr[nc][n][1] );
								ds2 = max( ds2, Mark_size_ls[nc][n][1] );
								dr2 = max( dr2, Mark_size_lr[nc][n][1] );
							}
							ds3 = ds2-ds1;
							dr3 = dr2-dr1;
						if((Option & REPI)!=0){
							if( ds3 >= 4 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( dr3 >= 4 ) Mark_Judge_re[nc][nm][1] = ERROR;
						} else {
							if(( ds3 >= 4 )&&( nc == 1 )) Mark_Judge_st[nc][nm][1] = ERROR;
							if(( dr3 >= 4 )&&( nc == 1 )) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;                                                  //2011.04.19
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;                                                  //2011.04.19
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
						}
					}
					//3 End
				//4
					nm = 4;
					if( TWA_Spec == SpecResin ) {
						//NP--III検査データ　３桁標印
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 7;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 8;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 8;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 7;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 8;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst > 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre > 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
					} else {
						//通常品検査データ
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
						if((Option & REPI)==0){
							rp = 0;                                                      //2011.04.19
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst < 4 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre < 4 ) Mark_Judge_re[nc][nm][1] = ERROR;
						}
					}
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

					if( TWA_Spec == SpecResin ) {
						//NP--III検査データ　３桁標印
							rp = 0;
						if((Option & REPI)!=0){
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst > 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre > 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
						} else {
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;     //1
							pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;	  //2
					//		pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 2 ) Mark_Judge_st[nc][nm][1] = ERROR;		//jig change nst > 0 to nst ==2
							if( nre == 2 ) Mark_Judge_re[nc][nm][1] = ERROR;		//jig change nre > 0 to nst ==2
						}

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst > 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre > 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
					} else {
						//通常品検査データ
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 0;
							pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

						if((Option & REPI)!=0){
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
						} else {
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 0;
							pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
						}
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

						if((Option & REPI)==0){
							rp = 0;                                                  //2011.04.19
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst <= 1 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre <= 1 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;                                                  //2011.04.19
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst <= 1 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre <= 1 ) Mark_Judge_re[nc][nm][1] = ERROR;
						}
					}
					//5 End
				//6
					nm = 6;
					if( TWA_Spec == SpecResin ) {
						//NP--III検査データ　３桁標印
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst > 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre > 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst > 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre > 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
					} else {
						//通常品検査データ
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 0;
//							pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
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

						if((Option & REPI)!=0){
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 3 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 3 ) Mark_Judge_re[nc][nm][1] = ERROR;
						} else {
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
						//	pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
						}

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

							//Mark L Size
							ds1 = dr1 = FX_SIZE;	ds2 = dr2 = 0;
							for( n=1; n<=5; n++ ) {
								ds1 = min( ds1, Mark_size_ls[nc][n][1] );
								dr1 = min( dr1, Mark_size_lr[nc][n][1] );
								ds2 = max( ds2, Mark_size_ls[nc][n][1] );
								dr2 = max( dr2, Mark_size_lr[nc][n][1] );
							}
							ds3 = ds2-ds1;
							dr3 = dr2-dr1;
						if((Option & REPI)!=0){
							if( ds3 < 5 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( dr3 < 5 ) Mark_Judge_re[nc][nm][1] = ERROR;
						} else {
							if(( ds3 < 5 )&&( nc == 1 )) Mark_Judge_st[nc][nm][1] = ERROR;
							if(( dr3 < 5 )&&( nc == 1 )) Mark_Judge_re[nc][nm][1] = ERROR;
 
							rp = 0;                                                  //2011.04.19
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
						}
					}
					//6 End
				//7
					nm = 7;
				if((Option & REPI)!=0){
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
				} else {
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
				//	pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
				//	pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 1;
				//	pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds2, &dr2 );
			//		if((ds1<2)&&(ds2<2)) Mark_Judge_st[nc][nm][1] = ERROR;					//jig cut out
			//		if((dr1<2)&&(dr2<2)) Mark_Judge_re[nc][nm][1] = ERROR;					//jig cut out
			//		if((ds1!=2)&&(ds2!=2)) Mark_Judge_st[nc][nm][1] = ERROR;				//jig cut out
			//		if((dr1!=2)&&(dr2!=2)) Mark_Judge_re[nc][nm][1] = ERROR;				//jig cut out

					if((ds1<1)&&(ds2!=1)) Mark_Judge_st[nc][nm][1] = ERROR;					//jig
					if((dr1<1)&&(dr2!=1)) Mark_Judge_re[nc][nm][1] = ERROR;					//jig
					if((ds1!=2)&&(ds2!=1)) Mark_Judge_st[nc][nm][1] = ERROR;				//jig
					if((dr1!=2)&&(dr2!=1)) Mark_Judge_re[nc][nm][1] = ERROR;				//jig
				}
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
					if( ds2 >= 3 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( dr2 >= 3 ) Mark_Judge_re[nc][nm][1] = ERROR;

					if( TWA_Spec == SpecResin ) {
						//NP--III検査データ　３桁標印
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst < 3 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre < 3 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 2;	pt[rp++] = 7;	pt[rp++] = 0;
							pt[rp++] = 2;	pt[rp++] = 8;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
					} else {
						//通常品検査データ
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
						if((Option & REPI)==0){
							rp = 0;                                                  //2011.04.19
							pt[rp++] = nc;	pt[rp++] = 1;
						//	pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;		//Jig cut off
							pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
						}
					}
					//7 End
				//8
					nm = 8;
					if( TWA_Spec == SpecResin ) {
						//NP--III検査データ　３桁標印
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;                                              //GEE
							pt[rp++] = nc;	pt[rp++] = 1;                        //GEE
							pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 1;        //GEE
							pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;        //GEE
							pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;        //GEE
							pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 1;        //GEE
							pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 1;        //GEE
							pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;        //GEE
							pt[rp++] = 0;	pt[rp++] = 7;	pt[rp++] = 1;        //GEE
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst <= 5 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre <= 5 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;                                              //GEE
							pt[rp++] = nc;	pt[rp++] = 1;                        //GEE
							pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;        //GEE
							pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;        //GEE
							pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;        //GEE
							pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;        //GEE
							pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 1;        //GEE
							pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;        //GEE
							pt[rp++] = 2;	pt[rp++] = 7;	pt[rp++] = 1;        //GEE
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst <= 5 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre <= 5 ) Mark_Judge_re[nc][nm][1] = ERROR;

			//				rp = 0;
			//				pt[rp++] = nc;	pt[rp++] = 0;
			//				pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 0;
			//				pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 0;
			//				pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 0;
			//				pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
			//				TWA_Mark_error_check( pt, &nst, &nre );
			//				if( nst == 3 ) Mark_Judge_st[nc][nm][1] = ERROR;
			//				if( nre == 3 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 7;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
						if((Option & REPI)==0){
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 8;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
						}
					} else {
						//通常品検査データ
						if((Option & REPI)!=0){
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
						} else {
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 3 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 3 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
						}

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst <= 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre <= 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 0;
							pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst != 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre != 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst < 4 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre < 4 ) Mark_Judge_re[nc][nm][1] = ERROR;

							//Mark L Size
							ds1 = dr1 = FX_SIZE;	ds2 = dr2 = 0;
							for( n=1; n<=5; n++ ) {
								ds1 = min( ds1, Mark_size_ls[nc][n][0] );
								dr1 = min( dr1, Mark_size_lr[nc][n][0] );
								ds2 = max( ds2, Mark_size_ls[nc][n][0] );
								dr2 = max( dr2, Mark_size_lr[nc][n][0] );
							}
							ds3 = ds2-ds1;
							dr3 = dr2-dr1;
						if((Option & REPI)!=0){
							if( ds3 < 4 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( dr3 < 4 ) Mark_Judge_re[nc][nm][1] = ERROR;
						} else {
							if(( ds3 < 4 )&&( nc == 1 )) Mark_Judge_st[nc][nm][1] = ERROR;
							if(( dr3 < 4 )&&( nc == 1 )) Mark_Judge_re[nc][nm][1] = ERROR;
						}
							ds1 = dr1 = FX_SIZE;	ds2 = dr2 = 0;
							for( n=1; n<=5; n++ ) {
								ds1 = min( ds1, Mark_size_ls[nc][n][1] );
								dr1 = min( dr1, Mark_size_lr[nc][n][1] );
								ds2 = max( ds2, Mark_size_ls[nc][n][1] );
								dr2 = max( dr2, Mark_size_lr[nc][n][1] );
							}
							ds3 = ds2-ds1;
							dr3 = dr2-dr1;
						if((Option & REPI)!=0){
							if( ds3 < 4 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( dr3 < 4 ) Mark_Judge_re[nc][nm][1] = ERROR;
						} else {
							if(( ds3 < 4 )&&( nc == 1 )) Mark_Judge_st[nc][nm][1] = ERROR;
							if(( dr3 < 4 )&&( nc == 1 )) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;                                                  //2011.04.19
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;                                                  //2011.04.19
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
						}
					}
					//8 End
				//9
					nm = 9;
					if( TWA_Spec == SpecResin ) {
						//NP--III検査データ　３桁標印
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
							pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst > 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre > 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
					} else {
						//通常品検査データ
						if((Option & REPI)!=0){
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
						} else {
							rp = 0;									//jig change 1 to 0
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 0;				//jig change check white( =0) to check black (=1) 
							pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;				//jig change check white( =0) to check black (=1) 
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;			//jig nst ==2 to nst ==0
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;			//jig nst ==2 to nst ==0
						}

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 1;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst <= 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre <= 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
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

							//Mark L Size
							ds1 = dr1 = FX_SIZE;	ds2 = dr2 = 0;
							for( n=1; n<=5; n++ ) {
								ds1 = min( ds1, Mark_size_ls[nc][n][0] );
								dr1 = min( dr1, Mark_size_lr[nc][n][0] );
								ds2 = max( ds2, Mark_size_ls[nc][n][0] );
								dr2 = max( dr2, Mark_size_lr[nc][n][0] );
							}
							ds3 = ds2-ds1;
							dr3 = dr2-dr1;
						if((Option & REPI)!=0){
							if( ds3 < 5 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( dr3 < 5 ) Mark_Judge_re[nc][nm][1] = ERROR;
						} else {
							if(( ds3 < 5 )&&( nc == 1 )) Mark_Judge_st[nc][nm][1] = ERROR;
							if(( dr3 < 5 )&&( nc == 1 )) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;                                                  //2011.04.19
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
						}
					}
					//9 End
				//R
					nm = 10;
					if( TWA_Spec == SpecResin ) {
						//NP--III検査データ　３桁標印
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 8;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 7;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 8;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst != 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre != 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
					} else	{
						//通常品検査データ
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
							pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

						if((Option & REPI)!=0){
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
						} else {
							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 1;
							pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 1;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 1 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 1 ) Mark_Judge_re[nc][nm][1] = ERROR;

							rp = 0;
							pt[rp++] = nc;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;
							pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 0;
							pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
							TWA_Mark_error_check( pt, &nst, &nre );
							if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
							if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
						}
					}
					//R End

	}
////////////////////////////////////////////
////////////////////////////////////////////

static void	TWA_Mark_J_error( int nc )
	{
		int				nst, nre;
		int				ds1, ds2, ds3;
		int				dr1, dr2, dr3;
		int				nm;
		int				pt[55];
		int				rp = 0;

				for( nm=0; nm<11; nm++ ) {
					Mark_Judge_st[nc][nm][1] = OK;
					Mark_Judge_re[nc][nm][1] = OK;
				}

				if( TWA_Insp_mode == Insp_Pinhole ) return;

				//0
					nm = 0;
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 0;
					pt[rp++] = 3;	pt[rp++] = 2;	pt[rp++] = 0;
					pt[rp++] = 3;	pt[rp++] = 3;	pt[rp++] = 0;
					pt[rp++] = 3;	pt[rp++] = 4;	pt[rp++] = 0;
					pt[rp++] = 4;	pt[rp++] = 2;	pt[rp++] = 0;
					pt[rp++] = 4;	pt[rp++] = 3;	pt[rp++] = 0;
					pt[rp++] = 4;	pt[rp++] = 4;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds1, &dr1 );
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 3;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 3;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 3;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = 4;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = 4;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 4;	pt[rp++] = 4;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &ds2, &dr2 );
					ds3 = ds1 - ds2;
					dr3 = dr1 - dr2;
//					if( ds3<=3 ) Mark_Judge_st[nc][nm][1] = ERROR;
//					if( dr3<=3 ) Mark_Judge_re[nc][nm][1] = ERROR;
					if( ds3<=4 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( dr3<=4 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 3;	pt[rp++] = 0;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 5;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 6;	pt[rp++] = 3;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = 5;	pt[rp++] = 5;	pt[rp++] = 1;
					pt[rp++] = 3;	pt[rp++] = 6;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
//					if( nst<3 ) Mark_Judge_st[nc][nm][1] = ERROR;
//					if( nre<3 ) Mark_Judge_re[nc][nm][1] = ERROR;
					if( nst<4 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre<4 ) Mark_Judge_re[nc][nm][1] = ERROR;

					//0 End
	}

///////////////////////////////////////////
///           Nijimi Check              ///
///////////////////////////////////////////
static void	TWA_Nijimi_error( int nc )
	{
		int				nst=0, nre=0;
//		int				ds1, ds2, ds3, dr1, dr2, dr3;
//		int				nm;
//		int				pt[55];
//		int				rp = 0;
		int				ny;

		//		for( nc=0; nc<3; nm++ ) {                        //GEE change nm for nc
		//			Nijimi_Judge[nc][1] = OK;
		//		}

				for( ny=0; ny<Mark_div_y; ny++ ) {
					if(Mark_div_ws1[nc][ny] == 1)  nst++;
					if(Mark_div_bs1[nc][ny] == 0)  nre++;
					
				}
//					if((Mark_div_ws1[nc][0] == 1) || (Mark_div_ws1[nc][Mark_div_y-1] == 1)) Nijimi_Judge[nc] = ERROR;
					if( nst>=2 ) Nijimi_Judge[nc] = ERROR;

	}


/*******************************************************************************
void	TWA_Mark_error_check( int pt[], int *nst, int *nre )
********************************************************************************/

void	TWA_Mark_error_check( int pt[], int *nst, int *nre )
	{
		int				rp = 0;
		int				cn;
		int				nx, ny;
		int				nd, ns, nr;
		int				co;
		int				mode;

			nd = ns = nr = 0;
			cn = pt[rp++];
			mode = pt[rp++];

			for( ; ; ) {
				nx = pt[rp++];
				ny = pt[rp++];
				if( nx < 0 ) break;
				if( ny < 0 ) break;
				co = pt[rp++];

				nd = ny*Mark_div_x + nx;
				if( mode == 0 ) {
					if( Mark_div_bs[cn][nd] == co ) ns++;		// ＋
					if( Mark_div_br[cn][nd] == co ) nr++;		// ＋
				} else {
					if( Mark_div_ws[cn][nd] == co ) ns++;		// □
					if( Mark_div_wr[cn][nd] == co ) nr++;		// □
				}
			}
			*nst = ns;
			*nre = nr;
	}

/*******************************************************************************
	関数名：	ＴＷＡ＿ｇｅｔ＿ｃｏｌｏｒ＿ａｒｅａ
	機　能：	範囲内の白／黒を数える
	形　式：	void TWA_get_color_area( int page, int pt[], int *jw, int *jb, int nc )
    引き数：	page		バイナリプレーン番号
				pt[0]		計測範囲中心座標 xc
				pt[1]		計測範囲中心座標 yc
				pt[2]		白判定level
				pt[3]		黒判定level
				jb			黒画素数格納ポインタ
				jw			白画素数格納ポインタ
				nc			文字の位置（桁）
	戻り値：	ありません。結果は指定ポインタに格納されます。
********************************************************************************/

static void	TWA_get_color_area( int page, int pt[], int *jw, int *jb, int nc )
	{
		int		 		xs, ys, xe, ye;
		int				lw, lb;
		int				rx, ry, rl;
		unsigned char			*ad;
		int				nw, nb, nt, nj;


			xs = xe = pt[0];
			ys = ye = pt[1];
			lw = pt[2];
			lb = pt[3];

			if( TWA_Spec == SpecResin ) {
				//ＮＰーIII検査データ　３桁標印		約37um/point
					xs -= 5;
					xe += 4;
					ys -= 5;
					ye += 4;
			} else {
                //通常品検査データ
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

	//		nj = nt/2;
	//	 	if( nw >= nj ) *jw = 1;	
	//		else           *jw = 0;
	//	
	//		if( nb <= nj ) *jb = 1;
	//		else           *jb = 0;

		if( TWA_Spec == SpecResin ) {

			 	nj = nt/4;			//JIG^^ 
				if( nw >= nj ) *jw = 1;	
				else           *jw = 0;
				nj = nt/2;
				if( nb <= nj ) *jb = 1;
				else           *jb = 0;
		}	
		else {
		//	if((MarkChar[nc] == '0')&&(nc == 1)){

				nj = nt/2;			//JIG^^ 
				if( nw >= nj ) *jw = 1;	
				else           *jw = 0;
				nj = nt/4;
				if( nb <= nj ) *jb = 1;
				else           *jb = 0;
			}
		//	else {
		//		nj = nt/4;			//JIG^^  
		// 		if( nw >= nj ) *jw = 1;	
		//		else           *jw = 0;
		//		nj = nt/2;
		//		if( nb <= nj ) *jb = 1;
		//		else           *jb = 0;
		//	}
	//	}	


								
//				R_move_gray_memory( 4, 2, 0, 0, FxSize-1, FySize-1 );
//				R_move_gray_memory( 5, 1, 0, 0, FxSize-1, FySize-1 );
			
				
			
	}
/*******************************************************************************
	関数名：	ＴＷＡ＿Ｍａｒｋ＿Ｓｉｚｅ
	機　能：	標印のサイズ計測
	形　式：	void TWA_Mark_Size( int p, int pt[], int *ls, int *le )
    引き数：	page		バイナリプレーン番号
				pt[0]		左座標 x
				pt[1]		左座標 y
				pt[2]		右座標 x
				pt[3]		右座標 y
				pt[4]		Level
				ls			左座標 X start
				le			右座標 X start
	戻り値：	ありません。結果は指定ポインタに格納されます。
********************************************************************************/

void	TWA_Mark_Size( int page, int pt[], int *ls, int *le )
	{

		int				xs, xe;
		int				les;
		int				rx, ry, rl;
		unsigned char	*ad;
		int				i;

			les = pt[4];

		//左-->右
			xs = pt[0] - 10;
			xe = pt[2];
			ry = pt[1];
			ad = Gray_address[page]+ry*MxSize;
			i = 0;
			for( rx=xs; rx<xe; rx++ ) {
				rl = *(ad+rx);
				if( rl >= les ) {
					i++;
					if( i >= 4 ) {
						i--;
						rx -= i;
						break;
					}
				} else {
					i = 0;
				}
			}
			*ls = rx;

		//右-->左
			xs = pt[2] + 10;
			xe = pt[0];
			ry = pt[3];
			ad = Gray_address[page]+ry*MxSize;
			i = 0;
			for( rx=xs; rx>xe; rx-- ) {
				rl = *(ad+rx);
				if( rl >= les ) {
					i++;
					if( i >= 4 ) {
						i--;
						rx += i;
						break;
					}
				} else {
					i = 0;
				}
			}
			*le = rx;

	}

/*******************************************************************************
	関数名：	ＴＷＡ＿ｇｅｔ＿ｃｏｌｏｒ＿ａｒｅａ  f o r  n i j i m i 
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

static void	TWA_get_color_area_nijimi( int page, int pt[], int *jw, int *jb, int nc )
	{
		int		 		xs, ys, xe, ye;
		int				lw, lb;
		int				rx, ry, rl;
		unsigned char			*ad;
		int				nw, nb, nt, nj;
			xs = xe = pt[0];
			ys = ye = pt[1];
			lw = pt[2];
			lb = pt[3];

			if( TWA_Spec == SpecResin ) {
				//ＮＰーIII検査データ　３桁標印		約37um/point
					xs -= 5;
					xe += 4;
					ys -= 5;
					ye += 4;
			} else {
                //通常品検査データ
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

		if( TWA_Spec == SpecResin ) {

			 	nj = nt/1.25;			//JIG^^ 
				if( nw >= nj ) *jw = 1;	
				else           *jw = 0;
				nj = nt/2;
				if( nb <= nj ) *jb = 1;
				else           *jb = 0;
		}	
		else {
	 
				nj = nt/2;			//JIG^^ 
	  			if( nw >= nj ) *jw = 1;	
				else           *jw = 0;
	 	 		nj = nt/4;
				if( nb <= nj ) *jb = 1;
				else           *jb = 0;
			}	
	}


/************************************************
	検査ポイントデータクリア
************************************************/

void	TWA_point_cls( void )
	{
		int			n, n1, n2;

		//文字エリア
			for( n=0; n<125; n++ ) {
				for( n1=0; n1<4; n1++ ) {
					for( n2=0; n2<4; n2++ ) {
						Mark_area_x[n][n1][n2] = 0xffff;
						Mark_area_y[n][n1][n2] = 0xffff;
					}
				}
			}

		//文字検査データ
			for( n=0; n<125; n++ ) {
				for( n1=0; n1<4; n1++ ) {
					for( n2=0; n2<30; n2++ ) {
						Mark_rx[n][n1][n2] = 0xffff;
						Mark_ry[n][n1][n2] = 0xffff;
					}
				}
			}

		//標印文字分割Data
			for( n=0; n<4; n++ ) {
				for( n1=0; n1<50; n1++ ) {
					Mark_div_ws[n][n1] = 0xffff;
					Mark_div_wr[n][n1] = 0xffff;
					Mark_div_bs[n][n1] = 0xffff;
					Mark_div_br[n][n1] = 0xffff;
				}
			}

		//文字間データ
			for( n=0; n<125; n++ ) {
				for( n1=0; n1<5; n1++ ) {
					for( n2=0; n2<10; n2++ ) {
						Mark_space_x[n][n1][n2] = 0xffff;
						Mark_space_y[n][n1][n2] = 0xffff;
					}
				}
			}

	}
/************************************************
	抵抗値設定
************************************************/

void TWA_mark_set_m03( void )
	{
		int			n, i;

		TWA_R_data[0]=TWA_R_data[1]=TWA_R_data[2]=TWA_R_data[3]=TWA_R_data[4]=0;

		i = 0;
		for( n=0; n<4; n++ ) {
			Comment[0] = MarkChar[n]; Comment[1] = 0;
			if( MarkChar[n] == ' ' ) break;
			if( MarkChar[n] == 'R' ) TWA_R_data[n] = 10;
			else TWA_R_data[n] = atoi( Comment );
			if( TWA_R_data[n] == 1 ) i++;
		}

		TWA_R_n = n;

//		TWA_ver_title_set();

		TWA_mark_position_set();
		// RIST 
		if((Option & REPI)==0){
			Mark_Check_St[ 3][ 3]=0x010ff1ff;
			Mark_Check_St[ 3][15]=0x010ff1ff;
			Mark_Check_St[ 7][11]=0x010ff1ff;
			Mark_Check_St[ 9][ 4]=0x000ff1ff;
			Mark_Check_St[ 9][ 7]=0x000ff1ff;
			Mark_Check_St[ 9][17]=0x010ff1ff;
			Mark_Check_P3[ 0][ 0]=0x010ff1ff;
			Mark_Check_P3[ 0][ 2]=0x010ff1ff;
			Mark_Check_P3[ 0][24]=0x010ff1ff;
			Mark_Check_P3[ 0][26]=0x010ff1ff;
			Mark_Check_P3[ 2][ 3]=0x010ff1ff;
			Mark_Check_P3[ 2][ 4]=0x010ff1ff;
			Mark_Check_P3[ 2][13]=0x010ff1ff;
			Mark_Check_P3[ 2][18]=0x010ff1ff;
			Mark_Check_P3[ 2][19]=0x000ff1ff;
			Mark_Check_P3[ 2][22]=0x010ff1ff;
			Mark_Check_P3[ 2][23]=0x010ff1ff;
			Mark_Check_P3[ 3][12]=0x000ff0ff;
			Mark_Check_P3[ 5][12]=0x010ff1ff;
			Mark_Check_P3[ 8][ 0]=0x010ff1ff;
			Mark_Check_P3[ 8][ 2]=0x010ff1ff;
			Mark_Check_P3[ 8][24]=0x010ff1ff;
			Mark_Check_P3[ 8][26]=0x010ff1ff;
			Mark_Check_P3[ 9][26]=0x000ff0ff;
			Mark_Check_P3[10][16]=0x010ff125;
			Mark_Check_P3[10][17]=0x000ff0ff;
			Mark_Check_P3[10][19]=0x010ff126;
			Bin_Ma_l_ini[0][0]=Bin_Ma_l_ini_rist[0][0];
			Bin_Ma_l_ini[0][1]=Bin_Ma_l_ini_rist[0][1];
			Bin_Ma_l_ini[1][0]=Bin_Ma_l_ini_rist[1][0];
			Bin_Ma_l_ini[1][1]=Bin_Ma_l_ini_rist[1][1];
			Bin_Ma_l_ini[2][0]=Bin_Ma_l_ini_rist[2][0];
			Bin_Ma_l_ini[2][1]=Bin_Ma_l_ini_rist[2][1];
		}
	}


/************************************************
	標印検査ポイント設定
************************************************/

static void	TWA_mark_position_set( void )
	{
		int			n, n1, n2, i, deg10;
		int			x, y;
		int			offset_x[4] = {0,0,0,0};
		int			l[4] = {0,0,0,0};
		int			w = 0;
		int			moji;
		double		zoom;
		int			i_mode	= R_CS_GetInspectMode();

		int			x1, x2, x3, x4, x5;
		int			div_n;

		if( i_mode & INSPECT_STEP ) {
			sprintf( Comment,"Debug1:%d",TWA_R_n);
			DrawMessage( Comment, DRAW_GREEN, 2, 2 );
			R_DrawCls();
		}
		TWA_point_cls();

        //文字検査データ(JPW)
			for( n=0; n<50; n++ ) {
				Mark_rx_J[n] = 0xffff;
				Mark_ry_J[n] = 0xffff;
			}

		//Out side Data
			for( n=0; n<5; n++ ) {
				for( n1=0; n1<50; n1++ ) {
					Mark_div_ws1[n][n1] = 0xffff;
			//		Mark_div_wr[n][n1] = 0xffff;
					Mark_div_bs1[n][n1] = 0xffff;
			//		Mark_div_br[n][n1] = 0xffff;
				}
			}


		zoom = (double)1000/(double)SystemPara.nRateX;
		switch( TWA_R_n ) {
			case 1:
				offset_x[0] = Mark_area_0[0];
				l[0] = Mark_area_0[1];
				w  = Mark_area_0[2];
				TWA_mark_area_p[0] = (offset_x[0]-l[0]/2)*zoom;
				TWA_mark_area_p[2] = (offset_x[0]+l[0]/2)*zoom;
				TWA_mark_area_p[1] = (0-w/2)*zoom;
				TWA_mark_area_p[3] = (w/2)*zoom;

				Mark_area_xl = TWA_mark_area_p[2] - TWA_mark_area_p[0];
				Mark_area_yl = w*zoom;
				//標印文字分割数
				Mark_div_x = 7;
				Mark_div_y = 7;
				break;
			case 3:
				if( TWA_Spec == SpecResin ) {
					offset_x[0] = Mark_area_P3[0];
					offset_x[1] = Mark_area_P3[1];
					offset_x[2] = Mark_area_P3[2];
					l[0] = Mark_area_P3[3];
					l[1] = Mark_area_P3[4];
					l[2] = Mark_area_P3[5];
					w  = Mark_area_P3[6];
					//標印文字分割数
					Mark_div_x = 3;
					Mark_div_y = 9;
				} else {
					offset_x[0] = Mark_area_St3[0];
					offset_x[1] = Mark_area_St3[1];
					offset_x[2] = Mark_area_St3[2];
					l[0] = Mark_area_St3[3];
					l[1] = Mark_area_St3[4];
					l[2] = Mark_area_St3[5];
					w  = Mark_area_St3[6];
					//標印文字分割数
					Mark_div_x = 3;
					Mark_div_y = 7;
				}
				TWA_mark_area_p[0] = (offset_x[0]-l[0]/2)*zoom;
				TWA_mark_area_p[2] = (offset_x[2]+l[2]/2)*zoom;
				TWA_mark_area_p[1] = (0-w/2)*zoom;
				TWA_mark_area_p[3] = (w/2)*zoom;

				Mark_area_xl = TWA_mark_area_p[2] - TWA_mark_area_p[0];
				Mark_area_yl = w*zoom;

				break;
			case 4:
				offset_x[0] = Mark_area_St4[0];
				offset_x[1] = Mark_area_St4[1];
				offset_x[2] = Mark_area_St4[2];
				offset_x[3] = Mark_area_St4[3];
				l[0] = Mark_area_St4[4];
				l[1] = Mark_area_St4[5];
				l[2] = Mark_area_St4[6];
				l[3] = Mark_area_St4[7];
				w  = Mark_area_St4[8];
				//標印文字分割数
				Mark_div_x = 3;
				Mark_div_y = 7;

				TWA_mark_area_p[0] = (offset_x[0]-l[0]/2)*zoom;
				TWA_mark_area_p[2] = (offset_x[3]+l[3]/2)*zoom;
				TWA_mark_area_p[1] = (0-w/2)*zoom;
				TWA_mark_area_p[3] = (w/2)*zoom;

				Mark_area_xl = TWA_mark_area_p[2] - TWA_mark_area_p[0];
				Mark_area_yl = w*zoom;

				break;
			default:
				offset_x[0] = Mark_area_St3[0];
				offset_x[1] = Mark_area_St3[1];
				offset_x[2] = Mark_area_St3[2];
				l[0] = Mark_area_St3[3];
				l[1] = Mark_area_St3[4];
				l[2] = Mark_area_St3[5];
				w  = Mark_area_St3[6];
				//標印文字分割数
				Mark_div_x = 3;
				Mark_div_y = 7;
				TWA_mark_area_p[0] = (offset_x[0]-l[0]/2)*zoom;
				TWA_mark_area_p[2] = (offset_x[2]+l[2]/2)*zoom;
				TWA_mark_area_p[1] = (0-w/2)*zoom;
				TWA_mark_area_p[3] = (w/2)*zoom;

				Mark_area_xl = TWA_mark_area_p[2] - TWA_mark_area_p[0];
				Mark_area_yl = w*zoom;

				break;
		}

		//文字エリア
			i = 60;
			for( n=0; n<TWA_R_n; n++ ) {
				Mark_area_x[i][n][0] = Mark_area_x[i][n][2] = (offset_x[n]-l[n]/2)*zoom;		//x1,x3
				Mark_area_x[i][n][1] = Mark_area_x[i][n][3] = (offset_x[n]+l[n]/2)*zoom;		//x2,x4
				Mark_area_y[i][n][0] = Mark_area_y[i][n][1] = (0-w/2)*zoom;						//y1,y2
				Mark_area_y[i][n][2] = Mark_area_y[i][n][3] = (w/2)*zoom;						//y3,y4

		 		if( i_mode & INSPECT_STEP ) {					
					sprintf(Comment,"Mark_area_x[i][%2d][0]=%2d Mark_area_x[i][%2d][1]=%2d",n,Mark_area_x[i][n][0],n,Mark_area_x[i][n][1]);
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					R_DrawCls();

				}

			}

            //文字エリアデータ回転
			for( deg10=-300, i=0; deg10<=300; deg10+=5, i++ ) {
				for( n=0; n<TWA_R_n; n++ ) {
					for( n1=0; n1<4; n1++ ) {
						//検査座標設定
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

		//標印文字範囲設定
//			for( n=0; n<4; n++ ) {
//				TWA_mark_size[n][0] = offset_x[n]*zoom;		//X
//				TWA_mark_size[n][1] = 0;					//Y
//				TWA_mark_size[n][2] = l[n]*zoom;			//L
//				TWA_mark_size[n][3] = w*zoom;				//W
//			}

        //文字検査データ
			i = 60;
			div_n = Mark_div_x * Mark_div_y;
            if( TWA_R_n == 1 ) {	//ＪＰＷ検査データ
				for( n=0; n<div_n; n++ ) {
					Mark_rx_J[n] = ( Mark_point_0[n][0]+offset_x[0] ) * zoom;
					Mark_ry_J[n] = Mark_point_0[n][1] * zoom;
				}
			} else {
				for( n=0; n<TWA_R_n; n++ ) {
					moji = TWA_R_data[n];
					for( n1=0; n1<div_n; n1++ ) {
						if( TWA_Spec == SpecResin ) {
                            //ＮＰーIII検査データ　３桁標印
							Mark_rx[i][n][n1] = ( Mark_point_P3[moji][n1][0]+offset_x[n] ) * zoom;
							Mark_ry[i][n][n1] = Mark_point_P3[moji][n1][1] * zoom;
						} else {
                            //通常品検査データ
							Mark_rx[i][n][n1] = ( Mark_point_St[moji][n1][0]+offset_x[n] ) * zoom;
							Mark_ry[i][n][n1] = Mark_point_St[moji][n1][1] * zoom;
						}
					}
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
                case 1:		//ＪＰＷ検査データ
					x1 = (offset_x[0]-500/2)*zoom;
					x2 = (offset_x[0]+500/2)*zoom;
					for( n=0; n<Mark_div_y; n++ ) {
						Mark_space_x[i][0][n] = x1;
						Mark_space_x[i][1][n] = x2;
					}
					for( n1=0; n1<Mark_div_y; n1++ ) {
						n2 = n1*Mark_div_x;
						Mark_space_y[i][0][n1] = Mark_space_y[i][1][n1] = Mark_ry_J[n2];
					}
					break;
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
				for( n1=0; n1<11; n1++ ) {
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
/************************************************
    インデックステーブルエッジサーチ
************************************************/
/*
int		TWA_Index_search_bin( void )
	{
		int					l, ni;
		int					count, t_count, ans;
		int					ans_up, ans_lo;
		int					xs, xe, ys, ye;
		int					yl;
		int					rxs, rxe;
		int					ans_index = ERROR;
		int						grayPn	= InspGrayPn;
		int						binPn	= InspBinPn;
		int						i_mode	= R_CS_GetInspectMode();
		R_RECT					area;

		// Initialze
			BinClsPn( binPn, 0, 0, FxSize-1, FySize-1 );
		// Search index
			R_gray2bin(100 , 0,0,FxSize-1, FySize-1,  Gray_address[grayPn], Bin_address[binPn] );				//check again bin level

			xs = TWA_X_size/3;
//			xe = xs + ((TWA_X_size*2)/3-1);
			xe = xs+200;
			l = TWA_Y_size/2;
			t_count = xe-xs+1;
			if( t_count == 0 ) return(ERROR);


		//上
			ans_up = 100;
			ys = 0;
			for( ys=ni=2; ys<l; ys++ ) {
				ye = ys;
				count = R_count_bin_area( binPn, xs, ys, xe, ye );
				ans = count*100/t_count;
				if( ans < 40 ) {
					ys+=2;
					ye = ys;
					count = R_count_bin_area(  binPn, xs, ys, xe, ye );
					ans_up = count*100/t_count;
		 			break;
				}

			}


			
			if( ys == ni ) return ERROR;
			if( ys < l ) Index_pos[0] = ys+2;
			else         Index_pos[0] = 0xffff;

				if( i_mode & INSPECT_STEP ) {
					R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
		//			R_DrawLine( DRAW_GREEN,0, Index_pos[0], FxSize-1,  Index_pos[0], DRAW_REVERSE, DASHED_LINE );
				}

		//下
			ans_lo = 100;
			for( ye=ni=TWA_Y_size-2; ye>l; ye-- ) {
				ys = ye;
				count = R_count_bin_area( binPn, xs, ys, xe, ye );
				ans = count*100/t_count;
				if( ans < 30 ) {
					ye-=2;
					ys = ye;
					count = R_count_bin_area(  binPn, xs, ys, xe, ye );
					ans_lo = count*100/t_count;
					break;
				}
			}
			if( ye == ni ) return ERROR;
			if( ye > l ) Index_pos[1] = ye-2;
			else         Index_pos[1] = 0xffff;

//				if( i_mode & INSPECT_STEP ) {
//					R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
//					R_DrawLine( DRAW_GREEN,0, Index_pos[1], FxSize-1,  Index_pos[1], DRAW_REVERSE, DASHED_LINE );
//				}

			if((Index_pos[0]==0xffff)&&(Index_pos[1]==0xffff)) return ERROR;
//			if( Index_pos[0]==0xffff ) {
//				Index_pos[0] = Index_pos[1] - Index_pos[2];
//				return ERROR2;
//			}
//			if( Index_pos[1]==0xffff ) {
//				Index_pos[1] = Index_pos[0] + Index_pos[2];
//				return ERROR2;
//			}
		// Index size
			Index_pos[3] = Index_pos[0];	// 上
			Index_pos[4] = Index_pos[1];	// 下

		//Index left size
			ys = TWA_Y_size/2;
			ye = ys + ((TWA_Y_size*4)/3-1);
			l = TWA_X_size/2;
			t_count = ye-ys+1;
			if( t_count == 0 ) return(ERROR);

			for( xs=ni=2; xs<l; xs++ ) {
				xe = xs;
				count = R_count_bin_area( binPn, xs, ys, xe, ye );
				ans = count*100/t_count;
				if( ans < 30 ) {
					xs+=2;
					xe = xs;
					count = R_count_bin_area(  binPn, xs, ys, xe, ye );
					ans_up = count*100/t_count;
		 			break;
				}
			}
			if( xs == ni ) return ERROR;
			if( xs < l ) Index_pos[5] = xs-2;
 			else         Index_pos[5] = 0xffff;



				if( i_mode & INSPECT_STEP ) {
					R_move_bin_memory( binPn, BinP0,Index_pos[5] ,Index_pos[0],IGuidePos[DataX], Index_pos[1] );
					area.left	= Index_pos[5];
					area.right	= IGuidePos[DataX];
				 	area.top	= Index_pos[0]-2;
					area.bottom	= Index_pos[1]+2;
					R_DrawBox( DRAW_GREEN, area,  DRAW_REVERSE, DASHED_LINE);
					sprintf(Comment,"Index area\nleft = %d\nright = %d\ntop = %d\nbottom = %d",area.left,area.right,area.top,area.bottom);
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				}

*/
/*			ans_index = ERROR;
			if((Index_pos[3]!=0xffff)&&(Index_pos[4]!=0xffff)) {
				TWA_Index_size = Index_pos[4]-Index_pos[3];							 
				yl = abs( TWA_Index_size - Index_pos[2] );							//check again index_pos[2]=680*zoom
				if( yl > Judge_Index[1] ) {
					if( TWA_Index_mode != 0 ) {
						TWA_Index_er++;
						if( TWA_Index_er > Judge_Index_n ) TWA_Index_Stop = ON;
					} else {
						TWA_Index_Stop = OFF;
						TWA_Index_er = 0;
					}
				} else {
					ans_index = OK;
					TWA_Index_Stop = OFF;
					TWA_Index_er = 0;
				}
			}

			if( ans_index != OK ) {
				if( ans_up < ans_lo ) {
					Index_pos[1] = Index_pos[0]+Index_pos[2];
				} else {
					Index_pos[0] = Index_pos[1]-Index_pos[2];
				}
			}

			if( Index_pos[0] < 5 ) return ERROR; 
			if( Index_pos[1] > (TWA_Y_size-5) ) return ERROR; 

			if( i_mode & INSPECT_STEP ) {
			//if( TWA_test != 0 ) {
                R_line( 0, Index_pos[0], FxSize-1, Index_pos[0], GRAPH_REVERSE, DASHED_LINE );
                R_line( 0, Index_pos[1], FxSize-1, Index_pos[1], GRAPH_REVERSE, DASHED_LINE );
				if( ans_index == OK ) {
					sprintf( Comment, "Index size = %s", "OK" );
				} else {
					sprintf( Comment, "Index size = %s", "NG" );
				}
				R_chrdisp( 1, 18, Comment );
				WaitMouseClick();
				sprintf( Comment, "                  " );
				R_chrdisp( 1, 18, Comment );
			}

			Index_pos[2] = TWA_Index[1];

		// Index ガイド位置検出
			l = TWA_X_size/4;
			// up
				ye = Index_pos[0];
				ys = ye - 10;
				if( ys < 0 ) ys = 0;
				t_count = ye-ys+1;
				if( t_count == 0 ) return(ERROR);
				rxs=TWA_X_size-2;
				for( ; rxs>l; rxs-- ) {
					rxe = rxs;
					count = R_count_bin_area( 2, rxs, ys, rxe, ye );
					ans = count*100/t_count;
					if( ans < 10 ) break;
				}
				if( rxs > l ) Index_Gpos[0] = rxs;
				else          Index_Gpos[0] = 0xffff;
			// lo
				ys = Index_pos[1];
				ye = ys + 10;
				if( ye > TWA_Y_size ) ye = TWA_Y_size;
				t_count = ye-ys+1;
				if( t_count == 0 ) return(ERROR);
				rxs=TWA_X_size-2;
				for( ; rxs>l; rxs-- ) {
					rxe = rxs;
					count = R_count_bin_area( 2, rxs, ys, rxe, ye );
					ans = count*100/t_count;
					if( ans < 10 ) break;
				}
				if( rxs > l ) Index_Gpos[1] = rxs;
				else          Index_Gpos[1] = 0xffff;

				if((Index_Gpos[0]!=0xffff)&&(Index_Gpos[1]!=0xffff)) {
					Index_Gpos[2] = min( Index_Gpos[0], Index_Gpos[1] );
				} else if((Index_Gpos[0]==0xffff)&&(Index_Gpos[1]==0xffff)) {
					return ERROR;
				} else if( Index_Gpos[0] == 0xffff ) {
					Index_Gpos[2] = Index_Gpos[1];
				} else if( Index_Gpos[1] == 0xffff ) {
					Index_Gpos[2] = Index_Gpos[0];
				}
			if( i_mode & INSPECT_STEP ) {
				//if( TWA_test != 0 ) {
					R_line( Index_Gpos[2], Index_pos[0], Index_Gpos[2], Index_pos[1], GRAPH_REVERSE, SOLID_LINE );
					WaitMouseClick();
				}

*//*		return OK;
	}
*/
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// APP SEARCH FOR GLASS ////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/*
int	SearchApp_Glass(void)
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
        int                     area_max;
		int						rx, ry,blob_max_no=0;
		int						dn,data;
        int 					elec = 0;
		R_RECT					area;
        int                     App_Y_err=0,App_X_err=0;
		// Initialze
			BinClsPn( binPn, 0, 0, FxSize-1, FySize-1 );

		// Search bin level set
			if( ChipEsLevelSet() != OK )	return ERROR;

		// Chip search Bin image
			ChipGray2Bin();

			BinClsPn( binPn, ElecPosRough[DataLeft][2], 0, ElecPosRough[DataRight][0], FySize-1 );

			if( i_mode & INSPECT_STEP ) {
				R_move_bin_memory( binPn, BinP0,   0, 0, FxSize-1, FySize-1 );
				// --- comment
				sprintf( Comment, " test " );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
			}

		// Initial
			ChipBlobNo = ERROR;
			Elec[DataLeft].BlobNo = Elec[DataRight].BlobNo = ERROR;

		// Initial	( Data clear )
			for( side=0; side<4; side++ ) {
				for( dn=0; dn<700; dn++ ) {
					ElecEdge[elec][side].dataX[dn]	= 0x0ffff;
					ElecEdge[elec][side].dataY[dn]	= 0x0ffff;
					ElecEdge[elec][side].dataR[dn]	= 0x0ffff;
				}
			}

		// area set
			xmin	= Index_pos[5];
			xmax	= IGuidePos[DataX];
			xsize	= xmax-xmin+1;
			ymin	= Index_pos[0];
			ymax	= Index_pos[1];
			ysize	= ymax-ymin+1;

//			xmin	= 0;
//			xmax	= IGuidePos[DataX];
//			xsize	= xmax-xmin+1;
//			ymin	= 0;
//			ymax	= FySize-1;
//			ysize	= ymax-ymin+1;

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
						if( Mpx.center_x[rbn] < (FxSize/2) ) {
							if( Elec[DataLeft].BlobNo == ERROR )	Elec[DataLeft].BlobNo = rbn;	// Left
							Center_L = Mpx.center_x[rbn];                                           // GEE V1.02A
						} else {
							if( Elec[DataRight].BlobNo == ERROR )	Elec[DataRight].BlobNo = rbn;	// Right
							Center_R = Mpx.center_x[rbn];                                           // GEE V1.02A
						}
						if(( Elec[DataLeft].BlobNo != ERROR )&&( Elec[DataRight].BlobNo != ERROR ))	break;
					}
				}
							Center_C = (Center_L + Center_R)/2;
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

		// チップ傾き
			ChipAtan = 0;
			if( res == 0 ){
				xl = Mpx.center_x[Elec[DataRight].BlobNo] - Mpx.center_x[Elec[DataLeft].BlobNo];
				yl = Mpx.center_y[Elec[DataRight].BlobNo] - Mpx.center_y[Elec[DataLeft].BlobNo];
				ChipAtan = R_atan( xl, yl);
				if( i_mode & INSPECT_STEP ) {
					sprintf( Comment, "atan(X:%d Y:%d):%d", xl, yl, ChipAtan );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					R_DrawCls();

				}
			}



		// Initial
			ChipBlobNo = ERROR;
			Elec[DataLeft].BlobNo = Elec[DataRight].BlobNo = ERROR;


		// Initialze
			BinClsPn( binPn, 0, 0, FxSize-1, FySize-1 );

		// Chip search Bin image
			ChipGray2Bin();


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
		
			////////////////////////////////////////////////////
			///////  SEARCH APP FOR SPEC GLASS   ///////////////
			////////////////////////////////////////////////////
				elec = 0 ;
				area_max = 0 ;
				for( bn=0; bn<=Mpx.bn; bn++ ) {
					if( Mpx.color[bn] == ColorBlack ) continue; 
					if(area_max < Mpx.area[bn]) {
						area_max	 = Mpx.area[bn];
						blob_max_no	 = bn;
			 		}
				}
				if( i_mode & INSPECT_STEP ) {
					area.left	= Mpx.imin[blob_max_no];
					area.right	= Mpx.imax[blob_max_no];
					area.top	= Mpx.jmin[blob_max_no];
					area.bottom	= Mpx.jmax[blob_max_no];
					R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
					sprintf( Comment, "Blob_no=%d",blob_max_no );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				}


			for( dn=0; dn<Mpx.runsize; dn++ ) {
//				if( Mpx.runnumb[dn] == Elec[elec].BlobNo ) {
				if( Mpx.runnumb[dn] == blob_max_no ) {
						ry = Mpx.yaxis[dn];
						rx = Mpx.runstrt[dn];
						// --- Xmin
						side = PosXmin;
						if(( ElecEdge[elec][side].dataX[ry] == 0x0ffff )||
							( ElecEdge[elec][side].dataX[ry] > Mpx.runstrt[dn] )) {
								ElecEdge[elec][side].dataX[ry] = Mpx.runstrt[dn];
								ElecEdge[elec][side].dataY[ry] = ry;
								ElecEdge[elec][side].dataR[ry] = OK;
						}
						// --- Xmax
						side = PosXmax;
						if(( ElecEdge[elec][side].dataX[ry] == 0x0ffff )||
							( ElecEdge[elec][side].dataX[ry] < Mpx.runstrt[dn]+Mpx.runleng[dn] )) {
								ElecEdge[elec][side].dataX[ry] = Mpx.runstrt[dn]+Mpx.runleng[dn];
								ElecEdge[elec][side].dataY[ry] = ry;
								ElecEdge[elec][side].dataR[ry] = OK;
						}
						// --- Ymin
						side = PosYmin;
						rx = Mpx.runstrt[dn];
						for( bn=0; bn<Mpx.runleng[dn]; bn++, rx++ ) {
							if(( ElecEdge[elec][side].dataY[rx] == 0x0ffff )||
								( ElecEdge[elec][side].dataY[rx] > Mpx.yaxis[dn] )) {
									ElecEdge[elec][side].dataY[rx] = Mpx.yaxis[dn];
									ElecEdge[elec][side].dataX[rx] = rx;
									ElecEdge[elec][side].dataR[rx] = OK;
							}
						}
						// --- Ymax
						side = PosYmax;
						rx = Mpx.runstrt[dn];
						for( bn=0; bn<Mpx.runleng[dn]; bn++, rx++ ) {
							if(( ElecEdge[elec][side].dataY[rx] == 0x0ffff )||
								( ElecEdge[elec][side].dataY[rx] < Mpx.yaxis[dn] )) {
									ElecEdge[elec][side].dataY[rx] = Mpx.yaxis[dn];
									ElecEdge[elec][side].dataX[rx] = rx;
									ElecEdge[elec][side].dataR[rx] = OK;
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
							if( ElecEdge[elec][side].dataR[rx]  != OK )	continue;
							R_DrawPset( DRAW_GREEN, ElecEdge[elec][side].dataX[rx], ElecEdge[elec][side].dataY[rx], DRAW_REVERSE );
						}
						// Comment disp
							sprintf( Comment, "Electrode search position Ymax" );
							DrawMessage( Comment, DRAW_GREEN, 2, 2 );
					// Bin clear
						R_DrawCls();
//						BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
				}

					for( side=0; side<4; side++ ) {
						if( Regression( elec, side ) != OK )		return ERROR;
					}
					

			/////////////////////////// CHECK APP Y SIZE //////////////////////////   BY JIG
						for( rx=0; rx<FxSize; rx++ ) {
							//check Y size
							if(( ElecEdge[elec][PosYmax].dataY[rx]- ElecEdge[elec][PosYmin].dataY[rx]) > 320 )			//check again (size) compare with min and ma
								App_Y_err++;
						}
						for(ry=0; ry<FySize; ry++){
							//check X size
							if(( ElecEdge[elec][PosXmax].dataX[ry]- ElecEdge[elec][PosXmin].dataX[ry]) > 450 )			//check again (size)
								App_X_err++;
						}
						//Judge
						if(App_Y_err > 20 ) return ERROR2;
						if(App_X_err > 20 ) return ERROR3;

			return OK;
	}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////// G2 search For Glass /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
int		G2App_Glass(void)
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
		int                 			        Center_L, Center_R;              //GEE V1.02A
		int						area_max;
		int						rx, ry,blob_max_no=0;
		int						dn,data;
        int 						elec=0;
		R_RECT						area;
		int						x_edge,y_edge,x_robust,y_robust;
		int						i_left;
		int						y_kake_up,x_kake_up;
		int						y_kake_lo,x_kake_lo;
		int						G2_W,G2_err=0,G2_L,G2_L_err=0;
		int						xs,xe,ys,ye;
			for( side=0; side<4; side++ ) {
				for( dn=0; dn<700; dn++ ) {
					ElecEdge[elec][side].dataX[dn]	= 0x0ffff;
					ElecEdge[elec][side].dataY[dn]	= 0x0ffff;
					ElecEdge[elec][side].dataR[dn]	= 0x0ffff;
				}
			}

		// area set
//				area.left	= ElecPosRough[DataLR][PosXmin];
//				area.right	= ElecPosRough[DataLR][PosXmax];
//				area.top	= ElecPosRough[DataLR][PosYmin];
//				area.bottom	= ElecPosRough[DataLR][PosYmax];

			xmin	= ElecPosRough[DataLR][PosXmin];
			xmax	= ElecPosRough[DataLR][PosXmax];
			xsize	= xmax-xmin+1;
			ymin	= ElecPosRough[DataLR][PosYmin];
			ymax	= ElecPosRough[DataLR][PosYmax];
			ysize	= ymax-ymin+1;

//					if( i_mode & INSPECT_STEP ) {
//						area.left	= xmin;
///						area.right	= xmax;
//						area.top	= ymin;
//						area.bottom	= ymax;
//						R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
//						sprintf( Comment, "start search area G2" );
//						DrawMessage( Comment, DRAW_GREEN, 2, 2 );
//					}

//			xmin	= 0;
//			xmax	= IGuidePos[DataX];
//			xsize	= xmax-xmin+1;
//			ymin	= 0;
//			ymax	= FySize-1;
//			ysize	= ymax-ymin+1;

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
		
			////////////////////////////////////////////////////
			///////  SEARCH APP FOR SPEC GLASS   ///////////////
			////////////////////////////////////////////////////
				elec = 0 ;
				area_max = 0 ;
				for( bn=0; bn<=Mpx.bn; bn++ ) {
					if( Mpx.color[bn] == ColorWhite ) continue; 
					if(area_max < Mpx.area[bn]) {
						area_max	 = Mpx.area[bn];
						blob_max_no	 = bn;
			 		}
				
				}
				if( i_mode & INSPECT_STEP ) {
					area.left	= Mpx.imin[blob_max_no];
					area.right	= Mpx.imax[blob_max_no];
					area.top	= Mpx.jmin[blob_max_no];
					area.bottom	= Mpx.jmax[blob_max_no];
					R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
					sprintf( Comment, "Blob_no=%d",blob_max_no );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				}


			for( dn=0; dn<Mpx.runsize; dn++ ) {
//				if( Mpx.runnumb[dn] == Elec[elec].BlobNo ) {
				if( Mpx.runnumb[dn] == blob_max_no ) {
						ry = Mpx.yaxis[dn];
						rx = Mpx.runstrt[dn];
						// --- Xmin
						side = PosXmin;
						if(( ElecEdge[elec][side].dataX[ry] == 0x0ffff )||
							( ElecEdge[elec][side].dataX[ry] > Mpx.runstrt[dn] )) {
								ElecEdge[elec][side].dataX[ry] = Mpx.runstrt[dn];
								ElecEdge[elec][side].dataY[ry] = ry;
								ElecEdge[elec][side].dataR[ry] = OK;
						}
						// --- Xmax
						side = PosXmax;
						if(( ElecEdge[elec][side].dataX[ry] == 0x0ffff )||
							( ElecEdge[elec][side].dataX[ry] < Mpx.runstrt[dn]+Mpx.runleng[dn] )) {
								ElecEdge[elec][side].dataX[ry] = Mpx.runstrt[dn]+Mpx.runleng[dn];
								ElecEdge[elec][side].dataY[ry] = ry;
								ElecEdge[elec][side].dataR[ry] = OK;
						}
						// --- Ymin
						side = PosYmin;
						rx = Mpx.runstrt[dn];
						for( bn=0; bn<Mpx.runleng[dn]; bn++, rx++ ) {
							if(( ElecEdge[elec][side].dataY[rx] == 0x0ffff )||
								( ElecEdge[elec][side].dataY[rx] > Mpx.yaxis[dn] )) {
									ElecEdge[elec][side].dataY[rx] = Mpx.yaxis[dn];
									ElecEdge[elec][side].dataX[rx] = rx;
									ElecEdge[elec][side].dataR[rx] = OK;
							}
						}
						// --- Ymax
						side = PosYmax;
						rx = Mpx.runstrt[dn];
						for( bn=0; bn<Mpx.runleng[dn]; bn++, rx++ ) {
							if(( ElecEdge[elec][side].dataY[rx] == 0x0ffff )||
								( ElecEdge[elec][side].dataY[rx] < Mpx.yaxis[dn] )) {
									ElecEdge[elec][side].dataY[rx] = Mpx.yaxis[dn];
									ElecEdge[elec][side].dataX[rx] = rx;
									ElecEdge[elec][side].dataR[rx] = OK;
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
//						R_DrawCls();
				}

					for( side=0; side<4; side++ ) {
						if( Regression( elec, side ) != OK )		return ERROR;
					}


				//////////// G2 W side ///////////////
					ys = ElecEdge[elec][PosYmin].MeanY-ElecEdge[elec][PosYmin].Rev*(ElecEdge[elec][PosYmin].MeanX-xmin)+10;
					ye = ElecEdge[elec][PosYmax].MeanY-ElecEdge[elec][PosYmax].Rev*(ElecEdge[elec][PosYmax].MeanX-xmin)-10;
					for( ry=ys; ry<ye; ry++ ) {
						G2_W = ElecEdge[elec][PosXmax].dataX[ry]- ElecEdge[elec][ PosXmin].dataX[ry];
						if(G2_W < 230 ){							//check again (G2 side W)							
						 	G2_err++;	
							if( i_mode & INSPECT_STEP ) {
								R_DrawLine( DRAW_RED,   ElecEdge[elec][PosXmin].dataX[ry],ry,ElecEdge[elec][PosXmax].dataX[ry],ry,  DRAW_REVERSE, SOLID_LINE  );
							}
						}
					}

					//Judge G2 W
					if (G2_err > 20 ) {
						if( i_mode & INSPECT_STEP ) {
							sprintf( Comment, "Check G2 W error" );
							DrawMessage( Comment, DRAW_GREEN, 2, 2 );
						}
						return ERROR;
					}



				//////////// G2 L side ///////////////
					xs =  ElecPosRough[DataLeft][2]+10;
		 			xe =  ElecPosRough[DataRight][0]-10;
//			 		xs =  200;
//					xe =  500;
//						if( i_mode & INSPECT_STEP ) {
//							sprintf( Comment, "xs=%d ,xe = %d",xs,xe );
//							DrawMessage( Comment, DRAW_GREEN, 2, 2 );
//						}

					for( rx=xs; rx<xe; rx++ ) {
						G2_L = ElecEdge[elec][PosYmax].dataY[rx]-ElecEdge[elec][PosYmin].dataY[rx];
						if(G2_L < 140 ){							//check again (G2 side L)							
						 	G2_L_err++;	
							if( i_mode & INSPECT_STEP ) {
								R_DrawLine( DRAW_RED,rx,ElecEdge[elec][PosYmin].dataY[rx],rx,ElecEdge[elec][PosYmax].dataY[rx],  DRAW_REVERSE, SOLID_LINE  );
							}
						}
					}

					//Judge G2 L
					if (G2_L_err > 20 ) {
						if( i_mode & INSPECT_STEP ) {
							sprintf( Comment, "Check G2 L error" );
							DrawMessage( Comment, DRAW_GREEN, 2, 2 );
						}
						return ERROR;
					}







				//////////// G2 KAKE CHECK  ///////////
					//UPER
						//ROBUST DATA
		
						if( i_mode & INSPECT_STEP ) {
							R_DrawCls();
								side = PosYmin;
								xmin = ElecPosRough[DataLeft][2];
								ymin = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
								xmax = ElecPosRough[DataRight][0];
								ymax = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
							R_line( xmin, ymin, xmax, ymax, GRAPH_DRAW, SOLID_LINE );
								side = PosYmax;
								xmin = ElecPosRough[DataLeft][2];
								ymin = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmin);
								xmax = ElecPosRough[DataRight][0];
								ymax = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xmax);
							R_line( xmin, ymin, xmax, ymax, GRAPH_DRAW, SOLID_LINE );
							sprintf( Comment, "Robust line" );
							DrawMessage( Comment, DRAW_GREEN, 2, 2 );
						}
						//Compare data robus and edge search

						for( rx=(ElecPosRough[DataLeft][2]+10); rx<(ElecPosRough[DataRight][0]-10); rx++ ) {
							//upper
							side = PosYmin;
								x_edge	 = ElecEdge[elec][side].dataX[rx];
								y_edge	 = ElecEdge[elec][side].dataY[rx];
								x_robust = ElecEdge[elec][side].dataX[rx];
								y_robust = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-x_robust);
								if( i_mode & INSPECT_STEP ) {
									R_DrawPset( DRAW_GREEN,  x_edge,y_edge, DRAW_REVERSE );
									R_DrawPset( DRAW_GREEN,  x_robust, y_robust, DRAW_REVERSE );
								}

								//Judge
								y_kake_up = y_robust-y_edge;
								if( y_kake_up < -10 ){
									x_kake_up ++;
									if( i_mode & INSPECT_STEP ) {
										R_DrawLine( DRAW_RED, x_edge,y_edge,x_robust, y_robust, DRAW_REVERSE, SOLID_LINE  );
									}
								}
							//lower
							side = PosYmax;
								x_edge	 = ElecEdge[elec][side].dataX[rx], 
								y_edge	 = ElecEdge[elec][side].dataY[rx];
								x_robust = ElecEdge[elec][side].dataX[rx];
								y_robust = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-x_robust);
								if( i_mode & INSPECT_STEP ) {
									R_DrawPset( DRAW_GREEN,  x_edge,y_edge, DRAW_REVERSE );
									R_DrawPset( DRAW_GREEN,  x_robust, y_robust, DRAW_REVERSE );
								}
								//Judge
								y_kake_lo = y_robust-y_edge;
								if( y_kake_lo > 10 ){
									x_kake_lo ++;
									if( i_mode & INSPECT_STEP ) {
//										R_line( x_edge,y_edge,x_robust, y_robust, GRAPH_DRAW, SOLID_LINE );
										R_DrawLine( DRAW_RED,  x_edge,y_edge,x_robust, y_robust, DRAW_REVERSE, SOLID_LINE  );
									}
								}


						}
							if( i_mode & INSPECT_STEP ) {
								sprintf( Comment, "Check G2 kake" );
								DrawMessage( Comment, DRAW_GREEN, 2, 2 );
							}

						//G2 kake Judge
						if( x_kake_up > 10 ) return ERROR;
						if( x_kake_lo > 10 ) return ERROR;
			return OK;

	}
*/
////////////////////////////////////////////////////////////////////////////////////
////////////////////////// Pinhole inspection  /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
int		Pinhole_insp(void)
{
    int						grayPn	= InspGrayPn;
    int						binPn	= InspBinPn;
    int						i_mode	= R_CS_GetInspectMode();
    R_RECT					area;
    int						elec=0,side;
    int						xs_G2,ys_G2,xe_G2,ye_G2;
    int						xs_Ma,ys_Ma,xe_Ma,ye_Ma;

    side = PosYmin;
    xs_G2 = ElecPosRough[DataLeft][2]+5;
    ys_G2 = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xs_G2)+5;
    side = PosYmax;
    xe_G2 = ElecPosRough[DataRight][0]-5;
    ye_G2 = ElecEdge[elec][side].MeanY-ElecEdge[elec][side].Rev*(ElecEdge[elec][side].MeanX-xe_G2)-5;

    if( i_mode & INSPECT_STEP ) {
        area.left	 = xs_G2;
        area.top 	 = ys_G2;
        area.right	 = xe_G2;
        area.bottom	 = ye_G2;
        R_gray2bin( 60,xs_G2,ys_G2,xe_G2,ye_G2, Gray_address[grayPn], Bin_address[binPn] );			//check again (check bin)
        R_move_gray_memory(InspGrayPn,GrayP0, xs_G2,ys_G2,xe_G2,ye_G2 );
        R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
        sprintf( Comment, "G2 area" );
        DrawMessage( Comment, DRAW_GREEN, 2, 2 );
    }
 
    if( TWA_Spec == SpecNormal ) {
        xs_Ma =Ma_pos[0]-5;
        ys_Ma =Ma_pos[2]-5;
        xe_Ma =Ma_pos[1]+5;
        ye_Ma =Ma_pos[3]+5;
        area.left	 = xs_Ma;
        area.top 	 = ys_Ma;
        area.right	 = xe_Ma;
        area.bottom	 = ye_Ma;
//		GrayClsPn( BASE_GRAYADDRESS,xs_Ma,ys_Ma,xe_Ma,ye_Ma );
//      R_bin_cls( 0, 0, FxSize-1, FySize-1 );
        if( i_mode & INSPECT_STEP ) {
            R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
            R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
            sprintf( Comment, "Marking area" );
            DrawMessage( Comment, DRAW_GREEN, 2, 2 );
        }
//      R_move_gray_memory(InspGrayPn,GrayP0, 0, 0, FxSize-1, FySize-1 );
    }
    return OK;
}

/*********************************************************************************
	関数名：	ＴＷＡ＿ｃｏｌｏｒ＿ｐｏｉｎｔ
	機　能：	指定座標の色判別
	形　式：	int TWA_color_point( int p, int x, int y )
	引き数：	p		バイナリプレーン番号
				x,y		指定座標
	戻り値：	値		定　数		意　味
				0		KURO		黒
				1		SHIRO		白
	解　説：	指定画素の色を返す。
*********************************************************************************/
/*
int		TWA_color_point( int p, int x, int y )
{
	div_t bit;
	unsigned long	*ad;
	unsigned long	i = 0x00000001;

	bit = div( x, 32 );
	if( FxSize <= 512 ) {
		ad = (unsigned long *)Bin_address[p] + y*16 + bit.quot;
	} else {
		ad = (unsigned long *)Bin_address[p] + y*FxSize/32 + bit.quot;
	}
	if( bit.rem != 0 ) i <<= bit.rem;

	if( (*ad & i) == 0 ) return( KURO );
	return( SHIRO );
	}
*/
/*********************************************************************************
    関数名：	ＴＷＡ＿ｅｓ＿ｂｉｎ＿ｓｅａｒｃｈ
    機　能：	２値化画像で白黒の境界を捜す
    形　式：	int TWA_es_bin_search( int p, int *rx, int *ry, int deg, int l, int color )
    引き数：	p		処理対象のページ番号
                x,y		認識開始位置の座標
                deg		認識方向を角度（°）で指定( 0, 90, 180, 270 )
                l		サーチ距離
                color	エッジの種類（白→黒：TWA_ES_BLACK、黒→白:TWA_ES_WHITE）を捜すか指定する。
    戻り値：	値		定　数		意　味
                0		OK			エッジが見つかった
                -1		ERROR		エッジが見つからなかった
    解　説：	２値化画像で白黒の境界を捜す。
*********************************************************************************/

int		TWA_es_bin_search( int p, int *rx, int *ry, int deg, int l, int flag )
    {
        div_t           bit;
        unsigned long	*ad;
        unsigned long	i = 0x00000001;
        int				x = *rx;
        int				y = *ry;
        int				color = 0;
        int				es_p = 0;
        int				y_p = 1;
        int				ad_p = MxSize/32;

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
                    for( ; l>0; l-=y_p, y+=y_p, ad+=ad_p ) {
                        if( (*ad & i) == KURO ) break;
                    }
                    break;
                case 270:	//認識方向　上
                    for( ; l>0; l-=y_p, y-=y_p, ad-=ad_p ) {
                        if( (*ad & i) == KURO ) break;
                    }
                    break;
                case 180:	//認識方向　左
                    for( ; l>0; l--, x--, i>>=1 ) {
                        if( i == 0x00000000 ) {
                            ad--;
                            i = 0x80000000;
                        }
                        if( (*ad & i) == KURO ) break;
                    }
                    break;
                case 0:		//認識方向　右
                    for( ; l>0; l--, x++, i<<=1 ) {
                        if( i == 0x00000000 ) {
                            ad++;
                            i = 0x00000001;
                        }
                        if( (*ad & i) == KURO ) break;
                    }
                    break;
                }
        }
        else {
            switch( deg ) {
                case 90:	//認識方向　下
                    for( ; l>0; l-=y_p, y+=y_p, ad+=ad_p ) {
                        if( (*ad & i) != KURO ) break;
                    }
                    break;
                case 270:	//認識方向　上
                    for( ; l>0; l-=y_p, y-=y_p, ad-=ad_p ) {
                        if( (*ad & i) != KURO ) break;
                    }
                    break;
                case 180:	//認識方向　左
                    for( ; l>0; l--, x--, i>>=1 ) {
                        if( i == 0x00000000 ) {
                            ad--;
                            i = 0x80000000;
                        }
                        if( (*ad & i) != KURO ) break;
                    }
                    break;
                case 0:		//認識方向　右
                    for( ; l>0; l--, x++, i<<=1 ) {
                        if( i == 0x00000000 ) {
                            ad++;
                            i = 0x00000001;
                        }
                        if( (*ad & i) != KURO ) break;
                    }
                    break;
                }
        }
        *rx = x;
        *ry = y;
        if( l <= 0 ) return(ERROR);						//認識範囲オーバー
        return(OK);
    }

/************************************************
	Ｇ２エッジGrayサーチ
************************************************/

void	TWA_es_gray_search( int p, int *xs, int *ys, int deg, int rl, int mode )
{
	unsigned char	*ad0, *ad1, *ad2, *ad3, *ad4;
	int				rx, ry;
	int				rl1, rl2;
	int				level, l_max, l_min;
	int				sh, sl;
	int				n;

	rx = *xs;
	ry = *ys;

	l_max = 0;		l_min = 255;

	ad0 = Gray_address[p]+ry*MxSize+rx;
	if((deg==0)||(deg==180)) {
		ad1 = ad0 - MxSize*2;
		ad2 = ad0 + MxSize*2;
		ad3 = ad0 - 1;
		ad4 = ad0 + 1;
	} else {
		ad1 = ad0 - 1;
		ad2 = ad0 + 1;
		ad3 = ad0 - MxSize*2;
		ad4 = ad0 + MxSize*2;
	}

	switch( mode ) {
		case 0:
			rl2 = *ad0;
			break;
		case 1:
			rl2 = ( *ad0 + *ad1 + *ad2 )/3;
			break;
		case 2:
			rl2 = ( *ad0 + *ad1 + *ad2 + *ad3 + *ad4 )/5;
			break;
	}
//	if( mode != 2 ) {
//		rl2 = *ad0;
//	} else {
//		rl2 = ( *ad0 + *ad1 + *ad2 )/3;
//	}

	//最初の山を探す
	for( n=0; n<rl; n++ ) {
		rl1 = rl2;
		switch( deg ) {
			case 90:	//認識方向　下
				ry++;
				ad0 += MxSize;
				ad1 += MxSize;
				ad2 += MxSize;
				ad3 += MxSize;
				ad4 += MxSize;
				break;
			case 270:	//認識方向　上
				ry--;
				ad0 -= MxSize;
				ad1 -= MxSize;
				ad2 -= MxSize;
				ad3 -= MxSize;
				ad4 -= MxSize;
				break;
			case 180:	//認識方向　左
				rx--;
				ad0--;
				ad1--;
				ad2--;
				ad3--;
				ad4--;
				break;
			case 0:		//認識方向　右
				rx++;
				ad0++;
				ad1++;
				ad2++;
				ad3++;
				ad4++;
			break;
		}
		switch( mode ) {
			case 0:
				rl2 = *ad0;
				break;
			case 1:
				rl2 = ( *ad0 + *ad1 + *ad2 )/3;
				break;
			case 2:
				rl2 = ( *ad0 + *ad1 + *ad2 + *ad3 + *ad4 )/5;
				break;
		}
//		if( mode != 2 ) {
//			rl2 = *ad0;
//		} else {
//			rl2 = ( *ad0 + *ad1 + *ad2 )/3;
//		}

		level = (rl1+rl2)/2;
		if( level > l_max ) {
			l_max = level;
			if( mode != 2 ) {
				sh = level*2/3;
			} else {
				sh  = level/2;
			}
//			if( mode == 0 ) sh = level*2/3;
//			else            sh = level/2;
		}
		if( level < sh ) {
			if( mode != 0 ) {
				*xs = rx;
				*ys = ry;
			}
			break;
		}
	}
	if( mode != 0 ) return;

	//最初の谷を探す
	l_min = 255;
	for( ; n<rl; n++ ) {
		rl1 = rl2;
		switch( deg ) {
			case 90:	//認識方向　下
				ry++;
				ad0 += MxSize;
				ad1 += MxSize;
				ad2 += MxSize;
				ad3 += MxSize;
				ad4 += MxSize;
				break;
			case 270:	//認識方向　上
				ry--;
				ad0 -= MxSize;
				ad1 -= MxSize;
				ad2 -= MxSize;
				ad3 -= MxSize;
				ad4 -= MxSize;
				break;
			case 180:	//認識方向　左
				rx--;
				ad0--;
				ad1--;
				ad2--;
				ad3--;
				ad4--;
				break;
			case 0:		//認識方向　右
				rx++;
				ad0++;
				ad1++;
				ad2++;
				ad3++;
				ad4++;
			break;
		}
		switch( mode ) {
			case 0:
				rl2 = *ad0;
				break;
			case 1:
				rl2 = ( *ad0 + *ad1 + *ad2 )/3;
				break;
			case 2:
				rl2 = ( *ad0 + *ad1 + *ad2 + *ad3 + *ad4 )/5;
				break;
		}
//		if( mode != 2 ) {
//			rl2 = *ad0;
//		} else {
//			rl2 = ( *ad0 + *ad1 + *ad2 )/3;
//		}

		level = (rl1+rl2)/2;
		if( level < l_min ) {
			l_min = level;
//			sl = level*2;
			sl = level*3/2;
		}
		if( level > sl ) {
			*xs = rx;
			*ys = ry;
			break;
		}
	}
}



/************************************************
	外形エッジサーチ（上）
************************************************/

int		TWA_Ap_search_up( int binPn ,int mode)
	{
        int					xe, ys, yl, ye;
		int					G2_xs, G2_xe;
		int					rx, ry, rl;
		int					n;
		int					sum_y, sum_n;
		int					i_mode	= R_CS_GetInspectMode();
		R_RECT				area;
//
//		i_mode|=4;

        if( i_mode & INSPECT_STEP ) {
             R_move_bin_memory( binPn, 0, 0, 0, FxSize-1, FySize-1 );
		}

        for( n=0; n<80; n++ ) {
            Ap_data_u[n][0] = 0xffff;
            Ap_data_u[n][1] = 0xffff;
        }
        //サーチ条件設定
			xe = Ap_pos[1]-ES_Ap_xp;									//サーチ終了座標

		//外形上認識開始点の設定
			for( rx=0; rx<Ap_pos[0]; rx+=ES_Ap_xp );
			rx+=ES_Ap_xp;
		//G2 Area
			G2_xs = G2_data_u[0][0];// - 10;
			for( n=0; n<80; n++ ) if( G2_data_u[n][0] == 0xffff ) break;
			n--;
			G2_xe = G2_data_u[n][0];// + 10;
			yl = (G2_pos[3]-G2_pos[2])/5;
		//	rx = G2_xs;
		//	xe = G2_xe;

		//外形エッジサーチ
			sum_y = sum_n = 0;
			for( n=0; n<79; n++, rx+=ES_Ap_xp ) {
				if( rx >= xe ) break;
				ys = ry = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-rx);		//検査スタートＹ座標
//				area.left	= rx;
//				area.right	= rx;
///				area.top	= ry-rl;
//				area.bottom	= ry;
//				R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );

                if(mode != 0){
                    ye = ElecEdge[DataLR][PosYmin].MeanY-ElecEdge[DataLR][PosYmin].Rev*(ElecEdge[DataLR][PosYmin].MeanX-rx);
                }
//                if( TWA_color_point( binPn, rx, ry ) == KURO ) {		//認識開始点の色判別
				if( R_pstate( rx, ry, binPn ) == KURO ) {		//認識開始点の色判別
					if((rx>=G2_xs)&&(rx<=G2_xe)) {
						//Ｇ２～外
                        if(mode == 0){
                            if(ry < yl)	rl = ry;
                            else		rl = yl;
                        } else {
                            rl = ry-ye+2;
                        }
						if( TWA_es_bin_search( binPn, &rx, &ry, 270, rl, TWA_ES_WHITE ) == ERROR ) {
                            if(mode == 0)   ry = ys;
                            else            ry = ye;
							Ap_data_u[n][0] = rx;								//外形座標データ保存
							Ap_data_u[n][1] = ry;								//外形座標データ保存
							sum_y += ry;
							sum_n++;
							continue;
						}
					} else {
						//外～Ｇ２
						rl = yl;
						if( TWA_es_bin_search( binPn, &rx, &ry, 90, rl, TWA_ES_WHITE ) == ERROR ) {
							Ap_data_u[n][0] = rx;								//外形座標データ保存
							Ap_data_u[n][1] = ry;								//外形座標データ保存
							sum_y += ry;
							sum_n++;
							continue;
						}
					}
				}
				//Ｇ２～外
                if(mode == 0){
                    if(ry < yl-2)	rl = ry;
                    else			rl = yl-2;
                } else {
                    rl = ry-ye+2;
					if(rl < 30) rl = 30;
					if(ry-rl < 0)  rl=ry;
				}

                if( TWA_es_bin_search( binPn, &rx, &ry, 270, rl, TWA_ES_BLACK ) != ERROR ){// ry = 2;
                    Ap_data_u[n][0] = rx;								//外形座標データ保存
                    Ap_data_u[n][1] = ry;								//外形座標データ保存
                    sum_y += ry;
                    sum_n++;
				} else {
				//	if(mode==1){
						Ap_data_u[n][0] = rx;								//外形座標データ保存
					//	Ap_data_u[n][1] = ye;								//外形座標データ保存
						Ap_data_u[n][1] = ElecPosRough[DataLR][PosYmin];	//外形がうまく取れない場合はラフ位置決めの値を当てはめる 20170612 tanaka
						sum_y += ye;
						sum_n++;
				//	}
				}
			}
			for( ; n<80; n++ ) {
				Ap_data_u[n][0] = 0xffff;
				Ap_data_u[n][1] = 0xffff;
			}

			if( i_mode & INSPECT_STEP ) {
				for( n=0; n<80; n++ ) {
					if( Ap_data_u[n][0] != 0xffff ) {
						rx = Ap_data_u[n][0];
						ry = Ap_data_u[n][1];
						R_box( rx-1, ry-1, rx+1, ry+1, GRAPH_REVERSE, SOLID_LINE );
					}
				}
				WaitMouseClick();
			}

			if( sum_n == 0 ) ry = 0;
			else			 ry = sum_y/sum_n;

		return(ry);

	}


/************************************************
	外形エッジサーチ（下）
************************************************/

int		TWA_Ap_search_lo( int binPn ,int mode)
	{
		int					xe, ys, ye, yl;
		int					G2_xs, G2_xe;
		int					rx, ry, rl;
		int					n;
		int					sum_y, sum_n;
		int					i_mode	= R_CS_GetInspectMode();
//i_mode |=4;
        for( n=0; n<80; n++ ) {
            Ap_data_d[n][0] = 0xffff;
            Ap_data_d[n][1] = 0xffff;
        }

        //サーチ条件設定
			xe = Ap_pos[1]-ES_Ap_xp;									//サーチ終了座標
			ye = ElecPosRough[DataLR][PosYmax]+3;					// mode==0のときはこの値
			if(ye > FySize-1)	ye==FySize-1;
		//外形上認識開始点の設定
			for( rx=0; rx<Ap_pos[0]; rx+=ES_Ap_xp );
			rx+=ES_Ap_xp;

		//G2 Area
			G2_xs = G2_data_d[0][0] - 10;
			for( n=0; n<80; n++ ) if( G2_data_d[n][0] == 0xffff ) break;
			n--;
			G2_xe = G2_data_d[n][0] + 10;
			yl = (G2_pos[3]-G2_pos[2])/5;
		//	rx = G2_xs;
		//	xe = G2_xe;

		//外形エッジサーチ
			sum_y = sum_n = 0;
			for( n=0; n<79; n++, rx+=ES_Ap_xp ) {
				if( rx >= xe ) break;
				ys = ry = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-rx);	//検査スタートＹ座標
                if(mode != 0){
                    ye = ElecEdge[DataLR][PosYmax].MeanY-ElecEdge[DataLR][PosYmax].Rev*(ElecEdge[DataLR][PosYmax].MeanX-rx);
				}

//				if( TWA_color_point( binPn, rx, ry ) == KURO ) {		//認識開始点の色判別
				if( R_pstate( rx, ry, binPn ) == KURO ) {		//認識開始点の色判別
					if((rx>=G2_xs)&&(rx<=G2_xe)) {
						//Ｇ２～外
					//    if(mode==0){
					//		if(ry + yl > FySize-1 )	rl = FySize-1-(ry+yl);
					//        else		rl = yl;
					//    } else {
                            rl = ye -ry+2;
					//    }
						if( TWA_es_bin_search( binPn, &rx, &ry, 90, rl, TWA_ES_WHITE ) == ERROR ) {
						//	if(mode==0)	ry = ys;
						//	else		ry = ye;
							ry = ye;
							Ap_data_d[n][0] = rx;								//外形座標データ保存
							Ap_data_d[n][1] = ry;								//外形座標データ保存
							sum_y += ry;
							sum_n++;
							continue;
						}
					} else {
						//外～Ｇ２
						rl = yl;
						if( TWA_es_bin_search( binPn, &rx, &ry, 270, rl, TWA_ES_WHITE ) == ERROR ) {
							Ap_data_d[n][0] = rx;								//外形座標データ保存
							Ap_data_d[n][1] = ry;								//外形座標データ保存
							sum_y += ry;
							sum_n++;
							continue;
						}
					}
				}
				//Ｇ２～外
		 //       if(mode==0){
		 //           if(ry + yl > FySize-1-2 )	rl = FySize-1-2-ry;
		 //           else		rl = yl;
		 //       } else {
                    rl = ye - ry+2;
					if(rl < 30) rl = 30;
					if(ry+rl > FySize-1)  rl=FySize-1 - ry;
		//		}
                if( TWA_es_bin_search( binPn, &rx, &ry, 90, rl, TWA_ES_BLACK ) != ERROR ){     // ry = FySize-1-2;
                    Ap_data_d[n][0] = rx;								//外形座標データ保存
                    Ap_data_d[n][1] = ry;								//外形座標データ保存
                    sum_y += ry;
                    sum_n++;
				} else {
				//	if(mode==1){
						Ap_data_d[n][0] = rx;								//外形座標データ保存
					//	Ap_data_d[n][1] = ye;								//外形座標データ保存
						Ap_data_d[n][1] = ElecPosRough[DataLR][PosYmax];	//外形がうまく取れない場合はラフ位置決めの値を当てはめる 20170612 tanaka
						sum_y += ye;
						sum_n++;
				//	}
				}
			}
			for( ; n<80; n++ ) {
				Ap_data_d[n][0] = 0xffff;
				Ap_data_d[n][1] = 0xffff;
			}

			if( i_mode & INSPECT_STEP ) {
				for( n=0; n<80; n++ ) {
					if( Ap_data_d[n][0] != 0xffff ) {
						rx = Ap_data_d[n][0];
						ry = Ap_data_d[n][1];
						R_box( rx-1, ry-1, rx+1, ry+1, GRAPH_REVERSE, SOLID_LINE );
					}
				}
				WaitMouseClick();
			}

			if( sum_n == 0 ) ry = 0;
			else			 ry = sum_y/sum_n;

		return(ry);

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

	//上
		//G2 Area
			G2_xs = G2_data_u[0][0];
			for( n=0; n<80; n++ ) if( G2_data_u[n][0] == 0xffff ) break;
			n--;
			G2_xe = G2_data_u[n][0];

		for( d=0; d<80; d++ ) {
			rx = Ap_data_u[d][0];
			if( rx == 0xffff ) return(ERROR);
			if( rx > G2_xs ) {
				ds = d;
				break;
			}
		}
		for( ; d<80; d++ ) {
			rx = Ap_data_u[d][0];
			if( rx == 0xffff ) return(ERROR);
			if( rx > G2_xe ) {
				d--;
				de = d;
				break;
			}
		}

		if( mode != 0 ) {
			up_sd =  2*Ap_up_y.sd;
			lo_sd = -2*Ap_up_y.sd;
		}
		n = sumx = sumy = 0;
		for( d=ds; d<=de; d++ ) {
			if( mode != 0 ) {
				i = Ap_data_u[d][1];
				i -= Ap_up.kaiki_ey-Ap_up.kaiki_b*(Ap_up.kaiki_ex-Ap_data_u[d][0]);
				if( i < lo_sd ) continue;
				if( i > up_sd ) continue;
			}
			sumx += Ap_data_u[d][0];
			sumy += Ap_data_u[d][1];
			n++;
		}
		if( n == 0 ) return ERROR;
		ex = sumx/n;		//平均Ｘ
		ey = sumy/n;		//平均Ｙ
		sum = sumx = 0;
		for( d=ds; d<=de; d++ ) {
			if( mode != 0 ) {
				i = Ap_data_u[d][1];
				i -= Ap_up.kaiki_ey-Ap_up.kaiki_b*(Ap_up.kaiki_ex-Ap_data_u[d][0]);
				if( i < lo_sd ) continue;
				if( i > up_sd ) continue;
			}
			sumx += pow((Ap_data_u[d][0]-ex),2);
			sum  += ( Ap_data_u[d][0]-ex )*( Ap_data_u[d][1]-ey);
		}
		if( sumx == 0 ) return ERROR;
		Ap_up.kaiki_ex = ex;
		Ap_up.kaiki_ey = ey;
		Ap_up.kaiki_b  = sum/sumx;

	//下
		//G2 Area
			G2_xs = G2_data_d[0][0];
			for( n=0; n<80; n++ ) if( G2_data_d[n][0] == 0xffff ) break;
			n--;
			G2_xe = G2_data_d[n][0];

		for( d=0; d<80; d++ ) {
			rx = Ap_data_d[d][0];
			if( rx == 0xffff ) return(ERROR);
			if( rx > G2_xs ) {
				ds = d;
				break;
			}
		}
		for( ; d<80; d++ ) {
			rx = Ap_data_d[d][0];
			if( rx == 0xffff ) return(ERROR);
			if( rx > G2_xe ) {
				d--;
				de = d;
				break;
			}
		}

		if( mode != 0 ) {
			up_sd =  2*Ap_lo_y.sd;
			lo_sd = -2*Ap_lo_y.sd;
		}
		n = sumx = sumy = 0;
		for( d=ds; d<=de; d++ ) {
			if( mode != 0 ) {
				i = Ap_data_d[d][1];
				i -= Ap_lo.kaiki_ey-Ap_lo.kaiki_b*(Ap_lo.kaiki_ex-Ap_data_d[d][0]);
				if( i < lo_sd ) continue;
				if( i > up_sd ) continue;
			}
			sumx += Ap_data_d[d][0];
			sumy += Ap_data_d[d][1];
			n++;
		}
		if( n == 0 ) return ERROR;
		ex = sumx/n;		//平均Ｘ
		ey = sumy/n;		//平均Ｙ
		sum = sumx = 0;
		for( d=ds; d<=de; d++ ) {
			if( mode != 0 ) {
				i = Ap_data_d[d][1];
				i -= Ap_lo.kaiki_ey-Ap_lo.kaiki_b*(Ap_lo.kaiki_ex-Ap_data_d[d][0]);
				if( i < lo_sd ) continue;
				if( i > up_sd ) continue;
			}
			sumx += pow((Ap_data_d[d][0]-ex),2);
			sum  += ( Ap_data_d[d][0]-ex )*( Ap_data_d[d][1]-ey);
		}
		if( sumx == 0 ) return ERROR;
		Ap_lo.kaiki_ex = ex;
		Ap_lo.kaiki_ey = ey;
		Ap_lo.kaiki_b  = sum/sumx;
	//上下平均
		Ap_w.kaiki_ex = ( Ap_up.kaiki_ex + Ap_lo.kaiki_ex )/2;
		Ap_w.kaiki_ey = ( Ap_up.kaiki_ey + Ap_lo.kaiki_ey )/2;
		Ap_w.kaiki_b  = ( Ap_up.kaiki_b  + Ap_lo.kaiki_b )/2;
/*
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
*/
		return OK;

	}	// KAIKI_keisuu_Ap( int mode ) end

/************************************************
	平均値・標準偏差
************************************************/

static int		TWA_meansd_Ap( void )
	{
		int					d, n, ds, de;
		int					G2_xs, G2_xe, rx;
		float				i, s1, s2;
		int					i_mode	= R_CS_GetInspectMode();

	//上
		//G2 Area
			G2_xs = G2_data_u[0][0];
			for( n=0; n<80; n++ ) if( G2_data_u[n][0] == 0xffff ) break;
			n--;
			G2_xe = G2_data_u[n][0];

		for( d=0; d<80; d++ ) {
			rx = Ap_data_u[d][0];
			if( rx == 0xffff ) return(ERROR);
			if( rx > G2_xs ) {
				ds = d;
				break;
			}
		}
		for( ; d<80; d++ ) {
			rx = Ap_data_u[d][0];
			if( rx == 0xffff ) return(ERROR);
			if( rx > G2_xe ) {
				d--;
				de = d;
				break;
			}
		}

		n=s1=s2=0;
		for( d=ds; d<=de; d++ ) {
			i = Ap_data_u[d][1];
			i -= Ap_up.kaiki_ey-Ap_up.kaiki_b*(Ap_up.kaiki_ex-Ap_data_u[d][0]);
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
		Ap_up_y.mean = s1;
		Ap_up_y.sd = s2;

	//下
		//G2 Area
			G2_xs = G2_data_d[0][0];
			for( n=0; n<80; n++ ) if( G2_data_d[n][0] == 0xffff ) break;
			n--;
			G2_xe = G2_data_d[n][0];

		for( d=0; d<80; d++ ) {
			rx = Ap_data_d[d][0];
			if( rx == 0xffff ) return(ERROR);
			if( rx > G2_xs ) {
				ds = d;
				break;
			}
		}
		for( ; d<80; d++ ) {
			rx = Ap_data_d[d][0];
			if( rx == 0xffff ) return(ERROR);
			if( rx > G2_xe ) {
				d--;
				de = d;
				break;
			}
		}

		n=s1=s2=0;
		for( d=ds; d<=de; d++ ) {
			i = Ap_data_d[d][1];
			i -= Ap_lo.kaiki_ey-Ap_lo.kaiki_b*(Ap_lo.kaiki_ex-Ap_data_d[d][0]);
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
		Ap_lo_y.mean = s1;
		Ap_lo_y.sd = s2;

		if( i_mode & INSPECT_STEP ) {
			printf( "Upper Xbar=%g  SD=%g\n", Ap_up_y.mean, Ap_up_y.sd );
			printf( "Lower Xbar=%g  SD=%g\n", Ap_lo_y.mean, Ap_lo_y.sd );
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

	//	Ap_l_ex = TWA_Ap_search_le();							//外形左検査
	//	Ap_r_ex = TWA_Ap_search_ri();							//外形右検査

		Ap_ue_ey =	TWA_Ap_search_up(binPn,mode);							//外形上検査
        Ap_shita_ey = TWA_Ap_search_lo(binPn,mode);						//外形下検査

/*
		if(( Ap_r_ex-Ap_l_ex ) < Judge_Ap_Lmin[2] ) return(ERROR);

		l = abs( Ap_l_ex - Ap_pos[0] );
		if( l > Judge_Ap_Lsub[2] ) return(ERROR);
		l = abs( Ap_pos[1] - Ap_r_ex );
		if( l > Judge_Ap_Lsub[2] ) return(ERROR);

		for( n=0; n<80; n++ ) {
			if( Ap_data_u[n][1] == 0xffff ) break;
			if( Ap_data_d[n][1] == 0xffff ) break;
			w[n] = Ap_data_d[n][1] - Ap_data_u[n][1];
		}
		w_n = n;
		for( ; n<80; n++ ) w[n] = 0xffff;

		//Ｗ寸法ｍｉｎ判定
		judge_wn = Judge_Ap_Wmin[2] / ES_Ap_xp;
		if( judge_wn <= 0 ) judge_wn = 1;
		for( n=n1=0; n<w_n; n++ ) {
			if( w[n] < Judge_Ap_Wmin[3] ) n1++;
			else                          n1 = 0;
			if( n1 >= judge_wn ) {
				if( i_mode & INSPECT_STEP ) {
					R_line( Ap_data_u[n][0], Ap_data_u[n][1], Ap_data_d[n][0], Ap_data_d[n][1], GRAPH_REVERSE, SOLID_LINE );
				}
				return(ERROR);
			}
		}

		//Ｗ寸法ｍａｘ判定
		judge_wn = Judge_Ap_Wmax[2] / ES_Ap_xp;
		if( judge_wn <= 0 ) judge_wn = 1;
		if( TWA_Insp_level != Insp_Low ) {
			for( n=n1=0; n<w_n; n++ ) {
				if( w[n] > Judge_Ap_Wmax[3] ) n1++;
				else                          n1 = 0;
				if( n1 >= judge_wn ) {
					if( i_mode & INSPECT_STEP ) {
						R_line( Ap_data_u[n][0], Ap_data_u[n][1], Ap_data_d[n][0], Ap_data_d[n][1], GRAPH_REVERSE, SOLID_LINE );
					}
					return(ERROR2);
				}
			}
		}

		w_min = FY_SIZE;
		w_max = 0;
		for( n=2; n<w_n; n++ ) {
			w_sub = abs(w[n]-w[n-1]);
			if( w_sub > Judge_Ap_Wsub[3] ) continue;
			w_sub = abs(w[n]-w[n-2]);
			if( w_sub > Judge_Ap_Wsub[3] ) continue;
			w_sub = abs(w[n-1]-w[n-2]);
			if( w_sub > Judge_Ap_Wsub[3] ) continue;

			w_av = (w[n]+w[n-1]+w[n-2])/3;
			w_min = min( w_min, w_av );
			w_max = max( w_max, w_av );
		}

		w_sub = w_max-w_min;
		if( w_sub > Judge_Ap_Wsub[3] ) {
			if( TWA_test == TWA_Ap ) {
				sprintf( Comment, "W min:%d  max:%d", w_min, w_max );
				R_chrdisp( 1, 18, Comment );
				WaitMouseClick();
			}
			return(ERROR2);
		}
*/
		//回帰係数計算
			if( KAIKI_keisuu_Ap(0) != OK ) return(ERROR3);
		//平均値・標準偏差
			if( TWA_meansd_Ap() != OK ) return(ERROR3);
		//回帰係数計算
			if( KAIKI_keisuu_Ap(1) != OK ) return(ERROR3);
		//外形角確認
//			if( TWA_Ap_corner() != OK ) return(ERROR3);

		return(OK);
	}


/************************************************
	外形認識
************************************************/

int		TWA_Insp_Ap( int mode )
	{
        int				grayPn	= InspGrayPn;
        int				binPn	= InspBinPn;
		int				level = g2level-10;		//Bin_Ap_l[TWA_Spec];
		int				l_mode = 0;
		int				l_p = 4;
		int				ans;
		int				pt[4];
		int				i_mode	= R_CS_GetInspectMode();

		//インデックステーブルエッジ検出
//			switch( TWA_Index_search() ) {
//				case OK:
//					break;
//				case ERROR:
//					return(21);
//					break;
//				case ERROR2:
//					return( TWA_Hyouri_Insp() );
//					break;
//			}
		//２値化（グレープレーンから２値プレーンへ）
			R_gray2bin16( level, 0, 0, TWA_X_size, TWA_Y_size, Gray_address[grayPn], Bin_address[binPn] );
			
			if( i_mode & INSPECT_STEP ) {
				R_move_bin_memory( binPn, 0, 0, 0, FxSize-1, FySize-1 );
				R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30   ,"[TWA_Insp_Ap]");
				R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30+2 ,"level:%d=g2level:%d-10 "   ,level,g2level);
				WaitMouseClick();
				R_DrawCls();
			}
			
		//検査エリア外消去
		//	TWA_bin_cls( binPn, 0, 0, TWA_X_size-1, Index_pos[0]+2 );
		//	TWA_bin_cls( binPn, 0, Index_pos[1]-2, TWA_X_size-1, TWA_Y_size );
		//	TWA_bin_cls( binPn, Index_Gpos[2], Index_pos[0]-2, TWA_X_size-1, Index_pos[1]+2 );
		//ノイズ除去処理
			pt[0]=0;	pt[1]=0;	pt[2]=TWA_X_size-1;	pt[3]=TWA_Y_size-2;
			TWA_expand2( binPn, Ap_expand_x, Ap_expand_y, pt );
			TWA_contrct2( binPn, Ap_contrct_x, Ap_contrct_y, pt );
//			TWA_move_bin_scale( 2, 2, 200, TWA_Y_size );

		//外形検査
        ans = TWA_kensa_Ap(binPn,mode);
/*
		while( ans != OK ) {
			switch( l_mode ) {
				case 0:
					Bin_Ap_l[TWA_Spec] -= l_p;
					if( Bin_Ap_l[TWA_Spec] >= Bin_Ap_l_ini[TWA_Spec][0] ) break;
					Bin_Ap_l[TWA_Spec] = level;
					l_mode++;
				case 1:
					Bin_Ap_l[TWA_Spec] += l_p;
					if( Bin_Ap_l[TWA_Spec] <= Bin_Ap_l_ini[TWA_Spec][1] ) break;
					Bin_Ap_l[TWA_Spec] = level;
					l_mode++;
				default:		//不良
					Bin_Ap_l[TWA_Spec] = level;
					if( TWA_test  == TWA_Ap ) {
						WaitMouseClick();
					}
					return( ans );
			}
			//インデックステーブルエッジ検出
//				switch( TWA_Index_search() ) {
//					case OK:
//						break;
//					case ERROR:
//						return(21);
//						break;
//					case ERROR2:
//						return( TWA_Hyouri_Insp() );
//						break;
//				}
			//２値化（グレープレーンから２値プレーンへ）
				R_gray2bin16( Bin_Ap_l[TWA_Spec], 0, 0, TWA_X_size, TWA_Y_size, Gray_address[2], Bin_address[2] );
			//検査エリア外消去
			//	TWA_bin_cls( binPn, 0, 0, TWA_X_size-1, Index_pos[0]+2 );
			//	TWA_bin_cls( binPn, 0, Index_pos[1]-2, TWA_X_size-1, TWA_Y_size );
			//	TWA_bin_cls( binPn, Index_Gpos[2], Index_pos[0]-2, TWA_X_size-1, Index_pos[1]+2 );
			//ノイズ除去処理
				pt[0]=0;	pt[1]=0;	pt[2]=TWA_X_size-1;	pt[3]=TWA_Y_size-1;
				TWA_expand2( binPn, Ap_expand_x, Ap_expand_y, pt );
				TWA_contrct2( binPn, Ap_contrct_x, Ap_contrct_y, pt );
//				TWA_move_bin_scale( binPn, 2, 200, TWA_Y_size );
			//外形左右位置サーチ
		//		switch( TWA_Ap_search_rl() ) {
		//			case ERROR:
		//				return(23);
		//				break;
		//			case ERROR2:
		//				return(21);
		//				break;
		//			default:
		//				break;
		//		}
			//外形検査
				ans = TWA_kensa_Ap();
		}
*/
		return(ans);
/*
			//２値化（グレープレーンから２値プレーンへ）
				R_gray2bin16( Bin_Ap_l[TWA_Spec], 0, 0, TWA_X_size, TWA_Y_size, Gray_address[2], Bin_address[2] );
			//検査エリア外消去
				TWA_bin_cls( 2, 0, 0, TWA_X_size-1, Index_pos[0]+2 );
				TWA_bin_cls( 2, 0, Index_pos[1]-2, TWA_X_size-1, TWA_Y_size );
				TWA_bin_cls( 2, Index_Gpos[2], Index_pos[0]-2, TWA_X_size-1, Index_pos[1]+2 );
			//ノイズ除去処理
				pt[0]=0;	pt[1]=0;	pt[2]=TWA_X_size-1;	pt[3]=TWA_Y_size-1;
				TWA_expand2( binPn, Ap_expand_x, Ap_expand_y, pt );
				TWA_contrct2( binPn, Ap_contrct_x, Ap_contrct_y, pt );
//				TWA_move_bin_scale( binPn, 2, 200, TWA_Y_size );
			//外形左右位置サーチ
				switch( TWA_Ap_search_rl() ) {
					case ERROR:
						return(23);
						break;
					case ERROR2:
						return(21);
						break;
					default:
						break;
				}
			//外形検査
				ans = TWA_kensa_Ap();
		}

		return(ans);
*/
	}


//Ｇ２検査部

/************************************************
	Ｇ２エッジサーチ（左右）
************************************************/

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


/************************************************
	Ｇ２エッジサーチ（上）
************************************************/
static int		TWA_G2_search_gray_up( int binPn, int grayPn, int mode )
{
		int					xs, ys, xe, ye;
		int					rx, ry, rl, ry2;
		int					rc;
		int					xl, yl;
		int					y_inc;							//傾き*1000000
		int					n;
		int					yo;
		int					i_mode	= R_CS_GetInspectMode();
		R_RECT				area;

			yo = 0;
			if( mode == 0 ) {
			//	if( TWA_Spec == SpecResin ) {
				if( bIsG2Straight ) {
					yo = 8;				// 4->8  2017.03.21  tanaka
				} else {
					//v2.20 if(!bIsLTR50)	yo = 35;
					if((!bIsLTR50) && (!bIsLTR18))	yo = 35;
					else							yo = 40;			// 4->8  2017.03.21  tanaka
				}
			}

		//傾き計算
			xs = (Ap_pos[0]+G2_pos[0])/2;
			xe = (Ap_pos[1]+G2_pos[1])/2;
			xl = xe-xs;
			if( xl <= 0 ) return(ERROR);                //GEE Program hang
			yl = Ap_pos[4]-Ap_pos[2];
			y_inc = yl*10000000/xl;

			//グレーでのエッジサーチにminフィルター後画像を使う
			R_min_filter(grayPn, GrayP2, 0, 0, FxSize-1, FySize/2+10);

		//サーチ条件設定
			xe = G2_pos[1]-4;							//サーチ終了座標
			yl = (Ap_pos[3]-Ap_pos[2])/2;			//サーチ距離 W/2

		//Ｇ２上認識開始点の設定
			rx = ((G2_pos[0]+7)/ES_G2_xp) * ES_G2_xp;		// 8dot 刻みの位置に整列
			rx+=ES_G2_xp;

		for( n=0; n<80; n++ ) {
			G2_data_u[n][0] = 0xffff;
			G2_data_u[n][1] = 0xffff;
			APG2_data_u[n]  = 0xffff;
		}

		//v2.11 if(bIsMCR03 || bIsMCR01){
		if((bIsMCR03 || bIsMCR01)||(Option & BACKINSP_MODE)) {
			//暫定 G2エッジが取れないため、左右の端子上下ポイントで直線を近似
			for( n=0; n<79; n++, rx+=ES_G2_xp ) {
				if( rx >= xe ) break;
				ry = (rx-xs)*y_inc/10000000 + Ap_pos[2] + 4;
				G2_data_u[n][0] = rx;								//Ｇ２座標データ保存
				G2_data_u[n][1] = ry;								//Ｇ２座標データ保存
				APG2_data_u[n]  = ry;
			}
		} else {
			//Ｇ２エッジサーチ
			for( n=0; n<79; n++, rx+=ES_G2_xp ) {
				if( rx >= xe ) break;
				ys = (rx-xs)*y_inc/10000000 + Ap_pos[2] + 4;
				ry = ys + yo;

				if( R_pstate( rx, ry, binPn ) != KURO ) {		//認識開始点の色判別
				//外～Ｇ２
					//if( TWA_es_bin_search( binPn, &rx, &ry, 90, yl, TWA_ES_BLACK ) == ERROR ) {
					R_es_set_page( binPn );			//	Bin画面binPnページを処理対象とする。
					R_es_set_degree( 90 );			//	認識方向をdown 90°方向に設定する。
					R_es_set_length( yl );			//	サーチ距離をylとする。
					R_es_set_noise( 5 );			//	5画素以下の黒（又は白）は無視する。
					R_es_set_start( rx, ry );		//	認識開始点の設定
					rc = ES_WHITE;					//  w -> b
					if( R_es_bin_search( &rx, &ry, &rc ) != OK ){
						if(bIsESR25 ==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){	// MCR03 MCR01
						//	if( TWA_Spec == SpecResin ) {				// ここには来ないはず 要整理
							if( bIsG2Straight ) {
								ry = ys - 4;
							}
						//	else {
						//		return(ERROR);
						//	}
						} else {			// ESR25
							ry = ry2 = ys + yo;
							TWA_es_gray_search( grayPn, &rx, &ry, 90, yl, 1 );
							if( ry == ry2 ) {
								ry -= yl;
							}
						}
					}
				} else {
					//if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){			//MCR03
					if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0){			//MCR03
						if( mode == 0 ) {
							//Ｇ２～外
							rl = ry - ys;
							R_es_set_page( binPn );			//	Bin画面binPnページを処理対象とする。
							R_es_set_degree( 270 );			//	認識方向をup 270°方向に設定する。
							R_es_set_length( rl );			//	サーチ距離をrlとする。
							if(bIsSFR25){
								R_es_set_noise( 2 );			//	5画素以下の黒（又は白）は無視する。
							}else{
								R_es_set_noise( 5 );			//	5画素以下の黒（又は白）は無視する。
							}
							R_es_set_start( rx, ry );		//	認識開始点の設定
							rc = ES_BLACK;					//  black -> white
							if( R_es_bin_search( &rx, &ry, &rc ) != OK ) ry=ys;
						} else {
							//外～Ｇ２
							rl = yl/2;
							R_es_set_page( binPn );			//	Bin画面binPnページを処理対象とする。
							R_es_set_degree( 90 );			//	認識方向をdown 90°方向に設定する。
							R_es_set_length( rl );			//	サーチ距離をrlとする。
							if(bIsSFR25){
								R_es_set_noise( 2 );			//	5画素以下の黒（又は白）は無視する。
							}else{
								R_es_set_noise( 5 );			//	5画素以下の黒（又は白）は無視する。
							}
							R_es_set_start( rx, ry );		//	認識開始点の設定
							rc = ES_WHITE;					//  w -> b
							if( R_es_bin_search( &rx, &ry, &rc ) != OK ) ry=ys;
						}
					} else {
						//Ｇ２～外
						rl = ry - ys;
						R_es_set_page( GrayP2 );		//	グレー画面grayPnページを処理対象とする。
						R_es_set_degree( 270 );			//	認識方向をup 270°方向に設定する。
						R_es_set_length( rl+10 );		//	サーチ距離をrl+10とする。
						R_es_set_level( 10 );			//	10未満の階調変化はエッジとみなさない。
						R_es_set_average( 1 );			//	片側１画素、計３画素の平均を取る。
						R_es_set_noise( 5 );			//	5画素以下の黒（又は白）は無視する。
						R_es_set_slice( 500 );			//	最大値と最小値の真ん中でエッジと判断する。
						R_es_set_start( rx, ry );		//	認識開始点の設定
						rc = ES_BLACK;					//  black -> white
						if( R_es_search( &rx, &ry, &rc ) == OK ){
							ry /=1000;
						} else {
							ry=ys;
						}
						rx /=1000;
					}
				}
				G2_data_u[n][0] = rx;								//Ｇ２座標データ保存
				G2_data_u[n][1] = ry;								//Ｇ２座標データ保存
				if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){	//MCR03 MCR01
					ry = ry - 1;                                        //2011.04.19
					rl = abs(G2_pos[2]-Ap_pos[2])+20;
					R_es_set_page( binPn );			//	Bin画面binPnページを処理対象とする。
					R_es_set_degree( 270 );			//	認識方向をupper 270°方向に設定する。
					R_es_set_length( rl );			//	サーチ距離をrlとする。
					R_es_set_noise( 5 );			//	5画素以下の黒（又は白）は無視する。
					R_es_set_start( rx, ry );		//	認識開始点の設定
					rc = ES_WHITE;					//  w -> b
					if( R_es_bin_search( &rx, &ry, &rc ) != OK ) ry= G2_data_u[n][1];
					APG2_data_u[n]  = ry;
				}
			}
		}

		if( i_mode & INSPECT_STEP ) {
			R_move_gray_memory(GrayP2, GrayP0, 0, 0, FxSize-1,FySize-1);
			for( n=0; n<80; n++ ) {
				if( G2_data_u[n][0] == 0xffff ) break;
				rx = G2_data_u[n][0];
				ry = G2_data_u[n][1];
				R_DrawDispCross( DRAW_GREEN, rx, ry, 2, SOLID_LINE);
				if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){	//MCR03
					rx = G2_data_u[n][0];                           //2011.04.19
					ry = APG2_data_u[n];
					R_DrawDispCross( DRAW_GREEN, rx, ry, 2, SOLID_LINE);
				}
			}
			R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30   ,"[TWA_G2_search_gray_up]");
			//R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30+2 ,"level:%d=g2level:%d-10 "   ,level,g2level);
			WaitMouseClick();
			//R_DrawCls();
		}
	return( OK );
}


/************************************************
	Ｇ２エッジサーチ（下）
************************************************/
static int		TWA_G2_search_gray_lo( int binPn, int grayPn, int mode )
	{
		int					xs, ys, xe, ye;
		int					rx, ry, rl, ry2;
		int					rc;
		int					xl, yl;
		int					y_inc;							//傾き*1000000
		int					n;
		int					yo;
		int					i_mode	= R_CS_GetInspectMode();
		R_RECT				area;


			yo = 0;
			if( mode == 0 ) {
			//	if( TWA_Spec == SpecResin ) {
				if( bIsG2Straight ) {
					yo = 8;				// 4->8  2017.03.21  tanaka
				} else {
					//v2.20 if(!bIsLTR50)	yo = 35;			// 4->8  2017.03.21  tanaka
					if((!bIsLTR50) && (!bIsLTR18))	yo = 35;
					else							yo = 40;
				}
			}

		//傾き計算
			xs = (Ap_pos[0]+G2_pos[0])/2;
			xe = (Ap_pos[1]+G2_pos[1])/2;
			xl = xe-xs;
			if( xl <= 0 ) return(ERROR);                //GEE Program hang
			yl = Ap_pos[5]-Ap_pos[3];
			y_inc = yl*10000000/xl;

		//グレーでのエッジサーチにminフィルター後画像を使う
			R_min_filter(grayPn, GrayP2, 0, FySize/2-10, FxSize-1, FySize-1);

		//サーチ条件設定
			xe = G2_pos[1]-4;							//サーチ終了座標
			yl = (Ap_pos[3]-Ap_pos[2])/2;			//サーチ距離 W/2

		//Ｇ２上認識開始点の設定
			for( rx=0; rx<G2_pos[0]; rx+=ES_G2_xp );
			rx+=ES_G2_xp;

		for( n=0; n<80; n++ ) {
			G2_data_d[n][0] = 0xffff;
			G2_data_d[n][1] = 0xffff;
			APG2_data_d[n]  = 0xffff;
		}

		//v2.11 if(bIsMCR03 || bIsMCR01){
		if((bIsMCR03 || bIsMCR01)||(Option & BACKINSP_MODE)) {
			//暫定 G2エッジが取れないため、左右の端子上下ポイントで直線を近似
			for( n=0; n<79; n++, rx+=ES_G2_xp ) {
				if( rx >= xe ) break;
				ry = (rx-xs)*y_inc/10000000 + Ap_pos[3] + 4;
				G2_data_d[n][0] = rx;								//Ｇ２座標データ保存
				G2_data_d[n][1] = ry;								//Ｇ２座標データ保存
				APG2_data_d[n]  = ry;
			}
		} else {
			//Ｇ２エッジサーチ
			for( n=0; n<79; n++, rx+=ES_G2_xp ) {
				if( rx >= xe ) break;
				ys = (rx-xs)*y_inc/10000000 + Ap_pos[3] - 4;
				ry = ys - yo;

				if( R_pstate( rx, ry, binPn ) != KURO ) {		//認識開始点の色判別
					//外～Ｇ２
					//if( TWA_es_bin_search_n( binPn, &rx, &ry, 270, yl, TWA_ES_BLACK, 5 ) == ERROR ) {
					R_es_set_page( binPn );			//	Bin画面binPnページを処理対象とする。
					R_es_set_degree( 270 );			//	認識方向をup 270°方向に設定する。
					R_es_set_length( yl );			//	サーチ距離をrlとする。
					R_es_set_noise( 5 );			//	5画素以下の黒（又は白）は無視する。
					R_es_set_start( rx, ry );		//	認識開始点の設定
					rc = ES_WHITE;					//  w -> b
					if( R_es_bin_search( &rx, &ry, &rc ) != OK ){
						if(bIsESR25 ==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){	// MCR03 MCR01
							//if( TWA_Spec == SpecResin ) {				//MCR03は、暫定で外形を取るためここにはこないはず
							if( bIsG2Straight ) {
								ry = ys + 4;
							}
						//	else {
						//		return(ERROR);
						//	}
						} else {
							ry = ry2 = ys - yo;
							TWA_es_gray_search( grayPn, &rx, &ry, 270, yl, 1 );
							if( ry == ry2 ) {
								ry += yl;
							}
						}
					}
				} else {
					//if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){			//MCR03
					if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0){			//MCR03
						if( mode == 0 ) {
							//Ｇ２～外
							rl = ys - ry;
							R_es_set_page( binPn );			//	Bin画面binPnページを処理対象とする。
							R_es_set_degree( 90 );			//	認識方向をdown 90°方向に設定する。
							R_es_set_length( rl );			//	サーチ距離をrlとする。
							if(bIsSFR25){
								R_es_set_noise( 2 );			//	5画素以下の黒（又は白）は無視する。
							}else{
								R_es_set_noise( 5 );			//	5画素以下の黒（又は白）は無視する。
							}
							R_es_set_start( rx, ry );		//	認識開始点の設定
							rc = ES_BLACK;					//  black -> white
							if( R_es_bin_search( &rx, &ry, &rc ) != OK ) ry=ys;
						} else {
							//外～Ｇ２
							rl = yl/2;
							R_es_set_page( binPn );			//	Bin画面binPnページを処理対象とする。
							R_es_set_degree( 270 );			//	認識方向をup 270°方向に設定する。
							R_es_set_length( rl );			//	サーチ距離をrlとする。
							if(bIsSFR25){
								R_es_set_noise( 2 );			//	5画素以下の黒（又は白）は無視する。
							}else{
								R_es_set_noise( 5 );			//	5画素以下の黒（又は白）は無視する。
							}
							R_es_set_start( rx, ry );		//	認識開始点の設定
							rc = ES_WHITE;					//  w -> b
							if( R_es_bin_search( &rx, &ry, &rc ) != OK ) ry=ys;
						}
					} else {
						//Ｇ２～外
						rl = ys - ry;
						R_es_set_page( GrayP2 );				//	グレー画面grayPnページを処理対象とする。
						R_es_set_degree( 90 );			//	認識方向をdown 90°方向に設定する。
						R_es_set_length( rl+10 );			//	サーチ距離をrlとする。
						R_es_set_level( 10 );			//	10未満の階調変化はエッジとみなさない。
						R_es_set_average( 1 );			//	片側１画素、計３画素の平均を取る。
						R_es_set_noise( 5 );			//	2画素以下の黒（又は白）は無視する。
						R_es_set_slice( 500 );			//	最大値と最小値の真ん中でエッジと判断する。
						R_es_set_start( rx, ry );		//	認識開始点の設定
						rc = ES_BLACK;					//  black -> white
						if( R_es_search( &rx, &ry, &rc ) == OK ){
							ry /=1000;
						} else {
							ry=ys;
						}
						rx /=1000;
					}
				}
				G2_data_d[n][0] = rx;								//Ｇ２座標データ保存
				G2_data_d[n][1] = ry;								//Ｇ２座標データ保存
				if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){	//MCR03
					ry = ry + 1;                                        //2011.04.19
					rl = abs(Ap_pos[3]-G2_pos[3])+20;
					R_es_set_page( binPn );			//	Bin画面binPnページを処理対象とする。
					R_es_set_degree( 90 );			//	認識方向をdown 90°方向に設定する。
					R_es_set_length( rl );			//	サーチ距離をrlとする。
					R_es_set_noise( 5 );			//	5画素以下の黒（又は白）は無視する。
					R_es_set_start( rx, ry );		//	認識開始点の設定
					rc = ES_WHITE;					//  w -> b
					if( R_es_bin_search( &rx, &ry, &rc ) != OK ) ry = G2_data_d[n][1];
					APG2_data_d[n]  = ry;
				}
			}
		}
		if( i_mode & INSPECT_STEP ) {
			R_move_gray_memory(GrayP2, GrayP0, 0, 0, FxSize-1,FySize-1);
			for( n=0; n<80; n++ ) {
				if( G2_data_d[n][0] == 0xffff ) break;
				rx = G2_data_d[n][0];
				ry = G2_data_d[n][1];
				R_DrawDispCross( DRAW_GREEN, rx, ry, 2, SOLID_LINE);
				if(bIsESR25==0 && bIsMCR10==0 && bIsMCR18==0 && bIsSFR25==0){	//MCR03
					rx = G2_data_d[n][0];
					ry = APG2_data_d[n];
					R_DrawDispCross( DRAW_GREEN, rx, ry, 2, SOLID_LINE);
				}
			}
			R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30   ,"[TWA_G2_search_gray_lo]");
			WaitMouseClick();
			R_DrawCls();
		}
	return( OK );
}
/************************************************
	Ｇ２エッジサーチ（左）
************************************************/

static int		TWA_G2_search_le( int binPn )
	{
		int					ys, ye, rx, ry, rl;
		int					rc;
		int					y_sum, y_n;
		int					n;
		int					haba_y = 8;					//Ｇ２検査幅
		int					i_mode	= R_CS_GetInspectMode();

		R_es_set_page( binPn );			//	バイナリ画面binPnページを処理対象とする。

		//Ｇ２左認識開始点の設定
			y_sum = y_n = 0;
			for( n=3; n<8; n++ ) {
				if( G2_data_u[n][0] == 0xffff ) break;
				y_sum += G2_data_u[n][1];
				y_n++;
			}
			if( y_n == 0 ) return(ERROR);
//			ys = y_sum/y_n+10;
			ys = y_sum/y_n+10;

			y_sum = y_n = 0;
			for( n=3; n<8; n++ ) {
				if( G2_data_d[n][0] == 0xffff ) break;
				y_sum += G2_data_d[n][1];
				y_n++;
			}
			if( y_n == 0 ) return(ERROR);
//			ye = y_sum/y_n-10;
			ye = y_sum/y_n-10;

		//Ｇ２エッジサーチ
			ry = ys;
			for( n=0; n<79; n++, ry+=haba_y ) {
				if( ry >= ye ) break;
				rx = G2_pos[0];
				rl = G2_pos[0] - Ap_pos[0] - 20;
			//	if( TWA_color_point( binPn, rx, ry ) != KURO ) {		//認識開始点の色判別
				if( R_pstate( rx, ry, binPn ) != KURO ) {		//認識開始点の色判別
					//外～Ｇ２
					if( TWA_es_bin_search( binPn, &rx, &ry, 0, 50, TWA_ES_BLACK ) == ERROR ) rx = G2_pos[0] + 100;	//return(ERROR);
				} else {
					//Ｇ２～外
					if( TWA_es_bin_search( binPn, &rx, &ry, 180, rl, TWA_ES_WHITE ) == ERROR ) rx = G2_pos[0];	//return(ERROR);
				}
				G2_data_l[n][0] = rx;								//Ｇ２座標データ保存
				G2_data_l[n][1] = ry;								//Ｇ２座標データ保存
			}
            for( ; n<80; n++ ) {
				G2_data_l[n][0] = 0xffff;
				G2_data_l[n][1] = 0xffff;
			}

		if( i_mode & INSPECT_STEP ) {
//		if( TWA_test == TWA_G2 ) {
			R_RECT area;
			for( n=0; n<80; n++ ) {
				if( G2_data_l[n][0] == 0xffff ) break;
				rx = G2_data_l[n][0];
				ry = G2_data_l[n][1];
			//	R_box( rx-1, ry-1, rx+1, ry+1, GRAPH_REVERSE, SOLID_LINE );
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
	Ｇ２エッジサーチ（右）
************************************************/

static int		TWA_G2_search_ri( int binPn )
	{
		int					ys, ye, rx, ry, rl;
		int					rc;
		int					y_sum, y_n;
		int					n;
		int					haba_y = 8;					//Ｇ２検査幅
		int					i_mode	= R_CS_GetInspectMode();

		R_es_set_page( binPn );			//	バイナリ画面binPnページを処理対象とする。

		//Ｇ２右認識開始点の設定
			y_sum = y_n = 0;
			for( n=0; n<80; n++ ) if( G2_data_u[n][0] == 0xffff ) break;
			n-=5;
			if( n < 5 ) return(ERROR);
			for( ; n<80; n++ ) {
				if( G2_data_u[n][0] == 0xffff ) break;
				y_sum += G2_data_u[n][1];
				y_n++;
			}
			if( y_n == 0 ) return(ERROR);
//			ys = y_sum/y_n+10;
			ys = y_sum/y_n+10;

			y_sum = y_n = 0;
			for( n=0; n<80; n++ ) if( G2_data_d[n][0] == 0xffff ) break;
			n-=5;
			if( n < 5 ) return(ERROR);
			for( ; n<80; n++ ) {
				if( G2_data_d[n][0] == 0xffff ) break;
				y_sum += G2_data_d[n][1];
				y_n++;
			}
			if( y_n == 0 ) return(ERROR);
//			ye = y_sum/y_n-10;
			ye = y_sum/y_n-10;

		//Ｇ２エッジサーチ
			ry = ys;
			for( n=0; n<79; n++, ry+=haba_y ) {
				if( ry >= ye ) break;
				rx = G2_pos[1];
				rl = Ap_pos[1] - G2_pos[1] - 20;
			//	if( TWA_color_point( binPn, rx, ry ) != KURO ) {		//認識開始点の色判別
				if( R_pstate( rx, ry, binPn ) != KURO ) {		//認識開始点の色判別
					//外～Ｇ２
					if( TWA_es_bin_search( binPn, &rx, &ry, 180, 50, TWA_ES_BLACK ) == ERROR ) rx = G2_pos[1] - 50;	// return(ERROR);
				} else {
					//Ｇ２～外
					if( TWA_es_bin_search( binPn, &rx, &ry, 0, rl, TWA_ES_WHITE ) == ERROR ) rx = G2_pos[1];	//return(ERROR);
				}
				G2_data_r[n][0] = rx;								//Ｇ２座標データ保存
				G2_data_r[n][1] = ry;								//Ｇ２座標データ保存
			}
            for( ; n<80; n++ ) {
				G2_data_r[n][0] = 0xffff;
				G2_data_r[n][1] = 0xffff;
			}

		if( i_mode & INSPECT_STEP ) {
			R_RECT area;
			for( n=0; n<80; n++ ) {
				if( G2_data_r[n][0] == 0xffff ) break;
				rx = G2_data_r[n][0];
				ry = G2_data_r[n][1];
			//	R_box( rx-1, ry-1, rx+1, ry+1, GRAPH_REVERSE, SOLID_LINE );
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

int		TWA_G2_ES_search( int mode )
	{
        int					grayPn	= InspGrayPn;
        int					binPn	= InspBinPn;
        int					i_mode	= R_CS_GetInspectMode();

        if( i_mode & INSPECT_STEP ){
            R_display_control( BIN_MEMORY | GRAY_MEMORY );
            R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1);
        }

		if( i_mode & INSPECT_STEP ){
			R_move_bin_memory( binPn, 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
		}

	//Ｇ２上
		if( TWA_G2_search_gray_up( binPn, grayPn, mode ) != OK ) return(ERROR);
	//Ｇ２下
		if( TWA_G2_search_gray_lo( binPn, grayPn, mode ) != OK ) return(ERROR);

		if( mode != 0 ) return(OK);

	//Ｇ２左
		if( TWA_G2_search_le(binPn) != OK ) return(ERROR);
	//Ｇ２右
		if( TWA_G2_search_ri(binPn) != OK ) return(ERROR);

		if( i_mode & INSPECT_STEP ){
			WaitMouseClick();
		}
		return(OK);

	}

/************************************************
	回帰係数
************************************************/

int		KAIKI_keisuu( int mode )
	{
		int						ns, ne, dn;
		int						xs, xe, ys, ye;
		float					sum, sumx, sumy;
		float					ex, ey;
		float					i, up_sd, lo_sd;
        int						i_mode	= R_CS_GetInspectMode();

	//上
		if( mode != 0 ) {
//			up_sd =  2*up_y.sd;
			up_sd =  5;
			lo_sd = -2*up_y.sd;
//			lo_sd = -5;
		}
		dn = sumx = sumy = 0;
        for( ns=2; ns<80; ns++ ) {		//最初と最後の 2dataは、無効とする。
			if( G2_data_u[ns+2][0] == 0xffff ) break;
			if( mode != 0 ) {
				i = G2_data_u[ns][1];
				i -= ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-G2_data_u[ns][0]);
				if( i < lo_sd ) continue;
				if( i > up_sd ) continue;
			}
			sumx += G2_data_u[ns][0];
			sumy += G2_data_u[ns][1];
			dn++;
		}
		if( dn == 0 ) return ERROR;
		ne = ns;
		if( dn > 0 ) {
			ex = sumx/dn;		//平均Ｘ
			ey = sumy/dn;		//平均Ｙ
		}
		sum = sumx = 0;
		for( ns=2; ns<ne; ns++ ) {
			if( mode != 0 ) {
				i = G2_data_u[ns][1];
				i -= ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-G2_data_u[ns][0]);
				if( i < lo_sd ) continue;
				if( i > up_sd ) continue;
			}
			sumx += pow((G2_data_u[ns][0]-ex),2);
			sum  += ( G2_data_u[ns][0]-ex )*( G2_data_u[ns][1]-ey);
		}
		if( sumx > 0 ) {
			ue.kaiki_ex = ex;
			ue.kaiki_ey = ey;
			ue.kaiki_b  = sum/sumx;
		}

	//下
		if( mode != 0 ) {
			up_sd =  2*lo_y.sd;
//			up_sd =  5;
//			lo_sd = -2*lo_y.sd;
			lo_sd = -5;
		}
		dn = sumx = sumy = 0;
        for( ns=2; ns<80; ns++ ) {		//最初と最後の 2dataは、無効とする。
			if( G2_data_d[ns+2][0] == 0xffff ) break;
			if( mode != 0 ) {
				i = G2_data_d[ns][1];
				i -= shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-G2_data_d[ns][0]);
				if( i < lo_sd ) continue;
				if( i > up_sd ) continue;
			}
			sumx += G2_data_d[ns][0];
			sumy += G2_data_d[ns][1];
			dn++;
		}
		if( dn == 0 ) return(ERROR);
		ne = ns;
		if( dn > 0 ) {
			ex = sumx/dn;		//平均Ｘ
			ey = sumy/dn;		//平均Ｙ
		}
		sum = sumx = 0;
		for( ns=2; ns<ne; ns++ ) {
			if( mode != 0 ) {
				i = G2_data_d[ns][1];
				i -= shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-G2_data_d[ns][0]);
				if( i < lo_sd ) continue;
				if( i > up_sd ) continue;
			}
			sumx += pow((G2_data_d[ns][0]-ex),2);
			sum  += ( G2_data_d[ns][0]-ex )*( G2_data_d[ns][1]-ey);
		}
		if( sumx > 0 ) {
			shita.kaiki_ex = ex;
			shita.kaiki_ey = ey;
			shita.kaiki_b  = sum/sumx;
		}
	//上下平均
		w.kaiki_ex = ( ue.kaiki_ex + shita.kaiki_ex )/2;
		w.kaiki_ey = ( ue.kaiki_ey + shita.kaiki_ey )/2;
		w.kaiki_b  = ( ue.kaiki_b  + shita.kaiki_b )/2;

	//右
		if( mode != 0 ) {
//			up_sd =  2*ri_x.sd;
//			lo_sd = -2*ri_x.sd;
			up_sd =  5;
			lo_sd = -5;
			if(bIsMCR01){		//2017.08.22 凸状電極のため回帰直線がずれる
				up_sd = 15;
				lo_sd = -15;
			}
		}
		dn = sumx = sumy = 0;
        for( ns=1; ns<80; ns++ ) {		//最初と最後のデータは、無効とする。
			if( G2_data_r[ns+1][0] == 0xffff ) break;
			if( mode != 0 ) {
				i = G2_data_r[ns][0];
				i -= migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-G2_data_r[ns][1]);
				if( i < lo_sd ) continue;
				if( i > up_sd ) continue;
			}
			sumx += G2_data_r[ns][0];
			sumy += G2_data_r[ns][1];
			dn++;
		}
		if( dn == 0 ) return(ERROR);
		ne = ns;
		if( dn > 0 ) {
			ex = sumx/dn;		//平均Ｘ
			ey = sumy/dn;		//平均Ｙ
		}
		sum = sumy = 0;
		for( ns=1; ns<ne; ns++ ) {
			if( mode != 0 ) {
				i = G2_data_r[ns][0];
				i -= migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-G2_data_r[ns][1]);
				if( i < lo_sd ) continue;
				if( i > up_sd ) continue;
			}
			sumy += pow((G2_data_r[ns][1]-ey),2);
			sum  += ( G2_data_r[ns][0]-ex )*( G2_data_r[ns][1]-ey);
		}
		if( sumy > 0 ) {
			migi.kaiki_ex = ex;
			migi.kaiki_ey = ey;
			migi.kaiki_b  = sum/sumy;
		}

	//左
		if( mode != 0 ) {
//			up_sd =  2*le_x.sd;
//			lo_sd = -2*le_x.sd;
			up_sd =  5;
			lo_sd = -5;
			if(bIsMCR01){		//2017.08.22 凸状電極のため回帰直線がずれる
				up_sd = 15;
				lo_sd = -15;
			}
		}
		dn = sumx = sumy = 0;
        for( ns=1; ns<80; ns++ ) {		//最初と最後のデータは、無効とする。
			if( G2_data_l[ns+1][0] == 0xffff ) break;
			if( mode != 0 ) {
				i = G2_data_l[ns][0];
				i -= hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-G2_data_l[ns][1]);
				if( i < lo_sd ) continue;
				if( i > up_sd ) continue;
			}
			sumx += G2_data_l[ns][0];
			sumy += G2_data_l[ns][1];
			dn++;
		}
		if( dn == 0 ) return(ERROR);
		ne = ns;
		if( dn > 0 ) {
			ex = sumx/dn;		//平均Ｘ
			ey = sumy/dn;		//平均Ｙ
		}
		sum = sumy = 0;
		for( ns=1; ns<ne; ns++ ) {
			if( mode != 0 ) {
				i = G2_data_l[ns][0];
				i -= hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-G2_data_l[ns][1]);
				if( i < lo_sd ) continue;
				if( i > up_sd ) continue;
			}
			sumy += pow((G2_data_l[ns][1]-ey),2);
			sum  += ( G2_data_l[ns][0]-ex )*( G2_data_l[ns][1]-ey);
		}
		if( sumy > 0 ) {
			hidari.kaiki_ex = ex;
			hidari.kaiki_ey = ey;
			hidari.kaiki_b  = sum/sumy;
		}
	//左右平均
		l.kaiki_ex = ( migi.kaiki_ex + hidari.kaiki_ex )/2;
		l.kaiki_ey = ( migi.kaiki_ey + hidari.kaiki_ey )/2;
		l.kaiki_b  = ( migi.kaiki_b  + hidari.kaiki_b )/2;

        if( i_mode & INSPECT_STEP ){
			if( mode != 0 ) {
			//	R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
				R_DrawCls();
//				R_move_gray_memory( grayPn, GrayP0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
			}
			xs = 5; xe = TWA_X_size-5;
				ys = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-xs);
				ye = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-xe);
				//R_line( xs, ys, xe, ye, GRAPH_REVERSE, SOLID_LINE );
				R_DrawLine(DRAW_GREEN, xs, ys, xe, ye, GRAPH_DRAW, SOLID_LINE);
				R_mouse_de_yesno();
				//sprintf(Comment,"what line line 1");
				//R_chrdisp(1,16,Comment);
				//WaitMouseClick();

				ys = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-xs);
				ye = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-xe);
				//R_line( xs, ys, xe, ye, GRAPH_REVERSE, SOLID_LINE );
				R_DrawLine(DRAW_GREEN, xs, ys, xe, ye, GRAPH_DRAW, SOLID_LINE);
				R_mouse_de_yesno();
				//sprintf(Comment,"what line line 2");
				//R_chrdisp(1,16,Comment);
				//WaitMouseClick();

			ys = 5; ye = TWA_Y_size-5;
				xs = hidari.kaiki_ex-w.kaiki_b*(ys-hidari.kaiki_ey);
				xe = hidari.kaiki_ex-w.kaiki_b*(ye-hidari.kaiki_ey);
//				xs = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-ys);
//				xe = hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-ye);
				//R_line( xs, ys, xe, ye, GRAPH_REVERSE, SOLID_LINE );
				R_DrawLine(DRAW_GREEN, xs, ys, xe, ye, GRAPH_DRAW, SOLID_LINE);
				R_mouse_de_yesno();
				//sprintf(Comment,"what line line 3");
				//R_chrdisp(1,14,Comment);
				//WaitMouseClick();

				xs = migi.kaiki_ex-w.kaiki_b*(ys-migi.kaiki_ey);
				xe = migi.kaiki_ex-w.kaiki_b*(ye-migi.kaiki_ey);
//				xs = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-ys);
//				xe = migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-ye);
				//R_line( xs, ys, xe, ye, GRAPH_REVERSE, SOLID_LINE );
				R_DrawLine(DRAW_GREEN, xs, ys, xe, ye, GRAPH_DRAW, SOLID_LINE);
				R_mouse_de_yesno();
				//sprintf(Comment,"what line line 4");
				//R_chrdisp(1,16,Comment);
				//WaitMouseClick();

				WaitMouseClick();
		}

		return OK;

	}	// KAIKI_keisuu( void ) end


/************************************************
	平均値・標準偏差
************************************************/

void	TWA_meansd( void )
	{
		int					n, dn;
		float				i, s1, s2;

	//上
		dn=s1=s2=0;
        for( n=1; n<80; n++ ) {		//最初と最後のデータは、無効とする。
			if( G2_data_u[n+1][0] == 0xffff ) break;
			i = G2_data_u[n][1];
			i -= ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-G2_data_u[n][0]);
			i -= s1;
			dn++;
			s1 += i/dn;
			s2 += ( dn - 1 ) * i * i / dn;
		}
		if( dn > 1 ) {
			s2 = sqrt( s2 / ( dn -1 ));
		} else {
			s2 = 0;
		}
		up_y.mean = s1;
		up_y.sd = s2;
	//下
		dn=s1=s2=0;
        for( n=1; n<80; n++ ) {
			if( G2_data_d[n+1][1] == 0xffff ) break;
			i = G2_data_d[n][1];
			i -= shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-G2_data_d[n][0]);
			i -= s1;
			dn++;
			s1 += i/dn;
			s2 += ( dn - 1 ) * i * i / dn;
		}
		if( dn > 1 ) {
			s2 = sqrt( s2 / ( dn -1 ));
		} else {
			s2 = 0;
		}
		lo_y.mean = s1;
		lo_y.sd = s2;
	//左
		n=s1=s2=0;
		while( G2_data_l[n][0] != 0xffff ) {
			i = G2_data_l[n][0];
			i -= hidari.kaiki_ex-hidari.kaiki_b*(hidari.kaiki_ey-G2_data_l[n][1]);
			i -= s1;
			n++;
			s1 += i/n;
			s2 += ( n - 1 ) * i * i / n;
		}
		if( n > 1 ) {
			s2 = sqrt( s2 / ( n -1 ));
		} else {
			s2 = 0;
		}
		le_x.mean = s1;
		le_x.sd = s2;
	//右
		n=s1=s2=0;
		while( G2_data_r[n][0] != 0xffff ) {
			i = G2_data_r[n][0];
			i -= migi.kaiki_ex-migi.kaiki_b*(migi.kaiki_ey-G2_data_r[n][1]);
			i -= s1;
			n++;
			s1 += i/n;
			s2 += ( n - 1 ) * i * i / n;
		}
		if( n > 1 ) {
			s2 = sqrt( s2 / ( n -1 ));
		} else {
			s2 = 0;
		}
		ri_x.mean = s1;
		ri_x.sd = s2;


//		if( TWA_test == TWA_G2 ) {
//			printf( "上 Xbar=%g  SD=%g\n", up_y.mean, up_y.sd );
//			printf( "下 Xbar=%g  SD=%g\n", lo_y.mean, lo_y.sd );
//			printf( "左 Xbar=%g  SD=%g\n", le_x.mean, le_x.sd );
//			printf( "右 Xbar=%g  SD=%g\n", ri_x.mean, ri_x.sd );
//		}

	}






/************************************************
	電極幅検査
************************************************/

int		TWA_JudgeC2SizeX( int bin_pn )
	{
		int						xs, xe;
		int						ys, ye;
		int						rx, ry, rl, rc;
		int						m;
		int						G2_x;
		int						d, dn;
		int						c2l, C2fail, C2fail_max;
		int						judge_c2n;
		int						cx, cy;
		int						offset_lx = 2;
		int						offset_rx = 2;
		int						offset_xs = 20;        	// -3  nok for adjust up corner
		int						offset_uy =  6;			//2  JHONG
		int						offset_ly =  4;       	//2   nok for adj.down corner
        int						size_y = 40;
		int						judge_lu, judge_ll, judge_ru, judge_rl;
		int						n=0;
		int						countlu=0;
		int						countylu=0;
		int						countylo=0;
		int						countyru=0;
		int						countyrl=0;
		int						countlo=0;
		int						countru=0;
		int						countrl=0;
		int						rx_leup=0;
		int						rx_lelo=0;
		int						rx_riup=0;
		int						rx_rilo=0;
		int						FreqData_leup_max=0;
		int						FreqData_lelo_max=0;
		int						FreqData_riup_max=0;
		int						FreqData_rilo_max=0;
		int						xs_leup=0;
		int						xs_lelo=0;
		int						xs_riup=0;
		int						xs_rilo=0;
		int						ys_leup[200];
		int						ys_lelo[200];
		int						xxs_leup[200];
		int						xxs_lelo[200];
		int						ys_riup[200];
		int						ys_rilo[200];
		int						xxs_riup[200];
		int						xxs_rilo[200];
//		int 						be_nlu=0;

		int						C2_le_up_size=0;
		int						C2_le_lo_size=0;
		int						C2_ri_up_size=0;
		int						C2_ri_lo_size=0;


		int	le_up[4];				//xs, ys, xe, ye
		int	le_lo[4];
		int	ri_up[4];
		int	ri_lo[4];
		int						i_mode	= R_CS_GetInspectMode();

		if(bIsESR25 || bIsMCR10 || bIsMCR18 || bIsSFR25){
            offset_uy =  0;
			offset_ly =  0;
            size_y    = 20;
        }

		Corner_pos[0]=Elec[DataLR].CornerPosX[0];	Corner_pos[1]=Elec[DataLR].CornerPosY[0];
		Corner_pos[2]=Elec[DataLR].CornerPosX[1];	Corner_pos[3]=Elec[DataLR].CornerPosY[1];
		Corner_pos[4]=Elec[DataLR].CornerPosX[2];	Corner_pos[5]=Elec[DataLR].CornerPosY[2];
		Corner_pos[6]=Elec[DataLR].CornerPosX[3];	Corner_pos[7]=Elec[DataLR].CornerPosY[3];
		
		R_es_set_page( bin_pn );
		// Judge data
//			judge_c2n = Judge_C2_Lmin[3];
//			judge_c2n = (45 * 1000)/3420; //Judge_C2_Lmin[3];
			judge_c2n = InspPara.ElecC2Xmin[TWA_Spec].nYsize;
		// initial
			//TWA_bin_cls( 1, 0, 0, FX_SIZE-1, FY_SIZE-1 );
		//	TWA_bin_cls( bin_pn, Index_Gpos[2]-2, Index_pos[0]-2, TWA_X_size-1, Index_pos[1]+2 );   //GEE
		//	TWA_bin_cls( bin_pn, Index_Gpos[2]-2, Index_pos[0]-2, TWA_X_size-1, Index_pos[1]+2 );   //GEE

		// 検査範囲設定
		//左上 area
			// xe
				le_up[0] = Corner_pos[0] - offset_lx;
				// xs
				G2_x = dn = 0;
				for( d=2; d<80; d++ ) {
					if( G2_data_l[d][0] == 0xffff ) return ERROR;
					G2_x += G2_data_l[d][0];
					dn++;
					if( dn >= 5 ) break;
				}
				if( dn == 0 ) return ERROR;
				le_up[2] = G2_x / dn + offset_xs;		//xs
				// ys
				le_up[1] = Corner_pos[1] + offset_uy;
				// ye
				le_up[3] = le_up[1] + size_y;

			//左下 area
				// xe
				le_lo[0] = Corner_pos[2] - offset_lx;
				// xs
				G2_x = dn = 0;
				for( d=0; d<80; d++ ) {
					if( G2_data_l[d][0] == 0xffff ) break;
				}
				d-=3;
				for( ; d>0; d-- ) {
					G2_x += G2_data_l[d][0];
					dn++;
					if( dn >= 5 ) break;
				}
				if( dn == 0 ) return ERROR;
				le_lo[2] = G2_x / dn + offset_xs;		//xs
				// ys
				le_lo[3] = Corner_pos[3] - offset_ly;
				// ye
				le_lo[1] = le_lo[3] - size_y;	//ye

			//右上 area
				// xe
				ri_up[2] = Corner_pos[4] + offset_rx;
				// xs
				G2_x = dn = 0;
				for( d=2; d<80; d++ ) {
					if( G2_data_r[d][0] == 0xffff ) return ERROR;
					G2_x += G2_data_r[d][0];
					dn++;
					if( dn >= 5 ) break;
				}
				if( dn == 0 ) return ERROR;
				ri_up[0] = G2_x / dn - offset_xs;		//xs
				// ys
				ri_up[1] = Corner_pos[5] + offset_uy;
				// ye
				ri_up[3] = ri_up[1] + size_y;

			//右下 area
				// xe
				ri_lo[2] = Corner_pos[6] + offset_rx;
				// xs
				G2_x = dn = 0;
				for( d=0; d<80; d++ ) {
					if( G2_data_r[d][0] == 0xffff ) break;
				}
				d-=3;
				for( ; d>0; d-- ) {
					G2_x += G2_data_r[d][0];
					dn++;
					if( dn >= 5 ) break;
				}
				if( dn == 0 ) return ERROR;
				ri_lo[0] = G2_x / dn - offset_xs;		//xs
				// ys
				ri_lo[3] = Corner_pos[7] - offset_ly;
				// ye
				ri_lo[1] = ri_lo[3] - size_y;

			if( i_mode & INSPECT_STEP ) {
				R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
				R_move_bin_memory( bin_pn, BinP0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
				R_box( le_up[0], le_up[1], le_up[2], le_up[3], GRAPH_REVERSE, SOLID_LINE );
				R_box( le_lo[0], le_lo[1], le_lo[2], le_lo[3], GRAPH_REVERSE, SOLID_LINE );
				R_box( ri_up[0], ri_up[1], ri_up[2], ri_up[3], GRAPH_REVERSE, SOLID_LINE );
				R_box( ri_lo[0], ri_lo[1], ri_lo[2], ri_lo[3], GRAPH_REVERSE, SOLID_LINE );
				//R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
				WaitMouseClick();
			}
		//検査範囲設定 end

		// Insp.
			// 左上 insp.
				ys = le_up[1];
				ye = le_up[3];
				xs = le_up[2];
				c2l = C2fail = C2fail_max = n = 0;
				for( ry=ys+4; ry<=ye-4; ry++,n++ ) {						// nok
					rx = xs;
					xe = le_up[0];
					rl = xs-xe+1;
					R_es_set_degree( 180 );			//	認識方向をleft 180°方向に設定する。
					R_es_set_length( rl );			//	サーチ距離をrlとする。
					R_es_set_noise( 2 );			//	5画素以下の黒（又は白）は無視する。
					R_es_set_start( rx, ry );		//	認識開始点の設定
					rc = ES_BLACK;					//  black -> white
					if( R_es_bin_search( &rx, &ry, &rc ) != OK ) rx=xe;
			rx-=6; //フィルタでシフトする分を補正
						C2_data_lu[n][0] = rx;
						C2_data_lu[n][1] = ry;
						countlu=n;
				}
                for(n=0; n<1000; n++){
					FreqData[n] = 0;
				}
				xs = le_up[0];
				xe = le_up[2];
				for(xs=le_up[0]; xs<xe; xs++){
                    for(n=0; n<=countlu; n++){
						rx_leup=C2_data_lu[n][0];
						if(abs(xs-rx_leup+1)<4)	FreqData[xs]++;
					}
				}
				
				for(xs=le_up[0];xs<xe;xs++){
					FreqData_leup_max=max(FreqData[xs],FreqData_leup_max);		
				}
				for(xs=le_up[0];xs<xe;xs++){
					if(FreqData[xs] == FreqData_leup_max) xs_leup = xs;
				}
				for(m=0; m<200; m++){
					xxs_leup[m] = 0;
				}
				m=0;
				for(n=0;n<countlu;n++){
					if(abs(xs_leup - C2_data_lu[n][0])<=4) {
						xxs_leup[m] = C2_data_lu[n][0];
						ys_leup[m] = C2_data_lu[n][1];
						m++;
					
					}else{
						continue;
					}
					countylu=m;
				}
                if(countylu<=0){        // 2017.04.09 tanaka
                    C2fail_max = judge_c2n+1;
                } else {
                    C2_le_up_size = xs_leup -le_up[0];
                    for(m=0;m<=countylu-1;m++){
                        rx=xxs_leup[m];
                        ry=ys_leup[m];
                        xe =le_up[0];
                        c2l= C2_le_up_size;

                        if( c2l < InspPara.ElecC2Xmin[TWA_Spec].nXsize ) {
                            C2fail++;
                            if( i_mode & INSPECT_STEP ) {
                                R_line( xe-1, ry, rx, ry, GRAPH_REVERSE, SOLID_LINE );
                            }
                        } else {
                            C2fail = 0;
                            if( i_mode & INSPECT_STEP ) {
                                R_pset( rx, ry, GRAPH_REVERSE );
                                R_pset( xe, ry, GRAPH_REVERSE );
                            }
                        }
                        C2fail_max = max( C2fail, C2fail_max );
                    }
                }
				if( i_mode & INSPECT_STEP ) {
					sprintf( Comment, "fail:%3d", C2fail_max );
					cx = 0;		cy = le_up[1]/16-3;	if(cy < 0) cy=0;
					R_chrdisp( cx, cy, Comment );
				}
				if( C2fail_max > judge_c2n )	judge_lu = ERROR;
				else							judge_lu = OK;

			//左下 insp.
				ys = le_lo[1];
				ye = le_lo[3];
				xs = le_lo[2];
				c2l = C2fail = C2fail_max = n= 0;
				for( ry=ys+4; ry<=ye-4; ry++ ,n++) {
					rx = xs;
					xe = le_lo[0];
					rl = xs-xe+1;
					R_es_set_degree( 180 );			//	認識方向をleft 180°方向に設定する。
					R_es_set_length( rl );			//	サーチ距離をrlとする。
					R_es_set_noise( 2 );			//	5画素以下の黒（又は白）は無視する。
					R_es_set_start( rx, ry );		//	認識開始点の設定
					rc = ES_BLACK;					//  black -> white
					if( R_es_bin_search( &rx, &ry, &rc ) != OK ) rx=xe;
					rx-=6; //フィルタでシフトする分を補正
						C2_data_lo[n][0] = rx;
						C2_data_lo[n][1] = ry;
						countlo=n;
				}
				for(n=0; n<1000; n++){
					FreqData[n] = 0;
				}
				xs = le_lo[0];
				xe = le_lo[2];
				for(xs=le_lo[0]; xs<xe; xs++){
					for(n=0; n<=countlu; n++){
						rx_lelo=C2_data_lo[n][0];
						if(abs(xs-rx_lelo+1)<4)	FreqData[xs]++;
					}
				}

				for(xs=le_lo[0];xs<xe;xs++){
					FreqData_lelo_max=max(FreqData[xs],FreqData_lelo_max);
				}
				for(xs=le_lo[0];xs<xe;xs++){
					if(FreqData[xs] == FreqData_lelo_max) xs_lelo = xs;
				}
				for(m=0; m<200; m++){
					xxs_lelo[m] = 0;
				}
				m=0;
				for(n=0;n<countlo;n++){
					if(abs(xs_lelo - C2_data_lo[n][0])<=4) {
						xxs_lelo[m] = C2_data_lo[n][0];
						ys_lelo[m] = C2_data_lo[n][1];
						m++;

					}else{
						continue;
					}
					countylo=m;
				}
                if(countylo<=0){        // 2017.04.09 tanaka
                    C2fail_max = judge_c2n+1;
                } else {
                    C2_le_lo_size = xs_lelo-le_lo[0];
                    for(m=0;m<=countylo-1;m++){
                        rx=xxs_lelo[m];
                        ry=ys_lelo[m];
                        xe =le_lo[0];
                        c2l= C2_le_lo_size;

                        if( c2l <  InspPara.ElecC2Xmin[TWA_Spec].nXsize ) {
                            C2fail++;
                            if( i_mode & INSPECT_STEP ) {
                                R_line( xe-1, ry, rx, ry, GRAPH_REVERSE, SOLID_LINE );
                            }
                        } else {
                            C2fail = 0;
                            if( i_mode & INSPECT_STEP ) {
                                R_pset( rx, ry, GRAPH_REVERSE );
                                R_pset( xe, ry, GRAPH_REVERSE );
                            }
                        }
                        C2fail_max = max( C2fail, C2fail_max );
                    }
                }
				if( i_mode & INSPECT_STEP ) {
					sprintf( Comment, "fail:%3d", C2fail_max );
					cx = 0;		cy = le_lo[1]/16+3;
					R_chrdisp( cx, cy, Comment );
				}
				if( C2fail_max > judge_c2n )	judge_ll = ERROR;
				else							judge_ll = OK;

			//右上
				ys = ri_up[1];
				ye = ri_up[3];
				xs = ri_up[0];
				c2l = C2fail = C2fail_max = n = 0;
				for( ry=ys+4; ry<=ye-4; ry++ ,n++) {
					rx = xs;
					xe = ri_up[2];
					rl = xe-xs+1;
					R_es_set_degree( 0 );			//	認識方向をright 0°方向に設定する。
					R_es_set_length( rl );			//	サーチ距離をrlとする。
					R_es_set_noise( 2 );			//	5画素以下の黒（又は白）は無視する。
					R_es_set_start( rx, ry );		//	認識開始点の設定
					rc = ES_BLACK;					//  black -> white
					if( R_es_bin_search( &rx, &ry, &rc ) != OK ) rx=xe;
					rx+=6; //フィルタでシフトする分を補正
						C2_data_ru[n][0] = rx;
						C2_data_ru[n][1] = ry;
						countru=n;				
					}
				for(n=0; n<1000; n++){
					FreqData[n] = 0;
				}
				xs = ri_up[2];
				xe = ri_up[0];
				for(xs=ri_up[2]; xs>ri_up[0]; xs--){
					for(n=0; n<=countru; n++){
						rx_riup=C2_data_ru[n][0];
						if(abs(xs-rx_riup+1)<4)	FreqData[xs]++;
					}
				}
				for(xs=ri_up[2];xs>ri_up[0];xs--){
					FreqData_riup_max=max(FreqData[xs],FreqData_riup_max);
				}
				for(xs=ri_up[2];xs>ri_up[0];xs--){
					if(FreqData[xs] == FreqData_riup_max) xs_riup = xs;
				}
				for(m=0; m<200; m++){
					xxs_riup[m] = 0;
				}
				m=0;
				for(n=0;n<countru;n++){
					if(abs(xs_riup - C2_data_ru[n][0])<=4) {
						xxs_riup[m] = C2_data_ru[n][0];
						ys_riup[m] = C2_data_ru[n][1];
						m++;
					
					}else{
						continue;
					}
					countyru=m;
				}
                if(countyru<=0){        // 2017.04.09 tanaka
                    C2fail_max = judge_c2n+1;
                } else {
                    C2_ri_up_size = ri_up[2]- xs_riup;
                    for(m=0;m<=countyru-1;m++){
                        rx=xxs_riup[m];
                        ry=ys_riup[m];
                        xe =ri_up[2];
                        c2l= C2_ri_up_size;
		
                        if( c2l <  InspPara.ElecC2Xmin[TWA_Spec].nXsize ) {
                            C2fail++;
                            if( i_mode & INSPECT_STEP ) {
                                R_line( rx, ry, xe, ry, GRAPH_REVERSE, SOLID_LINE );
                            }
                        } else {
                            C2fail = 0;
                            if( i_mode & INSPECT_STEP ) {
                                R_pset( rx, ry, GRAPH_REVERSE );
                                R_pset( xe, ry, GRAPH_REVERSE );
                            }
                        }
                        C2fail_max = max( C2fail, C2fail_max );
                    }
                }
				if( i_mode & INSPECT_STEP ) {
					sprintf( Comment, "fail:%3d", C2fail_max );
					cx = 70;		cy = ri_up[1]/16-3;	if(cy < 0) cy=0;
					R_chrdisp( cx-1, cy, Comment );
				}
				if( C2fail_max > judge_c2n )	judge_ru = ERROR;
				else							judge_ru = OK;

			//右下
				ys = ri_lo[1];
				ye = ri_lo[3];
				xs = ri_lo[0];
				c2l = C2fail = C2fail_max = n = 0;
				for( ry=ys+4; ry<=ye-4; ry++,n++ ) {
					rx = xs;
					xe = ri_lo[2];
					rl = xe-xs+1;
					//if( TWA_es_bin_search_n( bin_pn, &rx, &ry, 0, rl, TWA_ES_WHITE,5 ) != OK )	rx = xe;
					R_es_set_degree( 0 );			//	認識方向をright 0°方向に設定する。
					R_es_set_length( rl );			//	サーチ距離をrlとする。
					R_es_set_noise( 2 );			//	5画素以下の黒（又は白）は無視する。
					R_es_set_start( rx, ry );		//	認識開始点の設定
					rc = ES_BLACK;					//  black -> white
					if( R_es_bin_search( &rx, &ry, &rc ) != OK ) rx=xe;
					rx+=6; //フィルタでシフトする分を補正
						C2_data_rl[n][0] = rx;
						C2_data_rl[n][1] = ry;
						countrl=n;				
					}
				for(n=0; n<1000; n++){
					FreqData[n] = 0;
				}
				xs = ri_lo[2];
				xe = ri_lo[0];
				for(xs=ri_lo[2]; xs>ri_lo[0]; xs--){
					for(n=0; n<=countrl; n++){
						rx_rilo=C2_data_rl[n][0];
						if(abs(xs-rx_rilo+1)<4)	FreqData[xs]++;
					}
				}
				for(xs=ri_lo[2];xs>ri_lo[0];xs--){
					FreqData_rilo_max=max(FreqData[xs],FreqData_rilo_max);
				}
				for(xs=ri_lo[2];xs>ri_lo[0];xs--){
					if(FreqData[xs] == FreqData_rilo_max) xs_rilo = xs;
				}
				for(m=0; m<200; m++){
					xxs_rilo[m] = 0;
				}
				m=0;
				for(n=0;n<countrl;n++){
					if(abs(xs_rilo - C2_data_rl[n][0])<=4) {
						xxs_rilo[m] = C2_data_rl[n][0];
						ys_rilo[m] = C2_data_rl[n][1];
						
						m++;
					
					}else{
						continue;
					}
					countyrl=m;
				}
                if(countyrl<=0){        // 2017.04.09 tanaka
                    C2fail_max = judge_c2n+1;
                } else {
                    C2_ri_lo_size = ri_lo[2]- xs_rilo;
                    for(m=0;m<=countyrl-1;m++){
                        rx=xxs_rilo[m];
                        ry=ys_rilo[m];
                        xe =ri_lo[2];
                        c2l= C2_ri_lo_size;

                        if( c2l <  InspPara.ElecC2Xmin[TWA_Spec].nXsize ) {
                            C2fail++;
                            if( i_mode & INSPECT_STEP ) {
                                R_line( rx, ry, xe, ry, GRAPH_REVERSE, SOLID_LINE );
                            }
                        } else {
                            C2fail = 0;
                            if( i_mode & INSPECT_STEP ) {
                                R_pset( rx, ry, GRAPH_REVERSE );
                                R_pset( xe, ry, GRAPH_REVERSE );
                            }
                        }
                        C2fail_max = max( C2fail, C2fail_max );
                    }
                }

				if( i_mode & INSPECT_STEP ) {
					sprintf( Comment, "fail:%3d", C2fail_max );
					cx = 70;		cy = ri_lo[1]/16+3;
					R_chrdisp( cx-1, cy, Comment );
				}
				if( C2fail_max > judge_c2n )	judge_rl = ERROR;
				else							judge_rl = OK;
			
				if( i_mode & INSPECT_STEP ) {
					R_DrawPrintf(judge_lu != OK? DRAW_RED:DRAW_WHITE , DISP_NORMAL, 0, 31, "C2_le_up_size = %5d\n ",C2_le_up_size );
					R_DrawPrintf(judge_ll != OK? DRAW_RED:DRAW_WHITE , DISP_NORMAL, 0, 32, "C2_le_lo_size = %5d\n ",C2_le_lo_size );
					R_DrawPrintf(judge_ru != OK? DRAW_RED:DRAW_WHITE , DISP_NORMAL, 0, 33, "C2_ri_up_size = %5d\n ",C2_ri_up_size );
					R_DrawPrintf(judge_rl != OK? DRAW_RED:DRAW_WHITE , DISP_NORMAL, 0, 34, "C2_ri_lo_size = %5d\n ",C2_ri_lo_size );
					WaitMouseClick();
					R_DrawCls();
				}
			//	 TWA_Judge_G2_furyo();   
			// Judge C2 l width
				if(( judge_lu != OK )||( judge_ll != OK )) return ERROR;
				if(( judge_ru != OK )||( judge_rl != OK )) return ERROR;

		return OK;

	}	// TWA_JudgeC2SizeX( int bin_pn ) end

/*******************************************************************************
	関数名：	ＴＷＡ＿ｇｒａｙ＿ｆｉｌｌ
	機　能：	グレー画面の四角形領域の塗り潰し
	形　式：	#include "R_tool.h"
				void TWA_gray_fill( int x1, int y1, int x2, int y2, int lebel, int p );
	引き数：	１）ｘ１は、四角形領域の頂点１のｘ座標です。
				２）ｙ１は、四角形領域の頂点１のｙ座標です。
				３）ｘ２は、四角形領域の頂点２のｘ座標です。
				４）ｙ２は、四角形領域の頂点２のｙ座標です。
	戻り値：	ありません。
	解　説：	頂点１と頂点２を対角とした四角形領域を指定階調で塗り潰します。
	留意事項：	ｘ１＜ｘ２、ｙ１＜ｙ２としてください。
*******************************************************************************/

void	TWA_gray_fill( int x1, int y1, int x2, int y2, int level, int p )
	{
		int					i, j, xsm, xs, xem, xe;
		unsigned int		l_data = level*0x01010101;

		xsm = x1 & 0x03;
		xs = ((x1+3)/4)*4;
		xem = x2 & 0x03;
		xe = ((x2-3)/4)*4;

		if( xsm ) {
			for( i=y1; i<=y2; i++ ) {
				for( j=xsm; j<4; j++ ) *( ( char * )( Gray_address[p]+i*MxSize+xs-4+j ) ) = level;
			}
		}
		if( xem != 3 ) {
			for( i=y1; i<=y2; i++ ) {
				for( j=0; j<xem+1; j++ ) *( ( char * )( Gray_address[p]+i*MxSize+xe+4+j ) ) = level;
			}
		}
		for( i=y1; i<=y2; i++ ) {
			for( j=xs; j<=xe; j+=4 ) *( ( int * )( Gray_address[p]+i*MxSize+j ) ) = l_data;
		}

	}	// TWA_gray_fill( int x1, int y1, int x2, int y2, int level, int p ) end
/************************************************
	TWA_Insp_white_or
************************************************/

void	TWA_Insp_white_or( int src_mem_no, int dst_mem_no, int xs, int ys, int xe, int ye, int l_hi )
	{
		int						rx, ry;
		int						level, set_level;
		unsigned char			*ad_src, *ad_dst;

		set_level = Bin_C2_Loffset + 10;
		if( set_level < 0 )		set_level = 0;
		if( set_level > 255 )	set_level = 255;

		// Gray画像補正
			for( ry=ys; ry<=ye; ry++ ) {
				rx = xs;
				ad_src = (unsigned char *)(Gray_address[src_mem_no]+ry*MxSize+rx);
				ad_dst = (unsigned char *)(Gray_address[dst_mem_no]+ry*MxSize+rx);
				for( ; rx<=xe; rx++ ) {
					level = *ad_src;
					if( level >= l_hi )  *ad_dst = set_level;
					ad_src++;
					ad_dst++;
				}
			}

	}	// TWA_Insp_white_or( int src_mem_no, int dst_mem_no, int xs, int ys, int xe, int ye, int l_hi ) end

/************************************************
	平滑化
************************************************/

int		TWA_smoothing( int src_mem_no, int dst_mem_no, int pt[] )
	{
		int						ans, n;
		int						xs, xe, xl, rx, rxs, rxe, dx, dxs, dxe;
		int						ys, ye, yl, ry, rys, rye, dy, dys, dye;
		int						filter_x_size;
		int						filter_y_size;
		int						filter_n;
		int						bright_r;
		int						Bright_d[640];

		union R_POINTER			sp, dp;

		// initial
			filter_x_size = pt[0];
			filter_y_size = pt[1];
			xs = pt[2];
			ys = pt[3];
			xe = pt[4];
			ye = pt[5];
			xl = xe - xs + 1;
			yl = ye - ys + 1;
			filter_n = filter_x_size * filter_y_size;

		// 処理メモリー確認
			if( src_mem_no == dst_mem_no ) return ERROR;
			if( xs > xe ) return ERROR;
			if( ys > ye ) return ERROR;
			if( xe >= FX_SIZE ) xe = FX_SIZE-1;
			if( ye >= FY_SIZE ) ye = FY_SIZE-1;

		// フィルタサイズ確認
				 if( filter_x_size >= xl ) ans = ERROR2;
			else if( filter_y_size >= yl ) ans = ERROR2;
			else if( filter_x_size == 0 )  ans = ERROR2;
			else if( filter_y_size == 0 )  ans = ERROR2;
			else						   ans = OK;
			if( ans != OK ) {
				R_move_gray_memory( src_mem_no, dst_mem_no, xs, ys, xe, ye );
				return ans;
			}

		// address
			sp.b = Gray_address[src_mem_no]+xs+MxSize*ys;
			dp.b = Gray_address[dst_mem_no]+xs+MxSize*ys;

			dxs = xs + filter_x_size/2;
			dxe = xe - filter_x_size/2;
			dys = ys + filter_y_size/2;
			dye = ye - filter_y_size/2;

		// 周囲をクリア
			// up
			for( ry=ys; ry<dys; ry++ ) {
				rx = xs;
				dp.b = Gray_address[dst_mem_no]+rx+MxSize*ry;
				for( ; rx<=xe; rx++ ) {
					*dp.b = 0;
					dp.b++;
				}
			}
			// doun
			for( ry=dye+1; ry<=ye; ry++ ) {
				rx = xs;
				dp.b = Gray_address[dst_mem_no]+rx+MxSize*ry;
				for( ; rx<=xe; rx++ ) {
					*dp.b = 0;
					dp.b++;
				}
			}
			// Left
			for( rx=xs; rx<dxs; rx++ ) {
				ry = ys;
				dp.b = Gray_address[dst_mem_no]+rx+MxSize*ry;
				for( ry=ys; ry<=ye; ry++ ) {
					*dp.b = 0;
					dp.b += MxSize;
				}
			}
			// Right
			for( rx=dxe+1; rx<=xe; rx++ ) {
				ry = ys;
				dp.b = Gray_address[dst_mem_no]+rx+MxSize*ry;
				for( ry=ys; ry<=ye; ry++ ) {
					*dp.b = 0;
					dp.b += MxSize;
				}
			}

		// filter
			for( n=0; n<MxSize; n++ ) Bright_d[n] = 0;

			for( dy=dys; dy<=dye; dy++ ) {
				if( filter_y_size == 1 ) {
					ry = dy;
					rx = xs;
					sp.b = Gray_address[src_mem_no]+rx+MxSize*ry;
					for( ; rx<=xe; rx++ ) {
						Bright_d[rx] = *sp.b;
						sp.b++;
					}
				} else {
					rys = dy - filter_y_size/2;
					rye = rys + filter_y_size;
					if( dy == dys ) {
						for( ry=rys; ry<rye; ry++ ) {
							rx = xs;
							sp.b = Gray_address[src_mem_no]+rx+MxSize*ry;
							for( ; rx<=xe; rx++ ) {
								Bright_d[rx] += *sp.b;
								sp.b++;
							}
						}
					} else {
						rx = xs;
						ry = rys - 1;
						sp.b = Gray_address[src_mem_no]+rx+MxSize*ry;
						for( ; rx<=xe; rx++ ) {
							Bright_d[rx] -= *sp.b;
							sp.b++;
						}
						rx = xs;
						ry = rye - 1;
						sp.b = Gray_address[src_mem_no]+rx+MxSize*ry;
						for( ; rx<=xe; rx++ ) {
							Bright_d[rx] += *sp.b;
							sp.b++;
						}
					}
				}

				dx = dxs;
				rxs = dx - filter_x_size/2;
				rxe = rxs + filter_x_size;
				bright_r = 0;
				for( rx=rxs; rx<rxe; rx++ ) {
					bright_r += Bright_d[rx];
				}

				dp.b = Gray_address[dst_mem_no]+dx+MxSize*dy;
				*dp.b = bright_r / filter_n;
				while( dx <= dxe ) {
					bright_r -= Bright_d[rxs];
					bright_r += Bright_d[rxe];
					rxs++;
					rxe++;
					dp.b++;
					*dp.b = bright_r / filter_n;
					dx++;
				}
			}

		return OK;

	}	// TWA_smoothing( int src_mem_no, int dst_mem_no, int pt[] ) end



/************************************************
	Surf Board Filter
************************************************/

void	TWA_surf_board( int src_mem_no1, int src_mem_no2, int dst_mem_no, int pt[], int level_min )
	{
		int						i1, i2;
		int						xs, xe, rx;
		int						ys, ye, ry;
		int						bright_r;
		int						bright_r1, bright_r2, bright_r3;
		int						coefficient;
		int						mode;

		union R_POINTER			sp1, sp2, dp;

		// initial
			xs = pt[0];
			ys = pt[1];
			xe = pt[2];
			ye = pt[3];
			coefficient = pt[4];
			mode = pt[5];

		// filter
			for( ry=ys; ry<=ye; ry++ ) {
				bright_r1 = bright_r2 = bright_r3 = 0;
				rx = xs;
				sp1.b = Gray_address[src_mem_no1]+rx+MxSize*ry;
				sp2.b = Gray_address[src_mem_no2]+rx+MxSize*ry;
				dp.b  = Gray_address[dst_mem_no] +rx+MxSize*ry;
				for( ; rx<=xe; rx++ ) {
					i1 = *sp1.b;
					i2 = *sp2.b;
					if(( i1 > level_min )&&( i2 > level_min )) {
						bright_r = abs( *sp2.b - *sp1.b );
//						bright_r = *sp2.b - *sp1.b;
						if( bright_r < 0 ) bright_r = 0;
						bright_r *= coefficient;
					} else {
						bright_r = 0;
					}
					if( bright_r > 256 ) bright_r = 255;

					bright_r1 = bright_r2;
					bright_r2 = bright_r3;
					bright_r3 = bright_r;
					if( mode == 1 ) {
						bright_r = max( bright_r1, bright_r2 );
						bright_r = max( bright_r,  bright_r3 );
					}
					*dp.b = bright_r;
					sp1.b++;
					sp2.b++;
					dp.b++;
				}
			}

	}	// TWA_surf_board( int src_mem_no1, int src_mem_no2, int dst_mem_no, int pt[] )




/*********************************************************************************

	TWA_InspC2Xsize( void )

*********************************************************************************/

int		TWA_InspC2Xsize( void )
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

        //
		if(bIsESR25 || bIsMCR10 || bIsMCR18 || bIsSFR25){
            TWA_CoefficientC2 = 8;
            TWA_SurfLoC2      = 80;
        }

        // 外形ポジション代入
		Ap_pos[2]=Elec[DataLR].CornerPosY[0];	//Left Upper
		Ap_pos[3]=Elec[DataLR].CornerPosY[1];	//Left Lower
		Ap_pos[4]=Elec[DataLR].CornerPosY[2];	//Right Upper
		Ap_pos[5]=Elec[DataLR].CornerPosY[3];	//Right Lower
		// initial
			TWA_bin_cls( binPn, 0, 0, FX_SIZE-1, FY_SIZE-1 );
			TWA_gray_fill( 0, 0, FX_SIZE-1, FY_SIZE-1, 0, BASE3_GRAYADDRESS );

		// 平滑化
			// left
				le_xs = Ap_pos[0] - 20;			if( le_xs < xl_limit ) le_xs = xl_limit;
				le_xe = G2_pos[0] + 20;			if( le_xe > xu_limit ) le_xe = xu_limit;
				le_ys = Ap_pos[2] - 20;			if( le_ys < yl_limit ) le_ys = yl_limit;
				le_ye = Ap_pos[3] + 20;			if( le_ye > yu_limit ) le_ye = yu_limit;
				pt[0] = TWA_SmoothingXsizeC2;	pt[1] = TWA_SmoothingYsizeC2;
				pt[2] = le_xs;					pt[3] = le_ys;
				pt[4] = le_xe;					pt[5] = le_ye;
			//	TWA_smoothing( GrayP2, GrayP3, pt );
				TWA_smoothing( grayPn, BASE3_GRAYADDRESS, pt );
			// right
				ri_xs = G2_pos[1] - 20;			if( ri_xs < xl_limit ) ri_xs = xl_limit;
				ri_xe = Ap_pos[1] + 20;			if( ri_xe > xu_limit ) ri_xe = xu_limit;
				ri_ys = Ap_pos[4] - 20;			if( ri_ys < yl_limit ) ri_ys = yl_limit;
				ri_ye = Ap_pos[5] + 20;			if( ri_ye > yu_limit ) ri_ye = yu_limit;
				pt[0] = TWA_SmoothingXsizeC2;	pt[1] = TWA_SmoothingYsizeC2;
				pt[2] = ri_xs;					pt[3] = ri_ys;
				pt[4] = ri_xe;					pt[5] = ri_ye;
			//	TWA_smoothing( GrayP2, GrayP3, pt );
				TWA_smoothing( grayPn, BASE3_GRAYADDRESS, pt );
			// view
				if( i_mode & INSPECT_STEP ) {
				//if( TWA_test != 0 ) {
					R_gray_fill( 0, 0, FX_SIZE-1, FY_SIZE-1, 0 );
					TWA_bin_cls( 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
				//	R_move_gray_memory( GrayP3, GrayP0, 0, 0, TWA_X_size-1, TWA_Y_size-1 );
					R_move_gray_memory( BASE3_GRAYADDRESS, GrayP0, 0, 0, TWA_X_size-1, TWA_Y_size-1 );
					WaitMouseClick();
				}

		// サーフボードフィルター
			// left
				pt[0] = le_xs;							pt[1] = le_ys;
				pt[2] = le_xe;							pt[3] = le_ye;
				pt[4] = TWA_CoefficientC2;								// level up 倍率
				pt[5] = 0;	// 1;										// 均最大値 : 1
				//TWA_surf_board( GrayP2, GrayP3, GrayP3, pt, TWA_SurfLoC2 );
				TWA_surf_board( grayPn, BASE3_GRAYADDRESS, BASE3_GRAYADDRESS, pt, TWA_SurfLoC2 );
			// right
				pt[0] = ri_xs;							pt[1] = ri_ys;
				pt[2] = ri_xe;							pt[3] = ri_ye;
				pt[4] = TWA_CoefficientC2;								// level up 倍率
				pt[5] = 0;	// 1;										// 均最大値 : 1
				//TWA_surf_board( GrayP2, GrayP3, GrayP3, pt, TWA_SurfLoC2 );
				TWA_surf_board( grayPn, BASE3_GRAYADDRESS, BASE3_GRAYADDRESS, pt, TWA_SurfLoC2 );
			// view
				if( i_mode & INSPECT_STEP ) {
				//if( TWA_test != 0 ) {
					R_gray_fill( 0, 0, FX_SIZE-1, FY_SIZE-1, 0 );
					TWA_bin_cls( 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
				//	R_move_gray_memory( GrayP3, GrayP0, 0, 0, TWA_X_size-1, TWA_Y_size-1 );
					R_move_gray_memory( BASE3_GRAYADDRESS, GrayP0, 0, 0, TWA_X_size-1, TWA_Y_size-1 );
					WaitMouseClick();
				}

		// White or
			// left
				//TWA_Insp_white_or( GrayP2, GrayP3, le_xs, le_ys, le_xe, le_ye, C1_level );
				TWA_Insp_white_or( grayPn, BASE3_GRAYADDRESS, le_xs, le_ys, le_xe, le_ye, C1_level );
			// right
				//TWA_Insp_white_or( GrayP2, GrayP3, ri_xs, ri_ys, ri_xe, ri_ye, C1_level );
				TWA_Insp_white_or( grayPn, BASE3_GRAYADDRESS, ri_xs, ri_ys, ri_xe, ri_ye, C1_level );
			// view
				if( i_mode & INSPECT_STEP ) {
				//if( TWA_test != 0 ) {
					R_gray_fill( 0, 0, FX_SIZE-1, FY_SIZE-1, 0 );
					TWA_bin_cls( 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					//R_move_gray_memory( GrayP3, GrayP0, 0, 0, TWA_X_size-1, TWA_Y_size-1 );
					R_move_gray_memory( BASE3_GRAYADDRESS, GrayP0, 0, 0, TWA_X_size-1, TWA_Y_size-1 );
					WaitMouseClick();
				}

		// Gray to Bin
			// left
				le_xl = le_xe-le_xs+1;		le_yl = le_ye-le_ys+1;
				level = Bin_C2XsizeLevel;
				//R_gray2bin( level, le_xs, le_ys, le_xl, le_yl, Gray_address[GrayP3], Bin_address[BinP2] );
				R_gray2bin( level, le_xs, le_ys, le_xl, le_yl, Gray_address[BASE3_GRAYADDRESS], Bin_address[binPn] );
			// right
				ri_xl = ri_xe-ri_xs+1;		ri_yl = ri_ye-ri_ys+1;
				level = Bin_C2XsizeLevel;
				//R_gray2bin( level, ri_xs, ri_ys, ri_xl, ri_yl, Gray_address[GrayP3], Bin_address[BinP2] );
				R_gray2bin( level, ri_xs, ri_ys, ri_xl, ri_yl, Gray_address[BASE3_GRAYADDRESS], Bin_address[binPn] );
			// view
				if( i_mode & INSPECT_STEP ) {
				//if( TWA_test != 0 ) {
					R_display_control( BIN_MEMORY | GRAY_MEMORY );
					R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
					//R_move_gray_memory( GrayP2, 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					//R_move_bin_memory(  BinP2, 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					R_move_gray_memory( grayPn, 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					R_move_bin_memory(  binPn, 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
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

				TWA_contrct2( binPn, 1, 5, pt );                      //GEE 1 to 3
				TWA_expand2(  binPn, 1, 3, pt );                      //GEE 1 to 3
				TWA_expand2(  binPn, 1, 3, pt );                      //GEE 1 to 3
				TWA_contrct2( binPn, 1, 5, pt );                      //GEE 1 to 3

			// right
				pt[0] = ri_xs;					pt[1] = ri_ys;
				pt[2] = ri_xe;					pt[3] = ri_ye;
				TWA_expand2(  binPn, Pl_expand_x,  Pl_expand_y,  pt );
				TWA_contrct2( binPn, Pl_contrct_x, Pl_contrct_y, pt );
				TWA_contrct2( binPn, Pl_contrct_x, Pl_contrct_y, pt );
				TWA_expand2(  binPn, Pl_expand_x,  Pl_expand_y,  pt );

				TWA_contrct2( binPn, 1, 5, pt );                      //GEE 1 to 3
				TWA_expand2(  binPn, 1, 3, pt );                      //GEE 1 to 3
				TWA_expand2(  binPn, 1, 3, pt );                      //GEE 1 to 3
				TWA_contrct2( binPn, 1, 5, pt );                      //GEE 1 to 3

			// view
				if( i_mode & INSPECT_STEP ) {
				//if( TWA_test != 0 ) {
					R_display_control( BIN_MEMORY | GRAY_MEMORY );
					R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
					R_move_gray_memory( grayPn, 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					R_move_bin_memory(  binPn, 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					WaitMouseClick();
				}

			// Judge C2 X size
				if( TWA_JudgeC2SizeX( binPn ) != OK ) return ERROR;

		return OK;

	}	// TWA_InspC2Xsize( void ) end

/************************************************
	C1 Mezumari Left <0001>
*************************************************/

int		TWA_Judge_Mzleft( void )
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

int		TWA_Judge_Mzright( void )
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

int		TWA_Judge_Mz( void )
	{

		//C1 Mezumari（Left）
			if( TWA_Judge_Mzleft() != OK )	return ERROR;
		//C1 Mezumari（Right）
			if( TWA_Judge_Mzright() != OK )	return ERROR;

		return OK;

	}	// TWA_Judge_Mz( void ) end


/************************************************
	C1 Mezumari Filtering <z1>
************************************************/

int		TWA_Mezu_Area( void )
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
/*
				TWA_expand2(  binPn, 1, Pl_expand_y,  pt );
				TWA_contrct2( binPn, 1, Pl_contrct_y, pt );
				TWA_contrct2( binPn, 1, Pl_contrct_y, pt );
				TWA_expand2(  binPn, 1, Pl_expand_y,  pt );
*/
			// right
				pt[0] = ri_xs;					pt[1] = ri_ys;
				pt[2] = ri_xe;					pt[3] = ri_ye;
				TWA_expand2(  binPn, Pl_expand_x,  Pl_expand_y,  pt );
				TWA_contrct2( binPn, Pl_contrct_x, Pl_contrct_y, pt );
				TWA_contrct2( binPn, Pl_contrct_x, Pl_contrct_y, pt );
				TWA_expand2(  binPn, Pl_expand_x,  Pl_expand_y,  pt );
/*
				TWA_expand2(  binPn, 1, Pl_expand_y,  pt );
				TWA_contrct2( binPn, 1, Pl_contrct_y, pt );
				TWA_contrct2( binPn, 1, Pl_contrct_y, pt );
				TWA_expand2(  binPn, 1, Pl_expand_y,  pt );
*/
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
	
//-------------------<z3>-------------------//

void MaskMarkingMCR03(int binPn)
{
	int xs,xe,ys,ye,ym,y;
	int dx,dy,s,ss;
	int	nc;
	int i,deg10;
	int bak_graphpage = Graph_page;
	int ofs=15;

	//標印傾き計算
		deg10 = TWA_katamuki();
		i = (300+deg10)/5;
		if((i<0)||(i>120)) return;

	if(TWA_R_n == 1){
		Graph_page = binPn;
		nc=0;
		xs = Mark_rx_J[0];
		ys = Mark_ry_J[0];
		xe = Mark_rx_J[Mark_div_x*(Mark_div_y-1)];
		ye = Mark_ry_J[Mark_div_x*(Mark_div_y-1)];
		dx = (xe-xs+1);
		dy = (ye-ys+1);
		if(dy == 0)	s=0;
		else s  = (dx*1000)/dy;
		xs-= dx/4+15;
		xe = Mark_rx_J[Mark_div_x-1]+dx/4+10;
		ym = Mark_ry_J[Mark_div_x-1]-dy/16;
		ys-= dy/16+15;
		ye+= dy/16+15;
		ss = 0;
		for(y=ys;y<ye;y++,ym++){
			R_line(xs+ss/1000+mox[nc],y+moy[nc],xe+ss/1000+mox[nc],ym+moy[nc],GRAPH_ERASE,SOLID_LINE);
			ss+=s;
		}

		Graph_page = bak_graphpage;
	} else {
		//R_DrawCls();
		Graph_page = binPn;
		for(nc=0;nc<TWA_R_n;nc++){
			xs = Mark_rx[i][nc][0];
			ys = Mark_ry[i][nc][0];
			xe = Mark_rx[i][nc][Mark_div_x*(Mark_div_y-1)];
			ye = Mark_ry[i][nc][Mark_div_x*(Mark_div_y-1)];
			dx = (xe-xs+1);
			dy = (ye-ys+1);
			if(dy == 0)	s=0;
			else s  = (dx*1000)/dy;
			xs-= dx/4+ofs;
			xe = Mark_rx[i][nc][Mark_div_x-1]+dx/4+10;

			ym = Mark_ry[i][nc][Mark_div_x-1];

			ys-= ofs;
			ym-= ofs;

			ye+= ofs;
			ss = 0;
			for(y=ys;y<ye;y++,ym++){
				R_line(xs+ss/1000+mox[nc],y+moy[nc],xe+ss/1000+mox[nc],ym+moy[nc],GRAPH_ERASE,SOLID_LINE);
				//R_DrawLine( DRAW_GREEN, xs+ss/1000+mox[nc],y+moy[nc],xe+ss/1000+mox[nc],ym+moy[nc], DRAW_NORMAL, SOLID_LINE );
				ss+=s;
			}
		}
		Graph_page = bak_graphpage;
	}
}
/************************************************
	Ｇ２ズレ検査（Ｗ方向）
************************************************/
int		TWA_kensa_zure_w( void )
	{
	int					x1, y1, y2, l[2];
	int					res;
	int					i_mode	= R_CS_GetInspectMode();

	//	if( TWA_Spec == SpecMCL ) return OK;

	//Ｇ２ズレ検査（上）
		y1 = Ap_ue_ey;
		y2 = ue.kaiki_ey;
		x1 = ue.kaiki_ex;
		if( Ap_ue_ey == 0 ) l[0] = 0;
		else l[0] = ue.kaiki_ey - Ap_ue_ey;
		if( l[0] < 0 ) l[0] = 0;

		if( i_mode & INSPECT_STEP ) {
			if( l[0]>0 ) {
				R_DrawLine( DRAW_GREEN, x1-2, y1, x1+2, y1, DRAW_REVERSE, SOLID_LINE );
				R_DrawLine( DRAW_GREEN, x1-2, y2, x1+2, y2, DRAW_REVERSE, SOLID_LINE );
				R_DrawLine( DRAW_GREEN, x1, y1, x1, y2, DRAW_REVERSE, SOLID_LINE );
			}
			res = DRAW_WHITE;
			if( l[0] < InspPara.G2ZureYmin[TWA_Spec].nYsize ) res = DRAW_RED;	//Judge_Zure_Wmin[3]
			if( l[0] > InspPara.G2ZureYmax[TWA_Spec].nYsize ) res = DRAW_RED;	//Judge_Zure_Wmax[3]

			if( R_get_language() == JPN_DISP )  
				R_DrawPrintf(res , DISP_NORMAL, 0, 30   ,"Ｇ２ズレ（上）=%3d bit,G2ZureLimit Ymin=%3d Ymax=%3d bit ", l[0], InspPara.G2ZureYmin[TWA_Spec].nYsize, InspPara.G2ZureYmax[TWA_Spec].nYsize );
			else								
				R_DrawPrintf(res , DISP_NORMAL, 0, 30   ,"G2 Zure(Upper）=%3d bit,G2ZureLimit Ymin=%3d Ymax=%3d bit ", l[0], InspPara.G2ZureYmin[TWA_Spec].nYsize, InspPara.G2ZureYmax[TWA_Spec].nYsize);
		}

	//Ｇ２ズレ検査（下）
		y1 = shita.kaiki_ey;
		y2 = Ap_shita_ey;
		x1 = shita.kaiki_ex;
		if( shita.kaiki_ey == 0 ) l[1] = 0;
		else l[1] = Ap_shita_ey - shita.kaiki_ey;
		if( l[1] < 0 ) l[1] = 0;

		if( i_mode & INSPECT_STEP ) {
			if( l[1]>0 ) {
				R_DrawLine( DRAW_GREEN, x1-2, y1, x1+2, y1, DRAW_REVERSE, SOLID_LINE );
				R_DrawLine( DRAW_GREEN, x1-2, y2, x1+2, y2, DRAW_REVERSE, SOLID_LINE );
				R_DrawLine( DRAW_GREEN, x1, y1, x1, y2, DRAW_REVERSE, SOLID_LINE );
			}
			res = DRAW_WHITE;
			if( l[1] < InspPara.G2ZureYmin[TWA_Spec].nYsize ) res = DRAW_RED;	//Judge_Zure_Wmin[3]
			if( l[1] > InspPara.G2ZureYmax[TWA_Spec].nYsize ) res = DRAW_RED;	//Judge_Zure_Wmax[3]
			if( R_get_language() == JPN_DISP )  
				R_DrawPrintf(res, DISP_NORMAL, 0, 31   , "Ｇ２ズレ（下）=%3d bit,G2ZureLimit Ymin=%3d Ymax=%3d bit ", l[1], InspPara.G2ZureYmin[TWA_Spec].nYsize, InspPara.G2ZureYmax[TWA_Spec].nYsize );
			else								
				R_DrawPrintf(res, DISP_NORMAL, 0, 31   , "G2 Zure(Lower) =%3d bit,G2ZureLimit Ymin=%3d Ymax=%3d bit ", l[1], InspPara.G2ZureYmin[TWA_Spec].nYsize, InspPara.G2ZureYmax[TWA_Spec].nYsize );
			R_mouse_de_yesno();
		}

		//printf("l=,%d,%d\n",l[0],l[1]);

		if( l[0] < InspPara.G2ZureYmin[TWA_Spec].nYsize ) return(ERROR);	//Judge_Zure_Wmin[3]
		if( l[0] > InspPara.G2ZureYmax[TWA_Spec].nYsize ) return(ERROR);	//Judge_Zure_Wmax[3]

		if( l[1] < InspPara.G2ZureYmin[TWA_Spec].nYsize ) return(ERROR);	//Judge_Zure_Wmin[3]
		if( l[1] > InspPara.G2ZureYmax[TWA_Spec].nYsize ) return(ERROR);	//Judge_Zure_Wmax[3]
		return(OK);
	}


/************************************************
	Ｇ２ズレ検査（Ｌ方向）
************************************************/

int		TWA_kensa_zure_l( void )
	{
		int			x1, x2;
		int			n;

//		x1 = Ap_data_l[0][0];
//		x2 = G2_data_l[0][0];
		x1 = ElecEdge[DataUL][PosXmin].MeanX;	// Ap_le.kaiki_ex;
		x2 = hidari.kaiki_ex;
//printf("le %d,",(x2-x1));
		if(( x2-x1 ) < InspPara.G2ZureXmin[TWA_Spec].nXsize ) return(ERROR);	//Judge_Zure_Lmin[2]

//		x1 = Ap_data_r[0][0];
//		x2 = G2_data_r[0][0];
		x1 = ElecEdge[DataUL][PosXmax].MeanX;	//Ap_ri.kaiki_ex;
		x2 = migi.kaiki_ex;
//printf("ri %d \n",(x1-x2));
		if(( x1-x2 ) < InspPara.G2ZureXmin[TWA_Spec].nXsize ) return(ERROR);	//Judge_Zure_Lmin[2]
		
/*
		for( n=0; n<64; n++ ) if( Ap_data_l[n][0] == 0xffff ) break;
		x1 = Ap_data_u[n-1][0];
		for( n=0; n<64; n++ ) if( G2_data_l[n][0] == 0xffff ) break;
		x2 = G2_data_u[n-1][0];
		if(( x2-x1 ) < InspPara.G2ZureXmin[TWA_Spec].nXsize ) return(ERROR);	//Judge_Zure_Lmin[2]

		for( n=0; n<64; n++ ) if( Ap_data_d[n][0] == 0xffff ) break;
		x1 = Ap_data_d[n-1][0];
		for( n=0; n<64; n++ ) if( G2_data_d[n][0] == 0xffff ) break;
		x2 = G2_data_d[n-1][0];
		if(( x1-x2 ) < InspPara.G2ZureXmin[TWA_Spec].nXsize ) return(ERROR);	//Judge_Zure_Lmin[2]
*/
		return(OK);
	}

//================================================
//		G2 std. edge position set
//================================================

int		G2StdEdgeSet( void )
	{
		int						dn, ns, ne;
		int						offset;
		int						rx, ry;
		int						vx, vy, xl, yl, xlmax, ylmax;
		int						left_x, right_x;
		int						ymin_l[3], ymax_l[3], ymin_r[3], ymax_r[3];
		int						up_yl[3], lo_yl[3];
		int						kaikiB = 0;
	//	int						hist[480];
		// initialze
			G2StdEdgeUp[0] = G2StdEdgeUp[1] = 0x0ffff;
			G2StdEdgeLo[0] = G2StdEdgeLo[1] = 0x0ffff;
			G2StdEdgeLe[0] = G2StdEdgeLe[1] = 0x0ffff;
			G2StdEdgeRi[0] = G2StdEdgeRi[1] = 0x0ffff;
			offset = G2StdOffsetX[1]/ES_G2_xp+1;

			//v2.20	if(!bIsLTR50){		// MCRのみ上下の傾き角平均化
			if((!bIsLTR50) && (!bIsLTR18)){		// MCRのみ上下の傾き角平均化
				// --- 電極位置X
				left_x	= (Ap_pos[0]+G2_pos[0])/2;
				right_x	= (Ap_pos[1]+G2_pos[1])/2;

			// Check upper and lower kaiki_b
				// --- 電極位置Y
				ymin_l[0] = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-left_x);
				ymin_l[1] = ue.kaiki_ey-w.kaiki_b*(ue.kaiki_ex-left_x);
				ymin_r[0] = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-right_x);
				ymin_r[1] = ue.kaiki_ey-w.kaiki_b*(ue.kaiki_ex-right_x);
				ymax_l[0] = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-left_x);
				ymax_l[1] = shita.kaiki_ey-w.kaiki_b*(shita.kaiki_ex-left_x);
				ymax_r[0] = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-right_x);
				ymax_r[1] = shita.kaiki_ey-w.kaiki_b*(shita.kaiki_ex-right_x);
				// ---
				up_yl[0] = abs(Ap_pos[2]-ymin_l[0]) + abs(Ap_pos[4]-ymin_r[0]);
				up_yl[1] = abs(Ap_pos[2]-ymin_l[1]) + abs(Ap_pos[4]-ymin_r[1]);
				lo_yl[0] = abs(Ap_pos[3]-ymax_l[0]) + abs(Ap_pos[5]-ymax_r[0]);
				lo_yl[1] = abs(Ap_pos[3]-ymax_l[1]) + abs(Ap_pos[5]-ymax_r[1]);
				// ---
				if((up_yl[0]>=up_yl[1])&&(lo_yl[0]>=lo_yl[1])) {
													kaikiB = 1;
				} else {
					if( up_yl[0] < lo_yl[0] ) {
						if( up_yl[0] >= up_yl[1] )	kaikiB = 1;
						else						kaikiB = 2;
					} else {
						if( lo_yl[0] >= lo_yl[1] )	kaikiB = 1;
						else						kaikiB = 3;
					}
				}
				// ---
				switch( kaikiB ) {
					case 0:
						return ERROR;
						break;
					case 1:
						ue.kaiki_b		= w.kaiki_b;
						shita.kaiki_b	= w.kaiki_b;
						break;
					case 2:
						shita.kaiki_b	= ue.kaiki_b;
						break;
					case 3:
						ue.kaiki_b		= shita.kaiki_b;
						break;
					default:
						return ERROR;
						break;
				}
			}
		// Upper
			// --- start and end data
			for( dn=0; dn<80; dn++ )	if( G2_data_u[dn][0] == 0xffff ) break;
			ns = offset;
			ne = dn-offset;
			if( ns >= ne ) return ERROR;
			// --- Y min pos.
			ylmax = 0;
		//	bzero(hist,480*sizeof(int));
			for( dn=ns; dn<ne; dn++ ) {
				rx = G2_data_u[dn][0];
				ry = G2_data_u[dn][1];
				vy = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-rx);
				if(ry < ElecPosRough[DataLR][PosYmin])	ry=vy;	// ラフ位置決めより外側のポイントは無視する 20170612 tanaka
				yl = vy - ry;
		//		if(yl >=0)	hist[yl]++;
				ylmax = max( ylmax, yl );
			}
		//	if(ylmax >= 5){		条件を付けようとしたが本当のNGが取れないため外し  20170424
		//		if(hist[ylmax]+hist[ylmax-1]+hist[ylmax-2] < 3) ylmax=0;
		//	}
			G2StdEdgeUp[0] = ue.kaiki_ex;
			G2StdEdgeUp[1] = ue.kaiki_ey - ylmax;

		// Lower
			// --- start and end data
			for( dn=0; dn<80; dn++ )	if( G2_data_d[dn][0] == 0xffff ) break;
			ns = offset;
			ne = dn-offset;
			if( ns >= ne ) return ERROR;
			// --- Y max pos.
			ylmax = 0;
		//	bzero(hist,480*sizeof(int));
			for( dn=ns; dn<ne; dn++ ) {
				rx = G2_data_d[dn][0];
				ry = G2_data_d[dn][1];
				vy = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-rx);
				if(ry > ElecPosRough[DataLR][PosYmax])	ry=vy;	// ラフ位置決めより外側のポイントは無視する 20170612 tanaka
				yl = ry - vy;
		//		if(yl >=0)	hist[yl]++;
				ylmax = max( ylmax, yl );
			}
		//	if(ylmax >= 5){
		//		if(hist[ylmax]+hist[ylmax-1]+hist[ylmax-2] < 3) ylmax=0;
		//	}
			G2StdEdgeLo[0] = shita.kaiki_ex;
			G2StdEdgeLo[1] = shita.kaiki_ey + ylmax;

		// Left
			// --- start and end data
			for( dn=0; dn<80; dn++ )	if( G2_data_l[dn][0] == 0xffff ) break;
			ns = 1;
			ne = dn-1;
			if( ns >= ne ) return ERROR;
			// --- X min pos.
			xlmax = 0;
			for( dn=ns; dn<ne; dn++ ) {
				rx = G2_data_l[dn][0];
				ry = G2_data_l[dn][1];
				vx = hidari.kaiki_ex-w.kaiki_b*(ry-hidari.kaiki_ey);
				xl = vx - rx;
				xlmax = max( xlmax, xl );
			}
			G2StdEdgeLe[0] = hidari.kaiki_ex - xlmax;
			G2StdEdgeLe[1] = hidari.kaiki_ey;

		// Right
			// --- start and end data
			for( dn=0; dn<80; dn++ )	if( G2_data_r[dn][0] == 0xffff ) break;
			ns = 1;
			ne = dn-1;
			if( ns >= ne ) return ERROR;
			// --- X max pos.
			xlmax = 0;
			for( dn=ns; dn<ne; dn++ ) {
				rx = G2_data_r[dn][0];
				ry = G2_data_r[dn][1];
				vx = migi.kaiki_ex-w.kaiki_b*(ry-migi.kaiki_ey);
				xl = rx - vx;
				xlmax = max( xlmax, xl );
			}
			G2StdEdgeRi[0] = migi.kaiki_ex + xlmax;
			G2StdEdgeRi[1] = migi.kaiki_ey;

		return OK;

	}	// G2StdEdgeSet( void ) end

/************************************************
	外形からＧ２までの寸法判定
	Ｇ２回帰直線からＧ２までの寸法判定
************************************************/

int		G2_Judge_w_SDl( void )
	{
		int						n, ns, ne;
		int						judge_sd, judge_sd_n;
		int						bad_n;
		int						rx, ry, iy, std_y;
		int						xs, xe, ys, ye;
		int						i_mode	= R_CS_GetInspectMode();

		judge_sd   = InspPara.G2YSdl[TWA_Spec].nYsize;				//Judge_G2W_SDtoG2[TWA_Spec][3]
		judge_sd_n = InspPara.G2YSdl[TWA_Spec].nXsize / ES_G2_xp;	//Judge_G2W_SDtoG2[TWA_Spec][2]
		if( judge_sd_n <= 0 ) judge_sd_n = 1;

		// G2 Up
			// data n
				for( n=0; n<80; n++ ) if( G2_data_u[n][0] == 0xffff ) break;
				ns = 1;
				ne = n-1;
				if( ns >= ne ) return ERROR;

				if( i_mode & INSPECT_STEP ) {
					R_DrawCls();
					xs = G2_data_u[ns][0];
					xe = G2_data_u[ne][0];
					ys = G2StdEdgeUp[1]-ue.kaiki_b*(G2StdEdgeUp[0]-xs);
					ye = G2StdEdgeUp[1]-ue.kaiki_b*(G2StdEdgeUp[0]-xe);
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_REVERSE, SOLID_LINE );
					R_mouse_de_yesno();
					ys = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-xs);
					ye = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-xe);
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_REVERSE, DOTTED_LINE );
					R_mouse_de_yesno();
				}

			// Judge  SD - G2
				bad_n = 0;
				for( n=ns; n<ne; n++ ) {
					rx = G2_data_u[n][0];
					ry = G2_data_u[n][1];
//					std_y = ue.kaiki_ey-ue.kaiki_b*(ue.kaiki_ex-rx);
                    std_y = G2StdEdgeUp[1]-ue.kaiki_b*(G2StdEdgeUp[0]-rx);
					iy = ry - std_y;
					if( iy < 0 ) iy = 0;
					if( iy > judge_sd ) {
						bad_n++;
						if( i_mode & INSPECT_STEP ) {
							R_DrawLine( DRAW_GREEN, rx, std_y, rx, ry, DRAW_REVERSE, SOLID_LINE );
						}
						if( bad_n >= judge_sd_n ) {
							// disp
								if( i_mode & INSPECT_STEP ) {
									R_DrawLine( DRAW_RED, rx, std_y, rx, ry, DRAW_REVERSE, SOLID_LINE );
					                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30   ,"[G2_Judge_w_SDl Up]");
					                R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30+2 ,"iy=%d    > judge_sd=%d"   ,iy,judge_sd);
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

		// G2 Lo
			// data n
				for( n=0; n<80; n++ ) if( G2_data_d[n][0] == 0xffff ) break;
				ns = 1;
				ne = n-1;
				if( ns >= ne ) return ERROR;

				if( i_mode & INSPECT_STEP ) {
					xs = G2_data_d[ns][0];
					xe = G2_data_d[ne][0];
					ys = G2StdEdgeLo[1]-shita.kaiki_b*(G2StdEdgeLo[0]-xs);
					ye = G2StdEdgeLo[1]-shita.kaiki_b*(G2StdEdgeLo[0]-xe);
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_REVERSE, SOLID_LINE );
					ys = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-xs);
					ye = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-xe);
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_REVERSE, DOTTED_LINE );
				}
			// Judge  SD - G2
				bad_n = 0;
				for( n=ns; n<ne; n++ ) {
					rx = G2_data_d[n][0];
					ry = G2_data_d[n][1];
//					std_y = shita.kaiki_ey-shita.kaiki_b*(shita.kaiki_ex-rx);
                    std_y = G2StdEdgeLo[1]-shita.kaiki_b*(G2StdEdgeLo[0]-rx);
					iy = std_y - ry;
					if( iy < 0 ) iy = 0;
					if( iy > judge_sd ) {
						bad_n++;
						// disp
						if( i_mode & INSPECT_STEP ) {
							R_DrawLine( DRAW_GREEN, rx, ry, rx, std_y, DRAW_REVERSE, SOLID_LINE );
						}
						if( bad_n >= judge_sd_n ) {
							if( i_mode & INSPECT_STEP ) {
								R_DrawLine( DRAW_RED, rx, ry, rx, std_y, DRAW_REVERSE, SOLID_LINE );
					            R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30   ,"[G2_Judge_w_SDl Low]");
					            R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30+2 ,"iy=%d    > judge_sd=%d"   ,iy,judge_sd);
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


/************************************************
	外形からＧ２までの寸法判定
	Ｇ２回帰直線からＧ２までの寸法判定
************************************************/

int		G2_Judge_w_APl( void )
	{
		int						n, ns, ne;
		int						judge_ap, judge_ap_n;
		int						bad_n;
		int						rx, ry, iy, std_y;
		int						xs, xe, ys, ye;
		int						i_mode	= R_CS_GetInspectMode();

		judge_ap   = InspPara.G2YApl[TWA_Spec].nYsize;				//Judge_G2W_APtoG2[TWA_Spec][3]
		judge_ap_n = InspPara.G2YApl[TWA_Spec].nXsize / ES_G2_xp;	//Judge_G2W_APtoG2[TWA_Spec][2]
		if( judge_ap_n <= 0 ) judge_ap_n = 1;

		// G2 Up
			// data n
				for( n=0; n<80; n++ ) if( G2_data_u[n][0] == 0xffff ) break;
				ns = 1;
				ne = n-1;
				if( ns >= ne ) return ERROR;

				if( i_mode & INSPECT_STEP ) {
					R_DrawCls();
					xs = G2_data_u[ns][0];
					xe = G2_data_u[ne][0];
					ys = G2StdEdgeUp[1]-Ap_up.kaiki_b*(G2StdEdgeUp[0]-xs);
					ye = G2StdEdgeUp[1]-Ap_up.kaiki_b*(G2StdEdgeUp[0]-xe);
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_REVERSE, SOLID_LINE );
				}
			// Judge  Ap - G2
				bad_n = 0;
				for( n=ns; n<ne; n++ ) {
					rx = G2_data_u[n][0];
					ry = G2_data_u[n][1];
//					std_y = Ap_up.kaiki_ey-Ap_up.kaiki_b*(Ap_up.kaiki_ex-rx);
//	//				std_y = ue.kaiki_ey-Ap_up.kaiki_b*(ue.kaiki_ex-rx);
					std_y = G2StdEdgeUp[1]-Ap_up.kaiki_b*(G2StdEdgeUp[0]-rx);
					iy = ry - std_y;
                    if( iy < 0 ) iy = 0;
                    if( iy > judge_ap){ // || iy<0) {
						bad_n++;
						// disp
						if( i_mode & INSPECT_STEP ) {
							R_DrawLine( DRAW_GREEN, rx, std_y, rx, ry, DRAW_REVERSE, SOLID_LINE );
						}
						if( bad_n >= judge_ap_n) {
							if( i_mode & INSPECT_STEP ) {
								R_DrawLine( DRAW_RED, rx, std_y, rx, ry, DRAW_REVERSE, SOLID_LINE );
					            R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30   ,"[G2_Judge_w_APl Up]");
					            R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30+2 ,"iy=%d    > judge_ap=%d"   ,iy,judge_ap);
					            R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30+3 ,"bad_n=%d > judge_ap_n=%d" ,bad_n,judge_ap_n);
								WaitMouseClick();
							}
							return ERROR2;
						}
					} else {
						bad_n = 0;
					}
				}

		// G2 Lo
			// data n
				for( n=0; n<80; n++ ) if( G2_data_d[n][0] == 0xffff ) break;
				ns = 1;
				ne = n-1;
				if( ns >= ne ) return ERROR;

				if( i_mode & INSPECT_STEP ) {
					xs = G2_data_d[ns][0];
					xe = G2_data_d[ne][0];
					ys = G2StdEdgeLo[1]-Ap_lo.kaiki_b*(G2StdEdgeLo[0]-xs);
					ye = G2StdEdgeLo[1]-Ap_lo.kaiki_b*(G2StdEdgeLo[0]-xe);
					R_DrawLine( DRAW_GREEN, xs, ys, xe, ye, DRAW_REVERSE, SOLID_LINE );
				}

				// Judge  Ap - G2
				bad_n = 0;
				for( n=ns; n<ne; n++ ) {
					rx = G2_data_d[n][0];
					ry = G2_data_d[n][1];
//					std_y = Ap_lo.kaiki_ey-Ap_lo.kaiki_b*(Ap_lo.kaiki_ex-rx);
//	//				std_y = shita.kaiki_ey-Ap_lo.kaiki_b*(shita.kaiki_ex-rx);
					std_y = G2StdEdgeLo[1]-Ap_lo.kaiki_b*(G2StdEdgeLo[0]-rx);
					iy = std_y - ry;
                    if( iy < 0 ) iy = 0;
                    if( iy > judge_ap ){    // || iy< 0) {
						bad_n++;
						// disp
						if( i_mode & INSPECT_STEP ) {
							R_DrawLine( DRAW_GREEN, rx, ry, rx, std_y, DRAW_REVERSE, SOLID_LINE );
						}
						if( bad_n >= judge_ap_n) {
							if( i_mode & INSPECT_STEP ) {
								R_DrawLine( DRAW_RED, rx, ry, rx, std_y, DRAW_REVERSE, SOLID_LINE );
					            R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30   ,"[G2_Judge_w_APl Low]");
					            R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30+2 ,"iy=%d    > judge_ap=%d"   ,iy,judge_ap);
					            R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, 0, 30+3 ,"bad_n=%d > judge_ap_n=%d" ,bad_n,judge_ap_n);
								WaitMouseClick();
							}
							return ERROR3;
						}
					} else {
						bad_n = 0;
					}
				}

		// judge end

		return OK;

	}	// G2_Judge_w_APl( void ) end

void	ElecDataSet(int elec,int side)
{
	int rd;
	int n;
	switch(side){
		case 0:
			for(rd=0;rd<80;rd++){
				n=Ap_data_l[rd][1];
				if(n==0xffff)	break;
				ElecEdge[elec][side].dataR[n]=OK;
				ElecEdge[elec][side].dataX[n]=Ap_data_l[rd][0];
				ElecEdge[elec][side].dataY[n]=n;
			}
			break;
		case 1:
			for(rd=0;rd<80;rd++){
				n=Ap_data_u[rd][0];
				if(n==0xffff)	break;
				ElecEdge[elec][side].dataR[n]=OK;
				ElecEdge[elec][side].dataX[n]=n;
				ElecEdge[elec][side].dataY[n]=Ap_data_u[rd][1];
			}
			break;
		case 2:
			for(rd=0;rd<80;rd++){
				n=Ap_data_r[rd][1];
				if(n==0xffff)	break;
				ElecEdge[elec][side].dataR[n]=OK;
				ElecEdge[elec][side].dataX[n]=Ap_data_r[rd][0];
				ElecEdge[elec][side].dataY[n]=n;
			}
			break;
		case 3:
			for(rd=0;rd<80;rd++){
				n=Ap_data_d[rd][0];
				if(n==0xffff)	break;
				ElecEdge[elec][side].dataR[n]=OK;
				ElecEdge[elec][side].dataX[n]=n;
				ElecEdge[elec][side].dataY[n]=Ap_data_d[rd][1];
			}
			break;
	}
}

/************************************************
	インデックステーブルエッジサーチ
************************************************/

static int TWA_Index[2]={422,253};		//1000,600 インデックスサイズ（仮）
//static	int				Index_Gpos[5];										// Index ガイド　エッジ位置( upx, lox, minx )
extern int IGuidePos[2];
int		TWA_Index_search( void )
{
	int					n, l, ni;
	int					count[4], t_count, ans[4];
	int					xs[4], xe[4], ys, ye;
	int					judge_Wmin, judge_Wmax, w;
	int					yl;
	int					rxs, rxe;
	int					i_mode	= R_CS_GetInspectMode();

	int src_n;
	int dst_n;
	int i;
	R_LINE retl;

	//インデツクステーブルエッジ検出＆外形検査2値画像処理
	//２値化（グレープレーンから２値プレーンへ）
	R_gray2bin16( Bin_Ap_l_ini[TWA_Spec][0], 0, 0, TWA_X_size, TWA_Y_size, Gray_address[InspGrayPn], Bin_address[InspBinPn] );
	
	if( i_mode & INSPECT_STEP ) {
		R_move_bin_memory(InspBinPn,BinP0,0,0,FxSize-1,FySize-1);
		WaitMouseClick();
	}

	//インデックスＷ寸法判定値
	judge_Wmin = TWA_Index[1]* 8/10;
	judge_Wmax = TWA_Index[1]*12/10;
	
	//サーチ条件設定
/*	l = FX_SIZE/4 - 1;
	xs[0] = 0;
	xe[0] = xs[0] + l;
	xs[1] = FX_SIZE/4;
	xe[1] = xs[1] + l;
	xs[2] = FX_SIZE/2;
	xe[2] = xs[2] + l;
	xs[3] = FX_SIZE*3/4;
	xe[3] = xs[3] + l;
	t_count = l + 1;
*/
	xs[0] = 150;
	xe[0] = 209;
	xs[1] = 210;
	xe[1] = 269;
	xs[2] = 270;
	xe[2] = 329;
	xs[3] = 330;
	xe[3] = 389;
	t_count = 60;

//	l = TWA_Y_size/4;
	l = 150;

	//上
	for( ys=ni=2; ys<l; ys++ ) {
		ye = ys;
		for( n=0; n<4; n++ ) {
			count[n] = R_count_bin_area( InspBinPn, xs[n], ys, xe[n], ye );
			ans[n] = count[n]*100/t_count;
			if( ans[n] < 30 ) {
				ans[n] = OK;
//				break;
			} else {
				ans[n] = ERROR;
			}
		}
		if( ans[0]+ans[1]+ans[2]+ans[3] >= -2 ) break;
	}
	if( ys == ni ) return(ERROR);
	if( ys < l ) Index_pos[0] = (ys-1)+5;
	else         Index_pos[0] = 0xffff;
	
	//下
	for( ye=ni=TWA_Y_size-1; ye>l; ye-- ) {
		//			for( ye=ni=FY_SIZE/2-2; ye>l; ye-- ) {
		ys = ye;
		for( n=0; n<4; n++ ) {
			count[n] = R_count_bin_area( InspBinPn, xs[n], ys, xe[n], ye );
			ans[n] = count[n]*100/t_count;
			if( ans[n] < 30 ) {
				ans[n] = OK;
	//			break;
			} else {
				ans[n] = ERROR;
			}
		}
		if( ans[0]+ans[1]+ans[2]+ans[3] >= -2 ) break;
	}
	if( ye == ni ) return(ERROR);
	if( ye > l ) Index_pos[1] = (ye+1)-5;
	else         Index_pos[1] = 0xffff;
	
	// Index size
	Index_pos[3] = Index_pos[0];	// 上
	Index_pos[4] = Index_pos[1];	// 下
/*
	if( TWA_Index_mode != 0 ) {
		if((Index_pos[3]!=0xffff)&&(Index_pos[4]!=0xffff)) {
			TWA_Index_size = Index_pos[4]-Index_pos[3];
			yl = abs( TWA_Index_size - Index_pos[2] );
			if( yl > Judge_Index[1] ) {
				TWA_Index_er++;
				if( TWA_Index_er > Judge_Index_n ) TWA_Index_Stop = ON;
			} else {
				TWA_Index_er = 0;
			}
		}
	} else {
		TWA_Index_Stop = OFF;
		TWA_Index_er = 0;
	}
*/
	if((Index_pos[0]==0xffff)&&(Index_pos[1]==0xffff)) return(ERROR);
	if( Index_pos[0]==0xffff ) {
		Index_pos[0] = Index_pos[1] - Index_pos[2];
		return(ERROR2);
	}
	if( Index_pos[1]==0xffff ) {
		Index_pos[1] = Index_pos[0] + Index_pos[2];
		return(ERROR2);
	}
	
	if( Index_pos[0] < 5 ) return(ERROR); 
	if( Index_pos[1] > (TWA_Y_size-5) ) return(ERROR); 
	//			if( Index_pos[1] > (FY_SIZE-5) ) return(ERROR); 
	
	if( i_mode & INSPECT_STEP ) {
		R_line( 0, Index_pos[0], FX_SIZE-1, Index_pos[0], GRAPH_REVERSE, SOLID_LINE );
		R_line( 0, Index_pos[1], FX_SIZE-1, Index_pos[1], GRAPH_REVERSE, SOLID_LINE );
		WaitMouseClick();
	}
	
	w = Index_pos[1]-Index_pos[0];
	if( judge_Wmin > w ) return(ERROR);
	if( judge_Wmax < w ) return(ERROR);
	
	Index_pos[2] = TWA_Index[1];
/*
	// Index ガイド位置検出
	l = TWA_X_size/4;
	// up
	ye = Index_pos[0];
	ys = ye - 5;
	if( ys < 0 ) ys = 0;
	t_count = ye-ys+1;
	if( t_count == 0 ) return ERROR;            //Hang
	rxs=TWA_X_size-2;
	for( ; rxs>l; rxs-- ) {
		rxe = rxs;
		count = R_count_bin_area( InspBinPn, rxs, ys, rxe, ye );
		ans = count*100/t_count;
		if( ans < 10 ) break;
	}
	if( rxs > l ) Index_Gpos[0] = rxs;
	else          Index_Gpos[0] = 0xffff;
	// lo
	ys = Index_pos[1];
	ye = ys + 5;
	if( ye > TWA_Y_size ) ye = TWA_Y_size;
	t_count = ye-ys+1;
	if( t_count == 0 ) return ERROR;            //Hang
	rxs=TWA_X_size-2;
	for( ; rxs>l; rxs-- ) {
		rxe = rxs;
		count = R_count_bin_area( InspBinPn, rxs, ys, rxe, ye );
		ans = count*100/t_count;
		if( ans < 10 ) break;
	}
	if( rxs > l ) Index_Gpos[1] = rxs;
	else          Index_Gpos[1] = 0xffff;
	
	if((Index_Gpos[0]!=0xffff)&&(Index_Gpos[1]!=0xffff)) {
		Index_Gpos[2] = min( Index_Gpos[0], Index_Gpos[1] );
	} else if((Index_Gpos[0]==0xffff)&&(Index_Gpos[1]==0xffff)) {
		return ERROR;
	} else if( Index_Gpos[0] == 0xffff ) {
		Index_Gpos[2] = Index_Gpos[1];
	} else if( Index_Gpos[1] == 0xffff ) {
		Index_Gpos[2] = Index_Gpos[0];
	}
	if( i_mode & INSPECT_STEP ) {
		R_line( Index_Gpos[2], Index_pos[0], Index_Gpos[2], Index_pos[1], GRAPH_REVERSE, SOLID_LINE );
		WaitMouseClick();
	}
*/
	int sumx = 0;
	int county = 0;
	int rx,ry,rc;
	ROBUST_RESULT result; //ロバスト推定結果
	R_es_set_page( InspGrayPn );		//	グレー画面grayPnページを処理対象とする。
	R_es_set_degree( 0 );			//	認識方向をright 0°方向に設定する。
	R_es_set_length( 130 );			//	サーチ距離を130とする。
	R_es_set_level( 10 );			//	10未満の階調変化はエッジとみなさない。
	R_es_set_average( 1 );			//	片側１画素、計３画素の平均を取る。
	R_es_set_noise( 1 );			//	1画素以下の黒（又は白）は無視する。
//	R_es_set_slice( 90 );			//	最大値と最小値の真ん中でエッジと判断する。
	R_es_set_slice( 200 );			//	最大値と最小値の真ん中でエッジと判断する。
//	for(ys = Index_pos[0]+5;ys < Index_pos[1]-5;ys++){
	for(ys = Index_pos[0]+60;ys < Index_pos[1]-5;ys++){
	//	rx = 10;
		rx = 40;
		ry = ys;
		R_es_set_start( rx, ry );		//	認識開始点の設定
		rc = ES_WHITE;					//  white -> black
		if( R_es_search( &rx, &ry, &rc ) == OK ){
//			sumx += rx;
			Src_point[county].x = rx/1000;
			Src_point[county].y = ry/1000;
			county++;
		}
	}
	// 座標点群の直線成分の検出
	if(county > 0){
		src_n =  county;
		RobustCond.deg_min = 880;
		RobustCond.deg_max = 920;
		//if(OK!=R_robust_get_angle( src_n, Src_point, RobustCond, &result )) return ERROR;

		R_robust_sampling_line( src_n, Src_point, &dst_n, Dst_point, RobustCond, 0 );	// 最頻値相当の成分のみ抽出
		if( dst_n == 0 )	return ERROR;

		if( i_mode & INSPECT_STEP ) {
			for(i=0;i<dst_n;i++){
				R_DrawPset(DRAW_RED,Dst_point[i].x,Dst_point[i].y,DRAW_NORMAL);
			}
		}
		//座標点群の直線近似
		R_robust_get_line( dst_n, Dst_point, RobustCond, &retl ); //直線近似
	//	R_printf(0,3, "a=%d,b=%d,c=%d",l.a,l.b,l.c); //直線の係数表示

		//IGuidePos[1]=(sumx/county) / 1000;
		if(retl.a !=0){
			IGuidePos[1]=-(int)(retl.c/retl.a);
		}
	}
	if( i_mode & INSPECT_STEP ) {
		R_line( IGuidePos[1], Index_pos[0], IGuidePos[1], Index_pos[1], GRAPH_REVERSE, SOLID_LINE );
		WaitMouseClick();
	}

	return(OK);
}


