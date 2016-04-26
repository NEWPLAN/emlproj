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
#include <assert.h>
char* workspace=NULL;
char* dicpath=NULL;

typedef char (*SpiPtr)[255];
extern int  RelasePage(void);
static SpiPtr ImportDic();

extern "C"
{
    int SpliterMain(int argc, char* argv[])
	{
		int index=0;
		char *q=NULL;
		clock_t A=clock();
		int NumPatt=0;

		if(argc>=4)
		    dicpath=argv[3];

		SpiPtr dicptr=ImportDic();
		if(dicptr)
		{
		    char filepath[1024];
		    DIR* d;
		    struct dirent *file;
		    if(!(d=opendir(argv[1])))
		    {
		        printf("error in open dir : %s\n",argv[1]);
		        return 0;
		    }
		    while((file=readdir(d))!=NULL)
		    {
		        if(strncmp(file->d_name,".",1)==0)
		            continue;
		        {
		            /*判断是文件夹处理下一个*/
		            struct  stat info;
		            stat(file->d_name,&info);
		            if(S_ISDIR(info.st_mode))
		                continue;
		        }
		        workspace=argv[1];
		        assert(workspace!=NULL);
		        memset(filepath,0,sizeof(filepath));
		        strcat(filepath,argv[1]);
		        filepath[strlen(filepath)]='/';
		        strcat(filepath,file->d_name);
	#ifdef __DEBUG__8899
				printf("before testImportUserDict\n");
	#endif
		        q=testImportUserDict(1,&NumPatt,filepath);
	#ifdef __DEBUG__8899
		        printf("after testImportUserDict\n");
	#endif		        
	#ifdef __DEBUG
		        printf("\n------using brute match methods---------\n");
	#endif
		        index=HashMach(q,dicptr,NumPatt);
		        //printf("RelasePage before\n");
		        RelasePage();
		        //printf("RelasePage after\n");
		        if(index)
		        {
		        	//printf("free before\n");
		        	free(dicptr);
		        	//printf("free after\n");
		        	closedir(d);
		        	return index;
		        }
		    }
		    //printf("exit free before\n");
		    free(dicptr);
		    //printf("exit free after\n");
		    closedir(d);
		    return index;
		}
		//printf("return 0\n");
		return 0;
	}
}	
	
	
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


static SpiPtr ImportDic()
{
    int i=0;
    FILE *filPtr=NULL;
    SpiPtr p=(SpiPtr)malloc(300*255);
    if(!p)
    {
    	printf("fatal error!, can't malloc memory for dictionary, will return NULL\n");
        return NULL;
    }   
    if(!dicpath)
        filPtr=fopen("userdict.txt","rb");
    else
        filPtr=fopen(dicpath,"rb");
    if(!filPtr)
    {
        free(p);
        return NULL;
    }
    while (!feof(filPtr))
    {
        memset(p[i],0,255);
        int nu=fscanf(filPtr, "%s",p[i]);
        if(nu<0)
            continue;
        i++;
//		printf("the add dict is %s\n",p[i-1]);
    }
    fclose(filPtr);
    if(i==0)
    {
    	printf("dic is empty, will go back directly\n");
    	free(p);
    	return NULL;
    }
    return p;
}
