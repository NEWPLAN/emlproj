/*************************************************************************
	> File Name: all.h
	> Author:
	> Mail:
	> Created Time: 2016年03月30日 星期三 14时20分18秒
 ************************************************************************/

#ifndef SQLS_ALL_H
#define SQLS_ALL_H
typedef enum B
{
    NO,CONFIRMED,OK
} CheckType;
typedef enum A
{
    IGNORE,BLOCK,TRASH,NONE
} strategyType;

#include "mainall.h"
typedef struct MIMEDATASTRUCT
{
    char* filepath;
    char* workspace;
    GmimeDataPtr mimedata;
    char* sender;
    char* receiver;
    char* srcIP;
    char* destIP;
    char* protocol;
} mimeType,*mimePtr;

CheckType spamCheck(mimePtr email, char* owner, int direction);
CheckType virusCheck(mimePtr email, char* owner, int direction);
CheckType keywordCheck(mimePtr email,char* owner,int direction);
CheckType keywordClassCheck(mimePtr email,char* owner,int direction);
CheckType urlCheck(mimePtr email,char* owner,int direction);
#endif
