 #include<stdio.h>
 #include<stdlib.h>
 #include<unistd.h>
 #include<sys/types.h> //用于包含Unix/Linux系统中系统数据类型定义的头文件。

 int main(int argc, char const *argv[])
 {
    printf("当前进程号为：%d\n",getpid());

    pid_t pid = fork();

    if(pid < 0)
    {
        printf("子进程生成失败\n");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0) //在子进程中pid会被修改为0,从而区分父进程
    {
        printf("子进程%d生成成功,它的父进程为%d\n",getpid(),getppid());
    }
    else //父进程进入该条件判断
    {
        printf("父进程%d仍然存在,它的子进程是%d\n",getpid(),pid);
    }

    // while(1){}

    return 0;
 }