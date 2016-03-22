/*************************************************************************
	> File Name: statistic_eml_info_basic.c
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 13时19分25秒
 ************************************************************************/

#include<stdio.h>
#include "Statistic_Emlinfo_basic.h"
#include "dboperate.h"
#include <string.h>

static const char * const tableName="Statistic_Emlinfo_Basic";
static int rowNum=0;

emlinfoPtr load_emlinfo(void* A, void* B)
{

    FetchRtePtr val=NULL;
    emlinfoPtr emlinfolist=NULL;
    int index=0;
    if((val=database_query(tableName))==NULL)
    {
        printf("error in loading table emlinfo \n");
        return NULL;
    }
    rowNum=val->row;
    emlinfolist=(emlinfoPtr)malloc(sizeof(emlinfo)*(val->row+1));
    if(!emlinfolist)
    {
        printf("error in malloc for emlinfo \n");
        return NULL;
    }
    memset(emlinfolist,0,sizeof(emlinfo)*(val->row+1));
    for(index=0; index<val->row; index++)
    {
        memcpy(emlinfolist[index].eml_id,val->dataPtr[index][0],sizeof(emlinfolist[index].eml_id)-1);
        memcpy(emlinfolist[index].eml_name,val->dataPtr[index][1],sizeof(emlinfolist[index].eml_name)-1);
        memcpy(emlinfolist[index].eml_from,val->dataPtr[index][2],sizeof(emlinfolist[index].eml_from)-1);
        memcpy(emlinfolist[index].eml_to,val->dataPtr[index][3],sizeof(emlinfolist[index].eml_to)-1);
        memcpy(emlinfolist[index].eml_from_to,val->dataPtr[index][4],sizeof(emlinfolist[index].eml_from_to)-1);
        memcpy(emlinfolist[index].eml_attachment_name,val->dataPtr[index][5],sizeof(emlinfolist[index].eml_attachment_name)-1);
        memcpy(emlinfolist[index].eml_attachment_type,val->dataPtr[index][6],sizeof(emlinfolist[index].eml_attachment_type)-1);
        memcpy(emlinfolist[index].eml_attachment_virus,val->dataPtr[index][7],sizeof(emlinfolist[index].eml_attachment_virus)-1);
        memcpy(emlinfolist[index].eml_isspam,val->dataPtr[index][8],sizeof(emlinfolist[index].eml_isspam)-1);
        memcpy(emlinfolist[index].eml_DLPkeywordhit,val->dataPtr[index][9],sizeof(emlinfolist[index].eml_DLPkeywordhit)-1);
        memcpy(emlinfolist[index].eml_DLPkeyclasshit,val->dataPtr[index][10],sizeof(emlinfolist[index].eml_DLPkeyclasshit)-1);
        memcpy(emlinfolist[index].eml_URLhit,val->dataPtr[index][11],sizeof(emlinfolist[index].eml_URLhit)-1);
    }
    free_memory(val);
    val=NULL;
    *((int *)A)=rowNum;
    return emlinfolist;
}

void print_emlinfo(void)
{
    emlinfoPtr var=load_emlinfo(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in print_emlinfo\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\n",
               var[rowindex].eml_id,var[rowindex].eml_name,var[rowindex].eml_from,var[rowindex].eml_to,var[rowindex].eml_from_to,
               var[rowindex].eml_attachment_name,var[rowindex].eml_attachment_type,var[rowindex].eml_attachment_virus,var[rowindex].eml_isspam,
               var[rowindex].eml_DLPkeywordhit,var[rowindex].eml_DLPkeyclasshit,var[rowindex].eml_URLhit);
        rowindex++;
    }
    emlinfoRelase(&var);
}
void emlinfoRelase(emlinfoPtr* tables)
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
int StatisticEmlInfoBasicTest(int flags)
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
        database_insert(tableName,"1,'eml_name','eml_from','eml_to','eml_from_to','eml_attachment_name','eml_attachment_type',2,3,4,5,6");
    //database_query("Protocol_switch");

    if(flags&DISPLAY_DATA_FLAGS)
        print_emlinfo();

    if(flags&UPDATE_DB_FLAGS)
        database_update(tableName,"set eml_attachment_name='对对对 就是eml_attachment_name' where eml_id=1");
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
        print_emlinfo();

    if(flags&DELETE_DB_FLAGS)
        database_delete(tableName,"eml_id");

    if(flags&DISPLAY_DATA_FLAGS)
        print_emlinfo();

    if(flags&DEBUG_DB_FLAGS)
		printf("Test for %s done!\n",tableName);
    return 0;
}
