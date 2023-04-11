#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include "thread_pool.h"

#define PORT 8080
#define IP "127.0.0.1"
#define BACKLOG 10
#define MAX_EVENT 1024
#define MAX_CLIENT 10

void *server_handle(void *p_fd);
int set_noblock(int fd);
void add_fd_epoll(int epollfd, int fd, int enable_et);

int main(int argc, char const *argv[])
{
    int ss;
    struct sockaddr_in s_addr, c_addr;
    int err = 0;
    socklen_t len;
    threadpool_t *pool = threadpool_create(10, 20, 15);

    ss = socket(AF_INET, SOCK_STREAM, 0);
    if(ss < 0){
        perror("socket()");
        return -1;
    }

    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &s_addr.sin_addr);

    len = sizeof(struct sockaddr);

    if(bind(ss, (struct sockaddr*)&s_addr, len) < 0 ){
        perror("bind()");
        return -1;
    }

    if(listen(ss, BACKLOG) < 0){
        perror("listen()");
        return -1;
    }
    struct epoll_event events[MAX_EVENT];
    int cur_fd = 0, i, wait_fds;
    int epollfd = epoll_create(MAX_CLIENT);
    if(epollfd < 0){
        perror("epoll_create()");
        return -1;
    }
    add_fd_epoll(epollfd, ss, 0);
    cur_fd++;
    while (1)
    {
        wait_fds = epoll_wait(epollfd, events, cur_fd, 3);
        if(wait_fds < 0){
            perror("epoll_wait()");
            return -1;
        }
        for(i = 0; i < wait_fds; i++){
            int cs;
            if(events[i].data.fd == ss && cur_fd < MAX_CLIENT){
                cs = accept(ss, (struct sockaddr*)&c_addr, &len);
                if(cs  < 0){
                    perror("accept()");
                    continue;
                }
                add_fd_epoll(epollfd, cs, 0);
                cur_fd++;
                printf("新连接: cs: %d, ip: %s\n", cs, inet_ntoa(c_addr.sin_addr));
                continue;
            }
            threadpool_add_task(pool, (void*)server_handle, (void*)&cs);
        }
    }

    threadpool_free(pool);
    threadpool_destroy(pool);

    return 0;
}

int set_noblock(int fd){
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void add_fd_epoll(int epollfd, int fd, int enable_et){
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if(enable_et){
        ev.events |= EPOLLET;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    set_noblock(fd);
}

void *server_handle(void *p_fd){
    int *cs = p_fd;
    ssize_t size;
    char readbuf[1024];
    char writebuf[1024];
    size = recv(*cs, readbuf, sizeof(readbuf), 0);
    if(size < 0){
        printf("断开连接");
    }
}
