/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2016年03月30日 星期三 14时20分11秒
 ************************************************************************/

#include<stdio.h>
#include"all.h"
#include "dboperate.h"
#include "assist.h"

extern CheckType spamCheck(char* email, char* owner, int direction);
extern CheckType virusCheck(char* email, char* owner, int direction);
extern CheckType keywordCheck(char* email,char* owner,int direction);
extern CheckType keywordClassCheck(char* email,char* owner,int direction);
extern CheckType urlCheck(char* email,char* owner,int direction);


strategyType combine_strategy(strategyType strategy_A, strategyType strategy_B)
{
	if(strategy_A==BLOCK||strategy_B==BLOCK)
		return BLOCK;
	if(strategy_A==TRASH||strategy_B==TRASH)
		return TRASH;
	return strategy_A;
}


void notify_sender(char* sender,char* notify_info)//通知发件人
{
	return;
}

strategyType get_valid_strategy(char* owner,char* field,int direction)
{
	char command1[1024]={0},command2[1024]={0},command3[1024]={0};
	FetchRtePtr rteval=NULL;
	char*domain=NULL;
	sprintf(command1,"SELCET %s in owner=%s and level=0 and direction=%d",field ,owner, direction);
	if((rteval=sql_query(command1))!=NULL)/*user_strategy*/
		return IGNORE/**rteval*/;
	domain = getDomain(owner);
	sprintf(command2,"SELCET %s in owner=%s and level=1 and direction=%d",field ,domain, direction);
	if((rteval=sql_query(command2))!=NULL)/*domain_strategy*/
		return IGNORE/**rteval*/;
	sprintf(command3,"SELCET %s in owner=%s and level=2 and direction=%d",field ,owner, direction);
	if((rteval=sql_query(command3))!=NULL)/*global_strategy*/
		return IGNORE/**rteval*/;
	return IGNORE;
}

void overall_check_single_side(char* email,char* owner,int direction, strategyType *final_strategy,char* notify_info)/*:#final_strategy和notify_info传引用*/
{
	CheckType spam_result,virus_result,keyword_result,keywordclass_result,url_result;
    //#1.垃圾检测
    spam_result = spamCheck(email,owner,direction);    
    //#2.病毒检测
    virus_result = virusCheck(email,owner,direction);
    //#3.关键字检测
    keyword_result = keywordCheck(email,owner,direction);
    //#4.关键字类检测
    keywordclass_result = keywordClassCheck(email,owner,direction);
    //#5.url检测
    url_result = urlCheck(email,owner,direction);
    
    memset(notify_info,0,strlen(notify_info));
        
    if(spam_result==CONFIRMED)
    {
    	strcat(notify_info,"垃圾邮件\t");// #类似的信息
    	strategyType spam_strategy = get_valid_strategy("spam", owner, direction);
    	*final_strategy=combine_strategy(*final_strategy, spam_strategy);
    }
    
    if(virus_result==CONFIRMED)
    {
    	strcat(notify_info,"您的邮件中含有病毒\t");// #类似的信息
    	strategyType virus_strategy = get_valid_strategy("virus", owner, direction);
    	*final_strategy=combine_strategy(*final_strategy, virus_strategy); 	
    }
    
    if(keyword_result==CONFIRMED)
    {
    	strcat(notify_info,"您的邮件中含有关键字\t");// #类似的信息
    	strategyType keyword_strategy = get_valid_strategy("keyword", owner, direction);
    	*final_strategy=combine_strategy(*final_strategy, keyword_strategy); 	    	
    }
    
    if(keywordclass_result==CONFIRMED)
    {
    	strcat(notify_info,"您的邮件中含有关键字类\t");// #类似的信息
    	strategyType keywordclass_strategy = get_valid_strategy("keywordClass", owner, direction);
    	*final_strategy=combine_strategy(*final_strategy, keywordclass_strategy);  	
    }
    
    if(url_result==CONFIRMED)
    {
    	strcat(notify_info,"您的邮件中含有URL\t");// #类似的信息
    	strategyType url_strategy = get_valid_strategy("url", owner, direction);
    	*final_strategy=combine_strategy(*final_strategy, url_strategy);   	
    }
    return ;
    
}

char* getSender(char* email)
{
	return NULL;
}

char* getReceiver(char* email)
{
	return NULL;
}

int overall_check(char* email)
{  
    //#发送端检测
    char* sender = getSender(email);
    strategyType sender_final_strategy = IGNORE;// #默认放行
    char notify_info[1024]={0};
        
    if(checkInGateway(sender))/*sender in netgate*/
    {
    	memset(notify_info, 0, sizeof(notify_info));
    	overall_check_single_side(email,sender,1,&sender_final_strategy,notify_info);
    	if(sender_final_strategy!=IGNORE)
    		notify_sender(sender,notify_info);//通知发件人
    }
    
        
    char* receiver = getReceiver(email);
    strategyType receiver_final_strategy =IGNORE;    
    
    if(receiver_final_strategy!=BLOCK&&
    checkInGateway(receiver)/*final_strategy != 'block' and receiver in netgate*/)//: #邮件能从发送端网关发出
        //#接收端检测
    { 
        memset(notify_info, 0, sizeof(notify_info));
        
        overall_check_single_side(email,receiver,0,&receiver_final_strategy,notify_info);
        
        if(receiver_final_strategy ==BLOCK)
            return 1;//#堵截
        else if (receiver_final_strategy == TRASH)
            return 2;//#发送到垃圾箱
        else if (receiver_final_strategy == IGNORE)
            return 0;//#直接发送
	}
}


int main(int argc, char* argv[])
{
	printf("hello world\n");
	return 0;
}
