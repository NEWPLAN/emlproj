/*************************************************************************
	> File Name: keywords.h
	> Author:
	> Mail:
	> Created Time: 2016��03��15�� ���ڶ� 15ʱ33��00��
 ************************************************************************/

#ifndef DB__KEYWORDS_H
#define DB__KEYWORDS_H
typedef struct KEYWORDS
{
    char keyword[255+1];
} keywords, *keywordsPtr;

int load_keywords(void* A, void* B);
#endif
