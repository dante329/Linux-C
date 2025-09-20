#include<stdio.h>

int main()
{
    //打开文件
    FILE* ioFile = fopen("info.txt","r");

    if(ioFile == NULL) printf("文件打开失败\n");
    else printf("文件打开成功\n");

    //fscanf读取
    char name[50],wife[50];
    int age;
    int fscanfR;

    //一次读入
    // int fscanfR = fscanf(ioFile,"%s %d %s",name,&age,wife);
    // if(fscanfR != EOF) printf("%s在%d岁的时候爱上了%s",name,age,wife);
    // else printf("fail"); 
    
    //循环读入所有
    while((fscanfR = fscanf(ioFile,"%s %d %s",name,&age,wife)) != EOF)
    {
        printf("当前成功匹配参数%d个\n",fscanfR);
        printf("%s在%d岁的时候爱上了%s\n",name,age,wife);
    }

    //关闭文件
    int ret = fclose(ioFile);

    if(ret == EOF) printf("文件关闭失败\n");
    else printf("文件关闭成功\n");
    
    return 0;
}
