#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define SOCKET_PATH "unix_domain.socket"
#define SERVER_MODE 1
#define CLIENT_MODE 2
#define BUF_LEN 1024

static struct sockaddr_un socket_addr; //UNIX 域套接字地址 结构体：struct sockaddr_un
static char *buf;

void handle_error(char *err_msg)
{
    perror(err_msg);
    unlink(SOCKET_PATH);
    exit(-1);
}

void server_mode(int sockfd)
{
    int client_fd;
    static struct sockaddr_un client_addr;

    //1.bind 服务端的bind会创建Unix域套接字文件，使用完需要unlink。客户端只连接，不创建或删除socket文件。
    if(bind(sockfd,(struct sockaddr*)&socket_addr,sizeof(socket_addr)) < 0)
    {
        handle_error("bind");
    }

    //listen
    if(listen(sockfd,128) < 0)
    {
        handle_error("listen");
    }

    //3.accept
    socklen_t client_addr_len = sizeof(client_addr);
    if((client_fd = accept(sockfd,(struct sockaddr*)&client_addr,&client_addr_len)) < 0)
    {
        handle_error("accept");
    }

    puts("连接到客户端!");

    //收发消息
    do
    {
        // memset(buf,0,BUF_LEN);
        if(recv(client_fd,buf,BUF_LEN,0) < 0) handle_error("recv"); //为什么这里的BUF_LEN改成1023之后就会出现bug，服务端会重复打印两次"收到客户端的消息:"。
        if(strncmp(buf,"EOF",3) != 0)
        {
            printf("收到客户端的消息:%s",buf);
            strcpy(buf,"OK\n\0");
        }
        else
        {
            puts("接收到EOF,停止接收数据!");
        }
        // send(client_fd,buf,BUF_LEN,0);
        send(client_fd,buf,strlen(buf),0);

    } while(strncmp(buf,"EOF",3) != 0);

    if (shutdown(client_fd, SHUT_RDWR) < 0)
    {
        handle_error("shutdown server");
    }
    unlink(SOCKET_PATH);
}

void client_mode(int sockfd)
{
    if (connect(sockfd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)))
    {
        handle_error("connect");
    }

    puts("连接到服务端!");

    do
    {
        puts("请输入要发送的信息:");

        memset(buf, 0, BUF_LEN);

        read(STDIN_FILENO, buf, BUF_LEN);

        if(send(sockfd, buf, BUF_LEN, 0) < 0) handle_error("send");

        memset(buf, 0, BUF_LEN);

        if(recv(sockfd, buf, 1023, 0) < 0) handle_error("recv"); //recv() 直接从 buf[0] 开始覆盖，不需要清空

        printf("收到服务端的消息:%s",buf);
        
    } while (strncmp(buf, "EOF", 3) != 0);
}

int main(int argc, char const *argv[])
{
    int sockfd = 0, mode = 0; //mode控制客户端/服务端

    if(argc == 1 || strncmp(argv[1],"server",6) == 0) //不填写参数 或 填写"server"
    {
        mode = SERVER_MODE;
    }
    else if(strncmp(argv[1],"client",6) == 0) //填写"client"
    {
        mode = CLIENT_MODE;
    }
    else
    {
        perror("参数填写错误");
    }

    //初始化
    buf = calloc(1024,sizeof(char));
    socket_addr.sun_family = AF_UNIX;
    strcpy(socket_addr.sun_path, SOCKET_PATH);

    if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        handle_error("socket");
    }

    switch (mode)
    {
    case SERVER_MODE:
        server_mode(sockfd);
        break;
    case CLIENT_MODE:
        client_mode(sockfd);
        break;
    }

    if(shutdown(sockfd,SHUT_RDWR) < 0)
    {
        handle_error("shutdown");
    }

    free(buf);

    return 0;
}
