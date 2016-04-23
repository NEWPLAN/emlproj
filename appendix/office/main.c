/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2016年04月20日 星期三 16时51分51秒
 ************************************************************************/

#include<stdio.h>
#include "office_extract.h"
#include <unistd.h>
int main(int argc, char * argv[])
{
	int ii=10;
	while(ii--)
	{
	if(fork()==0){
    docmain("test.docx","/home/gw/newplan/emlproj/appendix","office");
    pptmain("test.pptx","/home/gw/newplan/emlproj/appendix","office");
    xlsmain("test.xlsx","/home/gw/newplan/emlproj/appendix","office");
    return 0;
    }}
    return 0;
}
