#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//不要担心线程变成"僵尸线程"，只有僵尸进程！
//线程无论如何都会自动回收资源即使不调用 pthread_join，线程资源也会被回收，不会产生"僵尸线程"

void* task(void* arg)
{
    puts("task thread begin.\n"); 

    //禁用取消响应
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
    puts("Thread CancelState is disabled\n");

    //更改取消类型为异步
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

    puts("working...\n");

    puts("after canceled.\n");

    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t pid;
    pthread_create(&pid,NULL,task,NULL);
    
    if(pthread_cancel(pid) != 0)
    {
        perror("pthread_cancel");
    }

    void *ret;
    pthread_join(pid,&ret); //pthread_join会初始化ret，所以ret并不是野指针

    if(ret == PTHREAD_CANCELED)
    {
        puts("Thread was canceled!\n");
    }
    else
    {
        printf("Thread was not canceled!exit code:%ld",(long)ret);
    }
    
    return 0;
}
