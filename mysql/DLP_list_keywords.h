/*************************************************************************
	> File Name: strategywords.h
	> Author:
	> Mail:
	> Created Time: 2016年03月17日 星期四 13时05分21秒
 ************************************************************************/

#ifndef DB_STRATEGYWORDS_H
#define DB_STRATEGYWORDS_H
typedef struct STRATEGYWORDS
{
    char strategy_id[11];/*策略编号,int*/
    char strategy_type[10+1];/*策略类型,list type :0->black,1-white*/
    char strategy_level[5+1];/*策略制定者的权限，等级,0->admain,1->group, 2->mamager, 3->employee*/
    char strategy_info[255+1];/*策略信息,useless*/
    char strategy_terminal[5+1];/*针对源还是目的,0->terminal, 1->source*/
    char strategy_target[255+1];/*策略目标，源还是目的*/
    char strategy_content[1023+1];/*策略内容*/
} strategywords,*strategywordsPtr;
/*
strategy_id, id随机数，一般取当前系统时间
strategy_type[10+1];/策略类型,list type :0->black,1-white
strategy_level[5+1];/策略制定者的权限，等级,0->admain,1->group, 2->mamager, 3->employee
strategy_info[255+1];/策略信息,buptlrl@163.com 或者 192.168.1.1
strategy_terminal[5+1];/针对源还是目的,1->terminal, 0->source
strategy_target[255+1];/策略目标，针对ip还是域名
strategy_content/该策略需要匹配的关键字，建议使用特殊字符分割不同关键字，例如 清华&中国&北京/
*/
strategywordsPtr load_strategywords(void* A, void* B);
void print_strategywords(void);
void strategywordsRelase(strategywordsPtr* tables);
int StrategyWordsTest(int flags);
#endif
