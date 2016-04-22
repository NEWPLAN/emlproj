/*************************************************************************
	> File Name: emailbody.c
	> Author:
	> Mail:
	> Created Time: 2016年04月08日 星期五 14时41分36秒
 ************************************************************************/

#include "emailhead.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "../sql2/all.h"
#include "assert.h"
#include <sys/wait.h>

extern mimePtr mimeCy;

int Email_InitAll(void)
{
	mimeCy=(mimePtr)malloc(sizeof(mimeType));
    assert(mimeCy!=NULL);
    memset(mimeCy,0,sizeof(mimeType));

    if(SpliterInit())
        printf("init spliter successfully\n");
    else
    {
        printf("init spliter failed");
        return SCANNER_RET_ERR;
    }
    return 1;
}
int Email_FreeAll(void)
{
	return 1;
}
#define __clear
int Email_ParseFile(EmailTypePtr parasPtr)
{
	int rtevals=0;
	if(parasPtr)
	{
		if(parasPtr->filePath)
		{
			char runningFiles[1024]= {0};
            char newpath_temps[1024]= {0};
            char newpath_appendix[1024]= {0};
            char command[1024]= {0};
            EmailTypePtr eeeee=(EmailTypePtr)malloc(sizeof(EmailType));
            assert(eeeee!=NULL);
            eeeee->ipto=parasPtr->ipto;
            eeeee->ipfrom=parasPtr->ipfrom;
            eeeee->protocol=parasPtr->protocol;
            
            sprintf(runningFiles,"runningFiles_%d",getpid());
            sprintf(newpath_temps,"%s/temps",runningFiles);
            sprintf(newpath_appendix,"%s/appendix",runningFiles);
            if(mkdir(runningFiles, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)!=0)/*!success*/
                goto exits;
            if(mkdir(newpath_temps, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)!=0)/*!success*/
                goto exits;
            if(mkdir(newpath_appendix, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)!=0)/*!success*/
                goto exits;
            rtevals=ParseAEmail(parasPtr->filePath,runningFiles,eeeee);
            free(eeeee);
exits:
            sprintf(command,"rm -rf %s",runningFiles);
            usleep(1);
#ifdef __clear
            if(fork()==0)
            	execlp("rm","rm","-rf",runningFiles,NULL);
            wait(NULL);
#endif            
            printf("检查结果是 %d,即将退出\n",rtevals);
            if(rtevals==0)
            	return SCANNER_RET_OK;
           	else
           		return SCANNER_RET_VIRUS;
		}
	}
	return SCANNER_RET_OK;
}
