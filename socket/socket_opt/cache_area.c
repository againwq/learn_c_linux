// 获取和设置tcp的发送和接收缓存区大小
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    int err = -1;
    int ss;
    int send_size = 0;
    int recv_size = 0;
    socklen_t optlen = 0;

    ss = socket(AF_INET, SOCK_STREAM, 0);  //创建tcp socket
    if(ss < 0){
        perror("socket()");
        return -1;
    }

    optlen = sizeof(send_size);
    err = getsockopt(ss, SOL_SOCKET, SO_SNDBUF, &send_size, &optlen);  //获取缓冲区大小
    if(err){
        printf("获取发送缓冲区大小错误\n");
    }

    printf("发送缓冲区原始大小为: %dKB\n", send_size / 1024);

    optlen = sizeof(recv_size);
    err = getsockopt(ss, SOL_SOCKET, SO_RCVBUF, &recv_size, &optlen);
    if(err){
        printf("获取接收缓冲区大小错误\n");
    }
    printf("接收缓冲区原始大小为: %dKB\n", recv_size / 1024);
    
    send_size = 4096;  // 4K
    optlen = sizeof(send_size);
    err = setsockopt(ss, SOL_SOCKET, SO_SNDBUF, &send_size, &optlen);
    if(err){
        printf("设置发送缓冲区失败\n");
    }

    recv_size = 8192;  // 8K
    optlen = sizeof(recv_size);
    err = setsockopt(ss, SOL_SOCKET, SO_RCVBUF, &recv_size, &optlen);
    if(err){
        printf("设置接收缓冲区失败\n");
    }

    optlen = sizeof(send_size);
    err = getsockopt(ss, SOL_SOCKET, SO_SNDBUF, &send_size, &optlen);  //获取缓冲区大小
    if(err){
        printf("获取发送缓冲区大小错误\n");
    }

    printf("发送缓冲区更改后大小为: %dKB\n", send_size / 1024);  // 8KB

    optlen = sizeof(recv_size);
    err = getsockopt(ss, SOL_SOCKET, SO_RCVBUF, &recv_size, &optlen);
    if(err){
        printf("获取接收缓冲区大小错误\n");
    }
    printf("接收缓冲区更改后大小为: %dKB\n", recv_size / 1024); // 16KB

    // 可以发现缓存区的实际大小是用户手动设置大小的两倍
    // 实际上不仅仅是这样,linux内核规定缓冲区有一个最大和最小值,可以在内核源码中找到

    return 0;
}




