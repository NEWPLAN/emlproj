/*************************************************************************
	> File Name: statistic_keyclass_basic.h
	> Author: 
	> Mail: 
	> Created Time: 2016��03��15�� ���ڶ� 16ʱ44��31��
 ************************************************************************/

#ifndef DB__STATISTIC_KEYCLASS_BASIC_H
#define DB__STATISTIC_KEYCLASS_BASIC_H

typedef struct STATISTICKEYCLASS
{
    char dlp_keywordsclass_id[11+1];
    char dlp_keywordsclass_eml_id[255+1];
    char dlp_keyclass_hitlist[255+1];
}dlpkeyclass,*dlpkeyclassPtr;

int load_dlpkeyclass(void* A, void* B);

#endif
