#include<stdio.h>
#include<stdlib.h>

int main(int argc, char const *argv[])
{
    //使用stdlib标准库函数创建子进程
    /**
     * int system (const char *__command)
     */
    int sysR = system("ping -c 10 www.baidu.com");
    //成功返回0

    if(sysR != 0)
    {
        perror("system");
        return 1;
    }

    return 0;
}
