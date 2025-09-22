#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wait.h>

int main(int argc, char const *argv[])
{
    //跳转之前
    printf("老同学%d在学校深造\n",getpid());
    int* subprocess_status;
    //fork子进程
    int pid = fork();

    if(pid < 0)
    {
        perror("fork");
        return 1;
    }
    else if(pid == 0)
    {
        //子进程跳转
        char* args[] = {"/usr/bin/ping","-c","10","www.baidu.com",NULL};
        char* envs[] = {NULL};
        printf("新同学%d联系老弟10次\n",getpid());
        int execveR = execve(args[0],args,envs);
    }
    else
    {
        //父进等待子进程
        printf("老同学%d等待新同学%d联系\n",getpid(),pid);
        // fflush(stdout);
        waitpid(pid,subprocess_status,0);    
        printf("新同学%d联系完毕\n",pid);
    }
     
    return 0;
}
