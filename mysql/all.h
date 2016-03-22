/*************************************************************************
	> File Name: all.h
	> Author:
	> Mail:
	> Created Time: 2016年03月16日 星期三 14时13分44秒
 ************************************************************************/

#ifndef DB__ALL_H
#define DB__ALL_H



#include "DLP_list_keywords.h"
#include "keyword_list.h"
#include "spam_list.h"
#include "Statistic_DLP_URL.h"
#include "Statistic_keywordclass_Basic.h"
#include "Statistic_Spam_Basic.h"
#include "Syslog.h"
#include "user_info.h"
#include "behavior_log.h"
#include "DLP_list_keywordsclass.h"
#include "keywordclass_list.h"
#include "Protocol_switch.h"
#include "Statistic_DLP_keywords.h"
#include "Statistic_Emlinfo_basic.h"
#include "Statistic_Protocol.h"
#include "Statistic_Virus_Basic.h"
#include "URL_list.h"
#include "virus_list.h"


typedef struct MYSQLINTERFACE
{
	int behavior_Num;
	behaviorlogPtr Behavior_data;/*behavior_log 表*/

	int DLP_list_keywords_Num;
	strategywordsPtr DLP_list_keywords_data;/*DLP_list_keywords 表*/

	int DLP_list_keywordsclass_Num;
	strategyclassPtr DLP_list_keywordsclass_data; /*DLP_list_keywordsclass 表*/

	int keyword_list_Num;
	keywordsPtr keyword_list_data;/*keyword_list 表*/

	int keywordclass_list_Num;
	keyclassPtr keywordclass_list_Data;/*keywordclass_list*/

	int Protocol_switch_Num;
	protocolPtr	Protocol_switch_Data;/*Protocol_switch*/

	int spam_list_Num;
	spamlistPtr	spam_list_Data;/*spam_list*/

	int Statistic_DLP_keywords_Num;
	statisticwordsPtr	Statistic_DLP_keywords_Data;/*Statistic_DLP_keywords*/

	int Statistic_DLP_URL_Num;
	statisticurlPtr	Statistic_DLP_URL_Data;/*Statistic_DLP_URL*/

	int Statistic_Emlinfo_basic_Num;
	emlinfoPtr	Statistic_Emlinfo_basic_Data;/*Statistic_Emlinfo_basic*/

	int Statistic_keywordclass_Basic_Num;
	dlpkeyclassPtr	Statistic_keywordclass_Basic_Data;/*Statistic_keywordclass_Basic*/

	int Statistic_Protocol_Num;
	staprotocolPtr	Statistic_Protocol_Data;/*Statistic_Protocol*/

	int Statistic_Spam_Basic_Num;
	statspamPtr	Statistic_Spam_Basic_Data;/* Statistic_Spam_Basic*/

	int Statistic_Virus_Basic_Num;
	statvirusPtr Statistic_Virus_Basic_Data;/*Statistic_Virus_Basic*/

	int Syslog_Num;
	syslogPtr	Syslog_Data;/*Syslog*/

	int URL_list_Num;
	urllistPtr	URL_list_Data;/*URL_list*/

	int user_info_Num;
	userinfoPtr	user_info_Data;/*user_info*/

	int virus_list_Num;
	viurlistPtr virus_list_Data;/*virus_list*/

}DataType, *DataPtr;

DataPtr LoadAll(void);
int FreeAll(DataPtr A);
int TestAll(void);

#endif
