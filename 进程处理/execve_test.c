#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char const *argv[])
{
    //跳转前
    char* name = "朱白岩";
    printf("我是%s,编号%d,我现在在一楼\n",name,getpid());

    //execve跳转
    /**
     * int execve (const char *__path, char *const __argv[],
		   char *const __envp[])
        const char *__path:执行程序的路径，是执行程序的argv[0]
        char *const __argv[]:执行程序中main函数的argv，要自己写出来,要求：
            1.第一个参数必须是执行程序的路径
            2.后面的参数分别是命令行中的参数
            3.最后一个必须是NULL
        char *const __envp[]:多个环境变量参数
            1.按键值对格式传入环境变量"key=value"
            2.最后一个必须是NULL
        return:成功不返回任何东西，因为下面的代码都不再执行了
               失败返回-1
     */
    
    char* args[] = {"/home/dante/process_test/erlou",name,NULL};
    char* envs[] = {"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin",NULL};

    int execveR = execve(args[0],args,envs);

    if(execveR == -1)
    {
        perror("execve");
        exit(EXIT_FAILURE);
    }

    return 0;
}
