#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//默认取消类型为延迟
void* task(void* arg)
{
    puts("task thread begin.\n");  

    //模拟工作
    sleep(1); //如果这里不sleep1秒，子进程就会在pthread_cancel发送cancel请求前执行完了，就会报Thread was not canceled!
    puts("working...\n");

    //调用点函数取消线程
    pthread_testcancel();

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
