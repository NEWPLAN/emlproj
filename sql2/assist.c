/*************************************************************************
	> File Name: assist.c
	> Author:
	> Mail:
	> Created Time: 2016年04月06日 星期三 10时19分01秒
 ************************************************************************/

#include <stdio.h>
#include "assist.h"
#include "dboperate.h"
#include "kmp.h"

int checkInGateway(char* owner)
{
    int index=0;
    if(owner==NULL)
        return 0;
    char* domain=getDomain(owner);
    if(domain==NULL)
        return 0;
    //char command[1024]={0};
    FetchRtePtr rteval=database_query("strategy_netgatedomain");
    if(rteval==NULL)
        return 0;
    for(index=0; index<rteval->row; index++)
    {
        if((rteval->dataPtr)[index][1]!=NULL)
            if(check_sub(domain, strlen(domain), (rteval->dataPtr)[index][1], strlen((rteval->dataPtr)[index][1]))!=0)/*is the subs*/
                return 1;
    }
    if(rteval)
    	free_memory(rteval);
  	rteval=NULL;
    return 0;
}

FetchRtePtr sql_query(char* command)
{
    FetchRtePtr val=NULL;
    if((val=database_query_withCommand(command))==NULL)
    {
        printf("error in loading table spamlist \n");
        return NULL;
    }
    return val;
}

char* getDomain(char* owner)
{
    char * ptr=strchr(owner,'@');
    if(ptr)/*查到就不会为空*/
        return ++ptr;
    return NULL;
}


