//
//  handleconf.h
//  ctest2
//
//  Created by tsinghua476 on 16/1/15.
//  Copyright © 2016年 tsinghua476. All rights reserved.
//

#ifndef handleconf_h
#define handleconf_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int isMatchKey(char *content,char *key);

int deleteKeyFromConf(char *path,char *key);//删除成功返回1，未找到返回0

int addConfAtLast(char *path,char *name);
//如果name不存在，则在文件末尾添加name，path传入的为绝对路径，name为添加的内容

pid_t getPidNumberByName(char *pidname);//通过进程名字获取进程ID，主要针对spamd的

#endif /* handleconf_h */
