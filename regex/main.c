#include <pcre.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "all.h"

extern char* txtstr;
int flags=1;
/*
 *argv[1]表示文件名的输入参数
 * */
#define EML__SYSTEMS__
#ifdef  EML__SYSTEMS__
int RegexMain(int argc, char* argv[])
#else
int main(int argc,char* argv[])
#endif
{
    char oldpath[80]= {0};
    DIR* d;
    getcwd(oldpath,sizeof(oldpath));
    struct dirent *file;
    if(argc<2)
    {
        printf("error in input format!\n");
        return -1;
    }
    if(!(d=opendir(argv[1])))
    {
        printf("error in open dir : %s\n",argv[1]);
        return -1;
    }
    chdir(argv[1]);/*change into new path*/
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
#if __DEBUG
                printf("This is a directory\n");
#endif
                continue;
            }
        }
        if(LoadTxtPage(file->d_name)<0)
        {
            printf("error in LoadTxtPage \n");
            return -1;
        }
#if __DEBUG
        printf("\033[35m this page can be descrribed as follow:\n%s\n\033[0m",txtstr);
#endif
        /*
        -bankcard---idcard-----mobphone---emailadd---urls-------ip_address-
        --------07---------06---------05---------04---------03---------02--
        */
        char ss=255;
        argv[2]=(char*)(&ss);
        if((*argv[2])&(1<<4))
        {
#if __DEBUG
        	printf("in Matching email\n");
#endif
            //EmlMatch();
            if(EmlMatch())
            {
                *argv[2]=1;
               // goto BYSBYE;
            }
        }
        if((*argv[2])&(1<<6))
        {
#if __DEBUG
        	printf("in matching id card\n");
#endif
            //idCardMatch();
            if(idCardMatch())
            {
                *argv[2]=1;
               // goto BYSBYE;
            }
        }
        if((*argv[2])&(1<<5))
        {
#if __DEBUG
        	printf("in matching mobile phone\n");
#endif
            //mobileMatch();
            if(mobileMatch())
            {
                *argv[2]=1;
                //goto BYSBYE;
            }
        }
        if((*argv[2])&(1<<7))
        {
#if __DEBUG
        	printf("in matching bank card\n");
#endif
            //bankCardMatch();
            if(bankCardMatch())
            {
                *argv[2]=1;
                //goto BYSBYE;
            }
        }
        if((*argv[2])&(1<<3))
        {
#if __DEBUG
        	printf("in matching urls\n");
#endif
            //UrlMatch();
            if(UrlMatch())
            {
                *argv[2]=1;
               // goto BYSBYE;
            }
        }
        if((*argv[2])&(1<<2))
        {
#if __DEBUG
        	printf("in matching IP address\n");
#endif
            //IPMatch();
            if(IPMatch())
            {
                *argv[2]=1;
               // goto BYSBYE;
            }
        }
        RelasePage();
    }
BYSBYE:
    chdir(oldpath);
    closedir(d);
    return 0;
}

#include <assert.h>
int checkInclass(char * file, char* pattern)
{
    assert(file!=NULL && pattern!=NULL);
    if(LoadTxtPage(file)<0)
    {
        printf("error in LoadTxtPage \n");
        return -1;
    }
    int rte= compliemain(pattern,txtstr);
    RelasePage();
    return rte;
}


int main(int argc, char* argv[])
{
    UrlMatch();
    return;
}
