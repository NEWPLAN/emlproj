/*'''
table KeywordClassList 管理员的关键字类型名单
id      主键，自动生成
owner   规则拥有者，
        用户创建的关键字类，owner为用户邮件地址
        域管理员创建的关键字类，owner为该域域名，适用于该域收发的邮件；
        网关管理员创建的关键字类，owner为GLOBAL，适用于全网关收发的邮件
level   规则层级， 0（用户级），1（域级），2（网关级）

direction 0（进）、1（出）
name    用户的关键字类型名单
regex   正则表达式
'''

#owner, direction, 0 表示进， 1表示出
#keywordClassCheck(email, sender, 1)，进行sender的关键字类检测
#keywordClassCheck(email, receiver, 0)，进行receiver的关键字类检测
*/
#include "all.h"
#include "assist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

CheckType keywordClassCheck(char* email,char* owner,int direction)
{

    if(checkInGateway(owner))
        return NONE;

    //#1.user级处理
    char command[1024]={0};
    sprintf(command, "SELECT * FORM　KeywordClassList　WHERE owner == %s AND level == 0 AND direction == %d",owner, direction);
    FetchRtePtr user_keywordclasses=sql_query(command);

    if(0/*email match user_keywordclasses*/)
        return CONFIRMED;

    //#2.domain级处理
    char* ownerDomain = getDomain(owner);
    memset(command,0,sizeof(command));
    sprintf(command,"SELECT * FORM　KeywordClassList　WHERE owner == %s AND level == 1 AND direction == %d",ownerDomain,direction);
    FetchRtePtr domain_keywordclasses=sql_query(command);

    if(0/*email match domain_keywordclasses*/)
        return CONFIRMED;

    //#3.网关级处理
    memset(command,0,sizeof(command));
    sprintf(command,"SELECT * FORM　KeywordClassList　WHERE owner == %s AND level == 2 AND direction == %d","GLOBAL",direction);
    FetchRtePtr global_keywordclasses=sql_query(command);

    if(0/*email match global_keywordclasses:*/)
        return CONFIRMED;
    return OK;
}
