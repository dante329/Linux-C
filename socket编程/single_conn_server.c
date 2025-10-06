//建立单个TCP连接 server端
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

#define handle_error(cmd, result) \
    if (result < 0)               \
    {                             \
        perror(cmd);              \
        return -1;                \
    }

void* write_to_client(void *arg)
{
    //将stdin中的数据send给client
    char *write_buf = malloc(1024);
    int clientfd = *(int*)arg;
    size_t count; //记录读了多少个字符
    int send_num;

    if(write_buf == NULL)
    {
        printf("写缓存分配失败，断开连接\n");
        shutdown(clientfd, SHUT_WR);
        perror("malloc server write_buf");
        return NULL;
    }
    
    while(fgets(write_buf,1023,stdin) != NULL)
    {
        count = strlen(write_buf);
        send_num = send(clientfd,write_buf,count,0);
        if(send_num == -1)
        {
            perror("send");
        }
    }

    printf("接收到命令行的终止信号，不再写入，关闭连接......\n");
    shutdown(clientfd, SHUT_WR);
    free(write_buf);

    return NULL;
}

void* read_from_client(void *arg)
{
    //将recv到的数据输出到stdout
    char *read_buf = malloc(1024);
    int clientfd = *(int*)arg;
    size_t count = 0;

    if(read_buf == NULL)
    {
        printf("读缓存分配失败，断开连接\n");
        shutdown(clientfd, SHUT_WR);
        perror("malloc server write_buf");
        return NULL;
    }
    
    while((count = recv(clientfd,read_buf,1024,0)))
    {
        if(count == -1)
        {
            perror("recv");
        }
        fputs(read_buf,stdout);
    }

    printf("客户端请求关闭连接......\n");
    free(read_buf);

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
    // inet_pton(AF_INET,"192.168.6.101", &server_addr.sin_addr);
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
    // clientfd是能与客户端收发消息的文件描述符
    //调用accept后如果没有客户端连接，会挂起等待
    int  clientfd = accept(sockfd,(struct sockaddr *)&client_addr,&cliaddr_len);
    handle_error("accept", clientfd);

    printf("与客户端 from %s at PORT %d 文件描述符 %d 建立连接\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port),  clientfd);

    pthread_t pid_write, pid_read;

    pthread_create(&pid_write,NULL,write_to_client,(void*)&clientfd);
    pthread_create(&pid_read,NULL,read_from_client,(void*)&clientfd);

    pthread_join(pid_write,NULL);
    pthread_join(pid_read,NULL);

    printf("关闭资源\n");
    close(clientfd);
    close(sockfd);

    return 0;
}
