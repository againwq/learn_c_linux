#include <stdio.h>
#include <sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char const *argv[])
{
    int fd = -1;
    ssize_t size = -1;
    char buffer[] = "这是追加的数据";
    fd = open("test.txt", O_RDWR | O_APPEND);  //
    if(fd == -1){
        printf("文件打开错误, fd:%d\n",fd);
        return -1;
    }else{
        size = write(fd, buffer, strlen(buffer));
        printf("写入了%ld字节", size);
        close(fd);
    }
    return 0;
}
