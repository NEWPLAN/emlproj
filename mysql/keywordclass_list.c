/*************************************************************************
	> File Name: keyclass.c
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 12时52分06秒
 ************************************************************************/

#include<stdio.h>
#include "keywordclass_list.h"
#include "dboperate.h"
static const char * const tableName="keywordclass_list";
static int rowNum=0;

keyclassPtr load_keyclass(void* A, void* B)
{

    FetchRtePtr val=NULL;
    keyclassPtr prolist=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table keyclass \n");
        return NULL;
    }
    rowNum=val->row;
    prolist=(keyclassPtr)malloc(sizeof(keyclass)*(val->row+1));
    if(!prolist)
    {
        printf("error in malloc for keyclass \n");
        return NULL;
    }
    memset(prolist,0,sizeof(keyclass)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(prolist[index].class_id,val->dataPtr[index][0],sizeof(prolist[index].class_id)-1);
        memcpy(prolist[index].class_name,val->dataPtr[index][1],sizeof(prolist[index].class_name)-1);
        memcpy(prolist[index].class_regex,val->dataPtr[index][2],sizeof(prolist[index].class_regex)-1);
    }
    free_memory(val);
    val=NULL;
    *((int *)A)=rowNum;
    return prolist;
}

void print_keyclass(void)
{
    keyclassPtr var=load_keyclass(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in print_keyclass\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\n",var[rowindex].class_id,var[rowindex].class_name,var[rowindex].class_regex);
        rowindex++;
    }
    keyclassRelase(&var);
}
void keyclassRelase(keyclassPtr* tables)
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
int KeyClassTest(int flags)
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
        database_insert(tableName,"11,'bankcard','[\\d]{19,17}'");
    //database_query("Protocol_switch");

    if(flags&DISPLAY_DATA_FLAGS)
        print_keyclass();

    if(flags&UPDATE_DB_FLAGS)
        database_update(tableName,"set class_name='银行卡号码', class_regex='Tsinghua' where class_id=11");
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
        print_keyclass();

    if(flags&DELETE_DB_FLAGS)
        database_delete(tableName,"class_id=11");

    if(flags&DISPLAY_DATA_FLAGS)
        print_keyclass();

    if(flags&DEBUG_DB_FLAGS)
		printf("Test for %s done!\n",tableName);
    return 0;
}
