/*************************************************************************
	> File Name: keywords.h
	> Author:
	> Mail:
	> Created Time: 2016年03月15日 星期二 15时33分00秒
 ************************************************************************/

#ifndef DB__KEYWORDS_H
#define DB__KEYWORDS_H
typedef struct KEYWORDS
{
    char keyword[255+1];
} keywords, *keywordsPtr;

int load_keywords(void* A, void* B);
#endif
