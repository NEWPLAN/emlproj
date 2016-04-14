/*************************************************************************
	> File Name: main.c
	> Author:
	> Mail:
	> Created Time: 2016年03月30日 星期三 14时20分11秒
 ************************************************************************/
#include <stdio.h>
#include "all.h"
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/time.h>
#include "mainall.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "assist.h"
#include "moduleswitch.h"
#include "../api/emailhead.h"

//static GmimeDataPtr mimedata=NULL;

static char workspace[1024]= {0};
/*黑白名单*/
#define WHITELIST '1'
#define BLACKLIST '0'

/*针对源地址和目的地址*/
#define TERMINAL  '1'
#define SOURCE    '0'

#include "dboperate.h"

mimePtr mimeCy=NULL;

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
//	printf("the %s has sended an illegal email, and detail info is %s\n",sender,notify_info);
    return;
}

static strategyType get_valid_strategy(char* field,char* owner,int direction)
{
    char command1[1024]= {0},command2[1024]= {0},command3[1024]= {0};
    FetchRtePtr rteval=NULL;
    char*domain=NULL;
    sprintf(command1,"SELECT %s FROM strategy_strategy WHERE owner='%s' AND level=0 AND direction=%d",field,owner, direction);
    if((rteval=sql_query(command1))!=NULL)/*user_strategy*/
    {
        if(rteval->row)
            goto  RTE;
    }
    domain = getDomain(owner);
    sprintf(command2,"SELECT %s FROM strategy_strategy WHERE owner='%s' AND level=1 AND direction=%d",field,domain, direction);
    if((rteval=sql_query(command2))!=NULL)/*domain_strategy*/
    {
        if(rteval->row)
            goto  RTE;
    }
    sprintf(command3,"SELECT %s FROM strategy_strategy WHERE owner='%s' AND level=2 AND direction=%d",field,"GLOBAL", direction);
    if((rteval=sql_query(command3))!=NULL)/*global_strategy*/
    {
        if(rteval->row)
            goto  RTE;
    }
    free_memory(rteval);
    return -1;
RTE:
    if((rteval->dataPtr)[0][0]!=NULL)
    {
        if(strcmp((rteval->dataPtr)[0][0],"IGNORE")==0)
        {
            free_memory(rteval);
            return IGNORE;
        }
        if(strcmp((rteval->dataPtr)[0][0],"TRASH")==0)
        {
            free_memory(rteval);
            return TRASH;
        }
        if(strcmp((rteval->dataPtr)[0][0],"BLOCK")==0)
        {
            free_memory(rteval);
            return BLOCK;
        }
    }
    else
        return -1;
}


static void overall_check_single_side(mimePtr email,char* owner,int direction, strategyType *final_strategy,char* notify_info)/*:#final_strategy和notify_info传引用*/
{
    CheckType spam_result=NO,virus_result=NO,keyword_result=NO,keywordclass_result=NO,url_result=NO;
    int rts=loadmodule();
    //#1.垃圾检测
    if(rts&SPAM_SWITCH)
    	spam_result = spamCheck(email,owner,direction);
    //#2.url检测
	if(rts&URL_SWITCH)
    	url_result = urlCheck(email,owner,direction);
    //#3.病毒检测
	if(rts&VIRUS_SWITCH)
    	virus_result = virusCheck(email,owner,direction);
    //#4.关键字类检测
	if(rts&KEYCLASS_SWITCH)
    	keywordclass_result = keywordClassCheck(email,owner,direction);
    //#5.关键字检测
	if(rts&KEYWORDS_SWITCH)
    	keyword_result = keywordCheck(email,owner,direction);

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
    char *sender=NULL;
    if(mimeCy->mimedata)
    {
        char *ptr=NULL;
        sender=(char*)malloc(strlen(mimeCy->mimedata->from)+12);
        memset(sender,0,strlen(mimeCy->mimedata->from)+12);
        if((ptr=strchr(mimeCy->mimedata->from,'<'))!=NULL)
            strcat(sender,++ptr);
        if((ptr=strchr(sender,'>'))!=NULL)
            *ptr=0;
    }
    mimeCy->sender=sender;
    return sender;
}

static char* getReceiver(char* email)
{
    char *sender=NULL;
    char *temps=NULL;
    if(mimeCy->mimedata)
        temps=mimeCy->mimedata->to!=NULL?mimeCy->mimedata->to:mimeCy->mimedata->replayto;
    if(temps)
    {
        char *ptr=NULL;
        sender=(char*)malloc(strlen(temps)+12);
        memset(sender,0,strlen(temps)+12);
        if((ptr=strchr(temps,'<'))!=NULL)
            strcat(sender,++ptr);
        if((ptr=strchr(sender,'>'))!=NULL)
            *ptr=0;
    }
    mimeCy->receiver=sender;
    return sender;
}

static int overall_check(mimePtr email)
{
    //#发送端检测
    char* sender = getSender(NULL);
    char* receiver = getReceiver(NULL);
    strategyType sender_final_strategy = IGNORE;// #默认放行
    char notify_info[1024]= {0};

    if(checkInGateway(sender))/*sender in netgate*/
    {
        memset(notify_info, 0, sizeof(notify_info));
        overall_check_single_side(email,sender,1,&sender_final_strategy,notify_info);
        if(sender_final_strategy!=IGNORE)
            notify_sender(sender,notify_info);//通知发件人
    }

	{/**争议的地方**/
		if(sender_final_strategy ==BLOCK)
        {
        	printf("堵截\n");
            return 1;//#堵截
        }
        else if (sender_final_strategy == TRASH)
        {
        	printf("发送到垃圾箱\n");
            return 2;//#发送到垃圾箱
        }
	}

    strategyType receiver_final_strategy =IGNORE;

    if((receiver_final_strategy!=BLOCK) && checkInGateway(receiver))/*final_strategy != 'block' and receiver in netgate*///: #邮件能从发送端网关发出
        //#接收端检测
    {
        memset(notify_info, 0, sizeof(notify_info));

        overall_check_single_side(email,receiver,0,&receiver_final_strategy,notify_info);

        if(receiver_final_strategy ==BLOCK)
        {
        	printf("堵截\n");
            return 1;//#堵截
        }
        else if (receiver_final_strategy == TRASH)
        {
        	printf("发送到垃圾箱\n");
            return 2;//#发送到垃圾箱
        }
        else if (receiver_final_strategy == IGNORE)
        {
        	printf("直接发送\n");
            return 0;//#直接发送
        }
    }
    return 0;/*??*/
}





int ParseEML(char* filename,GmimeDataPtr* rtevalPtr)
{
    char oldpath[1024]= {0};
    char* inputs[3]= {NULL,filename};
    void *handle;
    GmimeDataPtr A=NULL;
    GmimeDataPtr (*dlfunc)(int argc,char* argv[]);
    struct timeval tBeginTime, tEndTime;
    float fCostTime;

    strcat(oldpath,workspace);// add new workspace
    inputs[2]=oldpath;


    gettimeofday(&tBeginTime,NULL);
#ifdef __DEBUG
    printf("hello in ParseEML \n");
#endif
    handle=dlopen("./gmimelibs.so",RTLD_LAZY);
#ifdef __DEBUG
    printf("in open libs\n");
#endif
    dlfunc=dlsym(handle,"GmimeMain");
#ifdef __DEBUG
    printf("in geting mainenter\n");
#endif
    if(!(handle&&dlfunc))
    {
        printf("error in open dynamic libs %s\n",dlerror());
        return 0;
    }

    A=dlfunc(2,inputs);

#ifdef __DEBUG
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
    //mimeCy->mimedata=A;
    gettimeofday(&tEndTime,NULL);
    fCostTime = 1000000*(tEndTime.tv_sec-tBeginTime.tv_sec)+(tEndTime.tv_usec-tBeginTime.tv_usec);
    fCostTime /= 1000000;
#ifdef __DEBUG
    printf("\033[31m the execute time for decoding EML is = %f(Second)\n\033[0m",fCostTime);
#endif
    return 1;
}


int ParseURL(char* filename)
{
    // printf("URL测试模块\n");
    return 1;
}


int ParseAppendix(char* filedirname)
{
    char * ins[2]= {NULL,filedirname};
    int (*dlfunc)(int argc, char* argv[]);
    void *handle;
    char oldpath[1024]= {0};

    sprintf(oldpath,"%s/%s",workspace,filedirname);
    ins[1]=oldpath;
#ifdef __DEBUG
    printf("hello in ParseAppendix\n");
#endif
    handle=dlopen("./appendix.so",RTLD_LAZY);
    if (!handle)
        return 1;
#ifdef __DEBUG
    printf("in open libs\n");
#endif
    dlfunc=dlsym(handle,"AppendixMain");
    if(!(handle&&dlfunc))
    {
        printf("error in open dynamic libs %s\n",dlerror());
        return 0;
    }
    dlfunc(2,ins);
    
    return 1;
}


static int AllInits(void)
{
    mimeCy=(mimePtr)malloc(sizeof(mimeType));
    assert(mimeCy!=NULL);
    memset(mimeCy,0,sizeof(mimeType));

    if(SpliterInit())
        printf("init spliter successfully\n");
    else
    {
        printf("init spliter failed");
        return 0;
    }
    return 1;
}

static int AllFree(void)
{
    if(mimeCy->sender)
    {
    	free(mimeCy->sender);
    	mimeCy->sender=NULL;
    }
    if(mimeCy->receiver)
    {
    	free(mimeCy->receiver);
    	mimeCy->receiver=NULL;
    }
    return 0;
}
static int AllRelease(void)
{
    SpliterExit();
    return 0;
}
int ParseAEmail(char*filepath,char*workpath)
{
	int vals=0;
    char errorinfo[1024]= {0};/*错误处理*/
    assert(filepath!=NULL && workpath!=NULL);
    strcat(workspace,workpath);
	
	mimeCy->workspace=workpath;
	mimeCy->filepath=filepath;
//GMIMEPARSE:/*邮件解析*/
    if(ParseEML(filepath,&(mimeCy->mimedata))==0)
    {
        strcat(errorinfo,"ParseEML");
        goto  exit;
    }
    //PARSEAPPENDIX:/*分析附件*/
    if(ParseAppendix("appendix")==0)
    {
        strcat(errorinfo,"Appendix");
        goto  exit;
    }
    vals=overall_check(mimeCy);
exit:/*退出，结束*/
    //cleanAll();
    printf("done for parsing an single email %s in workspace %s\n",filepath,workpath);
    return vals;
}




int main(int argc, char* argv[])
{

    int rte = 1000;
    AllInits();
    while(rte--)
    {
#if 1
        	if(fork()==0)
#endif
        {
            char runningFiles[1024]= {0};
            char newpath_temps[1024]= {0};
            char newpath_appendix[1024]= {0};
            char command[1024]= {0};
            sprintf(runningFiles,"runningFiles_%d",getpid());
            sprintf(newpath_temps,"%s/temps",runningFiles);
            sprintf(newpath_appendix,"%s/appendix",runningFiles);
            if(mkdir(runningFiles, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)!=0)/*! success*/
                goto exits;
            if(mkdir(newpath_temps, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)!=0)/*! success*/
                goto exits;
            if(mkdir(newpath_appendix, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)!=0)/*! success*/
                goto exits;

            printf("%d\t%d\n",getpid(),ParseAEmail(argv[1],runningFiles));
exits:
			AllFree();
            sprintf(command,"rm -rf %s",runningFiles);
            usleep(1);
#if 1
            	if(fork()==0)
            		execlp("rm","rm","-rf",runningFiles,NULL);
            wait(NULL);
#endif
            return 0;
        }
    }
#if 1
	wait(NULL);
#endif
    AllRelease();
    printf("done\n");
    return 0;
}
