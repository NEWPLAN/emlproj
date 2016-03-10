//
//  handleWhiteBlackList.h
//  ctest2
//
//  Created by tsinghua476 on 16/1/15.
//  Copyright © 2016年 tsinghua476. All rights reserved.
//

#ifndef handleWhiteBlackList_h
#define handleWhiteBlackList_h

#include <stdio.h>
#include "handleconf.h"
#define MAINCONFPATH "/etc/mail/spamassassin/local.cf"
//#define MAINCONFPATH "/Users/tsinghua476/Desktop/light/ctest2/ctest2/conf"
int addWhiteList(char *name);//添加白名单 成功返回1 失败返回0

int deleteWhiteList(char *name);//添加白名单 存在并删除成功返回1 不存在返回0

int addBlackList(char *name);

int deleteBlackList(char *name);

int saveBlackWhiteListConf();  //保存配置，成功返回1，失败返回0

#endif /* handleWhiteBlackList_h */
