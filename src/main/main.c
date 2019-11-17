#include "core/wwk_type.h"
#include "core/wwk_str,h"
#include "core/cfg/wwk_main_cfg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
 
#define MAXLINE 80
#define SERV_PORT 8000
#define EXT_FILE "../ext/wserver.cfg"

void wwk_error_handle(void)
{
    wwk_debug("not found ext file .\n");
}

int main(int argc, char **argv)
{
    wwk_string_t *main_cfg_file;
    int main_serv_port;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cliaddr_len;
    int listenfd, connfd;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];
    int i, n;
    int ret = 0;

    if(argc < 2)
    {
        wwk_debug("Use defaule ext file.\n");
        main_cfg_file = wwk_string_dup(EXT_FILE);
    }
    else{
        //process param
        //需要解析传递的参数，例如 -v -d -c 等等方式
        main_cfg_file = wwk_string_dup(argv[1]);
    }
    
    //解析配置文件(需要添加参数类型分辨的函数处理)
    //例如：readCFG_int, readCFG_str
    // readCFG(main_cfg_file->data, "SERV_PORT", )
 
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
 
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, 20);
 
    printf("Accepting connections ...\n");
    while(1) {
        cliaddr_len = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
        n = read(connfd, buf, MAXLINE);
        printf("received from %s at PORT %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));
        for(i = 0; i < n; i++)
            buf[i] = toupper(buf[i]);
        ret = write(connfd, buf, n);
        wwk_debug("write return ret = %d\n", ret);
        close(connfd);
    }
}
