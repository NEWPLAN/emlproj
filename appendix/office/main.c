/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2016年04月20日 星期三 16时51分51秒
 ************************************************************************/

#include<stdio.h>
#include "office_extract.h"
int main(int argc, char * argv[])
{
    docmain("test.docx","/home/host1/git/emlproj/appendix","office");
    pptmain("test.pptx","/home/host1/git/emlproj/appendix","office");
    xlsmain("test.xlsx","/home/host1/git/emlproj/appendix","office");
    return 0;
}
