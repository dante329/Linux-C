#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* task(void* arg)
{
    puts("task thread begin.\n");

    sleep(2);

    puts("task thread ended.\n");

    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t pid;
    pthread_create(&pid,NULL,task,NULL);
    pthread_detach(pid);

    puts("main thread continuing...\n");
    
    sleep(3);

    puts("main thread ended.\n");
    return 0;
}
