#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <strings.h>
#include <sys/select.h>
#include <string.h>
#include <errno.h>

#define PORT 8888
#define MAX_QUEUE_LEN 10
#define MAX_CLIENT 10
#define MAX_BUF 512

int max_client(int fds[], int len, int ss);
int server_handle(int c_fd);

int fds[MAX_CLIENT] = {0};
char tmpBuf[MAX_BUF];
int connections = 0; // 存在的集合数量
int main(int argc, char const *argv[])
{
    int ss, cs;  // 服务端,客户端的文件描述符
    struct sockaddr_in server_addr, client_addr;  // 地址
    int res;

    fd_set rd, wd; // 读写文件描述符集合
    struct timeval tv;  // 设置超时的结构体

    ss = socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&server_addr, 0);
    server_addr.sin_port = htons(PORT);
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    res = bind(ss, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
    if(res < 0){
        printf("绑定失败\n");
        return -1;
    }
    
    res = listen(ss, MAX_QUEUE_LEN);
    if(res < 0){
        printf("监听失败\n");
        return -1;
    }
    int i = 0;
    socklen_t addrlen = sizeof(struct sockaddr);
    while (1)
    {
        FD_ZERO(&rd);   //清空描述符集合
        FD_ZERO(&wd);
        FD_SET(ss, &rd);

        connections = 0;  

        for (i = 0; i < MAX_CLIENT; i++){   //添加存在的客户端连接
            if(fds[i] != 0){
                FD_SET(fds[i], &rd);
                FD_SET(fds[i], &wd);
                connections++;   // 记录有多少个已经存在的连接
            }
        }

        tv.tv_sec = 5;
        tv.tv_usec = 0;

        res = select(max_client(fds, MAX_CLIENT, ss) + 1, &rd, &wd, NULL, &tv); //select
        if(res < 0){
            perror("select");
            continue;
        }else if(res == 0){
            printf("超时了\n");
            continue;
        }else{
            for(i = 0; i < MAX_CLIENT; i++){         // 处理准备就绪的连接
                if(FD_ISSET(fds[i], &rd) && FD_ISSET(fds[i], &wd)){
                    res = server_handle(fds[i]);
                    if(res < 0){
                        fds[i] = 0;
                        connections --;
                        printf("当前总连接数: %d", connections);
                    }
                }
            }
        }
        if(FD_ISSET(ss, &rd)){  // 服务端可以读取了
            cs = accept(ss, (struct sockaddr*)&client_addr, &addrlen);
            if( cs < 0 ){
                printf("新连接建立失败\n");
                continue;
            }
            if(connections + 1 >= MAX_CLIENT){  //超出最大连接
                printf("超出最大连接！\n");
                send(cs, "忙线中...", sizeof("忙线中..."), 0);
                close(cs);
                continue;
            }else{
                fds[connections++] = cs;
                printf("新加连接: %d, PORT: %d, ip: %d\n", cs, client_addr.sin_port, client_addr.sin_addr.s_addr);
                printf("当前总连接数: %d\n", connections);
                for(int i = 0; i < connections; i++){
                    if(FD_ISSET(fds[i], &wd)){
                        memset(tmpBuf, 0, MAX_BUF);
                        sprintf(tmpBuf, "c_fd: %d 加入\n", cs);
                        write(fds[i], tmpBuf, MAX_BUF);
                    }
                }
            }
        }
        
    }
    return 0;
}

int max_client(int fds[], int len, int ss){
    int tmp = fds[0];
    for(int i = 1; i < len; i++){
        if(tmp < fds[i]){
            tmp = fds[i];
        }
    }
    return (tmp > ss) ? tmp : ss;
}

int server_handle(int c_fd){
    ssize_t size;
    char writebuf[MAX_BUF * 2];

    memset(tmpBuf, 0, MAX_BUF);
    size = recv(c_fd, tmpBuf, MAX_BUF, 0);
    if(size <= 0){
        printf("c_fd: %d 断开连接\n", c_fd);
        close(c_fd);
        return -1;
    }else{
        sprintf(writebuf, "来自%d的数据: %s", c_fd, tmpBuf);
        for(int i = 0; i < MAX_CLIENT; i++){
            if(fds[i] != 0){
                size = send(fds[i], writebuf, sizeof(writebuf), 0);
                if(size < 0){
                    printf("写入失败 源: %d, 目的: %d\n", c_fd, fds[i]);
                    continue;
                }
            }
        }
    }

    return 0;
}
