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
#include "R_cs.h"
#include "R_cdraw.h"
#include "tmcr00i.h"
#include "tp_sub.h"
#include "mcr03.h"
#include "mark18x3x1.h"

#define Mark_Debug_E25  0

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
//static	int         	max_st[5][2], max_re[5][2];

//標印検査
		int				Mark_Xseg[50][5];									//文字切り出し結果 X[ judge, xs, xe, level, 誤差 ]
		int				Mark_Yseg[50][5];									//文字切り出し結果 Y[ judge, ys, ye, level, 誤差 ]
		int				Mark_zure[4]		= { 0,0,0,0 };					//標印ズレdata
		int				Mark_area_xl;										//標印文字範囲( X size )
		int				Mark_area_yl;										//標印文字範囲( Y size )
		int				Mark_Slevel_s[5][3];								//標印文字有り無し判断レベル( min, center, max )
		int				Mark_Slevel_r[5][3];								//標印文字有り無し判断レベル( min, center, max )
		int				Mark_div_x;											//標印文字分割数 X
		int				Mark_div_y;											//標印文字分割数 Y
		int				Mark_div_ws[4][50];									//標印文字分割Data(Bin正方向)White
		int				Mark_div_wr[4][50];									//標印文字分割Data(Bin逆方向)White
		int				Mark_div_bs[4][50];									//標印文字分割Data(Bin正方向)Black
		int				Mark_div_br[4][50];									//標印文字分割Data(Bin逆方向)Black
		int				Mark_area_x[125][4][4];								//標印文字範囲X ([θ][桁][x1,x2,x3,x4])
		int				Mark_area_y[125][4][4];								//標印文字範囲Y ([θ][桁][y1,y2,y3,y4])
		int				Mark_space_x[125][5][10];							//標印文字間隔X ([θ][桁+1][x1,x2,,..])
		int				Mark_space_y[125][5][10];							//標印文字間隔Y ([θ][桁+1][y1,y2,,..])
		int				Mark_size_ls[5][10][2];									//標印文字Size L White
		int				Mark_size_lr[5][10][2];									//標印文字Size L White
//static	int				Mark_space_l[5][10];								//標印文字間隔Data(gray)
		int				Mark_rx[125][4][30];								//標印検査ポイントX([θ][桁][Xdata])
		int				Mark_ry[125][4][30];								//標印検査ポイントY([θ][桁][Ydata])
//static	int				Mark_rx_J[50];										//標印検査ポイントX([Xdata])JPW
//static	int				Mark_ry_J[50];										//標印検査ポイントY([Ydata])JPW
//		int				Mark_Judge_st[5][11][5];							//標印文字判定結果[桁][文字][一致,Check,定点]
//		int				Mark_Judge_re[5][11][5];							//標印文字判定結果[桁][文字][一致,Check,定点]
		int				Mark_Judge_st[5][NumMarkChar][5];					//標印文字判定結果[桁][文字][一致,Check,定点]
		int				Mark_Judge_re[5][NumMarkChar][5];					//標印文字判定結果[桁][文字][一致,Check,定点]
//static	int				Nijimi_Judge[5];							//標印文字判定結果[桁][文字][一致,Check,定点]
		int				Mark_L_set[5]		= { 0,0,0,0,0 };				//標印詳細検査設定（0:検査無し 1:詳細検査）
		int				Mark_Ans_st[5];
		int				Mark_Ans_re[5];
		int				Mark_J100_st[5];
		int				Mark_J100_re[5];
		int				Mark_Insp_mode		= 0x0000;						//標印詳細検査設定bit（1桁目 0x01, 2桁目 0x02, 3桁目0x04, 4桁目0x08）
//static int  Mark_pos_le;
//static int 	Mark_pos_ri;
//static int	Diff_mark_seg;
		int	mark_zure1	= ERROR;
		int	mark_zure2	= ERROR;
		int	mark_zure3	= ERROR;
		int	mark_zure4	= ERROR;

		int				mox[4],moy[4];	//標印のオフセットメモ用

//標印
extern char				MarkChar[10];

		int             TWA_R_data[6];										//設定抵抗値（数値）

		int				TWA_mark_area_p[8];									//標印文字範囲
//static	int				TWA_Judge_Ma		= ERROR;						//判定結果
#define	Insp_Pinhole		2

#define TWA_Ma			3

//static  int				Mark_temp[6];
//static	int				Bin_Ma_l_ini[4][2]	= {{ 30, 40},{ 30, 40},{ 30, 40},{ 30, 40}};	//標印検査時の２値化レベル設定範囲（ 白Level ）
static	int				Bin_Ma_l_ini[4][2]	= {{ 80,100},{ 80,100},{ 80,100},{ 80,100}};	//標印検査時の２値化レベル設定範囲
//static	int				Bin_Pl_l_ini[4][2]	= {{ 45, 55},{ 45, 55},{ 45, 55},{ 45, 55}};	//メッキ付着検査時の２値化レベル設定範囲
//static	int				Bin_Bl_l_ini[4][2]	= {{ 60, 80},{ 60, 80},{ 60, 80},{ 60, 80}};	//黒チップ検査時の２値化レベル設定範囲
//Auto bin level														//jig add search auto bin level for search sigmentation

extern  char			SpecData[20];										// バーコードデータ
extern	int				Index_pos[6];										//インデックステーブル　エッジ位置( uy, dy, ly )

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

static void	TWA_G2_level( int *level, int *l_lo, int *l_up );
static int	TWA_segmentation( int deg10, int level, int n_seg, int offset_x, int offset_y );
static int	TWA_Mark_level( int deg10, int ox, int oy );
	   int	TWA_Mark_point_10_25( int deg10, int ox, int oy, int ol );
static int	TWA_Mark_Judge( void );
static void	TWA_get_color_area( int page, int pt[], int *jw, int *jb );
static void	TWA_Mark_Size_E25( int page, int pt[], int *ls, int *le );

	   void	TWA_Mark_div_check( int pt[], int *nst, int *nre, int *sum );
static void	TWA_Mark_error( int nc );

static void	TWA_mark_position_set( int area );
	   int	ChipEsLevelSet( void );
	   void	ChipGray2Bin( void );
	   int	Regression( int elec, int side );

#define KURO			0
#define SHIRO			1
//================================================
//	電極幅検査( ESR )
//================================================
/*

int		TWA_Insp_C2_width( void )
	{
		int					pt[4];
		int					xs, xe, xl;
		int					ys, ye, yl;
		int					rx, ry, rl;
		int					G2_x;
		int					d, dn;
		int					le_up[4];				//xs, ys, xe, ye
		int					le_lo[4];
		int					ri_up[4];
		int					ri_lo[4];
		int					c2l, c2lsum, c2ln, c2lpass;
		int					judge_c2n = 4;
		int					cx, cy;
		int					offset_lx = 1;
		int					offset_rx = 1;
		int					offset_xs = -4;
		int					offset_uy =  2;	//5;
		int					offset_ly =  2;	//5;
		int					res_lu, res_ll, res_ru, res_rl;

		// Judge data
			judge_c2n = Judge_C2_Lmin[3];
		// initial
			TWA_bin_cls( 1, 0, 0, FX_SIZE-1, FY_SIZE-1 );
		// 検査範囲２値化
			// y position
			ys = Index_pos[0]-10;
			ye = Index_pos[1]+10;
			yl = ye-ys+1;
			// left
			xs = Ap_pos[0]-10;		if( xs < 5 ) xs = 5;
			xe = G2_pos[0]+10;		if( xe > TWA_X_size-5 ) xe = TWA_X_size - 5;
			xl = xe-xs+1;
//			TWA_gray2bin( Bright_C1_left, xs, ys, xl, yl, 2, 1 );
			pt[0]=xs;	pt[1]=ys;	pt[2]=xe;	pt[3]=ye;
			// right
			xs = G2_pos[1]-10;		if( xs < 5 ) xs = 5;
			xe = Ap_pos[1]+10;		if( xe > TWA_X_size-5 ) xe = TWA_X_size - 5;
			xl = xe-xs+1;
//			TWA_gray2bin( Bright_C1_right, xs, ys, xl, yl, 2, 1 );
			pt[0]=xs;	pt[1]=ys;	pt[2]=xe;	pt[3]=ye;

		//検査範囲設定
			//左上 area
				// xe
				le_up[0] = Corner_pos[0] - offset_lx;
				// xs
				G2_x = dn = 0;
				for( d=2; d<64; d++ ) {
					if( G2_data_l[d][0] == 0xffff ) return(ERROR);
					G2_x += G2_data_l[d][0];
					dn++;
					if( dn >= 5 ) break;
				}
				if( dn == 0 ) return(ERROR);
				le_up[2] = G2_x / dn + offset_xs;		//xs
				// ys
				le_up[1] = Corner_pos[1] + offset_uy;
				// ye
				le_up[3] = le_up[1] + 9;

			//左下 area
				// xe
				le_lo[0] = Corner_pos[2] - offset_lx;
				// xs
				G2_x = dn = 0;
				for( d=0; d<64; d++ ) {
					if( G2_data_l[d][0] == 0xffff ) break;
				}
				d-=3;
				for( ; d>0; d-- ) {
					G2_x += G2_data_l[d][0];
					dn++;
					if( dn >= 5 ) break;
				}
				if( dn == 0 ) return(ERROR);
				le_lo[2] = G2_x / dn + offset_xs;		//xs
				// ys
				le_lo[3] = Corner_pos[3] - offset_ly;
				// ye
				le_lo[1] = le_lo[3] - 9;	//ye

			//右上 area
				// xe
				ri_up[2] = Corner_pos[4] + offset_rx;
				// xs
				G2_x = dn = 0;
				for( d=2; d<64; d++ ) {
					if( G2_data_r[d][0] == 0xffff ) return(ERROR);
					G2_x += G2_data_r[d][0];
					dn++;
					if( dn >= 5 ) break;
				}
				if( dn == 0 ) return(ERROR);
				ri_up[0] = G2_x / dn - offset_xs;		//xs
				// ys
				ri_up[1] = Corner_pos[5] + offset_uy;
				// ye
				ri_up[3] = ri_up[1] + 9;

			//右下 area
				// xe
				ri_lo[2] = Corner_pos[6] + offset_rx;
				// xs
				G2_x = dn = 0;
				for( d=0; d<64; d++ ) {
					if( G2_data_r[d][0] == 0xffff ) break;
				}
				d-=3;
				for( ; d>0; d-- ) {
					G2_x += G2_data_r[d][0];
					dn++;
					if( dn >= 5 ) break;
				}
				if( dn == 0 ) return(ERROR);
				ri_lo[0] = G2_x / dn - offset_xs;		//xs
				// ys
				ri_lo[3] = Corner_pos[7] - offset_ly;
				// ye
				ri_lo[1] = ri_lo[3] - 9;

			if( TWA_test  == TWA_G2 ) {
				R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
//				R_move_bin_memory( 1, 0, 0, 0, TWA_X_size-1, TWA_Y_size-1 );
				R_box( le_up[0], le_up[1], le_up[2], le_up[3], GRAPH_REVERSE, SOLID_LINE );
				R_box( le_lo[0], le_lo[1], le_lo[2], le_lo[3], GRAPH_REVERSE, SOLID_LINE );
				R_box( ri_up[0], ri_up[1], ri_up[2], ri_up[3], GRAPH_REVERSE, SOLID_LINE );
				R_box( ri_lo[0], ri_lo[1], ri_lo[2], ri_lo[3], GRAPH_REVERSE, SOLID_LINE );
				WaitMouseClick();
				R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
			}
		//検査範囲設定 end

		// Insp.
			// 左上 insp.
				ys = le_up[1];
				ye = le_up[3];
				xs = le_up[2];
				c2l = c2lsum = c2ln = c2lpass = 0;
				for( ry=ys; ry<=ye; ry++ ) {
					rx = xs;
					xe = le_up[0];
					rl = xs-xe+1;
					if( TWA_es_gray_search_C1( 2, &rx, &ry, 180, rl, Bright_C1_left ) == OK ) {
						c2l = rx-xe+1;
						c2lsum += c2l;
						if((c2l>=Judge_C2_Lmin[2])&&(c2l<=Judge_C1_Lmax[TWA_Spec][2])) {
							c2lpass++;
						}
						if( TWA_test  == TWA_G2 ) {
							R_pset( rx, ry, GRAPH_REVERSE );
							R_pset( xe, ry, GRAPH_REVERSE );
//							R_line( xe, ry, rx, ry, GRAPH_REVERSE, SOLID_LINE );
						}
					}
					c2ln++;
				}
				if( c2ln > 0 ) {
					c2l = c2lsum/c2ln;
				} else {
					c2l = 0;
				}
				if( TWA_test  == TWA_G2 ) {
					sprintf( Comment, "L:%3dbit", c2l );
					cx = 0;
					cy = le_up[1]/12+2;
					R_chrdisp( cx, cy, Comment );		cy++;
					sprintf( Comment, "P:%3d", c2lpass );
					R_chrdisp( cx, cy, Comment );
					WaitMouseClick();
				}
				if( c2lpass < judge_c2n ) res_lu = ERROR;		// return(ERROR);
				else					  res_lu = OK;

			//左下 insp.
				ys = le_lo[1];
				ye = le_lo[3];
				xs = le_lo[2];
				c2l = c2lsum = c2ln = c2lpass = 0;
				for( ry=ys; ry<=ye; ry++ ) {
					rx = xs;
					xe = le_lo[0];
					rl = xs-xe+1;
					if( TWA_es_gray_search_C1( 2, &rx, &ry, 180, rl, Bright_C1_left ) == OK ) {
						c2l = rx-xe+1;
						c2lsum += c2l;
						if((c2l>=Judge_C2_Lmin[2])&&(c2l<=Judge_C1_Lmax[TWA_Spec][2])) {
							c2lpass++;
						}
						if( TWA_test  == TWA_G2 ) {
							R_pset( rx, ry, GRAPH_REVERSE );
							R_pset( xe, ry, GRAPH_REVERSE );
//							R_line( xe, ry, rx, ry, GRAPH_REVERSE, SOLID_LINE );
						}
					}
					c2ln++;
				}
				if( c2ln > 0 ) {
					c2l = c2lsum/c2ln;
				} else {
					c2l = 0;
				}
				if( TWA_test  == TWA_G2 ) {
					sprintf( Comment, "L:%3dbit", c2l );
					cx = 0;
					cy = le_lo[1]/12+2;
					R_chrdisp( cx, cy, Comment );		cy++;
					sprintf( Comment, "P:%3d", c2lpass );
					R_chrdisp( cx, cy, Comment );
					WaitMouseClick();
				}
				if( c2lpass < judge_c2n ) res_ll = ERROR;		// return(ERROR);
				else					  res_ll = OK;

			//右上
				ys = ri_up[1];
				ye = ri_up[3];
				xs = ri_up[0];
				c2l = c2lsum = c2ln = c2lpass = 0;
				for( ry=ys; ry<=ye; ry++ ) {
					rx = xs;
					xe = ri_up[2];
					rl = xe-xs+1;
					if( TWA_es_gray_search_C1( 2, &rx, &ry, 0, rl, Bright_C1_right ) == OK ) {
						c2l = xe-rx+1;
						c2lsum += c2l;
						if((c2l>=Judge_C2_Lmin[2])&&(c2l<=Judge_C1_Lmax[TWA_Spec][2])) {
							c2lpass++;
						}
						if( TWA_test  == TWA_G2 ) {
							R_pset( rx, ry, GRAPH_REVERSE );
							R_pset( xe, ry, GRAPH_REVERSE );
//							R_line( rx, ry, xe, ry, GRAPH_REVERSE, SOLID_LINE );
						}
					}
					c2ln++;
				}
				if( c2ln > 0 ) {
					c2l = c2lsum/c2ln;
				} else {
					c2l = 0;
				}
				if( TWA_test  == TWA_G2 ) {
					sprintf( Comment, "L:%3dbit", c2l );
					cx = 34;
					cy = ri_up[1]/12+2;
					R_chrdisp( cx, cy, Comment );		cy++;
					sprintf( Comment, "P:%3d", c2lpass );
					R_chrdisp( cx, cy, Comment );
					WaitMouseClick();
				}
				if( c2lpass < judge_c2n ) res_ru = ERROR;		// return(ERROR);
				else					  res_ru = OK;

			//右下
				ys = ri_lo[1];
				ye = ri_lo[3];
				xs = ri_lo[0];
				c2l = c2lsum = c2ln = c2lpass = 0;
				for( ry=ys; ry<=ye; ry++ ) {
					rx = xs;
					xe = ri_lo[2];
					rl = xe-xs+1;
					if( TWA_es_gray_search_C1( 2, &rx, &ry, 0, rl, Bright_C1_right ) == OK ) {
						c2l = xe-rx+1;
						c2lsum += c2l;
						if((c2l>=Judge_C2_Lmin[2])&&(c2l<=Judge_C1_Lmax[TWA_Spec][2])) {
							c2lpass++;
						}
						if( TWA_test  == TWA_G2 ) {
							R_pset( rx, ry, GRAPH_REVERSE );
							R_pset( xe, ry, GRAPH_REVERSE );
//							R_line( rx, ry, xe, ry, GRAPH_REVERSE, SOLID_LINE );
						}
					}
					c2ln++;
				}
				if( c2ln > 0 ) {
					c2l = c2lsum/c2ln;
				} else {
					c2l = 0;
				}
				if( TWA_test  == TWA_G2 ) {
					sprintf( Comment, "L:%3dbit", c2l );
					cx = 34;
					cy = ri_lo[1]/12+2;
					R_chrdisp( cx, cy, Comment );		cy++;
					sprintf( Comment, "P:%3d", c2lpass );
					R_chrdisp( cx, cy, Comment );
					WaitMouseClick();
				}
				if( c2lpass < judge_c2n ) res_rl = ERROR;		// return(ERROR);
				else					  res_rl = OK;

			// Judge
				if(( res_lu != OK )||( res_ll != OK )) return ERROR;
				if(( res_ru != OK )||( res_rl != OK )) return ERROR;

		return OK;

	}	// TWA_Insp_C2_width( void ) end

*/
/************************************************
	電極部平均明るさ計測
************************************************/
/*
int		TWA_Bright_C1( void )
	{
		int					xs, xe, ys, ye, xl, yl;
		int					bright_av;
		int					bright_t, bright_n, bright_a, bright_r;
		int					bright_left, bright_right;
		int					bright_low;
		int					n;
		int					offset_y = 20;
		int					cl, clmin, clmax;

		// initial
			if( TWA_Spec == SpecMCL ) {
				bright_low = Bin_Ap_l_ini[TWA_Spec][1];
			} else {
				bright_low = Bin_G2_l_ini[TWA_Spec][1];		//Bin_Ap_l_ini[TWA_Spec][1]
			}
		//左側電極　計測範囲設定
			xs = Ap_pos[0];		ys = Ap_pos[2]+offset_y;
			xe = G2_pos[0];		ye = Ap_pos[3]-offset_y;
			//指定エリアのヒストグラムを取る。（１ドットおきに間引く）
				bright_av = R_get_histgram( xs, ys, xe, ye, Gray_address[2] );
				if( bright_av == ERROR ) return ERROR;
				if( TWA_Spec == SpecMCL ) {
					bright_low = (bright_av + Bin_Ap_l_ini[TWA_Spec][1])/2;
				} else {
					bright_low = (bright_av + Bin_G2_l_ini[TWA_Spec][1])/2;
				}
				bright_t = bright_n = 0;
				for( n=bright_low; n<=255; n++ ) {
					bright_a = Bright[n] * n;
					bright_t += bright_a;
					bright_n += Bright[n];
				}
				if( bright_n == 0 ) {
					bright_left = bright_low;
					clmin = clmax = bright_left;
				} else {
					bright_r = bright_n/5 + 1;
					//min level
					cl = 0;
					for( n=bright_low; n<255; n++ ) {
						cl += Bright[n];
						if( cl > bright_r ) break;
					}
					clmin = n;
					if( clmin < 10 ) clmin = 10;	//最低レベル 10
					//max level
					cl = 0;
					for( n=255; n>0; n-- ) {
						cl += Bright[n];
						if( cl > bright_r ) break;
					}
					clmax = n;
					bright_left = ( bright_low + clmin )/2;
				}
			//manu
				if( TWA_test != 0 ) {
					R_display_control( BIN_MEMORY | GRAY_MEMORY );
					R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
					R_move_gray_memory( 2, 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					TWA_pset_histgram( 10, 50, Bright) ;
					R_box( xs, ys, xe, ye, GRAPH_REVERSE, DASHED_LINE );
					WaitMouseClick();
					ys -= offset_y;		ye += offset_y;
					xl = xe-xs+1;		yl = ye-ys+1;
					TWA_gray2bin( bright_left, xs, ys, xl, yl, 2, 0 );
					sprintf( Comment, "C1 LEFT bright min:%3d  av:%3d  max:%3d", clmin, bright_av, clmax );
					R_chrdisp( 1, 17, Comment );
					sprintf( Comment, "C1 LEFT bright Insp. level:%3d", bright_left );
					R_chrdisp( 1, 18, Comment );
					WaitMouseClick();
					R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
				}

		//右側電極　計測範囲設定
			xs = G2_pos[1];		ys = Ap_pos[4]+offset_y;
			xe = Ap_pos[1];		ye = Ap_pos[5]-offset_y;
			//指定エリアのヒストグラムを取る。（１ドットおきに間引く）
				bright_av = R_get_histgram( xs, ys, xe, ye, Gray_address[2] );
				if( bright_av == ERROR ) return(ERROR);
				if( TWA_Spec == SpecMCL ) {
					bright_low = (bright_av + Bin_Ap_l_ini[TWA_Spec][1])/2;
				} else {
					bright_low = (bright_av + Bin_G2_l_ini[TWA_Spec][1])/2;
				}
				bright_t = bright_n = 0;
				for( n=bright_low; n<=255; n++ ) {
					bright_a = Bright[n] * n;
					bright_t += bright_a;
					bright_n += Bright[n];
				}
				if( bright_n == 0 ) {
					bright_right = bright_low;
					clmin = clmax = bright_right;
				} else {
					bright_r = bright_n/5 + 1;
					//min level
					cl = 0;
					for( n=bright_low; n<255; n++ ) {
						cl += Bright[n];
						if( cl > bright_r ) break;
					}
					clmin = n;
					if( clmin < 10 ) clmin = 10;	//最低レベル 10
					//max level
					cl = 0;
					for( n=255; n>0; n-- ) {
						cl += Bright[n];
						if( cl > bright_r ) break;
					}
					clmax = n;
					bright_right = ( bright_low + clmin )/2;
				}
			//manu
				if( TWA_test != 0 ) {
					R_display_control( BIN_MEMORY | GRAY_MEMORY );
					R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
					R_move_gray_memory( 2, 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
					TWA_pset_histgram( 10, 50, Bright) ;
					R_box( xs, ys, xe, ye, GRAPH_REVERSE, DASHED_LINE );
					WaitMouseClick();
					ys -= offset_y;		ye += offset_y;
					xl = xe-xs+1;		yl = ye-ys+1;
					TWA_gray2bin( bright_right, xs, ys, xl, yl, 2, 0 );
					sprintf( Comment, "C1 RIGHT bright min:%3d  av:%3d  max:%3d", clmin, bright_av, clmax );
					R_chrdisp( 1, 17, Comment );
					sprintf( Comment, "C1 RIGHT bright Insp. level:%3d", bright_right );
					R_chrdisp( 1, 18, Comment );
					WaitMouseClick();
					R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
				}

		//電極平均明るさ
			Bright_C1_left  = bright_left;
			Bright_C1_right = bright_right;
			//manu
				if( TWA_test != 0 ) {
					sprintf( Comment, "Bright  left:%3d  right:%3d", bright_left, bright_right );
					R_chrdisp( 1, 16, Comment );
					WaitMouseClick();
					R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
				}

			return OK;

	}	// TWA_Bright_C1( void ) end

*/

/************************************************
	C1電極幅検査
************************************************/
/*
int		TWA_Judge_C1L( void )
	{
		int				res = ERROR;

		//C1電極幅検査
//			if( TWA_Spec != SpecMCL ) {
//				//（左）
///					res = TWA_Insp_C1_Left();
//					if( res != OK ) return(res);
///				//（右）
//					res = TWA_Insp_C1_Right();
//					if( res != OK ) return(res);
//			}

		// C1電極検査レベル算出
			TWA_Bright_C1();
		// C2電極被り込み幅検査
		//	if( Insp_C2_mode != 0 ) {
			// C2 width Insp.
				res = TWA_Insp_C2_width();
				if( res != OK ) return ERROR2;
		//	}

		return OK;

	}	// TWA_Judge_C1L( void ) end

*/


/************************************************
	標印検査
************************************************/

int		TWA_kensa_me25( void )
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
        int				offset_Mal[3] = { 0, 10, -10 };
		int				nm;

		//標印検査有り無し
			if( TWA_Mark_Insp != 0 ) return(OK);

		//標印文字検査
			if( TWA_Insp_mode == Insp_Pinhole ) return(OK);		//メッキ付着検査

		//標印傾き計算
			deg10 = TWA_katamuki();
			i = (300+deg10)/5;
			if((i<0)||(i>120)) return(ERROR);

		//Mark Level
			ans = ERROR;
			level = 0;
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

		//文字レベル
				ox = oy = 0;
                if(TWA_Mark_level( deg10, ox, oy ) != OK ) {
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
//************* <0000>

//			if(((TWA_R_data[0]==10)&&(TWA_R_data[1]!=1)&&(TWA_R_data[2]!=1)&&(TWA_R_data[3]!=1)) ||		//<0002>
//				((TWA_R_data[0]==10)&&(TWA_R_data[1]==1)&&(TWA_R_data[2]!=1)&&(TWA_R_data[3]!=1))) {		//<0002>
////			if(TWA_R_data[0]==10){
//				off_px = off_py = 1;
//			} else {
//			    off_px = off_py = 1; //<0000>
//			}

//************** <0000>
//			off_px = off_py = 2; //<0000>
            off_px = off_py = 2; //<0000>

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
								break;
							}
						}
//						for( nc=0; nc<TWA_R_n; nc++ ) {
//							Mark_J100_st[nc] = ERROR;
//							Mark_J100_re[nc] = ERROR;
//						}
					}
					if( ans == OK ) {
						break;
					}
				}
				if( ans == OK ) {
					Ma_pos[0] += ox;
					Ma_pos[1] += ox;
					Ma_pos[2] += oy;
					Ma_pos[3] += oy;
					break;
				}
			}
			if( ans != OK ) return(ERROR4);

		return(OK);

	}	// TWA_kensa_m END


/*********************************************************************************
*********************************************************************************/

static void	TWA_G2_level( int *level, int *l_lo, int *l_up )
	{
		int					xs, xe, ys, ye;
		int					cl, clmin, clcen, clmax;
		int					n;
		int					bright_n;
        R_RECT				area;
        int					i_mode	= R_CS_GetInspectMode();
        int					grayPn	= InspGrayPn;
        int					binPn	= InspBinPn;
        int 				elec =0;

		//検査範囲設定
			xs = G2_pos[0] + 10;
			xe = G2_pos[1] - 10;
            ys = G2_pos[2] + 5;
            ye = G2_pos[3] - 5;
            xs	= ElecPosRough[DataLeft][PosXmax]+20;
            xe	= ElecPosRough[DataRight][PosXmin]-20;

		//指定エリアのヒストグラムを取る。（１ドットおきに間引く）
			R_get_histgram( xs, ys, xe, ye, Gray_address[InspGrayPn] );
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
				*l_lo = clcen - cl;				//min level
				*l_up = clcen + cl;				//max level
				if( *l_lo < Bin_Ma_l_ini[TWA_Spec][0] ) *l_lo = Bin_Ma_l_ini[TWA_Spec][0];
				*level = clcen;

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
				TWA_gray2bin( clcen, xs, ys, xe, ye,grayPn, binPn );
			 	R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				R_move_bin_memory( binPn, BinP0, xs, ys, xe, ye);
				sprintf( Comment, "xs=%2d,ys=%2d  xe=%2d,ye=%2d",xs,ys,xe,ye );
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				sprintf(Comment,"clcen = %3d",clcen);
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				sprintf(Comment, "Mark Seg.Bright min:%d  cen:%d  max:%d  n:%d\nMark Seg.Level  min:%d  cen:%d  max:%d\n ", clmin, clcen, clmax, bright_n, *l_lo, clcen, *l_up);
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
	 			R_DrawCls();

				printf( "Mark Seg.Bright min:%d  cen:%d  max:%d  n:%d\n", clmin, clcen, clmax, bright_n );
				printf( "Mark Seg.Level  min:%d  cen:%d  max:%d\n", *l_lo, clcen, *l_up );
			
			}

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
        R_RECT			area;
		int				i_mode	= R_CS_GetInspectMode();
        int				grayPn	= InspGrayPn;
        int				binPn	= InspBinPn;
		int				elec = 0;
//i_mode|=INSPECT_STEP;

			if( TWA_R_n == 3 ) {
				slx2 = 3;
				sly = 3;
				snx = 3;	// 5;
				sny = 3;
			} else {
                slx2 = 5;      //3
				sly = 3;
				snx = 3;
				sny = 3;
			}

//			if( TWA_test == TWA_Ma ) {
//				R_display_control( BIN_MEMORY | GRAY_MEMORY );
//				R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
//				R_move_gray_memory( 2, 0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
//			}

		//標印傾き確認
			i = (300+deg10)/5;
			if((i<0)||(i>120)) return(ERROR);

			for( n=0; n<640; n++ ) {
				xp[n] = 0;
			}
			for( n=0; n<480; n++ ) {
				yp[n] = 0;
			}

		//検査範囲設定
//			xs = (Ap_pos[0]+G2_pos[0])/2 + 5;
//			xe = (Ap_pos[1]+G2_pos[1])/2 - 5;
//			ys = G2_pos[2]/2 + 5 + offset_y;
//			ye = G2_pos[3]/2 - 5 - offset_y;
			xs		= ElecPosRough[DataLeft][2]+10;
			xe		= ElecPosRough[DataRight][0]-10;
			ys		= max(ElecEdge[elec][PosYmin].MeanY-ElecEdge[elec][PosYmin].Rev*(ElecEdge[elec][PosYmin].MeanX-xs),ElecEdge[elec][PosYmin].MeanY-ElecEdge[elec][PosYmin].Rev*(ElecEdge[elec][PosYmin].MeanX-xe))+10+30;
			ye 		= min(ElecEdge[elec][PosYmax].MeanY-ElecEdge[elec][PosYmax].Rev*(ElecEdge[elec][PosYmax].MeanX-xs),ElecEdge[elec][PosYmax].MeanY-ElecEdge[elec][PosYmax].Rev*(ElecEdge[elec][PosYmax].MeanX-xe))-10-30;
			G2_pos[2]	= ys;
            G2_pos[3]	= ye;

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
//					xs1 = G2_pos[0] + 15;
//					xe1 = G2_pos[1] - 15;
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
								y1 = (ys+n*2-2);
								x1 = G2_pos[1] - yp[n];
								x2 = G2_pos[1];
								R_line( x1, y1, x2, y1, GRAPH_DRAW, SOLID_LINE );
							}
					}
				}
				if( yp_n != 0 ) {
//					sly = yp_max/4;
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
//							Ma_pos[2] = (ys+n-1)*2;
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
//							Ma_pos[3] = (ys+n+1)*2;
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
					sprintf( Comment, "Mark Y position" );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				}

			//Y judge
				ans = OK;
				if( Mark_Yseg[n_seg][1] < G2_pos[2] ) ans = ERROR;
				if( Mark_Yseg[n_seg][2] > G2_pos[3] ) ans = ERROR;

				yl = Mark_Yseg[n_seg][2] - Mark_Yseg[n_seg][1];
				if( ans == OK ) {
					if( Mark_area_yl == 0 ) return(ERROR);                       //GEE Program hang
					y = abs( yl - Mark_area_yl )*100/Mark_area_yl;
					if( y < 30 ) {
						Mark_Yseg[n_seg][0] = OK;
						Mark_Yseg[n_seg][3] = level;
						Mark_Yseg[n_seg][4] = y;
					}
				}

				if( i_mode & INSPECT_STEP ) {
					sprintf( Comment, "ans=%d\nok=%d error=%d",ans,OK,ERROR );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
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
					xs = G2_pos[0]+8+offset_x;
					xe = G2_pos[1]-8-offset_x;
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
                    slx = xp_av/3;
//					slx = xp_av/2;
//					slx = xp_av*2/3;
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
//				if( Mark_zure[0] == 0 ) {
					for( n=0; n<xn; n++ ) {
						for( n1=0; n1<snx; n1++ ) {
							if( xp[n+n1] < slx ) break;
						}
						if( n1 == snx ) {
							Mark_Xseg[n_seg][1] = xs+n*4;
//							Ma_pos[0] = xs+n*4;
							break;
						}
					}
					if( n >= xn ) Mark_Xseg[n_seg][1] = ERROR;
//					if( n >= xn ) Ma_pos[0] = ERROR;
//				} else {
//					Ma_pos[0] = Mark_zure[0];
//				}

				if( Mark_Xseg[n_seg][1] != ERROR ) {
					for( ; n>0; n-- ) {
						if( xp[n] < slx2 ) break;
					}
					n++;
					Mark_Xseg[n_seg][1] = xs+n*4;
				}

			//Xri
//				if( Mark_zure[1] == 0 ) {
					for( n=xn-1; n>=0; n-- ) {
						for( n1=0; n1<snx; n1++ ) {
							if( xp[n-n1] < slx ) break;
						}
						if( n1 == snx ) {
							Mark_Xseg[n_seg][2] = xs+n*4;
//							Ma_pos[1] = xs+n*4;
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
				 	sprintf(Comment, "mark pos1" );
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				}
				if( Mark_Xseg[n_seg][2]!=ERROR ) {
					xe = Mark_Xseg[n_seg][2];
					R_line( xe, ys, xe, ye, GRAPH_DRAW, SOLID_LINE );
	 		 		sprintf(Comment, "mark pos2" );
			 		DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				}
			}

			if( Mark_Xseg[n_seg][1] == ERROR ) return(ERROR);
			if( Mark_Xseg[n_seg][2] == ERROR ) return(ERROR);

			x0 = (Mark_Xseg[n_seg][1]+Mark_Xseg[n_seg][2])/2;
			y0 = (Mark_Yseg[n_seg][1]+Mark_Yseg[n_seg][2])/2;

			if( i_mode & INSPECT_STEP ) {
				R_display_control( BIN_MEMORY | GRAY_MEMORY );
				R_bin_cls( 0, 0, FxSize-1, FySize-1 );
				R_move_gray_memory( grayPn, 0, 0, 0, FxSize-1, FySize-1 );
				xs = Mark_Xseg[n_seg][1];
				xe = Mark_Xseg[n_seg][2];
				ys = Mark_Yseg[n_seg][1];
				ye = Mark_Yseg[n_seg][2];
//				R_box( xs, ys, xe, ye, GRAPH_REVERSE, SOLID_LINE );
				area.left	= xs;
			 	area.right	= xe;
			 	area.top	= ys;
			 	area.bottom	= ye;
			 	R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
			 	sprintf(Comment,"box segment");
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				R_DrawCls();

				R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );

				//標印傾き確認
					i = (300+deg10)/5;
				//文字エリア
				for( n=0; n<TWA_R_n; n++ ) {
					xs = x0 + Mark_area_x[i][n][0];
					xe = x0 + Mark_area_x[i][n][3];
					ys = y0 + Mark_area_y[i][n][0];
					ye = y0 + Mark_area_y[i][n][2];
//					R_box( xs, ys, xe, ye, GRAPH_REVERSE, SOLID_LINE );
					area.left	= xs;
				 	area.right	= xe;
				 	area.top	= ys;
				 	area.bottom	= ye;
				 	R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
					sprintf(Comment,"x0 = %2d y0 = %2d \n xs= %2d xe = %2d ys = %2d ye = %2d",x0,y0,xs,xe,ys,ye);
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );

				}
				//文字検査データ
				div_n = Mark_div_x * Mark_div_y;
				for( n=0; n<TWA_R_n; n++ ) {
					for( n1=0; n1<div_n; n1++ ) {
						//検査座標設定
						x = x0 + Mark_rx[i][n][n1];
						y = y0 + Mark_ry[i][n][n1];
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
					sprintf(Comment,"in box");
					DrawMessage( Comment, DRAW_GREEN, 2, 2 );
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

			if((Mark_Xseg[n_seg][0]!=OK)||(Mark_Yseg[n_seg][0]!=OK)) return(ERROR);

		return(OK);

	}	//TWA_segmentation End

//////////////////////////////////////////////////

static int	TWA_Mark_level( int deg10, int ox, int oy )
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
        int					grayPn	= InspGrayPn;

		//上下左右ズレ
			mark_zure1 = ERROR;
			mark_zure2 = ERROR;
			mark_zure3 = ERROR;
			mark_zure4 = ERROR;

		//Manual
			if( i_mode & INSPECT_STEP ) {
				R_display_control( BIN_MEMORY | GRAY_MEMORY );
				R_bin_cls( 0, 0, FxSize-1, FySize-1 );
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
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
				//指定エリアのヒストグラムを取る。（１ドットおきに間引く）
//					R_get_step_histgram( xs, ys, xe, ye, Gray_address[2] );
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
					Mark_Slevel_s[cn][1] = (clmin+clmax)/2;				//center level
					Mark_Slevel_s[cn][0] = Mark_Slevel_s[cn][1] - cl - 10;		//min level
					Mark_Slevel_s[cn][2] = Mark_Slevel_s[cn][1] + cl;		//max level
					if( Mark_Slevel_s[cn][0] < 10 ) Mark_Slevel_s[cn][0] = 10;
				//Manual
					if( i_mode & INSPECT_STEP ) {
						TWA_gray2bin( Mark_Slevel_s[cn][1], xs, ys, xe-xs+1, ye-ys+1, grayPn, 0 );
//						R_gray2bin( Mark_Slevel_s[cn][1], xs, ys, xe-xs+1, ye-ys+1, Gray_address[2], Bin_address[0] );
//						R_box( xs, ys, xe, ye, GRAPH_REVERSE, SOLID_LINE );
						area.left	= xs;
					 	area.right	= xe;
					 	area.top	= ys;
					 	area.bottom	= ye;
					 	R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
						R_DrawCls();
						printf( "Mark %d Bright Lo:%d  Hi:%d  n:%d\n", cn, clmin, clmax, bright_n );
						printf( "Mark %d Level min:%d  max:%d\n", cn, Mark_Slevel_s[cn][0], Mark_Slevel_s[cn][2] );
					}
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
				//指定エリアのヒストグラムを取る。（１ドットおきに間引く）
//					R_get_step_histgram( xs, ys, xe, ye, Gray_address[2] );
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
					Mark_Slevel_r[cn][1] = (clmin+clmax)/2;				//center level
					Mark_Slevel_r[cn][0] = Mark_Slevel_r[cn][1] - cl - 10;		//min level
					Mark_Slevel_r[cn][2] = Mark_Slevel_r[cn][1] + cl;		//max level
					if( Mark_Slevel_r[cn][0] < 10 ) Mark_Slevel_r[cn][0] = 10;
				//Manual
					if( i_mode & INSPECT_STEP ) {
						TWA_gray2bin( Mark_Slevel_r[cn][1], xs, ys, xe-xs+1, ye-ys+1, grayPn, 0 );
//						R_gray2bin( Mark_Slevel_r[cn][1], xs, ys, xe-xs+1, ye-ys+1, Gray_address[2], Bin_address[0] );
//						R_box( xs, ys, xe, ye, GRAPH_REVERSE, SOLID_LINE );
						area.left	= xs;
					 	area.right	= xe;
					 	area.top	= ys;
					 	area.bottom	= ye;
					 	R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
						printf( "Mark R %d Bright Lo:%d  Hi:%d  n:%d\n", cn, clmin, clmax, bright_n );
						printf( "Mark R %d Level min:%d  max:%d\n", cn, Mark_Slevel_r[cn][0], Mark_Slevel_r[cn][2] );
					}
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
					top_length   = mark_top - (Elec[DataLR].CornerPosY[0]+Elec[DataLR].CornerPosY[2])/2;
					bottom_length= (Elec[DataLR].CornerPosY[1]+Elec[DataLR].CornerPosY[3])/2 -mark_bottom;

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

            if( i_mode & INSPECT_STEP ) {
                sprintf(Comment,"TWA mark level");
				DrawMessage( Comment, DRAW_GREEN, 2, 2 );
				R_DrawCls();
			}

        return(OK);
	}
/*********************************************************************************
	関数名：	ＴＷＡ＿Ｍａｒｋ＿ｐｏｉｎｔ（ int deg10, int ox, int oy, int ol )
    機　能：　	標印データ読み取り
	形　式：	void	TWA_Mark_point( int deg10, int ox, int oy, int ol )
	引き数：	int		deg10		文字角度データ(10倍)
				int		ox
				int		oy
				int		ol
	戻り値：	値			定　数			意　味
				0			OK				正常に文字切り出しが完了しました。
				-1			ERROR			異常終了しました。文字の切り出しが出来なかった。
	解　説：	標印文字の切り出しを行う。
	留意事項：	
*********************************************************************************/

int		TWA_Mark_point_10_25( int deg10, int ox, int oy, int ol )
	{
		int				x0, y0, x0s, x0r;
		int				xs, xe, ys, ye;
		int				div_n;
		int				nc, nd, i;
		int				x, y;
		int				nx, ny;
		int				pt[8];
		int				jw, jb;
		int				ls, le;
		int				ans;
		R_RECT			area;
		int				i_mode	= R_CS_GetInspectMode();
		int				grayPn	= InspGrayPn;
		int				binPn	= InspBinPn;

		//標印傾き確認
			i = (300+deg10)/5;

		//文字エリア中心
			x0 = (Ma_pos[0]+Ma_pos[1])/2;
			y0 = (Ma_pos[2]+Ma_pos[3])/2;
			x0s = x0r = x0 + ox;
			y0 += oy;
		//180525 if(bIsESR25 || bIsMCR18 || bIsSFR25 || bIsLTR50 || bIsLTR18){		//v2.20		// MCR10では1の根本の拡がり部分でズレが抑止される為補正を外す 20170608 tanaka
		if(bIsESR25 || bIsMCR18 || bIsSFR25 || bIsLTR50){		//v2.20		// MCR10では1の根本の拡がり部分でズレが抑止される為補正を外す 20170608 tanaka
			if( TWA_R_n == 3 ) {
                if( MarkChar[0] == '1' ) {
                    x0s -= 6;
                    x0r += 6;
				}
                if( MarkChar[2] == '1' ) {
                    x0s += 6;
                    x0r -= 6;
				}
		} else {
                if( MarkChar[0] == '1' ) {
                    x0s -= 4;
                    x0r += 4;
				}
                if( MarkChar[3] == '1' ) {
                    x0s += 4;
                    x0r -= 4;
				}
			}
		}
			if( i_mode & INSPECT_STEP ) {
				//文字エリア
				for( nc=0; nc<TWA_R_n; nc++ ) {
                    area.left	= x0s + Mark_area_x[i][nc][0];
                    area.right	= x0s + Mark_area_x[i][nc][3];
                    area.top	= y0  + Mark_area_y[i][nc][0];
                    area.bottom	= y0  + Mark_area_y[i][nc][2];
				 	R_DrawBox( DRAW_RED, area, GRAPH_DRAW, SOLID_LINE );
				}
			}

            xs = G2_pos[0]-10;
            xe = G2_pos[1]+10;
            ys = G2_pos[2]-10;
            ye = G2_pos[3]+10;
			ans = OK;
			for( nc=0; nc<TWA_R_n; nc++ ) {
				mox[nc] = x0;
				moy[nc] = y0;
				for( nd=0; nd<4; nd++ ) {
					x = x0 + Mark_area_x[i][nc][nd];
					y = y0 + Mark_area_y[i][nc][nd];
					if((x<xs)||(x>xe)) ans = ERROR;
					if((y<ys)||(y>ye)) ans = ERROR;
				}
			}
			if( ans != OK ) {
//				if( TWA_test == TWA_Ma ) {
//					WaitMouseClick();
//				}
				return(ERROR);
			}

	 		if( i_mode & INSPECT_STEP ) {
				R_display_control( BIN_MEMORY | GRAY_MEMORY );
				R_bin_cls( 0, 0, FxSize-1, FySize-1 );
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
			}

		//文字データ読み取り
			div_n = Mark_div_x * Mark_div_y;
			for( nc=0; nc<TWA_R_n; nc++ ) {
				for( ny=0; ny<Mark_div_y; ny++ ) {
					for( nx=0; nx<Mark_div_x; nx++ ) {
						nd = Mark_div_x*ny+nx;
						//検査座標設定
						x = Mark_rx[i][nc][nd];
						y = Mark_ry[i][nc][nd];
						pt[0] = x + x0s;						//X center
						pt[1] = y + y0;							//Y center
						pt[2] = Mark_Slevel_s[nc][0] + ol;		//White level
						if( pt[2] < 10 ) pt[2] = 10;
						pt[3] = Mark_Slevel_s[nc][2] + ol;		//Black level
						if( pt[3] < 10 ) pt[3] = 10;
						//範囲内の白／黒を数える
						TWA_get_color_area( grayPn, pt, &jw, &jb );
						Mark_div_ws[nc][nd] = jw;
						Mark_div_bs[nc][nd] = jb;
						//検査座標設定（１８０°反転）
						x = 0 - Mark_rx[i][nc][nd];
						y = 0 - Mark_ry[i][nc][nd];
						pt[0] = x + x0r;						//X center
						pt[1] = y + y0;							//Y center
						pt[2] = Mark_Slevel_r[nc][0] + ol;		//White level
						if( pt[2] < 10 ) pt[2] = 10;
						pt[3] = Mark_Slevel_r[nc][2] + ol;		//Black level
						if( pt[3] < 10 ) pt[3] = 10;
						//範囲内の白／黒を数える
                        TWA_get_color_area( grayPn, pt, &jw, &jb );
                        Mark_div_wr[nc][nd] = jw;
						Mark_div_br[nc][nd] = jb;
						/*
						//Test
						if( i_mode & INSPECT_STEP ) {
						//検査座標設定
							x = Mark_rx[i][nc][nd];
							y = Mark_ry[i][nc][nd];
							x += x0s;
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
						*/
					}
					//標印Size
						nd = Mark_div_x*ny;
						pt[0] = Mark_rx[i][nc][nd] + x0s;
						pt[1] = Mark_ry[i][nc][nd] + y0;
						nd = Mark_div_x*(ny+1)-1;
						pt[2] = Mark_rx[i][nc][nd] + x0s;
						pt[3] = Mark_ry[i][nc][nd] + y0;
						pt[4] = Mark_Slevel_s[nc][0] + ol;		//White level
						if( pt[4] < 10 ) pt[4] = 10;
						TWA_Mark_Size_E25( grayPn, pt, &ls, &le );
						Mark_size_ls[nc][ny][0] = ls;
						Mark_size_ls[nc][ny][1] = le;
					//標印Size（１８０°反転）
						nd = Mark_div_x*ny;
						pt[2] = 0 - Mark_rx[i][nc][nd] + x0r;
						pt[3] = 0 - Mark_ry[i][nc][nd] + y0;
						nd = Mark_div_x*(ny+1)-1;
						pt[0] = 0 - Mark_rx[i][nc][nd] + x0r;
						pt[1] = 0 - Mark_ry[i][nc][nd] + y0;
						pt[4] = Mark_Slevel_r[nc][0] + ol;		//White level
						if( pt[4] < 10 ) pt[4] = 10;
						TWA_Mark_Size_E25( grayPn, pt, &ls, &le );
						Mark_size_lr[nc][ny][0] = ls;
						Mark_size_lr[nc][ny][1] = le;
                        if( i_mode & INSPECT_STEP ) {
							printf( "Mark Size %d-%d:%3d-%3d  %3d-%3d\n", nc, ny,
										Mark_size_ls[nc][ny][0], Mark_size_ls[nc][ny][1],
										Mark_size_lr[nc][ny][0], Mark_size_lr[nc][ny][1] );
					}
				}
			}
//            if( i_mode & INSPECT_STEP ) {
//                sprintf(Comment,"MARKING");
//                DrawMessage( Comment, DRAW_GREEN, 2, 2 );
//                R_DrawCls();
//
//            }

		//Test
			if( i_mode & INSPECT_STEP ) {
				for( nc=0; nc<TWA_R_n; nc++ ) {
					for( ny=0; ny<Mark_div_y; ny++ ) {
						for( nx=0; nx<Mark_div_x; nx++ ) {
							nd = Mark_div_x*ny+nx;
							//検査座標設定
							x = Mark_rx[i][nc][nd];
							y = Mark_ry[i][nc][nd];
							x += x0s;
							y += y0;
							xs = x - 5;
							xe = x + 4;
							ys = y - 5;
							ye = y + 4;
							if( Mark_div_ws[nc][nd] == 1 ) {
								R_box( xs, ys, xe, ye, GRAPH_DRAW, SOLID_LINE );
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
					}
				}
				WaitMouseClick();
				R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );

				for( nc=0; nc<TWA_R_n; nc++ ) {
					for( ny=0; ny<Mark_div_y; ny++ ) {
						for( nx=0; nx<Mark_div_x; nx++ ) {
							nd = Mark_div_x*ny+nx;
							//検査座標設定（１８０°反転）
							x = 0 - Mark_rx[i][nc][nd];
							y = 0 - Mark_ry[i][nc][nd];
							x += x0r;
							y += y0;
							xs = x - 4;
							xe = x + 5;
							ys = y - 4;
							ye = y + 5;
							if( Mark_div_wr[nc][nd] == 1 ) {
								R_box( xs, ys, xe, ye, GRAPH_DRAW, SOLID_LINE );
							}
							if( Mark_div_br[nc][nd] == 0 ) {
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
					}
				}
//                sprintf(Comment,"MARKING");
//                DrawMessage( Comment, DRAW_GREEN, 2, 2 );
//                R_DrawCls();

			}





		//標印文字分割Data(Bin逆方向)
//			div_n = Mark_div_x * Mark_div_y;
//			for( nc=0; nc<TWA_R_n; nc++ ) {
//				for( nd=0, ndr=(div_n-1); nd<div_n; nd++, ndr-- ) {
//					Mark_div_wr[nc][ndr] = Mark_div_ws[nc][nd];
//					Mark_div_br[nc][ndr] = Mark_div_bs[nc][nd];
//				}
//				for( nd=0, ndr=(Mark_div_y-1); nd<Mark_div_y; nd++, ndr-- ) {
//					Mark_size_lr[nc][ndr][0] = Mark_size_ls[nc][nd][1];
//					Mark_size_lr[nc][ndr][1] = Mark_size_ls[nc][nd][0];
//				}
//			}

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

			if( TWA_Insp_mode != Insp_Pinhole ) judge = InspParaM.MarkLevel[TWA_Spec].nData1;      // 60 GEE 60 to 80
			else								judge = InspParaM.MarkLevel[TWA_Spec].nData2;      // 80 GEE 80 to 95

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
					else                       Mark_L_set[1] = 1;
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
				for( nm=0; nm<11; nm++ ) {
					c1st = c1re = c1sum = 0;
					c2st_t = c2re_t = c2sum_t = 0;
					c2st_w = c2re_w = c2sum_w = 0;
					c2st_b = c2re_b = c2sum_b = 0;
						if( TWA_R_n == 3 ) {
							for( ny=0; ny<Mark_div_y; ny++ ) {
								for( nx=0; nx<Mark_div_x; nx++ ) {
									nd = ny*Mark_div_x + nx;
									pt[0] = nc;												//桁
									pt[1] = (Mark_Check_3[nm][nd]>>TWA_Mark_co ) & 0x0f;	//Color
									pt[3] = nx;												//Check 1
									pt[4] = ny;												//Check 1
									//Check 1
									pt[2] = (Mark_Check_3[nm][nd]>>TWA_Mark_ch1) & 0x0f;	//Check1
									pt[5] = (Mark_Check_3[nm][nd]>>TWA_Mark_oX1) & 0x0f;	//or X1
									pt[6] = (Mark_Check_3[nm][nd]>>TWA_Mark_oY1) & 0x0f;	//or Y1
									if( pt[2] != 0 ) TWA_Mark_div_check( pt, &c1st, &c1re, &c1sum );
									//Check 2
									pt[2] = (Mark_Check_3[nm][nd]>>TWA_Mark_ch2) & 0x0f;	//Check2
									pt[5] = (Mark_Check_3[nm][nd]>>TWA_Mark_oX2) & 0x0f;	//or X2
									pt[6] = Mark_Check_3[nm][nd] & 0x0f;					//or Y2
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
							for( ny=0; ny<Mark_div_y; ny++ ) {
								for( nx=0; nx<Mark_div_x; nx++ ) {
									nd = ny*Mark_div_x + nx;
									pt[0] = nc;												//桁
									pt[1] = (Mark_Check_4[nm][nd]>>TWA_Mark_co ) & 0x0f;	//Color
									pt[3] = nx;												//Check X
									pt[4] = ny;												//Check Y
									//Check 1
									pt[2] = (Mark_Check_4[nm][nd]>>TWA_Mark_ch1) & 0x0f;	//Check1
									pt[5] = (Mark_Check_4[nm][nd]>>TWA_Mark_oX1) & 0x0f;	//or X1
									pt[6] = (Mark_Check_4[nm][nd]>>TWA_Mark_oY1) & 0x0f;	//or Y1
									if( pt[2] != 0 ) TWA_Mark_div_check( pt, &c1st, &c1re, &c1sum );
									//Check 2
									pt[2] = (Mark_Check_4[nm][nd]>>TWA_Mark_ch2) & 0x0f;	//Check2
									pt[5] = (Mark_Check_4[nm][nd]>>TWA_Mark_oX2) & 0x0f;	//or X2
									pt[6] = Mark_Check_4[nm][nd] & 0x0f;					//or Y2
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
					c2st_t  = c2st_w  + c2st_b;
					c2re_t  = c2re_w  + c2re_b;
					c2sum_t = c2sum_w + c2sum_b;
/********************* //<0000>
					if( c2sum_t == 0 ) {
						Mark_Judge_st[nc][nm][0] = 0;
						Mark_Judge_re[nc][nm][0] = 0;
					} else {
						Mark_Judge_st[nc][nm][0] = c2st_t*100/c2sum_t;
						Mark_Judge_re[nc][nm][0] = c2re_t*100/c2sum_t;
					}
					Mark_Judge_st[nc][nm][1] = OK;
					Mark_Judge_re[nc][nm][1] = OK;
					if( c1sum == 0 ) {
						Mark_Judge_st[nc][nm][2] = 0;
						Mark_Judge_re[nc][nm][2] = 0;
					} else {
						Mark_Judge_st[nc][nm][2] = c1st*100/c1sum;
						Mark_Judge_re[nc][nm][2] = c1re*100/c1sum;
					}
					if( c2sum_w == 0 ) {
						Mark_Judge_st[nc][nm][3] = 0;
						Mark_Judge_re[nc][nm][3] = 0;
					} else {
						Mark_Judge_st[nc][nm][3] = c2st_w*100/c2sum_w;
						Mark_Judge_re[nc][nm][3] = c2re_w*100/c2sum_w;
					}
					if( c2sum_b == 0 ) {
						Mark_Judge_st[nc][nm][4] = 0;
						Mark_Judge_re[nc][nm][4] = 0;
					} else {
						Mark_Judge_st[nc][nm][4] = c2st_b*100/c2sum_b;
						Mark_Judge_re[nc][nm][4] = c2re_b*100/c2sum_b;
					}
*********************************/ //<0000>

//*************** <0000>
					if( c2sum_t <= 0 ) break;                               //2017.03.19 tanaka Program hang from MCR03 GEE's
					if( c1sum <= 0 )   break;                               //2017.03.19 tanaka Program hang from MCR03 GEE's
					if( c2sum_w <= 0 ) break;                               //2017.03.19 tanaka Program hang from MCR03 GEE's
					if( c2sum_b <= 0 ) break;                               //2017.03.19 tanaka Program hang from MCR03 GEE's
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
//*************** <0000>
				}
				TWA_Mark_error( nc );
			}


			for( nc=0; nc<TWA_R_n; nc++ ) {
				for( nm=0; nm<11; nm++ ) {
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

					for( nm=0; nm<11; nm++ ) {
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
//						printf( "Mst:%d  Judge:%d : %d\n", nm,
//										Mark_Judge_st[nc][nm][0],
//										Mark_Judge_st[nc][nm][1] );
//						printf( "Mre:%d  Judge:%d : %d\n", nm,
//										Mark_Judge_re[nc][nm][0],
//										Mark_Judge_re[nc][nm][1] );
//						WaitMouseClick();
					}
				}


				for( nc=0; nc<TWA_R_n; nc++ ) {
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
				}

#ifdef Mark_Debug_E25

            if( i_mode & INSPECT_STEP ) {
                int posi_res_x,posi_res_y;
				R_display_control( BIN_MEMORY | GRAY_MEMORY );
                R_bin_cls( 0, 0, FxSize-1, FySize-1 );
                R_move_gray_memory( grayPn, GrayP0, 0, 0, FxSize-1, FySize-1 );
				for( nc=0; nc<TWA_R_n; nc++ ) {
					if( TWA_Spec != SpecJPW ) {
                        posi_res_x = nc * 31;
                        posi_res_y = 30;
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"Rst : %d",nc);
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
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+6 ,"%2d:%3d(%2d)%2d max:%2d(%3d)",
                                    so_st[nc][10][0], so_st[nc][10][1], so_st[nc][10][2], so_st[nc][10][3],
                                    max_st[nc][1], max_st[nc][0] );

//                        sprintf( Comment, "Rst : %d \n %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d \n %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d \n %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d \n %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d max:%2d(%3d)",
//								nc, so_st[nc][0][0], so_st[nc][0][1], so_st[nc][0][2], so_st[nc][0][3],
//									so_st[nc][1][0], so_st[nc][1][1], so_st[nc][1][2], so_st[nc][1][3],
//									so_st[nc][2][0], so_st[nc][2][1], so_st[nc][2][2], so_st[nc][2][3],
//									so_st[nc][3][0], so_st[nc][3][1], so_st[nc][3][2], so_st[nc][3][3],
//									so_st[nc][4][0], so_st[nc][4][1], so_st[nc][4][2], so_st[nc][4][3],
//									so_st[nc][5][0], so_st[nc][5][1], so_st[nc][5][2], so_st[nc][5][3],
//									so_st[nc][6][0], so_st[nc][6][1], so_st[nc][6][2], so_st[nc][6][3],
//									so_st[nc][7][0], so_st[nc][7][1], so_st[nc][7][2], so_st[nc][7][3],
//									so_st[nc][8][0], so_st[nc][8][1], so_st[nc][8][2], so_st[nc][8][3],
//									so_st[nc][9][0], so_st[nc][9][1], so_st[nc][9][2], so_st[nc][9][3],
//									so_st[nc][10][0], so_st[nc][10][1], so_st[nc][10][2], so_st[nc][10][3],
//									max_st[nc][1], max_st[nc][0] );
//						DrawMessage( Comment, DRAW_GREEN, 2, 2);
//						R_DrawCls();

						ncr = TWA_R_n - nc - 1;
                        posi_res_x = ncr * 31;
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+7 ,"Rre : %d",ncr);
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+8 ,"%2d:%3d(%2d)%2d %2d:%3d(%2d)%2d",
                                    so_re[ncr][0][0], so_re[ncr][0][1], so_re[ncr][0][2], so_re[ncr][0][3],
                                    so_re[ncr][1][0], so_re[ncr][1][1], so_re[ncr][1][2], so_re[ncr][1][3]);
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+9 ,"%2d:%3d(%2d)%2d %2d:%3d(%2d)%2d",
                                    so_re[ncr][2][0], so_re[ncr][2][1], so_re[ncr][2][2], so_re[ncr][2][3],
                                    so_re[ncr][3][0], so_re[ncr][3][1], so_re[ncr][3][2], so_re[ncr][3][3]);
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+10 ,"%2d:%3d(%2d)%2d %2d:%3d(%2d)%2d",
                                    so_re[ncr][4][0], so_re[ncr][4][1], so_re[ncr][4][2], so_re[ncr][4][3],
                                    so_re[ncr][5][0], so_re[ncr][5][1], so_re[ncr][5][2], so_re[ncr][5][3]);
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+11 ,"%2d:%3d(%2d)%2d %2d:%3d(%2d)%2d",
                                    so_re[ncr][6][0], so_re[ncr][6][1], so_re[ncr][6][2], so_re[ncr][6][3],
                                    so_re[ncr][7][0], so_re[ncr][7][1], so_re[ncr][7][2], so_re[ncr][7][3]);
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+12 ,"%2d:%3d(%2d)%2d %2d:%3d(%2d)%2d",
                                    so_re[ncr][8][0], so_re[ncr][8][1], so_re[ncr][8][2], so_re[ncr][8][3],
                                    so_re[ncr][9][0], so_re[ncr][9][1], so_re[ncr][9][2], so_re[ncr][9][3]);
                        R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+13 ,"%2d:%3d(%2d)%2d max:%2d(%3d)",
                                    so_re[ncr][10][0],so_re[ncr][10][1],so_re[ncr][10][2],so_re[ncr][10][3],
                                    max_re[ncr][1], max_re[ncr][0] );


//                        sprintf( Comment, "Rre : %d \n %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d \n %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d \n %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d \n %2d:%3d(%2d)%2d %2d:%3d(%2d)%2d max:%2d(%3d)",
//								nc, so_re[ncr][0][0], so_re[ncr][0][1], so_re[ncr][0][2], so_re[ncr][0][3],
//									so_re[ncr][1][0], so_re[ncr][1][1], so_re[ncr][1][2], so_re[ncr][1][3],
//									so_re[ncr][2][0], so_re[ncr][2][1], so_re[ncr][2][2], so_re[ncr][2][3],
//									so_re[ncr][3][0], so_re[ncr][3][1], so_re[ncr][3][2], so_re[ncr][3][3],
//									so_re[ncr][4][0], so_re[ncr][4][1], so_re[ncr][4][2], so_re[ncr][4][3],
//									so_re[ncr][5][0], so_re[ncr][5][1], so_re[ncr][5][2], so_re[ncr][5][3],
//									so_re[ncr][6][0], so_re[ncr][6][1], so_re[ncr][6][2], so_re[ncr][6][3],
//									so_re[ncr][7][0], so_re[ncr][7][1], so_re[ncr][7][2], so_re[ncr][7][3],
//									so_re[ncr][8][0], so_re[ncr][8][1], so_re[ncr][8][2], so_re[ncr][8][3],
//									so_re[ncr][9][0], so_re[ncr][9][1], so_re[ncr][9][2], so_re[ncr][9][3],
//									so_re[ncr][10][0], so_re[ncr][10][1], so_re[ncr][10][2], so_re[ncr][10][3],
//									max_re[ncr][1], max_re[ncr][0] );

//						DrawMessage( Comment, DRAW_GREEN, 2, 2);
					}
                }
				WaitMouseClick();
                R_DrawCls();
            }
#endif

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
//					if( Mark_Ans_st[nc] == OK ) continue;
					jm = TWA_R_data[nc];
					if( Mark_L_set[nc] != 0 ) {
						if( TWA_R_data[nc] != max_st[nc][1] ) break;
						if( max_st[nc][0] < judge ) break;
						if( Mark_Judge_st[nc][jm][3] < judge ) break;
					} else {
//						if( TWA_R_data[nc] != Mark_J100_st[nc] ) {
//							if( Mark_Judge_st[nc][jm][2] < 100 ) break;
							if( TWA_R_data[nc] != max_st[nc][1] ) break;
							if( max_st[nc][0] < judge ) break;
//						}
					}
					Mark_Ans_st[nc] = OK;
				}
				if( nc == TWA_R_n ) return(OK);
			}

			if( insp_re == OK ) {
				for( nc=0; nc<TWA_R_n; nc++ ) {
//					if( Mark_Ans_re[nc] == OK ) continue;
					jm = TWA_R_data[nc];
					if( Mark_L_set[nc] != 0 ) {
						if( TWA_R_data[nc] != max_re[nc][1] ) break;
						if( max_re[nc][0] < judge ) break;
						if( Mark_Judge_re[nc][jm][3] < judge ) break;
					} else {
//						if( TWA_R_data[nc] != Mark_J100_re[nc] ) {
//							if( Mark_Judge_re[nc][jm][2] < 100 ) break;
							if( TWA_R_data[nc] != max_re[nc][1] ) break;
							if( max_re[nc][0] < judge ) break;
//						}
					}
					Mark_Ans_re[nc] = OK;
				}
				if( nc == TWA_R_n ) return(OK);
			}

		return(ERROR);

	}

static void	TWA_Mark_error( int nc )
	{
		int				nst, nre;
//		int				ds1, ds2, ds3, dr1, dr2, dr3;
		int				ds1, ds2, dr1, dr2;
		int				nm;
		int				pt[55];
		int				rp = 0;
//		int				n;

				for( nm=0; nm<11; nm++ ) {
					Mark_Judge_st[nc][nm][1] = OK;
					Mark_Judge_re[nc][nm][1] = OK;
				}

				//0
                nm = 0;
                if( TWA_R_n == 3 ) {
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
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst>=1 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre>=1 ) Mark_Judge_re[nc][nm][1] = ERROR;

					// Yogore 20170323
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 1;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst<=4 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre<=4 ) Mark_Judge_re[nc][nm][1] = ERROR;

					// Kasure 20170323
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 0;
					pt[rp++] = 0;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst>=3 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre>=3 ) Mark_Judge_re[nc][nm][1] = ERROR;

					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 1;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst<=4 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre<=4 ) Mark_Judge_re[nc][nm][1] = ERROR;

					// Kasure 20170323
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 0;
					pt[rp++] = 2;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst>=3 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre>=3 ) Mark_Judge_re[nc][nm][1] = ERROR;

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
            }else{
                    //4桁
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
                    if( nst>=1 ) Mark_Judge_st[nc][nm][1] = ERROR;          // 20170410     tanaka
                    if( nre>=1 ) Mark_Judge_re[nc][nm][1] = ERROR;          // 20170410     tanaka

                    rp = 0;
                    pt[rp++] = nc;	pt[rp++] = 1;	//Bcack
                    pt[rp++] = 1;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 1;
                    pt[rp++] = 1;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 1;
                    pt[rp++] = 1;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 1;
                    pt[rp++] = 1;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 1;
                    pt[rp++] = 1;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 1;
                    pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
                    TWA_Mark_error_check( pt, &nst, &nre );
                    if( nst>=2 ) Mark_Judge_st[nc][nm][1] = ERROR;          // 20170409 緩和 tanaka
                    if( nre>=2 ) Mark_Judge_re[nc][nm][1] = ERROR;          // 20170409     tanaka

                    // Yogore 20170323
                    rp = 0;
                    pt[rp++] = nc;	pt[rp++] = 1;
                    pt[rp++] = 0;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 1;
                    pt[rp++] = 0;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 1;
                    pt[rp++] = 0;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 1;
                    pt[rp++] = 0;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 1;
                    pt[rp++] = 0;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 1;
                    pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
                    TWA_Mark_error_check( pt, &nst, &nre );
                    if( nst<=3 ) Mark_Judge_st[nc][nm][1] = ERROR;          // 20170409 緩和 tanaka
                    if( nre<=3 ) Mark_Judge_re[nc][nm][1] = ERROR;

                    // Kasure 20170323
/*                    rp = 0;
                    pt[rp++] = nc;	pt[rp++] = 0;
                    pt[rp++] = 0;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 0;
                    pt[rp++] = 0;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 0;
                    pt[rp++] = 0;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 0;
                    pt[rp++] = 0;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 0;
                    pt[rp++] = 0;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 0;
                    pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
                    TWA_Mark_error_check( pt, &nst, &nre );
                    if( nst>=2 ) Mark_Judge_st[nc][nm][1] = ERROR;
                    if( nre>=2 ) Mark_Judge_re[nc][nm][1] = ERROR;
*/
                    rp = 0;
                    pt[rp++] = nc;	pt[rp++] = 1;
                    pt[rp++] = 2;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 1;
                    pt[rp++] = 2;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 1;
                    pt[rp++] = 2;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 1;
                    pt[rp++] = 2;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 1;
                    pt[rp++] = 2;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 1;
                    pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
                    TWA_Mark_error_check( pt, &nst, &nre );
                    if( nst<=3 ) Mark_Judge_st[nc][nm][1] = ERROR;          // 20170409 緩和 tanaka
                    if( nre<=3 ) Mark_Judge_re[nc][nm][1] = ERROR;

                    // Kasure 20170323
 /*                   rp = 0;
                    pt[rp++] = nc;	pt[rp++] = 0;
                    pt[rp++] = 2;	pt[rp++] = Mark_div_y/2-2;	pt[rp++] = 0;
                    pt[rp++] = 2;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 0;
                    pt[rp++] = 2;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 0;
                    pt[rp++] = 2;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 0;
                    pt[rp++] = 2;	pt[rp++] = Mark_div_y/2+2;	pt[rp++] = 0;
                    pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
                    TWA_Mark_error_check( pt, &nst, &nre );
                    if( nst>=2 ) Mark_Judge_st[nc][nm][1] = ERROR;
                    if( nre>=2 ) Mark_Judge_re[nc][nm][1] = ERROR;
*/
                //	rp = 0;
                //	pt[rp++] = nc;	pt[rp++] = 1;
                //	pt[rp++] = 0;	pt[rp++] = Mark_div_y-1;	pt[rp++] = 1;
                //	pt[rp++] = 1;	pt[rp++] = Mark_div_y-1;	pt[rp++] = 1;
                //	pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
                //	TWA_Mark_error_check( pt, &ds1, &dr1 );
                //	rp = 0;
                //	pt[rp++] = nc;	pt[rp++] = 1;
                //	pt[rp++] = 1;	pt[rp++] = Mark_div_y-2;	pt[rp++] = 1;
                //	pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
                //	TWA_Mark_error_check( pt, &ds2, &dr2 );
                //	if((ds1==0)&&(ds2==1)) Mark_Judge_st[nc][nm][1] = ERROR;
                //	if((dr1==0)&&(dr2==1)) Mark_Judge_re[nc][nm][1] = ERROR;
                }

					//0 End
				//1
					nm = 1;
					if( TWA_R_n == 3 ) {
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
						pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst <= 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre <= 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					} else {
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
						pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst <= 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre <= 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					}
						//1 End
				//2
					nm = 2;
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

					// Yogore
					rp = 0;
					pt[rp++] = nc;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;
					pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 1;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst >= 1 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre >= 1 ) Mark_Judge_re[nc][nm][1] = ERROR;


					if( TWA_R_n == 3 ) {
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;
						pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 0;
						pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

					} else {
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;
						pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 0;
						pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

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

					if( TWA_R_n == 3 ) {
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

					} else {
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

					}
					//3 End
				//4
					nm = 4;
					if( TWA_R_n == 3 ) {
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 0;
						pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;
						pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 0;
						pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 0;
						pt[rp++] = 0;	pt[rp++] = 0;	pt[rp++] = 0;
						pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 0;
						pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;


/* ZZ
						// 20170424 追加 2のニジミをOKとしてしまう
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 3;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
 ZZ */

						// 20170509 追加 2のニジミをOKとしてしまう
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;
						pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;
						pt[rp++] = 0;	pt[rp++] = 3;	pt[rp++] = 1;
						pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 1;
						pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 1;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					} else {
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 0;
						pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 0;
						pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;
						pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 0;
						pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 0;
						pt[rp++] = 0;	pt[rp++] = 6;	pt[rp++] = 0;
						pt[rp++] = 1;	pt[rp++] = 6;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;
					}
					//4 End
				//5
					nm = 5;
					if( TWA_R_n == 3 ) {
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 0;
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
					} else {
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 0;
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
					}
					//5 End
				//6
					nm = 6;
					if( TWA_R_n == 3 ) {
						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 1;
						pt[rp++] = 0;	pt[rp++] = 2;	pt[rp++] = 1;
						pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;
						pt[rp++] = 1;	pt[rp++] = 0;	pt[rp++] = 1;
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
					} else {
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
					}
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

					if( TWA_R_n == 3 ) {
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
						pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					} else {
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
						pt[rp++] = 2;	pt[rp++] = 0;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 1;	pt[rp++] = 1;
						pt[rp++] = 2;	pt[rp++] = 2;	pt[rp++] = 1;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;

						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 4;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 5;	pt[rp++] = 0;
						pt[rp++] = 2;	pt[rp++] = 6;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst < 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre < 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

					}
					//7 End
				//8
					nm = 8;
					if( TWA_R_n == 3 ) {
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

					} else {
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

                        rp = 0;                                        // 異物 bin level 異常で黒が無かったらエラー 20170409 tanaka
                        pt[rp++] = nc;	pt[rp++] = 1;
                        pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 1;
                        pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 1;
                        pt[rp++] = 1;	pt[rp++] = 4;	pt[rp++] = 1;
                        pt[rp++] = 1;	pt[rp++] = 5;	pt[rp++] = 1;
                        pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
                        TWA_Mark_error_check( pt, &nst, &nre );
                        if( nst >= 2 ) Mark_Judge_st[nc][nm][1] = ERROR;
                        if( nre >= 2 ) Mark_Judge_re[nc][nm][1] = ERROR;

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

					}
					//8 End
				//9
					nm = 9;
					if( TWA_R_n == 3 ) {
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
						pt[rp++] = nc;	pt[rp++] = 0;
						pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 0;
						pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
					} else {
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
						pt[rp++] = nc;	pt[rp++] = 0;
						pt[rp++] = 0;	pt[rp++] = 4;	pt[rp++] = 0;
						pt[rp++] = 0;	pt[rp++] = 5;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
					}
					//9 End
				//R
					nm = 10;
					if( TWA_R_n == 3 ) {
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
						pt[rp++] = nc;	pt[rp++] = 1;
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
					} else {
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
						pt[rp++] = nc;	pt[rp++] = 1;
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
					}
					//R End

	}

////////////////////////////////////////////
////////////////////////////////////////////


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

static void	TWA_Mark_Size_E25( int page, int pt[], int *ls, int *le )
	{

		int				xs, xe;
		int				les;
		int				rx, ry, rl;
		unsigned char	*ad;
		int				i;

			les = pt[4];

		//左-->右
			xs = pt[0] - 5;
			xe = pt[2];
			ry = pt[1];
            ad = Gray_address[page]+ry*MxSize;
			i = 0;
			for( rx=xs; rx<xe; rx++ ) {
				rl = *(ad+rx);
				if( rl >= les ) {
					i++;
					if( i >= 2 ) {
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
			xs = pt[2] + 5;
			xe = pt[0];
			ry = pt[3];
            ad = Gray_address[page]+ry*MxSize;
			i = 0;
			for( rx=xs; rx>xe; rx-- ) {
				rl = *(ad+rx);
				if( rl >= les ) {
					i++;
					if( i >= 2 ) {
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
				xs -= 2;
				xe += 2;
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


/************************************************
	抵抗値設定
************************************************/

void TWA_mark_set_e25( void )
	{
		int					n;
		int					area;

		TWA_R_data[0]=TWA_R_data[1]=TWA_R_data[2]=TWA_R_data[3]=0;

		for( n=0; n<4; n++ ) {
			Comment[0] = MarkChar[n]; Comment[1] = 0;
			if( MarkChar[n] == ' ' ) break;
			if( MarkChar[n] == 'R' ) TWA_R_data[n] = 10;
			else TWA_R_data[n] = atoi( Comment );
		}

		TWA_R_n = n;

		if( TWA_R_n == 3 ) area = 0;	// xxx
		else {
			area = 0;																						// xxxx
			if((TWA_R_data[0]==1)&&(TWA_R_data[1]!=1)&&(TWA_R_data[2]!=1)&&(TWA_R_data[3]!=1)) area =  1;	// 1xxx
			if((TWA_R_data[0]!=1)&&(TWA_R_data[1]==1)&&(TWA_R_data[2]!=1)&&(TWA_R_data[3]!=1)) area =  2;	// x1xx
			if((TWA_R_data[0]!=1)&&(TWA_R_data[1]!=1)&&(TWA_R_data[2]==1)&&(TWA_R_data[3]!=1)) area =  3;	// xx1x
			if((TWA_R_data[0]!=1)&&(TWA_R_data[1]!=1)&&(TWA_R_data[2]!=1)&&(TWA_R_data[3]==1)) area =  4;	// xxx1
			if((TWA_R_data[0]!=1)&&(TWA_R_data[1]!=1)&&(TWA_R_data[2]!=1)&&(TWA_R_data[3]==4)) area =  4;	// xxx4
			if((TWA_R_data[0]==1)&&(TWA_R_data[1]==1)&&(TWA_R_data[2]!=1)&&(TWA_R_data[3]!=1)) area =  5;	// 11xx
			if((TWA_R_data[0]==1)&&(TWA_R_data[1]!=1)&&(TWA_R_data[2]==1)&&(TWA_R_data[3]!=1)) area =  6;	// 1x1x
			if((TWA_R_data[0]==1)&&(TWA_R_data[1]!=1)&&(TWA_R_data[2]!=1)&&(TWA_R_data[3]==1)) area =  7;	// 1xx1
			if((TWA_R_data[0]==1)&&(TWA_R_data[1]!=1)&&(TWA_R_data[2]!=1)&&(TWA_R_data[3]==4)) area =  7;	// 1xx4
			if((TWA_R_data[0]!=1)&&(TWA_R_data[1]==1)&&(TWA_R_data[2]==1)&&(TWA_R_data[3]!=1)) area =  8;	// x11x
			if((TWA_R_data[0]!=1)&&(TWA_R_data[1]==1)&&(TWA_R_data[2]!=1)&&(TWA_R_data[3]==1)) area =  9;	// x1x1
			if((TWA_R_data[0]!=1)&&(TWA_R_data[1]==1)&&(TWA_R_data[2]!=1)&&(TWA_R_data[3]==4)) area =  9;	// x1x4
			if((TWA_R_data[0]!=1)&&(TWA_R_data[1]!=1)&&(TWA_R_data[2]==1)&&(TWA_R_data[3]==1)) area = 10;	// xx11
			if((TWA_R_data[0]!=1)&&(TWA_R_data[1]!=1)&&(TWA_R_data[2]==1)&&(TWA_R_data[3]==4)) area = 10;	// xx14
			if((TWA_R_data[0]==1)&&(TWA_R_data[1]==1)&&(TWA_R_data[2]==1)&&(TWA_R_data[3]!=1)) area = 11;	// 111x
			if((TWA_R_data[0]==1)&&(TWA_R_data[1]==1)&&(TWA_R_data[2]!=1)&&(TWA_R_data[3]==1)) area = 12;	// 11x1
			if((TWA_R_data[0]==1)&&(TWA_R_data[1]==1)&&(TWA_R_data[2]!=1)&&(TWA_R_data[3]==4)) area = 12;	// 11x4
			if((TWA_R_data[0]==1)&&(TWA_R_data[1]!=1)&&(TWA_R_data[2]==1)&&(TWA_R_data[3]==1)) area = 13;	// 1x11
			if((TWA_R_data[0]==1)&&(TWA_R_data[1]!=1)&&(TWA_R_data[2]==1)&&(TWA_R_data[3]==4)) area = 13;	// 1x14
			if((TWA_R_data[0]!=1)&&(TWA_R_data[1]==1)&&(TWA_R_data[2]==1)&&(TWA_R_data[3]==1)) area = 14;	// x111
			if((TWA_R_data[0]!=1)&&(TWA_R_data[1]==1)&&(TWA_R_data[2]==1)&&(TWA_R_data[3]==4)) area = 14;	// x114
			if((TWA_R_data[0]==1)&&(TWA_R_data[1]==1)&&(TWA_R_data[2]==1)&&(TWA_R_data[3]==1)) area = 15;	// 1111
			if((TWA_R_data[0]==1)&&(TWA_R_data[1]==1)&&(TWA_R_data[2]==1)&&(TWA_R_data[3]==4)) area = 15;	// 1114
//			if( TWA_R_data[0]==10 ) area = 16; //<0000>																// Rxxx
			if((TWA_R_data[0]==10)&&(TWA_R_data[1]!=1)&&(TWA_R_data[2]!=1)&&(TWA_R_data[3]!=1)) area = 16;	// Rxxx <0000>
//			if( TWA_R_data[0]==10 ) area = 16;																// Rxxx <0000>
			if((TWA_R_data[0]==10)&&(TWA_R_data[1]==1)&&(TWA_R_data[2]!=1)&&(TWA_R_data[3]!=1)) area = 17;	// R1xx <0000>

		}

		Mark_area_No = area;

//		TWA_ver_title_set();

		TWA_mark_position_set(area);
}

/************************************************
	標印検査ポイント設定
************************************************/

/************************************************
	標印検査ポイント設定
************************************************/

static void	TWA_mark_position_set( int area )
	{
		int			n, n1, n2, i, deg10;
		int			x, y;
		int			offset_x[4] = {0,0,0,0};
		int			l[4] = {0,0,0,0};
		int			w = 0;
		int			moji;
        double		zoomx;
        double		zoomy;
        int			i_mode	= R_CS_GetInspectMode();

		int				x1, x2, x3, x4, x5;
		int				div_n;

		if( i_mode & INSPECT_STEP ) {
			sprintf( Comment,"Debug1:%d",TWA_R_n);
			DrawMessage( Comment, DRAW_GREEN, 2, 2 );
			R_DrawCls();
		}
		TWA_point_cls();

        zoomx = (double)1000/(double)7500;      // SystemPara.nRateX;
        zoomy = (double)1000/(double)SystemPara.nRateY;

		if( TWA_R_n == 3 ) {
			offset_x[0] = Mark_area_3[0];
			offset_x[1] = Mark_area_3[1];
			offset_x[2] = Mark_area_3[2];
			l[0] = Mark_area_3[3];
			l[1] = Mark_area_3[4];
			l[2] = Mark_area_3[5];
			w  = Mark_area_3[6];
			//標印文字分割数
			Mark_div_x = 3;
			Mark_div_y = 7;
            TWA_mark_area_p[0] = (offset_x[0]-l[0]/2)*zoomx;
            TWA_mark_area_p[2] = (offset_x[2]+l[2]/2)*zoomx;
            TWA_mark_area_p[1] = (0-w/2)*zoomy;
            TWA_mark_area_p[3] = (w/2)*zoomy;

			Mark_area_xl = TWA_mark_area_p[2] - TWA_mark_area_p[0];
            Mark_area_yl = w*zoomy;

			if( MarkChar[0] == '1' ) Mark_area_xl -= 7;
			if( MarkChar[0] == '7' ) Mark_area_xl -= 7;
			if( MarkChar[3] == '1' ) Mark_area_xl -= 7;
		} else {
			offset_x[0] = Mark_area_4[area][0];
			offset_x[1] = Mark_area_4[area][1];
			offset_x[2] = Mark_area_4[area][2];
			offset_x[3] = Mark_area_4[area][3];
			l[0] = Mark_area_4[area][4];
			l[1] = Mark_area_4[area][5];
			l[2] = Mark_area_4[area][6];
			l[3] = Mark_area_4[area][7];
			w  = Mark_area_4[area][8];
			//標印文字分割数
			Mark_div_x = 3;
			Mark_div_y = 7;
            TWA_mark_area_p[0] = (offset_x[0]-l[0]/2)*zoomx;
            TWA_mark_area_p[2] = (offset_x[3]+l[3]/2)*zoomx;
            TWA_mark_area_p[1] = (0-w/2)*zoomy;
            TWA_mark_area_p[3] = (w/2)*zoomy;

			Mark_area_xl = TWA_mark_area_p[2] - TWA_mark_area_p[0];
            Mark_area_yl = w*zoomy;

			if( MarkChar[0] == '1' ) Mark_area_xl -= 5;
			if( MarkChar[3] == '1' ) Mark_area_xl -= 5;
		}

		//文字エリア
			i = 60;
			for( n=0; n<TWA_R_n; n++ ) {
                Mark_area_x[i][n][0] = Mark_area_x[i][n][2] = (offset_x[n]-l[n]/2)*zoomx + 0.5;		//x1,x3
                Mark_area_x[i][n][1] = Mark_area_x[i][n][3] = (offset_x[n]+l[n]/2)*zoomx + 0.5;		//x2,x4
                Mark_area_y[i][n][0] = Mark_area_y[i][n][1] = (0-w/2)*zoomy;						//y1,y2
                Mark_area_y[i][n][2] = Mark_area_y[i][n][3] = (w/2)*zoomy;						//y3,y4
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

		//文字検査データ
			i = 60;
			div_n = Mark_div_x * Mark_div_y;
			for( n=0; n<TWA_R_n; n++ ) {
				moji = TWA_R_data[n];
				if( TWA_R_n == 3 ) {
					for( n1=0; n1<div_n; n1++ ) {
                        Mark_rx[i][n][n1] = ( Mark_point_3[moji][n1][0]+offset_x[n] ) * zoomx;
                        Mark_ry[i][n][n1] = Mark_point_3[moji][n1][1] * zoomy;
					}
				} else {
					if( l[n] == 250 ) {
						for( n1=0; n1<div_n; n1++ ) {
                            Mark_rx[i][n][n1] = ( Mark_point_4s[moji][n1][0]+offset_x[n] ) * zoomx;
                            Mark_ry[i][n][n1] = Mark_point_4s[moji][n1][1] * zoomy;
						}
					} else {
						for( n1=0; n1<div_n; n1++ ) {
                            Mark_rx[i][n][n1] = ( Mark_point_4l[moji][n1][0]+offset_x[n] ) * zoomx;
                            Mark_ry[i][n][n1] = Mark_point_4l[moji][n1][1] * zoomy;
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

	}

