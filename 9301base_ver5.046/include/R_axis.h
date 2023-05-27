/******************************************************************************
	R_AXIS.H / R_AXIS.C
	
	変更履歴
	Ver 3.10	1999/04/14	release
	Ver 3.22	2000/04/05	DR-CY-72A対応
	Ver 3.27	2000/08/07	DR-CY-72A Ｓ字駆動対応
	Ver 3.27	2000/08/07	DR-CY-72A 加速減速個別設定対応
	Ver 3.275b	2000/10/31	set関数順序合否判定フラグ追加
	Ver 3.28	2000/10/31	DR-CY-57B Initial後move関数不具合修正
	Ver 3.34	2002/12/26	DR-CY-90対応
	Ver 3.34	2003/03/25	R_axis_closeバグ修正、AXISﾒﾝﾊﾞにid追加
	Ver 3.35	2003/02/20	DR-CY-88対応
	Ver 3.50	2009/02/13	DOS & LINUX ソース統合
	Ver.3.50	2009/07/15	内部関数static化とプロトタイプ宣言追加
	Ver 4.01	2011/07/05	DR-CY-132/134対応
	Ver 4.01	2012/04/05	マルチプロセス対応
	Ver 4.01	2012/09/11	AXIS構造体にbase、s_flag追加
	Ver 4.01	2013/09/05	AXIS構造体にstart_mode追加
	Ver 4.01	2016/06/07	AXIS構造体のヌルポインタチェック追加
	Ver 4.01	2017/04/25	DR-CY-132/134のFPGAバージョンチェック追加
	Ver 5.010	2019/01/08	DR-CY-144対応
******************************************************************************/

#ifndef	R_AXIS_H_
#define	R_AXIS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define	DR_CY_57	1
#define	DR_CZ_65	2
#define	DR_CY_68	3
#define	DR_CY_72	4
#define	DR_CY_88	5
#define	DR_CY_132	7

#define	DEVICE_X3301		1
#define	DEVICE_PCL240MS		2
#define	DEVICE_PCL5022		3
#define	DEVICE_FPGA_TL		4

// オープン関数エラー要因
#define	AXIS_ALLOCATED		(-1)		// 既にＡＸＩＳ構造体が、領域確保されている。
#define	AXIS_NOT_EXIST		(-2)		// 該当する基板が存在しない。
#define	AXIS_ALLOC_ERR		(-3)		// 領域確保できない。
#define	AXIS_NOT_AVAILABLE	(-4)		// 基板が使用不可能。
										// 　DR-CY-132/134のFPGAバージョンが古いなど。

// 現在位置管理方法設定
#define	WITHOUT_FEEDBACK	0x00000000	// 帰還パルスを利用しない。
#define	WITH_FEEDBACK		0x00000001	// 帰還パルスを利用する。

// パルス出力方式設定
#define	PULSE_2CLOCK		0x00000000	// パルス発振を２クロック方式で行う。
#define	PULSE_GATE			0x00000010	// パルス発振をゲート方式で行う。

// 原点センサ論理設定
#define	ORG_ACTIVE_LOW		0x00000000	// 原点センサ負論理
#define	ORG_ACTIVE_HIGH		0x00000100	// 原点センサ正論理

// フィードバックパルスカウント方式設定
#define	PHASEX1				0x0000		// ２相クロック１逓倍（Ａ相、Ｂ相）
#define	PHASEX2				0x1000		// ２相クロック２逓倍（Ａ相、Ｂ相）
#define	PHASEX4				0x2000		// ２相クロック４逓倍（Ａ相、Ｂ相）
#define	BICLOCK				0x3000		// ２クロック負論理（ＣＷ、ＣＣＷ）

// 発振パルス発生方向設定
#define	FORWARD_CW			0x00000		// ＋方向発振がＣＷ
#define	FORWARD_CCW			0x10000		// ＋方向発振がＣＣＷ
#define	ForwardCW(nMode)	!((nMode) & FORWARD_CCW)
#define	ForwardCCW(nMode)	((nMode) & FORWARD_CCW)

// フィードバックパルス方向設定
#define	ENCODER_NORMAL		0x00000		// エンコーダカウント方向正転
#define	ENCODER_REVERSE		0x20000		// エンコーダカウント方向逆転
#define	EncoderNormal(nMode)	!((nMode) & ENCODER_REVERSE)
#define	EncoderReverse(nMode)	((nMode) & ENCODER_REVERSE)

// 原点復帰動作設定
#define	AXIS_INIT_FORWARD	0x0			// +方向に原点復帰
#define	AXIS_INIT_BACK		0x1			// -方向に原点復帰
#define	AXIS_INIT_ORG		0x00		// ＯＲＧのＯＮで停止。
#define	AXIS_INIT_EZ		0x10		// ＯＲＧがＯＮした後のＥＺのＯＮで停止。

// 
#define	AXIS_INFINITY_FORWARD	0x0
#define	AXIS_INFINITY_BACK		0x1

// 基板ＩＤ設定
#define	AXIS_CHANNEL_0	0x00000
#define	AXIS_CHANNEL_1	0x01000
#define	AXIS_CHANNEL_2	0x02000
#define	AXIS_CHANNEL_3	0x03000
#define	AXIS_CHANNEL_4	0x04000
#define	AXIS_CHANNEL_5	0x05000
#define	AXIS_CHANNEL_6	0x06000
#define	AXIS_CHANNEL_7	0x07000
#define	AXIS_CHANNEL_8	0x08000
#define	AXIS_CHANNEL_9	0x09000
#define	AXIS_CHANNEL_A	0x0A000
#define	AXIS_CHANNEL_B	0x0B000
#define	AXIS_CHANNEL_C	0x0C000
#define	AXIS_CHANNEL_D	0x0D000
#define	AXIS_CHANNEL_E	0x0E000
#define	AXIS_CHANNEL_F	0x0F000
#define	AXIS_CHANNEL(n)	(n<<12)

// デバイス指定
#define	AXIS_DEVICE_1	0x0104
#define	AXIS_DEVICE_2	0x0504
#define	AXIS_DEVICE_3	0x0904
#define	AXIS_DEVICE_4	0x0D04

#define	AXIS_PORT(rdsw,num)	(0x104|(num<<10)|(rdsw<<12))

#define	FH1_SSSD		0x15			// ＦＨ１高速スタート
#define	FH2_SSSD		0x17			// ＦＨ２高速スタート
#define	FL_CONST		0x10			// ＦＬ定速スタート
#define	FH1_CONST		0x11			// ＦＨ１定速スタート
#define	FH2_CONST		0x13			// ＦＨ２定速スタート

#define	WITH_LIMIT_CHECK	(int)0x0000	// リミットチェックを行う
#define	WITHOUT_LIMIT_CHECK	(int)0x0100	// リミットチェックを行わない
#define	WithLimitCheck(nMode)	!((nMode) & WITHOUT_LIMIT_CHECK)

#define	RANGE_DEFAULT		15			// 周波数倍率デフォルト値
#define	FL_DEFAULT			1000		// 
#define	FH1_DEFAULT			30000		// 
#define	FH2_DEFAULT			10000		// 
#define	ACC_DEFAULT			10			// 加減速時間デフォルト値
#define	ALPHA_DEFAULT		100			// Ｓ字加減速領域デフォルト値

typedef struct {
	int		id;							// ＩＤ（ＲＤＳＷ）
	int		board;						// 基板名
	int		axis;						// 軸
	int		device;						// デバイス名
	int		fd;
	unsigned long	port;				// ポートアドレス
	double	range;						// 周波数倍率

	int		nFreqFL;					// 自起動周波数	added by sugimoto 2000/09/28
	int		nFreqFH1;					// 最高周波数	added by sugimoto 2000/09/28
	int		nFreqFH2;					// 最高周波数	added by sugimoto 2000/09/28

	int		reg_range;					// 周波数倍率レジスタ値 added by sugimoto 2000/08/07
	int		freq_min;					// 自起動周波数レジスタ値
	int		freq_max1;					// 最高周波数レジスタ値
	int		freq_max2;					// 最高周波数レジスタ値
	int		nTgUp;						// 加速時間 added by sugimoto 2000/08/07
	int		nTgDown;					// 減速時間 added by sugimoto 2000/08/07
	int		nRegAccUp;					// 加速レジスタ値	acc->nRegAccUp revised by sugimoto 2000/08/07
	int		nRegAccDown;				// 減速レジスタ値	added by sugimoto 2000/08/07
	int		nAlphaUp;					// Ｓ字加速領域		alpha->nAlphaUp revised by sugimoto 2000/08/07
	int		nAlphaDown;					// Ｓ字減速領域		added by sugimoto 2000/08/07
	int		limit_cw;					// 移動最大点（リミット）ｃｗ方向
	int		limit_ccw;					// 移動最大点（リミット）ｃｃｗ方向
	int		mode;
	
	int		manual;						// マニュアル動作時動作モード
	int		destination_point;			// 移動目標点
	int		back_point;					// 移動開始点

//added by sugimoto 00/08/07 ->
	int		nRegAlphaUp;				// P5023 R16	
	int		nRegAlphaDown;				// P5023 R17	
	int		nFH1TriangleBorder1;		// FH1三角駆動条件境界点1	
	int		nFH1TriangleBorder2;		// FH1三角駆動条件境界点2	
	int		nFH1TriangleBorder3;		// FH1三角駆動条件境界点3	
	double	lfFH1AutoBorder;			// FH1ＦＨオート補正境界点	added by sugimoto 2000/09/28
	int		nFH2TriangleBorder1;		// FH2三角駆動条件境界点1	
	int		nFH2TriangleBorder2;		// FH2三角駆動条件境界点2	
	int		nFH2TriangleBorder3;		// FH2三角駆動条件境界点3	
	double	lfFH2AutoBorder;			// FH2ＦＨオート補正境界点	added by sugimoto 2000/09/28
//added by sugimoto 00/08/07 -<

	int		nSet_Flag;					// set関数実行フラグ		added by sugimoto 2000/09/28

	int		nRMDReg;					// PCL5022動作モードレジスタ	added by sugimoto 2000/12/07
	int		nCMD2;						// X3301動作制御コマンド	added by sugimoto 2000/10/31
	
	const char*	board_name;
	unsigned long	base;
	int		s_flag;
	int		r_delay;
	
	int		start_mode;
} AXIS;




int		RDrcy72Open(int i, int j);
int		RDrcy88Open(int i, int j);
int		RDrcy90Open(int i, int j);

int		RDrcy72DriverOpen(int fd, uintptr_t header);
int		RDrcy90DriverOpen(int fd, uintptr_t header);

int		r_axis_regist_id(int id, int type);
#define	PCI_AXIS_DRCY72		1
#define	PCI_AXIS_DRCY90		2
#define	PCI_AXIS_DRCY132	3
#define	PCI_AXIS_DRCY134	4
#define	PCI_AXIS_DRCY144	5

// プロトタイプ宣言

int		R_axis_board_is_present(int rdsw);

/*!
	@brief	パルス発生基板のオープン
	
	@param	axis_p	サーボ構造体ポインタのアドレス
	@param	channel	パルス発生基板のID番号（基板上のRDSWで設定）と軸番号のOR
				定 数					意 味
				AXIS_CHANNEL_0			ID = 0
				AXIS_CHANNEL_1			ID = 1
				AXIS_CHANNEL_2			ID = 2
				AXIS_CHANNEL_3			ID = 3
				AXIS_CHANNEL_4			ID = 4
				AXIS_CHANNEL_5			ID = 5
				AXIS_CHANNEL_6			ID = 6
				AXIS_CHANNEL_7			ID = 7
				AXIS_CHANNEL_8			ID = 8
				AXIS_CHANNEL_9			ID = 9
				AXIS_CHANNEL_A			ID = A
				AXIS_CHANNEL_B			ID = B
				AXIS_CHANNEL_C			ID = C
				AXIS_CHANNEL_D			ID = D
				AXIS_CHANNEL_E			ID = E
				AXIS_CHANNEL_F			ID = F
				AXIS_CHANNEL(n)			ID = n
				定 数					意 味
				AXIS_DEVICE_1			１軸目
				AXIS_DEVICE_2			２軸目
				AXIS_DEVICE_3			３軸目
				AXIS_DEVICE_4			４軸目
	@param		mode
				定 数					意 味
				WITH_LIMIT_CHECK		リミットチェックを行う
				WITHOUT_LIMIT_CHECK		リミットチェックを行わない
				定 数					意 味
				PULSE_2CLOCK			パルス発振を２クロック方式で行う
				PULSE_GATE				パルス発振をゲート方式で行う
				定 数					意 味
				PHASEX1					２相クロック１逓倍（Ａ相、Ｂ相）
				PHASEX2					２相クロック２逓倍（Ａ相、Ｂ相）
				PHASEX4					２相クロック４逓倍（Ａ相、Ｂ相）
				BICLOCK					２クロック負論理（ＣＷ、ＣＣＷ）
				定 数					意 味
				FORWARD_CW				＋方向指令でＣＷ発振
				FORWARD_CCW				＋方向指令でＣＣＷ発振
				定 数					意 味
				ORG_ACTIVE_LOW			原点センサ負論理
				ORG_ACTIVE_HIGH			原点センサ正論理
				定 数					意 味
	@return
			OK						正常終了
			AXIS_ALLOCATED			既にＡＸＩＳ構造体が、領域確保されている
			AXIS_NOT_EXIST			該当する基板が存在しない
			AXIS_ALLOC_ERR			領域確保できない
*/
int		R_axis_open(AXIS** axis_p, int channel, int mode);
int		R_axis_close(AXIS* lpAxis);

// パラメータ設定関数
int		R_axis_set_range(AXIS* lpAxis, int nRange);
int		R_axis_set_freq_min(AXIS* lpAxis, int nFreq);
int		R_axis_set_freq_max1(AXIS* lpAxis, int nFreq);
int		R_axis_set_freq_max2(AXIS* lpAxis, int nFreq);
int		R_axis_set_acc(AXIS* lpAxis, int nTg, int nAlpha);
/*以下の行を追加*/
int		R_axis_set_acc2(AXIS* lpAxis, int nTga, int nTgb, int nAlpha1, int nAlpha2);

// ソフトウェアリミット設定関数
int		R_axis_set_limit_cw(AXIS* lpAxis, int nLimit);
int		R_axis_set_limit_ccw(AXIS* lpAxis, int nLimit);

// 原点復帰動作関数
int		R_axis_initial(AXIS* lpAxis, int mode);

int		R_axis_reset(AXIS* lpAxis);
int		R_axis_stop(AXIS* lpAxis);
int		R_axis_down(AXIS* lpAxis);

// 位置決め動作関数
int		R_axis_move_absolute(AXIS* lpAxis, int nMode, int nPoint);
int		R_axis_move_relative(AXIS* lpAxis, int nMode, int nOffset);
int		R_axis_move_increase(AXIS* lpAxis);
int		R_axis_move_decrease(AXIS* lpAxis);

// 非常停止復帰関数
int		R_axis_emergency_move(AXIS* lpAxis, int nMode);
int		R_axis_emergency_back(AXIS* lpAxis, int nMode);

// 無限パルス発振関数
int		R_axis_move_infinity(AXIS* lpAxis, int nMode, int nDirection);

// 現在位置カウンタ関数
int		R_axis_get_position(AXIS* lpAxis);
int		R_axis_set_position(AXIS* lpAxis, int nCounter);
int		R_axis_reset_position(AXIS* lpAxis);

// ダウンカウンタ関数
int		R_axis_get_down_counter(AXIS* lpAxis);

// 汎用出力関数
int		R_axis_clr_oneshot(AXIS* lpAxis);
int		R_axis_clr_on(AXIS* lpAxis);
int		R_axis_clr_off(AXIS* lpAxis);
int		R_axis_res_on(AXIS* lpAxis);
int		R_axis_res_off(AXIS* lpAxis);

// ステータス関数
int		R_axis_get_status(AXIS* lpAxis);
int		R_axis_on_move(AXIS* lpAxis);
int		R_axis_on_inp(AXIS* lpAxis);
int		R_axis_on_orgi(AXIS* lpAxis);
int		R_axis_on_ez(AXIS* lpAxis);
int		R_axis_on_el_cw(AXIS* lpAxis);
int		R_axis_on_el_ccw(AXIS* lpAxis);
int		R_axis_on_alm(AXIS* lpAxis);

// ２軸直線補間関数
int		R_axis2_reset(AXIS* lpAxis1, AXIS *lpAxis2);
int		R_axis2_move_absolute(AXIS* lpAxis1, AXIS* lpAxis2, int nMode, int nPoint1, int nPoint2);
int		R_axis2_move_relative(AXIS* lpAxis1, AXIS* lpAxis2, int nMode, int nOffset1, int nOffset2);
int		R_axis2_emergency_back(AXIS* lpAxis1, AXIS* lpAxis2, int nMode);
int		R_axis2_emergency_move(AXIS* lpAxis1, AXIS* lpAxis2, int nMode);

int		R_axis_count_of_drcy132(void);
int		R_axis_count_of_drcy144(void);

#ifdef __GNUC__
void	r_axis_init_channel(void);
int	r_axis_regist_channel(int virtual_channel, int physical_channel);
int	r_axis_virtual_channel(int physical_channel);
int	r_axis_physical_channel(int virtual_channel);
#endif

#ifdef __cplusplus
}
#endif

#endif	// R_AXIS_H_
