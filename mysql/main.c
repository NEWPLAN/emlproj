#include "dboperate.h"

/***********************************************
*
*
*测试文件
*
************************************************/
int main(int argc, char* argv[])
{
    database_connect_local("root");
    database_connect("127.0.0.1","root");
    //int ret = database_insert("Protocol_switch","'789','789',123");
    database_insert("Protocol_switch","'789','789',456");
    database_query("Protocol_switch");
    database_update("Protocol_switch","set protocol='456',direction='456' where switch=456");
    database_query("Protocol_switch");
    if(argc>=2)
        database_delete("Protocol_switch","switch=456");
//    int ret = database_delete("Protocol_switch","switch=456");
//    database_query("Protocol_switch");
//    printf("%d\n",ret);
    return 0;
}

