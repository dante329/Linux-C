#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

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

    //2.绑定地址
    socklen_t seraddr_len = sizeof(server_addr);
    socklen_t cliaddr_len = sizeof(client_addr);
    temp_result = bind(sockfd,(struct sockaddr*)&server_addr,seraddr_len);
    handle_error("bind",temp_result);

    //收发数据  
    do
    {
        memset(buf,0,1024);
        //接收数据存到缓冲  
        temp_result = recvfrom(sockfd,buf,1023,0,(struct sockaddr*)&client_addr,&cliaddr_len);
        handle_error("recvfrom",temp_result);

        //判断消息不是EOF就打印
        if(strncmp(buf,"EOF",3) != 0)
        {
            printf("接收到客户端%s port:%d 信息:%s\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port),buf);
            strcpy(buf,"OK\n");
        }
        else
        {
            printf("收到结束信息，准备关闭\n");   
        }
        
        //回复信息
        temp_result = sendto(sockfd,buf,1024,0,(struct sockaddr*)&client_addr,cliaddr_len);
        handle_error("sendto",temp_result);
    } while (strncmp(buf,"EOF",3) != 0);
    
    free(buf);

    return 0;
}