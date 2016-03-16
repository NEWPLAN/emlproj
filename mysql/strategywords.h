/*************************************************************************
	> File Name: keywords.h
	> Author:
	> Mail:
	> Created Time: 2016年03月15日 星期二 14时14分30秒
 ************************************************************************/

#ifndef DB_STRATEGYWORDS_H
#define DB_STRATEGYWORDS_H
typedef struct STRATEGYWORDS
{
    char strategy_id[11];
    char strategy_type[10+1];
    char strategy_level[5+1];
    char strategy_info[255+1];
    char strategy_terminal[5+1];
    char strategy_target[255+1];
    char strategy_content[1023+1];
} strategywords,*strategywordsPtr;

int load_strategywords(void* A, void *B);
#endif
