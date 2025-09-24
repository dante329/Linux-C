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
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 100;
    attr.mq_flags = 0;
    attr.mq_curmsgs = 0;

    //生产者端 创建消息队列（使用O_CREAT）
    char* mq_name = "/p_c_mq";
    mqd_t mq = mq_open(mq_name, O_RDWR | O_CREAT, 0644, &attr);
    if(mq == -1)
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    //在stdin中循环读取数据
    char send_buf[100] = {0};
    struct timespec time_info;
    while(1)
    {
        //每次read前，清空send_buf中残留的数据
        memset(send_buf,0,sizeof(send_buf));
        ssize_t read_byte = read(STDIN_FILENO,send_buf,sizeof(send_buf));
        // if (read_byte > 0 && send_buf[read_byte - 1] == '\n') 
        // {
        //     read_byte--;  // 去掉换行符
        //     send_buf[read_byte] = '\0';  // 添加字符串结束符
        // }
        if(read_byte == -1)
        {
            perror("读取错误:");
            continue;
        }
        else if(read_byte == 0)
        {
            //ctrl + d退出进程
            printf("进程已退出,对话结束!\n");
            //同时告诉子进程对话结束
            char eof = -1;
            //设置等待时间
            clock_gettime(0,&time_info);
            time_info.tv_sec += 15;
            if(mq_timedsend(mq,&eof,1,0,&time_info) == -1)
            {
                perror("mq_timedsend");
            }
            break;
        }
        //正常发送消息
        clock_gettime(0,&time_info);
        time_info.tv_sec += 15;
        if(mq_timedsend(mq,send_buf,strlen(send_buf),0,&time_info) == -1)
        {
            perror("mq_timedsend");
        } 
        printf("消息已发送至消费者端\n");
    }

    close(mq);

    //在消费者端再mq_unlink，因为消费者端需要接受到“对话结束”的信息后才能关闭mq
    // mq_unlink(mq_name);
    return 0;
}
