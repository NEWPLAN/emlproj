#include "all.h"


CheckType keywordCheck(char* email,char* owner,int direction)
{
	int index=0,index2;
  	if(/*owner in netagte*/)
  		return NONE;
  	char command[1024];

  	//#1.user级处理
  	memset(command, 0, sizeof(command));
  	sprintf(command,"SELECT id FROM KeywordList WHERE owner= %s AND level ==0 AND direction == %d",owner,direction);
    //user_listids = SELECT id FROM KeywordList WHERE owner == owner AND level == 0 AND direction == direction
    char*** user_listids=sql_query(command);
    for(index=0; index<user_listids.len;index++)
    {
    	char command2[1024]={0};
    	sprintf(command2,"SELECT keyword FROM keywords WHERE list_id==%d",list_id);
    	char*** keywords=sql_query(command2);
    	for(index2=0;index2<keywords.len;index2++)
    	{
    		if(match_email(emial,keywords));
    		return CONFIRMED;
    	}
    }


        
    //#2.domain级处理
    char *domain = getDomain(owner);// #domain 在网关下

    memset(command, 0, sizeof(command));
  	sprintf(command,"SELECT id FROM KeywordList WHERE owner= %s AND level == 1 AND direction == %d",domain,direction);    
    //domain_listids = SELECT id FROM KeywordList WHERE owner == domain AND level == 1 AND direction == direction

  	char*** domain_listids=sql_query(command);
    for(index=0; index<domain_listids.len;index++)
    {
    	char command2[1024]={0};
    	sprintf(command2,"SELECT keyword FROM keywords WHERE list_id==%d",list_id);
    	char*** keywords=sql_query(command2);
    	for(index2=0;index2<keywords.len;index2++)
    	{
    		if(match_email(emial,keywords));
    		return CONFIRMED;
    	}
    }
    
    //#3.网关级处理
    memset(command, 0, sizeof(command));
  	sprintf(command,"SELECT id FROM KeywordList WHERE owner= %s AND level == 2 AND direction == %d","GLOBAL",direction);
  	char*** global_listids=sql_query(command);
    for(index=0; index<global_listids.len;index++)
    {
    	//list_id=global_listids[index];
    	char command2[1024]={0};
    	sprintf(command2,"SELECT keyword FROM keywords WHERE list_id==%d",list_id);
    	char*** keywords=sql_query(command2);
    	for(index2=0;index2<keywords.len;index2++)
    	{
    		if(match_email(emial,keywords));
    		return CONFIRMED;
    	}
    }        
    return OK;
}