/******************************************************************************
	R_DRCY88.H
	
	変更履歴
		Ver 3.xx	2003/02/20	βリリース
		Ver 3.35	2003/04/7	正式リリース
		Ver 3.50	2009/02/12	DOS & LINUX ソース統合
******************************************************************************/

#ifndef	R_DRCY88_H_
#define	R_DRCY88_H_

int		r_drcy88_dac_open(int nID);
void	r_drcy88_dac_close(int nHandle);
int		R_drcy88_dac_set(int nHandle, int nChannel, int nData);

int		r_drcy88_get_apic21_port(int nHandle);
int		r_drcy88_get_register_port(int nHandle);
int		r_drcy88_set_ref(int nHandle, int nChannel, int nData);
int		r_drcy88_dac_current_initial(int nHandle);
int		r_drcy88_set_current(int nHandle,int port,int data);

// プロトタイプ宣言

#define	R_drcy88_axis_open				R_axis_open
#define	R_drcy88_axis_close				R_axis_close

// パラメータ設定関数
#define	R_drcy88_axis_set_range 		R_axis_set_range
#define	R_drcy88_axis_set_freq_min 		R_axis_set_freq_min
#define	R_drcy88_axis_set_freq_max1 	R_axis_set_freq_max1
#define	R_drcy88_axis_set_freq_max2 	R_axis_set_freq_max2
#define	R_drcy88_axis_set_acc 			R_axis_set_acc
/*以下の行を追加*/
#define	R_drcy88_axis_set_acc2 			R_axis_set_acc2

// ソフトウェアリミット設定関数
#define	R_drcy88_axis_set_limit_cw 		R_axis_set_limit_cw
#define	R_drcy88_axis_set_limit_ccw 	R_axis_set_limit_ccw

// 原点復帰動作関数
#define	R_drcy88_axis_initial 			R_axis_initial

#define	R_drcy88_axis_reset 			R_axis_reset
#define	R_drcy88_axis_stop 				R_axis_stop
#define	R_drcy88_axis_down 				R_axis_down

// 位置決め動作関数
#define	R_drcy88_axis_move_absolute 	R_axis_move_absolute
#define	R_drcy88_axis_move_relative 	R_axis_move_relative
#define	R_drcy88_axis_move_increase 	R_axis_move_increase
#define	R_drcy88_axis_move_decrease 	R_axis_move_decrease

// 非常停止復帰関数
#define	R_drcy88_axis_emergency_move 	R_axis_emergency_move
#define	R_drcy88_axis_emergency_back 	R_axis_emergency_back

// 無限パルス発振関数
#define	R_drcy88_axis_move_infinity 	R_axis_move_infinity

// 現在位置カウンタ関数
#define	R_drcy88_axis_get_position 		R_axis_get_position
#define	R_drcy88_axis_set_position 		R_axis_set_position
#define	R_drcy88_axis_reset_position 	R_axis_reset_position

// ダウンカウンタ関数
#define	R_drcy88_axis_get_down_counter 	R_axis_get_down_counter

// 汎用出力関数
#define	R_drcy88_axis_clr_oneshot 		R_axis_clr_oneshot
#define	R_drcy88_axis_clr_on 			R_axis_clr_on
#define	R_drcy88_axis_clr_off 			R_axis_clr_off
#define	R_drcy88_axis_res_on 			R_axis_res_on
#define	R_drcy88_axis_res_off 			R_axis_res_off

// ステータス関数
#define	R_drcy88_axis_get_status 		R_axis_get_status
#define	R_drcy88_axis_on_move 			R_axis_on_move
#define	R_drcy88_axis_on_inp 			R_axis_on_inp
#define	R_drcy88_axis_on_orgi 			R_axis_on_orgi
#define	R_drcy88_axis_on_ez 			R_axis_on_ez
#define	R_drcy88_axis_on_el_cw 			R_axis_on_el_cw
#define	R_drcy88_axis_on_el_ccw 		R_axis_on_el_ccw
#define	R_drcy88_axis_on_alm 			R_axis_on_alm

// ２軸直線補間関数
#define	R_drcy88_axis2_reset 			R_axis2_reset
#define	R_drcy88_axis2_move_absolute 	R_axis2_move_absolute
#define	R_drcy88_axis2_move_relative 	R_axis2_move_relative
#define	R_drcy88_axis2_emergency_back 	R_axis2_emergency_back
#define	R_drcy88_axis2_emergency_move 	R_axis2_emergency_move

// Ｉ／Ｏ関数
#define	R_drcy88_gpio_open				R_gpio_open
#define	R_drcy88_gpio_write				R_gpio_write
#define	R_drcy88_gpio_read				R_gpio_read
#define	R_drcy88_gpio_on				R_gpio_on
#define	R_drcy88_gpio_off				R_gpio_off

#endif	// R_DRCY88_H_
