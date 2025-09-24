#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    //1.创建共享内存对象，fd指向该共享内存对象
    char* shm_name = "/father_son_mq";
    int fd = shm_open(shm_name,O_RDWR | O_CREAT, 0644);
    if(fd < 0) 
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    
    //2.指定文件大小
    ftruncate(fd,100);

    //映射内存
    /**
     * void *mmap (void *__addr, size_t __len, int __prot,
		   int __flags, int __fd, __off_t __offset)
        返回值为一个void *，不明确类型的万用指针，根据实际需要在内存中写什么数据转化成什么类型的指针   
     */
    char* share = (char*)mmap(NULL,100,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    if(share == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    //映射完成后，关闭fd
    close(fd);

    pid_t pid = fork();

    if(pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        //写数据
        sprintf(share,"老同学%d,你是一个好人\n",getppid());
        printf("新学员%d成功写入数据\n",getpid());
    }
    else
    {
        waitpid(pid,NULL,0);
        printf("老学员%d看到新同学%d写入的内容是:%s",getpid(),pid,share);
        
        //释放映射区 断开自己进程里的“窗口”
        //只有父进程是是使用了mmap，所以在只用在父进程中使用munmap
        /**
         * int munmap (void *__addr, size_t __len)
         * void *addr: 这是指向之前通过 mmap() 映射的内存区域的起始地址的指针
         */
        int ret = munmap(share,100);
        if(ret == -1)
        {
            perror("munmap");
            exit(EXIT_FAILURE);
        }
    }

    //删除共享内存文件 从系统目录里把“房子注销”，但只要还有人住，就不会立刻消失。
    shm_unlink(shm_name);
    return 0;
}
