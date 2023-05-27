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
#include "R_cursor.h"
#include "R_cs.h"
#include "R_cdraw.h"
#include "tmcr00i.h"
#include "tp_sub.h"
#include "mark18x2.h"
#include "mcr03.h"

extern  MACHINE_PARAMETER	SystemPara;
extern  INSPECT_PARAMETER	InspPara, InspParaM;

// --- Chip 傾き ---
extern  int        		ChipAtan;       				// Chip 角度×１０の値（アークタンジェント）
// --- Inspection Inage(Gray)
extern int				GetGrayPn;                  	// Get capture		gray memory
// --- Inspection Inage(Bin)
extern int				InspBinPn;					// Inspection		bin memory
//extern Regression_def   ElecEdge[3][8];
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
static	int				MarkChoice_st[5][11];								// 標印選択回数
static	int				MarkChoice_re[5][11];								// 標印選択回数
static	int				MarkChoiceMax_st[5][2];								// 標印選択 max[character][data]
static	int				MarkChoiceMax_re[5][2];								// 標印選択 max[character][data]
static	char			MarkCharacter[12][2]= { "0","1","2","3","4",
                                                "5","6","7","8","9","R" };	// 標印表示文字

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
static	int				Bin_Ma_l_ini[5][2]	= {{ 60, 80},{ 60, 80},{ 60, 80},{ 60, 80},{ 60, 80}};	//標印検査時の２値化レベル設定範囲
static	int				Bin_Ma_l[5]			= { 35, 35, 35, 40,40};					//標印検査時の２値化レベル

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

static void     TWA_G2_level( int *level, int *l_lo, int *l_up );
static int		TWA_segmentation( int deg10, int level, int n_seg, int offset_x, int offset_y );
int		TWA_Mark_point_10_25( int deg10, int ox, int oy, int ol );
static int		TWA_Mark_Judge( void );
static int      TWA_Mark_level( int deg10, int ox, int oy );
static int		TWA_Mark_More( void );
static void     TWA_Mark_error( int nc );
static void     TWA_mark_position_set( void );

void	TWA_gray2bin( int level, int xs, int ys, int xl, int yl, int gn, int bn );


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
static	unsigned int	Judge_M_More[2] = {750, 50};						// 標印漏れ出し（にじみ）判定値[data] //<K1>

#define KURO			0
#define SHIRO			1

/************************************************
	標印検査
************************************************/

int		TWA_kensa_m18( void )
	{
		int				deg10;
		int				i, nc, nm;
		int				level, l_lo, l_ce, l_up;
		int				res, res_st, res_re; // MIA
		int				hx, hy, hf, hfx, hfy, ox, oy;
		int				hl_x, hl_y;
		int				off_px, off_py;
		int				n_seg, n_x, n_y;
		int				x_seg, y_seg;
		int				seg_offset_x, seg_offset_y;
		int				offset_Mal[3] = { 0, 5, -5 };
		int				binPn	= InspBinPn;
		int				grayPn  = InspGrayPn;
		int				i_mode	= R_CS_GetInspectMode();
		int				dc, dl; // MIA
        char            CommentM[15][50];					// Comment Buffer(Mark)

		//There is marking inspection, no  //標印検査有り無し
			if( TWA_Mark_Insp != 0 ) return(OK);

		// Initialze // MIA
			for( nc=0; nc<TWA_R_n; nc++ ) {
				for( nm=0; nm<11; nm++ ) {
					MarkChoice_st[nc][nm] = 0;
					MarkChoice_re[nc][nm] = 0;
				}
				MarkChoiceMax_st[nc][0] = ERROR;
				MarkChoiceMax_st[nc][1] = 0;
				MarkChoiceMax_re[nc][0] = ERROR;
				MarkChoiceMax_re[nc][1] = 0;
			} // MIA

		//marking character inspection 
			if( TWA_Insp_mode == Insp_Pinhole ) return(OK);		//Plating adhesion inspection 

		//marking inclination calculation 
			deg10 = TWA_katamuki();
			i = (300+deg10)/5;
			if((i<0)||(i>120)) return(ERROR);

			// 4文字標印が細いため くびれると位置決めを失敗するので太らせる
			R_max_filter(InspGrayPn,BASE3_GRAYADDRESS,0,0,FxSize-1,FySize-1);
		//Mark Level
			level = 0;
			TWA_G2_level( &l_ce, &l_lo, &l_up );

		//A character is brought down. 
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

		//Character level 
				ox = oy = 0;
				if(TWA_Mark_level( deg10, ox, oy )!=OK){
					return ERROR3;
				}
		//		if(TWA_Mark_More() != OK){ 	//<z1>
		//			return ERROR4;					//<z1>
		//		}									//<z1>

			for( nc=0; nc<TWA_R_n; nc++ ) {
				Mark_J100_st[nc] = ERROR;
				Mark_J100_re[nc] = ERROR;
			}

 /*********************************************
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
 ********************************************/

			hl_x = 8;	//; //6
			hl_y = 8;	//; //2

			off_px = off_py = 4;

			res = ERROR;
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
						if(TWA_R_n == 4)	InspGrayPn = BASE3_GRAYADDRESS;	//max filter image
						//marking  data reading
                        if( TWA_Mark_point_10_25( deg10, ox, oy, offset_Mal[nm] ) == OK ) {
							//Character decision 
							if( TWA_Mark_Judge() == OK ) {
								if( res != OK ) {
									Ma_pos[0] += ox;
									Ma_pos[1] += ox;
									Ma_pos[2] += oy;
									Ma_pos[3] += oy;
								}
								res = OK;
							}
						}
						InspGrayPn=grayPn;
					}
				}
			}
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
			if( i_mode & INSPECT_STEP ) {
				R_display_control( BIN_MEMORY | GRAY_MEMORY );
				BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
			//	R_move_bin_memory( binPn, BinP0, 0, 0, FxSize-1, FySize-1 );
				// --- Title
				dc = 1;		dl = 1;
				sprintf( Comment, "Marking[%s]", MarkChar );
					R_chrdisp( dc, dl, Comment );		dl+=2;

				for( nm=0; nm<11; nm++ )	sprintf( CommentM[nm], "[%s]|", MarkCharacter[nm] );
				for(     ; nm<15; nm++ )	sprintf( CommentM[nm], ":" );
				// --- Character data
				for( nc=0; nc<TWA_R_n; nc++ ) {
					// --- Character data
					for( nm=0; nm<11; nm++ ) {
						sprintf( Comment, "(%2d)(%2d)|", MarkChoice_st[nc][nm], MarkChoice_re[TWA_R_n-1-nc][nm] );
						strcat( CommentM[nm], Comment );
					}
					// --- Max Character data(st)
					nm = MarkChoiceMax_st[nc][0];
					if( nm < 0 )	sprintf( Comment, "%s(%2d): ", "-", 0 );
					else			sprintf( Comment, "%s(%2d): ", MarkCharacter[nm], MarkChoice_st[nc][nm] );
					strcat( CommentM[12], Comment );
					nm = MarkChoiceMax_re[TWA_R_n-1-nc][0];
					if( nm < 0 )	sprintf( Comment, "%s(%2d): ", "-", 0 );
					else			sprintf( Comment, "%s(%2d): ", MarkCharacter[nm], MarkChoice_re[TWA_R_n-1-nc][nm] );
					strcat( CommentM[13], Comment );
				}
				for( nm=0; nm<15; nm++ ) {
					R_chrdisp( dc, dl, CommentM[nm] );		dl++;
				}
				// wait
				R_mouse_de_yesno();
				BinClsPn( BinP0, 0, 0, FxSize-1, FySize-1 );
			}

		// Judge // MIA
			// --- Judge Max Choice(st)
			for( nc=0; nc<TWA_R_n; nc++ ) {
				if( MarkChoiceMax_st[nc][0] != TWA_R_data[nc] )	break;
				if( MarkChoiceMax_st[nc][1] < 9 )				break;
			}
			if( TWA_R_n == nc )	res_st = OK;
			else				res_st = ERROR;
			// --- Judge Max Choice(re)
			for( nc=0; nc<TWA_R_n; nc++ ) {
				if( MarkChoiceMax_re[nc][0] != TWA_R_data[nc] ) break;
				if( MarkChoiceMax_re[nc][1] < 9 )				break;
			}
			if( TWA_R_n == nc )	res_re = OK;
			else				res_re = ERROR;
	
			if( res != OK )	return ERROR4;
			if(( res_st != OK )&&( res_re != OK ))	return ERROR5;
			
			return OK;

	}	// TWA_kensa_m( void ) end
	

/*********************************************************************************
*********************************************************************************/

static void	TWA_G2_level( int *level, int *l_lo, int *l_up )
	{
		int					xs, xe, ys, ye;
		int					cl, clmin, clbase, clmax;		//clcen
		int					n;
		int					bright_n;
		R_RECT				area;
		int					i_mode	= R_CS_GetInspectMode();
		int					grayPn	= InspGrayPn;
		int					binPn	= InspBinPn;
        int                 posi_res_x,posi_res_y;

		if(TWA_R_n == 4)	grayPn	= BASE3_GRAYADDRESS;
		//検査範囲設定
			xs = G2_pos[0] + 10;
			xe = G2_pos[1] - 10;
			ys = G2_pos[2] + 5;
			ye = G2_pos[3] - 5;
			xs	= ElecPosRough[DataLeft][PosXmax]+20;		// 20170614
			xe	= ElecPosRough[DataRight][PosXmin]-20;
			

            if( ERROR == R_bgray_thresholding_open( THRESH_DEPTH_8BIT )) return;    // ERROR;					//JIG addition search light auto binary
			//指定エリアのヒストグラムを取る。
			R_get_histgram( xs, ys, xe, ye, Gray_address[grayPn] );
			clbase = R_bgray_thresholding( Bright, 10, 240, THRESH_OTSU );
			R_bgray_thresholding_close();

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
				if(cl < 3)	cl=3;	// l_loとl_upの差が5以上無いと2値レベルを変えたサーチができない

				*l_lo = clbase - cl;				//min level
				*l_up = clbase + cl;				//max level
				if( *l_lo < Bin_Ma_l_ini[TWA_Spec][0] ) *l_lo = Bin_Ma_l_ini[TWA_Spec][0];
				if( *l_up < *l_lo )                     *l_up = *l_lo + 5;
				*level = clbase;

			//Manual
	 		if( i_mode & INSPECT_STEP ) {
				R_display_control( BIN_MEMORY | GRAY_MEMORY );
				R_bin_cls( 0, 0, FX_SIZE-1, FY_SIZE-1 );
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
                //TWA_pset_histgram( 10, 50, Bright) ;  // ヒストグラムを描く
				WaitMouseClick();
				TWA_gray2bin( clbase, xs, ys, xe-xs+1, ye-ys+1, grayPn, 0 );
				area.left	= xs;
				area.right	= xe;
				area.top	= ys;
				area.bottom	= ye;
				R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );	
                printf( "Mark Seg.Bright min:%d  cen:%d  max:%d  n:%d\n", clmin, clbase, clmax, bright_n );
                printf( "Mark Seg.Level  min:%d  cen:%d  max:%d\n", *l_lo, clbase, *l_up );

				posi_res_x = 0;
				posi_res_y = 30;
				R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"Marking Bin Level (J-Class)" ); //<K1> 9499			
				R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+1 ,"Average: %3d", clbase );  //<K1> 9499 Ave

				WaitMouseClick();

			}
				
			xsm = xs; //<z1>
			ysm = ys; //<z1>
			xem = xe; //<z1>
			yem = ye; //<z1> 
			
			more_lvl = clbase;//<z1>
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
		int          	nm, chk_mark, chk_error, dn;
		int             ans_seg;
		int             offset, offset_l;
		int             mark_le_diff,mark_ri_diff,width_seg;
		R_RECT			area;
        int				i_mode	= R_CS_GetInspectMode();
		int				grayPn	= InspGrayPn;
		int				binPn	= InspBinPn;
        int				elec = 0;

		if(TWA_R_n == 4)	grayPn	= BASE3_GRAYADDRESS;

		if( TWA_R_n == 3 ) {
			slx2 = 3;
			sly = 3;
			snx = 5;
			sny = 3;
		} else {
			slx2 = 3;
			sly = 3;
			snx = 3;
			sny = 3;
		}

		//標印傾き確認
		i = (300+deg10)/5;
		if((i<0)||(i>120)) return(ERROR);

		for( n=0; n<640; n++ ) xp[n] = 0;
		for( n=0; n<480; n++ ) yp[n] = 0;

		//検査範囲設定
//		xs = (Ap_pos[0]+G2_pos[0])/2 + 5;
//		xe = (Ap_pos[1]+G2_pos[1])/2 - 5;
		ys = G2_pos[2] + 5 + offset_y;
		ye = G2_pos[3] - 5 - offset_y;
		xs = ElecPosRough[DataLeft][2]+10;
		xe = ElecPosRough[DataRight][0]-10;

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
		}

		//Y judge
		ans = OK;
		if( Mark_Yseg[n_seg][1] < G2_pos[2] ) ans = ERROR;
		if( Mark_Yseg[n_seg][2] > G2_pos[3] ) ans = ERROR;

		yl = Mark_Yseg[n_seg][2] - Mark_Yseg[n_seg][1];
		if( ans == OK ) {
			y = abs( yl - Mark_area_yl )*100/Mark_area_yl;
			if( y < 20 ) {
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
			xs = G2_pos[0]+16+offset_x;
			xe = G2_pos[1]-16-offset_x;

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
//			slx = xp_av/4; //  3 ok
			slx = xp_av/6; //  3 ok
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
/*
		if(ans_seg == ERROR) {
		//Segmentation X axis 2010.08.04
			chk_mark = chk_error = nm = 0;
			Mark_temp[0] = Mark_temp[1] = Mark_temp[2] = Mark_temp[3] = Mark_temp[4] = Mark_temp[5] = Mark_temp[6] = Mark_temp[7] = ERROR;
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
//					if( xp[n] <= (slx*3)/4 && xp[n+1] <= (slx*3)/4){ nm = n; break;}
					if( xp[n] <= slx && xp[n+1] <= slx){ nm = n; break;}
				}
				Mark_temp[chk_mark+1] = xs+n*4;

				if(TWA_R_n == 3){
					if((TWA_Spec == SpecNormal))  offset = 50;   // 30x2 20170128
					if((TWA_Spec == SpecResin))   offset = 50;   // 35x2 20170128
					if(TWA_Spec==SpecNormal) offset_l = 14;                     //  9x2 20170128
					if(TWA_Spec==SpecResin)  offset_l = 14;                     // 13x2 20170128
				} else {
					if((TWA_Spec == SpecNormal))  offset = 50;   // 30x2 20170128
					if((TWA_Spec == SpecResin))   offset = 50;   // 35x2 20170128
					if(TWA_Spec==SpecNormal) offset_l = 14;                     //  9x2 20170128
					if(TWA_Spec==SpecResin)  offset_l = 14;                     // 13x2 20170128
				}

				if(((Mark_temp[chk_mark+1]-Mark_temp[chk_mark] ) > offset_l )&&((Mark_temp[chk_mark+1]-Mark_temp[chk_mark] ) < offset )){
					chk_mark +=2;
				}else{
					Mark_temp[chk_mark+1] = Mark_temp[chk_mark] = ERROR;
					chk_error ++;
				}
				if( chk_error >= 10 ) break;
				if(( TWA_R_n == 3 )&&( chk_mark == 6 )) break;
				if(( TWA_R_n == 4 )&&( chk_mark == 8 )) break;
			}

			Mark_Xseg[n_seg][1] = Mark_temp[0];
			if( TWA_R_n == 3){
				Mark_Xseg[n_seg][2] = Mark_temp[5];
				for(n=0; n<6; n++){
					if( Mark_temp[n] == ERROR ) {ans_seg = ERROR; break;}
					if(n==5) ans_seg = OK;
				}
			} else {
				Mark_Xseg[n_seg][2] = Mark_temp[7];
				for(n=0; n<8; n++){
					if( Mark_temp[n] == ERROR ) {ans_seg = ERROR; break;}
					if(n==7) ans_seg = OK;
				}
			}
		}

		if( ans_seg == OK ){
			if(TWA_Spec == SpecNormal){
				if( TWA_R_n == 3){
					if(abs((Mark_temp[1]-Mark_temp[0])-(Mark_temp[3]-Mark_temp[2])) >= 32 ) return ERROR;   // 8x2 20170128tanaka
					if(abs((Mark_temp[3]-Mark_temp[2])-(Mark_temp[5]-Mark_temp[4])) >= 32 ) return ERROR;   // 8x2 20170128tanaka
				} else {
					if(abs((Mark_temp[1]-Mark_temp[0])-(Mark_temp[3]-Mark_temp[2])) >= 28 ) return ERROR;   // 8x2 20170128tanaka
					if(abs((Mark_temp[3]-Mark_temp[2])-(Mark_temp[5]-Mark_temp[4])) >= 28 ) return ERROR;   // 8x2 20170128tanaka
					if(abs((Mark_temp[5]-Mark_temp[4])-(Mark_temp[7]-Mark_temp[6])) >= 28 ) return ERROR;   // 8x2 20170128tanaka
				}
			}
		}
*/
		if( ans_seg == ERROR ){

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
			WaitMouseClick();
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
/*
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
*/
/////////////////////////////////////////////
//////////CHECK WIDTH OF X SEGMENT////////////			BY JIG
///////////////////////////////////////////
/*
		Diff_mark_seg = ERROR ;
		width_seg = 0

		width_seg = abs( Mark_Xseg[n_seg][2]-Mark_Xseg[n_seg][1] );
		if(( width_seg >= 150)&&( width_seg <= 174)) {					//75x2 84x2=168 -> 174 check again
			Diff_mark_seg = OK;
		} else {
			Diff_mark_seg = ERROR;
		}
*/
/////////////END CHECK WIDTH  SEGMENT/////////////////

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

			//文字エリア 反転
			for( n=0; n<TWA_R_n; n++ ) {
				xe = x0 - Mark_area_x[i][n][0];
				xs = x0 - Mark_area_x[i][n][3];
				ye = y0 - Mark_area_y[i][n][0];
				ys = y0 - Mark_area_y[i][n][2];
				area.left	= xs;
				area.right	= xe;
				area.top	= ys;
				area.bottom	= ye;
				R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
			}
			WaitMouseClick();


			//文字検査データ
			/*
			div_n = Mark_div_x * Mark_div_y;
			for( n=0; n<TWA_R_n; n++ ) {
				for( n1=0; n1<div_n; n1++ ) {
					//検査座標設定
					x = x0 + Mark_rx[i][n][n1];
					y = y0 + Mark_ry[i][n][n1];
					xs = x - 5;		// 20170614
					xe = x + 4;
					ys = y - 5;
					ye = y + 4;
				//<z1>		R_box( xs, ys, xe, ye, GRAPH_DRAW, SOLID_LINE );
				}
			}
			WaitMouseClick();
			*/
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

		if(TWA_R_n == 4)	grayPn	= BASE3_GRAYADDRESS;

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
				//指定エリアのヒストグラムを取る。（１ドットおきに間引く）
//					R_get_step_histgram( xs, ys, xe, ye, Gray_address[grayPn] );
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
				//指定エリアのヒストグラムを取る。（１ドットおきに間引く）
//					R_get_step_histgram( xs, ys, xe, ye, Gray_address[GrayP2] );
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
//						R_gray2bin( Mark_Slevel_r[cn][1], xs, ys, xe-xs+1, ye-ys+1, Gray_address[GrayP2], Bin_address[BinP0] );						
					}
//<---<z1>
				xs_mb[cn] = xs;
				ys_mb[cn] = ys;
				xe_mb[cn] = xe;
				ye_mb[cn] = ye;
//---><z1>
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
		//	top_length   = mark_top - (Elec[DataLR].CornerPosY[0]+Elec[DataLR].CornerPosY[2])/2;
		//	bottom_length= (Elec[DataLR].CornerPosY[1]+Elec[DataLR].CornerPosY[3])/2 -mark_bottom;
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
//<z4>		int				judge;

		int				so_st[5][12][4], so_re[5][12][4];
		int				j, i, l0, l1, l2, l3;
		int				jm;

		int				jm101st, jm101re, n_jm101;
		int				insp_st, insp_re;

		int				insp_mode;
		int				i_mode	= R_CS_GetInspectMode();
		int				grayPn	= InspGrayPn;

//<z4>			if( TWA_Insp_mode != Insp_Pinhole ) judge = 76;	//60;  - viter 8.88
//<z4>			else								judge = 80; //

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
					}
				}

		// Mark choice // MIA
			for( nc=0; nc<TWA_R_n; nc++ ) {
				// --- check matching rate
//<z4>				if( max_st[nc][0] > judge ) {
				if( max_st[nc][0] > Judge_Mark ) { //<z4>
					nm = max_st[nc][1];
					MarkChoice_st[nc][nm]++;
				}
				// --- check matching rate
//<z4>				if( max_re[nc][0] > judge ) {
				if( max_re[nc][0] > Judge_Mark ) { //<z4>
					nm = max_re[nc][1];
					MarkChoice_re[nc][nm]++;
				}
			} // MIA

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

//#ifdef	Mark_DEBUG

            if( i_mode & INSPECT_STEP ) {
                int posi_res_x,posi_res_y;
                R_display_control( BIN_MEMORY | GRAY_MEMORY );
				R_move_gray_memory( grayPn, GrayP0, 0, 0, FX_SIZE-1, FY_SIZE-1 );
				for( nc=0; nc<TWA_R_n; nc++ ) {
					if( TWA_Spec != SpecJPW ) {
						posi_res_x = nc * 31;
						posi_res_y = 30;
						R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y   ,"Rst : %d                    ", nc );
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


						ncr = TWA_R_n - nc - 1;
						posi_res_x = ncr * 31;
						R_DrawPrintf(DRAW_WHITE, DISP_NORMAL, posi_res_x, posi_res_y+7 ,"Rre : %d                    ",ncr);
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
//<z4>						if( max_st[nc][0] < judge ) break;
//<z4>						if( Mark_Judge_st[nc][jm][3] < judge ) break;
						if( max_st[nc][0] < Judge_Mark ) break; //<z4>
						if( Mark_Judge_st[nc][jm][3] < Judge_Mark ) break; //<z4>
					} else {
						if( TWA_R_data[nc] != max_st[nc][1] ) break;
//<z4>						if( max_st[nc][0] < judge ) break;
						if( max_st[nc][0] < Judge_Mark ) break; //<z4>
					}
				}
				if( nc == TWA_R_n ) return OK;
			}

			if( insp_re == OK ) {
				for( nc=0; nc<TWA_R_n; nc++ ) {
					jm = TWA_R_data[nc];
					if( Mark_L_set[nc] != 0 ) {
						if( TWA_R_data[nc] != max_re[nc][1] ) break;
//<z4>						if( max_re[nc][0] < judge ) break;
//<z4>						if( Mark_Judge_re[nc][jm][3] < judge ) break;
						if( max_re[nc][0] < Judge_Mark ) break; //<z4>
						if( Mark_Judge_re[nc][jm][3] < Judge_Mark ) break; //<z4>
					} else {
						if( TWA_R_data[nc] != max_re[nc][1] ) break;
//<z4>						if( max_re[nc][0] < judge ) break;
						if( max_re[nc][0] < Judge_Mark ) break; //<z4>
					}
				}
				if( nc == TWA_R_n ) return OK;
			}

		return ERROR;

	}	// TWA_Mark_Judge( void ) end

static void	TWA_Mark_error( int nc )
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
					pt[rp++] = nc;	pt[rp++] = 0;	//Bcack
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2-1;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2;	pt[rp++] = 0;
					pt[rp++] = 1;	pt[rp++] = Mark_div_y/2+1;	pt[rp++] = 0;
					pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
					TWA_Mark_error_check( pt, &nst, &nre );
					if( nst!=3 ) Mark_Judge_st[nc][nm][1] = ERROR;
					if( nre!=3 ) Mark_Judge_re[nc][nm][1] = ERROR;

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
					} else {
/*						rp = 0;
						pt[rp++] = nc;	pt[rp++] = 0;
						pt[rp++] = 1;	pt[rp++] = 1;	pt[rp++] = 0;
						pt[rp++] = 1;	pt[rp++] = 2;	pt[rp++] = 0;
						pt[rp++] = 1;	pt[rp++] = 3;	pt[rp++] = 0;
						pt[rp++] = -1;	pt[rp++] = -1;	pt[rp++] = -1;
						TWA_Mark_error_check( pt, &nst, &nre );
						if( nst == 0 ) Mark_Judge_st[nc][nm][1] = ERROR;
						if( nre == 0 ) Mark_Judge_re[nc][nm][1] = ERROR;
*/
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

void TWA_mark_set_m18( void )
	{
		int						n;
		int						area;

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
		}

		Mark_area_No = area;

//		TWA_ver_title_set();

		TWA_mark_position_set();
}

/************************************************
	標印検査ポイント設定
************************************************/

static void	TWA_mark_position_set( void )
	{
		int			n, n1, n2, i, deg10, area;
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

		TWA_point_cls();

		zoomx = (double)1000/(double)SystemPara.nRateX;
		zoomy = (double)1000/(double)SystemPara.nRateY;
		area = Mark_area_No;

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
			TWA_mark_area_p[2] = (offset_x[2]+l[2]/2)*zoomy;
			TWA_mark_area_p[1] = (0-w/2)*zoomy;
			TWA_mark_area_p[3] = (w/2)*zoomy;

			Mark_area_xl = TWA_mark_area_p[2] - TWA_mark_area_p[0];
            Mark_area_yl = w*zoomy;

			if( MarkChar[0] == '1' ) Mark_area_xl -= 14;
			if( MarkChar[0] == '7' ) Mark_area_xl -= 14;
			if( MarkChar[3] == '1' ) Mark_area_xl -= 14;
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

			if( MarkChar[0] == '1' ) Mark_area_xl -= 10;
			if( MarkChar[3] == '1' ) Mark_area_xl -= 10;
		}

		//文字エリア
			i = 60;
			for( n=0; n<TWA_R_n; n++ ) {
				Mark_area_x[i][n][0] = Mark_area_x[i][n][2] = (offset_x[n]-l[n]/2)*zoomx;		//x1,x3
				Mark_area_x[i][n][1] = Mark_area_x[i][n][3] = (offset_x[n]+l[n]/2)*zoomx;		//x2,x4
				Mark_area_y[i][n][0] = Mark_area_y[i][n][1] = (0-w/2)*zoomy;					//y1,y2
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

/************************************************
	Marking More (Blotted) Inspection Start <z1>
************************************************/
static int		TWA_Mark_More( void )
{

	int ans,n;
	int	grayPn	= InspGrayPn;
	int binPn   = BinP2;
	int	i_mode	= R_CS_GetInspectMode();
	R_RECT area;
	TWA_gray2bin( more_lvl, xsm, ysm, xem-xsm+1, yem-ysm+1, grayPn, binPn );
	for(n=0;n<4;n++){
		BinClsPn( binPn, xs_mb[n]-1, ys_mb[n]-1, xe_mb[n]+1, ye_mb[n]+1 );
	}	

	ans = R_count_bin_area( binPn, xsm, ysm, xem, yem);
						

	if( i_mode & INSPECT_STEP) {
		R_move_bin_memory(binPn, BinP0, xsm, ysm, xem, yem);
		area.left	= xsm;
		area.right	= xem;
		area.top	= ysm;
		area.bottom	= yem;
		R_DrawBox( DRAW_GREEN, area, GRAPH_DRAW, SOLID_LINE );
		sprintf( Comment1, "Excess Marking Binarization" );
		R_chrdisp( 1, 14, Comment1 );
		sprintf( Comment2, "Count : %5d  Limit : %5d ",ans,Judge_M_More[0] );
		R_chrdisp( 1, 15, Comment2 );
		WaitMouseClick();											
	}
							
	if(ans>Judge_M_More[0]){
		return ERROR;
	}
				
	return OK;
}
/************************************************
	Marking More (Blotted) Inspection End <z1>
************************************************/
