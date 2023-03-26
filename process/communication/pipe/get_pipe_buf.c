#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    int pipefd[2]; // 管道文件描述符数组
    int pipe_buf; // 管道缓冲区大小

    if (pipe(pipefd) == -1) // 创建管道失败
    {
        perror("pipe");
        return -1;
    }

    int flags = fcntl(pipefd[0], F_GETFL); // 获取管道标志位

    if (fcntl(pipefd[0], 1031, 65536) == -1) // 设置管道大小为65536字节
    {
        perror("fcntl");
        return -1;
    }

    pipe_buf = fcntl(pipefd[0], 1032); // 获取管道实际大小

    printf("PIPE_BUF = %dB\n", pipe_buf); // 打印出PIPE_BUF的大小

    return 0;
}