/*************************************************************************
	> File Name: url_list.h
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 13时09分40秒
 ************************************************************************/

#ifndef DB__URL_LIST_H
#define DB__URL_LIST_H

typedef struct URLLIST
{
    char strategy_id[10+1];
    char strategy_type[255+1];
    char strategy_content[255+1];
    char URL_level[255+1];
    char level_info[255+1];
} urllist,*urllistPtr;


urllistPtr load_url_list(void* A, void* B);
void print_urllist(void);
void urllistRelase(urllistPtr* tables);
int UrlListTest(int flags);
#endif
