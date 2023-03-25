#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#define COUNT 10

int main(int argc, char const *argv[])
{
    int fd = -1;
    char buffer[COUNT];
    ssize_t size = -1;
    char filename[] = "test.txt";
    
    fd = open(filename, O_RDWR | O_CREAT | O_EXCL, S_IRWXU);
    
    if( fd == -1){
        printf("%s文件打开错误\n", filename);
        fd = open(filename, O_RDWR);
    }else{
        printf("%s文件打开成功%d\n", filename, fd);
    }

    while (size)
    {
        size = read(fd, buffer, COUNT);  //参数：文件标识符，缓冲区，字节数
        if(size == -1){
            close(fd);
            printf("文件读取错误\n");
            return -1;
        }else{
            if(size > 0){
                printf("共读取字节%ld\n", size);
                for( int i = 0; i < size; i++)
                    printf("%c", buffer[i]);
                printf("\n");
            }else{
                printf("文件读取完毕\n");
            }
        }
    }
 
    return 0;
}
