/*************************************************************************
	> File Name: protocol.h
	> Author:
	> Mail:
	> Created Time: 2016年03月15日 星期二 16时05分02秒
 ************************************************************************/

#ifndef DB__PROTOCOL_H
#define DB__PROTOCOL_H

typedef struct PROTOCOL
{
    char protocol[255+1];
    char direction[255+1];
    char switchs[10+1];
} ptotocol, *protocolPtr;

int load_protocolSitch(void* A, void* B);

#endif
