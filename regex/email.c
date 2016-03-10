#include "email.h"
#include <stdio.h>
#include <pcre.h>
#include <string.h>
#include "compileRegur.h"
#include "flags.h"
/*
 * Email邮箱
 * 如：zhangsan@163.com、li-si@236.net、wan_gwu999@SEED.NET.TW
 */

int EmlMatch(void)
{
    /*匹配的目标串*/
	char src[]="newplan001@163.com is my Email邮箱, my second mobilephone is zuricher@126.com ,2012223010013@uestc.edu.$com也是我的邮箱 is too and my phone is 1182612551@qq.c$om";
/*	char ppt[]="62270038190009959 is 我的银行卡, my second mobilephone is6227003819000995963 ,6227003819000995964也是我的银行卡 is too and my phone is 6227003819000995965\
newplan001@163.com is my Email邮箱, my second mobilephone is zuricher@126.com ,2012223010013@uestc.edu.$com也是我的邮箱 is too and my phone is 1182612551@qq.c$om\
413026199207214211 is 我的身份证号, my second mobilephone is413026199207214212 ,413026199207314321也是我的邮箱 is too and my phone is 413026199207214122\
my ip is y 1.1.1.1 and 192.168.2.33  do you know here is 2.12.21.1.92.168.2.33?, 3.3.3.3. ?5.5.5.5? .6.6.6.6 7.7.7.7\
13354296148is also my phone number, my second mobilephone is 1325381979144  and my phone is18661828256\
foo.bar hello 欢迎访问我的地址：www.baidu.com and another url like http://www.uestc.edu.cn besides www.tsinghua.edu.cn is also available, www.stanford.edu";*/
    /*规则*/
    char* regEmail="(^|\\b)(([a-zA-Z0-9]+[_|\\-|\\.]?)*[a-zA-Z0-9]+@([a-zA-Z0-9]+[_|\\-|\\.]?)*[a-zA-Z0-9]+(\\.[a-zA-Z]{2,3})+)(\\b|$)";
//    printf("%s\n",ppt);
    compliemain(regEmail,flags?txtstr:src);
    return 0;
}
