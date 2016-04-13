//
//  uncompress.h
//  cTEST
//
//  Created by tsinghua476 on 16/3/9.
//  Copyright © 2016年 tsinghua476. All rights reserved.
//

#ifndef uncompress_h
#define uncompress_h

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include "getsubstr.h"

int uncompress(char *compressedFile);

#endif /* uncompress_h */

