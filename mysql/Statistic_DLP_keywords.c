/*************************************************************************
	> File Name: statistic_DLP_keywords.c
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 13时22分08秒
 ************************************************************************/

#include<stdio.h>
#include"Statistic_DLP_keywords.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="Statistic_DLP_keywords";
static int rowNum=0;


statisticwordsPtr load_statistic_DLP_keywords(void* A, void* B)
{

    FetchRtePtr val=NULL;
    statisticwordsPtr sta_keyword_list=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table Statistic_DLP_keywords\n");
        return NULL;
    }
    rowNum=val->row;
    sta_keyword_list=(statisticwordsPtr)malloc(sizeof(statisticwords)*(val->row+1));
    if(!sta_keyword_list)
    {
        printf("error in malloc for Statistic_DLP_keywords\n");
        return NULL;
    }
    memset(sta_keyword_list,0,sizeof(statisticwords)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(sta_keyword_list[index].DLP_keyword_id,val->dataPtr[index][0],sizeof(sta_keyword_list[index].DLP_keyword_id)-1);
        memcpy(sta_keyword_list[index].DLP_keyword_eml_id,val->dataPtr[index][1],sizeof(sta_keyword_list[index].DLP_keyword_eml_id)-1);
        memcpy(sta_keyword_list[index].DLP_keyword_hitlist,val->dataPtr[index][2],sizeof(sta_keyword_list[index].DLP_keyword_hitlist)-1);
    }
    free_memory(val);
    val=NULL;
    if(A)
    *((int *)A)=rowNum;
    return sta_keyword_list;
}

void printStatistic_DLP_keywords(void)
{
    statisticwordsPtr var=load_statistic_DLP_keywords(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in printStatistic_DLP_keywords\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\n",var[rowindex].DLP_keyword_id,var[rowindex].DLP_keyword_eml_id,var[rowindex].DLP_keyword_hitlist);
        rowindex++;
    }
    Statistic_DLP_keywordsRelase(&var);
}
void Statistic_DLP_keywordsRelase(statisticwordsPtr* tables)
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
int StatisticDLPKeyWordsTest(int flags)
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
        database_insert(tableName,"1,2,'dlp_keywords_hitlist'");
    //database_query("Protocol_switch");

    if(flags&DISPLAY_DATA_FLAGS)
        printStatistic_DLP_keywords();

    if(flags&UPDATE_DB_FLAGS)
        database_update(tableName,"set DLP_keyword_hitlist='对对对, 就是DLP_keyword_hitlist' where DLP_keyword_id=1");
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
        printStatistic_DLP_keywords();

    if(flags&DELETE_DB_FLAGS)
        database_delete(tableName,"DLP_keyword_id=1");

    if(flags&DISPLAY_DATA_FLAGS)
        printStatistic_DLP_keywords();

    if(flags&DEBUG_DB_FLAGS)
		printf("Test for %s done!\n",tableName);
    return 0;
}
