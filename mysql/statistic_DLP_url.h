/*************************************************************************
	> File Name: statistic_DLP_url.h
	> Author: 
	> Mail: 
	> Created Time: 2016年03月15日 星期二 16时21分17秒
 ************************************************************************/

#ifndef DB__STATISTIC_DLP_URL_H
#define DB__STATISTIC_DLP_URL_H

typedef struct STATISTICURL
{
    char dlp_url_id[11+1];
    char dlp_url_eml_id[11+1];
    char dlp_url_hitlist[255+1];
}statisticurl,*statisticurlPtr;

int load_statisticURL(void* A, void* B);

#endif
