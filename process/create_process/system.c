#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//system函数也会创建一个进程,他背后要调用fork, execve, waitpid等函数,有一个失败就会导致system函数调用失败

int main(int argc, char const *argv[])
{
    int ret;
    printf("当前进程号: %d\n", getpid());
    ret = system("ping www.baidu.com -c 2");  //直接操作sh
    printf("返回值为ret: %d\n", ret);
    return 0;
}
