/*************************************************************************
	> File Name: emailbody.c
	> Author:
	> Mail:
	> Created Time: 2016年04月08日 星期五 14时41分36秒
 ************************************************************************/

#include "emailhead.h"
#include "../sql2/all.h"
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

extern mimePtr mimeCy;

int Email_ParseFile(EmailTypePtr parasPtr)
{
	int rtevals=0;
	if(parasPtr)
	{
		if(parasPtr->filepath)
		{
			char runningFiles[1024]= {0};
            char newpath_temps[1024]= {0};
            char newpath_appendix[1024]= {0};
            char command[1024]= {0};
            sprintf(runningFiles,"runningFiles_%d",getpid());
            sprintf(newpath_temps,"%s/temps",runningFiles);
            sprintf(newpath_appendix,"%s/appendix",runningFiles);
            if(mkdir(runningFiles, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)!=0)/*!success*/
                goto exits;
            if(mkdir(newpath_temps, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)!=0)/*!success*/
                goto exits;
            if(mkdir(newpath_appendix, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)!=0)/*!success*/
                goto exits;

            mimeCy->workspace=runningFiles;
            mimeCy->filepath=parasPtr->filePath;

            rtevals=ParseAEmail(parasPtr->filePath,runningFiles);
exits:
            sprintf(command,"rm -rf %s",runningFiles);
            usleep(1);
            	if(fork()==0)
            		execlp("rm","rm","-rf",runningFiles,NULL);
            wait(NULL);
            if(rtevals==0)
            	return SCANNER_RET_OK;
           	else
           		return SCANNER_RET_VIRUS;
		}
	}
	return SCANNER_RET_OK;
}
