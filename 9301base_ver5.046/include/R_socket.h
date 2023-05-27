/******************************************************************************
	R_socket.h
	
	�ύX����
	Ver 4.01	2013/08/07	���������[�X
	Ver 4.01	2013/09/19	R_socket_open_udp_client�֐��ǉ�
	Ver 4.01	2016/04/25	R_socket_write_string�֐��ǉ�
******************************************************************************/

#ifndef	R_SOCKET_H_
#define	R_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

int	R_socket_open_tcp_client(const char *sIpAddress, int nPort);			// �\�P�b�g(TCP/IP�N���C�A���g)�̃I�[�v���A�ڑ�
int R_socket_listen_tcp_server(const char *sIpAddress, int nPort);									// �\�P�b�g(TCP/IP�T�[�o)�̃I�[�v���A�ڑ�
int R_socket_accept_tcp_server(int nListenFd, int timeout);
int	R_socket_open_udp_client(const char* name, int port);
int	R_socket_close(int nSocketFd);											// �\�P�b�g�̃N���[�Y�A�ؒf
int	R_socket_read(int nSocketFd, char *sBuffer, int nBufferSize);			// �f�[�^��M
int	R_socket_write(int nSocketFd, const char *sBuffer, int nBufferSize);	// �f�[�^���M

int R_socket_set_keepalive(int nSocketFd, int idle, int intvl, int cnt);	// KEEPALIVE�@�\��ON����

/*!
 * @brief R_socket_write_string
 * @param fd
 * @param string
 * @return 
 */
int	R_socket_write_string(int fd, const char* string);						// �e�L�X�g�f�[�^���M

#ifdef __cplusplus
}
#endif

#endif	// R_SOCKET_H_
