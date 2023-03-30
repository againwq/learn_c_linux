#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define PROT 8000   //监听端口
#define BACKLOG 5   //监听队列长度
#define MAX_CLIENT 10
#define IP "127.0.0.1"

int push_fd(int client_fds[], int fd, int length);
void server_handle(int s_fd, int shmid);

int main(int argc, char const *argv[])
{
    int ss, cs;
    struct sockaddr_in server_addr;
    int shmid, key;
    int err;
    pid_t pid;

    key = ftok("/ipc/sem", 'a');
    shmid = shmget(key, 1024, IPC_CREAT | 0604); //设置共享内存存储所有客户端的fd

    int *shm_arr = (int*)shmat(shmid, 0, 0);

    ss = socket(AF_INET, SOCK_STREAM, 0);
    if(ss < 0){
        printf("服务端创建失败\n");
        return -1;
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PROT; //端口号
    inet_pton(AF_INET, IP, &server_addr.sin_addr); //IP

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
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(struct sockaddr);
        cs = accept(ss, (struct sockaddr*)&client_addr, &addrlen);
        if(cs < 0){
            continue;   //出错了，结束本次循环
        }
        push_fd(shm_arr, cs, MAX_CLIENT);
        pid = fork();
        if(pid == 0){
            close(ss);   //在子进程中关闭服务端
            server_handle(cs, shmid);
        }else{
            //close(cs);  //主进程中关闭客户端
            printf("cs: %d, port: %d, ip: %d\n", cs, client_addr.sin_port, client_addr.sin_addr.s_addr);
        }
    }
    

    return 0;
}

/*服务端处理*/

void server_handle(int c_fd, int shmid){
    ssize_t size = 0;
    char readbuf[512];
    char writebuf[1024];
    int *shm_fds;
    int ret;
    for(;;){
        size = read(c_fd, readbuf, 512); //读取客户端写入的数据
        if(size <= 0){
            write(c_fd, "服务器断开连接", sizeof("服务器断开连接"));
            close(c_fd);
            exit(-1);
        }
        sprintf(writebuf, "来自%d的消息: %s", c_fd, readbuf);
        shm_fds = (int*)shmat(shmid, 0, 0);  //挂接共享内存,获取所有的客户端fd
        for(int i = 0; shm_fds[i] != 0; i++){ //广播
            ret = write(shm_fds[i], writebuf, strlen(writebuf) + 1); // 发给客户端的数据
            printf("当前client: %d, 被广播的fd: %d\n", c_fd, shm_fds[i]);
            if(ret == -1){
                printf("error! fd: %d, client_fd: %d\n", c_fd, shm_fds[i]);
            }
        }
    }
}

int push_fd(int client_fds[], int fd, int length){
    static int count = 0;
    if( count == length - 1){
        printf("数组已满\n");
        return -1;
    }
    *(client_fds + count) = fd;
    count ++;
    *(client_fds + count) = 0;
    return 0;
}

