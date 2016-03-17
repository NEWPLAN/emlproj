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

