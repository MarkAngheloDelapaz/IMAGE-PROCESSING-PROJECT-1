/******************************************************************************

	R_CS_CMD.H
	
	IN-CY-84コマンド通信ヘッダ
	バージョンアップ時は、必ず履歴の追加を行い、
	グループ内で変更内容をアナウンスすること！！

									Authered. by	Kennichi Sassa
									Image Processing Group
									Production System Development Division
									ROHM CO.,LTD.
									21,Saiin Mizosaki-cho,Ukyo-ku,Kyoto
									615-8585, JAPAN

	履歴
	Version		Date			Name	Comment
				2004.03.15.000
				2004.04.23.001			DrawPlane描画関数(R_draw系)を組み込み
				2004.05.11.002			ハンドシェイク失敗時は、受信したコマンドを
										全て破棄して、やり直すよう仕様変更
				2004.06.11.003			バイナリのステータス表示不具合修正
				2005.09.05.004			R_CS_CommandBusyOn,Off,ReadyOn,Off追加
				2009.05.08				R_CS_CommndSetGpioBoard(),
										R_CS_CommandSetWaitTime(),R_CS_CommandSetWaitTime()の追加
	ver1.07b1.6	2009.05.27		saito	#define __CMD_GPIO__
										#define __CMD_DLIO__
										#define __CMD_SELECTABLE__
										による切り分け廃止
										以降 __CMD_DLIO__ の動作をデフォルトとする
										切替はR_CS_SetCommunicationMode()にて設定する
										int R_CS_InitialCommand(int nBoardType);	→廃止
										int R_CS_InitialCommand();					→こちらに統一
	1.07b4.4	2012.08.23				R_CS_CommandTransmitでユーザ関数コールバック追加
	1.07b4.8	2013.04.09				DP-RAM通信追加
	1.07b4.8	2013.04.10				COMM_TIMEOUTを変数化
******************************************************************************/

// --- define ---
// ver1.07b1.6	2009.05.27 saito 	#defineによる切り分け廃止
// 
//#define __CMD_GPIO__			// GPIO系
//#define __CMD_DLIO__			// DLIO系
//#define __CMD_SELECTABLE__		// GPIO/DLIOソフトウェア切り替え

#define COMMAND_BASE	0x20
#define COMMAND_MAX		0x80
//#define COMMAND_BUFF	256					// 可変パラメータ化  2007.11.16


// ビット設定
#define RX_ENABLE		0x01				// <in > コマンド受信
#define RX_COMPLETE		0x01				// <out> コマンド受信確認
#define TX_ENABLE		0x02				// <out> コマンド送信
#define TX_COMPLETE		0x02				// <in > コマンド送信確認

#define CMD_EXT_IN1		0x01				// <in>  EXT IN 1   <RX_ENABLE>
#define CMD_EXT_IN2		0x02				// <in>  EXT IN 2   <TX_COMPLETE>
#define CMD_EXT_IN3		0x04				// <in>  EXT IN 3
#define CMD_EXT_IN4		0x08				// <in>  EXT IN 4
#define CMD_EXT_OUT1	0x01				// <out> EXT OUT 1  <RX_COMPLETE>
#define CMD_EXT_OUT2	0x02				// <out> EXT OUT 2  <TX_ENABLE>
#define CMD_EXT_OUT3	0x04				// <out> EXT OUT 3
#define CMD_EXT_OUT4	0x08				// <out> EXT OUT 4

#define CMD_BUSY		0x04				// <out> 9301BUSY
#define CMD_READY		0x08				// <out> 9301初期化完了


typedef struct {
	char	*pCommand;				// コマンド名(複数文字可)
	int		(*pFunction)(void);		// 実行関数へのポインタ
	int		nCode;					// オート動作コード
	int		nCommandLength;			// コマンド長(※ライブラリで使用)
} FUNCTION_TABLE;


// --- variable ---
extern int COMMAND_BUFF;									// 可変パラメータ化  2007.11.16
extern char *R_CS_CommandBuffRx;							// コマンド受信バッファ  可変サイズ化  2007.11.16
extern char *R_CS_CommandBuffTx;							// コマンド送信バッファ  "
//extern char R_CS_CommandBuffRx[COMMAND_BUFF];							// コマンド受信バッファ
//extern char R_CS_CommandBuffTx[COMMAND_BUFF];							// コマンド送信バッファ

// --- prototype ---
//
// ver1.07b1.6	2009.05.27 saito 	通信方法の設定関数をR_CS_CMD.c/hへ
int R_CS_CommandGetCommunicationBoardId( void );
int R_CS_CommandGetCommunicationMode( void );
int R_CS_CommandSetCommunicationMode( int nMode, int nBoardId );

// ver1.07b1.6	2009.05.27 saito 	#defineによる切り分け廃止
//#ifdef __CMD_SELECTABLE__
//int R_CS_InitialCommand(int nBoardType);
//#else
int R_CS_InitialCommand(void);
//#endif
int R_CS_CommandBufferAllocate(int nSize);
int R_CS_CommandBufferClear(void);				// 2007.11.16
int R_CS_CommndSetGpioBoard(int nRdsw,int nDataPort,int nControlPort);		//<1.07b2.7>

int R_CS_CommandCheck(void);
//int R_CS_CommandExecute(void);
int R_CS_CommandTransmit(char *command);
int R_CS_CommandTxChar(char command);
//void R_CS_CommandIOCheck(void);
int R_CS_CommandBusyOn(void);
int R_CS_CommandBusyOff(void);
int R_CS_CommandReadyOn(void);
int R_CS_CommandReadyOff(void);
int R_CS_CommandWrite(int nPort, int nData);
int R_CS_CommandRead(int nPort);
int R_CS_CommandWriteExtOut(int nData);
int R_CS_CommandReadExtIn(void);
int R_CS_CommandExtOut3On(void);
int R_CS_CommandExtOut3Off(void);
int R_CS_CommandExtOut4On(void);
int R_CS_CommandExtOut4Off(void);

int R_CS_CommandDisplayStatusMode(int mode);
void R_CS_CommandDisplayStatus(void);

void R_CS_CommandSetWaitTime(int nTime);
int R_CS_CommandGetWaitTime(void);

// ver1.07b4.8	2013.04.10	新規追加 通信タイムアウト時間設定
int	R_CS_CommandSetCommunicationTimeLimit(int usec);
int	R_CS_CommandGetCommunicationTimeLimit(void);
