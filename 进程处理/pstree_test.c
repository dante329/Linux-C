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
        //永久挂起，"ps -ef"查看进程，可以看到子进程的pid以及ppid，顺着ppid一直找下去就会找到pid为1的进程，就是/sbin/init splash
        char ch = fgetc(stdin);    
    }

    return 0;
}
