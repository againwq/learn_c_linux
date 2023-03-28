#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <string.h>

typedef int sem_t;

union semun
{
    int val;  //整型变量
    struct semid_ds *buf;  
    unsigned short *array;
};

sem_t CreateSem(key_t key, int value){
    union semun sem;
    sem_t sem_id;
    sem.val = value;

    sem_id = semget(key, 0, IPC_CREAT | 0666);
    if(sem_id == -1){
        printf("创建信号量失败\n");
        return -1;
    }
    semctl(sem_id, 0, SETVAL, sem);

    return sem_id;

}

int Sem_P(sem_t sem_id){
    struct sembuf sops = { 0, +1, IPC_NOWAIT};
    return semop(sem_id, &sops, 1);
}

int Sem_V(sem_t sem_id){
    struct sembuf sops = {0, -1, IPC_NOWAIT};
    return semop(sem_id, &sops, 1);
}

void SetvalueSem(sem_t semid, int value){
    union semun sem;
    sem.val = value;
    
    semctl(semid, 0, GETVAL, sem);
}

int GetvalueSem(sem_t semid){
    union semun sem;
    return semctl(semid, 0, GETVAL, sem);
}

void DestroySem(sem_t semid){
    union semun sem;
    sem.val = 0;
    semctl(semid, 0, IPC_RMID, sem);
}


int main(int argc, char const *argv[])
{
    key_t key;
    int semid, shmid;
    char msg[] = "共享内存";

    key = ftok("/ipc/sem", 'a');
    shmid = shmget(key, 1024, IPC_CREAT | 0604);

    semid = CreateSem(key, 0);
    pid_t p = fork();
    if(p == -1){
        printf("创建子进程失败\n");
        return -1;
    }
    if(p > 0){
        char *shms = (char*)shmat(shmid, 0, 0); //挂接共享内存

        memcpy(shms, msg, strlen(msg) + 1); //在共享内存中写数据
        sleep(10);
        Sem_P(semid);
        shmdt(shms);  // 摘除共享内存
    }else if(p == 0){
        char *shmc = (char*)shmat(shmid, 0, 0);  //挂接共享内存
        Sem_V(semid);
        printf("共享内存的值为: %s\n", shmc);  // 获取共享内存的值
        shmdt(shmc);   //删除共享内存
    }

    return 0;
}
