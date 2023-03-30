#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PROT 8000
#define IP "127.0.0.1"

void client_handle(int *s_fd);
void get_server_client(void *arg);

pthread_t p;
int main(int argc, char const *argv[])
{
    int s;
    struct sockaddr_in server_addr;
    

    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0){
        printf("创建socket失败\n");
        return -1;
    }
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PROT;

    inet_pton(AF_INET, IP, &server_addr.sin_addr); //解析命令行传过来的参数作为ip地址

    connect(s, (struct sockaddr*)&server_addr, sizeof(server_addr));
    client_handle(&s);
    close(s);

    return 0;
}

void client_handle(int *s_fd){
    ssize_t size;
    char buf[1024];
    int res = pthread_create(&p, NULL, (void*)get_server_client, s_fd);
    if(res == -1){
        printf("线程创建失败\n");
        exit(-1);
    }
    for(;;){
        size = read(0, buf, 1024);  //从标准输入(即用户在命令行的输入)获取到数据
        if(size > 0){
            write(*s_fd, buf, size); // 向服务端的fd写入获取到的数据
        }
    }
}

void get_server_client(void *arg){
    int *ss_fd = arg;
    while (1)
    {
        char buf[1024];
        ssize_t size = read(*ss_fd, buf, 1024);
        write(1, buf, size);
    }
}