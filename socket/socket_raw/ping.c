#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <strings.h>
#include "CRC.h"

#define K 1024
#define BUFFSIZE 72
static unsigned char send_buff[BUFFSIZE];
static unsigned char recv_buff[2 * K];
static struct sockaddr_in dest;
static int rawsock = 0;
static pid_t pid = 0;
static int alive = 0;
static short packet_send = 0;
static short packet_recv = 0;
static char dest_str[80];
static struct timeval tv_end, tv_begin, tv_interval;
//用于保存已经发送的包的状态值
typedef struct pingm_packet{
    struct timeval tv_begin;
    struct timeval tv_end;
    short seq;
    int flag;   // 1表示已经发送但是没有接收到回应,0表示接收到回应
} pingm_packet;
static pingm_packet pingpacket[128];

struct timeval icmp_tvsub(struct timeval end, struct timeval begin){
    struct timeval tv;
    tv.tv_sec = end.tv_sec - begin.tv_sec;
    tv.tv_usec = end.tv_usec - begin.tv_usec;
    if(tv.tv_usec < 0){     //us可能小于0,需要向秒位借1s
        tv.tv_sec --;
        tv.tv_usec += 1000000;
    }
    return tv;
}

static void icmp_sigint(int signo){
    alive = 0;
    gettimeofday(&tv_end, NULL);
    tv_interval = icmp_tvsub(tv_end, tv_begin);
    return;
}

static pingm_packet *icmp_findpacket(int seq){
    int i = 0;
    pingm_packet *found = NULL;
    if(seq = -1){   // 查找空包
        for(i = 0; i < 128; i++){    
            if(pingpacket[i].flag == 0){
                found = &pingpacket[i];
                break;
            }
        }
    }else if(seq >= 0){  // 查找seq匹配的包
        for(i = 0; i < 128; i++){
            if(pingpacket[i].seq == seq){
                found = &pingpacket[i];
                break;
            }
        }
    }
    return found;
}

static void icmp_statistics(void){
    long time = (tv_interval.tv_sec * 1000) + (tv_interval.tv_usec / 1000);
    printf("--- %s ping statistics -- \n", dest_str);
    printf("%d packets transmitted, %d received, %d%c packet loss, time %ld ms\n", 
            packet_send, 
            packet_recv, 
            (packet_send-packet_recv)*100/packet_send,
            '%',
            time);
}

// 设置icmp报头
void icmp_pack(struct icmp *icmph, int seq, struct timeval *tv, int length){
    unsigned char i = 0;
    icmph->icmp_type = ICMP_ECHO;  //ICMP回显请求
    icmph->icmp_code = 0;   // 回显请求code值为0
    icmph->icmp_cksum = 0;  // 校验码暂时用0填充
    icmph->icmp_seq = seq;  //本报的序列号
    icmph->icmp_id = pid & 0xffff;
    for(i = 0; i < length; i++){
        icmph->icmp_data[i] = i;
    }
    icmph->icmp_cksum = CRC16_MODBUS((unsigned char*)icmph, length);
}

//解析icmp报头
int icmp_unpack(char *buf, int len){
    int i, iphdrlen;
    struct ip *ip;
    struct icmp *icmp = NULL;
    int rtt;

    ip = (struct ip*)buf;     // 让ip指向ip报文头部
    iphdrlen = ip->ip_hl * 4;  // 获取ip头部长度
    icmp = (struct icmp*)(buf + iphdrlen); // 获取到ICMP头部地址
    len -= iphdrlen;

    if(len < 8){
        printf("ICMP 包的长度小于8\n");
        return -1;
    }
    if((icmp->icmp_type == ICMP_ECHOREPLY && (icmp->icmp_id == pid))){  // icmp类型为回显类型且是本进程的ID
        struct timeval tv_internel, tv_recv, tv_send;
        pingm_packet *packet = icmp_findpacket(icmp->icmp_seq);
        if(packet == NULL){
            return -1;
        }
        packet->flag = 0;
        tv_send = packet->tv_begin;

        gettimeofday(&tv_recv, NULL);
        tv_internel = icmp_tvsub(tv_recv, tv_send);
        rtt = tv_internel.tv_sec * 1000 + tv_internel.tv_usec / 1000;

        printf("%d byte from %s: icmp_seq=%u ttl=%d rtt=%d ms\n", len, inet_ntoa(ip->ip_src),icmp->icmp_seq, ip->ip_ttl,rtt);

        packet_recv++;
        
    }else{
        return -1;
    }
}


void *icmp_send(void *arg){
    gettimeofday(&tv_begin, NULL);
    while (alive)
    {
        int size = 0;
        struct timeval tv;
        gettimeofday(&tv, NULL);
        pingm_packet *packet = icmp_findpacket(-1);
        if(packet){
            packet->seq = packet_send;
            packet->flag = 1;
            gettimeofday( &packet->tv_begin, NULL);
        }
        icmp_pack((struct icmp*)&send_buff, packet_send, &tv, 64);
        size = sendto(rawsock, send_buff, 64, 0, (struct sockaddr*)&dest, sizeof(dest));
        if(size < 0){
            perror("sendto()");
            continue;
        }
        packet_send++;
        sleep(1);
    }
}


void *icmp_recv(void *arg){
    struct timeval tv;
    tv.tv_usec = 200;
    tv.tv_sec = 0;
    fd_set readfd;

    while (alive)
    {
        int res = 0;
        FD_ZERO(&readfd);
        FD_SET(rawsock, &readfd);
        res = select(rawsock + 1, &readfd, NULL, NULL, &tv);
        switch (res)
        {
        case -1:
            break;
        case 0: break;
        default:{
            int fromlen = 0;
            socklen_t len = sizeof(struct sockaddr);
            int size = recvfrom(rawsock, recv_buff, sizeof(recv_buff), 0, (struct sockaddr*)&dest, &len);
            if(size < 0){
                perror("recv()");
                continue;
            }
            res = icmp_unpack(recv_buff, size);
            if(res == -1){
                continue;
            }
        }
            break;
        }
    }
    
}

int main(int argc, char const *argv[])
{
    struct hostent *host = NULL;
    struct protoent *protocol = NULL;
    char protoname[] = "icmp";
    unsigned long inaddr = 1;
    int size = 128*K;

    if(argc < 2){
        printf("参数不够");
        return -1;
    }
    protocol = getprotobyname(protoname);  //获取协议类型
    if(protocol == NULL){
        perror("getprotobyname()");
        return -1;
    }

    memcpy(dest_str, argv[1], strlen(argv[1]) + 1);  //获取目的地址

    memset(pingpacket, 0, sizeof(pingm_packet) * 128);

    rawsock = socket(AF_INET, SOCK_RAW, protocol->p_proto); // icmp协议socket

    if(rawsock < 0) {
        perror("socket()");
        return -1;
    }

    pid = getuid();
    setsockopt(rawsock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
    bzero(&dest, sizeof(dest));

    dest.sin_family = AF_INET;

    inaddr = inet_addr(argv[1]);
    if(inaddr == INADDR_NONE){  //输入的是域名
        host = gethostbyname(argv[1]);   //解析域名
        if(host == NULL){
            perror("gethostbyname()");
            return -1;
        }
        memcpy((char *)&dest.sin_addr, &host->h_addr_list[0], host->h_length);
    }else{
        memcpy((char*)&dest.sin_addr, &inaddr, sizeof(inaddr));
    }

    inaddr = dest.sin_addr.s_addr;
    printf("PING %s (%ld.%ld.%ld.%ld) 56(84) bytes of data.\n",
        dest_str,
        (inaddr&0x000000ff)>>0,
        (inaddr&0x0000ff00)>>8,
        (inaddr&0x00ff0000)>>16,
        (inaddr&0xff000000)>>24);
    signal(SIGINT, icmp_sigint);


    alive = 1;
    pthread_t send_id, recv_id;
    int err = 0;
    err = pthread_create(&send_id, NULL, icmp_send, NULL);  //发送线程
    if(err < 0){
        perror("pthread_create()");
        return -1;
    }
    err = pthread_create(&recv_id, NULL, icmp_recv, NULL); // 接收线程
    if(err < 0){
        perror("pthread_create()");
        return -1;
    }

    pthread_join(send_id, NULL);
    pthread_join(recv_id, NULL);

    close(rawsock);
    icmp_statistics();

    return 0;
}
