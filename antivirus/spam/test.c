#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "handleWhiteBlackList.h"
//#include "basicControl.h"
#include "antispam.h"
//#include "mystruct.h"

int main()
{
    struct mes result = detectSpam("/home/host1/emlproj/main/testdata/7777.eml");
    printf("the score is  %lf\n" ,result.score);
    return 0;
}

