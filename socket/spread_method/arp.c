#include <stdio.h>
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

struct arppacket{
    unsigned short ar_hrd;  // 硬件类型
    unsigned short ar_pro;  // 协议类型
    unsigned char ar_hln;  // 硬件地址长度
    unsigned char ar_pln; // 协议地址长度
    unsigned short ar_op; //arp操作码
    unsigned char ar_sha[ETH_ALEN]; //发送方mac地址
    unsigned char *ar_sip;  //发送方IP地址
    unsigned char ar_tha[ETH_ALEN]; //目的mac地址
    unsigned char *ar_tip; //目的IP地址
};

int main(int argc, char const *argv[])
{
    char ef[ETH_FRAME_LEN];  //以太网帧缓冲区
    struct ethhdr *p_ethhr;  //以太网帧头部指针
    char eth_dest[ETH_ALEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,}; //目的以太网地址
    char eth_source[ETH_ALEN] = {0x00, 0x15, 0x5d, 0xd3, 0x87, 0xb3}; //源以太网地址
    int fd;
    fd = socket(AF_INET, SOCK_PACKET, htons(ETH_P_IP)); //创建数据链路层的socket

    p_ethhr = (struct ethhdr*)ef;  //使p_ethhdr指向以太网帧的帧头
    memcpy(p_ethhr->h_source, eth_source, ETH_ALEN);  //设置源mac地址
    memcpy(p_ethhr->h_dest, eth_dest, ETH_ALEN); // 设置目的mac地址
    p_ethhr->h_proto = htons(0x0806);  //设置协议类型,以太网0x0806
    struct arppacket *p_arp;
    p_arp = (struct arppacket*)ef + ETH_HLEN; // 定位arp包地址
    p_arp->ar_hrd = htons(0x1); //arp硬件类型
    p_arp->ar_pro = htons(0x0800); //协议为ip协议
    p_arp->ar_hln = 6; //硬件地址长度
    p_arp->ar_pln = 4; //IP地址长度

    memcpy(p_arp->ar_sha, eth_source, ETH_ALEN);  // 设置源mac地址
    p_arp->ar_sip = (unsigned char*)inet_addr("172.19.111.143"); //设置发送方ip
    memcpy(p_arp->ar_tha, eth_dest, ETH_ALEN); //设置目的mac地址(实际上是广播地址)

    p_arp->ar_tip = (unsigned char*)inet_addr("192.168.1.107");

    int i;
    ssize_t size;
    for(i = 0; i < 8; i++){
        size = write(fd, ef, ETH_FRAME_LEN);
        if(size < 0){
            perror("write()");
            return -1;
        }
        sleep(1);
    }
    char readbuf[512];
    size = read(fd, readbuf, sizeof(readbuf));
    if(size < 0){
        perror("read()");
        return -1;
    }
    printf("读到的数据%s\n", readbuf);
    close(fd);
    return 0;
}
