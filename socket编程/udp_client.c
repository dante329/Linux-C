#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define handle_error(cmd, result) \
    if (result < 0)               \
    {                             \
        perror(cmd);              \
        return -1;                \
    }

int main(int argc, char const *argv[])
{
    int sockfd, temp_result, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char *buf = calloc(1024,sizeof(char));

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // 声明IPV4通信协议
    server_addr.sin_family = AF_INET;
    // 我们需要绑定0.0.0.0地址，转换成网络字节序后完成设置
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 端口随便用一个，但是不要用特权端口
    server_addr.sin_port = htons(6666);

    //udp编程流程
    //1.创建socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    handle_error("socket",sockfd);

    //2.客户端不绑定地址，自动分配端口
    socklen_t seraddr_len = sizeof(server_addr);
    socklen_t cliaddr_len = sizeof(client_addr);

    //收发数据
    do
    {
        printf("请输入要发送的信息：\n");
        int buf_len = read(STDIN_FILENO,buf,1023); //留最后一个0当"\0"
        handle_error("read",buf_len);
        sendto(sockfd,buf,buf_len,0,(struct sockaddr*)&server_addr,seraddr_len);

        //清空缓冲区数据，用来接收数据
        memset(buf,0,1024);
        recvfrom(sockfd,buf,1024,0,NULL,NULL);
        if(strncmp(buf,"EOF",3) != 0)
        {
            printf("收到来自服务端%s:%d的消息:%s",inet_ntoa(server_addr.sin_addr),ntohs(server_addr.sin_port),buf);
        }
    } while (strncmp(buf,"EOF",3) != 0);
    
    free(buf);
    
    return 0;
}