/*************************************************************************
	> File Name: protocol.c
	> Author:
	> Mail:
	> Created Time: 2016年03月16日 星期三 18时44分02秒
 ************************************************************************/

#include <stdio.h>
#include "Protocol_switch.h"
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
    for(index=0; index<val->row; index++)
    {
        memcpy(prolist[index].protocol,val->dataPtr[index][0],sizeof(prolist[index].protocol)-1);
        memcpy(prolist[index].direction,val->dataPtr[index][1],sizeof(prolist[index].direction)-1);
        memcpy(prolist[index].switchs,val->dataPtr[index][2],sizeof(prolist[index].switchs)-1);
    }
    free_memory(val);
    val=NULL;
    return prolist;
}

void printProtocolSwitch(void)
{
    protocolPtr var=load_protocolSwitch(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in printProtocolSwitch\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%s\t%12s\n",var[rowindex].protocol,var[rowindex].direction,var[rowindex].switchs);
        rowindex++;
    }
    protocolRelase(&var);
}
void protocolRelase(protocolPtr* tables)
{
    if(*tables)
        free(*tables);
    *tables=NULL;
}

/********************************************************
***flags
***-31----8-7-6-5-4-3-2-1-0
***31-8不用
***7：本地连接测试     &128 0x80
***6：远程链接测试     &64  0x40
***5：插入数据库测试   &32  0x20
***4：无条件查询数据库 &16  0x10
***3：打印查询数据结构 &8   0x08
***2：删除数据库      &4   0x04
***1：更新数据库      &2   0x02
***0：打印测试信息		&1	 0x01
********************************************************/
int ProtocolSwitchTest(int flags)
{
    char*** results=NULL/*,***tptr=NULL*/;
    FetchRtePtr retval=NULL;

	if(flags&DEBUG_DB_FLAGS)
		printf("Test for %s done!\n",tableName);

    if(flags&LOCAL_CONNECT_FLAGS)
        database_connect_local("root");

    if(flags&REMOT_CONNECT_FLAGS)
        database_connect("127.0.0.1","root");

    //int ret = database_insert("Protocol_switch","'789','789',123");

    if(flags&INSERT_DB_FLAGS)
        database_insert(tableName,"'789','789',456");
    //database_query("Protocol_switch");

    if(flags&DISPLAY_DATA_FLAGS)
        printProtocolSwitch();

    if(flags&UPDATE_DB_FLAGS)
        database_update(tableName,"set direction='对对对，就是这个协议' where switch=456");
    //database_update("Protocol_switch","set protocol='456',direction='456' where switch=456");
    if(flags&UNCON_QUERRY_FLAGS)
    {
        retval=(FetchRtePtr)database_query(tableName);
        // results=(char***)database_query("Protocol_switch");
        results=retval->dataPtr;
        if(results==NULL)
            return 0;
    }
    if(flags&DISPLAY_DATA_FLAGS)
        printProtocolSwitch();

    if(flags&DELETE_DB_FLAGS)
        database_delete(tableName,"protocol=789");

    if(flags&DISPLAY_DATA_FLAGS)
        printProtocolSwitch();

    if(flags&DEBUG_DB_FLAGS)
		printf("Test for %s done!\n",tableName);
    return 0;
}
