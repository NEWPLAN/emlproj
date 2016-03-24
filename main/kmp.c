#include <stdio.h>
#include <string.h>
void preKmp(char *src, int m, int *rule)
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
int kmp(char *src, int m, char *dest, int n)
{
    int rule[10005];
    preKmp(src,m,rule);
    int i=0, j=0;
    while(i<n)
    {
        while(j!=-1 && src[j] != dest[i])
        {
            j = rule[j];
        }
        i++,j++;
        if(j==m)
        {
            //printf("%d\n",i-m);//匹配的位置
            //recount++;
            return i;
            j=rule[j];
        }
    }
    return 0;
}
/*
int main(int argc, char* argv[])
{
    char p2[]="asdasdasdfghj中文";
    char p1[]="hj中文";
    int i=kmp(p2,strlen(p2),p1,strlen(p1));
    printf("%d\n",i);
    return 0;
}
*/
