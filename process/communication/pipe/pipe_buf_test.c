/**
 * 管道阻塞和管道的原子性操作
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#define K 1024
#define BUFFER_SIZE K * 128
int main(int argc, char const *argv[])
{
    int result = -1;
    int fd[2],nbytes;
    pid_t pid;
    char string[] = "写入管道的数据";
    int *write_fd = &fd[1];
    int *read_fd = &fd[0];
    char buf[BUFFER_SIZE];
    char readBuffer[10*K];

    result = pipe(fd);
    if(result == -1){
        printf("管道建立失败\n");
        return -1;
    }

    pid = fork();
    if(pid == -1){
        printf("子进程创建失败\n");
        return -1;
    }

    if(pid == 0){   //子进程
        close(*read_fd);
        int write_size = BUFFER_SIZE;
        result = 0;
        while (write_size > 0)
        {
            result = write(*write_fd, string, write_size);
            if(result > 0){
                write_size = write_size - result;
                printf("写入了%d个字节\n", result);
            }else{
                printf("管道满了,等10s\n");
                sleep(10);
            }
        }
        
        return 0;
        
    }else{
        close(*write_fd);

        while (1)
        {
            nbytes = read(*read_fd, readBuffer, sizeof(readBuffer));
            if(nbytes <= 0){
                printf("目前没有数据写入了\n");
                break;
            }
            printf("接收到%d个数据,内容为%s\n", nbytes, readBuffer);
        }
        
    }


    return 0;
}
