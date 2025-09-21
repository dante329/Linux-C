//execve一般是与fork搭配使用，使用fork创建出一个子进程，然后用子进程跳转
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char const *argv[])
{
    //跳转之前
    char* name = "daven";
    printf("老同学%s,编号%d,一开始在一楼学习\n",name,getpid());

    //跳转
    int pid = fork();
    
    if(pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        //子进程
        char* new_name = "morbee";
        char* args[] = {"./erlou",new_name,NULL};
        int execveR = execve(args[0],args,NULL);

        if(execveR == -1)
        {
            printf("上二楼失败\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        //父进程
        printf("老同学%d邀请完%d后继续在一楼深造\n",getpid(),pid);
        // sleep(1);
    }

    return 0;
}
