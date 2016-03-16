/*************************************************************************
	> File Name: spamlist.h
	> Author:
	> Mail:
	> Created Time: 2016��03��15�� ���ڶ� 16ʱ09��41��
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
