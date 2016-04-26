/*************************************************************************
	> File Name: all.h
	> Author:
	> Mail:
	> Created Time: 2016��02��22�� ����һ 16ʱ33��28��
 ************************************************************************/

#ifndef REGEX_ALL_H
#define REGEX_ALL_H
#include "email.h"
#include "idCard.h"
#include "mobile.h"
#include "url.h"
#include "bankCard.h"
#include "loadstr.h"
#include "ipnum.h"

#ifdef EML__SYSTEMS__
    int RegexMain(int argc, char* argv[]);
    int checkInclass(char * file, char* pattern);
#endif

#endif
