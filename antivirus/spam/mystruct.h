//
//  mystruct.h
//  ctest2
//
//  Created by tsinghua476 on 16/1/18.
//  Copyright © 2016年 tsinghua476. All rights reserved.
//

#ifndef mystruct_h
#define mystruct_h

struct content
{
    double score;
    char ruleName[30];
    char des[80];//description
};

struct mes
{
    int isSpam;
    double score;
    int line;//desciption有多少行
    struct content contents[20];
};

#endif /* mystruct_h */
