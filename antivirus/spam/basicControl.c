//
//  basicControl.c
//  ctest2
//
//  Created by tsinghua476 on 16/1/18.
//  Copyright © 2016年 tsinghua476. All rights reserved.
//

#include "basicControl.h"

int startSpamd()
{
    char *args[] = {NULL};
    char *envp[] = {NULL};
    int ret = execve("/usr/sbin/spamd", args, envp);
    if (ret < 0)
    {
        perror("exec error");
        return -1;
    }
    printf("success\n");
    return 0;
}

int closeSpamd()
{
    pid_t spamdPID = getPidNumberByName("spamd");
    int ret = kill(spamdPID, SIGINT);
    return ret;
}


