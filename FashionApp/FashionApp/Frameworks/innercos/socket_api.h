#ifndef SOCKET_API_H
#define SOCKET_API_H

extern int my_connect(const char* ip, const unsigned short port);

extern int my_send(const int sockfd, const char *data, unsigned length, unsigned timeout = 180);

extern int my_recv(const int sockfd, int (*check_recv_buf) (void *buf, unsigned len), 
			 char *recv_buf, int buf_length, unsigned timeout = 10);






#endif
