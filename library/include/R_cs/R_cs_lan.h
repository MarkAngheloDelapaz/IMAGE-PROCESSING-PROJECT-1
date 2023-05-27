#ifndef R_CS_LAN_H
#define R_CS_LAN_H

#ifdef __cplusplus
extern "C"
{
#endif
// --- define ---
#define	COMMUNICATION_COMMAND_NO_READY				0x00			// コマンドなし(未受信)
#define	COMMUNICATION_COMMAND_LOT_START				0x01			// ロットスタート　コマンド
#define	COMMUNICATION_COMMAND_LOT_END				0x02			// ロットエンド　コマンド
#define	COMMUNICATION_COMMAND_DATA_REFLESH			0x03			// データ更新要求
#define	COMMUNICATION_COMMAND_DATA_RECEIVED			0x04			// データ受信結果
#define	COMMUNICATION_COMMAND_RESULT_REQUEST		0x05			// Result Request
#define	COMMUNICATION_COMMAND_USER					0x06			// ユーザカスタムコマンド

#define COMMUNICATION_STATUS_ACK		0x00		// ステータス　ACK		2013/05/30 by sassa
#define COMMUNICATION_STATUS_NACK		0x01		// ステータス　NACK		2013/05/30 by sassa

#define R_CS_LAN_MAX_SUBSTRING		8
#define R_CS_LAN_MAX_TYPELEN		128
#define R_CS_LAN_MAX_IOBYTE			8				// IOポート数(バイト数)
#define	R_CS_LEN_MAX_MARK			32				// 標印文字列長(バイト数)

#define R_CS_LAN_COUNTERCLEAR_TYPE1		1			///<	ロットスタート前にカウンタクリア
#define R_CS_LAN_COUNTERCLEAR_TYPE2		2			///<	ロットエンド後にカウンタクリア


typedef struct
{
	int						nLanEnable;									// LAN通信をオープンするか(0:OFF,1:ON)
	int						nIsServer;									// サーバでオープンするかクライアントでオープンするか？
	int						nServerPort;								// サーバーのポート番号
	int						nInterval;									// ポーリングインターバル
	int						nReconnectTimer;							// 再接続時のタイマ
	char					sServerIpAddress[32];						// サーバーのIPアドレス
	char					sClientLocation[32];						// クライアントのロケーション名
	int						nAutoSend;									// 接続時自動で初期状態を送る

	int						nTypeStart[R_CS_LAN_MAX_SUBSTRING];			// 生QRコードからの切り出しスタート
	int						nTypeLen[R_CS_LAN_MAX_SUBSTRING];			// 生QRコードからの切り出し長さ

	int						nMarkStart;									// 生QRコードからの切り出しスタート
	int						nMarkLen;									// 生QRコードからの切り出し長さ

	int						nTrimSpace;

	int						nAutoCounterClear;							// QR受信時オートカウンタクリア

	int						nKeepAliveEnable;							// ソケット通信のKeepAlive機能を使用するかどうか
} R_CS_LANINFO;

typedef struct{
	void *comstruct;
} R_CS_LAN_HANDLE;

// --- variable ---

// --- prototype ---
R_CS_LAN_HANDLE R_CS_Lan_Open(char *sIpAddress, int nPort, char *sLocation);		// ロットデータ通信モジュールのオープン(領域確保)
R_CS_LAN_HANDLE R_CS_Lan_OpenServer(char *sIpAddress, int nPort, char *sLocation);	// ロットデータ通信モジュールをサーバーでオープン(領域確保)
int R_CS_Lan_Close(R_CS_LAN_HANDLE handle);											// ロットデータ通信モジュールのクローズ(領域解放)
int R_CS_Lan_Connect(R_CS_LAN_HANDLE handle, int nTimer);							// ロットデータ通信の接続
int R_CS_Lan_Connect_NoMsg(R_CS_LAN_HANDLE handle, int nTimer);						// ロットデータ通信の接続(メッセージ表示なし)
int R_CS_Lan_Disconnect(R_CS_LAN_HANDLE handle);									// ロットデータ通信の切断
int R_CS_Lan_ReceiveLotInfo(R_CS_LAN_HANDLE handle, const R_CS_LANINFO *info);		// ロットデータ受信
int R_CS_Lan_SendLotInfo(R_CS_LAN_HANDLE handle, int nStatus);						// ロットデータ送信
int R_CS_Lan_SendDataRefleshRequest(R_CS_LAN_HANDLE handle);
int R_CS_Lan_SendDataRefleshReceived(R_CS_LAN_HANDLE handle);
int R_CS_Lan_Receive(R_CS_LAN_HANDLE handle);										// データ受信
int R_CS_Lan_SetKeepAlive(R_CS_LAN_HANDLE handle, int idle, int intvl, int cnt);
char* R_CS_Lan_GetQr(R_CS_LAN_HANDLE handle);										// QRコード　文字列の取得
char* R_CS_Lan_GetLotNo(R_CS_LAN_HANDLE handle);									// LotNo     文字列の取得
char* R_CS_Lan_GetLocation(R_CS_LAN_HANDLE handle);									// Location  文字列の取得
char* R_CS_Lan_GetType(R_CS_LAN_HANDLE handle);										// Type      文字列の取得
char* R_CS_Lan_GetCommand(R_CS_LAN_HANDLE handle);									// Command   文字列の取得
char* R_CS_Lan_GetMarkChar(R_CS_LAN_HANDLE handle);

void R_CS_LAN_SetDataLen(R_CS_LAN_HANDLE handle, int len);
//void R_CS_LAN_SetDataLen(int len);

int R_CS_LAN_GetIO(R_CS_LAN_HANDLE handle, int channel);
int R_CS_LAN_GetOutputStatus(R_CS_LAN_HANDLE handle);
//int R_CS_LAN_GetIO(int channel);

void R_CS_LAN_IO_Off(R_CS_LAN_HANDLE handle, int channel, int bit);
void R_CS_LAN_IO_On(R_CS_LAN_HANDLE handle, int channel, int bit);
void R_CS_LAN_IO_SetAutoReflesh(int flag);

/**
 * @brief R_CS_LAN_SetUserCommand
 * @param command
 */
void R_CS_LAN_SetUserCommand(R_CS_LAN_HANDLE handle, const char* command, int (*pFunc)(char *msg));

#ifdef __cplusplus
}
#endif
#endif // R_CS_LAN_H
