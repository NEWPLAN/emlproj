#include "compileRegur.h"
#include <pcre.h>
#include <stdio.h>
#include <string.h>

extern char* txtstr;

//#define __DEBUG 

int compliemain(char* pattern , char* src)
{
	int flags=0;
    pcre *reCM;
    const char *error;
    int erroffset;
    int rcCM;
	int nums=0;
    int ovector[30];

#ifdef __DEBUG    
    printf("Pattern: \"%s\"\n", pattern);
#endif
    reCM=pcre_compile(pattern, 0, &error, &erroffset, NULL);
    if(NULL==reCM)
    {
        printf("PCRE compilation failed at offset %d %s",erroffset,error);
        return 0;
    }
    rcCM = pcre_exec(reCM, NULL, src, strlen(src), 0, 0, ovector, 30);  //匹配pcre编译好的模式，成功返回正数，失败返回负数
again:/*此处主要是在针对匹配多个重复的串*/
    if(rcCM<0)
    {
    	if(flags)
    	{
#ifdef __DEBUG    		
    		printf("match done!\n");
#endif    		
    		return 1;
    	}
    	else/*没有匹配到结果，返回0*/
    	{
#ifdef __DEBUG    		
    	    printf("error in match this string\n");
        	printf("can't match this pattern \n");
#endif        	
        	return 0;
        }
    }
	{
        char *substring_start = src + ovector[2*0];
        int substring_length = ovector[2*0+1] - ovector[2*0];
#ifdef __DEBUG        
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


static char* getstr(char* filename)
{
	int NByte=0;
	char* str=NULL;
    FILE *filptr=fopen(filename,"rb");
    if(!filptr)
    {
        printf("error in open file %s\n",filename);
        return NULL;
    }
    fseek(filptr,0,SEEK_END);
    NByte=ftell(filptr);
    {
        str=(char*)malloc(NByte+20);
        if(str)
        {
            memset(str,0,20+NByte);
        }else
        {
            printf("can't malloc such huge page for loading txt\n");
            fclose(filptr);
            return NULL;
        }
        fseek(filptr,0,SEEK_SET);
        fread(str,1,NByte+20-1,filptr);
    }
    fclose(filptr);
    return str;
}



urlptr rteStr(char* pattern , char* src)
{
	int flags=0;
    pcre *reCM;
    const char *error;
    int erroffset;
    int rcCM;
	int nums=0;
    int ovector[30];
    urlptr heads=NULL,cur=NULL;
    heads=(urlptr)malloc(sizeof(urltype));
    memset(heads,0,sizeof(urltype));
    cur=heads;
    
    char* str=getstr(src);
    if(str==NULL)/*加载失败*/
    	return heads;
    	
#ifdef __DEBUG    
    printf("Pattern: \"%s\"\n", pattern);
#endif
    reCM=pcre_compile(pattern, 0, &error, &erroffset, NULL);
    if(NULL==reCM)
    {
        printf("PCRE compilation failed at offset %d %s",erroffset,error);
        return 0;
    }
    rcCM = pcre_exec(reCM, NULL, str, strlen(str), 0, 0, ovector, 30);  //匹配pcre编译好的模式，成功返回正数，失败返回负数
second:/*此处主要是在针对匹配多个重复的串*/
    if(rcCM<0)
    {
    	if(flags)
    	{
#ifdef __DEBUG    		
    		printf("match done!\n");
#endif    	
			if(str)	
				free(str);
    		return heads;/*提取完成*/
    	}
    	else/*没有匹配到结果，返回0*/
    	{
#ifdef __DEBUG    		
    	    printf("error in match this string\n");
        	printf("can't match this pattern \n");
#endif      
			if(str)	
				free(str);  	
        	return heads;
        }
    }
	{
        char *substring_start = str + ovector[2*0];
        int substring_length = ovector[2*0+1] - ovector[2*0];
#ifdef __DEBUG         
        printf("$%2d: %.*s\n", ++nums, substring_length, substring_start);
#endif  
		{
			cur->next=(urlptr)malloc(sizeof(urltype));
			memset(cur->next,0,sizeof(urltype));
			cur->next->data=(char*)malloc(substring_length+10);
			memset(cur->next->data,0,substring_length+10);
			memcpy(cur->next->data,substring_start,substring_length);
			cur=cur->next;
		}
    }
    {
        int offsets = ovector[1];
        memset(ovector,0,sizeof(ovector));
        rcCM = pcre_exec(reCM, NULL, str, strlen(str), offsets, 0, ovector, 30);
        flags=1;  
        goto second;
    }
}
