#include <stdio.h>
#include <unistd.h>

//execve函数创建进程会替代掉原有的进程

int main(int argc, char const *argv[])
{
    char *args[] = {"/bin/ls", NULL};
    printf("当前进程号: %d", getpid());
    if(execve("/bin/ls", args, NULL) < 0)
        printf("创建进程失败");
    printf("当前进程号: %d", getpid());
    return 0;
}
