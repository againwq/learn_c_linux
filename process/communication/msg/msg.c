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
