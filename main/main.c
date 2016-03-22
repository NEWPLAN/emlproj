/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2016Äê02ÔÂ17ÈÕ ÐÇÆÚÈý 10Ê±07·Ö13Ãë
 ************************************************************************/

#include <stdio.h>
#include "all.h"
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/time.h>

int main(int argc,char* argv[])
{
    int flags=0;
    char errorinfo[1024]={0};
    if(argc<2)
    {
        printf("error in  input format , try again!\n");
        return  0;
    }
    struct timeval tBeginTime, tEndTime;
    float fCostTime;

    gettimeofday(&tBeginTime,NULL);/*calculate timer*/
    
    //printf("\033[41;36msomthe here\n\033[0m");
//   goto DEBUSGGG;
	{
		printf("test for DB\n");
		DataPtr A= LoadAll();
		TestAll();
		FreeAll(A);
	}
   	printf("run for ParseEML\n"); 
    /*½âÎöÓÊ¼þ*/
    flags=ParseEML(argv[1]);
    if(!flags)
    {
        strcat(errorinfo,"ParseEML");
        goto  exit;
    }
    printf("\033[32m-----------------------------------------------------------------------------------------\n\n\n\n\033[0m");
    /*·´À¬»ø*/
ANTISPAMSTEST:
    flags=AntiSpams(argv[1]);
    if(!flags)
    {
        strcat(errorinfo,"AntiSpams");
        goto exit;
    }
    printf("\033[32m-----------------------------------------------------------------------------------------\n\n\n\n\033[0m");
    /*´¦ÀíURL*/
    flags=ParseURL("urldic/url");
    if(!flags)
    {
        strcat (errorinfo,"ParseURL");
        goto  exit;
    }
    printf("\033[32m-----------------------------------------------------------------------------------------\n\n\n\n\033[0m");
     /*´¦Àí¸½¼þ*/
TEST:
    flags= ParseAppendix("testfile");
    if(!flags)
    {
        strcat(errorinfo,"ParseAppendix");
        goto exit;
    }
    printf("\033[32m-----------------------------------------------------------------------------------------\n\n\n\n\033[0m");
     /*´¦Àí¹Ø¼ü×ÖÀà*/
    flags=ParseKeyClass("temps");
    if(!flags)
    {
        strcat(errorinfo, "ParseKeyClass");
        goto  exit;
    }
    printf("\033[32m-----------------------------------------------------------------------------------------\n\n\n\n\033[0m");
DEBUSGGG:
    /*´¦Àí¹Ø¼ü×Ö*/
    flags=ParseKeyChs("temps");
   // flags=ParseKeyChs(temps);
    if(!flags)
    {
        strcat(errorinfo,"ParseKeyChs");
        goto exit;
    }
    printf("\033[32m-----------------------------------------------------------------------------------------\n\n\n\n\033[0m");
    
    printf("parse eml done!\n");
    gettimeofday(&tEndTime,NULL);
    fCostTime = 1000000*(tEndTime.tv_sec-tBeginTime.tv_sec)+(tEndTime.tv_usec-tBeginTime.tv_usec);
    fCostTime /= 1000000;
    printf("\033[31m the execute time for parsing an email is = %f(Second)\n\033[0m",
    	fCostTime);
    return 0;

exit:
    printf("error in module %s\n",errorinfo);
    return 0;
}

int ParseEML(char* filename)
{
    char* inputs[2]={NULL,filename};
    void *handle;
    int (*dlfunc)(int argc,char* argv[]);
    struct timeval tBeginTime, tEndTime;
    float fCostTime;

    gettimeofday(&tBeginTime,NULL);/*calculate timer*/
    printf("hello in ParseEML \n");
    handle=dlopen("./gmimelibs.so",RTLD_LAZY);
    printf("in open libs\n");
    dlfunc=dlsym(handle,"GmimeMain");
    printf("in geting mainenter\n");
    if(!(handle&&dlfunc))
    {
        printf("error in open dynamic libs %s\n",dlerror());
        return 0;
    }
    dlfunc(2,inputs);
    gettimeofday(&tEndTime,NULL);
    fCostTime = 1000000*(tEndTime.tv_sec-tBeginTime.tv_sec)+(tEndTime.tv_usec-tBeginTime.tv_usec);
    fCostTime /= 1000000;
    printf("\033[31m the execute time for decoding EML is = %f(Second)\n\033[0m",
    	fCostTime);  
    return 1;
}


int ParseURL(char* filename)
{
   // printf("´¦Àí¶ñÒâurl·ÖÎö\n");
    return 1;
}

int ParseKeyChs(char* filename)
{
    static flags=1;
    char * inputs[3]={NULL,filename,NULL};
    int (*dlfunc)(int argc, char* argv[]);
    void *handle;
    struct timeval tBeginTime, tEndTime;
    float fCostTime;

    gettimeofday(&tBeginTime,NULL);/*calculate timer*/
    printf("hello in ParseKeyChs\n");
    handle=dlopen("./spliter.so",RTLD_LAZY);
    printf("in open libs\n");
    dlfunc=dlsym(handle,"SpliterMain");
    if(!(handle&&dlfunc))
    {
        printf("error in open dynamic libs %s\n",dlerror());
        return 0;
    }
    dlfunc(flags? 3:1,inputs);
    gettimeofday(&tEndTime,NULL);
    fCostTime = 1000000*(tEndTime.tv_sec-tBeginTime.tv_sec)+(tEndTime.tv_usec-tBeginTime.tv_usec);
    fCostTime /= 1000000;
    printf("\033[31m the execute time for parsing key characters is = %f(Second)\n\033[0m",
    	fCostTime);
    flags=0;
    return 1;
}

int ParseKeyClass(char* filename)
{
    int (*dlfunc)(int argc, char* argv[]);
    void *handle;
    char *inputs[2]={NULL,filename};
    struct timeval tBeginTime, tEndTime;
    float fCostTime;

    gettimeofday(&tBeginTime,NULL);/*calculate timer*/
    printf("hello in ParseKeyClass\n");
    handle=dlopen("./regex.so",RTLD_LAZY);
    printf("in open libs\n");
    dlfunc=dlsym(handle,"RegexMain");
    if(!(handle&&dlfunc))
    {
        printf("error in open dynamic libs %s\n",dlerror());
        return 0;
    }
    dlfunc(2,inputs);
    gettimeofday(&tEndTime,NULL);
    fCostTime = 1000000*(tEndTime.tv_sec-tBeginTime.tv_sec)+(tEndTime.tv_usec-tBeginTime.tv_usec);
    fCostTime /= 1000000;
    printf("\033[31m the execute time for parsing key class is = %f(Second)\n\033[0m",
    	fCostTime);
    return 1;
}

int ParseAppendix(char* filedirname)
{
    char * ins[2]={NULL,filedirname};
    int (*dlfunc)(int argc, char* argv[]);
    void *handle;
    printf("hello in ParseAppendix\n");
    handle=dlopen("./appendix.so",RTLD_LAZY);
    if (!handle)
        return 1;
    printf("in open libs\n");
    dlfunc=dlsym(handle,"AppendixMain");
    if(!(handle&&dlfunc))
    {
        printf("error in open dynamic libs %s\n",dlerror());
        return 0;
    }
    dlfunc(2,ins);
    return 1;
}

int AntiSpams(char* filename)
{
    struct mes RteVal;
    struct mes (*dlfunc)(const char* path);
    void *handle;
    struct timeval tBeginTime, tEndTime;
    float fCostTime;

    gettimeofday(&tBeginTime,NULL);/*calculate timer*/
    printf("hello in AntiSpams\n");
    handle=dlopen("./libantispam.so",RTLD_LAZY);
    printf("in open libs\n");
    dlfunc=dlsym(handle,"detectSpam");
    if(!(handle&&dlfunc))
    {
        printf("error in open dynamic libs %s\n",dlerror());
        return 0;
    }
    RteVal=dlfunc(filename);
    printf("the score is %2.2lf\n",RteVal.score);
    gettimeofday(&tEndTime,NULL);
    fCostTime = 1000000*(tEndTime.tv_sec-tBeginTime.tv_sec)+(tEndTime.tv_usec-tBeginTime.tv_usec);
    fCostTime /= 1000000;
    printf("\033[31m the execute time for detecting spams is = %f(Second)\n\033[0m",
    	fCostTime);
    return 1;
}

int AntiVirus(char* filepath)
{
    struct antivirusInfo RteVal;
    struct antivirusInfo (*dlfunc)(char* path,int flags);
    void *handle;
    struct timeval tBeginTime, tEndTime;
    float fCostTime;

    gettimeofday(&tBeginTime,NULL);/*calculate timer*/    
    printf("hello in AntiVirus\n");
    handle=dlopen("./libantivirus.so",RTLD_LAZY);
    if (!handle)
        return 1;
    printf("in open libs\n");
    dlfunc=dlsym(handle,"antiVirus");
    if(!(handle&&dlfunc))
    {
        printf("error in open dynamic libs %s\n",dlerror());
        return 0;
    }
    RteVal=dlfunc(filepath,0);
    if(RteVal.errorInfo==0)
        printf("this detect result is %d [1->virus, 0->notvirus]\n\
                this size of file is %2.2LfMB\n\
                this detail of virus is :%s\n"
                ,RteVal.isVirus,RteVal.fileSize,RteVal.virusInfo);
    else{
        switch(RteVal.errorInfo)
        {
            case OPEN_FILE_ERROR:
                printf("open file error\n");
                break;
            case INIT_LIBCLAMAV_ERROR:
                printf("initilize libclamav error\n");
                break;
            case CREATE_ENGINE_ERROR:
                printf("create antivirus engine error\n");
                break;
            case LOAD_DATABASE_ERROR:
                printf("load libvirus database error\n");
                break;
            case DETECT_VIRUS_ERROR:
                printf("detecting file error\n");
                break;
            case DATABASE_INIT_ERROR:
                printf("antivirus database init error\n");
                break;
            default:
                break;
        }
    }
    gettimeofday(&tEndTime,NULL);
    fCostTime = 1000000*(tEndTime.tv_sec-tBeginTime.tv_sec)+(tEndTime.tv_usec-tBeginTime.tv_usec);
    fCostTime /= 1000000;
    printf("\033[31m the execute time for detecting virus is = %f(Second)\n\033[0m",
    	fCostTime);
    return 1;
}
