/*************************************************************************
	> File Name: url_list.h
	> Author:
	> Mail:
	> Created Time: 2016年03月15日 星期二 17时07分39秒
 ************************************************************************/

#ifndef DB__URL_LIST_H
#define DB__URL_LIST_H

typedef struct URLLIST
{
    char strategy_id[255+1];
    char strategy_type[255+1];
    char strategy_content[255+1];
    char url_level[255+1];
    char level_info[255+1];
} urllist,*urllistPtr;


int load_url_list(void* A, void* B);
#endif
