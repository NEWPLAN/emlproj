//#include "dboperate.h"
#include "all.h"
/***********************************************
*
*
*测试文件
*
************************************************/
int main(int argc, char* argv[])
{
	char*** results=NULL/*,***tptr=NULL*/;
    FetchRtePtr retval=NULL;
#if 1
    database_connect_local("root");
    database_connect("127.0.0.1","root");
    //int ret = database_insert("Protocol_switch","'789','789',123");
    database_insert("Protocol_switch","'789','789',456");
    //database_query("Protocol_switch");
    database_update("Protocol_switch","set direction='NEWPLAN' where switch=456");
    //database_update("Protocol_switch","set protocol='456',direction='456' where switch=456");
#endif
    retval=(FetchRtePtr)database_query("Protocol_switch");
   // results=(char***)database_query("Protocol_switch");
   results=retval->dataPtr;
    if(results==NULL)
    	return 0;
    printf("========================\n");
#if 0
	tptr=results;
    printf("row=%d\tcol=%d\n",retval->row,retval->col);
    while(*results)
    {
    	printf("%-12s\t%s\t%s\n",(*results)[0],(*results)[1],(*results)[2]);
    	free((*results)[0]);
        free((*results)[1]);
        free((*results)[2]);
        free(*results);
        results++;
    }
    free(tptr);
#endif
#if 1
    if(argc>=2)
        database_delete("Protocol_switch","switch=456");
#endif
	printProtocolSwitch();
//    int ret = database_delete("Protocol_switch","switch=456");
//    database_query("Protocol_switch");
//    printf("%d\n",ret);
    return 0;
}

