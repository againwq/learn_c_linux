#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <signal.h>

#define PORT 8888
#define IP "127.0.0.1"
typedef void (*sighandler_t)(int);

void sig_handle(int signo);
void client_handle(int *s_fd);
void get_server_client(void *arg);

pthread_t p;
int main(int argc, char const *argv[])
{
    int s;
    struct sockaddr_in server_addr;

    signal(SIGSTOP, sig_handle);
    signal(SIGKILL, sig_handle);
    signal(SIGINT, sig_handle);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0){
        printf("创建socket失败\n");
        return -1;
    }
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    inet_pton(AF_INET, IP, &server_addr.sin_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, IP, &server_addr.sin_addr); //解析命令行传过来的参数作为ip地址

    connect(s, (struct sockaddr*)&server_addr, sizeof(server_addr));
    client_handle(&s);
    close(s);

    return 0;
}

void client_handle(int *s_fd){
    ssize_t size;
    char buf[512];
    int res = pthread_create(&p, NULL, (void*)get_server_client, s_fd);
    if(res == -1){
        printf("线程创建失败\n");
        exit(-1);
    }
    for(;;){
        memset(buf, 0, sizeof(buf));
        size = read(0, buf, sizeof(buf));  //从标准输入(即用户在命令行的输入)获取到数据
        if(size > 0){
            send(*s_fd, buf, size, 0); // 向服务端的fd写入获取到的数据
        }
    }
}

void get_server_client(void *arg){
    int *ss_fd = arg;
    while (1)
    {
        char buf[1024];
        ssize_t size = read(*ss_fd, buf, 1024);
        if(size > 0){
            printf("%s", buf);
        }else{
            printf("感谢使用\n");
            exit(0);
        }
    }
}
void sig_handle(int signo){
    if(SIGSTOP == signo){
        printf("捕获到了sigstop\n");
        exit(0);
    }else if(SIGKILL == signo){
        printf("捕获到了SIGKILL\n");
        exit(0);
    }else if(signo == SIGINT){
        printf("捕获到了中断信号\n");
        exit(0);
    }
    else{
        printf("其他信号\n");
        exit(0);
    }
}