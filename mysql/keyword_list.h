/*************************************************************************
	> File Name: keywords.h
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 12时55分53秒
 ************************************************************************/

#ifndef DB__KEYWORDS_H
#define DB__KEYWORDS_H
typedef struct KEYWORDS
{
    char keyword[255+1];
} keywords, *keywordsPtr;

keywordsPtr load_keywords(void* A, void* B);
void print_keywords(void);
void keywordsRelase(keywordsPtr* tables);
int KeyWordsTest(int flags);
#endif
