#include <signal.h>
#include <stdio.h>
#include <unistd.h>
typedef void (*signhandler_t)(int);
static void sig_handle(int signo){
    if(SIGSTOP == signo){
        printf("接收到信号: SIGSTOP\n");
    }else if(SIGKILL == signo){
        printf("接收到信号: SIGKILL\n");
    }else{
        printf("接收到信号: %d\n", signo);
    }
    return;
}

int main(int argc, char const *argv[])
{
    signhandler_t ret;
    ret = signal(SIGSTOP, sig_handle);  //给指定信号挂载处理函数
    if(SIG_ERR == ret){
        printf("为SIGSTOP挂载信号处理函数失败\n");
        return -1;
    }
    ret = signal(SIGKILL, sig_handle);
    if(ret == SIG_ERR){
        printf("为SIGKILL挂载信号处理函数失败\n");
        return -1;
    }
    sleep(10);
    return 0;
}
