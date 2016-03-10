/*************************************************************************
	> File Name: unzips.c
	> Author: 
	> Mail: 
	> Created Time: 2016年01月26日 星期二 16时25分31秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include "all.h"
#define EML__SYSTEMS__
#ifdef  EML__SYSTEMS__
    int ZipsMain(int argc, char* argv[])
#else
    int main(int argc, char* argv[])
#endif
{
	pid_t childs;
    if(argc<2)
    {
        printf("error in input format!\n");
        return 0;
    }
    if((childs=fork())==0)
    {
        if(execlp("unzip","unzip",argv[1],(void *)0)==-1)
		{
		    printf("error in deal with  this zip file\n");
		    return 0;
		}
    }
    sleep(1);
    //waitpid(childs);

    
    return 0;
}
