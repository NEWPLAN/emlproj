/*************************************************************************
	> File Name: virus_list.h
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 13时06分08秒
 ************************************************************************/

#ifndef DB__VIRUS_LIST_H
#define DB__VIRUS_LIST_H

typedef struct VIRUSLIST
{
    char strategy_id[10+1];
    char strategy_type[10+1];
    char strategy_level[5+1];
    char strategy_info[255+1];
    char strategy_terminal[5+1];
    char strategy_target[255+1];
} viruslist,*viurlistPtr;

viurlistPtr load_virus_list(void* A, void* B);
void printvirus_list(void);
void virus_listRelase(viurlistPtr* tables);

#endif

