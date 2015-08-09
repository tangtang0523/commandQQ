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
#include <sys/wait.h>
#include <time.h>

#include "server.h"
#include "get_time.h"

int main(int argc, const char *argv[])
{
    int listenfd, connfd; 
    socklen_t len;
    struct sockaddr_in my_addr, their_addr;
    unsigned int myport, lisnum;
    char buf[MAXBUF + 1];
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd = -1;
    char *time_str = NULL;

    time_str = (char *)malloc(sizeof(char)*20);
    if (argv[1])
        myport = atoi(argv[1]);
    else
        myport = DEFAILT_PORT; 

    if (argv[2])
        lisnum = atoi(argv[2]);
    else
        lisnum = 2;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
       perror("socket");
       exit(1);
    }
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(myport);

    if (argv[3])
        my_addr.sin_addr.s_addr = inet_addr(argv[3]);
    else
        my_addr.sin_addr.s_addr = INADDR_ANY;

    int opt = 1; 
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(listenfd, (struct sockaddr*) &my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }
    
    if (listen(listenfd, lisnum) == -1)
    {
        perror("listen");
        exit(1);
    }

    //printf("\n**********************\n");
    while(1)
    {
        printf("\n*****wait for the new chat*****\n");
        len = sizeof(struct sockaddr);
        if ((connfd = accept(listenfd, (struct sockaddr*)&their_addr, &len)) == -1)
        {
            perror("accept");
            exit(errno);
        }
        else
        {
            printf("server:got connection from %s, port %d, socket %d\n", inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), connfd);
        }
        printf("\n***********deal with the chat***************\n");
        while(1)
        {
            FD_ZERO(&rfds);
            FD_SET(0, &rfds);
            maxfd = 0;
            FD_SET(connfd, &rfds);
            if (connfd > maxfd)
            {
                maxfd = connfd; 
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
                    if (FD_ISSET(0, &rfds)) 
                    {
                        bzero(buf, MAXBUF+1);
                        fgets(buf, MAXBUF, stdin);
                        if(!strncasecmp(buf, "quit", 4))
                        {
                            printf("You are requiring to exit");
                            break;
                        }
                        //buf[strlen(buf)] = '\0';/*XXX*/
                        len = send(connfd, buf, strlen(buf)-1, 0);
                        if (len > 0)
                        {
                            //printf("%s\tSent success! You have sent %d bytes\n",buf, len);
                            printf("\tSent success! You have sent %d bytes\n",len);
                            disp_time(time_str);
                        }
                        else
                        {
                            printf("%s\tSent Failed!, the errno is %d, the massege %s\n", buf, errno, strerror(errno)) ;
                            break;
                        }
                    }
                    if (FD_ISSET(connfd, &rfds))
                    {
                        bzero(buf, MAXBUF+1);
                        len = recv(connfd, buf, MAXBUF, 0);
                        if (len > 0)
                        {
                            disp_time(time_str);
                            printf("%s\n\tReceive success! You have sent %d bytes\n",buf, len);
                        }
                        else
                        {
                            if (len < 0)
                            {
                                printf("%s\tReceive Failed!, the errno is %d, the massege %s\n", buf, errno, strerror(errno)) ;
                            }
                            else
                            {
                                printf("The other side exit...\n");
                            }
                            break;
                        }
                    }
                }
            }
        }
        close(connfd);
        printf("Do you want want to chat with the other person?(no -> exit)");
        fflush(stdout);
        bzero(buf, MAXBUF+1);
        fgets(buf, MAXBUF, stdin);
        if (!strncasecmp(buf, "no", 2))
        {
            printf("exit1\n");
            break;
        }
    }
    close(listenfd);
    return 0;
}
