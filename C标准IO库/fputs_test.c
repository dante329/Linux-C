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
     * int fputc (int __c, FILE *__stream)
     * 成功返回char的值 失败返回EOF
     */
    int c = 97;
    int fputcR = fputc(c,ioFile);
    
    if(fputcR == EOF) printf("写入字符失败\n");
    else printf("写入字符%c成功,返回值%d\n",c,fputcR);

    /**
     * int fputs (const char *__restrict __s, FILE *__restrict __stream)
     */
    int fputsR = fputs(" love letter\n",ioFile);

    if(fputsR == EOF) printf("写入字符串失败\n");
    else printf("写入字符串成功\n");

    /**
     * int fclose (FILE *__stream)
     */
    int ret = fclose(ioFile);

    if(ret == EOF) printf("关闭文件失败\n");
    else printf("关闭文件成功\n");

    return 0;
}
