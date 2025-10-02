/**
 * sem作为计数信号量用于线程间通信。
 * 计数信号量主要是用于控制进程或线程执行顺序的。
 */
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

sem_t *full;
sem_t *empty;

int share_num;

int rand_num()
{
    srand(time(NULL));
    return rand();
}

void * producer_thread(void *arg)
{
    for (size_t i = 0; i < 5; i++)
    {     
        sem_wait(empty);
        printf("\n这是第%ld轮通信\n\n",i+1);
        sleep(1);
        share_num = rand_num();
        sem_post(full);
    }
}

void * consumer_thread(void *arg)
{
    for (size_t i = 0; i < 5; i++)
    {
        sem_wait(full);
        puts("消费者开始接收数据...");
        sleep(1);
        printf("收到的数字是：%d\n",share_num);
        sem_post(empty);
    }
}

int main(int argc, char const *argv[])
{
    full = malloc(sizeof(sem_t));
    empty = malloc(sizeof(sem_t));

    pthread_t producer,consumer;
    
    sem_init(full,0,0);
    sem_init(empty,0,1);

    pthread_create(&producer,NULL,producer_thread,NULL);
    pthread_create(&consumer,NULL,consumer_thread,NULL);

    pthread_join(producer,NULL);
    pthread_join(consumer,NULL);
    
    sem_destroy(full);
    sem_destroy(empty);

    return 0;
}
