#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

/*
* 文件描述符：0：stdin, 1:stdout, 2:stdout (标准输入,标准输出,标准错误)
*/
/*
* O_RDWR: 打开文件并进行读写
* O_EXCL: 如果文件已经存在，则返回 -1
* O_CREAT: 如果文件不存在就创建它，需要保证具有创建文件的权限
* O_APPEND: 写的操作在文件末尾执行
* O_TRUNC: 文件成功打开后会将文件内容清空
*/
int main(int argc, char const *argv[])
{
    int fd = -1;
    char filename1[] = "test.txt";
    char filename2[] = "test2.txt";
    fd = open(filename1, O_RDWR);//出错的话文件描述符返回-1，成功的话返回 > 2的整数
    if(fd == -1){
        printf("打开文件错误,fd:%d\n", fd);
    }else{
        printf("打开文件成功, fd:%d\n", fd);
    }
    fd = open(filename2, O_RDWR | O_CREAT | O_EXCL, S_IRWXU); //安全的打开一个文件
    if(fd == -1){
        printf("该文件已经存在,fd:%d\n", fd);
        fd = open(filename2, O_RDWR);
        printf("fd:%d\n", fd);
    }else{
        printf("打开文件成功, fd:%d\n", fd);
    }
    close(fd); //关闭文件，让这个文件描述符能再次被占用
    return 0;
}
