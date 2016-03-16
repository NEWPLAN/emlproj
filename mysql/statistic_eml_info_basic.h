/*************************************************************************
	> File Name: statistic_eml_info_basic.h
	> Author:
	> Mail:
	> Created Time: 2016年03月15日 星期二 16时28分15秒
 ************************************************************************/

#ifndef DB__STATISTIC_EML_INFO_BASIC_H
#define DB__STATISTIC_EML_INFO_BASIC_H

typedef struct STATISTICEMLINFO
{
    char eml_id[11+1];
    char eml_name[255+1];
    char eml_to[255+1];
    char eml_from_to[255+1];
    char eml_attachment_name[255+1];
    char eml_attachment_type[255+1];
    char eml_isspam[11+1];
    char eml_dlp_wordhit[11+1];
    char eml_dlp_classhit[11+1];
    char eml_urlhit[11+1];
} emlinfo,*emlinfoPtr;

int load_emlinfo(void* A, void* B);

#endif
