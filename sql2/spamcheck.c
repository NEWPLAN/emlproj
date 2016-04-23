#include "all.h"
#include <stdio.h>
#include "assist.h"
#include "mainall.h"
#include <assert.h>

static int matchGateway(mimePtr email,FetchRtePtr *sqlvalPtr)
{
    assert(email!=NULL);
    FetchRtePtr sqlval=*sqlvalPtr;
    char* sender=email->sender, *receiver=email->receiver,*srcip=email->srcIP, *destip=email->destIP;
    if((sender==NULL && receiver ==NULL && srcip==NULL && destip==NULL) || sqlval==NULL)
        return 0;
    int index=0,rte=0;
    for(index=0; index<sqlval->row; index++)
    {
        if((sqlval->dataPtr)[index][3]!=NULL)/*field*/
        {
            if(strcmp((sqlval->dataPtr)[index][3],"srcIP")==0)/* ip*/
            {
                if((sqlval->dataPtr)[index][4]!=NULL && srcip)
		            if(strcmp((sqlval->dataPtr)[index][4],srcip)==0)
		            {
		             	rte=1;
		             	break;
		            }
            }
            else if(strcmp((sqlval->dataPtr)[index][3],"destIP")==0)
            {
                if((sqlval->dataPtr)[index][4]!=NULL && destip)
                    if(strcmp((sqlval->dataPtr)[index][4],destip)==0)
                    {
		             	rte=1;
		             	break;
		            }
            }
            else if(strcmp((sqlval->dataPtr)[index][3],"receiver")==0)
            {
                if((sqlval->dataPtr)[index][4]!=NULL && receiver)
                    if(strcmp((sqlval->dataPtr)[index][4],receiver)==0)
                    {
		             	rte=1;
		             	break;
		            }
            }
            else if(strcmp((sqlval->dataPtr)[index][3],"sender")==0)/*sender or ewcwicewr*/
            {
                if((sqlval->dataPtr)[index][4]!=NULL && sender)
                    if(strcmp((sqlval->dataPtr)[index][4],sender)==0)
                    {
		             	rte=1;
		             	break;
		            }
            }
        }
    }
    free_memory(sqlval);
    *sqlvalPtr=NULL;
    return rte;
}

int spamEngineCheck(char* email)
{
    struct mes RteVal=detectSpam(email);
    return RteVal.isSpam;
}

CheckType spamCheck(mimePtr email,char* owner,int direction)
{
    FetchRtePtr user_blist=NULL, user_wlist=NULL;
    if(!checkInGateway(owner))/*owner not in netgate:*/
        return NONE;
    //#1.user级处理
    if (direction == 1)// #发出的邮件
    {
        //#不处理或只进行黑名单处理
        //#下面至今进行黑名单处理
        char command[1024]= {0};
        sprintf(command, "SELECT * FROM strategy_spamlist WHERE owner = '%s' AND level = 0 AND type = 0 AND direction = %d",owner,direction);
        user_blist = sql_query(command);
    }
    else//:#收到的邮件
    {
        char command1[1024]= {0},command2[1024]= {0};
        sprintf(command1, "SELECT * FROM strategy_spamlist WHERE owner = '%s' AND level = 0 AND type = 0 AND direction = %d",owner,direction);
        sprintf(command2, "SELECT * FROM strategy_spamlist WHERE owner = '%s' AND level = 0 AND type = 1 AND direction = %d",owner,direction);
        user_blist = sql_query(command1);
        user_wlist = sql_query(command2);
    }


    if(matchGateway(email,&user_blist))/*email match user_blist:*/
        return CONFIRMED;
    if(matchGateway(email,&user_wlist))/*email match user_wlist:*/
        return OK;

    //#2.domain级处理
    char* domain = getDomain(owner);
    char command_b[1024]= {0};
    char command_w[1024]= {0};
    sprintf(command_b," SELECT * FROM strategy_spamlist WHERE owner = '%s' AND level = 1 AND type = 0 AND direction = %d",domain,direction);
    sprintf(command_w," SELECT * FROM strategy_spamlist WHERE owner = '%s' AND level = 1 AND type = 1 AND direction = %d",domain,direction);
    FetchRtePtr domain_blist=sql_query(command_b);
    FetchRtePtr domain_wlist=sql_query(command_w);

    if(matchGateway(email,&domain_blist))/*email match domain_blist:*/
        return CONFIRMED;
    if(matchGateway(email,&domain_wlist))/*email match domain_wlist:*/
        return OK;

    //#3.网关级处理
    memset(command_b,0,sizeof(command_b));
    memset(command_w,0,sizeof(command_w));
    sprintf(command_b," SELECT * FROM strategy_spamlist WHERE owner = '%s' AND level = 2 AND type = 0 AND direction = %d","GLOBAL",direction);
    sprintf(command_w," SELECT * FROM strategy_spamlist WHERE owner = '%s' AND level = 2 AND type = 1 AND direction = %d","GLOBAL",direction);
    FetchRtePtr global_blist=sql_query(command_b);
    FetchRtePtr global_wlist=sql_query(command_w);

    if(matchGateway(email,&global_blist))/*email match global_blist:*/
        return CONFIRMED;
    if(matchGateway(email,&global_wlist))/*email match global_wlist:*/
        return OK;

    //#4.垃圾引擎处理
    if(spamEngineCheck(email->filepath))/*确定是否是垃圾*/
        return CONFIRMED;
    return OK;
}
