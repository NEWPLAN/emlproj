/*************************************************************************
	> File Name: statistic_eml_info_basic.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 13时19分25秒
 ************************************************************************/

#include<stdio.h>
#include "statistic_eml_info_basic.h"
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
        memcpy(emlinfolist[index].eml_to,val->dataPtr[index][2],sizeof(emlinfolist[index].eml_to)-1);
        memcpy(emlinfolist[index].eml_from_to,val->dataPtr[index][3],sizeof(emlinfolist[index].eml_from_to)-1);
        memcpy(emlinfolist[index].eml_attachment_name,val->dataPtr[index][4],sizeof(emlinfolist[index].eml_attachment_name)-1);
        memcpy(emlinfolist[index].eml_attachment_type,val->dataPtr[index][5],sizeof(emlinfolist[index].eml_attachment_type)-1);
        memcpy(emlinfolist[index].eml_isspam,val->dataPtr[index][6],sizeof(emlinfolist[index].eml_isspam)-1);
        memcpy(emlinfolist[index].eml_dlp_wordhit,val->dataPtr[index][7],sizeof(emlinfolist[index].eml_dlp_wordhit)-1);
        memcpy(emlinfolist[index].eml_dlp_classhit,val->dataPtr[index][8],sizeof(emlinfolist[index].eml_dlp_classhit)-1);
        memcpy(emlinfolist[index].eml_urlhit,val->dataPtr[index][9],sizeof(emlinfolist[index].eml_urlhit)-1);
    }
    free_memory(val);
    val=NULL;
    return emlinfolist;
}

void print_emlinfo(void)
{
    emlinfoPtr var=load_emlinfo(NULL,NULL);
    int rowindex=0,totalnum=rowNum;
    printf("test in print_emlinfo\n");
    while(rowindex<totalnum)
    {
        printf("%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\t%-12s\n",
               var[rowindex].eml_id,var[rowindex].eml_name,var[rowindex].eml_to,var[rowindex].eml_from_to,
               var[rowindex].eml_attachment_name,var[rowindex].eml_attachment_type,var[rowindex].eml_isspam,
               var[rowindex].eml_dlp_wordhit,var[rowindex].eml_dlp_classhit,var[rowindex].eml_urlhit);
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

