/*************************************************************************
	> File Name: assist.c
	> Author: 
	> Mail: 
	> Created Time: 2016年04月06日 星期三 10时19分01秒
 ************************************************************************/

#include<stdio.h>
#include "assist.h"
#include "dboperate.h"
static int rowNum=0;

int checkInGateway(char* owner)
{
    return 1;
}

FetchRtePtr sql_query(char* command)
{
	FetchRtePtr val=NULL;
	if((val=database_query_withCommand(command))==NULL)
    {
        printf("error in loading table spamlist \n");
        return NULL;
    }
    rowNum=val->row;
    free_memory(val);
    val=NULL;
    return val;
}

char* getDomain(char* owner)
{
	char * ptr=strchr(owner,'@');
    if(ptr)/*查到就不会为空*/
        return ++ptr;
    return NULL;
}


