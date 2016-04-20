//
//  uncompress.c
//  cTEST
//
//  Created by tsinghua476 on 16/3/9.
//  Copyright © 2016年 tsinghua476. All rights reserved.
//

#include "uncompress.h"
#include <string.h>
int whichKindOfCompressedFile(char *compressedFile)
//0->不是压缩文件   1 ->rar  2->zip  3->tar.gz 4->tar 5->tar.bz2
{
    char *supportsKindOfCompress[] = {"rar","zip","gz","tar","bz2"};
    int NumberOfType = sizeof(supportsKindOfCompress)/sizeof(supportsKindOfCompress[0]);
    char *p = NULL;
    int isCompressed = 0;
    p = GetSuffix(compressedFile);
    if (p)
    {
        int i = 0;
        while (i < NumberOfType)
        {
            if (!strcmp(supportsKindOfCompress[i], p))
            {
                isCompressed = i + 1;
                break;
            }
            i++;
        }
    }
    if (isCompressed == 0)
    {
        return 0;
    }
    return isCompressed;
}

int uncompress(char *compressedFile)//需要传入一个带有绝对路径的压缩文件
{
    char *commandPool[] = {"rar e -y -inul ","unzip -j -q ","tar -xzf ","tar -xf ","tar -xjf "};
    char command[256] = {0};
    int flag = whichKindOfCompressedFile(compressedFile);
    if (flag == 0)
    {
        return 1;//源文件不是压缩文件
    }
    strncpy(command, commandPool[flag-1], strlen(commandPool[flag-1]));

    char oldPath[128]= {0};
    char exePath[256]= {0};
    char curPath[128]= {0};
    char tempFile[10] = "temp";
    getcwd(oldPath, sizeof(oldPath));
    strcpy(curPath, oldPath);
    strcat(curPath, "/temp");
    mkdir(tempFile, 0777);
    chdir(tempFile);
    sprintf(exePath, "%s/%s",oldPath,compressedFile);
    strcat(command, exePath);
    if(flag == 1 )
    {
        strcat(command, " ./");
    }
    system(command);

    struct dirent *file;
    DIR *d;
    if(!(d=opendir(curPath)))
    {
        printf("error in open dir : %s\n",curPath);
        return -1;
    }
    while ((file = readdir(d)) != NULL)
    {
        if(strncmp(file->d_name,".",1)==0)
            continue;
        int tempFlag = whichKindOfCompressedFile(file->d_name);
        if (tempFlag > 0)
        {
            uncompress(file->d_name);
        }
        else
        {
        	char papapa[1024]={0};
        	getcwd(papapa,sizeof(papapa));
            printf("decode this file with name :%s/%s\n",papapa,file->d_name);
        }
    }
    chdir(oldPath);
    closedir(d);
    return EXIT_SUCCESS;
}

