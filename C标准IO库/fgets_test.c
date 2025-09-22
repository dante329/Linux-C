#include<stdio.h>

int main()
{
    //打开文件
    FILE* ioFile = fopen("io.txt","r");

    if(ioFile == NULL) printf("文件打开失败\n");
    else printf("文件打开成功\n");

    //fgets循环读整个文件
    char buffer[100];
    while(fgets(buffer, sizeof buffer, ioFile))
    {
        printf("%s",buffer); //%s 告诉 printf：把传入的参数当作 字符串指针（char*），从它指向的内存开始打印字符，直到遇到 \0 结束
    }

    //关闭文件
    int ret = fclose(ioFile);

    if(ret == EOF) printf("文件关闭失败\n");
    else printf("文件关闭成功\n");
    
    return 0;
}
