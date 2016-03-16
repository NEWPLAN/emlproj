/*************************************************************************
	> File Name: user_info.h
	> Author:
	> Mail:
	> Created Time: 2016��03��15�� ���ڶ� 17ʱ12��02��
 ************************************************************************/

#ifndef DB__USER_INFO_H
#define DB__USER_INFO_H

typedef struct USERINFO
{
    char user_id[255+1];
    char user_type[10+1];
    char user_name[255+1];
    char user_department[255+1];
    char user_tel[255+1];
    char user_passwd[255+1];
    char user_mailbox[255+1];
} userinfo, *userinfoPtr;

int load_user_info(void* A, void* B);
#endif
