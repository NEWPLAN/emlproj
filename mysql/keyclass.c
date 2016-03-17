/*************************************************************************
	> File Name: keyclass.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 12时52分06秒
 ************************************************************************/

#include<stdio.h>
#include "keyclass.h"
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
