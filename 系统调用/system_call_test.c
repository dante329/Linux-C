#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>

int main()
{
    int fd = open("io.txt",O_RDONLY);

    if(fd < 0) 
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char* buffer[1024];
    
    size_t byte_read;
    while((byte_read = read(fd,buffer,sizeof buffer)) > 0)
    {
        write(STDOUT_FILENO,buffer,byte_read);
    }

    if(byte_read == -1)
    {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    return 0;
}
