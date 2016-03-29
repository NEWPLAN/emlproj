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
-bankcard---idcard-----mobphone---emailadd---urls-------ip_address-
--------07---------06---------05---------04---------03---------02---------01---------00--------
---------|----------|----------|----------|----------|----------|----------|----------|--------
-0x00000080-0x00000040-0x00000020-0x00000010-0x00000008-0x00000004-0x00000002-0x00000001-------
-isspam-----isvirus----isclass----iskeywords-do_spam----do_virus---do_class---do_keywords------
*/
static int strategy_flags=0x0f;
static DataPtr sqldatas=NULL;
static GmimeDataPtr mimedata=NULL;
static int setRegular(void);

/*黑白名单*/
#define WHITELIST '1'
#define BLACKLIST '0'

/*针对源地址和目的地址*/
#define TERMINAL  '1'
#define SOURCE    '0'


int main(int argc,char* argv[])
{
    int flags=0;
    char errorinfo[1024]= {0};
//goto DEBUSGGG;
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
    setRegular();
	FreeAll(sqldatas);
	sqldatas=NULL;
    printf("\033[32m-----------------------------------------------------------------------------------------\n\n\n\n\033[0m");
    /*反垃圾*/
ANTISPAMSTEST:
    if(strategy_flags&(1<<3))/*如果需要反病毒*/
    {
        flags=AntiSpams(argv[1]);
        if(!flags)
        {
            strcat(errorinfo,"AntiSpams");
            goto exit;
        }
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

    if(strategy_flags&(1<<1))/*关键字类*/
    {
        flags=ParseKeyClass("temps");
        if(!flags)
        {
            strcat(errorinfo, "ParseKeyClass");
            goto  exit;
        }
    }
    printf("\033[32m-----------------------------------------------------------------------------------------\n\n\n\n\033[0m");
DEBUSGGG:
    /*关键字*/
    if(strategy_flags&(1<<0))
    {
        flags=ParseKeyChs("temps");
        if(!flags)
        {
            strcat(errorinfo,"ParseKeyChs");
            goto exit;
        }
    }
    printf("\033[32m-----------------------------------------------------------------------------------------\n\n\n\n\033[0m");

    printf("parse eml done!\n");
    gettimeofday(&tEndTime,NULL);
    fCostTime = 1000000*(tEndTime.tv_sec-tBeginTime.tv_sec)+(tEndTime.tv_usec-tBeginTime.tv_usec);
    fCostTime /= 1000000;
    printf("\033[31m the execute time for parsing an email is = %f(Second)\n\033[0m",fCostTime);
    //cleanAll();
    //return 0;
    printf("%d",strategy_flags);
    return strategy_flags&0xf0;

exit:
    cleanAll();
    printf("error in module %s\n",errorinfo);
    return 0;
}

int ParseEML(char* filename,GmimeDataPtr* rtevalPtr)
{
    char* inputs[3]= {NULL,filename};
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
    printf("\033[31m the execute time for decoding EML is = %f(Second)\n\033[0m",fCostTime);
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
    
    char Ate=(char)(strategy_flags&0xff);
    int backval;
    int (*dlfunc)(int argc, char* argv[]);
    void *handle;
    struct timeval tBeginTime, tEndTime;
    float fCostTime;
    
    
    char * inputs[3]= {NULL,filename,&Ate};
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
    
    backval=(int)Ate;
    backval=backval&0xf0;/*只需要取得结果就可以了*/
    strategy_flags&=0xffffff0f;
    strategy_flags|=backval;
    
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
    int backval=0;
    char Ate=(char)((strategy_flags>>8)&0xff);
    char *inputs[3]= {NULL,filename,&Ate};
    struct timeval tBeginTime, tEndTime;
    float fCostTime;

    gettimeofday(&tBeginTime,NULL);/*calculate timer*/
    printf("hello in ParseKeyClass\n");
    handle=dlopen("./libregex.so",RTLD_LAZY);
    printf("in open libs\n");
    dlfunc=dlsym(handle,"RegexMain");
    if(!(handle&&dlfunc))
    {
        printf("error in open dynamic libs %s\n",dlerror());
        return 0;
    }
    dlfunc(2,inputs);
    if(Ate)
    	 strategy_flags|=1<<5;
    /*
    backval=(int)Ate;
    backval=backval<<8;
    backval=backval&0xff00;
    strategy_flags&=0xffff00ff;
    strategy_flags|=backval;
    */
    gettimeofday(&tEndTime,NULL);
    fCostTime = 1000000*(tEndTime.tv_sec-tBeginTime.tv_sec)+(tEndTime.tv_usec-tBeginTime.tv_usec);
    fCostTime /= 1000000;
    printf("\033[31m the execute time for parsing key class is = %f(Second)\n\033[0m",
           fCostTime);
    return 1;
}

int ParseAppendix(char* filedirname)
{
    char Ate=(char)(strategy_flags&0xff);
    int backval;
    char * ins[3]= {NULL,filedirname,&Ate};
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
    backval=(int)Ate;
    backval=backval&0xf0;/*只需要取得结果就可以了*/
    strategy_flags&=0xffffff0f;
    strategy_flags|=backval;

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
    strategy_flags=(RteVal.score>5?(strategy_flags|(1<<7)):strategy_flags);/*高于5分是垃圾*/
    gettimeofday(&tEndTime,NULL);
    fCostTime = 1000000*(tEndTime.tv_sec-tBeginTime.tv_sec)+(tEndTime.tv_usec-tBeginTime.tv_usec);
    fCostTime /= 1000000;
    printf("\033[31m the execute time for detecting spams is = %f(Second)\n\033[0m",
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
/*set regular for a email*/
int setRegular(void)
{
    if(NULL==mimedata||NULL==sqldatas)
        return -1;
    int num=0;
    char* mime_cmp=NULL;
    char* backups="1231231231231";

    /*关键字，处理思路是：将关键字加入到用户自定义的文件中，用于分词结束后，进行匹配过滤*/
    for(num=0; num<sqldatas->DLP_list_keywords_Num; num++)
    {
        /*看看是源地址还是目的地址的模式匹配*/
        mime_cmp=NULL;
        if(sqldatas->DLP_list_keywords_Data[num].strategy_terminal[0]==SOURCE)
            mime_cmp=mimedata->from;
        else if(sqldatas->DLP_list_keywords_Data[num].strategy_terminal[0]==TERMINAL)
            mime_cmp=mimedata->to;
        else
        {
            printf("in DLP_list_keywords\t can not recognise this kind of parameters make sure its correction!\n ");
            mime_cmp=backups;
        }
        assert(mime_cmp!=NULL);/*确保不能为空*/


        if(sqldatas->DLP_list_keywords_Data[num].strategy_type[0]==BLACKLIST)/*黑名单*/
        {
            if(mime_cmp!=NULL)/*确保非空*/
            {
                if(check_sub(mime_cmp,strlen(mime_cmp),
                             sqldatas->DLP_list_keywords_Data[num].strategy_info,
                             strlen(sqldatas->DLP_list_keywords_Data[num].strategy_info)
                            )!=0)/*符合黑名单规则,添加进入用户字典*/
                {
                    FILE* fptr=fopen("userdict.txt","ab");
                    assert(fptr!=NULL);
                    char* ptrA,*ptrB;
                    ptrA=ptrB=sqldatas->DLP_list_keywords_Data[num].strategy_content;
                    while(*ptrA!=0)
                    {
                        if(*ptrA=='&')
                        {
                            *ptrA=0;
                            fwrite(ptrB,sizeof(char),strlen(ptrB),fptr);
                            fwrite("\n",sizeof(char),strlen("\n"),fptr);
                            *ptrA='&';
                            ptrB=++ptrA;
                        }
                        else
                            ptrA++;

                    }
                    if(ptrB!=ptrA)
                    {
                        fwrite(ptrB,sizeof(char),strlen(ptrB),fptr);
                        fwrite("\n",sizeof(char),strlen("\n"),fptr);
                        //*ptrA='&';
                    }
                    fclose(fptr);

                }
            }
        }
        else if(sqldatas->DLP_list_keywords_Data[num].strategy_type[0]==WHITELIST)/*白名单，不过*/
            ;/*白名单对于过滤关键字来说没有意义*/
    }

    /*关键字类列表，这一部分的规则是，扫描数据库信息，然后设置相关的开关，在符合条件的开关选项上进行选择性*/
    for(num=0; num<sqldatas->DLP_list_keywordsclass_Num; num++)
    {
        /*看看是源地址还是目的地址的模式匹配*/
        mime_cmp=NULL;
        if(sqldatas->DLP_list_keywordsclass_Data[num].strategy_terminal[0]==SOURCE)
            mime_cmp=mimedata->from;
        else if(sqldatas->DLP_list_keywordsclass_Data[num].strategy_terminal[0]==TERMINAL)
            mime_cmp=mimedata->to;
        else
        {
            printf("in DLP_list_keywordsclass\t can not recognise this kind of parameters make sure its correction!\n ");
            mime_cmp=backups;
        }
        assert(mime_cmp!=NULL);/*确保不能为空*/

        if(sqldatas->DLP_list_keywordsclass_Data[num].strategy_type[0]==BLACKLIST)/*黑名单*/
        {
            if((mime_cmp!=NULL))/*not null*/
            {
                if(check_sub(mime_cmp,strlen(mime_cmp),
                             sqldatas->DLP_list_keywordsclass_Data[num].strategy_info,
                             strlen(sqldatas->DLP_list_keywordsclass_Data[num].strategy_info)
                            )!=0)/*符合黑名单规则,添加*/
                {
//                	printf("====== sqldatas->DLP_list_keywordsclass_Data[%d].strategy_content %s\n",num,sqldatas->DLP_list_keywordsclass_Data[num].strategy_content);
                    if(!strcmp(sqldatas->DLP_list_keywordsclass_Data[num].strategy_content,"bank card"))/*银行卡*/
                    {
                        strategy_flags|=1<<15;
                    }
                    else if(!strcmp(sqldatas->DLP_list_keywordsclass_Data[num].strategy_content,"id card"))/*身份证号*/
                    {
                        strategy_flags|=1<<14;
                    }
                    else if(!strcmp(sqldatas->DLP_list_keywordsclass_Data[num].strategy_content,"mobile phone"))/*电话号码*/
                    {
                        strategy_flags|=1<<13;
                    }
                    else if(!strcmp(sqldatas->DLP_list_keywordsclass_Data[num].strategy_content,"email address"))/*邮件地址*/
                    {
                        strategy_flags|=1<<12;
                    }
                    else if(!strcmp(sqldatas->DLP_list_keywordsclass_Data[num].strategy_content,"url"))/*链接地址*/
                    {
                        strategy_flags|=1<<11;
                    }
                    else if(!strcmp(sqldatas->DLP_list_keywordsclass_Data[num].strategy_content,"ip address"))/*iP 地址*/
                    {
                        strategy_flags|=1<<10;
                    }
                }
            }
            else if(sqldatas->DLP_list_keywordsclass_Data[num].strategy_type[0]=WHITELIST)/*白名单，不过*/
                ;/*关键字类的白名单也是没有过滤意义，因此选择不处理*/
        }
    }

    /*垃圾列表,针对这一部分处理方法是：如果遇到白名单，直接通过，如果遇到黑名单，直接拉黑*/
    for(num=0; num<sqldatas->spam_list_Num; num++)
    {
        /*看看是源地址还是目的地址的模式匹配*/
        mime_cmp=NULL;
        if(sqldatas->spam_list_Data[num].strategy_terminal[0]==SOURCE)
            mime_cmp=mimedata->from;
        else if(sqldatas->spam_list_Data[num].strategy_terminal[0]==TERMINAL)
            mime_cmp=mimedata->to;
        else
        {
            printf("in spam_list\t can not recognise this kind of parameters make sure its correction!\n ");
            mime_cmp=backups;
        }
        assert(mime_cmp!=NULL);/*确保不能为空*/


        if(sqldatas->spam_list_Data[num].strategy_type[0]==BLACKLIST)/*黑名单*/
        {
            if(mime_cmp)
            {
                if(check_sub(mime_cmp,strlen(mime_cmp),
                             sqldatas->spam_list_Data[num].strategy_info,
                             strlen(sqldatas->spam_list_Data[num].strategy_info))!=0)/*check for current regular*/
                {
                    strategy_flags|=1<<7;/*set flag and return */
                    strategy_flags&=(~(1<<3));/*清零标志位，因为已经可以判断是垃圾了,后面不需要扫描判断了*/
                    break;/*exit for this for loop*/
                }
            }
            else break;
        }
        else if(sqldatas->spam_list_Data[num].strategy_type[0]==WHITELIST)/*white名单*/
        {
            if(mime_cmp)
            {
                if(check_sub(mime_cmp,strlen(mime_cmp),
                             sqldatas->spam_list_Data[num].strategy_info,
                             strlen(sqldatas->spam_list_Data[num].strategy_info))!=0)/*check for current regular*/
                {
                    strategy_flags&=(~(1<<3));/*清零标志位，因为已经可以判断是垃圾了,后面不需要扫描判断了*/
                    break;/*exit for this for loop*/
                }
            }
            else break;
        }

    }

    /*病毒列表，这一部分处理方法是：对于黑名单直接拉黑，不处理，对于白名单，直接返回，也不需要处理*/
    for(num=0; num<sqldatas->virus_list_Num; num++)
    {
        /*看看是源地址还是目的地址的模式匹配*/
        mime_cmp=NULL;
        if(sqldatas->virus_list_Data[num].strategy_terminal[0]==SOURCE)
            mime_cmp=mimedata->from;
        else if(sqldatas->virus_list_Data[num].strategy_terminal[0]==TERMINAL)
            mime_cmp=mimedata->to;
        else
        {
            printf("in virus_list\t can not recognise this kind of parameters make sure its correction!\n ");
            mime_cmp=backups;
        }
        assert(mime_cmp!=NULL);/*确保不能为空*/

        if(sqldatas->virus_list_Data[num].strategy_type[0]==BLACKLIST)/*黑名单*/
        {
            if(mime_cmp)
            {
                if(check_sub(mime_cmp,strlen(mime_cmp),
                             sqldatas->virus_list_Data[num].strategy_info,
                             strlen(sqldatas->virus_list_Data[num].strategy_info))!=0)/*check for current regular*/
                {
                    strategy_flags|=1<<6;/*set flag and return */
                    strategy_flags&=(~(1<<2));/*清零标志位，因为已经可以判断是垃圾了,后面不需要扫描判断了*/
                    break;/*exit for this for loop*/
                }
            }
            else
                break;

        }
        else if(sqldatas->virus_list_Data[num].strategy_type[0]==WHITELIST)/*white名单*/
        {
            if(mime_cmp!=NULL)
            {
                if(check_sub(mime_cmp,strlen(mime_cmp),
                             sqldatas->virus_list_Data[num].strategy_info,
                             strlen(sqldatas->virus_list_Data[num].strategy_info))!=0)/*check for current regular*/
                {
                    //strategy_flags&=(~(1<<6));/*set flag and return */
                    strategy_flags&=(~(1<<2));/*清零标志位，因为已经可以判断不是垃圾了,后面不需要扫描判断了*/
                    break;/*exit for this for loop*/
                }
            }
            else
                break;
        }
    }
    return 1;
}
