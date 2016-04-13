/*************************************************************************
	> File Name: main.c
	> Author:
	> Mail:
	> Created Time: 2016年03月30日 星期三 14时20分11秒
 ************************************************************************/
#include "emailhead.h"
#include <stdio.h>

int main(int argc, char* argv[])
{

    int rte = 1000;
    Email_InitAll();
    while(rte--)
    {
        if(fork()==0)
        {
        	EmailType A;
        	A.filePath=argv[1];
        	Email_ParseFile(&A);
        	printf("done in fork process\n");
        	return 0;
        }
    }
	wait(NULL);
    printf("done\n");
    return 0;
}
