/*************************************************************************
	> File Name: statistic_virus.h
	> Author:
	> Mail:
	> Created Time: 2016��03��15�� ���ڶ� 16ʱ59��58��
 ************************************************************************/

#ifndef DB__STATISTIC_VIRUS_H
#define DB__STATISTIC_VIRUS_H

typedef struct STATISTICVIRUS
{
    char virus_id[11+1];
    char virus_eml_id[11+1];
    char virus_filename[255+1];
    char virus_info[255+1];
} statvirus, *statvirusPtr;

int load_statistic_virus(void* A, void* B);

#endif
