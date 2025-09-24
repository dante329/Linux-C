#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    //打开有名管道
    char* pathname = "/tmp/mypipe";

    //open专用文件，注意：读端只读
    int fd = open(pathname,O_RDONLY);
    if(fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    //读取FIFO专用文件中的数据。注意：当进程通过FIFO交换数据时，内核会在内部传递所有数据，不会将其写入文件系统。
    char read_buf[100];
    ssize_t read_byte;
    while((read_byte = read(fd,read_buf,sizeof(read_buf))) > 0)
    {
        write(STDOUT_FILENO,read_buf,read_byte); //read_byte,读多少写多少
    }

    if(read_byte < 0)
    {
        perror("read");
        printf("接收管道读取异常，进程退出！");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("接收管道退出，进程终止\n");
    close(fd);

    return 0;
}
