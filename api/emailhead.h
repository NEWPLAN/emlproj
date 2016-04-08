/*************************************************************************
	> File Name: emailhead.h
	> Author: 
	> Mail: 
	> Created Time: 2016年04月08日 星期五 14时37分27秒
 ************************************************************************/

#ifndef EML_EMAILHEAD_H
#define EML_EMAILHEAD_H
typedef struct Emails{
    char* filePath;
    char* sender;
    char* receiver;
    char* srcIP;
    char* destip;
    void* Ptr;
    int*  retvals;
}EmailType,*EmailTypePtr;
int Email_InitAll(void);
int Email_FreeAll(void);
int Email_ParseFile(EmailTypePtr);
#endif
