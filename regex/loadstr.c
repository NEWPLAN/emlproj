/*************************************************************************
	> File Name: loadstr.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月03日 星期四 13时53分53秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include "loadstr.h"
#define ADDTIONBYTE 20
char* txtstr=NULL;
int LoadTxtPage(char* filepath)
{
    int NByte=0;
    FILE *filptr=fopen(filepath,"rb");
    if(!filptr)
    {
        printf("error in open file %s\n",filepath);
        return -1;
    }
    fseek(filptr,0,SEEK_END);
    NByte=ftell(filptr);
    if(txtstr==NULL)
    {
        txtstr=(char*)malloc(NByte+ADDTIONBYTE);
        if(txtstr)
        {
            memset(txtstr,0,ADDTIONBYTE+NByte);
        }else
        {
            printf("can't malloc such huge page for loading txt\n");
            fclose(filptr);
            return -1;
        }
        fseek(filptr,0,SEEK_SET);
        fread(txtstr,1,NByte+ADDTIONBYTE-1,filptr);
    }
    fclose(filptr);
    return 0;
}

int RelasePage(void)
{
    if(txtstr)
        free(txtstr);
    txtstr=NULL;
    return 0;
}
