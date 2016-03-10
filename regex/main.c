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
                printf("This is a directory\n");
                continue;
            }
        }
        if(LoadTxtPage(file->d_name)<0)
        {
            printf("error in LoadTxtPage \n");
            return -1;
        }
        printf("\033[35m this page can be descrribed as follow:\n%s\n\033[0m",txtstr);
        EmlMatch();
        idCardMatch();
        mobileMatch();
        bankCardMatch();
        UrlMatch();
        IPMatch();
        RelasePage();
    }
    chdir(oldpath);
    closedir(d);
    return 0;
}
