/*************************************************************************
    > File Name: emailhead.h
    > Author:
    > Mail:
    > Created Time: 2016年04月08日 星期五 14时37分27秒
 ************************************************************************/


#ifndef EML_EMAILHEAD_H
#define EML_EMAILHEAD_H

#define SCANNER_RET_OK        0
#define SCANNER_RET_ERR       -1
#define SCANNER_RET_VIRUS     2
#define SCANNER_RET_CRIT      3
#include <netinet/in.h>　
typedef struct Emails
{
    char* filePath;
    char* sender;
    char* receiver;

    struct sockaddr_in srcIP;
    struct sockaddr_in destip;
    void* Ptr;
    int*  retvals;
} EmailType, *EmailTypePtr;
int ParseAEmail(char*filepath,char*workpath);
int Email_InitAll(void);
int Email_FreeAll(void);
int Email_ParseFile(EmailTypePtr);
#endif
