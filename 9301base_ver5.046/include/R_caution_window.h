/******************************************************************************
	R_caution_window.h
	
	変更履歴
	Ver 4.01	2015.10.14	新規追加
******************************************************************************/

/*!
	@file	R_caution_window.h
	
	@brief	警告ウィンドウインタフェースライブラリ
	
	X Window Systemでのみ使用可能です。@n
	検査ソフトウェアが警告状態であることを@n
	X Window System画面上に最大サイズのウィンドウで表示します。@n
	この警告ウィンドウは透過表示されるので、@n
	検査ソフトの画面も確認することが可能です。@n
*/

#ifndef R_CAUTION_WINDOW_H
#define R_CAUTION_WINDOW_H

#ifdef __cplusplus
extern "C" {
#endif

/*!
	@brief	警告ウィンドウ・インタフェースのオープン
	
	警告ウィンドウとのインタフェースをオープンします。@n
	警告ウィンドウソフト"CautionWindow"を起動しておいてください。@n
	この関数で警告ウィンドウソフトが起動しているかを確認します。@n
	警告ウィンドウには複数のプロセスの警告文が同時に表示されますので、@n
	プロセスの識別ができるようなタイトルを指定してください。@n
	警告ウィンドウには、下記の様に表示されます。@n
	　[ タイトル文字列 ]@n
	　　警告文字列
	
	@param	title	タイトル文字列
	
	@return	実行結果@n
			OK　　　　正常終了@n
			OK以外　　異常終了
*/
int R_caution_window_open(const char* title);

/*!
	@brief	警告文字列の設定
	
	警告ウィンドウに表示する警告文字列を設定します。
	
	@param	string	警告文字列
	
	@return	実行結果@n
			OK　　　　正常終了@n
			OK以外　　異常終了
*/
int R_caution_window_set_string(const char* string);

/*!
	@brief	警告ウィンドウ状態の設定
	
	@param	on_off	警告状態@n
			ON  　　　警告状態有効@n
			OFF 　　　警告状態無効
	
	@return	実行結果@n
			OK　　　　正常終了@n
			OK以外　　異常終了
*/
int R_caution_window_activate(int on_off);

/*!
	@brief	警告ウィンドウ状態の取得
	
	警告ウィンドウプロセスが正常に動作しているかを確認します。@n
	警告ウィンドウプロセスが何らかの原因で異常終了してしまった場合、@n
	警告ウィンドウが表示されなくなってしまいます。@n
	この関数でウィンドウプロセスが維持されているかを定期的に確認してください。@n
	警告状態を有効にしている状態で警告ウィンドウプロセスが終了した場合、@n
	警告ウィンドウを再起動するようにメッセージを表示します。
	
	@return	警告ウィンドウ状態@n
			0以外　　警告状態有効@n
			0　　　　警告状態無効
*/
int	R_caution_window_is_active(void);

/**
*	@brief	警告ウィンドウ・インタフェースのクローズ
*
*	@return	実行結果@n
*			OK　　　　正常終了@n
*			OK以外　　異常終了
*/
int R_caution_window_close(void);

#ifdef __cplusplus
}
#endif

#endif // R_CAUTION_WINDOW_H
