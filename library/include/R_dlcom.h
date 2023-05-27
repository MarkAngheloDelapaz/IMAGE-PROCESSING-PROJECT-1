/******************************************************************************
	R_dlcom.h / R_dlcom.c
	
	変更履歴
	Ver 3.46		2005/06/20	正式リリース
	Ver 3.50beta48	2008/11/17	作動通信関数追加		by saito
	Ver 3.50beta60	2009/12/18	NAKｺｰﾄﾞ0x15に変更		by saito
	Ver 3.50		2009/05/29	r_dlio_reg_port関数追加
	Ver 3.50		2009/07/07	タイマ関数を変更
******************************************************************************/

#ifndef	R_DLCOM_H_
#define	R_DLCOM_H_

#ifdef __cplusplus
extern "C" {
#endif

//作動通信設定			2008.11.17 saito
//通信エラーコード
#define DLIO_COM_ERROR_INIT			-1	//作動通信初期化異常
#define DLIO_COM_ERROR_GET_START	-2	//受信時	データなし（STB bit off)
#define DLIO_COM_ERROR_PUT_START	-3	//送信時	相手側が準備できてない
#define DLIO_COM_ERROR_TIME_UP		-4	//送受信時	タイムアップエラー
#define DLIO_COM_ERROR_SUM			-5	//送受信時	チェックサムエラー
#define DLIO_COM_ERROR_RESET		-6	//送受信時	相手側リセット


//通信モード
#define DLIO_COM_SLAVE			0		//通信スレーブ設定（スキュー調整不可）
#define DLIO_COM_MASTER			1		//通信マスタ設定　（スキュー調整可能）

//制御ビット
#define DLIO_COM_RX_STB			0x01	//コマンド受信開始（入力）
#define DLIO_COM_TX_STB			0x01	//コマンド送信開始（出力）
#define DLIO_COM_TX_ACK			0x02	//コマンド送信完了（入力）
#define DLIO_COM_RX_ACK			0x02	//コマンド受信完了（出力）

//制御コード
#define DLIO_COM_CODE_DLM		0x0d	//デリミタコード
#define DLIO_COM_CODE_ACK		0x06	//ＡＣＫコード
#define DLIO_COM_CODE_NAK		0x15	//0xF9	//ＮＡＫコード（ＡＣＫの全ビット反転）

//スキュー調整
#define DLIO_COM_SKEW_RX_SETUP	0x00	//受信DATAのSETUP時間
#define DLIO_COM_SKEW_TX_SETUP	0x01	//送信DATAのSETUP時間

//作動通信関数			2008.11.17 saito
int R_dlio_com_open( int nHandle, int nMode );
int R_dlio_com_close( int nHandle );
int R_dilo_com_reset( int nHandle );
int R_dlio_com_gets( int nHandle, char *pCommand ); 
int R_dlio_com_puts( int nHandle, char *pCommand );
int R_dlio_com_set_skew( int nHandle, int nSkewMode, int nTime );
int R_dlio_com_set_timeup( int nHandle, int nTime );
void R_dlio_com_debug_disp_on ( int nHandle );
void R_dlio_com_debug_disp_off( int nHandle );
void R_dlio_com_debug_time_on ( int nHandle );
void R_dlio_com_debug_time_off( int nHandle );
void R_dlio_com_reset( int nHandle );

//デバッグ完了後、内部関数化
//void R_dlio_com_set_timeup_start( int nTime );
//void R_dlio_com_set_timeup_char ( int nTime );
//void R_dlio_com_set_timeup_str  ( int nTime );

#ifdef __cplusplus
}
#endif

#endif	// R_DLCOM_H_
