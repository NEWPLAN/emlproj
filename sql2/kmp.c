#include <stdio.h>
#include <string.h>
#include "kmp.h"
static void preKmp(char *src, int m, int *rule)
{
    int i=0,j=-1;
    rule[0]=j;
    while(i<m)
    {
        while(j!=-1 && src[j] != src[i])
            j=rule[j];
        i++,j++;
        if(j>=m)
            rule[i]=rule[j-1];
        else
            rule[i]=j;
    }
}
//int recount=0;
int check_sub(char *src, int slen, char *pattern, int plen)
{
    int rule[10005];
    preKmp(pattern,plen,rule);
    int i=0, j=0;
    while(i<slen)
    {
        while(j!=-1 && pattern[j] != src[i])
        {
            j = rule[j];
        }
        i++,j++;
        if(j==plen)
        {
            //printf("%d\n",i-m);//匹配的位置
            //recount++;
            return i;
            j=rule[j];
        }
    }
    return 0;
}
#if 0
int main(int argc, char* argv[])
{
    char p2[]="程阳 <zuricher@126.com>";
    char p1[]="zuricher@126.com";
    int i=check_sub(p2,strlen(p2),p1,strlen(p1));
    printf("%d\n",i);
    return 0;
}
#endif
