/******************************************************************************

	QRコード用ライブラリヘッダファイル

	2000.xx.xx	Ver.1.0完成				by matsu
	2001.10.12	QR_INFOにstatusを追加	by matsu
	2002. 2.28	ファインダパターン探索方法変更、エラーコード統一化	by matsu
	2002. 6.27	ファインダパターン座標を出力			by mastu
	2002.11. 5	QR-READER用関数追加		by saito
	2009.02.12	DOS & LINUX ソース統合

******************************************************************************/

#ifndef	R_QR_H_
#define	R_QR_H_

#ifdef __cplusplus
extern "C" {
#endif

// QRコードエラー訂正レベル
#define ERROR_L		1		// レベルＬ（～7%）
#define ERROR_M		0		// レベルＭ（～15%）
#define ERROR_Q		3		// レベルＱ（～25%）
#define ERROR_H		2		// レベルＨ（～30%）

// QRデコードステータスフラグ
/*
#define FINDER_ERR	0x01	// ファインダパターン検出エラー
#define VER_ERR		0x03	// バージョン算出エラー
*/
#define DECODE_OK	0x00	// デコード成功
#define ERR_READ_MEM		-9		// R_QR_readメモリmallocエラー
#define ERR_FINDER			-10		// ファインダパターン検出エラー
#define ERR_FINDER_UNDER	-11		// ファインダパターン検出不足
#define ERR_FINDER_OVER		-12		// ファインダパターン検出過多
#define ERR_FINDER_GROUP	-13		// ファインダパターングループ化エラー
#define ERR_FINDER_MEM		-19		// ファインダパターンmallocエラー
#define ERR_VERSION			-20		// バージョン決定エラー
#define ERR_CLOCK			-30		// クロック読み取りエラー
#define ERR_PATERN			-40		// クロック読み取りエラー
#define ERR_PATERN_MEM		-49		// クロック読み取りmallocエラー
#define ERR_DECODE			-50		// デコードエラー
#define ERR_DECODE_FORMAT	-51		// デコード(機能パターン)エラー
#define ERR_DECODE_CORRECT	-52		// デコード(エラー訂正)エラー
#define ERR_DECODE_MEM		-59		// デコードmallocエラー


#define BIN_TEMP	2		// 作業用２値画像メモリ番号


#define QR_BOARD_ID0 0x00	//ＱＲリーダ使用ボード ID = 0
#define QR_BOARD_ID1 0x10	//ＱＲリーダ使用ボード ID = 1
#define QR_BOARD_ID2 0x20	//ＱＲリーダ使用ボード ID = 2
#define QR_BOARD_ID3 0x30	//ＱＲリーダ使用ボード ID = 3
#define QR_CHANNEL_0 0x00	//ＱＲリーダ使用画像入力チャンネル CH = 0
#define QR_CHANNEL_1 0x01	//ＱＲリーダ使用画像入力チャンネル CH = 1
#define QR_CHANNEL_2 0x02	//ＱＲリーダ使用画像入力チャンネル CH = 2
#define QR_CHANNEL_3 0x03	//ＱＲリーダ使用画像入力チャンネル CH = 3



// 座標構造体
typedef struct {
	int x;
	int y;
} POINT_DATA;

// QRコード情報構造体
typedef struct {
	int status;				// QRデコードステータス
	int module;				// モジュール数
	int version;			// バージョン
	int model;				// モデル
	int level;				// エラー訂正レベル
	int mask;				// マスクNo
	int format1_err;		// フォーマット情報１誤りbit数					2002. 6.27 未対応
	int *format1_adr;		// フォーマット情報１誤り位置へのアドレス		2002. 6.27 未対応
	int format2_err;		// フォーマット情報２誤りbit数					2002. 6.27 未対応
	int *format2_adr;		// フォーマット情報２誤り位置へのアドレス		2002. 6.27 未対応
	int ver1_err;			// バージョン情報１誤りbit数					2002. 6.27 未対応
	int *ver1_adr;			// バージョン情報１情報誤り位置へのアドレス		2002. 6.27 未対応
	int ver2_err;			// バージョン情報２誤りbit数					2002. 6.27 未対応
	int *ver2_adr;			// バージョン情報２情報誤り位置へのアドレス		2002. 6.27 未対応
	int data_err;			// データ誤りbyte数								2002. 6.27 未対応
	int *data_adr;			// データ誤り位置へのアドレス					2002. 6.27 未対応
	POINT_DATA finder[3];	// ファインダパターン検出座標(未検出時は-1)		2002. 7. 2
} QR_INFO;


// QRコード関数プロトタイプ
int R_QR_open(void);
void R_QR_close(void);
int R_QR_gray2bin(int gray_no, int bin_no);
int R_QR_read(int bin_no, QR_INFO *qr, unsigned char *data);	// sai henkou 2002/07/01

// QRコードリーダ関数プロトタイプ
int	R_QR_start(char *str, QR_INFO *qr, int flag, int channel);
//int R_QR_start(char *data, QR_INFO *qr, char channel);
int R_QR_init(void);

#ifdef __cplusplus
}
#endif

#endif	// R_QR_H_
