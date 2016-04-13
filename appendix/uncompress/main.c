#include "uncompress.h"

int main(int argc, char  *argv[])

{
    //int flag = uncompress("compress2.zip");
    //int flag = uncompress("finalcp.rar");
    //int flag = uncompress("word.tar");
    int flag =0;
    if(argc>=2)
    	flag= uncompress(argv[1]);
    printf("%d",flag);
    return 0;
}


