#include "all.h"
#include "assist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dboperate.h"
#include <dlfcn.h>
#include <unistd.h>
#include <sys/time.h>
#include "mainall.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

static int match_email(char* workspace,FetchRtePtr keyworlds)
{
    int index, index2;
    char Ate=255;
    int backval;
    int (*dlfunc)(int argc, char* argv[]);
    void *handle;
    char command2[1024]={0};
    char workpath[1024]= {0};
    char dictpath[1024]={0};
    sprintf(dictpath,"%s/userdict.txt",workspace);

    sprintf(workpath,"%s/temps",workspace);

    char * inputs[4]= {NULL,workpath,&Ate,dictpath};

    FILE* fptr=fopen(dictpath,"wb");/*write to dictionary*/
    for(index=0; index<keyworlds->row; index++)
    {
        memset(command2,0,sizeof(command2));
        sprintf(command2,"SELECT keyword FROM strategy_keywords WHERE list_id = %s",keyworlds->dataPtr[index][0]);
        FetchRtePtr listids=sql_query(command2);
        for(index2=0; index2<listids->row; index2++)
        {
            char key[1024]={0};
            if(fptr)
            {
                memset(key,0,sizeof(key));
                sprintf(key,"%s\n",(listids->dataPtr)[index2][0]);
                fwrite(key,sizeof(char),strlen(key),fptr);
            }
        }
        free_memory(listids); 
    }

    fclose(fptr);
#ifdef __DEBUG
    printf("hello in ParseKeyChs\n");
#endif
    handle=dlopen("./spliter.so",RTLD_LAZY);
#ifdef __DEBUG
    printf("in open libs\n");
#endif
    dlfunc=dlsym(handle,"SpliterMain");
    if(!(handle&&dlfunc))
    {
        printf("error in open dynamic libs %s\n",dlerror());
        return 0;
    }
    return dlfunc(4,inputs);
}

CheckType keywordCheck(mimePtr email,char* owner,int direction)
{
    int index=0,index2;
    if(!checkInGateway(owner))
    	return NONE;
    char command[1024];

    //#1.user级处理
    memset(command, 0, sizeof(command));
    sprintf(command,"SELECT id FROM strategy_keywordlist WHERE owner= '%s' AND level = 0 AND direction = %d",owner,direction);
    FetchRtePtr user_listids=sql_query(command);
    if(match_email(email->workspace,user_listids))
            return CONFIRMED;

    //#2.domain级处理
    char *domain = getDomain(owner);// #domain 在网关下

    memset(command, 0, sizeof(command));
    sprintf(command,"SELECT id FROM strategy_keywordlist WHERE owner= '%s' AND level = 1 AND direction = %d",domain,direction);
    FetchRtePtr domain_listids=sql_query(command);
    if(match_email(email->workspace,domain_listids))
            return CONFIRMED;

    //#3.网关级处理
    memset(command, 0, sizeof(command));
    sprintf(command,"SELECT id FROM strategy_keywordlist WHERE owner= '%s' AND level = 2 AND direction = %d","GLOBAL",direction);
    FetchRtePtr global_listids=sql_query(command);
    if(match_email(email->workspace,global_listids))
            return CONFIRMED;
    return OK;
}
