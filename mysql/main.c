//#include "dboperate.h"
#include "all.h"
/***********************************************
*
*
*测试文件
*
************************************************/

static int TestAll(void);
static int LoadAll(void);
static int FreeAll(void);

int main(int argc, char* argv[])
{
	//printProtocolSwitch();
	 //printStatistic_DLP_keywords();
    LoadAll();
    TestAll();
    FreeAll();
    return 0;
}

int LoadAll(void)
{
    return 0;
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


int FreeAll(void)
{
    return 0;
}
