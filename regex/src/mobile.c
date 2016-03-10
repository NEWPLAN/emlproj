#include "mobile.h"
#include "compileRegur.h"
#include "flags.h"
/*
 * 手机号码格式
 * 只允许以13、15、18开头的号码
 * 如：13012345678、15929224344、18201234676
 */

/*
 * 固定电话号码格式
 * 因为固定电话格式比较复杂，情况比较多，主要验证了以下类型
 * 如：010-12345678、0912-1234567、(010)-12345678、(0912)1234567、(010)12345678、(0912)-1234567、01012345678、09121234567
 */
/*
 * Email邮箱
 * 如：zhangsan@163.com、li-si@236.net、wan_gwu999@SEED.NET.TW
 */
int mobileMatch(void)
{
    /*匹配的目标串*/
	char src[]="13354296148is also my phone number, my second mobilephone is 1325381979144  and my phone is18661828256";
    /*规则*/
    char* regMobile="(^|(?<=[^\\d\\.]))(1[3,5,8]\\d{9})((?=[^(\\.\\d)*])|$)";
   /* char* regPhone="(?<=[^\\d\\.])((^0\\d{2}-?\\d{8}$)|(^0\\d{3}-?\\d{7}$)|(^0\\d2-?\\d{8}$)|(^0\\d3-?\\d{7}$))(?=[^(\\.\\d)*])";*/
    compliemain(regMobile,flags?txtstr:src);
    return 0;
}
