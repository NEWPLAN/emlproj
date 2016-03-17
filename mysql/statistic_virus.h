/*************************************************************************
	> File Name: statistic_virus.h
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 13时12分46秒
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

statvirusPtr load_statistic_virus(void* A, void* B);
void print_statvirus(void);
void statvirusRelase(statvirusPtr* tables);

#endif

