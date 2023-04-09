#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

void *producter_f(void *arg);
void *consumer_f(void *arg);

pthread_mutex_t mutex;  //设置线程锁对象
int buffer_has_item = 0;
int running = 1;

int main(int argc, char const *argv[])
{
    pthread_t consumer_t;
    pthread_t producter_t;

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&consumer_t, NULL, (void*)consumer_f, NULL);
    pthread_create(&producter_t, NULL, (void*)producter_f, NULL);
    
    usleep(1);
    running = 0;
    pthread_join(consumer_t ,NULL);
    pthread_join(producter_t, NULL);
    pthread_mutex_destroy(&mutex);

    return 0;
}

void *producter_f(void *arg){
    while (running)
    {
        pthread_mutex_lock(&mutex);//使用线程锁
        buffer_has_item ++;
        printf("生产,总数量: %d\n", buffer_has_item);
        pthread_mutex_unlock(&mutex);//解除线程锁
    }
}

void *consumer_f(void *arg){
    while (running)
    {
        pthread_mutex_lock(&mutex);
        buffer_has_item--;
        printf("消费,总数量: %d\n", buffer_has_item);
        pthread_mutex_unlock(&mutex);
    }
}
