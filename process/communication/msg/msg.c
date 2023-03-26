#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>

int main(int argc, char const *argv[])
{
    char *msgpath = "/ipc/msg";
    key_t key = ftok(msgpath, 'a');
    if(key != -1){
        printf("成功建立KEY\n");
    }else{
        printf("建立KEY失败\n");
    }
    return 0;
}
