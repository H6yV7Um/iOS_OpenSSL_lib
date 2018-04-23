#include "socket_api.h"
#include <cstring>
#include <ctime>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h> 
#include <string>

using std::string;

int my_connect(const char* ip, const unsigned short port)
{		
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{		
		return -1;
	}

	struct sockaddr_in servaddr;
	memset(&servaddr, 0 ,sizeof(sockaddr_in) );
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip);
	servaddr.sin_port = htons(port);
	
	struct timeval timeout = {5, 0}; 
	int ret = setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (void *)&timeout, sizeof(struct timeval) );
	if (ret < 0)
	{		
		return -2;
	}
	ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (void *)&timeout, sizeof(struct timeval) );
	if (ret < 0)
	{		
		return -3;
	}

	ret = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr) );
	if (ret < 0)
	{		
		return -4;
	}

	return sockfd;	
}


int my_send(const int sockfd, const char *data, unsigned length, unsigned timeout)
{
	/*struct sockaddr_in sock_name;
	socklen_t sock_length;
	int ret = getpeername(sockfd, (sockaddr *)&sock_name, &sock_length);
	if (ret || sock_length != sizeof(sockaddr_in))
	{
		return -1;
	}
	unsigned port = ntohs(sock_name.sin_port);
	char ip[20] = {0};
	strncpy(ip, inet_ntoa(sock_name.sin_addr), sizeof(ip) );*/

	const char* ptr = data;
	size_t nleft = length;
	int send_length;
	unsigned begin_time = time(NULL);
	while (nleft > 0)
	{
		if (time(NULL) - begin_time > timeout)
		{
			return -100;			
		}

		send_length = send(sockfd, ptr, nleft, 0);	   	
		if (send_length < 0 && (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) )
		{
			send_length = 0;
		}
		else if (send_length < 0)
		{
			return -errno;
		}
		else
		{
			nleft -= send_length;
		    ptr += send_length;	
		}			
	}

	return 0;
}


int my_recv(const int sockfd, int (*check_recv_buf) (void *buf, unsigned len), 
			 char *recv_buf, int buf_length, unsigned timeout)
{
	char *ptr = recv_buf;
	int recvlength, pkglength;
	int total = 0;
	unsigned begin_time = time(NULL);
	while (1)
	{
		if (time(NULL) - begin_time > timeout)
		{
			return -100;			
		}
		if (buf_length <= total - 1)
		{
			return -10;
		}

		recvlength = recv(sockfd, ptr, buf_length - total, 0);
		if (recvlength < 0)
		{
			if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
				continue;
			else
			{
				return -errno;				
			}
		}
		else if (recvlength == 0)
		{
			return -20;
		}
		else
		{
			ptr += recvlength;
			total += recvlength;

			pkglength = check_recv_buf(recv_buf, total);
			if (pkglength < 0)  //error, discard
			{
				return -1;
			}
			else if (pkglength > 0)  //package is complete, pkglength is length
			{
				return total;
			}
		  //else pkglength == 0, receive continue
		}		
	}
}




