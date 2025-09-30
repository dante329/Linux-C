#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE];
int count = 0;

//初始化互斥锁
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//初始化条件变量
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

/**
 * producer和consumer两个线程是同时进行的。对于count有两种不合法的情况需要处理：
 * 1.producer工作时count==BUFFER_SIZE，就需要停下来并将mutex解锁，待consumer将count--后再工作
 * 2.sonsumer工作时count==0,就需要停下来并将mutex解锁，待producer将count++后再工作
 */

void* producer_thread(void *arg)
{
    int num = 1;
    pthread_mutex_lock(&mutex);
    while(1)
    {
        while(count == BUFFER_SIZE) //使用while判断条件而不是if，每次被唤醒都重新检查，防止虚假唤醒，保证条件真的满足
        {
            pthread_cond_wait(&cond,&mutex);
        }
        buffer[count++] = num++;
        printf("生产者发送一个数字：%d\n",buffer[count-1]);
        pthread_cond_signal(&cond); //如果此时没有消费者在等待，这个信号就丢失了
    }
    pthread_mutex_unlock(&mutex);
}

void* consumer_thread(void *arg)
{
    pthread_mutex_lock(&mutex);
    while(1)
    {
        while(count == 0)
        {
            pthread_cond_wait(&cond,&mutex);
        }
        printf("消费者接收到一个数字：%d\n",buffer[--count]);
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char const *argv[])
{
    pthread_t producer,consumer;
    pthread_create(&producer,NULL,producer_thread,NULL);
    pthread_create(&consumer,NULL,consumer_thread,NULL);

    pthread_join(producer,NULL);
    pthread_join(consumer,NULL);

    return 0;
}