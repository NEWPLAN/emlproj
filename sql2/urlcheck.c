#include "all.h"
#include "assist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../regex/compileRegur.h"
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

static urlptr loadUrl(char* filepath)
{
	char * regURL="(^|\\b)(((https|http|ftp|rtsp|mms)?\\:\\/\\/)[^\\s]+)($|\\b)";
	urlptr heads=NULL;
	if(filepath==NULL)
		return NULL;		
	urlptr A= rteStr(regURL , filepath);
	urlptr b=A->next;
	return b;
}

static int compareUrl(char* workspace,char*** src, int num)
{
	int index=0,second=0;
	DIR* d;
    struct dirent *file;
    
    char abspath[1024]={0};
    sprintf(abspath,"%s/temps",workspace);	
    if(!(d=opendir(abspath)))
    {
        printf("error in open dir : %s\n",abspath);
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
        char filepaths[1024]={0};
		sprintf(filepaths,"%s/temps/%s",workspace,file->d_name);
		urlptr temps=NULL,hhh=loadUrl(filepaths);
		if(hhh==NULL)
			continue;
		for(index=0;index<num;index++)
		{
			temps=hhh;
			while(temps)
			{
				printf("db:%s[%d]\ntxt:%s[%d]\n",temps->data,(int)strlen(temps->data),src[index][0],(int)strlen(src[index][0]));/*has match for this url*/
				if(strcmp(temps->data,src[index][0])==0)
					return 1;
				printf("%s\n%s\n",temps->data,src[index][0]);/*has match for this url*/
				temps=temps->next;
			}
		}              
    }
    closedir(d);
    return 0;
}

int matchUrl(mimePtr email,FetchRtePtr* vals)
{
	FetchRtePtr urlitems=NULL,sqlval=*vals;
	char command[1024]={0};
	int index=0,rteval=0;
	if(sqlval==NULL||sqlval->row==0)
		return rteval;
	sprintf(command,"SELECT URL FROM strategy_urls WHERE list_id = %s",sqlval->dataPtr[0][0]);
	urlitems=sql_query(command);
	if(urlitems==NULL)
		return rteval;
	printf("url check\n");
	rteval=compareUrl(email->workspace,urlitems->dataPtr,urlitems->row);
	if(!rteval)
	{
		printf("不要害怕，马上联网查你了！！！\n");
	}
	free_memory(urlitems);
	free_memory(sqlval);
	*vals=NULL;
	return rteval;

}

CheckType urlCheck(mimePtr email, char* owner, int direction)
{
	FetchRtePtr listids=NULL;
	char command1[1024]= {0};
	char *domain=NULL;
    if(!checkInGateway(owner))/*owner not in netgate:*/
        return NONE;
    
    //1.user级处理
    memset(command1,0,sizeof(command1));
    sprintf(command1, "SELECT id FROM strategy_urllist WHERE owner = '%s' AND level = 0 AND direction = %d",owner,direction);
    listids = sql_query(command1);
    if(listids!=NULL)
		if(matchUrl(email,&listids))
			return CONFIRMED;
        
    //2.domain级处理
    domain = getDomain(owner); //domain 在网关下
    memset(command1,0,sizeof(command1));
    sprintf(command1, "SELECT id FROM strategy_urllist WHERE owner = '%s' AND level = 1 AND direction = %d",domain,direction);
    listids = sql_query(command1);
    if(listids!=NULL)
		if(matchUrl(email,&listids))
			return CONFIRMED;

    //3.网关级处理
    memset(command1,0,sizeof(command1));
    sprintf(command1, "SELECT id FROM strategy_urllist WHERE owner = '%s' AND level = 2 AND direction = %d","GLOBAL",direction);
    listids = sql_query(command1);
    if(listids!=NULL)
		if(matchUrl(email,&listids))
			return CONFIRMED;

    return OK;
}
