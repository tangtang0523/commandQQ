#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <resolv.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>

#include "get_time.h"
#include "client.h"

int main(int argc, const char *argv[])
{
	int sockfd, len; 
	//socklen_t len;
	struct sockaddr_in dest;
	char buf[MAXBUF + 1];
	fd_set rfds;
	struct timeval tv;
	int retval, maxfd = -1;
	char *time_str = NULL;

	time_str = (char *)malloc(sizeof(char)*20);

	if (argc !=3)
	{
		printf("error! : \n\t\t%s IP PORT\n\t such as:\t%s 127.0.0.1 80\n",argv[0], argv[0]);
		exit(0);
	}
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
	   perror("socket");
	   exit(1);
	}
	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(atoi(argv[2]));
	if (inet_aton(argv[1], (struct in_addr *)&dest.sin_addr) == 0)
	{
		perror(argv[1]);
		exit(errno);
	}
	if (connect(sockfd, (struct sockaddr *)&dest, sizeof(dest)) != 0)
	{
		perror("connect");
		exit(errno);
	}
	printf("\n***********begin to chat*************\n");
	while(1)
	{
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		maxfd = 0;
		FD_SET(sockfd, &rfds);
		if (sockfd > maxfd)
		{
			maxfd = sockfd; 
		}
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
		if (retval == -1)
		{
			printf("Something wrong with the select %s,now it will exit......", strerror(errno));
			break;
		}
		else
		{
			if(retval == 0)  
			{
				continue; 
			}
			else
			{
				if (FD_ISSET(sockfd, &rfds)) 
				{
					bzero(buf, MAXBUF+1);
					len = recv(sockfd, buf, MAXBUF, 0);
					if (len > 0)
					{
						disp_time(time_str);
						printf("%s\n\tReceive success! You have sent %d bytes\n",buf, len);
					}
					else
					{
						if (len < 0)
						{
							printf("%s\n\tReceive Failed!, the errno is %d, the massege %s\n", buf, errno, strerror(errno)) ;
						}
						else
						{
							printf("The other side is exit...\n");
						}
						break;
					}
				}
				if (FD_ISSET(0, &rfds))
				{
					bzero(buf, MAXBUF+1);
					fgets(buf, MAXBUF, stdin);
					//printf("buf: %d\n",sizeof(buf));
					//printf("buf: %d\n",strlen(buf));
					if(!strncasecmp(buf, "quit", 4))
					{
						printf("You are requiring to exit");
						break;
					}
					//buf[strlen(buf)] = '\0';/*FIXME*/
					//printf("buf: %d\n",sizeof(buf));
					//printf("buf: %d\n",strlen(buf));
					len = send(sockfd, buf, strlen(buf)-1, 0);
					if (len < 0)
					{
						printf("%s\n\tSent Failed!, the errno is %d, the massege %s\n", buf, errno, strerror(errno)) ;
						break;
					}
					else
					{
						//printf("%s\n\tSent success! You have sent %d bytes\n",buf, len);
						disp_time(time_str);
						printf("\tSent success! You have sent %d bytes\n", len);
					}
				}
			}
		}
	}
	close(sockfd);
	return 0;
}

