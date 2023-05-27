/******************************************************************************
	１次元推移データ解析ライブラリ
	R_TRANS.C / R_TRANS.H
	
	履歴
	Version 3.50		2007.12.07.000		saito	オリジナル
						2008.03.18.001		saito	マクロ不具合修正
													
******************************************************************************/

#ifndef	R_TRANS_H_
#define	R_TRANS_H_

#define		TRANS_TYPE_INT			0				//int型データを処理する
#define		TRANS_TYPE_GRAYIMAGE	1				//GrayImage(unsigned char型）データを処理する
#define		TRANS_DIFF_STEP_MAX		3				

//エッジ検出方向
#define		TRANS_DIR_MASK			0x0000000f		//dir   のチェック用のマスク
#define		TRANS_DIR_0				0x00000001		//右  方向へデータ生成	only gray image
#define		TRANS_DIR_45			0x00000002		//右下方向へデータ生成	only gray image
#define		TRANS_DIR_90			0x00000003		//下  方向へデータ生成	only gray image
#define		TRANS_DIR_135			0x00000004		//左下方向へデータ生成	only gray image
#define		TRANS_DIR_180			0x00000005		//左  方向へデータ生成	only gray image
#define		TRANS_DIR_225			0x00000006		//左上方向へデータ生成	only gray image
#define		TRANS_DIR_270			0x00000007		//上  方向へデータ生成	only gray image
#define		TRANS_DIR_315			0x00000008		//右上方向へデータ生成	only gray image

//エッジ検出モード
#define		TRANS_MODE_MASK			0x000000f0		//mode  のチェック用のマスク
#define		TRANS_MODE_BtoW			0x00000010		//黒→白（値小→大）の変位点を検出
#define		TRANS_MODE_WtoB			0x00000020		//白→黒（値大→小）の変位点を検出
#define		TRANS_MODE_BWtoWB		0x00000030		//白→黒（値大→小）、黒→白（値小→大）の変位点を検出
#define		TRANS_MODE_BtoWtoB		0x00000040		//黒→白→黒（値小→大→小）の変位点を検出
#define		TRANS_MODE_WtoBtoW		0x00000050		//白→黒→白（値大→小→大）の変位点を検出

//エッジ検出選択
#define		TRANS_CHOICE_MASK		0x00000f00		//choideのチェック用のマスク
#define		TRANS_CHOICE_N_BEGIN	0x00000100		//データ点列の開始側からｎ番目の変位点を検出
#define		TRANS_CHOICE_N_END		0x00000200		//データ点列の終了側からｎ番目の変位点を検出
#define		TRANS_CHOICE_MAX_AMP	0x00000300		//データ点列の振幅最大の変位点を検出
#define		TRANS_CHOICE_MAX_AREA	0x00000400		//データ点列の振幅領域の変位点を検出
#define		TRANS_CHOICE_CLOSED		0x00000500		//未使用

//データ平滑化手法
#define		TRANS_SMOOTH_MASK		0x0000f000		//smoothのチェック用のマスク
#define		TRANS_SMOOTH_AVE		0x00001000		//平均値を用いる
#define		TRANS_SMOOTH_MEDIAN		0x00002000		//中央値を用いる

//データ作成用（未対応）
#define		TRANS_DIR_X				0				//Ｘ方向にデータ生成
#define		TRANS_DIR_Y				1				//Ｙ方向にデータ生成

#define		TRANS_CREATE_AVE		0				//平均値を用いる
#define		TRANS_CREATE_MEDIAN		1				//中央値（メディアン）を用いる
#define		TRANS_CREATE_SUM		2				//総和を用いる

//サーチパラメータ設定関数（マクロ）
#define R_trans_para_set_start(trans,start)				((trans).point_start=start)		// エッジ検出の処理開始点の設定
#define R_trans_para_set_end(trans,end)					((trans).point_end=end)			// エッジ検出の処理終了点の設定
#define R_trans_para_set_count_smooth(trans,smooth)		((trans).count_smooth=smooth)	// データの平滑化回数の設定
#define R_trans_para_set_index(trans,idx)				((trans).index=idx)				// 何番目のエッジを検出するか
#define R_trans_para_set_thickness(trans,thin)			((trans).thickness=thin)		// 平滑化のデータ処理幅
#define R_trans_para_set_diff_step(trans,step)			((trans).diff_step=step)		// 微分STEP : d(i) = d(i+step)-d(i-step)
#define R_trans_para_set_thresh_amp(trans,th_amp)		((trans).thresh_amp=th_amp)		// 振幅しきい値：これ以下の振幅のデータは無視する
#define R_trans_para_set_thresh_area(trans,th_area)		((trans).thresh_area=th_area)	// 面積しきい値：これ以下の面積のデータは無視する

#define R_trans_para_get_start(trans)					((trans).point_start)
#define R_trans_para_get_end(trans)						((trans).point_end)
#define R_trans_para_get_count_smooth(trans)			((trans).count_smooth)
#define R_trans_para_get_index(trans)					((trans).index)
#define R_trans_para_get_thickness(trans)				((trans).thickness)
#define R_trans_para_get_diff_step(trans)				((trans).diff_step)
#define R_trans_para_get_thresh_amp(trans)				((trans).thresh_amp)
#define R_trans_para_get_thresh_area(trans)				((trans).thresh_area)
#define R_trans_para_get_detect_point(trans)			((trans).point_edge)


// --- variable ---
//エッジ候補点構造体
typedef struct{
	int		num;					//データ番号（何番目か）
	int		amp;					//振幅値（％ｘ１００の値）
	int		area;					//面積値（振幅値の積分）
}R_TRANS_INDEX;

//１次データ配列構造体
typedef struct{
	int		id;						//構造体ID → CREATE時にTRANS_IDがｾｯﾄされる
	int 	*data_int;				//データ列（int   *型)
	UCHAR	*data_gray;				//データ列（uchar *型)
	int		*data_work;				//作業領域
	int		*data_smooth;			//平滑化処理後のデータ列（uchar*型）
	int		*data_diff;				//データ列の微分値 data_diff(n) = data(n+1)-data(n-1)
	int		*data_area;				//エッジ検出点の持つ面積
	int		*data_edge;				//エッジ検出フラグ 0:not edge  1:edge point
	int		data_type;				//データタイプの選択　（INT型/GRAYIMAGE型）
	int		dir;					//GRAYIMAGE型のみ　データ方向
	int		mem_offset;				//GRAYIMAGE型のみ　メモリアドレスオフセットサイズ
	int		point_start;			//start 位置 エッジ検出時 処理領域
	int		point_end;				//end   位置 エッジ検出時 処理領域
	int		point_edge;				//最終検出エッジ位置 データ番号(0,1,2,...)
	int		point_edge_fine;		//最終検出エッジ位置（高精度）データ番号の1000倍値 (0,1000,2000,...)
	int		count_data;				//データ要素数
	int		count_data_max;			//データ要素最大数
	int		count_smooth;			//平滑化回数
	int		thickness;				//平滑化処理時のデータ幅
	int		index;					//エッジ選択　～N_BEGIN,N_END時に何番目のエッジを検出するか
	int		diff_step;				//微分ＳＴＥＰ
	int		diff_max;				//微分値の最大値
	int		thresh_diff;			//振幅しきい値：これ以下の振幅のデータは無視する（元の値）
	int		thresh_amp;				//振幅しきい値：これ以下の振幅のデータは無視する（％の値）
	int		thresh_area;			//面積しきい値：これ以下の面積のデータは無視する
	R_TRANS_INDEX	*edge_index;	//エッジ候補点構造体
	int		edge_index_count;		//エッジ候補点数
}R_TRANS_DATA;

// --- prototype ---
int R_trans_data_open( R_TRANS_DATA *trans, int type_data, int count_data_max );							// １次元データ配列構造体のオープン
int R_trans_data_close( R_TRANS_DATA *trans );																// １次元データ配列構造体のクローズ
int R_trans_edge_detect_int( R_TRANS_DATA *trans, int *data, int data_count, int mode );
int R_trans_edge_detect_grayimage( R_TRANS_DATA *trans, int src_mem_no, int posi_x, int posi_y, int length, int mode );

#endif	// R_TRANS_H_
