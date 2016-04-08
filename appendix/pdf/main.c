/*************************************************************************
	> File Name: pdfpharse.c
	> Author:
	> Mail:
	> Created Time: 2016年01月25日 星期一 15时39分47秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include "all.h"
#include <string.h>
#include <stdlib.h>

void usages(void)
{
    printf("you must input your pdf files at least\n");
}
#define EML__SYSTEMS__
#ifdef  EML__SYSTEMS__
int PdfMain(int argc, char* argv[])
#else
int main(int argc,char *argv[])
#endif
{
    pid_t childs;

    if(argc<3)
    {
        usages();
        return 0;
    }
    //system("pdftotext linux.pdf -");
    //system("pdfinfo linux.pdf >> 666.txt");
    //return 0;
    char command[2048]= {0};

    char filePath[1024]= {0};
    strcpy(filePath,"../temps/");
    strcat(filePath,argv[1]);
    strcat(filePath,".txt");
#if __DEBUG    
    printf("===========extract text from a pdf(write to %s)============\n\n",
           strcmp(argv[2],"-")?argv[2]:"stdout");
#endif    
    if((childs=fork())==0)
    {
        if(execlp("pdftotext","pdftotext",argv[1],filePath,(void*)0)==-1)
        {
            printf("error in  pharse  pdf\n");
            return 0;
        }
    }
    waitpid(childs);
#if __DEBUG    
    printf("=========show pdf extra info (write to 666.txt)=============\n\n");
#endif    
    strcat(command,"pdfinfo ");
    strcat(command,argv[1]);
    strcat(command," >> ");
    strcat(command,filePath);
    system(command);
    return 0;
    if((childs=fork())==0)
    {
        /*
        char fileinpus[80]={0};
        strcat(fileinpus,"pdfinfo linux.pdf >> 666.txt");
        system(fileinpus);
        return 0;
        */
        if(execlp("pdfinfo","pdfinfo",argv[1],">>pdfinfo.txt",(void*)0)==-1)
        {
            printf("error in deal with  show pdf info\n");
            return 0;
        }
    }
    waitpid(childs);
    return 0;
}
