#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char const *argv[])
{
    if(argc < 2)
    {
        printf("参数不够，无法上二楼\n");
        return 1;
    }

    printf("我是%s,进程号为%d,父进程为%d,我现在跟老弟一起上二楼\n",argv[1],getpid(),getppid());
    return 0;
}
