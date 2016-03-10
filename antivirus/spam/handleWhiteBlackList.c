//
//  handleWhiteBlackList.c
//  ctest2
//
//  Created by tsinghua476 on 16/1/15.
//  Copyright © 2016年 tsinghua476. All rights reserved.
//

#include "handleWhiteBlackList.h"

int addWhiteList(char *name)
{
    char temp[128] = "whitelist_from ";
    strcat(temp, name);
    return addConfAtLast(MAINCONFPATH, temp);
}

int addBlackList(char *name)
{
    char temp[128] = "blacklist_from ";
    strcat(temp, name);
    return addConfAtLast(MAINCONFPATH, temp);
}

int deleteWhiteList(char *name)
{
    char temp[128] = "whitelist_from ";
    strcat(temp, name);
    return deleteKeyFromConf(MAINCONFPATH, temp);
}

int deleteBlackList(char *name)
{
    char temp[128] = "blacklist_from ";
    strcat(temp, name);
    return deleteKeyFromConf(MAINCONFPATH, temp);
}

int saveBlackWhiteListConf()
{
    pid_t pid = getPidNumberByName("spamd");
    if (pid == 1)
    {
        printf("spamd守护进程未启动");
        return 0;
    }
    kill(pid, SIGHUP);
    return 1;
}
