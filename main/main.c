/*************************************************************************
	> File Name: main.c
	> Author: NEWPLAN
	> Mail:
	> Created Time: 2016年02月17日 
 ************************************************************************/

#include <stdio.h>
#include "all.h"
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/time.h>

/*
--------31---------30---------29---------28---------27---------26---------25---------24--------
---------|----------|----------|----------|----------|----------|----------|----------|--------
-0x80000000-0x40000000-0x20000000-0x10000000-0x08000000-0x04000000-0x02000000-0x01000000-------

--------23---------22---------21---------20---------19---------18---------17---------16--------
---------|----------|----------|----------|----------|----------|----------|----------|--------
-0x00800000-0x00400000-0x00200000-0x00100000-0x00080000-0x00040000-0x00020000-0x00010000-------

--------15---------14---------13---------12---------11---------10---------09---------08--------
---------|----------|----------|----------|----------|----------|----------|----------|--------
-0x00008000-0x00004000-0x00002000-0x00001000-0x00000800-0x00000400-0x00000200-0x00000100-------

--------07---------06---------05---------04---------03---------02---------01---------00--------
---------|----------|----------|----------|----------|----------|----------|----------|--------
-0x00000080-0x00000040-0x00000020-0x00000010-0x00000008-0x00000004-0x00000002-0x00000001-------
-isspam----isvirus-----isclass----iskeywords-antispam---antivirus--keyclass---keywords---------
*/
static int strategy_flags=0;
static DataPtr sqldatas=NULL;
static GmimeDataPtr mimedata=NULL;
static int setRegular(void);

int main(int argc,char* argv[])
{
    int flags=0;
    char errorinfo[1024]= {0};
    
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
	load_db(&strategy_flags);
    
    
    printf("run for ParseEML\n");
    /*邮件解析模块*/
    flags=ParseEML(argv[1],&mimedata);
    if(!flags)
    {
        strcat(errorinfo,"ParseEML");
        goto  exit;
    }
    if(mimedata)
    {
   		printf("=-=-=-=-=-=-=test for newplan=-=-=-=-=-=-=\n");
    	printf("sender:%s\n",mimedata->from );
    	printf("to:%s\n",mimedata->to );
    	printf("ReplayTo%s\n",mimedata->replayto );
    	printf("subjects:%s\n",mimedata->subjects );
    	printf("messageID%s\n",mimedata->messageID );
    }
    setRegular();
    
    printf("\033[32m-----------------------------------------------------------------------------------------\n\n\n\n\033[0m");
    /*反垃圾*/
ANTISPAMSTEST:
    flags=AntiSpams(argv[1]);
    if(!flags)
    {
        strcat(errorinfo,"AntiSpams");
        goto exit;
    }
    printf("\033[32m-----------------------------------------------------------------------------------------\n\n\n\n\033[0m");
    /*url分析*/
    flags=ParseURL("urldic/url");
    if(!flags)
    {
        strcat (errorinfo,"ParseURL");
        goto  exit;
    }
    printf("\033[32m-----------------------------------------------------------------------------------------\n\n\n\n\033[0m");
    /*附件解析*/
TEST:
    flags= ParseAppendix("testfile");
    if(!flags)
    {
        strcat(errorinfo,"ParseAppendix");
        goto exit;
    }
    printf("\033[32m-----------------------------------------------------------------------------------------\n\n\n\n\033[0m");
    /*关键字类*/
    flags=ParseKeyClass("temps");
    if(!flags)
    {
        strcat(errorinfo, "ParseKeyClass");
        goto  exit;
    }
    printf("\033[32m-----------------------------------------------------------------------------------------\n\n\n\n\033[0m");
DEBUSGGG:
    /*关键字*/
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
    printf("\033[31m the execute time for parsing an email is = %f(Second)\n\033[0m",fCostTime);
    //cleanAll();
    return 0;

exit:
	cleanAll();
    printf("error in module %s\n",errorinfo);
    return 0;
}

int ParseEML(char* filename,GmimeDataPtr* rtevalPtr)
{
    char* inputs[2]= {NULL,filename};
    void *handle;
    GmimeDataPtr A=NULL;
    GmimeDataPtr (*dlfunc)(int argc,char* argv[]);
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
    A=dlfunc(2,inputs);
#if DEBUG
    if(A)
    {
    	printf("sender:%s\n",A->from );
    	printf("to:%s\n",A->to );
    	printf("ReplayTo%s\n",A->replayto );
    	printf("subjects:%s\n",A->subjects );
    	printf("messageID%s\n",A->messageID );
    }
#endif
    *rtevalPtr=A;
    gettimeofday(&tEndTime,NULL);
    fCostTime = 1000000*(tEndTime.tv_sec-tBeginTime.tv_sec)+(tEndTime.tv_usec-tBeginTime.tv_usec);
    fCostTime /= 1000000;
    printf("\033[31m the execute time for decoding EML is = %f(Second)\n\033[0m",
           fCostTime);
    return 1;
}


int ParseURL(char* filename)
{
    // printf("URL测试模块\n");
    return 1;
}

int ParseKeyChs(char* filename)
{
    static flags=1;
    char * inputs[3]= {NULL,filename,NULL};
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
    char *inputs[2]= {NULL,filename};
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
    char * ins[2]= {NULL,filedirname};
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
    else
    {
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

DataPtr load_db(int *dbflags)
{
	printf("test for DB\n");
	DataPtr A= LoadAll();
	//TestAll();
	//FreeAll(A);
	sqldatas=A;
	return A;
}

void cleanAll(void)
{
	if(sqldatas)
		FreeAll(sqldatas);
	sqldatas=NULL;
	system("make clean");
}

#include <assert.h>
int setRegular(void)
{
	if(NULL==mimedata||NULL==sqldatas)
		return -1;
    int num=0;
	for(num=0;num<sqldatas->DLP_list_keywords_Num;num++)/*关键字*/
    {
        if(sqldatas->DLP_list_keywords_data[num].strategy_type[0]=='0')/*黑名单*/
        {
        	if((mimedata->from!=NULL)&&(sqldatas->DLP_list_keywords_data[num].strategy_info!=NULL))/*not null*/
		    {
		    	if(check_sub(mimedata->from,
						strlen(mimedata->from),
						sqldatas->DLP_list_keywords_data[num].strategy_info,
						strlen(sqldatas->DLP_list_keywords_data[num].strategy_info)
					)!=0)/*符合黑名单规则,添加*/
				{
				    FILE* fptr=fopen("userdict.txt","wb");
				    assert(fptr!=NULL);
				    char* ptrA,*ptrB;
				    ptrA=ptrB=sqldatas->DLP_list_keywords_data[num].strategy_content;
				    while(*ptrA!=0)
				    {
				    	if(*ptrA=='&')
				    	{
				    		*ptrA=0;
				    		fwrite(ptrB,sizeof(char),strlen(ptrB),fptr);
				    		fwrite("\n",sizeof(char),strlen("\n"),fptr);
				    		*ptrA='&';
				    		ptrB=++ptrA;
				    	}else
				    		ptrA++;
				    	
				    }
				    if(ptrB!=ptrA)
				    {
				    	fwrite(ptrB,sizeof(char),strlen(ptrB),fptr);
				    	fwrite("\n",sizeof(char),strlen("\n"),fptr);
				    	*ptrA='&';
				    }
				    
				}
		    }
        }
        else if(sqldatas->DLP_list_keywords_data[num].strategy_type[0]=='1')/*白名单，不过*/
       {
        	printf("white list\n");
        }
        
    }
    
    
    for(num=0;num<sqldatas->DLP_list_keywordsclass_Num;num++)/*关键字列表*/
    {
        
    }

    for(num=0;num<sqldatas->spam_list_Num;num++)/*垃圾列表*/
    {

    }

    for(num=0;num<sqldatas->spam_list_Num;num++)/*病毒列表*/
    {
        
    }
}

