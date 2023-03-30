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
大小).那么该进程会阻塞,等待管道的数据被读出才恢复运行

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

</br>

## 消息队列

- 消息队列在Linux内核中是以链表的形式存储的,创建一个消息队列的大体过程如下:</p>

    1. 使用*ftok*函数创建一个key
    2. 将key传入*msgget*函数获取消息队列的唯一标识*msg_id*.之后对消息队列进行读写等操作都是依靠这个*msg_id*</p>
    3. 创建消息队列时也有几个可选模式
        1. 0: 取消息队列标识符，若不存在则函数会报错
        2. IPC_CREAT：当msgflg&IPC_CREAT为真时,如果内核中不存在键值与key相等的消息队列,则新建一个消息队列;如果
        存在这样的消息队列，返回此消息队列的标识符
        3. IPC_EXCL：如果内核中存在键值与key相等的消息队列,报错

- 为了描述一个消息队列,我们常常用到以下几个类型的数据:</p>

    1. msg_id: int类型,用户标明唯一的消息队列
    2. msg_info: *struct msqid_ds*类型, 该结构体用于保存消息队列的大小、数量、uid和gid等详细信息
    3. msg_mbuf: *struct 自定义*类型,该结构体是消息的缓冲区结构,即消息的结构。可以**自定义**

- msg_mbuf类型的参考格式

    ``` C
    struct msgmbuf 
    {
        int mtype; //消息类型
        char mtext[10];  //消息内容,可以是任意其他类型
    };
    ```

- 消息队列的相关操作如下:</p>

    1. 发送消息(即在消息队列中写入一个消息):
        - 函数: `int msgsnd (int msg_id, const void *__msgp, size_t msgsz, int msgflg)`
        - 参数:
            1. msg_id: 消息队列的id
            2. __msgp: 即上面提到的*mgs_info* 自定义的消息结构体(传入前要先把要发送的数据写在*msgmbuf.mtext*中)
            3. msgsz: 要发送的数据的长度(若为0,则默认为4,即mtype的长度)
            4. msgflag:
                1. 0: 当消息队列满时，msgsnd将会*阻塞*，直到消息能写进消息队列
                2. IPC_NOWAIT: 当消息队列已满的时候，msgsnd函数*不等待*立即返回
                3. IPC_NOERROR: 若发送的消息大于*msgsz*个字节，则把该消息截断，截断部分将被丢弃，且不通知发送进程。
    2. 接收消息
        - 函数: `ssize_t msgrcv (int msg_id, void *__msgp, size_t __msgsz, long int msgtyp, int msgflg);`
        - 参数: 同上,只介绍特殊的几个
            1. msgtyp: 对应*msgmbuf.mtype*,即消息类型。为0时返回消息队列第一个消息(不管是什么类型),大于零时才会找到</p>
            对应的消息类型的第一个消息, 小于零时会返回消息类型等于或者小于msgtyp绝对值的第一个消息
            2. msgflag:
                1. IPC_NOWAIT: 如果没有返回条件的消息调用立即返回，此时错误码为ENOMSG
                2. IPC_EXCEPT: 与msgtype配合使用返回队列中第一个类型不为msgtype的消息
                3. IPC_NOERROR: 如果队列中满足条件的消息内容大于所请求的size字节，则把该消息截断，截断部分将被丢弃
        - 返回值: 正常返回实际读取到的字节数,数据保存在*msgp.mtext*中
    3. 控制消息队列(如修改消息队列属性, 删除消息队列等)
        - 函数: `int msgctl (int msg_id, int cmd, struct msqid_ds *__buf)`
        - 参数: 这里只解释cmd的几种取值</p>
            1. IPC_STAT: 获得msgid的消息队列头数据到buf中
            2. IPC_SET: 设置消息队列的属性，要设置的属性需先存储在buf中，可设置的属性包括：msg_perm.uid、msg_perm.gid、msg_perm.mode以及msg_qbytes
            3. IPC_RMID: 删除消息队列(此时第三个参数传入*NULL*即可)

- 代码示例:

    ``` C
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/types.h>
    #include <sys/msg.h>
    #include <unistd.h>
    #include <time.h>
    #include <sys/ipc.h>
    #define MTEXT_SIZE 20

    void msg_show_attr(int msg_id, struct msqid_ds msg_info){
        int ret = -1;
        sleep(1);
        ret = msgctl(msg_id, IPC_STAT, &msg_info);
        if(ret == -1){
            printf("获取消息失败\n");
            return;
        }
        printf("现在消息队列总字节数: %ld\n", msg_info.__msg_cbytes);
        printf("队列中消息数: %d\n", (int)msg_info.msg_qnum);
        printf("队列中最大字节数: %d\n", (int)msg_info.msg_qbytes);
        printf("最后发送消息的进程pid: %d\n", msg_info.msg_lspid);
        printf("最后接收消息的进程pid: %d\n", msg_info.msg_lrpid);
        printf("最后发送消息的时间: %s\n", ctime(&(msg_info.msg_stime)));
        printf("最后接收消息的时间: %s\n", ctime(&(msg_info.msg_rtime)));
        printf("最后变化的时间: %s\n",ctime(&(msg_info.msg_ctime)));
        printf("消息的UID: %d\n", msg_info.msg_perm.uid);
        printf("消息的GID: %d\n", msg_info.msg_perm.gid);
    }

    struct msgmbuf   //消息的缓冲区结构
    {
        int mtype;
        char mtext[MTEXT_SIZE];
    };


    int main(int argc, char const *argv[])
    {
        int ret = -1;
        int msg_flags, msg_id;  //msg_id: 消息队列的唯一标识
        key_t key; // 创建消息队列所需要的key
        struct msqid_ds msg_info;  // msg_info：消息队列的属性
        struct msgmbuf msg_mbuf;  // 消息队列中每个消息的结构

        int msg_sflags, msg_rflags; //建立发送和接收数据时的模式(flag)

        char *msgpath = "/ipc/msg";
        key = ftok(msgpath, 'c');  //建立key,key在内核中用于区分其他消息队列

        if(key == -1){
            printf("创建key失败\n");
        }else{
            printf("成功建立key\n");
        }
        msg_flags = IPC_CREAT | IPC_EXCL;
        msg_id = msgget(key, msg_flags | 0x0666);  // 创建消息队列，获取消息队列的ID
        if(msg_id == -1){
            printf("消息建立失败\n");
            return 0;
        }
        msg_show_attr(msg_id, msg_info);

        msg_sflags = IPC_NOWAIT;
        msg_mbuf.mtype = 10;
        memcpy(msg_mbuf.mtext, "测试消息", sizeof("测试消息"));
        ret = msgsnd(msg_id, &msg_mbuf, sizeof("测试消息"), msg_sflags);  //发送消息

        if(ret == -1){
            printf("消息发送失败\n");
        }
        msg_show_attr(msg_id, msg_info);

        msg_rflags = IPC_NOWAIT | MSG_NOERROR;
        ret = msgrcv(msg_id, &msg_mbuf, MTEXT_SIZE, MTEXT_SIZE, msg_rflags);  // 接收消息
        if(ret == -1){
            printf("接收消息失败\n");
        }else{
            printf("接收消息成功,长度为: %d\n", ret);
        }
        msg_show_attr(msg_id, msg_info);

        msg_info.msg_perm.uid = 8;
        msg_info.msg_perm.gid = 8;
        msg_info.msg_qbytes = 12345;
        ret = msgctl(msg_id, IPC_SET, &msg_info);  //设置消息属性
        if(ret == -1){
            printf("设置消息属性失败\n");
            return 0;
        }
        msg_show_attr(msg_id, msg_info);

        ret = msgctl(msg_id, IPC_RMID, NULL);  //删除消息
        if(ret == -1){
            printf("删除消息失败\n");  
            return 0;
        }

        return 0;
    }

    ```

</br>

## 共享内存

共享内存就是将一块内存地址作为数据交换的区域来进行通信实现过程分为以下几步

- 借助ftok创建的key来创建共享内存
- 挂接共享内存
- 对共享内存的地址块进行读写等操作
- 删除共享内存

</br>

## 信号量

</br>

## 信号
