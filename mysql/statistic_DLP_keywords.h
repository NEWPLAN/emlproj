/*************************************************************************
	> File Name: statistic_DLP_keywords.h
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 13时22分45秒
 ************************************************************************/

#ifndef DB__STATICS_DLP_KEYWORDS_H
#define DB__STATICS_DLP_KEYWORDS_H

typedef struct STATISTICKEYWORDS
{
    char dlp_keywords_id[11+1];
    char dlp_keywords_eml_id[11+1];
    char dlp_keywords_hitlist[255+1];
} statisticwords,*statisticwordsPtr;

statisticwordsPtr load_statistic_DLP_keywords(void* A, void* B);
void printStatistic_DLP_keywords(void);
void Statistic_DLP_keywordsRelase(statisticwordsPtr* tables);


#endif

