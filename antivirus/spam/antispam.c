//
//  antispam.c
//  ctest2
//
//  Created by tsinghua476 on 16/1/18.
//  Copyright © 2016年 tsinghua476. All rights reserved.
//

#include "antispam.h"

void getrule(char *buf,char *result)//buf指向buf中的rulename第一个位置,从下标为5
{
    while (*buf != ' ')
    {
        *result++ = *buf++;
    }
    *result = '\0';
}

void getDescription(char *buf,char *result)//buf指向buf中的des第一个位置,从下标为28
{
    char *next = buf+1;
    while (*buf != ' ' || *next != ' ')
    {
        if (*buf == '\0' || *buf == '\n' || *buf == ',')  break;
        *result++ = *buf++;
        next++;
    }
    *result = '\0';
}

struct mes detectSpam(const char *path)
{
    struct mes *ret = (struct mes *)malloc(sizeof(struct mes));
    memset(ret,0,sizeof(struct mes));
    char command[100] = "spamc -R < ";
    strcat(command,path);
    /*double score = 0;*/
    int isSpam = 0;
    //FILE * f = fopen(path, "rb");
    FILE *f = popen(command,"r");
    if (f == NULL)
    {
        perror("open failure");
        exit(1);
    }
    char buf[BUFSIZE] = {0};
    int num = 0;
    int line = 0;//存储contents的行数
    char pscore[10] = {0};
    int isContent = 0;//是否到内存区域
    while (fgets(buf, BUFSIZE - 1, f) != NULL)
    {
        num++;
        if (num == 1)
        {
            int i = 0;
            char *p = &buf[0];
            while (*p != '\n' && *p != '/')
            {
                pscore[i++] = *p++;
            }
            double s = atof(pscore);
            if(s < 5.0)
            {
                ret->isSpam = isSpam;
                ret->score = s;
                return *ret;
            }
            else
            {
                ret->isSpam = 1;
                ret->score = s;
            }
        }
        if (buf[1] == '-' && buf[2] == '-')
        {
            isContent = 1;
            continue;
        }
        if (isContent == 1)
        {
            if (buf[0] != ' ' || buf[1] > '9' || buf[1] < '0')
            {
                continue;
            }
            char s[4] = {0};
            //for (int i = 1; i <= 3; i++) {
            //    s[i - 1] = buf[i];
            //}
            int i = 0;
            while(i < 3)
            {
                s[i] = buf[i+1];
                i++;
            }
            double onescore = atof(s);
            if (onescore < 0.5) continue;
            struct content *pcon = &ret->contents[line];
            pcon->score = onescore;
            getrule(&buf[5], &pcon->ruleName[0]);
            getDescription(&buf[28], &pcon->des[0]);
            line++;
            if(line == 20)  break;
        }
        //printf("%s",buf);
    }//end of while
    if (ferror(f))
    {
        printf("error");
        exit(1);
    }
    else
    {
        //printf("reach EOF\n");
        ret->line = line;
        pclose(f);
        return *ret;
    }
}

void printmes(struct mes ret)
{
    char isspam[10] = {0};
    if(ret.isSpam == 1)
        strcpy(isspam, "spam");
    else
        strcpy(isspam, "ham");
    printf("This message is a %s\n",isspam);
    printf("The score is %0.1lf\n",ret.score);
    int j = 0;
    printf("Score   Rule name                 Description\n");
    while(j < ret.line)
    {
        printf("%-8.1lf%-26s%-40s\n",ret.contents[j].score,ret.contents[j].ruleName,ret.contents[j].des);
        j++;
    }
}

