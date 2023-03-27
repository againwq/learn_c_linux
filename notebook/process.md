# Linux进程间通信

</br>

## 管道通信

&emsp;管道通信分为两者,匿名管道和命名管道.管道通信实际上就是两个进程都访问一个指定的管道文件,</p>
一个进程写,一个进程读.是*半双工通信*.使用管道通信时,需要使用进程的*两个文件描述符*,分别作为管</p>
道的读写端(通信双方都能进行读写操作).一般这样定义:</p>

`int fd[2]; int *write_fd = fd[1]; int *read_fd = fd[0]; int res = pipe(fd);` </p>

**进行管道通信时需要注意当前进程是*阻塞模式*还是*非阻塞模式*(模式切换使用fnctl函数)**</p>

- *阻塞模式*: 阻塞模式下对管道读端进行read,如果没有数据被写入,则进程会被阻塞住,等待有数据被写</p>
入才恢复运行.写操作时若一次性写入的数据超过了某个阈值(PIPE_BUF,一般为65535B,可以认为是管道的</p>
大小).那个该进程会阻塞,等待管道的数据被读出才恢复运行

- *非阻塞模式*: 对于阻塞模式下的情况,进程不会等待,read和write函数都会立即返回-1

### 关于管道操作的原子性问题

</br>

>&emsp; POSIX 规定, 小于 *PIPE_BUF* 的写操作必须是原子的：要写的数据应被连续地写到管道；大于 *PIPE_BUF* 的写操作可能是非原子的：内核可能会将数据与其它进程>写入的数据交织在一起。POSIX 规定 PIPE_BUF *至少为512字节*（Linux 中为4096字节）, 具体的语义如下：（其中n为要写的字节数）</br>
>&emsp;  n <= PIPE_BUF, O_NONBLOCK disable</br>
>&emsp;  写入具有原子性。如果没有足够的空间供 n 个字节全部立即写入, 则*阻塞*直到有足够空间将n个字节全部写入管道。</br>
>&emsp;  n <= PIPE_BUF, O_NONBLOCK enable</br>
>&emsp;  写入具有原子性。如果有足够的空间写入 n 个字节, 则 write 立即成功返回, 并写入所有 n 个字节；否则一个都不写入, write 返回错误, 并将 errno 设置> 为 EAGAIN。</br>
>&emsp;  n > PIPE_BUF, O_NONBLOCK disable</br>
>&emsp;  写入不具有原子性。*可能会和其它的写进程交替写*,直到将 n 个字节全部写入才返回, 否则阻塞等待写入。</br>
>&emsp;  n > PIPE_BUF, O_NONBLOCK enable</br>
>&emsp;  写入不具有原子性。如果管道已满, 则写入失败, write 返回错误, 并将 errno 设置为 EAGAIN；否则, 可以写入 1 ~ n 个字节, 即部分写入, 此时 write 返>回实际写入的字节数, 并且写入这些字节时可能与其他进程交错写入。</br>
>

</br>

### 匿名管道pipe

- 创建: `int fd[2]; int *write_fd = fd[1]; int *read_fd = fd[0]; int res = pipe(fd);` res为返回的管道标识符 </p>
- 示例:

    ``` C
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/types.h>

    int main(int argc, char const *argv[])
    {
        int fd[2];
        int res = -1;
        int *write_fd = &fd[1];   //指定读写端
        int *read_fd = &fd[0];
        pid_t pid;
        char buf[80];

        res = pipe(fd);
        if(res == -1){
            printf("管道创建失败");
            return -1;
        }
        pid = fork();
        if(pid == -1){
            printf("子进程创建失败\n");
            return -1;
        }
        if(pid == 0){   //子进程
            close(*write_fd);
            ssize_t size = read(*read_fd, buf, 80); //等待父进程写数据,阻塞等待
            printf("共读取了%ld字节, 内容为%s\n", size, buf);
        }else{
            sleep(5);   //父进程阻塞,子进程等待有数据被写入才能读出数据
            close(*read_fd);
            ssize_t size = write(*write_fd, "测试数据", sizeof("测试数据"));
            printf("写完数据了,写了%ld字节\n", size);
        }

        return 0;
    }

    ```

</br>

### 命名管道

## 消息队列

</br>

## 共享内存

</br>

## 信号量
