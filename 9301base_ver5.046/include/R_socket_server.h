/******************************************************************************
	R_socket_server.h
	
	ïœçXóöó
	Ver 4.01	2015.02.24	êVãKí«â¡
******************************************************************************/

#ifndef R_SOCKET_SERVER_H
#define R_SOCKET_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	void* p;
} R_SOCKET_SERVER;

R_SOCKET_SERVER*	R_socket_server_open(int port);
int	R_socket_server_fd(R_SOCKET_SERVER* server);
int	R_socket_server_read(R_SOCKET_SERVER* server, void* buf, int bytes);
int	R_socket_server_write(R_SOCKET_SERVER* server, const void* buf, int bytes);
int	R_socket_server_write_string(R_SOCKET_SERVER* server, const char* string);

#ifdef __cplusplus
}
#endif

#endif // R_SOCKET_SERVER_H
