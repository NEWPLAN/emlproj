#include "all.h"
#include "assist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int matchUrl(mimePtr email,FetchRtePtr* vals)
{
	FetchRtePtr urlitems=NULL,sqlval=*vals;
	char command[1024]={0};
	if(sqlval==NULL||sqlval->row==0)
		return 0;
	sprintf(command,"SELECT URL FROM strategy_urls WHERE list_id = %s",sqlval->dataPtr[0][0]);
	urlitems=sql_query(command);
	if(urlitems==NULL||urlitems->row==0)
		return 0;
	{
		/*add pasring url module here and return values*/
		
	}
	free_memory(urlitems);
	free_memory(sqlval);
	*vals=NULL;
	return 0;

}

CheckType urlCheck(mimePtr email, char* owner, int direction)
{
	FetchRtePtr listids=NULL;
	char command1[1024]= {0};
	char *domain=NULL;
    if(!checkInGateway(owner))/*owner not in netgate:*/
        return NONE;
    
    //1.user级处理
    memset(command1,0,sizeof(command1));
    sprintf(command1, "SELECT id FROM strategy_urllist WHERE owner = '%s' AND level = 0 AND direction = %d",owner,direction);
    listids = sql_query(command1);
    if(listids!=NULL)
		if(matchUrl(email,&listids))
			return CONFIRMED;
        
    //2.domain级处理
    domain = getDomain(owner); //domain 在网关下
    memset(command1,0,sizeof(command1));
    sprintf(command1, "SELECT id FROM strategy_urllist WHERE owner = '%s' AND level = 1 AND direction = %d",domain,direction);
    listids = sql_query(command1);
    if(listids!=NULL)
		if(matchUrl(email,&listids))
			return CONFIRMED;

    //3.网关级处理
    memset(command1,0,sizeof(command1));
    sprintf(command1, "SELECT id FROM strategy_urllist WHERE owner = '%s' AND level = 2 AND direction = %d","GLOBAL",direction);
    listids = sql_query(command1);
    if(listids!=NULL)
		if(matchUrl(email,&listids))
			return CONFIRMED;

    return OK;
}
