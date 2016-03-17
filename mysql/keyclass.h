/*************************************************************************
	> File Name: keyclass.h
	> Author: 
	> Mail: 
	> Created Time: 2016年03月17日 星期四 12时53分04秒
 ************************************************************************/

#ifndef DB__KEYCLASS_H
#define DB__KEYCLASS_H

typedef struct KEYCLASS
{
    char class_id[10+1];
    char class_name[255+1];
    char class_regex[255+1];
} keyclass, *keyclassPtr;

keyclassPtr load_keyclass(void* A, void* B);
void printkeyclass(void);
void keyclassRelase(keyclassPtr* tables);
#endif
