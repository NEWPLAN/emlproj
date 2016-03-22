/*************************************************************************
	> File Name: statistic_virus.c
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 13时12分04秒
 ************************************************************************/

#include<stdio.h>
#include "Statistic_Virus_Basic.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="Statistic_Virus_Basic";
static int rowNum=0;

statvirusPtr load_statistic_virus(void* A, void* B)
{

    FetchRtePtr val=NULL;
    statvirusPtr viruslist=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table statvirus switch\n");
        return NULL;
    }
    rowNum=val->row;
    viruslist=(statvirusPtr)malloc(sizeof(statvirus)*(val->row+1));
    if(!viruslist)
    {
        printf("error in malloc for statvirus switch\n");
        return NULL;
    }
    memset(viruslist,0,sizeof(statvirus)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(viruslist[index].virus_id,val->dataPtr[index][0],sizeof(viruslist[index].virus_id)-1);
        memcpy(viruslist[index].virus_eml_id,val->dataPtr[index][1],sizeof(viruslist[index].virus_eml_id)-1);
        memcpy(viruslist[index].virus_filename,val->dataPtr[index][2],sizeof(viruslist[index].virus_filename)-1);
        memcpy(viruslist[index].virus_info,val->dataPtr[index][3],sizeof(viruslist[index].virus_info)-1);
    }
    free_memory(val);
    val=NULL;
    *((int *)A)=rowNum;
    return viruslist;
}

void print_statvirus(void)
{
    statvirusPtr var=load_statistic_virus(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in printstatvirusSwitch\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\t%-12s\n",var[rowindex].virus_id,var[rowindex].virus_eml_id,var[rowindex].virus_filename,var[rowindex].virus_info);
        rowindex++;
    }
    statvirusRelase(&var);
}
void statvirusRelase(statvirusPtr* tables)
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
int StatisticVirusTest(int flags)
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
        database_insert(tableName,"1,'virus_eml_id','virus_filename','virus_info'");
    //database_query("Protocol_switch");

    if(flags&DISPLAY_DATA_FLAGS)
        print_statvirus();

    if(flags&UPDATE_DB_FLAGS)
        database_update(tableName,"set virus_filename='对对对,virus_filename' where virus_id=1");
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
        print_statvirus();

    if(flags&DELETE_DB_FLAGS)
        database_delete(tableName,"virus_id=1");

    if(flags&DISPLAY_DATA_FLAGS)
        print_statvirus();

    if(flags&DEBUG_DB_FLAGS)
		printf("Test for %s done!\n",tableName);
    return 0;
}
