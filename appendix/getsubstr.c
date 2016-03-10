#include <string.h>
#include <stdio.h>
#include "getsubstr.h"
//求子串  获取文件后缀名函数
char* substr(const char*str, unsigned start, unsigned end)
{
    unsigned n = end - start;
    static char stbuf[256];
    strncpy(stbuf, str + start, n);
    stbuf[n] = 0;                           //字串最后加上0
    return stbuf;
}
char* GetSuffix(char* fileName)
{
    char *ptr, c = '.';
    int pos;
    char * temps="F:\\workspace\\zhoulu.wmv.ppt";
    if(!fileName)
        fileName=temps;

    ptr = strrchr(fileName, c);    //最后一个出现c的位置
    if(ptr==0)
        return NULL;
    pos = ptr-fileName;             //用指针相减 求得索引
    return substr(fileName,pos+1,strlen(fileName));
}
