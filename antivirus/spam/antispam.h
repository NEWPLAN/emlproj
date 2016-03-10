//
//  antispam.h
//  ctest2
//
//  Created by tsinghua476 on 16/1/18.
//  Copyright © 2016年 tsinghua476. All rights reserved.
//

#ifndef antispam_h
#define antispam_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mystruct.h"
#include "handleWhiteBlackList.h"
#include "basicControl.h"


# define BUFSIZE 100

struct mes detectSpam(const char *path);//根据垃圾邮件路径返回对应结果的结构体

void printmes(struct mes ret);//打印返回结果

#endif /* antispam_h */
