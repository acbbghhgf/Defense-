#include "core/wwk_type.h"
#include "core/wwk_str,h"
#include "core/cfg/wwk_main_cfg.h"
#include <getopt.h>
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

typedef struct wwk_main_param{
    int server_port;
    wwk_string_t *ext_file;
}wwk_main_param_t;

static struct option long_options[] = {
    {"ext file name", required_argument, NULL, 'E'},
    {"server port", required_argument, NULL,'P'},
    {"help", no_argument, NULL,'H'},
    {0, 0, 0, 0}
};

static void wwk_usage(FILE *fp, int argc, char **argv)  
{  
        fprintf(fp,  
                 "Usage: %s [options]\n\n"  
                 "Version 1.3\n"  
                 "Options:\n"  
                 "-e | --ext file name   Video device name [%s]\n"  
                 "-h | --help          Print this message\n"  
                 "-s | --server port          Use memory mapped buffers [default]\n"    
                 "",  
                 argv[0], dev_name, frame_count);  
}  

wwk_main_param_t *wwk_getopt(int argc, char **argv)
{
    wwk_main_param_t *wk = wwk_malloc(sizeof(*wk));
    int c, idx;
    for(;;)
    {
        c = getopt_long(argc, argv, "e:p:h", long_options, &idx);
        if(-1 == c)
        {
            break;
        }
        switch (c)
        {
        case 0:
            wwk_debug("opt case 0\n");
            break;
        case 'e':
            wwk_debug("opt case e\n");
            wk->ext_file = wwk_string_dup(optarg);
            break;
        case 'p':
            wwk_debug("opt case p\n");
            wk->server_port = strtol(optarg, NULL, 0);
            break;
        default:
            wwk_usage(stderr, argc, argv);
            exit(EXIT_FAILURE);
            break;
        }
    }
    return wk;
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
