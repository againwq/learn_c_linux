#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#define FILE_SIZE 80
int main(int argc, char const *argv[])
{
    int fd = -1;
    const char filename[] = "mmap_file.txt";
    char *ptr = NULL;
    char content[] = "this is what I write in memory";

    fd = open(filename, O_RDWR | O_EXCL | O_CREAT, S_IRWXU);
    if( fd == -1 ){
        printf("文件已经存在\n");
        fd = open(filename, O_RDWR);
    }

    lseek(fd, FILE_SIZE - 1, SEEK_SET);  //把文件大小扩展到 79 B
    write(fd, "a", 1);  //写入一个字符，扩展到80
    
    ptr = (char*)mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //进行映射

    if(ptr == (char*)-1){
        printf("映射失败\n");
        close(fd);
        return -1;
    }
    memcpy(ptr + 16, content, strlen(content));   //从起始位置偏移16写入数据
    munmap(ptr, FILE_SIZE);  //关闭映射
    close(fd);

    return 0;
}
