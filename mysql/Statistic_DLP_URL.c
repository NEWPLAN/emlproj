/*************************************************************************
	> File Name: statistic_DLP_url.c
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 13时20分52秒
 ************************************************************************/

#include<stdio.h>
#include "Statistic_DLP_URL.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="Statistic_DLP_URL";
static int rowNum=0;

statisticurlPtr load_statisticURL(void* A, void* B)
{

    FetchRtePtr val=NULL;
    statisticurlPtr staurl_list=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table statisticurl \n");
        return NULL;
    }
    rowNum=val->row;
    staurl_list=(statisticurlPtr)malloc(sizeof(statisticurl)*(val->row+1));
    if(!staurl_list)
    {
        printf("error in malloc for statisticurl \n");
        return NULL;
    }
    memset(staurl_list,0,sizeof(statisticurl)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(staurl_list[index].DLP_URL_id,val->dataPtr[index][0],sizeof(staurl_list[index].DLP_URL_id)-1);
        memcpy(staurl_list[index].DLP_URL_eml_id,val->dataPtr[index][1],sizeof(staurl_list[index].DLP_URL_eml_id)-1);
        memcpy(staurl_list[index].DLP_URL_hitlist,val->dataPtr[index][2],sizeof(staurl_list[index].DLP_URL_hitlist)-1);
    }
    free_memory(val);
    val=NULL;
    if(A)
    *((int *)A)=rowNum;
    return staurl_list;
}

void print_statisticurl(void)
{
    statisticurlPtr var=load_statisticURL(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in print_statisticurl\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\n",var[rowindex].DLP_URL_id,var[rowindex].DLP_URL_eml_id,var[rowindex].DLP_URL_hitlist);
        rowindex++;
    }
    statisticurlRelase(&var);
}
void statisticurlRelase(statisticurlPtr* tables)
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
int StatisticDLPUrlTest(int flags)
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
        database_insert(tableName,"1,2,'DLP_URL_hitlist'");
    //database_query("Protocol_switch");

    if(flags&DISPLAY_DATA_FLAGS)
        print_statisticurl();

    if(flags&UPDATE_DB_FLAGS)
        database_update(tableName,"set DLP_URL_hitlist='对对对，就是dlp_keywords_hitlist' where DLP_URL_id=1");
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
        print_statisticurl();

    if(flags&DELETE_DB_FLAGS)
        database_delete(tableName,"DLP_URL_id=1");

    if(flags&DISPLAY_DATA_FLAGS)
        print_statisticurl();

    if(flags&DEBUG_DB_FLAGS)
		printf("Test for %s done!\n",tableName);
    return 0;
}
