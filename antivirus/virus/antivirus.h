//
//  antivirus.h
//
//  Created by tsinghua476 on 16/1/20.
//  Copyright © 2016年 tsinghua476. All rights reserved.
//

#ifndef antivirus_h
#define antivirus_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <clamav.h>
//#include "clamav.h"

#define NO_ERROR 0
#define OPEN_FILE_ERROR 1
#define INIT_LIBCLAMAV_ERROR 4
#define CREATE_ENGINE_ERROR 8
#define LOAD_DATABASE_ERROR 16
#define DATABASE_INIT_ERROR 32
#define DETECT_VIRUS_ERROR 64

//反病毒函数返回的结构体
struct antivirusInfo
{
    int isVirus;
    int errorInfo;
    long double fileSize;
    char virusInfo[128];
};

struct antivirusInfo antiVirus(const char *filePath, int flags);

void updateVirusDatabase();

void autoUpdate();//默认一天一次

#endif /* antivirus_h */

