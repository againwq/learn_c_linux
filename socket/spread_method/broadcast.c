/**
 * 广播是向局域网所有主机都发送数据,当然这要求每个主机的协议栈都支持广播
 * 广播常常用于获取服务器的ip
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>

#define PORT 8888

#define IP_FOUND "IP_FOUND"
#define IP_FOUND_ACK "IP_FOUND_ACK"
#define BUF_SIZE 32
#define IFNAME "eth0"

int main(int argc, char *argv[])
{
    int res = -1;
    int ss;
    struct sockaddr_in server_addr, broadcast_addr;
    int client_len = sizeof(struct sockaddr);
    struct ifreq ifr;
    int so_broadcast_val = 1;
    char buff[BUF_SIZE];
    ssize_t size;
    

    ss = socket(AF_INET, SOCK_DGRAM, 0);
    if(ss < 0){
        printf("socket创建失败\n");
        return -1;
    }
    memset(ifr.ifr_name, 0, sizeof(ifr.ifr_name));
    strncpy(ifr.ifr_name, IFNAME, strlen(IFNAME)); //指定网卡
    if(ioctl(ss, SIOCGIFBRDADDR, &ifr) < 0 ){ //设置出口网卡并获取到广播地址
        perror("ioctl()");
        return -1;
    }
    memcpy(&broadcast_addr, &ifr.ifr_broadaddr, sizeof(struct sockaddr_in)); // 设置广播地址
    broadcast_addr.sin_port = htons(PORT);  //  设置广播端口
    printf("broadcast: %d\n", broadcast_addr.sin_addr.s_addr);
    res = setsockopt(ss, SOL_SOCKET, SO_BROADCAST, &so_broadcast_val, sizeof(so_broadcast_val));

    int times = 10;
    int i = 0;
    for(i = 0; i < times; i++ ){
        res = sendto(ss, IP_FOUND, strlen(IP_FOUND), 0, (struct sockaddr*)&broadcast_addr, sizeof(broadcast_addr));  // 广播要获取IP的消息
        if(res < 0){
            continue;
        }
        size = recvfrom(ss, buff, BUF_SIZE, 0, (struct sockaddr *)&server_addr, &client_len);
        printf("接收到的数据为%s\n", buff);
        if(strncmp(IP_FOUND_ACK, buff, sizeof(IP_FOUND_ACK)) == 0){
            printf("找到服务器了 ip是%s\n", inet_ntoa(server_addr.sin_addr));
            break;
        }
    }
    return 0;
}