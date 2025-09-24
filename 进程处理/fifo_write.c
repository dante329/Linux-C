#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    /**
     * int mkfifo (const char *__path, __mode_t __mode)
     * const char *__path：有名管道路径，mkfifo会创建一个路径为__path的FIFO专用文件
     * __mode_t __mode：指定该文件的权限，八进制数字
     * return:成功返回0,失败返回-1
     */

    //打开有名管道
    char* pathname = "/tmp/mypipe";
    if(mkfifo(pathname,0664) == -1)
    {
        perror("mkfifo");
        if(errno != 17) //17对应的是pathname文件已存在，可以将将继续使用
        {
            exit(EXIT_FAILURE);
        }
    }

    //open专用文件，注意：写端只写
    int fd = open(pathname,O_WRONLY);
    if(fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    //要将在终端中输入的内容读取然后写到FIFO专用文件中
    char write_buf[100];
    ssize_t read_byte;
    while((read_byte = read(STDIN_FILENO,write_buf,sizeof(write_buf))) > 0)
    {
        write(fd,write_buf,read_byte); //read_byte,读多少写多少
    }

    if(read_byte < 0)
    {
        perror("read");
        printf("命令行数据读取异常，进程退出！");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("发送管道退出，进程终止\n");
    close(fd);

    //清理管道专用文件
    if(unlink(pathname) == -1)
    {
        perror("fifo_write unlink");
    }

    return 0;
}
