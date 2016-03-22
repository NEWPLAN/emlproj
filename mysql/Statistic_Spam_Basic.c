/*************************************************************************
	> File Name: statistic_spam.c
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 13时13分42秒
 ************************************************************************/

#include<stdio.h>
#include "Statistic_Spam_Basic.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="Statistic_Spam_Basic";
static int rowNum=0;

statspamPtr load_statistic_spam(void* A, void* B)
{

    FetchRtePtr val=NULL;
    statspamPtr prolist=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table statspam \n");
        return NULL;
    }
    rowNum=val->row;
    prolist=(statspamPtr)malloc(sizeof(statspam)*(val->row+1));
    if(!prolist)
    {
        printf("error in malloc for statspam \n");
        return NULL;
    }
    memset(prolist,0,sizeof(statspam)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(prolist[index].spam_id,val->dataPtr[index][0],sizeof(prolist[index].spam_id)-1);
        memcpy(prolist[index].spam_eml_path,val->dataPtr[index][1],sizeof(prolist[index].spam_eml_path)-1);
        memcpy(prolist[index].spam_eml_score,val->dataPtr[index][2],sizeof(prolist[index].spam_eml_score)-1);
        memcpy(prolist[index].spam_eml_info,val->dataPtr[index][3],sizeof(prolist[index].spam_eml_info)-1);
        memcpy(prolist[index].spam_eml_id,val->dataPtr[index][4],sizeof(prolist[index].spam_eml_id)-1);
    }
    free_memory(val);
    val=NULL;
    if(A)
    *((int *)A)=rowNum;
    return prolist;
}

void print_statspam(void)
{
    statspamPtr var=load_statistic_spam(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in print_statspam\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\t%-12s\t%-12s\n",var[rowindex].spam_id,var[rowindex].spam_eml_path,var[rowindex].spam_eml_score,var[rowindex].spam_eml_info,var[rowindex].spam_eml_id);
        rowindex++;
    }
    statspamRelase(&var);
}
void statspamRelase(statspamPtr* tables)
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
int StatisticSpamTest(int flags)
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
        database_insert(tableName,"1,'spam_eml_path','spam_eml_score','spam_eml_info',2");
    //database_query("Protocol_switch");

    if(flags&DISPLAY_DATA_FLAGS)
        print_statspam();

    if(flags&UPDATE_DB_FLAGS)
        database_update(tableName,"set spam_eml_path='对对对,spam_eml_path' where spam_id=1");
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
        print_statspam();

    if(flags&DELETE_DB_FLAGS)
        database_delete(tableName,"spam_id=1");

    if(flags&DISPLAY_DATA_FLAGS)
        print_statspam();

    if(flags&DEBUG_DB_FLAGS)
		printf("Test for %s done!\n",tableName);
    return 0;
}
