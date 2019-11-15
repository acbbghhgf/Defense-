/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: Fri 15 Nov 2019 12:38:23 AM PST
 ************************************************************************/

#include "core/wwk_type.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
#define MAXLINE 80
#define SERV_PORT 8000
 
int main(int argc, char *argv[])
{
    struct sockaddr_in servaddr;
    char buf[MAXLINE];
 
    int sockfd, n;
    char *str;
    int ret = 0;
 
    if(argc != 2) {
        fputs("usage: ./client message\n", stderr);
        exit(1);
    }
    
    str = argv[1];
 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    servaddr.sin_port = htons(SERV_PORT);
    
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
   
    ret = write(sockfd, str, strlen(str));
    if(ret != strlen(str))
    {
        wwk_debug("write faile , send data len : %d \n", ret);
    }
 
    n = read(sockfd, buf, MAXLINE);
    printf("Response from server:\n");
    ret = write(STDOUT_FILENO, buf, n);
    if(ret != n)
    {
        wwk_debug("write faile , send data len : %d \n", ret);
    }
 
    close(sockfd);
    return 0;
}
