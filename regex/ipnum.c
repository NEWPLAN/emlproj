#include "ipnum.h"
#include "ipnum.h"
#include <stdio.h>
#include <pcre.h>
#include <string.h>
#include "compileRegur.h"
#include "flags.h"

/*
 * IP地址
 * 如：192.168.1.102
 */
//char* regIP="((([1-9]\\d?)|(1\\d{2})|(2[0-4]\\d)|(25[0-5]))\\.){3}(([1-9]\\d?)|(1\\d{2})|(2[0-4]\\d)|(25[0-5]))";
/*
 * Email邮箱
 * 如：zhangsan@163.com、li-si@236.net、wan_gwu999@SEED.NET.TW
 */
int IPMatch(void)
{
    /*匹配的目标串*/
	char src[]="my ip is y 1.1.1.1 and 192.168.2.33  do you know here is 2.12.21.1.92.168.2.33?, 3.3.3.3. ?5.5.5.5? .6.6.6.6 7.7.7.7";
    /*规则*/
    char pattern[]="(^|(?<=[^\\d.]))(((([1-9]\\d?)|(1\\d{2})|(2[0-4]\\d)|(25[0-5]))\\.){3}(([1-9]\\d?)|(1\\d{2})|(2[0-4]\\d)|(25[0-5])))($|(?=[^.\\d]))";
    return compliemain(pattern,flags?txtstr:src);
}
