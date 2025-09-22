#include<stdio.h>

int main(int argc, char const *argv[])
{
    char* filename = "io.txt"; 

    /**
     * extern FILE *fopen (const char *__restrict __filename,
                const char *__restrict __modes)
    __attribute_malloc__ __attr_dealloc_fclose __wur;
    */
    FILE* ioFile = fopen(filename,"w");
    
    if(ioFile == NULL) printf("打开文件失败\n");
    else printf("打开文件成功\n");

    return 0;
}
