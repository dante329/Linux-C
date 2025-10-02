//使用信号量解决存在竞态条件的进程
//无名信号量默认只能在同一进程的线程间共享，不能在进程间共享。
//要在进程间共享无名信号量，必须把无名信号量放在共享内存中！
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>

int main() {
    char *shm_value_name = "unnamed_sem_shm_value";
    char *shm_sem_name = "unnamed_sem_shm_sem";
    // 创建内存共享对象
    int value_fd = shm_open(shm_value_name, O_CREAT | O_RDWR, 0666);
    int sem_fd = shm_open(shm_sem_name,O_CREAT | O_RDWR, 0666); //创建内存共享信号量
    // 调整内存共享对象的大小
    ftruncate(value_fd, sizeof(int));
    ftruncate(sem_fd, sizeof(sem_t));
    // 将内存共享对象映射到共享内存区域
    int *value = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, value_fd, 0);
    sem_t *sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, sem_fd, 0);
    // 初始化共享变量的值
    *value = 0;
    sem_init(sem,1,1);

    int pid = fork();

    if (pid > 0) {
        sem_wait(sem);
        int tmp = *value + 1;
        sleep(1);
        *value = tmp;
        sem_post(sem);
        
        // 等待子进程执行完毕
        waitpid(pid, NULL, 0);
        printf("this is father, child finished\n");
        printf("the final value is %d\n", *value);
        // 父进程作为最后一个使用者应该销毁共享信号量
        // sem_destroy 只能在没有其他进程/线程正在等待该信号量时调用
        if (sem_destroy(sem) == -1) {
            perror("sem_destroy");
        }
    } else if (pid == 0) {
        sem_wait(sem);
        int tmp = *value + 1;
        sleep(1);
        *value = tmp;
        sem_post(sem);
    } else {
        perror("fork");
    }

    // 无论父子进程都应该解除共享内存的映射，并关闭共享对象的文件描述符
    if (munmap(value, sizeof(int)) == -1)
    {
        perror("munmap value");
    }

    if (close(value_fd) == -1)
    {
        perror("close value");
    }

    if (munmap(sem, sizeof(sem_t)) == -1)
    {
        perror("munmap sem");
    }

    if (close(sem_fd) == -1)
    {
        perror("close sem");
    }

    // 如果调用时别的进程仍在使用共享对象，则等待所有进程释放资源后，才会销毁相关资源。
    // shm_unlink只能调用一次,这里在父进程中调用shm_unlink
    if (pid > 0)
    {
        if (shm_unlink(shm_value_name) == -1)
        {
            perror("father shm_unlink shm_value_name");
        }

        if (shm_unlink(shm_sem_name) == -1)
        {
            perror("father shm_unlink shm_sem_name");
        }
    }

    return 0;
}