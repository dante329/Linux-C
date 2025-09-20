#include<stdio.h>

int main(int argc, char const *argv[])
{
    char* filename = "io.txt"; 

    /**
     * extern FILE *fopen (const char *__restrict __filename,
                const char *__restrict __modes)
    */
    FILE* ioFile = fopen(filename,"w");
    
    if(ioFile == NULL) printf("打开文件失败\n");
    else printf("打开文件成功\n");

    /**
     * int fclose (FILE *__stream)
     */
    int ret = fclose(ioFile);

    if(ret == EOF) printf("关闭文件失败\n");
    else printf("关闭文件成功\n");

    return 0;
}
