#include "all.h"
#include "assist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

CheckType urlCheck(char* email, char* owner, int direction)
{
    return NONE;
}
/*'''
table URLList URL名单
id      主键，自动生成
owner   规则拥有者，
        普通用户创建的规则，owner为用户邮件地址，
        域管理员创建的规则，owner为该域域名，适用于该域收发的邮件；
        网关管理员创建的规则，owner为GLOBAL，适用于全网关收发的邮件
level   规则层级， 0（用户级），1（域级），2（网关级）
direction 0（进）、1（出）

table URLs
id      主键，自动生成
list_id  URL列表id
URL URL
'''
#跟keywordsCheck逻辑一模一样

#urlCheck(email, sender, 1)
#urlCheck(email, receiver, 0)

def urlCheck(email, owner, direction):

    if owner not in netgate:
        return 'None'

    #1.user级处理
    user_listids = SELECT list_id FROM URLList WHERE owner == owner AND level == 0 AND direction == direction
    user_urls = set() #空集

    for list_id in user_listids:
        urls = SELECT url FROM URLs WHERE list_id == list_id
        user_urls += urls #求并集

    if email match user_urls:
        return 'Confirmed'

    #2.domain级处理
    domain = getDomain(owner) #domain 在网关下

    domain_listids = SELECT list_id FROM URLList WHERE owner == domain AND level == 1 AND direction == direction
    domain_urls = set() #空集

    for list_id in domain_listids:
        urls = SELECT url FROM URLs WHERE list_id == list_id
        domain_urls += urls #求并集

    if email match domain_urls:
        return 'Confirmed'

    #3.网关级处理
    global_listids = SELECT list_id FROM URLList WHERE owner == GLOBAL AND level == 2 AND direction == direction
    global_urls = set() #空集

    for list_id in global_listids:
        urls = SELECT url FROM URLs WHERE list_id == list_id
        global_urls += urls #求并集

    if email match global_urls:
        return 'Confirmed'

    return 'OK'
*/
