#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{
    int fd[2];
    int res = -1;
    int *write_fd = &fd[1];
    int *read_fd = &fd[0];
    pid_t pid;
    char buf[80];

    res = pipe(fd);
    if(res == -1){
        printf("管道创建失败");
        return -1;
    }
    pid = fork();
    if(pid == -1){
        printf("子进程创建失败\n");
        return -1;
    }
    if(pid == 0){   //子进程
        close(*write_fd);
        ssize_t size = read(*read_fd, buf, 80); //等待父进程写数据,阻塞等待
        printf("共读取了%ld字节, 内容为%s\n", size, buf);
    }else{
        sleep(5);   //父进程阻塞,子进程等待有数据被写入才能读出数据
        close(*read_fd);
        ssize_t size = write(*write_fd, "测试数据", sizeof("测试数据"));
        printf("写完数据了,写了%ld字节\n", size);
    }

    return 0;
}
