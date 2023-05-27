/******************************************************************************
	R_widget.h / R_widget.c

	変更履歴
	Ver 4.01	2014.02.24	新規追加								by araki
******************************************************************************/

#ifndef R_WIDGET_H
#define R_WIDGET_H

#include "R_rect.h"
#include "R_plane.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	void*	p;
} R_IMAGE_VIEW;

/**
*	@brief	画像表示ウィンドウのオープン
*
*	@param	const char*	title	タイトル
*			R_RECT*	rect		表示領域（X Window画面上の座標指定）
*
*	@return	オブジェクトのポインタ
*			NULL		エラー発生
*			NULL以外	成功
*/
R_IMAGE_VIEW*	R_widget_image_view_open(const char* title, R_RECT* rect);

int R_widget_image_view_close(R_IMAGE_VIEW* p);

/**
*	@brief	画像の設定
*
*	@param	R_IMAGE_VIEW*	オブジェクトのポインタ
*			const R_PLANE*	plane	表示画像
*			const char*	comment	表示するコメント
*			int	code	表示判断用コード
*
*	@return	結果
*			OK		成功
*			OK以外	失敗
*/
int	R_widget_image_view_append(R_IMAGE_VIEW* p, const R_PLANE* plane, const char* comment, int code);

int	R_widget_image_view_clear(R_IMAGE_VIEW* p);




/**
*	@brief	画像表示の設定
*
*	@param	R_IMAGE_VIEW*	オブジェクトのポインタ
*			int	accept_code	表示許可コード
*
*	@return	結果
*			OK		成功
*			OK以外	失敗
*/
/*
	表示するコードをR_widget_image_view_append_accept_codeで追加していきます。
	R_widget_image_view_clear_accept_codeを実行すると、すべてのコードを表示します。
*/
int	R_widget_image_view_append_accept_code(R_IMAGE_VIEW* p, int accept_code);
int	R_widget_image_view_remove_accept_code(R_IMAGE_VIEW* p, int accept_code);
int R_widget_image_view_clear_accept_code(R_IMAGE_VIEW* p);

#ifdef __cplusplus
}
#endif

#endif // R_WIDGET_H
