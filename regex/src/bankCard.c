#include "bankCard.h"
#include "compileRegur.h"
#include "flags.h"
/*
银行卡号分为16位和19位均由数字构成
*/
int bankCardMatch(void)
{
    /*匹配的目标串*/
	char src[]="62270038190009959 is 我的银行卡, my second mobilephone is6227003819000995963 ,6227003819000995964也是我的银行卡 is too and my phone is 6227003819000995965";
    /*规则*/
    char* regIdCard="(^|\\b|(?<=[^\\d]))(\\d{19}|(\\d{16}))(\\b|$|(?=[^\\d]))";
    compliemain(regIdCard,flags?txtstr:src);
    return 0;
}
