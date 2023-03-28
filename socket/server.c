#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PROT 3000   //监听端口
#define BACKLOG 5   //监听队列长度

void server_handle(int s_fd);
int main(int argc, char const *argv[])
{
    int ss, cs;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int err;
    pid_t pid;

    ss = socket(AF_INET, SOCK_STREAM, 0);
    if(ss < 0){
        printf("服务端创建失败\n");
        return -1;
    }
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PROT); //端口号
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //本地地址
    err = bind(ss, (struct sockaddr*) &server_addr, sizeof(server_addr)); //给ss绑定端口号和ip
    if(err < 0){
        printf("绑定至%d:%d失败\n", server_addr.sin_addr.s_addr, server_addr.sin_port);
        return -1;
    }
    err = listen(ss, BACKLOG); //设置监听队列长度
    if(err < 0){
        printf("监听失败\n");
        return -1;
    }

    while (1)
    {
        socklen_t addrlen = sizeof(struct sockaddr);
        cs = accept(ss, (struct sockaddr*)&client_addr, &addrlen);
        if(cs < 0){
            continue;   //出错了，结束本次循环
        }
        pid = fork();
        if(pid == 0){
            close(ss);   //在子进程中关闭服务端
            server_handle(cs);
        }else{
            close(cs);  //主进程中关闭客户端
        }
    }
    

    return 0;
}

/*服务端处理*/

void server_handle(int c_fd){
    ssize_t size = 0;
    char buf[1024];
    for(;;){
        size = read(c_fd, buf, 1024); //读取客户端写入的数据
        if(size == 0){
            return;
        }
        sprintf(buf, "一共读出%ld个字节\n", size);
        sleep(5);
        write(c_fd, buf, strlen(buf) + 1); // 发给客户端的数据
    }
}

