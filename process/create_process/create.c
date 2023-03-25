#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char const *argv[])
{
    pid_t pid, ppid;
    pid = getpid(); //获取当前进程号
    ppid = getppid(); //获取当前父进程号

    printf("当前进程号%d\n", pid);
    printf("当前父进程好%d\n", ppid);

    pid = fork();  //分叉进程  
    // fork执行成功后,下面的函数会执行两次,一次是当前程序执行,一次是fork出的子程序执行
    //子程序获得的pid为0,他的父进程就是执行fork函数的进程
    if(pid == -1){
        printf("进程复制失败");
        return -1;
    }else if(pid == 0){
        printf("子进程, fork 返回值: %d, ID: %d 父进程ID: %d\n", pid, getpid(), getppid());
    }else{
        printf("父进程, fork 返回值: %d, ID: %d 父进程ID: %d\n", pid, getpid(), getppid());
    }
    return 0;
}
