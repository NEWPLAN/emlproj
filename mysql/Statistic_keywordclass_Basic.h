/*************************************************************************
	> File Name: statistic_keyclass_basic.h
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 13时18分28秒
 ************************************************************************/

#ifndef DB__STATISTIC_KEYCLASS_BASIC_H
#define DB__STATISTIC_KEYCLASS_BASIC_H

typedef struct STATISTICKEYCLASS
{
    char DLP_keywordclass_id[11+1];
    char DLP_keywordclass_eml_id[255+1];
    char DLP_keywordclass_hitlist[255+1];
} dlpkeyclass,*dlpkeyclassPtr;

dlpkeyclassPtr load_dlpkeyclass(void* A, void* B);
void print_dlpkeyclass(void);
void dlpkeyclassRelase(dlpkeyclassPtr* tables);
int StatisticKeyClassBasicTest(int flags);
#endif
