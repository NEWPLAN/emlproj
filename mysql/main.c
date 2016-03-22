//#include "dboperate.h"
#include "all.h"
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
#define MYSQL_LIBS
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
        A->Behavior_data=load_behaviorLog(&A->behavior_Num,NULL);
        A->DLP_list_keywords_data=load_strategywords(&A->DLP_list_keywords_Num,0);
        A->DLP_list_keywordsclass_data=load_strategyclass(&A->DLP_list_keywordsclass_Num,0);
        A->keyword_list_data=load_keywords(&A->keyword_list_Num,0);
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
    }
    return NULL;
}

int TestAll(void)
{
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    BehaviorLogTest(255);
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    StrategyWordsTest(255);
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    StrategyClassTest(255);
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    KeyWordsTest(255);
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    KeyClassTest(255);
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    ProtocolSwitchTest(255);
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    SpamListTest(255);
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    StatisticDLPKeyWordsTest(255);
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    StatisticDLPUrlTest(255);
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    StatisticEmlInfoBasicTest(255);
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    StatisticKeyClassBasicTest(255);
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    StatisticProtocolTest(255);
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    StatisticSpamTest(255);
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    StatisticVirusTest(255);
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    SysLogTest(255);
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    UrlListTest(255);
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    UserInfoTest(255);
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    VirusListTest(255);

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
