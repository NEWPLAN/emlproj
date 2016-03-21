/*************************************************************************
	> File Name: spamlist.h
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 13时01分57秒
 ************************************************************************/

#ifndef DB__SPAMLIST_H
#define DB__SPAMLIST_H

typedef struct SPAMLIST
{
    char strategy_id[10+1];
    char strategy_type[10+1];
    char strategy_level[5+1];
    char strategy_info[255+1];
    char strategy_terminal[5+1];
    char strategy_target[255+1];
} spamlist, *spamlistPtr;

spamlistPtr load_spamlist(void* A, void* B);
void print_spamlist(void);
void spamlistRelase(spamlistPtr* tables);
int SpamListTest(int flags);
#endif
