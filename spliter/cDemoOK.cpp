/*******************************************************************************
@All Right Reserved (C), NEWPLAN
Filepath   : /cDemoOK.cpp
Filename   : cDemoOK.cpp
Version    : ver 1.0
Author     : newplan001@163.com
Date       : 2015/12/22
Description: test for ICTCLAS LIBS
Notification:apply for Linux
History    :
1.2015/12/22 14:04 Created by newplan@163.com Version 1.0
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include  <string>
#include "NLPIR.h"

#include "cDemoOK.h"
#include "loadstr.h"

extern char * txtstr;
extern char* workspace;
extern char* dicpath;
char* testImportUserDict(int flags, int *NumPtr, char* filename)
{
    //初始化分词组件。
    //必须调用此接口后，才能调用其它接口！


    /*if(LoadTxtPage(filename)<0)
    {
        printf("error in load test file\n");
        return NULL;
    }
    char* sSentence=(char*)"随后温总理就离开了舟曲县城，预计温总理今天下午就回到北京。以上就是今天上午的最新动态。我是程序员 我喜欢编程， 你信不信？";
    if(txtstr)
        sSentence=txtstr;
    printf("%s\n",txtstr);
    int nPaLen=strlen(sSentence);*/


    /*int nRstLen=0;*/
    //char* sRst=0;//用户自行分配空间，用于保存结果；


    //未导入用户字典前的处理
    ///sRst=(char*)NLPIR_ParagraphProcess(sSentence,0);
    //printf("--------添加用户字典前分词结果--------\n%s\n\n",sRst);
    //free(sRst);
#if __DEBUG
    printf("---------------添加词典---------------\n");
#endif
    const char* pszDictBuffer=dicpath; //字符串形式导入用户词典
    /*int nLen=strlen(pszDictBuffer);*/
    unsigned int nItems=NLPIR_ImportUserDict(pszDictBuffer);
    *NumPtr = (int)nItems;
    //也可以将用户词汇写入文件，词语之间用换行符隔开
    //unsigned int nItems=NLPIR_ImportUserDictFile("userdict.txt",CODE_TYPE_UNKNOWN);
#if __DEBUG
    printf("\n添加了%d 个用户词\n\n",nItems); //文件形式导入用户字典
#endif
/*
    char* sRst1=0;
    sRst1=(char *)malloc(nPaLen*6);
    sRst1=(char*)NLPIR_ParagraphProcess(sSentence,0);//导入用户字典后分词处理
    printf("---------添加用户字典后分词结果-------\n%s\n",sRst1);

    NLPIR_SaveTheUsrDic(); //保存添加的用户词汇,下次使用依然有效
    int NumPt=strlen(sRst1);
    char* pt=(char*)malloc(NumPt+5);
    memset(pt,0,NumPt+5);
    strncpy(pt,sRst1,NumPt);
*/
	char tempth[1024]={0};

	sprintf(tempth,"%s/%s",workspace,"qqqqqqqqqq");
    NLPIR_FileProcess(filename,tempth,0);
    if(LoadTxtPage(tempth)<0)
    {
        printf("error in load test file\n");
        return NULL;
    }
#if __DEBUG
    printf("分词结果:\n%s\n",txtstr);
#endif
    //释放资源退出
    //NLPIR_Exit();
    return txtstr;
}
