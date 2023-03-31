#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8000
#define IP "127.0.0.1"
#define QUEUE_LEN 5

int client_thread(void *arg);
static int count = 0;
int cs_fds[10] = {0};
int main(int argc, char const *argv[])
{
    int ss;
    int res;
    struct sockaddr_in server_addr;
    ss = socket(AF_INET, SOCK_STREAM, 0);
    if(ss < 0){
        printf("服务端创建失败\n");
        return -1;
    }
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    inet_pton(AF_INET, IP, &server_addr.sin_addr);
    server_addr.sin_port = htons(PORT);
    server_addr.sin_family = AF_INET;
    
    res = bind(ss, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in));
    if(res < 0){
        printf("绑定至: %d:%d失败", server_addr.sin_addr.s_addr, server_addr.sin_port);
        return -1;
    }
    res = listen(ss, QUEUE_LEN);
    if(res == -1){
        printf("监听失败\n");
        return -1;
    }
    printf("准备就绪, port: %d\n", server_addr.sin_port);
    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(struct sockaddr);
        memset(&client_addr, 0, sizeof(struct sockaddr_in));
        int cs = accept(ss, (struct sockaddr*)&client_addr, &addrlen);
        if(cs < 0){
            printf("没连上\n");
            continue;
        }
        cs_fds[count] = cs;
        pthread_t pt;
        res = pthread_create(&pt, NULL, (void*)client_thread, &cs_fds[count]);
        if(res != 0){
            printf("线程创建失败\n");
            cs_fds[count] = 0;
            continue;
        }
        printf("cs: %d, port: %d\n", cs, client_addr.sin_port);
        count ++;
    }
    
    return 0;
}

int client_thread(void *arg){
    int *c_fd = arg;
    ssize_t size;
    char readbuf[512];
    char writebuf[1024];
    char errbuf[100];
    int ret, i;
    for(;;){
        memset(readbuf, 0, sizeof(writebuf));
        size = read(*c_fd, readbuf, sizeof(readbuf));
        if(size <= 0){
            write(*c_fd, "服务器断开连接", sizeof("服务器断开连接"));
            close(*c_fd);
            return -1;
        }
        memset(writebuf, 0, sizeof(writebuf));
        sprintf(writebuf, "来自%d的消息: %s", *c_fd, readbuf);
        for(i = 0; cs_fds[i] != 0; i++){
            ret = write(cs_fds[i], writebuf, sizeof(writebuf));
            if(ret < 0){
                sprintf(errbuf, "error, 源: %d, 目的: %d", *c_fd, cs_fds[i]);
                write(1, errbuf, sizeof(errbuf));
            }
        }
    }
}