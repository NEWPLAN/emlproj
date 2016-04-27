/*************************************************************************
	> File Name: test.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年03月30日 星期三 15时58分52秒
 ************************************************************************/

#include<iostream>
#include  <string>
#include "NLPIR.h"
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include "cDemoOK.h"
#include <sys/types.h>
#include <stdio.h>
#include <sys/time.h>

using namespace std;

#define __SHOW_TIMES

int main(int argc , char* argv[])
{
    if(!NLPIR_Init(0,1))
    {
        printf("NLPIR INIT FAILED!\n");  //初始化失败，退出。
        return 0;
    }
    else
    {
        printf("------------Init ok!--------------\n\n");
    }
    
#ifdef __SHOW_TIMES    
    struct timeval tBeginTime, tEndTime;
    float fCostTime;
    gettimeofday(&tBeginTime,NULL);
#endif
   
    //unsigned int nItems=NLPIR_ImportUserDictFile("ddd222",CODE_TYPE_UNKNOWN);
    //unsigned int nItems=NLPIR_ImportUserDictFile("ddd222",CODE_TYPE_UNKNOWN);
    unsigned int nItems=NLPIR_ImportUserDict("temps/ddd222");
    printf("inport dicts is %d\n",nItems);
	NLPIR_FileProcess("temps/rrrrrrrrr","temps/ddd",0);	
	//NLPIR_FileProcess("temps/pdf.txt","temps/pdf",0);
	//NLPIR_FileProcess("temps/test.email.txt","temps/eml",0);
	//NLPIR_SaveTheUsrDic(); //保存添加的用户词汇,下次使用依然有效
#ifdef __SHOW_TIMES	
	gettimeofday(&tEndTime,NULL);
    fCostTime = 1000000*(tEndTime.tv_sec-tBeginTime.tv_sec)+(tEndTime.tv_usec-tBeginTime.tv_usec);
    fCostTime /= 1000000;
    printf("the execute time  is = %f(Second)\n",fCostTime);
#endif	
    NLPIR_Exit();

        
    return 0;
}
