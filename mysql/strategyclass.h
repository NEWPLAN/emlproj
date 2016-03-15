/*************************************************************************
	> File Name: keywords.h
	> Author: 
	> Mail: 
	> Created Time: 2016��03��15�� ���ڶ� 14ʱ14��30��
 ************************************************************************/

#ifndef DB_STRATEGYCLASS_H
#define DB_STRATEGYCLASS_H
typedef struct STRATEGYCLASS
{
    char strategy_id[10+1];
    char strategy_type[10+1];
    char strategy_level[5+1];
    char strategy_info[255+1];
    char strategy_terminal[5+1];
    char strategy_target[255+1];
    char strategy_content[255+1];
}strategyclass,*strategyclassPtr;

int load_strategyclass(void *A, void* B);
#endif
