//建立单个TCP连接 client端
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

void* write_to_server(void *arg)
{
    //将stdin中的数据send给client
    char *write_buf = malloc(1024);
    int sockfd = *(int*)arg;
    size_t count; //记录读了多少个字符
    int send_num;

    if(write_buf == NULL)
    {
        printf("写缓存分配失败，断开连接\n");
        shutdown(sockfd, SHUT_WR);
        perror("malloc server write_buf");
        return NULL;
    }
    
    while(fgets(write_buf,1023,stdin) != NULL)
    {
        count = strlen(write_buf);
        send_num = send(sockfd,write_buf,count,0);
        if(send_num == -1)
        {
            perror("send");
        }
    }

    printf("接收到命令行的终止信号，不再写入，关闭连接......\n");
    shutdown(sockfd, SHUT_WR);
    free(write_buf);

    return NULL;
}

void* read_from_server(void *arg)
{
    //将recv到的数据输出到stdout
    char *read_buf = malloc(1024);
    int sockfd = *(int*)arg;
    size_t count = 0;

    if(read_buf == NULL)
    {
        printf("读缓存分配失败，断开连接\n");
        shutdown(sockfd, SHUT_WR);
        perror("malloc server write_buf");
        return NULL;
    }
    
    while((count = recv(sockfd,read_buf,1023,0)))
    {
        if(count == -1)
        {
            perror("recv");
        }
        read_buf[count] = '\0';
        fputs(read_buf,stdout);
    }

    printf("服务端请求关闭连接......\n");
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
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //连接本机 127.0.0.1
    server_addr.sin_port = htons(6666);
    
    //填写client_addr
    client_addr.sin_family = AF_INET;
    inet_pton(AF_INET,"192.168.6.101", &client_addr.sin_addr);
    // client_addr.sin_addr.s_addr = htonl(INADDR_LOCAL); //连接本机 192.168.6.101
    client_addr.sin_port = htons(8888); //不能与server的port相同
    
    //网络编程流程
    //1.创建socket
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    handle_error("socket",sockfd);
    
    int tmp_result;
    //2.bind绑定地址（可以不绑定，就自动分配端口）
    tmp_result = bind(sockfd,(struct sockaddr *)&client_addr,sizeof(client_addr));
    handle_error("bind",tmp_result);

    //3.连接服务端
    tmp_result = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    handle_error("connect", tmp_result);

    printf("连接上服务器%s 端口号：%d\n",inet_ntoa(server_addr.sin_addr),ntohs(server_addr.sin_port));

    pthread_t pid_write, pid_read;
    
    // 启动一个子线程，用来从命令行读取数据并发送到服务端
    pthread_create(&pid_write,NULL,write_to_server,(void*)&sockfd);
    // 启动一个子线程，用来读取服务端数据，并打印到 stdout
    pthread_create(&pid_read,NULL,read_from_server,(void*)&sockfd);

    pthread_join(pid_write,NULL);
    pthread_join(pid_read,NULL);

    printf("关闭资源\n");

    close(sockfd);

    return 0;
}
