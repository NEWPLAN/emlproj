#include "all.h"

CheckType spamCheck(char* email,char* owner,int direction)
{
	char ***user_blist=NULL, ***user_wlist=NULL;
    if(/*owner not in netgate:*/)
        return NONE;
        
    //#1.user级处理
    if (direction == 1)// #发出的邮件
    {
        //#不处理或只进行黑名单处理
        //#下面至今进行黑名单处理
        char command[1024]={0};
        sprintf(command, "SELECT * FROM SpamList WHERE owner == %s AND level == 0 AND type == 0 AND direction == %d",owner,direction);
        user_blist = sql_query(command);
    }
    else//:#收到的邮件
    {
    	char command1[1024]={0},command2[1024]={0};
        sprintf(command1, "SELECT * FROM SpamList WHERE owner == %s AND level == 0 AND type == 0 AND direction == %d",owner,direction);
        sprintf(command2, "SELECT * FROM SpamList WHERE owner == %s AND level == 0 AND type == 1 AND direction == %d",owner,direction);
        user_blist = sql_query(command1);
        user_wlist = sql_query(command2);
    }


    if(/*email match user_blist:*/)
        return CONFIRMED;
    if(/*email match user_wlist:*/)
        return OK;
    
    //#2.domain级处理
    char* domain = getDomain(owner);
    char command_b[1024]={0};
    char command_W[1024]={0};
    sprintf(command_b=" SELECT * FROM SpamList WHERE owner == %s AND level == 1 AND type == 0 AND direction == %d",domain,direction);
    sprintf(command_w=" SELECT * FROM SpamList WHERE owner == %s AND level == 1 AND type == 1 AND direction == %d",domain,direction);
    domain_blist=sql_query(command_b);
    domain_wlist=sql_query(command_w);

    if(/*email match domain_blist:*/)
        return CONFIRMED;
    if(/*email match domain_wlist:*/)
        return OK;
    
    //#3.网关级处理
    memset(command_b,0,sizeof(command_b));
    memset(command_w,0,sizeof(command_w));
    sprintf(command_b=" SELECT * FROM SpamList WHERE owner == %s AND level == 2 AND type == 0 AND direction == %d",domain,direction);
    sprintf(command_w=" SELECT * FROM SpamList WHERE owner == %s AND level == 2 AND type == 1 AND direction == %d",domain,direction);
   	global_blist=sql_query(command_b);
    global_blist=sql_query(command_w);
    
    if(/*email match global_blist:*/)
        return CONFIRMED;
    if(/*email match global_wlist:*/)
        return OK;
        
    //#4.垃圾引擎处理
    if(spamEngineCheck(email)=="Spam")
        return CONFIRMED;
    return OK;
}