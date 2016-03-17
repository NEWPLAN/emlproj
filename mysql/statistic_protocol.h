/*************************************************************************
	> File Name: statistic_protocol.h
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 13时17分12秒
 ************************************************************************/

#ifndef DB__STATISTIC_PROTOCOL_H
#define DB__STATISTIC_PROTOCOL_H

typedef struct STATISTICPROTOCOL
{
    char protocol[255+1];
    char source_ip[255+1];
    char dest_ip[255+1];
    char open_time[255+1];
    char close_time[255+1];
    char filepath[255+1];
} staprotocol, *staprotocolPtr;

staprotocolPtr load_statisticProtocol(void* A, void* B);
void print_staprotocol(void);
void staprotocolRelase(staprotocolPtr* tables);

#endif

