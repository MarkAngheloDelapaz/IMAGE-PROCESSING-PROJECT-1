/******************************************************************************
	R_sio2.h / R_sio2.c
	
	変更履歴
	Ver 3.32	2002/09/27	正式リリース							by araki
	Ver 3.43	2004.08.06	同一ポートのボーレート変更対応			by matsu
							R_sio2_close後R_sio2_openにて再設定する
	Ver 4.01	2009/08/20	DOS & LINUX ソース統合					by araki
******************************************************************************/

#ifndef	R_SIO2_H_
#define	R_SIO2_H_

// チャンネル
									//	DOS		Linux
#define	SIO_CHANNEL0	0x10000		//	COM1	/dev/ttyS0
#define	SIO_CHANNEL1	0x20000		//	COM2	/dev/ttyS1
#define	SIO_CHANNEL2	0x30000		//	COM3	/dev/ttyS2
#define	SIO_CHANNEL3	0x40000		//	COM4	/dev/ttyS3

#define	SIO_CHANNEL(num)	(((num)+1)<<16)
#ifdef __DOS__
	#define	SIO_CHANNEL_MASK	0xF0000
#elif __GNUC__
	#define	SIO_USB				0x20000000
	#define	SIO_ACM				0x40000000
	#define	SIO_CHANNEL_MASK	(0xF0000 | SIO_USB | SIO_ACM)
#endif

// データビット長
#define	BIT_5		0x00
#define	BIT_6		0x01
#define	BIT_7		0x02
#define	BIT_8		0x03

// ストップビット長
#define	STOP_1		0x00
#define	STOP_2		0x04

// パリティ
#define	PARITY_NONE	0x00
#define	PARITY_ODD	0x08
#define	PARITY_EVEN	0x18
#ifdef __DOS__
	#define	PARITY_1	0x28
	#define	PARITY_0	0x38
#endif

// 通信速度
#define	RATE115200BPS	115200
#define	RATE57600BPS	57600
#ifdef __DOS__
	#define	RATE28800BPS	28800
	#define	RATE23040BPS	23040
#endif
#define	RATE38400BPS	38400
#define	RATE19200BPS	19200
#define	RATE9600BPS		9600
#define	RATE4800BPS		4800
#define	RATE2400BPS		2400
#define	RATE1200BPS		1200
#define	RATE600BPS		600
#define	RATE300BPS		300
#define	RATE150BPS		150
#define	RATE75BPS		75

// フロー制御
#define	FLOW_NONE		0x00
#define	FLOW_SOFT		0x01
#define	FLOW_HARD		0x02

#ifdef __cplusplus
extern "C" {
#endif

/*
	関数名：	Ｒ＿ｓｉｏ２＿ｏｐｅｎ

	機　能：	ＲＳ２３２Ｃドライバ初期化（複数チャンネル対応）

	形　式：	#include "R_sio2.h"
				int R_sio2_open( int channel, int bps, int len, int parity, int stops, int flow );

	引き数：	channel	初期化チャンネル
					値		定　数			意　味
											DOSの場合		Linuxの場合
					0x10000	SIO_CHANNEL0	COM0			/dev/ttyS0
					0x20000	SIO_CHANNEL1	COM1			/dev/ttyS1
					0x30000	SIO_CHANNEL2	COM2			/dev/ttyS2
					0x40000	SIO_CHANNEL3	COM3			/dev/ttyS3
					ＤＯＳの場合
					　channelに相当するポートをBIOSが認識していない場合、上記チャンネルにorして
					　強制的にベースアドレスポートを指定して初期化することが出来ます。（留意事項を参照）
					　EX)	SIO_CHANNEL|0x2f8
					Ｌｉｎｕｘの場合
					下記ビットをorすることにより、別デバイスを初期化します
					0x20000000		SIO_USB		/dev/ttyUSB?	ＵＳＢシリアル
					0x40000000		SIO_ACM		/dev/ttyACM?	ＣＤＣデバイス

				bps	通信速度
					値		定　数			意　味
					115200	RATE115200BPS	１１５２００ｂｐｓ
					57600	RATE57600BPS	５７６００ｂｐｓ
				*	28800	RATE28800BPS	２８８００ｂｐｓ
				*	23040	RATE23040BPS	２３０４０ｂｐｓ
					38400	RATE38400BPS	３８４００ｂｐｓ
					19200	RATE19200BPS	１９２００ｂｐｓ
					9600	RATE9600BPS		９６００ｂｐｓ
					4800	RATE4800BPS		４８００ｂｐｓ
					2400	RATE2400BPS		２４００ｂｐｓ
					1200	RATE1200BPS		１２００ｂｐｓ
					600		RATE600BPS		６００ｂｐｓ
					300		RATE300BPS		３００ｂｐｓ
					150		RATE150BPS		１５０ｂｐｓ
					75		RATE75BPS		７５ｂｐｓ
					＊　28800bps,23040bpsは、ＤＯＳの場合のみ使用可能

				len	データ長
					値		定　数		意　味
					0x00	BIT_5		５ビット
					0x01	BIT_6		６ビット
					0x02	BIT_7		７ビット
					0x03	BIT_8		８ビット

				parity	パリティ
					値		定　数		意　味
					0x00	PARITY_NONE	なし
					0x08	PARITY_ODD	奇数パリティ
					0x18	PARITY_EVEN	偶数パリティ
				*	0x28	PARITY_1	０固定
				*	0x38	PARITY_0	１固定
					＊　０固定、１固定は、ＤＯＳの場合のみ使用可能

				stops	ストップビット
					値		定　数		意　味
					0x00	STOP_1		１ビット
					0x04	STOP_2		２ビット

				flow	フロー制御
					値		定　数		意　味
					0x00	FLOW_NONE	フロー制御なし
					0x01	FLOW_SOFT	ソフトフロー（ＸＯＮ／ＸＯＦＦ）制御
					0x02	FLOW_HARD	ハードフロー（ＣＴＳ／ＲＴＳ）制御
	戻り値：
				値						意　味
				ハンドル番号(1以上)		正常終了しました。
				0						異常終了しました。（引数異常、指定ポートがＢＩＯＳチェックで存在しない。）

	解　説：	指定されたチャンネルを使用可能にします。
	留意事項：	チャンネル指定で、ポートを指定した場合、ポート指定が間違いかどうかのチェックのみになります。
				ポートが存在していなくても、エラーにはなりません。
*/
int		R_sio2_open(int channel, int bps, int len, int parity, int stops, int flow);




/*
	関数名：	Ｒ＿ｓｉｏ２＿ｃｌｏｓｅ

	機　能：	ＲＳ２３２Ｃ終了処理

	形　式：	#include "R_sio2.h"
				void R_sio2_close( int nHandle );

	引き数：	nHandle		ハンドル番号

	戻り値：	ありません。

	解　説：	Ｒ＿ｓｉｏ２＿ｏｐｅｎでオープンされたチャネルの使用を終了します。

	留意事項：	特にありません。
*/
void	R_sio2_close(int nHandle);




/*
	関数名：	Ｒ＿ｓｉｏ２＿ｇｅｔｃ

	機　能：	受信文字の取得

	形　式：	#include "R_sio2.h"
				int R_sio2_getc(int nHandle );

	引き数：	nHandle  ハンドル番号

	戻り値：	受信文字がある場合そのコード（０ｘ００～０ｘｆｆ）
	　　　　　　受信バッファがからの場合-1

	解　説：	受信バッファから１文字取り出します。

	留意事項：	特にありません。
*/
int		R_sio2_getc(int nHandle);




/*
	関数名：	Ｒ＿ｓｉｏ２＿ｐｕｔｃ

	機　能：	１文字送信

	形　式：	#include "R_sio2.h"
				void R_sio2_putc( int nHandle, int data );

	引き数：	nHandle ハンドル番号
				data	送信文字コード

	戻り値：	正常終了	　０
	　　　　　　送信失敗	-1

	解　説：	指定された文字を送信します。

	留意事項：	特にありません。
*/
int		R_sio2_putc(int nHandle, int data);




/*
	関数名：	Ｒ＿ｓｉｏ２＿ｐｕｔｓ

	機　能：	文字列送信

	形　式：	#include "R_sio2.h"
				void R_sio2_puts( int nHandle, char* str );

	引き数：	nHandle		ハンドル番号
				str			送信文字列ポインタ

	戻り値：	正常終了	　０
	　　　　　　異常終了	-1		送信バッファが一杯

	解　説：	指定された文字列を送信します。

	留意事項：	特にありません。
*/
int		R_sio2_puts(int nHandle, char* str);




// １行読み込み関数
// 改行文字までの文字をバッファに読み込む。
// 改行文字がなくても、指定した文字数を読み込めば終了する。
// タイムアップ時間を指定でき、タイムアップすればエラー。
// ただし、タイムアップまで読み込んだ文字はバッファに格納される。
// １）改行文字で終了			→正常終了 return OK
// ２）指定文字数まで読んで終了	→正常終了 return OK
// ３）タイムアップで終了		→異常終了 return ERROR
// どのパターンで終了しても、バッファの中の最後の文字のあとに必ず'\0'が付加されるので、
// 読み込み文字数は、strlenなどで確認できる。
int		R_sio2_gets(int nHandle, char* str, int size, int time_limit);




/*
	関数名：	Ｒ＿ｓｉｏ２＿ｓｎｄｂｕｆｆ

	機　能：	送信バッファの未送信文字数を得る

	形　式：	#include "R_sio2.h"
				void R_sio2_get_sndbuff( int nHandle );

	引き数：	nHandle		ハンドル番号

	戻り値：	未送信文字数

	解　説：	まだ送信されずにバッファ内に残っているも字数を得ます。

	留意事項：	特にありません。
*/
int		R_sio2_get_sndbuff(int nHandle);



/*
	関数名：	Ｒ＿ｓｉｏ２＿ｒｃｖｂｕｆｆ

	機　能：	受信バッファの文字数を得る

	形　式：	#include "R_sio2.h"
				void R_getsio_rcvbuff(int nHandle);

	引き数：	nHandle		ハンドル番号

	戻り値：	未送信文字数

	解　説：	受信バッファにたまっている文字数を得ます。

	留意事項：	特にありません。
*/
int		R_sio2_get_rcvbuff(int nHandle);




/*
	関数名：	Ｒ＿ｇｅｔ＿ｌｉｎｅ＿ｓｔａｔｕｓ

	機　能：	ライン状態を返す
*/
int		R_sio2_get_line_status(int nHandle);

#ifdef __cplusplus
}
#endif

#endif	// R_SIO2_H_
