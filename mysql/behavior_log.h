/*************************************************************************
	> File Name: behaviorLog.h
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 12时50分47秒
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

behaviorlogPtr load_behaviorLog(void* A, void* B);
void print_behaviorlog(void);
void behaviorlogRelase(behaviorlogPtr* tables);
int BehaviorLogTest(int flags);
#endif
