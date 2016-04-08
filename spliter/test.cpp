/*************************************************************************
	> File Name: test.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年03月30日 星期三 15时58分52秒
 ************************************************************************/

#include<iostream>
#include "allhead.h"
#include  <string>
#include "NLPIR.h"
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include "cDemoOK.h"
#include "loadstr.h"
#include <sys/types.h>
#include "sys/wait.h"

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
    char* sSentence=(char*)"随后温总理就离开了舟曲县城，预计温总理今天下午就回到北京。以上就是今天上午的最新动态。我是程序员 我喜欢编程， 你信不信？";
    int fff=20000;
    char* ppppp=(char*)malloc(1024);
    while(fff--)
    {
        if(fork()==0)
        {
        	pid_t ppp=getpid();
#if __DEBUG        	
        	printf("====================current pid is %d\n",ppp);
#endif        	
            /*int nRstLen=0;*/
            char* sRst=0;//用户自行分配空间，用于保存结果；


            //未导入用户字典前的处理
            sRst=(char*)NLPIR_ParagraphProcess(sSentence,0);
#if __DEBUG            
            printf("--------添加用户字典前分词结果--------\n%s\n\n",sRst);
#endif            
            free(sRst);
#if __DEBUG
            printf("---------------添加词典---------------\n");
#endif            
            const char* pszDictBuffer="userdict.txt"; //字符串形式导入用户词典
            /*int nLen=strlen(pszDictBuffer);*/
            unsigned int nItems=NLPIR_ImportUserDict(pszDictBuffer);

            //也可以将用户词汇写入文件，词语之间用换行符隔开
            //unsigned int nItems=NLPIR_ImportUserDictFile("userdict.txt",CODE_TYPE_UNKNOWN);
#if __DEBUG            
            printf("\n添加了%d 个用户词\n\n",nItems); //文件形式导入用户字典
#endif            

            char* sRst1=0;
            sRst1=(char*)NLPIR_ParagraphProcess(sSentence,0);//导入用户字典后分词处理
#if __DEBUG            
            printf("---------添加用户字典后分词结果-------\n%s\n",sRst1);
#endif            

            NLPIR_SaveTheUsrDic(); //保存添加的用户词汇,下次使用依然有效
#if __DEBUG            
            printf("===========%s\n",sRst1);
#endif            
            //NLPIR_Exit();
            free(ppppp);
            return 0;
            
        }
#if __DEBUG        
        printf("done\n");
#endif    
    }
    int sss;
    wait(&sss);
    
    //释放资源退出
    NLPIR_Exit();

        
    return 0;
}
