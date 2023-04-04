#include <stdio.h>
#include <sys/socket.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define IP "127.0.0.1"

static void udpclie_echo(int s, struct sockaddr* server);
int main(int argc, char const *argv[])
{
    int ss;
    struct sockaddr_in server_addr;
    ss = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&server_addr, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &server_addr.sin_addr);

    udpclie_echo(ss, (struct sockaddr*)&server_addr);

    return 0;
}

static void udpclie_echo(int s, struct sockaddr* to){
    char buf[1024] = "UDP TEST";

    struct sockaddr_in from;
    socklen_t addrlen = sizeof(*to);
    sendto(s, buf, 1024, 0, to, addrlen);
    recvfrom(s, buf, 1024, 0, (struct sockaddr*)&from, &addrlen);

    printf("接收到: %s\n", buf);
}