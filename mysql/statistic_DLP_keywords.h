/*************************************************************************
	> File Name: statics_DLP_keywords.h
	> Author: 
	> Mail: 
	> Created Time: 2016��03��15�� ���ڶ� 16ʱ15��07��
 ************************************************************************/

#ifndef DB__STATICS_DLP_KEYWORDS_H
#define DB__STATICS_DLP_KEYWORDS_H

typedef struct STATISTICKEYWORDS
{
    char dlp_keywords_id[11+1];
    char dlp_keywords_eml_id[11+1];
    char dlp_keywords_hitlist[255+1];
}statisticwords,*statisticwordsPtr;

int load_statistic_DLP_keywords(void* A, void* B);

#endif
