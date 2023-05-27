/******************************************************************************
	R_P5022.H / R_P5022.C
	
	変更履歴
	Ver 3.10	1999/04/14	release
	Ver 3.20	2000/01/18	三菱Ｊ２対応
	Ver 3.24	2000/04/05	DR-CY-72A対応
	Ver 3.24	2000/04/06	２軸直線補間バグ修正
	Ver 3.27	2000/08/07	Ｓ字加減速駆動対応
	Ver 3.27	2000/08/07	加速、減速個別設定対応
	Ver 3.27	2000/08/08	set_acc関数バグ修正
	Ver 3.27	2000/08/24	Start関数バグ修正
	Ver 3.27	2000/08/24	加減速の片方だけ直線駆動に対応
	Ver 3.273	2000/09/28	加減速の個別設定不可に修正（不具合対策）
	Ver 3.273	2000/09/28	move時set_acc関数自動計算対応
	Ver 3.273	2000/09/28	on関数io取得方法変更
	Ver 3.273	2000/09/28	move関数実行時busy信号on待ち抜け修正
	Ver 3.274	2000/10/10	加減速の個別設定可に修正
	Ver 3.274	2000/10/10	move時set_acc関数自動計算修正
	Ver 3.275	2000/10/25	R_axis_move_infinity不具合修正
	Ver 3.31b	2002/01/25	FL,FH同一値入力時対応
	Ver 3.33	2002/12/26	R_PCL5022_get_position関数バグフィクス
	Ver 3.49	2006/12/18	R_PCL5022_move系関数の戻り値追加
	Ver 3.49	2006/12/20	R_PCL5022_initial不具合修正
	Ver 3.50	2008/04/08	R_PCL5022_on_～の戻り値コメント修正
	Ver 3.50	2009/02/12	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_P5022_H_
#define	R_P5022_H_

int		R_PCL5022_open(AXIS* lpAxis);

// パラメータ設定関数
int		R_PCL5022_set_range(AXIS* lpAxis, int nRange);
int		R_PCL5022_set_freq_min(AXIS* lpAxis, int nFreq);
int		R_PCL5022_set_freq_max1(AXIS* lpAxis, int nFreq);
int		R_PCL5022_set_freq_max2(AXIS* lpAxis, int nFreq);
int		R_PCL5022_set_acc(AXIS* lpAxis, int nTg, int nAlpha);
/*	追加ここから	*/
int		R_PCL5022_set_acc2(AXIS* lpAxis, int nTga, int ntgb, int nAlpha1, int nAlpha2);
/*	追加ここまで	2000/08/07 by sugimoto */

// ソフトウェアリミット設定関数
int		R_PCL5022_set_limit_cw(AXIS* lpAxis, int nLimit);
int		R_PCL5022_set_limit_ccw(AXIS* lpAxis, int nLimit);

// 原点復帰動作関数
int		R_PCL5022_initial(AXIS* lpAxis, int mode);

int		R_PCL5022_reset(AXIS* lpAxis);
int		R_PCL5022_stop(AXIS* lpAxis);
int		R_PCL5022_down(AXIS* lpAxis);

// 位置決め動作関数
int		R_PCL5022_move_absolute(AXIS* lpAxis, int nMode, int nPoint);
int		R_PCL5022_move_relative(AXIS* lpAxis, int nMode, int nOffset);
//int	R_PCL5022_move_increase(AXIS* lpAxis);
//int	R_PCL5022_move_decrease(AXIS* lpAxis);

// 非常停止復帰関数
int		R_PCL5022_emergency_move(AXIS* lpAxis, int nMode);
int		R_PCL5022_emergency_back(AXIS* lpAxis, int nMode);

// 無限パルス発振関数
int		R_PCL5022_move_infinity(AXIS* lpAxis, int nMode, int nDirection);

// 現在位置カウンタ関数
int		R_PCL5022_get_position(AXIS* lpAxis);
int		R_PCL5022_set_position(AXIS* lpAxis, int nCounter);
int		R_PCL5022_reset_position(AXIS* lpAxis);

// ダウンカウンタ関数
int		R_PCL5022_get_down_counter(AXIS* lpAxis);

// 汎用Ｉ／Ｏ関数
int		R_PCL5022_clr_on(AXIS* lpAxis);
int		R_PCL5022_clr_off(AXIS* lpAxis);
int		R_PCL5022_res_on(AXIS* lpAxis);
int		R_PCL5022_res_off(AXIS* lpAxis);

// ステータス関数
int		R_PCL5022_get_status(AXIS* lpAxis);
int		R_PCL5022_on_move(AXIS* lpAxis);
int		R_PCL5022_on_inp(AXIS* lpAxis);
int		R_PCL5022_on_orgi(AXIS* lpAxis);
int		R_PCL5022_on_ez(AXIS* lpAxis);
int		R_PCL5022_on_el_cw(AXIS* lpAxis);
int		R_PCL5022_on_el_ccw(AXIS* lpAxis);
int		R_PCL5022_on_alm(AXIS* lpAxis);

// ２軸直線補間関数
int		R_PCL5022x2_reset(AXIS* lpAxis1, AXIS *lpAxis2);
int		R_PCL5022x2_move_absolute(AXIS* lpAxis1, AXIS* lpAxis2, int nMode, int nPoint1, int nPoint2);
int		R_PCL5022x2_move_relative(AXIS* lpAxis1, AXIS* lpAxis2, int nMode, int nOffset1, int nOffset2);
int		R_PCL5022x2_emergency_back(AXIS* lpAxis1, AXIS* lpAxis2, int nMode);
int		R_PCL5022x2_emergency_move(AXIS* lpAxis1, AXIS* lpAxis2, int nMode);

#endif	// R_P5022_H_
