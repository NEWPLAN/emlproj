#include "all.h"
#include "assist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>

int matchRegex(char* filepath,FetchRtePtr* valsPtr)
{
    char abspath[1024]={0};
    char appendixapth[1024]={0};
    DIR* d;
    struct dirent *file;
    assert(filepath!=NULL);
    int index=0;
    FetchRtePtr vals=*valsPtr;

    sprintf(appendixapth,"%s/%s",filepath,"temps");    
    if(!(d=opendir(appendixapth)))
    {
        printf("error in open dir : %s\n",filepath);
        return 0;
    }
    while((file=readdir(d))!=NULL)
    {
        if(strncmp(file->d_name,".",1)==0)
            continue;
        {
            /*判断是文件夹处理下一个*/
            struct stat info;
            stat(file->d_name,&info);
            if(S_ISDIR(info.st_mode))
                continue;
        }
        
        memset(abspath,0,sizeof(filepath));
        sprintf(abspath,"%s/%s",appendixapth,file->d_name);
        for(index=0;index<vals->row;index++)
        {
            if(checkInclass(abspath ,(vals->dataPtr)[index][5]))
                return 1;
        }
    }
    free_memory(vals);
    *valsPtr=NULL;
    return 0;
}

CheckType keywordClassCheck(mimePtr email,char* owner,int direction)
{

    if(checkInGateway(owner))
     ;//   return NONE;

    //#1.user级处理
    char command[1024]= {0};
    sprintf(command, "SELECT * FROM strategy_keywordclasslist WHERE owner = '%s' AND level = 0  AND direction = %d",owner, direction);

    FetchRtePtr user_keywordclasses=sql_query(command);

    if(matchRegex(email->workspace,&user_keywordclasses))/*email match user_keywordclasses*/
        return CONFIRMED;

    //#2.domain级处理
    char* ownerDomain = getDomain(owner);
    memset(command,0,sizeof(command));
    sprintf(command,"SELECT * FROM strategy_keywordclasslist WHERE owner = '%s' AND level = 1  AND direction = %d",ownerDomain,direction);
    FetchRtePtr domain_keywordclasses=sql_query(command);

    if(matchRegex(email->workspace,&domain_keywordclasses))/*email match domain_keywordclasses*/
        return CONFIRMED;

    //#3.网关级处理
    memset(command,0,sizeof(command));
    sprintf(command,"SELECT * FROM strategy_keywordclasslist WHERE owner = '%s' AND level = 2  AND direction = %d","GLOBAL",direction);
    FetchRtePtr global_keywordclasses=sql_query(command);

    if(matchRegex(email->workspace,&global_keywordclasses))/*email match global_keywordclasses:*/
        return CONFIRMED;
    return OK;
}
