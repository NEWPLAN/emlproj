/*************************************************************************
	> File Name: protocol.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月15日 星期二 16时04分53秒
 ************************************************************************/

#include<stdio.h>
#include "protocol.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="Protocol_switch";
static int rowNum=0;

protocolPtr load_protocolSwitch(void* A, void* B)
{
    
    FetchRtePtr val=NULL;
    protocolPtr prolist=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table protocol switch\n");
        return NULL;
    }
    rowNum=val->row;
    prolist=(protocolPtr)malloc(sizeof(protocol)*(val->row+1));
    if(!prolist)
    {
        printf("error in malloc for protocol switch\n");
        return NULL;
    }
    memset(prolist,0,sizeof(protocol)*(val->row+1));
    for(index=0;index<val->row;index++)
    {
    	memcpy(prolist[index].protocol,val->dataPtr[index][0],sizeof(prolist[index].protocol)-1);
    	memcpy(prolist[index].direction,val->dataPtr[index][1],sizeof(prolist[index].direction)-1);
    	memcpy(prolist[index].switchs,val->dataPtr[index][2],sizeof(prolist[index].switchs)-1);
    }
    return prolist;
}

void printProtocolSwitch(void)
{
    protocolPtr var=load_protocolSwitch(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in printProtocolSwitch\n");
    while(rowindex<totalnum)
    {
    	printf("%s\t%s\t%s\n",var[rowindex].protocol,var[rowindex].direction,var[rowindex].switchs);
    	rowindex++;
    }
}
