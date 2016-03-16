/*************************************************************************
	> File Name: statistic_spam.h
	> Author:
	> Mail:
	> Created Time: 2016��03��15�� ���ڶ� 16ʱ55��38��
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
