/*************************************************************************
	> File Name: user_info.h
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 13时08分22秒
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

userinfoPtr load_user_info(void* A, void* B);
void print_userinfo(void);
void userinfoRelase(userinfoPtr* tables);
#endif

