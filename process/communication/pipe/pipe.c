/**
 * 管道通信, 半双工通信, 将两个文件描述符作为管道通信的写入端和读出端
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{
    int result = -1;
    int fd[2], nbytes;
    pid_t pid;
    char string[] = "管道通信";
    char readbuffer[80];
    int *write_fd = &fd[1];   //设置管道的读和写端
    int *read_fd = &fd[0];

    result = pipe(fd);
    if(result == -1){
        printf("创建管道失败\n");
        return -1;
    }

    pid = fork();
    if(pid == -1){
        printf("创建进程失败\n");
        return -1;
    }
    if(pid == 0){
        close(*read_fd); //子进程关闭可读端,写数据
        result = write(*write_fd, string, strlen(string));

        return 0;
    }else{
        close(*write_fd); //当前进程关闭可写端,读出子进程传来的数据
        nbytes = read(*read_fd, readbuffer, sizeof(readbuffer));
        printf("接收到%d个数据,内容为: %s", nbytes, readbuffer);
    }


    return 0;
}
