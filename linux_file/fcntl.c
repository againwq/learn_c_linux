#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    int flags = -1;
    int accmode = -1;
    flags = fcntl(0, F_GETFL, 0);
    if(flags < 0){
        printf("获取文件属性失败");
        return -1;
    }
    accmode = flags & O_ACCMODE;
    if(accmode == O_RDONLY)
        printf("stdin read only\n");
    else if(accmode == O_WRONLY)
        printf("stdin write only\n");
    else if(accmode == O_RDWR)
        printf("stdin write read\n");
    else
        printf("stdin other mode\n");

    if(flags & O_APPEND)
        printf("附加模式\n");
    if(flags & O_NONBLOCK)
        printf("非阻塞模式\n");

    return 0;
}
