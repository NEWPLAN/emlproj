#include "all.h"
#include "assist.h"
#include <stdio.h>


int virusEngineCheck(char* virus)
{
	return 0;
}

CheckType virusCheck(char* email,char* owner,int direction)
{
	FetchRtePtr user_blist=NULL, user_wlist=NULL;
    if(checkInGateway(owner))/*owner not in netgate:*/
        return NONE;

    //#1.user级处理
    if (direction == 1)// #发出的邮件
    {
        //#不处理或只进行黑名单处理
        //#下面至今进行黑名单处理
        char command[1024]={0};
        sprintf(command, "SELECT * FROM VirusList WHERE owner == %s AND level == 0 AND type == 0 AND direction == %d",owner,direction);
        user_blist = sql_query(command);
    }
    else//:#收到的邮件
    {
    	char command1[1024]={0},command2[1024]={0};
        sprintf(command1, "SELECT * FROM VirusList WHERE owner == %s AND level == 0 AND type == 0 AND direction == %d",owner,direction);
        sprintf(command2, "SELECT * FROM VirusList WHERE owner == %s AND level == 0 AND type == 1 AND direction == %d",owner,direction);
        user_blist = sql_query(command1);
        user_wlist = sql_query(command2);
    }


    if(0)/*email match user_blist:*/
        return CONFIRMED;
    if(0)/*email match user_wlist:*/
        return OK;

    //#2.domain级处理
    char* domain = getDomain(owner);
    char command_b[1024]={0};
    char command_w[1024]={0};
    sprintf(command_b,"SELECT * FROM VirusList WHERE owner == %s AND level == 1 AND type == 0 AND direction == %d",domain,direction);
    sprintf(command_w,"SELECT * FROM VirusList WHERE owner == %s AND level == 2 AND type == 1 AND direction == %d",domain,direction);
    FetchRtePtr domain_blist=sql_query(command_b);
    FetchRtePtr domain_wlist=sql_query(command_w);

    if(0)/*email match domain_blist:*/
        return CONFIRMED;
    if(0)/*email match domain_wlist:*/
        return OK;

    //#3.网关级处理
    memset(command_b,0,sizeof(command_b));
    memset(command_w,0,sizeof(command_w));
    sprintf(command_b,"SELECT * FROM VirusList WHERE owner == %s AND level == 2 AND type == 0 AND direction == %d","GLOBAL",direction);
    sprintf(command_w,"SELECT * FROM VirusList WHERE owner == %s AND level == 2 AND type == 1 AND direction == %d","GLOBAL",direction);
   	FetchRtePtr global_blist=sql_query(command_b);
    FetchRtePtr global_wlist=sql_query(command_w);

    if(0)/*email match global_blist:*/
        return CONFIRMED;
    if(0)/*email match global_wlist:*/
        return OK;

    //#4.垃圾引擎处理
    if(virusEngineCheck(email))
        return CONFIRMED;
    return OK;
}
