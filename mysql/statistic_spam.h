/*************************************************************************
	> File Name: statistic_spam.h
	> Author:
	> Mail:
	> Created Time: 2016年03月15日 星期二 16时55分38秒
 ************************************************************************/

#ifndef DB__STATISTIC_SPAM_H
#define DB__STATISTIC_SPAM_H

typedef struct STATISTICSPAM
{
    char spam_id[10+1];
    char spam_eml_path[255+1];
    char spam_eml_score[255+1];
    char spam_eml_info[255+1];
    char eml_eml_id[11+1];
} statspam, *statspamPtr;

int load_statistic_spam(void* A, void* B);

#endif
