//fork后，父子进程共享文件描述符
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    int fd = open("io.txt",O_CREAT | O_WRONLY | O_APPEND, 0644);

    if(fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    char buffer[1024];

    if(pid < 0) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0) 
    {
        //子进程执行的代码
        strcpy(buffer,"这是子进程写的内容,卢浩杨是morbee\n");
    }
    else
    {
        //父进程执行的代码
        sleep(1);
        strcpy(buffer,"这是父进程写的内容,老弟是daven\n");
    }

    //父子进程都要执行的代码
    // ssize_t write_bytes = write(fd, buffer, sizeof buffer); //写入字符串不能用sizeof，sizeof会计算缓冲区总大小，可能会写入很多未知内容
    ssize_t write_bytes = write(fd, buffer, strlen(buffer)); //strlen才能获取字符串实际长度

    if(write_bytes == -1)
    {
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("写入数据成功\n");

    close(fd);
    
    if(pid == 0)
    {
        printf("子进程写入完毕，并释放文件描述符\n");
    }
    else
    {
        printf("父进程写入完毕，并释放文件描述符\n");
    }

    return 0;
}
