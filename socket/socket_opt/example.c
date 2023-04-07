#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <err.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <fcntl.h>


#define PORT 8888
#define BACKLOG 8
#define MAX_CLIENT 10
#define MAX_EVENT 1024
#define TIMEOUT 3

static int alive = 1;
static void sigpipe(int signo){
    alive = 0;
}

void server_handle(int cs);

int set_noblock(int fd);

void addfd(int epollfd, int fd, int enable_et);

int main(int argc, char const *argv[])
{
    int ss, cs;
    struct sockaddr_in server_addr, client_addr;
    int err = -1;
    socklen_t len;
    int optval = -1; 
    struct timeval tv;

    signal(SIGINT, sigpipe);
    signal(SIGPIPE, sigpipe);

    ss = socket(AF_INET, SOCK_STREAM, 0);
    if(ss < 0){
        perror("socket()");
        return -1;
    }

    optval = 1;
    len = sizeof(optval);
    err = setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, len);   // 设置地址重用
    if(err != -1){
        printf("设置socket可重用失败\n");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET,
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; // 本地任意地址
    
    err = bind(ss, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
    if(err == -1){
        perror("bind()");
        return -1;
    }
    optval = 128*1024;  // 设置接收缓冲区大小为128K
    len = sizeof(optval);
    err = setsockopt(ss, SOL_SOCKET, SO_RCVBUF, &optval, &len);
    if(err == -1){
        printf("设置接收缓冲区大小失败");
    }
    err = setsockopt(ss, SOL_SOCKET, SO_SNDBUF, &optval, &len);
    if(err == -1){
        printf("设置发送缓冲区大小失败\n");
    }

    tv.tv_sec = 1;
    tv.tv_usec = 200000; // 200ms
    len = sizeof(tv);
    err = setsockopt(ss, SOL_SOCKET, SO_RCVTIMEO, &tv, &len);   //设置接收超时时间
    if(err == -1){
        printf("设置超时时间失败\n");
    }

    err = listen(ss, BACKLOG);
    if(err < 0){
        perror("listen()");
        return -1;
    }

    printf("等待链接...\n");

    struct epoll_event events[MAX_EVENT]; 
    int epoll_fd = epoll_create(MAX_CLIENT);
    int cur_fd = 0;  //目前有几个需要epoll监听
    int wait_fd, i; //可以被处理的fd
    if(epoll_fd < 0){
        perror("epoll_create()");
        return -1;
    }
    addfd(epoll_fd, ss, 0);
    cur_fd = 1;
    len = sizeof(struct sockaddr);
    while (1)
    {
        wait_fd = epoll_wait(epoll_fd, events, cur_fd, TIMEOUT);
        if(wait_fd < 0){
            perror("epoll_wait()");
            return -1;
        }
        for(i = 0; i < wait_fd; i++){
            if(events[i].data.fd == ss && cur_fd < MAX_CLIENT){
                cs = accept(ss, (struct sockaddr*)&client_addr, &len);
                if(cs < 0){
                    perror("accept()");
                    continue;
                }
                printf("cs_fd: %d, IP: %s\n", cs, inet_ntoa(client_addr.sin_addr));
                addfd(epoll_fd, cs, 0);
                cur_fd++;
                continue;
            }else{
                server_handle(cs);
            }
        }
    }
    

    return 0;
}

void server_handle(int cs){  // 服务端处理逻辑

}

void addfd(int epollfd, int fd, int enable_et){  //向epoll中添加一个fd
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN; 
    if(enable_et){
        ev.events |= EPOLLET;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    set_noblock(fd);
}

int set_noblock(int fd){   // 将fd设置为非阻塞模式
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}