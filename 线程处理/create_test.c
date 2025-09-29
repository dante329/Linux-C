#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_LEN 1024

char* buf; //声明缓存区

//input从标准输入读取数据存到缓存
void* input_thread(void*)
{
    int i = 0;
    while(1)
    {
        int ch = getc(stdin);
        if(ch && ch != '\n')
        {
            buf[i++] = ch;
        }
        if(i >= BUF_LEN-1)
        {
            i = 0;
        }
    }
}

//output从缓存中读取数据写到标准输出，逐字符还行
void* output_thread(void*)
{
    int i = 0;
    while(1)
    {
        if(buf[i] != 0)
        {
            fputc(buf[i],stdout);
            fputc('\n',stdout);

            buf[i++] = 0; //清空读取完毕的字节。如果不清空，等到i到达1023重置为0后就会自动输出
            
            if(i >= BUF_LEN-1)
            {
                i = 0;
            }
        }
        else 
        {
            sleep(1);
        }
    }
}

//创建两个线程：input和output
int main(int argc, char const *argv[])
{
    pthread_t input_pid,output_pid;

    buf = (char*)calloc(BUF_LEN,1);

    pthread_create(&input_pid,NULL,input_thread,NULL);
    pthread_create(&output_pid,NULL,output_thread,NULL);

    pthread_join(input_pid,NULL);
    pthread_join(output_pid,NULL);

    free(buf);

    return 0;
}
