#include <sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>

int main(int argc, char const *argv[])
{
    struct stat st;  // 定义一个保存文件状态的结构体
    if( stat("test.txt", &st) == -1){
        printf("获取文件状态出错");
        return -1;
    }
    printf("设备ID: %ld\n", st.st_dev);
    printf("文件节点: %ld\n", st.st_ino);
    printf("文件模式: %d\n", st.st_mode);
    printf("文件硬链接数目: %ld\n", st.st_nlink);
    printf("文件所有者ID: %d\n", st.st_uid);
    printf("文件所有者的组ID: %d\n", st.st_gid);
    printf("设备ID(特殊设备): %ld\n", st.st_rdev);
    printf("文件大小: %ld\n", st.st_size);
    printf("文件所在文件系统块大小: %ld\n", st.st_blksize);
    printf("文件占用块数量: %ld\n", st.st_blocks);
    printf("文件最后访问时间: %ld\n", st.st_atime);
    printf("文件最后修改时间: %ld\n", st.st_mtime);
    printf("文件最后状态改变时间: %ld\n", st.st_ctime);
    return 0;
}
