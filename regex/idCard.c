#include "idCard.h"
#include "compileRegur.h"
#include "flags.h"

/*
 * 身份证15位编码规则：dddddd yymmdd xx p
 * dddddd：6位地区编码
 * yymmdd: 出生年(两位年)月日，如：910215
 * xx: 顺序编码，系统产生，无法确定
 * p: 性别，奇数为男，偶数为女
 *
 * 身份证18位编码规则：dddddd yyyymmdd xxx y
 * dddddd：6位地区编码
 * yyyymmdd: 出生年(四位年)月日，如：19910215
 * xxx：顺序编码，系统产生，无法确定，奇数为男，偶数为女
 * y: 校验码，该位数值可通过前17位计算获得
 *
 * 前17位号码加权因子为 Wi = [ 7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2 ]
 * 验证位 Y = [ 1, 0, 10, 9, 8, 7, 6, 5, 4, 3, 2 ]
 * 如果验证码恰好是10，为了保证身份证是十八位，那么第十八位将用X来代替
 * 校验位计算公式：Y_P = mod( ∑(Ai×Wi),11 )
 * i为身份证号码1...17 位; Y_P为校验码Y所在校验码数组位置
 */
//15位和18位身份证号码的正则表达式


/*
 * Email邮箱
 * 如：zhangsan@163.com、li-si@236.net、wan_gwu999@SEED.NET.TW
 */
int idCardMatch(void)
{
    /*匹配的目标串*/
	char src[]="413026199207214211 is 我的身份证号, my second mobilephone is413026199207214212 ,413026199207314321也是我的邮箱 is too and my phone is 413026199207214122";
    /*规则*/
    char* regIdCard="(^|\\b|(?<=[^\\d]))(\\d{17}(\\d|[Xx]))(\\b|$)";
    //char* regIdCard="(^|[^\\d]|\\b)(([1-9]\\d{7}((0\\d)|(1[0-2]))(([0|1|2]\\d)|3[0-1])\\d{3})|([1-9]\\d{5}[1-9]\\d{3}((0\\d)|(1[0-2]))(([0|1|2]\\d)|3[0-1])((\\d{4})|\\d{3}[Xx])))($|\\b)";
    return compliemain(regIdCard,flags?txtstr:src);
}
