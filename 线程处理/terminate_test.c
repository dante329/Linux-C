#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

//线程的回复通过Result传出
typedef struct Result
{
    char* p;
    int len; 
}Result;

//如果一个进程的多个线程同时使用到标准的输入输出，会造成冲突的问题，导致只有一个线程能使用到标准输入输出

void* red_thread(void* argv)
{
    char code = *((char*)argv);
    //pthread_exit的参数retval指向的区域不可以放在线程函数的栈内，所以用malloc开辟
    Result* ret = malloc(sizeof(Result));

    char ans[1024];

    while(1)
    {
        fgets(ans,1024,stdin); //读取标准输入中的内容
        if(ans[0] == code)
        {
            printf("红玫瑰离开了!\n");
            // char* redAns = strdup("红玫瑰独自去了纽约\n"); 没有必要使用堆上分配
            //strdup：使用 malloc 分配足够的内存来存储字符串副本，然后返回指向新字符串的指针

            char* redAns = "红玫瑰独自去了纽约\n";
            ret->len = strlen(redAns);
            ret->p = redAns;
            pthread_exit((void*)ret);
        }
        else
        {
            puts("红玫瑰还在等你!\n");
        }
    }
}

void* white_thread(void* argv)
{
    char code = *((char*)argv);
    //pthread_exit的参数retval指向的区域不可以放在线程函数的栈内，所以用malloc开辟
    Result* ret = malloc(sizeof(Result));

    // char* ans; 未初始化的野指针,会导致段错误

    char ans[1024]; //正确应该显式分配内存

    while(1)
    {
        fgets(ans,1024,stdin); //读取标准输入中的内容
        if(ans[0] == code)
        {
            printf("白玫瑰离开了!\n");
            char* redAns = "白玫瑰独自去了伯明翰\n";
            ret->len = strlen(redAns);
            ret->p = redAns;
            pthread_exit((void*)ret);
        }
        else
        {
            puts("白玫瑰还在等你!\n");
        }
    }
}

int main(int argc, char const *argv[])
{
    pthread_t red_pid,white_pid;
    char red_code = 'r',white_code = 'w'; //code传递线程函数，用于判断是给谁的回复

    pthread_create(&red_pid,NULL,red_thread,&red_code);
    pthread_create(&white_pid,NULL,white_thread,&white_code);

    Result *red_ret = NULL,*white_ret = NULL; //在变量声明中，* 修饰紧跟着的变量;在类型声明（包括函数返回类型、类型定义）中，* 是类型的一部分
    pthread_join(red_pid,(void**)&red_ret);
    printf("红玫瑰故事结局:%s\n",red_ret->p);
    free(red_ret);
    pthread_join(white_pid,(void**)&white_ret);
    printf("白玫瑰故事结局:%s\n",white_ret->p);
    free(white_ret);


    return 0;
}
