//
//  antivirus.h
//  cTEST
//
//  Created by tsinghua476 on 16/4/5.
//  Copyright © 2016年 tsinghua476. All rights reserved.
//

#ifndef antivirus_h
#define antivirus_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<unistd.h>

#define BUFFSIZE 1024
#define DEFAULTCOMMANDPATH "/usr/local/clamav/bin"
#define STARTCOMMANDPATH "/usr/local/clamav/sbin"

int antivirus(const char *path); //检测病毒  返回0代表不是  返回1代表是病毒
void startClamd(); //开启防毒服务器 默认设置开机自启动即可，并不需要调用
void updateDB(); //保证网络情况良好，并且clamav病毒库文件夹及里面文件的权限可修改



struct antivirusInfo antiVirus(const char *filePath, int flags);

void updateVirusDatabase();

void autoUpdate();//默认一天一次

int AntiVirusInit(void);






#endif /* antivirus_h */
