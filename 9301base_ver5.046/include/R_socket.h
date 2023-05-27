/******************************************************************************
	R_socket.h
	
	変更履歴
	Ver 4.01	2013/08/07	正式リリース
	Ver 4.01	2013/09/19	R_socket_open_udp_client関数追加
	Ver 4.01	2016/04/25	R_socket_write_string関数追加
******************************************************************************/

#ifndef	R_SOCKET_H_
#define	R_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

int	R_socket_open_tcp_client(const char *sIpAddress, int nPort);			// ソケット(TCP/IPクライアント)のオープン、接続
int R_socket_listen_tcp_server(const char *sIpAddress, int nPort);									// ソケット(TCP/IPサーバ)のオープン、接続
int R_socket_accept_tcp_server(int nListenFd, int timeout);
int	R_socket_open_udp_client(const char* name, int port);
int	R_socket_close(int nSocketFd);											// ソケットのクローズ、切断
int	R_socket_read(int nSocketFd, char *sBuffer, int nBufferSize);			// データ受信
int	R_socket_write(int nSocketFd, const char *sBuffer, int nBufferSize);	// データ送信

int R_socket_set_keepalive(int nSocketFd, int idle, int intvl, int cnt);	// KEEPALIVE機能をONする

/*!
 * @brief R_socket_write_string
 * @param fd
 * @param string
 * @return 
 */
int	R_socket_write_string(int fd, const char* string);						// テキストデータ送信

#ifdef __cplusplus
}
#endif

#endif	// R_SOCKET_H_
