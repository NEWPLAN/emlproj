/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月30日 星期三 14时20分11秒
 ************************************************************************/

#include<stdio.h>
#include"all.h"
#include "dboperate.h"
#include "assist.h"

extern CheckType spamCheck(char* email, char* owner, int direction);
extern CheckType virusCheck(char* email, char* owner, int direction);
extern CheckType keywordCheck(char* email,char* owner,int direction);
extern CheckType keywordClassCheck(char* email,char* owner,int direction);
extern CheckType urlCheck(char* email,char* owner,int direction);


static strategyType combine_strategy(strategyType strategy_A, strategyType strategy_B)
{
	if(strategy_A==BLOCK||strategy_B==BLOCK)
		return BLOCK;
	if(strategy_A==TRASH||strategy_B==TRASH)
		return TRASH;
	return strategy_A;
}


static void notify_sender(char* sender,char* notify_info)//通知发件人
{
	return;
}

static strategyType get_valid_strategy(char* owner,char* field,int direction)
{
	char command1[1024]={0},command2[1024]={0},command3[1024]={0};
	FetchRtePtr rteval=NULL;
	char*domain=NULL;
	sprintf(command1,"SELCET %s in owner='%s' and level=0 and direction=%d",field ,owner, direction);
	if((rteval=sql_query(command1))!=NULL)/*user_strategy*/
		return IGNORE/**rteval*/;
	domain = getDomain(owner);
	sprintf(command2,"SELCET %s in owner='%s' and level=1 and direction=%d",field ,domain, direction);
	if((rteval=sql_query(command2))!=NULL)/*domain_strategy*/
		return IGNORE/**rteval*/;
	sprintf(command3,"SELCET %s in owner='%s' and level=2 and direction=%d",field ,owner, direction);
	if((rteval=sql_query(command3))!=NULL)/*global_strategy*/
		return IGNORE/**rteval*/;
	return IGNORE;
}

static void overall_check_single_side(char* email,char* owner,int direction, strategyType *final_strategy,char* notify_info)/*:#final_strategy和notify_info传引用*/
{
	CheckType spam_result,virus_result,keyword_result,keywordclass_result,url_result;
    //#1.垃圾检测
    spam_result = spamCheck(email,owner,direction);    
    //#2.病毒检测
    virus_result = virusCheck(email,owner,direction);
    //#3.关键字检测
    keyword_result = keywordCheck(email,owner,direction);
    //#4.关键字类检测
    keywordclass_result = keywordClassCheck(email,owner,direction);
    //#5.url检测
    url_result = urlCheck(email,owner,direction);
    
    memset(notify_info,0,strlen(notify_info));
        
    if(spam_result==CONFIRMED)
    {
    	strcat(notify_info,"垃圾邮件\t");// #类似的信息
    	strategyType spam_strategy = get_valid_strategy("spam", owner, direction);
    	*final_strategy=combine_strategy(*final_strategy, spam_strategy);
    }
    
    if(virus_result==CONFIRMED)
    {
    	strcat(notify_info,"您的邮件中含有病毒\t");// #类似的信息
    	strategyType virus_strategy = get_valid_strategy("virus", owner, direction);
    	*final_strategy=combine_strategy(*final_strategy, virus_strategy); 	
    }
    
    if(keyword_result==CONFIRMED)
    {
    	strcat(notify_info,"您的邮件中含有关键字\t");// #类似的信息
    	strategyType keyword_strategy = get_valid_strategy("keyword", owner, direction);
    	*final_strategy=combine_strategy(*final_strategy, keyword_strategy); 	    	
    }
    
    if(keywordclass_result==CONFIRMED)
    {
    	strcat(notify_info,"您的邮件中含有关键字类\t");// #类似的信息
    	strategyType keywordclass_strategy = get_valid_strategy("keywordClass", owner, direction);
    	*final_strategy=combine_strategy(*final_strategy, keywordclass_strategy);  	
    }
    
    if(url_result==CONFIRMED)
    {
    	strcat(notify_info,"您的邮件中含有URL\t");// #类似的信息
    	strategyType url_strategy = get_valid_strategy("url", owner, direction);
    	*final_strategy=combine_strategy(*final_strategy, url_strategy);   	
    }
    return ;
    
}

static char* getSender(char* email)
{
	return NULL;
}

static char* getReceiver(char* email)
{
	return NULL;
}

static int overall_check(char* email)
{  
    //#发送端检测
    char* sender = getSender(email);
    strategyType sender_final_strategy = IGNORE;// #默认放行
    char notify_info[1024]={0};
        
    if(checkInGateway(sender))/*sender in netgate*/
    {
    	memset(notify_info, 0, sizeof(notify_info));
    	overall_check_single_side(email,sender,1,&sender_final_strategy,notify_info);
    	if(sender_final_strategy!=IGNORE)
    		notify_sender(sender,notify_info);//通知发件人
    }
    
        
    char* receiver = getReceiver(email);
    strategyType receiver_final_strategy =IGNORE;    
    
    if((receiver_final_strategy!=BLOCK) && checkInGateway(receiver))/*final_strategy != 'block' and receiver in netgate*///: #邮件能从发送端网关发出
        //#接收端检测
    { 
        memset(notify_info, 0, sizeof(notify_info));
        
        overall_check_single_side(email,receiver,0,&receiver_final_strategy,notify_info);
        
        if(receiver_final_strategy ==BLOCK)
            return 1;//#堵截
        else if (receiver_final_strategy == TRASH)
            return 2;//#发送到垃圾箱
        else if (receiver_final_strategy == IGNORE)
            return 0;//#直接发送
	}
	return 0;/*??*/
}

#include <stdio.h>
#include "all.h"
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/time.h>
#include "mainall.h"

static int strategy_flags=0x0f;
static DataPtr sqldatas=NULL;
static GmimeDataPtr mimedata=NULL;
static int setRegular(void);
static char workspace[1024]={0};
/*黑白名单*/
#define WHITELIST '1'
#define BLACKLIST '0'

/*针对源地址和目的地址*/
#define TERMINAL  '1'
#define SOURCE    '0'



int ParseEML(char* filename,GmimeDataPtr* rtevalPtr)
{
	char oldpath[1024]={0};
    char* inputs[3]= {NULL,filename};
    void *handle;
    GmimeDataPtr A=NULL;
    GmimeDataPtr (*dlfunc)(int argc,char* argv[]);
    struct timeval tBeginTime, tEndTime;
    float fCostTime;
    
    strcat(oldpath,workspace);// add new workspace
    inputs[2]=oldpath;
  
  
    gettimeofday(&tBeginTime,NULL);
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
    
    char workpath[1024]={0};

    sprintf(workpath,"%s/%s",workspace,filename);

    char * inputs[3]= {NULL,workpath,&Ate};
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
    char oldpath[1024]={0};
    char Ate=(char)((strategy_flags>>8)&0xff);
    char *inputs[3]= {NULL,filename,&Ate};
    struct timeval tBeginTime, tEndTime;
    float fCostTime;

	sprintf(oldpath,"%s/%s",workspace,filename);
	inputs[1]=oldpath;
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
    char oldpath[1024]={0};

	sprintf(oldpath,"%s/%s",workspace,filedirname);
	ins[1]=oldpath;
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

static int AllInits(void)
{
	if(SpliterInit())
		printf("init spliter successfully\n");
	else
	{
		printf("init spliter failed");
		return 0;
	}
	if(AntiVirusInit())
		printf("init AntiVirus successfully\n");
	else
	{
		printf("init AntiVirus failed");
		return 0;
	}
    return 1;
}

static int AllFree(void)
{
	SpliterExit();
}
static int ParseAEmail(char*filepath,char*workpath)
{
	char errorinfo[1024]= {0};/*错误处理*/
	assert(filepath!=NULL && workpath!=NULL);
	strcat(workspace,workpath);
	
GMIMEPARSE:/*邮件解析*/
	if(ParseEML(filepath,&mimedata)==0)
	{
		strcat(errorinfo,"ParseEML");
        goto  exit;
	}
ANITSPAM:/*反病毒*/
	if(AntiSpams(filepath)==0)
	{
		strcat(errorinfo,"Antispams");
        goto  exit;
	}
PARSEAPPENDIX:/*分析附件*/
	if(ParseAppendix("appendix")==0)
	{
		strcat(errorinfo,"Appendix");
        goto  exit;
	}
PARSEKEYCLASS:/*分析关键字类*/
	if(ParseKeyClass("temps")==0)
	{
		strcat(errorinfo,"ParseKeyClass");
        goto  exit;
	}
PARSEKEYWORDS:/*分析关键字*/
	if(ParseKeyChs("temps")==0)
	{
		strcat(errorinfo,"ParseKeyWords");
        goto  exit;
	}
	return 0;
exit:/*退出，结束*/
    //cleanAll();
    printf("error in module %s\n",errorinfo);
    return 0;
}


#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	
	static int flags=0;
	int rte = 1;
	AllInits();
	while(rte--)
	{
		if(fork()==0)
		{
			char runningFiles[1024]={0};
			char newpath_temps[1024]={0};
			char newpath_appendix[1024]={0};
			char command[1024]={0};
			sprintf(runningFiles,"runningFiles_%d",getpid());
			sprintf(newpath_temps,"%s/temps",runningFiles);
			sprintf(newpath_appendix,"%s/appendix",runningFiles);
			if(mkdir(runningFiles, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)==0)/*success*/
			{
				printf("%s\n",runningFiles);
			}
			if(mkdir(newpath_temps, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)==0)/*success*/
			{
				printf("%s\n",newpath_temps);
			}
			if(mkdir(newpath_appendix, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)==0)/*success*/
			{
				printf("%s\n",newpath_appendix);
			}
			//return 0;
			//chdir(runningFiles);
		 	printf("%d\t%d\n",getpid(),ParseAEmail(argv[1],runningFiles));
		 	
		 	if(remove(runningFiles)==0)
		 		printf("remove done\n");
		 	else
		 		printf("can't delete this fold\n");		 	
		 	sprintf(command,"rm -rf %s",runningFiles);
		system(command);
		 //	if(fork()==0)
		 //		execlp("rm","rm","-rf",runningFiles,NULL);
		 	return 0;
		}
	}
	wait(NULL);
	AllFree();
	printf("done\n");
	return 0;
}
