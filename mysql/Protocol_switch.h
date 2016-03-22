/*************************************************************************
	> File Name: protocol.h
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 13时00分03秒
 ************************************************************************/

#ifndef DB__PROTOCOL_H
#define DB__PROTOCOL_H

typedef struct PROTOCOL
{
    char protocol[255+1];
    char direction[255+1];
    char switchs[10+1];
} protocol, *protocolPtr;

protocolPtr load_protocolSwitch(void* A, void* B);
void printProtocolSwitch(void);
void protocolRelase(protocolPtr* tables);
int ProtocolSwitchTest(int flags);
#endif

