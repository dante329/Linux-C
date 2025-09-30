/**
 * 目标：测试写操作不加锁。
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_rwlock_t rwlock;

int share_data = 0; //等会两个线程对这个共享变量+1

void* wirte_thread(void *arg)
{
    int tmp = share_data + 1; //两个线程读取到的share_data值一定都是0，因为下面sleep了1秒
    sleep(1);
    share_data = tmp;

    printf("this is %s,share_data's value is %d\n",(char*)arg, share_data);
}

void* read_thread(void *arg)
{
    //读操作加锁对线程读取没有影响，多个线程可以同时获取读锁，这些西线程可以同时读取共享资源。只是在读操作获得锁的时候，写操作会阻塞。
    pthread_rwlock_rdlock(&rwlock);
    printf("this is %s,share_data's value is %d\n",(char*)arg, share_data);
    pthread_rwlock_unlock(&rwlock);
}

int main(int argc, char const *argv[])
{
    //动态分配读写锁
    pthread_rwlock_init(&rwlock,NULL);

    //创建2个写线程，6个读线程
    pthread_t writer1,writer2;
    pthread_t reader1,reader2,reader3,reader4,reader5,reader6;

    pthread_create(&writer1,NULL,wirte_thread,"writer1");
    pthread_create(&writer2,NULL,wirte_thread,"writer2");

    sleep(3); //等待writer1、writer2线程结束

    pthread_create(&reader1,NULL,read_thread,"reader1");
    pthread_create(&reader2,NULL,read_thread,"reader2");
    pthread_create(&reader3,NULL,read_thread,"reader3");
    pthread_create(&reader4,NULL,read_thread,"reader4");
    pthread_create(&reader5,NULL,read_thread,"reader5");
    pthread_create(&reader6,NULL,read_thread,"reader6");

    pthread_join(writer1,NULL);
    pthread_join(writer2,NULL);

    pthread_join(reader1,NULL);
    pthread_join(reader2,NULL);
    pthread_join(reader3,NULL);
    pthread_join(reader4,NULL);
    pthread_join(reader5,NULL);
    pthread_join(reader6,NULL);

    pthread_rwlock_destroy(&rwlock);

    return 0;
}
