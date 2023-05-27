/******************************************************************************
	R_chart.h
	
	変更履歴
	Ver 4.01	2014.02.09	新規追加
	Ver 4.01	2015.05.22	R_chart_multi_alarm_～、R_chart_scatter_plot_～関数追加
	Ver 4.01	2015.05.29	R_gui_display_clock_～関数追加
******************************************************************************/

#ifndef R_CHART_H_
#define R_CHART_H_

#include "R_rect.h"
#include "R_alarm.h"
#include "R_stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	const int*	x;
	const int*	y;
	int	num;
} R_CHART;

/**
*	@brief	ヒストグラム構造体
*/
typedef struct {
	const int*	data;		/*!< ヒストグラムデータ配列 */
	int	num;				/*!< ヒストグラムデータ数（データ配列のサイズ） */
    double	start;			/*!< Ｘ軸の開始値（data[0]のＸ軸値） */
	double	pitch;			/*!< Ｘ軸のピッチ（data[n]とdata[n+1]のＸ軸値の差） */
	double	upper_limit;	/*!< 限界上限値（グラフに赤線で表示される、Ｘ軸値で指定）*/
	double	lower_limit;	/*!< 限界下限値（グラフに赤線で表示される、Ｘ軸値で指定）*/
	double	upper_control;	/*!< 管理上限値（グラフに黄線で表示される、Ｘ軸値で指定）*/
	double	lower_control;	/*!< 管理下限値（グラフに黄線で表示される、Ｘ軸値で指定）*/
	double	p_tile;			/*!< 未使用 */
} R_HISTOGRAM;

int R_chart_plot(const R_CHART* data);
int R_chart_plot_histogram(const R_HISTOGRAM* data);

/**
*	@brief	ヒストグラム表示構造体
*/
typedef struct {
	void*	p;				/*!< ユーザは使用しないでください */
} R_HISTOGRAM_CHART;

/**
*	@brief	ヒストグラムウィンドウのオープン
*
*	@param	const char*	title	タイトル
*			R_RECT*	rect		ウィンドウの表示領域（X Window画面上の座標指定）
*
*	@return	オブジェクトのポインタ
*			NULL		エラー発生
*			NULL以外	成功
*/
R_HISTOGRAM_CHART* R_histogram_chart_open(const char* title, R_RECT* rect);

/**
*	@brief	ヒストグラムウィンドウのクローズ
*
*	@param	R_HISTOGRAM_CHART*	オブジェクトのポインタ
*
*	@return
*			OK			成功
*			OK以外		失敗
*/
int	R_histogram_chart_close(R_HISTOGRAM_CHART*);

/**
*	@brief	ヒストグラムデータの描画
*
*	@param	R_HISTOGRAM_CHART*	オブジェクトのポインタ
			const R_HISTOGRAM*	data	ヒストグラムデータ
*
*	@return	オブジェクトのポインタ
*			NULL		エラー発生
*			NULL以外	成功
*/
int	R_histogram_chart_draw(R_HISTOGRAM_CHART* p, const R_HISTOGRAM* data);

/**
*	@brief	ヒストグラムデータの描画とウィンドウタイトル変更
*
*	@param	R_HISTOGRAM_CHART*	オブジェクトのポインタ
*			const R_HISTOGRAM*	data	ヒストグラムデータ
*			const char*	title	タイトル
*
*	@return	オブジェクトのポインタ
*			NULL		エラー発生
*			NULL以外	成功
*/
int	R_histogram_chart_draw_with_title(R_HISTOGRAM_CHART* p, const R_HISTOGRAM* data, const char* title);




/**
*	@brief	複数アラームカウンタ表示ウィンドウ構造体
*
*			複数アラームカウンタ表示ウィンドウで使用します。
*/
typedef struct {
	void*	p;		//!< ウィンドウオブジェクトのポインタ
} R_MULTI_ALARM_CHART;

/**
*	@brief	複数アラームカウンタ表示ウィンドウのオープン
*
*			ウィンドウオブジェクトを作成し、複数アラームカウンタ表示を表示します。
*
*	@param	const char*		title	ウィンドウの表示タイトル
*			R_RECT*			rect	ウィンドウの表示領域（X Window画面上の座標指定）
*
*	@return		ウィンドウオブジェクトへのポインタ
*		@retval		NULL以外	成功
*		@retval		NULL		エラー発生
*/
R_MULTI_ALARM_CHART* R_chart_multi_alarm_open(const char* title, R_RECT* rect);

/**
*	@brief	複数アラームカウンタ表示ウィンドウのクローズ
*
*			複数アラームカウンタ表示ウィンドウを閉じ、ウィンドウオブジェクトを解放します。
*
*	@param	R_MULTI_ALARM_CHART*	chart	ウィンドウオブジェクトへのポインタ
*
*	@return
*		@retval		OK			成功
*		@retval		OK以外		エラー発生
*/
int R_chart_multi_alarm_close(R_MULTI_ALARM_CHART* chart);

/**
*	@brief	複数アラームカウンタ表示ウィンドウのデータ変更
*
*			表示するアラームデータを変更します。
*			変更前のデータへのポインタや配列サイズの変更時に使用します。
*
*	@param	R_MULTI_ALARM_CHART*	chart	ウィンドウオブジェクトへのポインタ
*			R_ALARM**		alarm	アラーム構造体ポインタの配列へのポインタ（R_ALARM* alarm[x][y]）
*			int				x		配列のｘ
*			int				y		配列のｙ
*
*	@return
*			OK			成功
*			OK以外		エラー発生
*/
int R_chart_multi_alarm_set_alarm_struct(R_MULTI_ALARM_CHART* chart, R_ALARM** alarm, int x, int y);

/**
*	@brief	複数アラームカウンタ表示ウィンドウの更新
*
*	@param	R_MULTI_ALARM_CHART*	chart	ウィンドウオブジェクトへのポインタ
*
*	@return
*			OK			成功
*			OK以外		エラー発生
*/
int R_chart_multi_alarm_update(R_MULTI_ALARM_CHART* chart);

/**
*	@brief	複数アラームカウンタ表示セルのダブルクリック時のコールバック関数の登録
*
*	@param	R_MULTI_ALARM_CHART*	chart	ウィンドウオブジェクトへのポインタ
*			int (*func)(int x, int y, int index)	コールバック関数のポインタ
*													x: ダブルクリックされたセルのX座標
*													y: ダブルクリックされたセルのY座標
*													index: ダブルクリック時に表示されているアラーム種類のインデックス
*													対象のデータへはalarm[x][y].item[index]でアクセスできる
*
*	@return
*			OK			成功
*			OK以外		エラー発生
*/
int	R_chart_multi_alarm_set_cell_double_clicked_callback_func(R_MULTI_ALARM_CHART* chart, int (*func)(int,int,int));

/**
*	@brief	複数アラームカウンタ表示セルを赤色表示する閾値
*
*	@param	R_MULTI_ALARM_CHART*	chart	ウィンドウオブジェクトへのポインタ
*			int						limit	閾値。アラームカウンタがこの値以上であれば表示セルを赤色表示する。
*
*	@return
*			OK			成功
*			OK以外		エラー発生
*/
int	R_chart_multi_alarm_set_upper_limit(R_MULTI_ALARM_CHART* chart, int limit);

/**
*	@brief	複数アラームカウンタ表のＸ方向表示を逆転する
*
*	@param	R_MULTI_ALARM_CHART*	chart	ウィンドウオブジェクトへのポインタ
*			int						on_off	OFF:	Ｘ方向表示は、1,2,3,...(X-2),(X-1)
*											ON:		Ｘ方向表示は、(X-1),(X-2),...3,2,1
*											ウィンドウオープン時にはOFFになっている。
*
*	@return
*			OK			成功
*			OK以外		エラー発生
*/
int	R_chart_multi_alarm_set_column_reversed(R_MULTI_ALARM_CHART* chart, int on_off);	// on_off: ON/OFF

/**
*	@brief	複数アラームカウンタ表のＸ方向ヘッダの表示文字列を設定する
*
*	@param	R_MULTI_ALARM_CHART*	chart	ウィンドウオブジェクトへのポインタ
*			int						column	Ｘ方向表示位置
*			const char*				label	表示文字列
*
*	@return
*			OK			成功
*			OK以外		エラー発生
*/
int	R_chart_multi_alarm_set_column_header_label(R_MULTI_ALARM_CHART* obj, int column, const char* label);

/**
*	@brief	複数アラームカウンタ表のＹ方向ヘッダの表示文字列を設定する
*
*	@param	R_MULTI_ALARM_CHART*	chart	ウィンドウオブジェクトへのポインタ
*			int						row		Ｙ方向表示位置
*			const char*				label	表示文字列
*
*	@return
*			OK			成功
*			OK以外		エラー発生
*/
int	R_chart_multi_alarm_set_row_header_label(R_MULTI_ALARM_CHART* obj, int row, const char* label);




/**
*	@brief	散布図表示ウィンドウ構造体
*
*			散布図表示ウィンドウで使用します。
*/
typedef struct {
	void*	p;		//!< ウィンドウオブジェクトのポインタ
} R_CHART_SCATTER_PLOT;

/**
*	@brief	散布図表示ウィンドウのオープン
*
*	@param	const char*		title	ウィンドウの表示タイトル
*			R_RECT*			rect	ウィンドウの表示領域（X Window画面上の座標指定）
*
*	@return
*			R_CHART_SCATTER_PLOT*	ウィンドウオブジェクトへのポインタ
*									NULLの場合エラー発生
*/
R_CHART_SCATTER_PLOT*	R_chart_scatter_plot_open(const char* title, R_RECT* rect);

/**
*	@brief	散布図表示ウィンドウの表示データ設定
*
*	@param	R_CHART_SCATTER_PLOT*	chart	ウィンドウオブジェクトへのポインタ
*			R_POINT*				point	データ配列（point[num]）
*			int						num		データ数
*
*	@return
*			OK			成功
*			OK以外		エラー発生
*/
int	R_chart_scatter_plot_set_points(R_CHART_SCATTER_PLOT* chart, R_POINT* point, int num);

/**
*	@brief	散布図表示ウィンドウのＸスケール設定
*
*	@param	R_CHART_SCATTER_PLOT*	chart	ウィンドウオブジェクトへのポインタ
*			int						min		最小値
*			int						max		最大値
*			int						step	目盛り間隔
*
*	@return
*			OK			成功
*			OK以外		エラー発生
*/
int	R_chart_scatter_plot_set_scale_x(R_CHART_SCATTER_PLOT* chart, int min, int max, int step);

/**
*	@brief	散布図表示ウィンドウのＹスケール設定
*
*	@param	R_CHART_SCATTER_PLOT*	chart	ウィンドウオブジェクトへのポインタ
*			int						min		最小値
*			int						max		最大値
*			int						step	目盛り間隔
*
*	@return
*			OK			成功
*			OK以外		エラー発生
*/
int	R_chart_scatter_plot_set_scale_y(R_CHART_SCATTER_PLOT* chart, int min, int max, int step);

/**
*	@brief	散布図表示ウィンドウの更新
*
*	@param	R_CHART_SCATTER_PLOT*	chart	ウィンドウオブジェクトへのポインタ
*
*	@return
*			OK			成功
*			OK以外		エラー発生
*/
int	R_chart_scatter_plot_update(R_CHART_SCATTER_PLOT* chart);




/**
*	@brief	時間表示ウィンドウ構造体
*
*			時間表示ウィンドウで使用します。
*/
typedef struct {
	void*	p;		//!< ウィンドウオブジェクトのポインタ
} R_GUI_DISPLAY_CLOCK;

R_GUI_DISPLAY_CLOCK* R_gui_display_clock_open(const char* title, R_RECT* rect);
int R_gui_display_clock_start(R_GUI_DISPLAY_CLOCK* gui, R_RECT* rect);
int R_gui_display_clock_stop(R_GUI_DISPLAY_CLOCK* gui);




#ifdef __cplusplus
}
#endif

#endif // R_CHART_H
