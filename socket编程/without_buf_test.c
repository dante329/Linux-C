#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    FILE *file = fopen("/home/dante/develop_c/socket_test/testfile.txt","w");

    if(file == NULL)
    {
        perror("Failed to open file");
        return 1;
    }

    if(setvbuf(file,NULL,_IOLBF,0) != 0)
    {
        perror("Failed to set buffer mode");
        return 1;
    }
    

    fprintf(file,"hello\n");

    // fflush(file);

    char *args[] = {"/usr/bin/ping","-c","2","www.baidu.com",NULL};

    char *envs[] = {NULL};

    execve(args[0],args,envs);

    perror("execve");

    return 0;
}
