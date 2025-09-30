#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_COUNT 10000

static pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER; //static声明的锁不用显式销毁

void* add_thread(void *arg)
{
    int *p = (int*)arg;

    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
        //累加之前加锁，这样其他线程在获取锁的时候会陷入阻塞
        pthread_mutex_lock(&counter_mutex);
        (*p)++;
        //累加之后释放锁
        pthread_mutex_unlock(&counter_mutex);
    }
    
    return NULL;
}

int main(int argc, char const *argv[])
{
    //创建两个线程
    pthread_t pid1,pid2;
    int num = 0;
    pthread_create(&pid1,NULL,add_thread,&num);
    pthread_create(&pid1,NULL,add_thread,&num);

    //等待两个线程结束
    pthread_join(pid1,NULL);
    pthread_join(pid2,NULL);
 
    printf("sum:%d\n",num);
    return 0;
}
