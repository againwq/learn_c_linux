// UNIX socket是用于同一台主机之间的进程通信,普通socket是面向不同主机的通信,这点需要区分开来
// UNIX socket的速度要更快,而且能传递文件描述符
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <strings.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

static void display_err(const char *on_what);

int main(int argc, char const *argv[])
{   
    int err;
    int sock_unix;
    struct sockaddr_un addr_unix;
    int len_unix;
    const char path[] = "/tmp/path"; //路径名 这是unix socket特殊的地方

    sock_unix = socket(AF_UNIX, SOCK_STREAM, 0); //使用unix协议族
    if(sock_unix < 0){
        display_err("socket()");
    }

    unlink(path);  // 解绑

    bzero(&addr_unix, 0);
    addr_unix.sun_family = AF_LOCAL;
    strcpy(addr_unix.sun_path, path);  // path用于描述一个unix socket, 不再使用IP
    len_unix = sizeof(struct sockaddr_un);

    addr_unix.sun_path[0] = 0;

    err = bind(sock_unix, (struct sockaddr*)&addr_unix, len_unix); // 绑定socket地址
    if( err < 0 ){
        display_err("bind()");
    }

    close(sock_unix);

    return 0;
}

static void display_err(const char *on_what){
    perror(on_what);
    exit(1);
}
