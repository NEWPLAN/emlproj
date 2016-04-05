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
	NONE,CONFIRMED,OK
}CheckType;
typedef enum A
{
	IGNORE,BLOCK,TRASH,NONE
}strategyType;
CheckType keywordCheck(char* email,char* owner,int direction);
CheckType spamCheck(char* email,char* owner,int direction);
#endif
