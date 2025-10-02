#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

sem_t *full;
sem_t *empty;

int shard_num;

int rand_num()
{
    srand(time(NULL));

    return rand();
}

void *producer(void *argv)
{
    for (int i = 0; i < 5; i++)
    {
        sem_wait(empty);
        printf("\n==========> 第 %d 轮数据传输 <=========\n\n", i + 1);
        sleep(1);
        shard_num = rand_num();
        printf("producer has sent data\n");
        sem_post(full);
    }
}

void *consumer(void *argv)
{
    for (int i = 0; i < 5; i++)
    {
        sem_wait(full);
        printf("consumer has read data\n");
        printf("the shard_num is %d\n", shard_num);
        sleep(1);
        sem_post(empty);
    }
}

int main()
{
    full = malloc(sizeof(sem_t));
    empty = malloc(sizeof(sem_t));

    sem_init(empty, 0, 1);
    sem_init(full, 0, 0);

    pthread_t producer_id, consumer_id;
    pthread_create(&producer_id, NULL, producer, NULL);
    pthread_create(&consumer_id, NULL, consumer, NULL);

    pthread_join(producer_id, NULL);
    pthread_join(consumer_id, NULL);

    sem_destroy(empty);
    sem_destroy(full);

    return 0;
}