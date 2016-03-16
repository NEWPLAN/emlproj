/*************************************************************************
	> File Name: behaviorLog.h
	> Author:
	> Mail:
	> Created Time: 2016年03月15日 星期二 15时01分15秒
 ************************************************************************/

#ifndef DB__BEHAVIORLOG_H
#define DB__BEHAVIORLOG_H

typedef struct behavoirStruct
{
    char time[128];
    char level[255+1];
    char user_id[11+1];
    char object[255+1];
    char behavior[255+1];
} behaviorlog,*behaviorlogPtr;

int loadbehavoirLog(void* A,void *B);

#endif
