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
    exit(-1);
}

void server_mode(int sockfd)
{
    int client_fd;
    static struct sockaddr_un client_addr;
    ssize_t nrecv;
    const char *ok = "OK\n";

    //1.bind 服务端的bind会创建Unix域套接字文件，使用完需要unlink。客户端只连接，不创建或删除socket文件。
    if (bind(sockfd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) < 0) {
        if (errno == EADDRINUSE) {
            /* 若文件已存在，先删除再重试一次 */
            unlink(SOCKET_PATH);
            if (bind(sockfd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) < 0) {
                handle_error("bind");
            }
        } else {
            handle_error("bind");
        }
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

    /* 正确处理 recv 返回值，按实际字节数回应 */
    while ((nrecv = recv(client_fd, buf, BUF_LEN - 1, 0)) > 0) {
        buf[nrecv] = '\0';
        if (strncmp(buf, "EOF", 3) == 0) {
            puts("接收到EOF,停止接收数据!");
            break;
        }
        printf("收到客户端的消息:%s", buf);
        if (send(client_fd, ok, strlen(ok), 0) < 0) handle_error("send");
    }
    if (nrecv < 0) handle_error("recv");
    /* nrecv == 0: 客户端已关闭连接 */

    close(client_fd);
    unlink(SOCKET_PATH);
}

void client_mode(int sockfd)
{
    if (connect(sockfd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) < 0) handle_error("connect");

    puts("连接到服务端!");

    ssize_t nread, nrecv;
    while (1) {
        printf("请输入要发送的信息:");
        nread = read(STDIN_FILENO, buf, BUF_LEN - 1);
        if (nread < 0) handle_error("read");
        if (nread == 0) break; /* EOF */
        buf[nread] = '\0';
        if (send(sockfd, buf, nread, 0) < 0) handle_error("send");
        nrecv = recv(sockfd, buf, BUF_LEN - 1, 0);
        if (nrecv < 0) handle_error("recv");
        if (nrecv == 0) //server端 调用 close() 或 shutdown() 时 
        { 
            puts("服务端关闭连接"); 
            break; 
        } 
        buf[nrecv] = '\0';
        printf("收到服务端的消息:%s", buf);
        if (strncmp(buf, "EOF", 3) == 0) break;
    }
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
