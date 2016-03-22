/*************************************************************************
	> File Name: url_list.c
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 13时09分01秒
 ************************************************************************/

#include<stdio.h>
#include"URL_list.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="URL_list";
static int rowNum=0;

urllistPtr load_url_list(void* A, void* B)
{

    FetchRtePtr val=NULL;
    urllistPtr prolist=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table url list\n");
        return NULL;
    }
    rowNum=val->row;
    prolist=(urllistPtr)malloc(sizeof(urllist)*(val->row+1));
    if(!prolist)
    {
        printf("error in malloc for url list\n");
        return NULL;
    }
    memset(prolist,0,sizeof(urllist)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(prolist[index].strategy_id,val->dataPtr[index][0],sizeof(prolist[index].strategy_id)-1);
        memcpy(prolist[index].strategy_type,val->dataPtr[index][1],sizeof(prolist[index].strategy_type)-1);
        memcpy(prolist[index].strategy_content,val->dataPtr[index][2],sizeof(prolist[index].strategy_content)-1);
        memcpy(prolist[index].URL_level,val->dataPtr[index][3],sizeof(prolist[index].URL_level)-1);
        memcpy(prolist[index].level_info,val->dataPtr[index][4],sizeof(prolist[index].level_info)-1);
    }
    free_memory(val);
    val=NULL;
    if(A)
    *((int *)A)=rowNum;
    return prolist;
}

void print_urllist(void)
{
    urllistPtr var=load_url_list(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in printurllistSwitch\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\t%-12s\t%-12s\n",var[rowindex].strategy_id,var[rowindex].strategy_type,var[rowindex].strategy_content,var[rowindex].URL_level,var[rowindex].level_info);
        rowindex++;
    }
    urllistRelase(&var);
}
void urllistRelase(urllistPtr* tables)
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
int UrlListTest(int flags)
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
        database_insert(tableName,"1,'strategy_type','strategy_content','URL_level','level_info'");
    //database_query("Protocol_switch");

    if(flags&DISPLAY_DATA_FLAGS)
        print_urllist();

    if(flags&UPDATE_DB_FLAGS)
        database_update(tableName,"set strategy_content='对对对,strategy_content' where strategy_id=1");
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
        print_urllist();

    if(flags&DELETE_DB_FLAGS)
        database_delete(tableName,"strategy_id=1");

    if(flags&DISPLAY_DATA_FLAGS)
        print_urllist();

    if(flags&DEBUG_DB_FLAGS)
		printf("Test for %s done!\n",tableName);
    return 0;
}
