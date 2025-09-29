#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* task(void* arg)
{
    puts("task thread begin.\n");  

    //更改取消类型为异步
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    
    puts("working...\n");

    //会在任意位置取消线程，取决于什么时候接收到取消的指令
    int i=0;
    while(1)
    {
        printf("%d\n",i++);
    }

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
