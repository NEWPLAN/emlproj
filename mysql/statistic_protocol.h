/*************************************************************************
	> File Name: statistic_protocol.h
	> Author: 
	> Mail: 
	> Created Time: 2016年03月15日 星期二 16时51分35秒
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
}staprotocol, *staprotocolPtr;

int load_statisticProtocol(void* A, void* B);

#endif
