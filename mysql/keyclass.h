/*************************************************************************
	> File Name: keyclass.h
	> Author: 
	> Mail: 
	> Created Time: 2016��03��15�� ���ڶ� 15ʱ38��58��
 ************************************************************************/

#ifndef DB__KEYCLASS_H
#define DB__KEYCLASS_H

typedef struct KEYCLASS
{
    char class_id[10+1];
    char class_name[255+1];
    char class_regex[255+1];
}keyclass, *keyclassPtr;

int load_keyclass(void* A, void* B);
#endif
