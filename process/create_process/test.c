#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{
    pid_t pid;

    int a = 0;    //子进程会拷贝一份a, a并不会共享

    pid = fork();
    printf("这是谁？ %d\n", getpid());
    if( pid == 0){
        printf("a: %d, a++: %d\n", a++, a);
    }else{
        printf("a: %d, a--: %d\n", a--, a);
    }

    return 0;
}
