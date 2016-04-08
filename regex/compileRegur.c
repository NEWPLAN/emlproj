#include "compileRegur.h"
#include <pcre.h>
#include <stdio.h>
#include <string.h>

extern char* txtstr;

int compliemain(char* pattern , char* src)
{
	int flags=0;
    pcre *reCM;
    const char *error;
    int erroffset;
    int rcCM;
	int nums=0;
    int ovector[30];

#if __DEBUG    
    printf("Pattern: \"%s\"\n", pattern);
#endif    

    reCM=pcre_compile(pattern, 0, &error, &erroffset, NULL);

    //printf("输入一串IP:");
    //scanf("%s",src);
    if(NULL==reCM)
    {
        printf("PCRE compilation failed at offset %d %s",erroffset,error);
        return 0;
    }

    rcCM = pcre_exec(reCM, NULL, src, strlen(src), 0, 0, ovector, 30);  //匹配pcre编译好的模式，成功返回正数，失败返回负数
/*此处主要是在针对匹配多个重复的串*/
again:
    if(rcCM<0)
    {
    	if(flags)
    	{
#if __DEBUG    		
    		printf("match done!\n");
#endif    		
    		return 1;
    	}
    	else/*没有匹配到结果，返回0*/
    	{
#if __DEBUG    		
    	    printf("error in match this string\n");
        	printf("can't match this pattern \n");
#endif        	
        	return 0;
        }
        
    }
	{
        char *substring_start = src + ovector[2*0];
        int substring_length = ovector[2*0+1] - ovector[2*0];
#if __DEBUG        
        printf("$%2d: %.*s\n", ++nums, substring_length, substring_start);
#endif        
    }
	
    //printf("Pattern_CM: \"%s\"\n", pattern);
    //printf("String : %s\n", src+ovector[0]);

    {
        int offsets = ovector[1];
        memset(ovector,0,sizeof(ovector));
        rcCM = pcre_exec(reCM, NULL, src, strlen(src), offsets, 0, ovector, 30);
        flags=1;  
        goto again;
    }


}
