/*************************************************************************
	> File Name: statistic_protocol.c
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 13时15分59秒
 ************************************************************************/

#include<stdio.h>
#include<stdio.h>
#include"Statistic_Protocol.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="Statistic_Protocol";
static int rowNum=0;

staprotocolPtr load_statisticProtocol(void* A, void* B)
{

    FetchRtePtr val=NULL;
    staprotocolPtr prolist=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table staprotocol \n");
        return NULL;
    }
    rowNum=val->row;
    prolist=(staprotocolPtr)malloc(sizeof(staprotocol)*(val->row+1));
    if(!prolist)
    {
        printf("error in malloc for staprotocol \n");
        return NULL;
    }
    memset(prolist,0,sizeof(staprotocol)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(prolist[index].protocol,val->dataPtr[index][0],sizeof(prolist[index].protocol)-1);
        memcpy(prolist[index].source_ip,val->dataPtr[index][1],sizeof(prolist[index].source_ip)-1);
        memcpy(prolist[index].dest_ip,val->dataPtr[index][2],sizeof(prolist[index].dest_ip)-1);
        memcpy(prolist[index].open_time,val->dataPtr[index][3],sizeof(prolist[index].open_time)-1);
        memcpy(prolist[index].close_time,val->dataPtr[index][4],sizeof(prolist[index].close_time)-1);
        memcpy(prolist[index].eml_file_path,val->dataPtr[index][5],sizeof(prolist[index].eml_file_path)-1);
    }
    free_memory(val);
    val=NULL;
    if(A)
    *((int *)A)=rowNum;
    return prolist;
}

void print_staprotocol(void)
{
    staprotocolPtr var=load_statisticProtocol(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in printstaprotocol\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\n",var[rowindex].protocol,var[rowindex].source_ip,var[rowindex].dest_ip,var[rowindex].open_time,var[rowindex].close_time,var[rowindex].eml_file_path);
        rowindex++;
    }
    staprotocolRelase(&var);
}
void staprotocolRelase(staprotocolPtr* tables)
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
int StatisticProtocolTest(int flags)
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
        database_insert(tableName,"'protocol','source_ip','dest_ip','2016:3:20','2016:3:21','eml_file_path'");
    //database_query("Protocol_switch");

    if(flags&DISPLAY_DATA_FLAGS)
        print_staprotocol();

    if(flags&UPDATE_DB_FLAGS)
        database_update(tableName,"set protocol='对对对,protocol' where eml_file_path='eml_file_path'");
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
        print_staprotocol();

    if(flags&DELETE_DB_FLAGS)
        database_delete(tableName,"eml_file_path='eml_file_path'");

    if(flags&DISPLAY_DATA_FLAGS)
        print_staprotocol();

    if(flags&DEBUG_DB_FLAGS)
		printf("Test for %s done!\n",tableName);
    return 0;
}
