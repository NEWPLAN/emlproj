#include "allhead.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include "NLPIR.h"
#define EML__SYSTEMS__

typedef char (*SpiPtr)[255];
extern int  RelasePage(void);
static SpiPtr ImportDic(void);
#ifndef EML__SYSTEMS__
int main(int argc, char *argv[])
#else
extern "C"
{
    int SpliterMain(int argc, char* argv[])
#endif
{
#if 0
    printf("shortpath here sorry\n");
    return 0;
#else
    int flags=0;
    if(argc==3)
        flags=1;
    char *q=NULL;
    clock_t A=clock();
    int NumPatt=0;
    SpiPtr pp=ImportDic();
    {
        char filepath[1024];
        DIR* d;
        struct dirent *file;
        struct stat info;
        stat(argv[1],&info);
        if(!S_ISDIR(info.st_mode))/*传递的如果是一个文件，单独处理,否则按照文件夹处理*/
            goto last_para;

        if(!(d=opendir(argv[1])))
        {
            printf("error in open dir : %s\n",argv[1]);
            return -1;
        }
        while((file=readdir(d))!=NULL)
        {
            if(strncmp(file->d_name,".",1)==0)
                continue;
            {
                /*判断是文件夹处理下一个*/
                struct stat info;
                stat(file->d_name,&info);
                if(S_ISDIR(info.st_mode))
                {
                    printf("This is a directory\n");
                    continue;
                }
            }
            memset(filepath,0,sizeof(filepath));
            strcat(filepath,argv[1]);
            filepath[strlen(filepath)]='/';
            strcat(filepath,file->d_name);
            q=testImportUserDict(flags,&NumPatt,filepath);
            printf("\n------using brute match methods---------\n");
            if(HashMach(q,pp,NumPatt))/*匹配到结果，可以返回垃圾*/
                *argv[2]|=1<<4;

            RelasePage();
            //free(q);
            flags=0;
        }
        closedir(d);
        return 0;
    }
    /*
    printf("\n-------using ACmachine methods----------\n");
    ACStart(q,pp,NumPatt);
    */
last_para:
    q=testImportUserDict(flags,&NumPatt,argv[1]);
    printf("\n------using brute match methods---------\n");
    //HashMach(q,pp,NumPatt);
    if(HashMach(q,pp,NumPatt))/*匹配到结果，可以返回垃圾*/
        *argv[2]|=1<<4;
    free(q);
    clock_t B=clock();
    printf("System Executing Time :%f(Second)\n",((double)B-A)/CLOCKS_PER_SEC);
    return 0;
#endif
}
#ifdef  EML__SYSTEMS__
}
#endif

extern "C"
{
    int SpliterInit(void)
    {
        if(!NLPIR_Init(0,1))
        {
            printf("NLPIR INIT FAILED!\n");  //初始化失败，退出。
            return 0;
        }
        else
        {
            printf("------------Init ok!--------------\n\n");
        }
        return 1;
    }

    int SpliterExit(void)
    {
        NLPIR_Exit();
        return 1;
    }
}


static SpiPtr ImportDic(void)
{
    int i=0;
    SpiPtr p=(SpiPtr)malloc(300*255);
    if(!p)
        return NULL;
    FILE *filPtr=fopen("userdict.txt","rb");
    if(!filPtr)
    {
        free(p);
        return NULL;
    }
    while (!feof(filPtr))
    {
        memset(p[i],0,255);
        int nu=fscanf(filPtr, "%s",p[i++]);
        if(nu<0)
            continue;
//		printf("%s\n",p[i-1]);
    }
    fclose(filPtr);
    return p;
}
