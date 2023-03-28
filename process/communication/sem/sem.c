#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
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
