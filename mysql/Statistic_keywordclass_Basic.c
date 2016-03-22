/*************************************************************************
	> File Name: statistic_keyclass_basic.c
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 13时17分54秒
 ************************************************************************/

#include<stdio.h>
#include "Statistic_keywordclass_Basic.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="Statistic_keywordclass_Basic";
static int rowNum=0;

dlpkeyclassPtr load_dlpkeyclass(void* A, void* B)
{

    FetchRtePtr val=NULL;
    dlpkeyclassPtr dlp_keyclass=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table dlpkeyclass \n");
        return NULL;
    }
    rowNum=val->row;
    dlp_keyclass=(dlpkeyclassPtr)malloc(sizeof(dlpkeyclass)*(val->row+1));
    if(!dlp_keyclass)
    {
        printf("error in malloc for dlpkeyclass \n");
        return NULL;
    }
    memset(dlp_keyclass,0,sizeof(dlpkeyclass)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(dlp_keyclass[index].DLP_keywordclass_id,val->dataPtr[index][0],sizeof(dlp_keyclass[index].DLP_keywordclass_id)-1);
        memcpy(dlp_keyclass[index].DLP_keywordclass_eml_id,val->dataPtr[index][1],sizeof(dlp_keyclass[index].DLP_keywordclass_eml_id)-1);
        memcpy(dlp_keyclass[index].DLP_keywordclass_hitlist,val->dataPtr[index][2],sizeof(dlp_keyclass[index].DLP_keywordclass_hitlist)-1);
    }
    free_memory(val);
    val=NULL;
    if(A)
    *((int *)A)=rowNum;
    return dlp_keyclass;
}

void print_dlpkeyclass(void)
{
    dlpkeyclassPtr var=load_dlpkeyclass(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in print_dlpkeyclass\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\n",var[rowindex].DLP_keywordclass_id,var[rowindex].DLP_keywordclass_eml_id,var[rowindex].DLP_keywordclass_hitlist);
        rowindex++;
    }
    dlpkeyclassRelase(&var);
}
void dlpkeyclassRelase(dlpkeyclassPtr* tables)
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
int StatisticKeyClassBasicTest(int flags)
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
        database_insert(tableName,"1,'DLP_keywordclass_eml_id','DLP_keywordclass_hitlist'");
    //database_query("Protocol_switch");

    if(flags&DISPLAY_DATA_FLAGS)
        print_dlpkeyclass();

    if(flags&UPDATE_DB_FLAGS)
        database_update(tableName,"set DLP_keywordclass_eml_id='对对对,DLP_keywordclass_eml_id' where DLP_keywordclass_id=1");
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
        print_dlpkeyclass();

    if(flags&DELETE_DB_FLAGS)
        database_delete(tableName,"DLP_keywordclass_id=1");

    if(flags&DISPLAY_DATA_FLAGS)
        print_dlpkeyclass();

    if(flags&DEBUG_DB_FLAGS)
		printf("Test for %s done!\n",tableName);
    return 0;
}
