/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2016年04月20日 星期三 16时58分14秒
 ************************************************************************/

#include<stdio.h>
#include "mp3parse.h"
#include "mp4parse.h"

int main(int argc, char* argv[])
{
	audiopaser("test.mp3",".",".");
	videoparser("test.mp4",".",".");
    return 0;
}
