/******************************************************************************

	ＴＳＣ９３０１（画像処理装置）

		２値画像処理ライブラリ用ヘッダ

	[history]
	version		date		comment
	Ver 1.02	1993.11. 1	オリジナル
	Ver 1.04	1994. 1.24	ラベリングアルゴリズム修正
	Ver 1.04	1994. 1.25	バグ修正（色テーブル作成ミス）
	Ver 1.04	1994. 2. 2	R_measure引き数変更（特徴量計算の指定追加）
	Ver 1.04	1994. 2. 2	面積順にソートする関数作成
	Ver 1.05	1994. 3. 7	 計測結果格納領域のサイズを可変とした
				1994. 3.10	小面積ブローブ消去関数を追加
				1994. 4.29	struct MEAS_DATA に page 追加（実は以前から変更されていた！！）
				1994. 4. 8	ブローブ消去関数修正
				1994. 5.23	円状の計測エリアを指定可能にした
				1994. 6. 9	アルゴリズムの説明を追加
				1994. 6.15	
				1996. 7. 1	指定面積以下ブローブ削除関数色指定追加
				1996. 7.11	ラン長指定以下無視対応追加
				1996. 7.19	指定面積以下無視対応追加
				1999.11.12	640x480対応のため、Σ２乗総和のテーブル、周囲長計算テーブルを
							R_measure_open()で初期化するようにする
				1999.09.10	640x480対応（FX_SIZE,FY_SIZE->FxSize,FySizeの変更、MxSizeの導入） by matsu
				1999.11.12	640x480対応のため、Σ２乗総和のテーブル、周囲長計算テーブルを
							R_measure_open()で初期化するようにする  by matsu
				1999.12.08	計算テーブルの開放をR_measure_close()から呼ぶようにする
				2000.07.17	cal_character3関数内のR_read_clk_countをコメントアウト
				2000.12.15	R_measure_close_table()でfree後ポインタにNULLを設定する
				2001.06.04	R_measure(),R_measure_area(),R_measure_fast()でエリアチェックを行うようにする
	Ver 3.32b	2002.10.25	エリア外の場合はエラー終了
	Ver 3.48	2006.03.28	labeling_1st （最後の）結合ランの処理の間違いを修正
	Ver 3.48	2006.06.05	labeling_1st で周囲長初期値の設定の間違いを修正
	Ver 3.49	2006.09.05	高画素対応
	Ver.3.50	2007.06.26	R_measure実行時の計測範囲チェックでサイズが小さい場合のチェックが抜けていたので追加
				2008.01.24	R_measure_open()実行時のFxSize,FySizeで初期化するのではなく基板画像サイズ最大値で初期化するように修正
				2008.09.04	オーバフロー対策・64bit演算化
				2008.09.29	R_measure_disp_blobを追加　安川
	Ver 3.50	2009.01.30	DOS & LINUX ソース統合
	Ver 4.01	2010.01.13	mpx->modeチェック廃止
				2010.05.22	軸方向が90°のときsinとcosの値が正しくなるようにした。_cal_character1関数修正
				2010.06.07	wsizeが0にならないようにした。_cal_character3関数修正（切捨てを四捨五入に変更）
				2010.10.12	DLONG構造体の宣言をR_stdlib.hへ移動
				2011.01.05	R_meas_calc_contour()で、輪郭の点数がmallocで確保した点数を超えないかチェックする部分を修正
				2012.07.20	R_get_run関数仕様変更、最大ラン数追加
				2012.07.23	ラン数管理(runsizeとrp)をunsigned shortからintに変更
				2012.08.06	R_meas_calc_contour修正。対象ブロブが１画素の場合、フリーズするのを修正。
				2016.05.27	R_meas_get_run_number関数追加
				2016.06.21	背景色は８連結処理するように変更。（親なしブロブ対策）
				2016.06.22	穴埋め領域のＸ総和とＹ総和を追加
				2016.06.24	R_sort_area2関数追加
******************************************************************************/

#ifndef	R_MEAS_H_
#define	R_MEAS_H_

#ifdef __x86_64__
#include <stdint.h>
#endif

#include "R_stdlib.h"	// 2010/10/12 by araki
#include "R_plane.h"			// 高画素対応  2006.09.05  by matsu

#ifdef __cplusplus	// 2010/10/12 by araki
extern "C" {
#endif

#define	PAI			3.141592654

//#define	MAXBLOB		2000		// 結合前の最大ブローブ数
//#define	MAXPOINT	480			// ｙ座標最大値
#define	_MAX_POINT		480			//	互換性維持のため

#define	CONV_MAX_DEFAULT		10
#define	OYAKO_MAX_DEFAULT		4000
#define	MAXBLOB_DEFAULT			1000
#define	MEASURE_WIDTH_DEFAULT	1024
#define	MEASURE_HEIGHT_DEFAULT	1024

#define	GROUP1		0			//	特徴量計算グループ
#define	GROUP2		1
#define	GROUP3		2
#define	GROUP4		3

#define	SMALL_BLACK		0
#define	SMALL_WHITE		1

#define	RM_BLACK		1
#define	RM_WHITE		2

#define RM_NORMAL	0
#define RM_RUN		1
#define RM_AREA		2

//　６４ビット数値用構造体 -> R_stdlib.hに移動	2010/10/12
//#ifndef	DLONG_DEF
//#define	DLONG_DEF
//typedef struct {
//	unsigned long	lo;
//	unsigned long	hi;
//} DLONG ;
//#endif

/*
//	イメージ構造体
#define	IMAGE_TYPE_BINARY	0
#define	IMAGE_TYPE_GRAY8	1

typedef struct {
	int type;
	int width;
	int height;
	int bytes_per_line;
	void *pData;
} rsImage;
*/

//　時間計測用
#ifdef	R_MEAS
	unsigned long		meas_time;
	unsigned long		measure_time;
	unsigned long		calc_time;
	unsigned long		calc2_time1;
	unsigned long		calc2_time2;
	unsigned long		calc2_time3;
	unsigned long		cbtime[ 500 ];
#else
	extern	unsigned long		measure_time;
	extern	unsigned long		conb_time;
	extern	unsigned long		calc_time;
	extern	unsigned long		calc2_time1;
	extern	unsigned long		calc2_time2;
	extern	unsigned long		calc2_time3;
#endif


#define	MEAS_TM_NUM					32
//---------------------------------------------------------
#define	MEAS_TM_MEASURE_MAKE_AREA_TABLE		0
#define	MEAS_TM_MEASURE_GET_RUN				1
#define	MEAS_TM_MEASURE_MODIFY_RUN			2
#define	MEAS_TM_MEASURE_LABELING_FAST		3
#define	MEAS_TM_MEASURE_LABELING_1ST		4
#define	MEAS_TM_MEASURE_LABELING_2ND		5
#define	MEAS_TM_MEASURE_GET_BLOB_SIZE		6
#define	MEAS_TM_MEASURE_CALC1				7
#define	MEAS_TM_MEASURE_CALC2				8
#define	MEAS_TM_MEASURE_CALC3				9

#define	MEAS_TM_MEASURE_TOTAL				10
//---------------------------------------------------------

#define	MEAS_ID			0x03112121
#define	MEAS_STANDARD	1000

#ifdef __x86_64__
typedef struct {
	int i_oyako_n;
	
	void*	top;
	int*		pi_conv_n;				//	結合テーブル用エリア１
	int*		pi_conv_table;			//	結合テーブル用エリア２
	int**		ppi_conv_table;			//	結合テーブル用エリア３
	uint32_t*	pul_sum_n2;		//	Σ２乗総和のテーブル
	uint32_t*	pul_perim;		//	周囲長計算テーブル初期化
	int*		pi_oyako;				//	親子テーブル初期化
	uint32_t*	pul_perim_work1;	//	周辺長作業エリア１
	uint32_t*	pul_perim_work2;	//	周辺長作業エリア２
	int*		pi_perim_work3;			//	周辺長作業エリア３
	int*		pi_child_table;			//	子供テーブル用エリア
	int*		pi_sort_work;				//	ソート用作業エリア
	unsigned short*	pus_area_work;	//	結合処理用
} MEASWORK;
#endif

#define	MEAS_MODE_COMPATIBLE	0			//	オリジナル互換モード
#define	MEAS_MODE_EXT			1			//	拡張モード
/*
enum {
	E_BMEAS_CANNOT_OPEN1 = 10000,
	E_BMEAS_CANNOT_OPEN2,
	E_BMEAS_NOT_OPENED,
	E_BMEAS_CANNOT_CREATE_AREA,
	E_BMEAS_ILLEGAL_IMAGE_TYPE,
	E_BMEAS_ILLEGAL_AREA,
	E_BMEAS_RUN_OVERFLOW,
	E_BMEAS_CONVERT_ERROR,
};
*/
typedef struct {
	int				id;

	int				mode;					// 処理モード
	int 			max_width;				// 処理可能な最大幅
	int 			max_height;				// 処理可能な最大高
	int 			conv_max;				// 結合関係最大値
	int				oyako_max;				// 親子関係最大値
	int				group;					// 計算グループ

	int				modify;					// ランテーブル編集フラグ
	int				areasize;				// 指定面積以下無視時のサイズ
	int				modicolor;				// 指定面積以下無視時の色指定
	int				errflag;				// エラー時に０以外が格納される

	R_PLANE			*pPlane;				// 処理対象画像   2006.09.05  by matsu
	int				x;						// 計測エリア左上ｘ座標
	int				y;						// 　　〃　　　　ｙ座標
	int				xl;						// 計測エリアｘ長
	int				yl;						// 　　〃　　ｙ長
	int				page;
	int				maxrun;
	int				maxblob;
	
	int				background_color;		// 背景色、0:黒 / 1:白。背景色は８連結処理する。デフォルトは0。	2016.06.21 by araki
	
	int				tm[MEAS_TM_NUM];		// 時間計測
	void			*top;					// 確保した先頭ポインタ

//　一次特徴量
//	unsigned short	bn;						// ブローブ番号最終値
//	unsigned short	runsize;				// ランの総数					（追加）
//	unsigned short	*rp;					// 各ｙ一番左のラン番号
	int				bn;						// ブローブ番号最終値	2012.07.20
	int				runsize;				// ランの総数			2012.07.20
	int*			rp;						// 各ｙ一番左のラン番号	2012.07.23
	unsigned short	*rn;					// 各ｙ上のランの数				（追加）
	unsigned short	*yaxis;					// 各ランのｙ座標				（追加）
	unsigned short	*rcol;					// 各ランの色					（追加）
	unsigned short	*runstrt;				// ランスタート位置
	unsigned short	*runleng;				// ランレングス
	unsigned short	*runnumb;				// 各ランの属するブローブ番号

	unsigned short	*parent;				// 親のブローブ番号
	unsigned short	*child;					// 子のブローブ番号
	unsigned short	*sibling;				// 兄弟のブローブ番号
	unsigned short	*color;					// 色（０：黒、１：白）
	unsigned short	*perim;					// 周囲長
	unsigned short	*nholes;				// 穴の数
	unsigned short	*imin;					// ｘ方向最小値
	unsigned short	*imax;					// ｘ方向最大値
	unsigned short	*jmin;					// ｙ方向最小値
	unsigned short	*jmax;					// ｙ方向最大値

#ifdef __x86_64__
	uint32_t*		area;					// 面積
	uint32_t*		sumi;					// ｘ方向総和
	uint32_t*		sumj;					// ｙ方向総和

	uint64_t*		sumi2;					// ｘ方向二乗和
	uint64_t*		sumij;					// ｘｙ積和
	uint64_t*		sumj2;					// ｙ方向二乗和
#else
	unsigned long	*area;					// 面積
	unsigned long	*sumi;					// ｘ方向総和
	unsigned long	*sumj;					// ｙ方向総和

	DLONG			*sumi2;					// ｘ方向二乗和
	DLONG			*sumij;					// ｘｙ積和
	DLONG			*sumj2;					// ｙ方向二乗和
#endif

	unsigned short	*sortnumb;				// 面積順ソート結果

//　二次特徴量
	unsigned short	*center_x;				// 重心ｘ座標
	unsigned short	*center_y;				// 重心ｙ座標

	double			*cos;					// 軸方向（ｃｏｓθ）
	double			*sin;					// 軸方向（ｓｉｎθ）

	unsigned short	*ellipse_a;				// 長軸長
	unsigned short	*ellipse_b;				// 短軸長

#ifdef __x86_64__
	uint32_t*		sholes;					// 穴面積
	uint32_t*		area2;					// 穴を含めた面積
	uint32_t*		sumi_of_area2;			// 穴を含めたｘ方向総和		// 2016.06.22
	uint32_t*		sumj_of_area2;			// 穴を含めたｙ方向総和		// 2016.06.22
#else
	unsigned long	*sholes;				// 穴面積
	unsigned long	*area2;					// 穴を含めた面積
	unsigned long*	sumi_of_area2;			// 穴を含めたｘ方向総和		// 2016.06.22
	unsigned long*	sumj_of_area2;			// 穴を含めたｙ方向総和		// 2016.06.22
#endif

	unsigned short	*hratio;				// 穴面積比
	unsigned short	*dhmax;					// 穴距離最大値
	unsigned short	*dhmin;					// 穴距離最小値
	unsigned short	*dhave;					// 穴距離平均値
	unsigned short	*nareas;				// 隣接エリア数
	unsigned short	*dsmax;					// 隣接エリア距離最大値
	unsigned short	*dsmin;					// 隣接エリア距離最小値
	unsigned short	*dsave;					// 隣接エリア距離平均値
	unsigned short	*dpmax;					// 外周距離最大値
	unsigned short	*dpmin;					// 外周距離最小値
	unsigned short	*dpave;					// 外周距離平均値
	unsigned short	*lsize;					// 外接長方形主軸方向幅
	unsigned short	*wsize;					// 外接長方形副軸方向幅
	unsigned short	*lsize_1;				// 外接長方形描画用
	unsigned short	*lsize_2;				// 外接長方形描画用
	unsigned short	*wsize_1;				// 外接長方形描画用
	unsigned short	*wsize_2;				// 外接長方形描画用
	unsigned short	*xdiff;					// ｘ範囲
	unsigned short	*ydiff;					// ｙ範囲
	unsigned short	*peround;				// 面積周囲長比（穴を含まない）
	unsigned short	*pps;					// 面積周囲長比（穴を含む）
#ifdef __x86_64__
	uint32_t*		xyarea;					// 外接長方形面積＃１
	uint32_t*		lwarea;					// 外接長方形面積＃２
#else
	unsigned long	*xyarea;				// 外接長方形面積＃１
	unsigned long	*lwarea;				// 外接長方形面積＃２
#endif
	unsigned short	*xyratio;				// 外接長方形縦横比＃１
	unsigned short	*wlratio;				// 外接長方形縦横比＃２
	unsigned short	*sratioxy;				// 外接長方形面積比＃１
	unsigned short	*sratiolw;				// 外接長方形面積比＃２
	unsigned short	*axisratio;				// 等価楕円主軸・副軸比
	
#ifdef __x86_64__
	//	測定領域指定
	short*			ps_marea_y;
	short*			ps_marea_x;
	short*			ps_marea_xl;
	// 作業領域
	MEASWORK*		work_area;
#endif
} MEASDATA;

typedef struct {
	unsigned short	xmin;
	unsigned short	xmax;
	unsigned short	y;
	unsigned short	yl;
	unsigned short	x[_MAX_POINT];
	unsigned short	xl[_MAX_POINT];
} MEASFORM;

typedef struct {
// グループ１特徴量
	unsigned short	imin;					// ｘ方向最小値
	unsigned short	imax;					// ｘ方向最大値
	unsigned short	jmin;					// ｙ方向最小値
	unsigned short	jmax;					// ｙ方向最大値
	
	unsigned long	area;					// 面積
	unsigned long	sumi;					// ｘ方向総和
	unsigned long	sumj;					// ｙ方向総和
	
	DLONG			sumi2;					// ｘ方向二乗和
	DLONG			sumij;					// ｘｙ積和
	DLONG			sumj2;					// ｙ方向二乗和
	
	double			center_x;				// 重心ｘ座標
	double			center_y;				// 重心ｙ座標
	
// グループ２特徴量
	double			cos;					// 軸方向（ｃｏｓθ）
	double			sin;					// 軸方向（ｓｉｎθ）
	unsigned short	ellipse_a;				// 長軸長
	unsigned short	ellipse_b;				// 短軸長
	unsigned short	axisratio;				// 等価楕円主軸・副軸比
	
} MEASCHAR;

// 特徴量格納領域確保
int R_measure_open(MEASDATA *mpx, int maxblob);
int R_measure_open_ex(MEASDATA *mpx, int maxblob, int nSizeX, int nSizeY, int nConvMax, int nOyakoMax);

// 特徴量格納領域開放
void R_measure_close( MEASDATA * );

// 特徴量演算　演算方式指定
int R_set_meas_configration( MEASDATA *mpx, int modsize, int areasize, int areacolor, int mode );

// 一次特徴量を求める
int R_measure( MEASDATA *, int, int, int, int, int, int );
// 一次特徴量を求める[高画素]
int R_measure_ex(MEASDATA *mpx, R_PLANE *pPlane, int x, int y, int xl, int yl, int mode);
int R_measure_circle_ex(MEASDATA *mpx, R_PLANE *pPlane, int x, int y, int r, int mode);

// 一次特徴量を求める（重心，面積のみ）
int R_measure_area( MEASDATA *, int, int, int, int, int );
// 一次特徴量を求める（重心，面積のみ）[高画素]
int R_measure_area_ex(MEASDATA *mpx, R_PLANE *pPlane, int x, int y, int xl, int yl);

// 一次特徴量を求める（指定面積以下無視）
//int R_measure_with_area( MEASDATA *, int, int, int, int, int, int, int );

// 面積順ブローブ番号テーブル作成
void R_sort_area( MEASDATA * );

// 全面積順ブローブ番号テーブル作成
void R_sort_area2(MEASDATA* mpx);

/*
	関数名：	R_meas_get_run_number
	機　能：	指定画素の属するラン番号取得
	形　式：	#include "R_meas.h"
				int R_meas_get_run_number(const MEASDATA* mpx, int x, int y);
	引き数：	mpx		特徴量構造体ポインタ
				x		x座標
				y		y座標
	戻り値：	ブローブ番号（負の値の場合は、範囲外）
	解　説：	座標（x, y）の画素が属するラン番号を返します。
	留意事項：	特にありません。
*/
int	R_meas_get_run_number(const MEASDATA* mpx, int x, int y);

// 指定画素の属するブローブ番号取得
int R_get_blob_number(const MEASDATA* mpx, int x, int y);

// 指定面積以下のブローブ削除
void R_del_small_blob( MEASDATA *, int );

// 指定面積以下のブローブ削除
void R_del_small_blob_with_color( MEASDATA *, int, int );

// 計測エリア指定（円）関数
int R_set_meas_circle( MEASFORM *, int, int, int );

int R_measure_form( MEASDATA *, int, MEASFORM *, int );

int R_measure_fast( MEASDATA *mpx, int mem_no, int x, int y, int xl, int yl, int color, int noise );
int R_measure_fast_ex(MEASDATA *mpx, R_PLANE *pPlane, int x, int y, int xl, int yl, int color, int noise);

int R_measure_test( MEASDATA *mpx, int mem_no, int x, int y, int xl, int yl );

// 特徴量計算テーブル開放
void R_measure_close_table( void );

//ブローブ輪郭抽出
int R_meas_calc_contour(const MEASDATA* mpx, int nBlobNumber, int hMultiPointHandle);

//ブローブを指定のBINプレーンに表示する
int R_measure_disp_blob(const MEASDATA* mpx, int page, int blobNo);

// 合成ブロブの特徴量の算出関数
int	R_measure_integrated_blob(const MEASDATA* mpx, const int* blob_no, int num_of_blobs, int group, MEASCHAR* result);

// ブロブ全体の特徴量の算出関数
int	R_measure_whole_blob(const MEASDATA* mpx, int blob_no, int group, MEASCHAR* result);

// ブロブのサブピクセル重心座標
R_DPOINT	R_measure_center_of_blob(const MEASDATA* mpx, int num);

// 包括するブロブを含めた全面積のサブピクセル重心座標
R_DPOINT	R_measure_center_of_whole_blob(const MEASDATA* mpx, int num);

//　アセンブラ関数
//int		R_get_run( long *, int, int, int, unsigned short *, unsigned short *, unsigned short * );
int		R_get_run(long*, int, int, int, unsigned short*, unsigned short*, unsigned short*, int);
int		R_get_run_ext( long *, int, int, int, unsigned short *, unsigned short *, unsigned short * );
long	R_div6432a( long, long, long, DLONG * );
void	R_mul3232( long, long, DLONG * );
void	dword_shr( long, long, long, DLONG * );
void	dword_div6( long, long, DLONG * );
#ifdef __GNUC__
	int		R_get_run_(long*, int, int, int, unsigned short*, unsigned short*, unsigned short*, int);
	long	R_div6432a_(long, long, long, DLONG*);
	void	R_mul3232_(long, long, DLONG*);
	void	dword_shr_(long, long, long, DLONG*);
#endif

#ifdef __cplusplus	// 2010/10/12 by araki
}
#endif

#endif	// R_MEAS_H_
