#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 100;
    attr.mq_flags = 0;
    attr.mq_curmsgs = 0;

    //消费者端 打开消息队列（mq_open会使用使用O_RDWR这个flag）
    char* mq_name = "/p_c_mq";
    mqd_t mq = mq_open(mq_name, O_RDWR | O_CREAT, 0644, &attr);
    if(mq == -1)
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    //使用mp_timedreceive读取数据
    char receive_buf[100] = {0};
    struct timespec time_info;
    while(1)
    {
        //每次接收数据前，清空receive_buf中残留的数据
        memset(receive_buf,0,sizeof(receive_buf));
        
        //设置等待时间
        clock_gettime(0,&time_info);
        time_info.tv_sec += 15;

        if (mq_timedreceive(mq, receive_buf, sizeof(receive_buf), NULL, &time_info) == -1) 
        {
            //如果是等待从从超时就直接退出程序
            if (errno == ETIMEDOUT) 
            {
                printf("子进程接收超时，退出循环\n");
                break;
            }
            perror("mq_timedreceive");
        } 
   
        //判断接收的消息是不是EOF
        if(receive_buf[0] == EOF)
        {
            printf("接收到退出指令，正在结束对话...\n");
            sleep(1);
            printf("对话已结束\n");
            break;
        }

        printf("dante:%s\n",receive_buf);
    }

    close(mq);

    //mq_unlink只调用一次
    mq_unlink(mq_name);
    return 0;
}
