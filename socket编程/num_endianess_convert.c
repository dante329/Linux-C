#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

int main(int argc, char const *argv[])
{
    unsigned short local_num = 0x1f, network_num = 0;

    network_num = htons(local_num); //host to network short

    printf("将主机字节序的无符号整数:0x%hx 转换为网络字节序的结果为:0x%hx\n", local_num, network_num);

    local_num = ntohs(network_num);

    printf("将网络字节序的无符号整数:0x%hx 转换为主机字节序的结果为:0x%hx\n", network_num, local_num);

    return 0;
}