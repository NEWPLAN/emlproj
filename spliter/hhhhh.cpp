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

using namespace std;


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
	
	NLPIR_FileProcess("temps/txt.txt","temps/txt",0);
	NLPIR_FileProcess("temps/pdf.txt","temps/pdf",0);
	NLPIR_FileProcess("temps/test.email.txt","temps/eml",0);
    NLPIR_Exit();

        
    return 0;
}
