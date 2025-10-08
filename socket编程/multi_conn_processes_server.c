/**
 * 可接受多个TCP连接的server端（多进程实现）
 * 功能：接收客户端发来的消息并回复收到
 */
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#define handle_error(cmd, result) \
    if (result < 0)               \
    {                             \
        perror(cmd);              \
        return -1;                \
    }

void zombie_dealer(int sig) {
    pid_t pid;
    int status;
    // 一个SIGCHLD可能对应多个子进程的退出
    // 使用while循环回收所有退出的子进程，避免僵尸进程的出现
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            printf("子进程: %d 以 %d 状态正常退出，已被回收\n", pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("子进程: %d 被 %d 信号杀死，已被回收\n", pid, WTERMSIG(status));
        } else {
            printf("子进程: %d 因其它原因退出，已被回收\n", pid);
        }
    }
}

void* read_from_client_then_reply(void *arg)
{
    int clientfd = *(int*)arg;

    int count = 0, send_count = 0;

    char* read_buf = malloc(1024 * sizeof(char));
    if(!read_buf)
    {
        printf("服务端读缓存创建异常,断开连接\n");
        //先关闭资源再退出
        close(clientfd);
        perror("server read_buf malloc");
        return NULL;
    }

    char* write_buf = malloc(1024 * sizeof(char));
    if(!read_buf)
    {
        printf("服务端写缓存创建异常,断开连接\n");
        //先关闭资源再退出
        close(clientfd);
        perror("server write_buf malloc");
        return NULL;
    }

    while((count = recv(clientfd,read_buf,1023,0))) //recv只接收1023个字节，因为recv是不会自动加上\0，所以留一位作为保险\0
    {
        if(count < 0)
        {
            perror("recv");
        }
        else //recv返回-1时，不能做 read_buf[count] = '\0'（即访问 read_buf[-1]）。要判断count不为负数
        {
            read_buf[count] = '\0'; //手动加上\0，双重保险
        }
        
        printf("收到客户读%d的消息:%s",clientfd,read_buf);

        strcpy(write_buf,"收到\n");
        // send_count = send(clientfd,write_buf,1024,0); //1024字节全传过去就有很多0在后面充当\0
        send_count = send(clientfd,write_buf,strlen(write_buf),0); //问题就出在这里，如果传的长度是strlen(write_buf)，那么传过去的数据是没有\0的，就要在客户端的recv代码段接收消息时加上\0

        if(send_count < 0)
        {
            perror("send");
        }
    }

    printf("客户端clientfd:%d 请求关闭连接...\n",clientfd);
    strcpy(write_buf, "服务端收到关闭请求\n");
    send_count = send(clientfd, write_buf, strlen(write_buf), 0);
    if (send_count < 0)
    {
        perror("send");
    }

    printf("已释放资源\n");
    // shutdown(clientfd,SHUT_WR);
    close(clientfd);
    free(read_buf);
    free(write_buf);

    return NULL;
}

int main(int argc, char const *argv[])
{
    struct sockaddr_in server_addr,client_addr;

    memset(&server_addr,0,sizeof(server_addr));
    memset(&client_addr,0,sizeof(client_addr));

    //填写server_addr
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(6666);
    
    //网络编程流程
    //1.调用socket
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    handle_error("socket",sockfd);
    
    //2.bind绑定地址
    int tmp_result = bind(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
    handle_error("bind",tmp_result);

    //3.进入监听状态
    tmp_result = listen(sockfd,128);
    handle_error("listen",tmp_result);

    //4.获取客户端的连接
    socklen_t cliaddr_len = sizeof(client_addr);

    // 注册信号处理函数，处理SIGCHLD信号，避免僵尸进程出现
    signal(SIGCHLD, zombie_dealer);

    while(1)
    {
        /**
         * 程序会停在这里，直到有客户端连接进来（accept会阻塞等待）
         * 当有客户端连接上之后，accept会返回该客户端的文件描述符clientf，通过这个clientf与客户端通信 
         */
        int clientfd = accept(sockfd,(struct sockaddr *)&client_addr,&cliaddr_len);
        handle_error("accept", clientfd);
        
        pid_t pid = fork();
        
        if(pid < 0)
        {
            perror("fork");
        }
        else if(pid == 0)
        {
            //关闭不需要使用的sockfd
            close(sockfd);
            printf("与客户端 from %s at PORT %d 文件描述符 %d 建立连接\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port),  clientfd);
            printf("新的服务端pid为: %d\n", getpid());
            read_from_client_then_reply((void*)&clientfd);     
            //exit()触发SIGCHLD信号
            exit(EXIT_SUCCESS); //即使子进程执行了 exit()，操作系统仍然在进程表中保留着子进程的状态信息。要通过zombie_dealer回收资源
        }
        else
        {
            //关闭不需要使用的clientfd
            close(clientfd);
        }      
    }

    printf("关闭资源\n");
    close(sockfd);

    return 0;
}
