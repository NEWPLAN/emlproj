/*************************************************************************
	> File Name: spamlist.h
	> Author:
	> Mail:
	> Created Time: 2016年03月15日 星期二 16时09分41秒
 ************************************************************************/

#ifndef DB__SPAMLIST_H
#define DB__SPAMLIST_H

typedef struct SPAMLIST
{
    char strategy_id[10+1];
    char strategy_type[10+1];
    char strategy_info[255+1];
    char strategy_terminal[5+1];
    char strategy_target[255+1];
} spamlist, *spanlistPtr;

int load_spamlist(void* A, void* B);
#endif
