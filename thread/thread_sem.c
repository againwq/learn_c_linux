#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

void *producter_handle (void *arg);
void *conmuser_handle(void *arg);
sem_t sem;
int running = 1;
int main(int argc, char const *argv[])
{
    pthread_t conmuser_t, producter_t;

    sem_init(&sem, 0, 16);

    pthread_create(&producter_t, NULL, (void*)producter_handle, NULL);
    pthread_create(&conmuser_t, NULL, (void*)conmuser_handle, NULL);
    sleep(1);

    running = 0;
    pthread_join(producter_t, NULL);
    pthread_join(conmuser_t, NULL);

    sem_destroy(&sem);
    return 0;
}

void *producter_handle(void *arg){
    int sem_val = 0;
    while (running)
    {
        usleep(1);
        sem_post(&sem); //使信号量 +1
        sem_getvalue(&sem, &sem_val); //获得信号量
        printf("生产,总数量: %d\n", sem_val);
    }
    
}

void *conmuser_handle(void *arg){
    int semval = 0;
    while (running)
    {
        usleep(1);
        sem_wait(&sem); // 使信号量-1
        sem_getvalue(&sem, &semval); //获得信号量
        printf("消费, 总数量为: %d\n", semval);
    }
    
}
