//
//  handleconf.c
//  ctest2
//
//  Created by tsinghua476 on 16/1/15.
//  Copyright © 2016年 tsinghua476. All rights reserved.
//

#include "handleconf.h"

int isMatchKey(char *content,char *key)
//如果存在并且没有被注释掉则返回1，存在并被注释掉了返回2，不存在返回0
{
    char compare[50] = {0};
    size_t lenght = strlen(key);
    strncpy(compare, content, lenght);
    if (strcmp(compare, key) == 0)
    {
        return 1;
    }
    if (content[0] != '#')
    {
        return 0;
    }
    char *p = content;
    memset(compare, 0, 50);
    p = p + 2;
    strncpy(compare, p, lenght);
    if (strcmp(compare, key) == 0)
    {
        return 2;
    }
    return 0;
}

int deleteKeyFromConf(char *path,char *key)//删除成功返回1，未找到返回0
{
    FILE *fconf = fopen(path, "rb+");
    if (fconf == NULL)
    {
        perror("open file failure");
        exit(1);
    }
    int isIn = 0;
    char rbuf[128] = {0};
    while (fgets(rbuf, 127, fconf) != NULL)
    {
        if (isMatchKey(rbuf, key) != 1)
            continue;
        else   //找到以后删除掉编程空格
        {
            char space = ' ';
            char *p = &space;
            fseek(fconf, -strlen(key)-1, SEEK_CUR);
            int i = 0;
            while (i <= strlen(key))
            {
                fwrite(p, 1, 1, fconf);
                i++;
            }
            isIn = 1;
            break;
        }
    }
    fclose(fconf);
    return isIn;
}

int addConfAtLast(char *path,char *name)
//如果name不存在，则在文件末尾添加name，path传入的为绝对路径，name为添加的内容
{

    FILE  *fconf = fopen(path, "rb+");
    if (fconf == NULL)
    {
        perror("open file failure");
        exit(1);
    }
    char buf[128] = {0};
    //int isIn = 0;
    while (fgets(buf, 127, fconf) != NULL)   //判断是否存在name
    {
        if (isMatchKey(buf, name) == 1)
        {
            fclose(fconf);
            return 1;
        }
    }
    if (ferror(fconf))
    {
        perror("reading error");
        return 0;
    }
    memset(buf, 0, strlen(buf));
    buf[0] = '\n';
    strcat(buf,name);
    fwrite(buf, 1, strlen(buf), fconf);
    fclose(fconf);
    return 1;
}

pid_t getPidNumberByName(char *pidname)
{
    FILE *pstr;
    char cmd[128],buff[512],tokbuf[512],*p;
    pid_t pID = 1;
    int pidnum = 0;
    int line = 0;
    memset(cmd,0,sizeof(cmd));
    memset(tokbuf, 0, sizeof(tokbuf));
    memset(buff,0,sizeof(buff));
    sprintf(cmd, "ps -ef|grep %s ",pidname);
    pstr=popen(cmd, "r");
    //pstr = fopen("/Users/tsinghua476/Desktop/light/ctest2/ctest2/test", "r");
    if(pstr==NULL)
    {
        perror("open failure");
        return 1;
    }
    while (fgets(buff,512,pstr) != NULL)
    {
        if (line == 0)
        {
            strcpy(tokbuf, buff);
            p=strtok(tokbuf, " ");
            p=strtok(NULL, " "); //这句是否去掉，取决于当前系统中ps后，进程ID号是否是第一个字段
            //在ubuntu中需要这句话的存在，因为进程ID在第二个字段
            if(p==NULL)
            {
                return 1;
            }
            if(strlen(p)==0)
            {
                return 1;
            }
            if((pidnum=atoi(p))==0)
            {
                return 1;
            }
            pID = (int)pidnum;
        }
        line++;
    }
    if (ferror(pstr))
    {
        pclose(pstr);
        perror("read error");
        return 1;
    }
    if (line > 3)
    {
        pclose(pstr);
        return pID;
    }
    return 1;
}
