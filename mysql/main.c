//#include "dboperate.h"
#include "all.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dboperate.h"
/***********************************************
*
*
*测试文件
*
************************************************/
/*
static int TestAll(void);
static DataPtr LoadAll(void);
static int FreeAll(DataPtr A);
*/
//#define MYSQL_LIBS
#ifndef MYSQL_LIBS
int main(int argc, char* argv[])
{
    //printProtocolSwitch();
    //printStatistic_DLP_keywords();
    LoadAll();
    TestAll();
    FreeAll(NULL);
    return 0;
}
#endif

DataPtr LoadAll(void)
{
    DataPtr A=(DataPtr)malloc(sizeof(DataType));
    if(!A)
    {
        printf("error in malloc memory for loading DataPtr\n");
        return NULL;
    }
    {
        A->Behavior_Data=load_behaviorLog(&A->behavior_Num,NULL);
        A->DLP_list_keywords_Data=load_strategywords(&A->DLP_list_keywords_Num,0);
        A->DLP_list_keywordsclass_Data=load_strategyclass(&A->DLP_list_keywordsclass_Num,0);
        A->keyword_list_Data=load_keywords(&A->keyword_list_Num,0);
        A->keywordclass_list_Data=load_keyclass(&A->keywordclass_list_Num,0);
        A->Protocol_switch_Data=load_protocolSwitch(&A->Protocol_switch_Num,0);
        A->spam_list_Data=load_spamlist(&A->spam_list_Num,0);
        A->Statistic_DLP_keywords_Data=load_statistic_DLP_keywords(&A->Statistic_DLP_keywords_Num,0);
        A->Statistic_DLP_URL_Data=load_statisticURL(&A->Statistic_DLP_URL_Num,0);
        A->Statistic_Emlinfo_basic_Data=load_emlinfo(&A->Statistic_Emlinfo_basic_Num,0);
        A->Statistic_keywordclass_Basic_Data=load_dlpkeyclass(&A->Statistic_keywordclass_Basic_Data,0);
        A->Statistic_Protocol_Data=load_statisticProtocol(&A->Statistic_Protocol_Num,0);
        A->Statistic_Spam_Basic_Data=load_statistic_spam(&A->Statistic_Spam_Basic_Num,0);
        A->Statistic_Virus_Basic_Data=load_statistic_virus(&A->Statistic_Virus_Basic_Num,0);
        A->Syslog_Data=load_syslog(&A->Syslog_Num,0);
        A->URL_list_Data=load_url_list(&A->URL_list_Num,0);
        A->user_info_Data=load_user_info(&A->user_info_Num,0);
        A->virus_list_Data=load_virus_list(&A->virus_list_Num,0);
        printf("%d\n\n",A->behavior_Num);
    }
    return NULL;
}

int TestAll(void)
{
    int flags=255;
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    BehaviorLogTest(flags&(~DELETE_DB_FLAGS));
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    StrategyWordsTest(flags&(~DELETE_DB_FLAGS));
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    StrategyClassTest(flags&(~DELETE_DB_FLAGS));
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    KeyWordsTest(flags&(~DELETE_DB_FLAGS));
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    KeyClassTest(flags&(~DELETE_DB_FLAGS));
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    ProtocolSwitchTest(flags&(~DELETE_DB_FLAGS));
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    SpamListTest(flags&(~DELETE_DB_FLAGS));
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    StatisticDLPKeyWordsTest(flags&(~DELETE_DB_FLAGS));
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    StatisticDLPUrlTest(flags&(~DELETE_DB_FLAGS));
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    StatisticEmlInfoBasicTest(flags&(~DELETE_DB_FLAGS));
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    StatisticKeyClassBasicTest(flags&(~DELETE_DB_FLAGS));
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    StatisticProtocolTest(flags&(~DELETE_DB_FLAGS));
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    StatisticSpamTest(flags&(~DELETE_DB_FLAGS));
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    StatisticVirusTest(flags&(~DELETE_DB_FLAGS));
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    SysLogTest(flags&(~DELETE_DB_FLAGS));
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    UrlListTest(flags&(~DELETE_DB_FLAGS));
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    UserInfoTest(flags&(~DELETE_DB_FLAGS));
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    VirusListTest(flags&(~DELETE_DB_FLAGS));

    return 0;
}


int FreeAll(DataPtr A)
{
    if(!A)
        return 0;
    free(A);
    A=NULL;
    return 0;
}
