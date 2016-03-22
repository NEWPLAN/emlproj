/*************************************************************************
	> File Name: user_info.c
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 13时07分45秒
 ************************************************************************/

#include<stdio.h>
#include "user_info.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="user_info";
static int rowNum=0;


userinfoPtr load_user_info(void* A, void* B)
{
    FetchRtePtr val=NULL;
    userinfoPtr prolist=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table user info\n");
        return NULL;
    }
    rowNum=val->row;
    prolist=(userinfoPtr)malloc(sizeof(userinfo)*(val->row+1));
    if(!prolist)
    {
        printf("error in malloc for user info\n");
        return NULL;
    }
    memset(prolist,0,sizeof(userinfo)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(prolist[index].user_id,val->dataPtr[index][0],sizeof(prolist[index].user_id)-1);
        memcpy(prolist[index].user_type,val->dataPtr[index][1],sizeof(prolist[index].user_type)-1);
        memcpy(prolist[index].user_name,val->dataPtr[index][2],sizeof(prolist[index].user_name)-1);
        memcpy(prolist[index].user_department,val->dataPtr[index][3],sizeof(prolist[index].user_department)-1);
        memcpy(prolist[index].user_tel,val->dataPtr[index][4],sizeof(prolist[index].user_tel)-1);
        memcpy(prolist[index].user_passwd,val->dataPtr[index][5],sizeof(prolist[index].user_passwd)-1);
        memcpy(prolist[index].user_mailbox,val->dataPtr[index][6],sizeof(prolist[index].user_mailbox)-1);
    }
    free_memory(val);
    val=NULL;
    if(A)
    *((int *)A)=rowNum;
    return prolist;
}

void print_userinfo(void)
{
    userinfoPtr var=load_user_info(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in printuserinfo\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\n",
               var[rowindex].user_id,var[rowindex].user_type,var[rowindex].user_name,
               var[rowindex].user_department,var[rowindex].user_tel,var[rowindex].user_passwd,var[rowindex].user_mailbox);
        rowindex++;
    }
    userinfoRelase(&var);
}
void userinfoRelase(userinfoPtr* tables)
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
int UserInfoTest(int flags)
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
        database_insert(tableName,"'2012',1,'NEWPLAN','Tsinghua','18661828256','123asd','newplan001@163.com'");
    //database_query("Protocol_switch");

    if(flags&DISPLAY_DATA_FLAGS)
        print_userinfo();

    if(flags&UPDATE_DB_FLAGS)
        database_update(tableName,"set user_department='对对对，Tsinghua就是清华' where user_id='2012'");
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
        print_userinfo();

    if(flags&DELETE_DB_FLAGS)
        database_delete(tableName,"user_id='2012'");

    if(flags&DISPLAY_DATA_FLAGS)
        print_userinfo();

    if(flags&DEBUG_DB_FLAGS)
		printf("Test for %s done!\n",tableName);
    return 0;
}
