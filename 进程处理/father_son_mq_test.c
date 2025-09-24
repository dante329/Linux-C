#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    //设置mq_attr结构体
    struct mq_attr attr;
    //这两个属性决定了消息队列的容量
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 100;
    //这两个属性对于mq_open没用
    attr.mq_curmsgs = 0;
    attr.mq_flags = 0;
    
    //1.创建消息队列
    char* mq_name = "/mq_test";
    mqd_t mq = mq_open(mq_name,O_CREAT | O_RDWR,0644,&attr);
    if(mq < 0)
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }       

    //2.创建子进程
    pid_t pid = fork();

    if(pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        //子进程读消息
        char receive_buf[100] = {0};
        struct timespec time_info;
        for(int i=1;i<=10;i++)
        {
            memset(receive_buf,0,sizeof(receive_buf));
            //设置等待时间
            clock_gettime(CLOCK_REALTIME,&time_info);
            time_info.tv_sec += 15;

            //调用 mq_timedreceive 时，如果消息队列中有消息，消息会立即被读取。
            //如果消息队列为空，子进程会等待（最多 15 秒，超时后返回错误），这使得子进程能够处理父进程发送的消息。
            if(mq_timedreceive(mq,receive_buf,sizeof(receive_buf),NULL,&time_info) == -1)
            {
                perror("mq_timedrecevie");
            }
            printf("子进程接收到数据：%s\n",receive_buf);
        }
    

    }
    else
    {
        //父进程写消息
        char send_buf[100] = {0};
        struct timespec time_info;
        for(int i = 1; i <= 10; i++)
        {
            memset(send_buf,0,sizeof(send_buf));
            sprintf(send_buf,"这是老同学发送的第%d条消息\n",i);
            
            //设置等待时间，mq_timedsend调用&time_info参数获得最大等待时间
            clock_gettime(CLOCK_REALTIME,&time_info);
            time_info.tv_sec += 5;
            
            if(mq_timedsend(mq,send_buf,strlen(send_buf),0,&time_info) == -1)
            {
                perror("mq_timedsend");
            }
            //让父进程暂停 1 秒，给子进程更多的时间去读取消息
            printf("父进程发送一条消息，休眠1s\n");
            sleep(1);
        } 
    }

    close(mq);

    if(pid > 0)
    {
        mq_unlink(mq_name);
    }

    return 0;
}
