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

int PdfParse(char *src, char* destpath,char* tpath)
{
	char command[1024]={0};
	char command2[1024]={0};
	char paths[1024]={0};
	getcwd(paths,sizeof(paths));
	sprintf(command,"pdftotext %s/%s/%s - >> %s/temps/PDF.txt",destpath,tpath,src,destpath);
	sprintf(command2,"pdfinfo %s/%s/%s >> %s/temps/PDF.txt",destpath,tpath,src,destpath);
	system(command2);
	system(command);
	return 0;
}

int PdfMain(int argc, char* argv[])
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
	sprintf(command,"pdfinfo %s >> ../temps/pdf.txt",argv[1]);
	/*
    strcat(command,"pdfinfo ");
    strcat(command,argv[1]);
    strcat(command," >> ");
    strcat(command,filePath);*/
    system(command);
    return 0;
}
