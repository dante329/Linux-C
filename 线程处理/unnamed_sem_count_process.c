/**
 * 无名信号量作为计数信号量控制进程的执行顺序
 * 要求即使子进程在水面，也要先执行子进程再执行父进程
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    char *shm_sem = "unnamed_shm_sem";

    int fd = shm_open(shm_sem, O_RDWR | O_CREAT, 0666);

    shm_unlink(shm_sem); //选择在开头直接删除共享文件，后面就不用考虑，由于fd对shm_sem还有引用计数，所以会在close(fd);后删除

    ftruncate(fd,sizeof(sem_t));

    sem_t *sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem_init(sem,1,0);

    /**
     * 子进程中pid变量会变成0
     * 父进程中的pid变量记录的是子进程的process ID
     */
    int pid = fork();

    if(pid < 0)
    {
        perror("fork");
    }
    else if(pid == 0)
    {
        //子进程
        sleep(1);
        puts("这是子进程!");
        sem_post(sem);
    }
    else
    {
        //父进程
        /**
         * 即使父进程先执行到这一步，也会因为sem为0而阻塞，
         * 只有等待子进程执行完之后调用sem_post(sem);使sem+1，这里才能继续执行
         */
        sem_wait(sem);  
        puts("这是父进程!");
    
        waitpid(pid,NULL,0);
        sem_destroy(sem); //没有进程在使用sem才能destroy
    }

    if(munmap(sem,sizeof(sem_t)) == -1)
    {
        perror("munmap");
    }

    if(close(fd) == -1)
    {
        perror("close");
    }

    return 0;
}
