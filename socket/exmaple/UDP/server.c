/**
 * UDP不需要预先建立连接,相比tcp编程,少了connect,listen,和accept函数
 * 使用UDP在应用中需要解决报文丢失、乱序和缓冲区溢出等问题
 * UDP的客户端也可以使用connect函数,但是使用后不能用sendto函数发送数据,要用write函数发送数据
 * UDP常用的函数有sendto,recvfrom等
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>

#define PORT 8888
#define IP "127.0.0.1"

void static udpserver_echo(int s, struct sockaddr *client);

int main(int argc, char const *argv[])
{
    int ss;
    struct sockaddr_in server_addr, client_addr;

    ss = socket(AF_INET, SOCK_DGRAM, 0);
    if(ss < 0){
        printf("socket创建失败\n");
        return -1;
    }
    bzero(&server_addr, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &server_addr.sin_addr);

    if(bind(ss, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
        printf("bind操作失败\n");
        return -1;
    }

    udpserver_echo(ss, (struct sockaddr*)&client_addr);
    
    return 0;
}

void static udpserver_echo(int s, struct sockaddr *client){
    ssize_t size;
    char buf[1024];
    socklen_t addrlen;
    while (1)
    {
        addrlen = sizeof(*client);
        size = recvfrom(s, buf, 1024, 0, client, &addrlen);
        sendto(s, buf, size, 0, client, addrlen);
    }
    
}