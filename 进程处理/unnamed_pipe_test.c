//父进程将argv[1]写入匿名管道，子进程读取后输出到终端
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    //参数不够报错
    if(argc < 2)
    {
        fprintf(stderr,"%s:请输入参数！",argv[0]);
        exit(EXIT_FAILURE);
    }
    
    //开匿名管道
    int pipefd[2];
    
    if(pipe(pipefd) == -1)
    {
        perror("管道创建失败");
        exit(EXIT_FAILURE);
    }

    char* buf = malloc(1); //方式1：如果声明char* buf;是不行的，没有为buf开辟空间，子进程在使用buf读取的时候会将数据存到随机的内存当中去
    //方式2：char buf; 直接声明一个buf也是可以的，在read和write中传参时对buf取地址即可

    //fork
    pid_t cpid = fork();
    if(cpid == -1)
    {
        perror("fork失败");
        exit(EXIT_FAILURE);
    }
    else if(cpid == 0)
    {
        //关闭写端
        close(pipefd[1]);
        //读数据
        usleep(1000);
            //方式1：
        char* str = "新学员收到消息:";
        write(STDOUT_FILENO,str,strlen(str));
            //方式2：
        char str[100] = {0};
        sprintf(str,"新学员%d收到消息:",cpid);
        write(STDOUT_FILENO,str,sizeof(str));
        while(read(pipefd[0],buf,1) > 0)
        {
            write(STDOUT_FILENO,buf,1);
        }
        //读完关闭读端
        write(STDOUT_FILENO,"\n",1);
        close(pipefd[0]);
        _exit(EXIT_SUCCESS);
    }
    else
    {
        //关闭读端
        close(pipefd[0]);
        //写入数据
        printf("老学员%d传出信息\n",getpid());
        write(pipefd[1],argv[1],strlen(argv[1]));
        //写入完毕后关闭写端
        close(pipefd[1]);
        //等待子进程结束
        waitpid(cpid,NULL,0);   
        exit(EXIT_SUCCESS);
    }
    
    return 0;
}
