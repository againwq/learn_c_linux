// epoll和select的最大区别在于他的时间复杂度是O(1),且用户不用遍历所有的文件描述符。

#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>


#define PORT 8888
#define IP "127.0.0.1"
#define MAX_LISTEN_QUEUE 10
#define MAX_EVENTS 1024
#define MAX_CLIENT 10
#define TIMEOUT 3

int set_no_blocking(int fd){ //将文件描述符设置为非阻塞
    int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
	return old_option;
}

void addfd(int epollfd, int fd, int enable_et){ // 向epoll中添加fd
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;  // 默认为LT模式
    if(enable_et){  
        event.events |= EPOLLET;   //ET模式
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    set_no_blocking(fd);
}

void server_handle(int cs);

int main(int argc, char const *argv[])
{
    int ss, cs;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(struct sockaddr);
    int res;
    int i, wait_fd, cur_fd;

    ss = socket(AF_INET, SOCK_STREAM, 0);
    if( ss < 0 ){
        printf("socket建立失败\n");
        return -1;
    }

    bzero(&server_addr, 0);
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &server_addr.sin_addr);
    server_addr.sin_family = AF_INET;

    if(bind(ss, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0){
        printf("绑定失败\n");
        return -1;
    }
    if(listen(ss, MAX_LISTEN_QUEUE) < 0){
        printf("监听失败\n");
        return -1;
    }

    struct epoll_event events[MAX_EVENTS];  // 这个用来存放需要监听的事件
    int epoll_fd = epoll_create(MAX_CLIENT);  // 创建epoll实例对象
    if(epoll_fd < 0){
        printf("创建epoll示例失败\n");
        return -1;
    }
    addfd(epoll_fd, ss, 0);
    cur_fd = 1;   // 用于标识有多少个连接需要监听
    while (1)
    {
        wait_fd = epoll_wait(epoll_fd, events, cur_fd, TIMEOUT);
        if(wait_fd < 0){
            printf("epoll_wait出错");
            return -1;
        }
        for(i = 0; i < wait_fd; i++){
            if(events[i].data.fd == ss && cur_fd < MAX_EVENTS){   //接收到连接请求
                cs = accept(ss, (struct sockaddr*)&client_addr, &addrlen);
                if(cs < 0){
                    printf("接收连接出错\n");
                    return -1;
                }
                printf("c_fd: %d PORT: %d, 连接成功\n", cs, client_addr.sin_port);
                addfd(epoll_fd, cs, 0);
                cur_fd ++;
                continue;
            }

            server_handle(events[i].data.fd);
        }
    }
    
    return 0;
}

void server_handle(int cs){
    char readbuf[800];
    char writebuf[1024];
    ssize_t size;
    size = recv(cs, readbuf, sizeof(readbuf), 0);
    if(size <= 0){
        printf("c_fd: %d 断开连接\n", cs);
        close(cs);
    }else{
        if(memcmp("bye", readbuf, sizeof("bye") - 1) == 0){
            printf("c_fd: %d 断开连接\n", cs);
            close(cs);
        }
        sprintf(writebuf, "来自%d的数据: %s", cs, readbuf);
        size = send(cs, writebuf, sizeof(writebuf), 0);
    }
}