#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

int shard_num = 0;
sem_t unnamed_sem;

void *plusOne(void *argv) {
    sem_wait(&unnamed_sem); //钥匙（信号量）-1
    int tmp = shard_num + 1;
    shard_num = tmp;
    sem_post(&unnamed_sem); //钥匙+1
}

int main() {
    sem_init(&unnamed_sem,0,1);

    pthread_t tid[10000];
    for (int i = 0; i < 10000; i++) {
        pthread_create(tid + i, NULL, plusOne, NULL);
    }

    for (int i = 0; i < 10000; i++) {
        pthread_join(tid[i], NULL);
    }
    
    printf("shard_num is %d\n", shard_num);

    sem_destroy(&unnamed_sem);
    
    return 0;
}