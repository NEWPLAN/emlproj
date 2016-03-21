/*************************************************************************
	> File Name: statistic_eml_info_basic.h
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 13时20分09秒
 ************************************************************************/

#ifndef DB__STATISTIC_EML_INFO_BASIC_H
#define DB__STATISTIC_EML_INFO_BASIC_H

typedef struct STATISTICEMLINFO
{
    char eml_id[11+1];
    char eml_name[255+1];
    char eml_from[255+1];
    char eml_to[255+1];
    char eml_from_to[255+1];
    char eml_attachment_name[255+1];
    char eml_attachment_type[255+1];
    char eml_attachment_virus[255+1];
    char eml_isspam[11+1];
    char eml_DLPkeywordhit[11+1];
    char eml_DLPkeyclasshit[11+1];
    char eml_URLhit[11+1];
} emlinfo,*emlinfoPtr;

emlinfoPtr load_emlinfo(void* A, void* B);
void emlinfoRelase(emlinfoPtr* tables);
void print_emlinfo(void);
int StatisticEmlInfoBasicTest(int flags);
#endif
