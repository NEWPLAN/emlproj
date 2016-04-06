#include "all.h"
#include "assist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dboperate.h"

int match_email(char* email,FetchRtePtr keyworlds)
{
	return 0;
}

CheckType keywordCheck(char* email,char* owner,int direction)
{
	int index=0,index2;
  	if(checkInGateway(owner))
  		return NONE;
  	char command[1024];

  	//#1.user级处理
  	memset(command, 0, sizeof(command));
  	sprintf(command,"SELECT id FROM KeywordList WHERE owner= %s AND level ==0 AND direction == %d",owner,direction);
    FetchRtePtr user_listids=sql_query(command);
    for(index=0; index<user_listids->row;index++)
    {
    	char command2[1024]={0};
    	sprintf(command2,"SELECT keyword FROM keywords WHERE list_id==%s",user_listids->dataPtr[index][0]);
    	FetchRtePtr keywords=sql_query(command2);
    	for(index2=0;index2<keywords->row;index2++)
    	{
    		if(match_email(email,keywords))
    			return CONFIRMED;
    	}
    }

    //#2.domain级处理
    char *domain = getDomain(owner);// #domain 在网关下

    memset(command, 0, sizeof(command));
  	sprintf(command,"SELECT id FROM KeywordList WHERE owner= %s AND level == 1 AND direction == %d",domain,direction);
  	FetchRtePtr domain_listids=sql_query(command);
    for(index=0; index<domain_listids->row;index++)
    {
    	char command2[1024]={0};
    	sprintf(command2,"SELECT keyword FROM keywords WHERE list_id==%s",domain_listids->dataPtr[index][0]);
    	FetchRtePtr keywords=sql_query(command2);
    	for(index2=0;index2<keywords->row;index2++)
    	{
    		if(match_email(email,keywords));
    		return CONFIRMED;
    	}
    }

    //#3.网关级处理
    memset(command, 0, sizeof(command));
  	sprintf(command,"SELECT id FROM KeywordList WHERE owner= %s AND level == 2 AND direction == %d","GLOBAL",direction);
  	FetchRtePtr global_listids=sql_query(command);
    for(index=0; index<global_listids->row;index++)
    {
    	//list_id=global_listids[index];
    	char command2[1024]={0};
    	sprintf(command2,"SELECT keyword FROM keywords WHERE list_id==%s",global_listids->dataPtr[index][0]);
    	FetchRtePtr keywords=sql_query(command2);
    	for(index2=0;index2<keywords->row;index2++)
    	{
    		if(match_email(email,keywords));
    		return CONFIRMED;
    	}
    }
    return OK;
}
