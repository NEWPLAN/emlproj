#include "all.h"
#include "assist.h"
#include <stdio.h>
#include "mainall.h"
#include <assert.h>

static int matchGateway(mimePtr email,FetchRtePtr sqlval)
{
    assert(email!=NULL);
    char* sender=email->sender, *receiver=email->receiver,*srcip=email->srcIP, *destip=email->destIP;
    if((sender==NULL && receiver ==NULL && srcip==NULL && destip==NULL) || sqlval==NULL)
        return 0;
    int index=0;
    for(index=0; index<sqlval->row; index++)
    {
        if((sqlval->dataPtr)[index][3]!=NULL)/*field*/
        {
            if(strcmp((sqlval->dataPtr)[index][3],"srcIP")==0)/* ip*/
            {
                if((sqlval->dataPtr)[index][4]!=NULL && srcip)
                    if(strcmp((sqlval->dataPtr)[index][4],srcip)==0)
                        return 1;
            }
            else if(strcmp((sqlval->dataPtr)[index][3],"destIP")==0)
            {
                if((sqlval->dataPtr)[index][4]!=NULL && destip)
                    if(strcmp((sqlval->dataPtr)[index][4],destip)==0)
                        return 1;
            }
            else if(strcmp((sqlval->dataPtr)[index][3],"receiver")==0)
            {
                if((sqlval->dataPtr)[index][4]!=NULL && receiver)
                    if(strcmp((sqlval->dataPtr)[index][4],receiver)==0)
                        return 1;
            }
            else if(strcmp((sqlval->dataPtr)[index][3],"sender")==0)/*sender or ewcwicewr*/
            {
                if((sqlval->dataPtr)[index][4]!=NULL && sender)
                    if(strcmp((sqlval->dataPtr)[index][4],sender)==0)
                        return 1;
            }
        }
    }
    return 0;
}

#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>

int virusEngineCheck(char* filepath)
{
    char abspath[1024]={0};
    char appendixapth[1024]={0};
    DIR* d;
    struct dirent *file;
    assert(filepath!=NULL);

    sprintf(appendixapth,"%s/%s",filepath,"appendix");    
    if(!(d=opendir(appendixapth)))
    {
        printf("error in open dir : %s\n",filepath);
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
                continue;
        }
        
        memset(abspath,0,sizeof(filepath));
        sprintf(abspath,"%s/%s",appendixapth,file->d_name);
        if(antivirus(abspath))
            return 1;
    }
    closedir(d);
    return 0;
}

    CheckType virusCheck(mimePtr email,char* owner,int direction)
    {
        FetchRtePtr user_blist=NULL, user_wlist=NULL;
        if(checkInGateway(owner))/*owner not in netgate:*/
            //return NONE;/*for test use*/
            ;
        //#1.user级处理
        if (direction == 1)// #发出的邮件
        {
            //#不处理或只进行黑名单处理
            //#下面至今进行黑名单处理
            char command[1024]= {0};
            sprintf(command, "SELECT * FROM strategy_viruslist WHERE owner = '%s' AND level = 0 AND type = 0 AND direction = %d",owner,direction);
            user_blist = sql_query(command);
        }
        else//:#收到的邮件
        {
            char command1[1024]= {0},command2[1024]= {0};
            sprintf(command1, "SELECT * FROM strategy_viruslist WHERE owner = '%s' AND level = 0 AND type = 0 AND direction = %d",owner,direction);
            sprintf(command2, "SELECT * FROM strategy_viruslist WHERE owner = '%s' AND level = 0 AND type = 1 AND direction = %d",owner,direction);
            user_blist = sql_query(command1);
            user_wlist = sql_query(command2);
        }

        if(matchGateway(email,user_blist))/*email match user_blist:*/
            return CONFIRMED;
        if(matchGateway(email,user_wlist))/*email match user_wlist:*/
            return OK;

        //#2.domain级处理
        char* domain = getDomain(owner);
        char command_b[1024]= {0};
        char command_w[1024]= {0};
        sprintf(command_b,"SELECT * FROM strategy_viruslist WHERE owner = '%s' AND level = 1 AND type = 0 AND direction = %d",domain,direction);
        sprintf(command_w,"SELECT * FROM strategy_viruslist WHERE owner = '%s' AND level = 2 AND type = 1 AND direction = %d",domain,direction);
        FetchRtePtr domain_blist=sql_query(command_b);
        FetchRtePtr domain_wlist=sql_query(command_w);

        if(matchGateway(email,domain_blist))/*email match domain_blist:*/
            return CONFIRMED;
        if(matchGateway(email,domain_wlist))/*email match domain_wlist:*/
            return OK;

        //#3.网关级处理
        memset(command_b,0,sizeof(command_b));
        memset(command_w,0,sizeof(command_w));
        sprintf(command_b,"SELECT * FROM strategy_viruslist WHERE owner = '%s' AND level = 2 AND type = 0 AND direction = %d","GLOBAL",direction);
        sprintf(command_w,"SELECT * FROM strategy_viruslist WHERE owner = '%s' AND level = 2 AND type = 1 AND direction = %d","GLOBAL",direction);
        FetchRtePtr global_blist=sql_query(command_b);
        FetchRtePtr global_wlist=sql_query(command_w);

        if(matchGateway(email,global_blist))/*email match global_blist:*/
            return CONFIRMED;
        if(matchGateway(email,global_wlist))/*email match global_wlist:*/
            return OK;

        //#4.垃圾引擎处理
        if(virusEngineCheck(email->workspace))
            return CONFIRMED;
        return OK;
    }
