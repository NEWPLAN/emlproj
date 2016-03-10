#include "all.h"
#include <stdio.h>
#define EML__SYSTEMS__
/*
#ifndef SYSTEMS
    int main(int argc, char const *argv[])
#else
    int GmimeMain(int argc, char const *argv[])
#endif
*/
#include "heads.h"
#include "messageB.h"
#include "basicInfo.h"
#include "EnableSwitch.h"
#include "messageParser.h"
#include "inits.h"

#ifdef  EML__SYSTEMS__
    int GmimeMain(int argc, char const * argv[])
#else
    int main(int argc, char const *argv[])
#endif
{
    /* code */
    GMimeMessage* pMessage=AllInits(argc,(char**)argv);
    if(!pMessage)
    {
        printf("error in ints \n");
        return 0;
    }

    /*analyze header message*/
    //GetHeader(pMessage);
    GetBasicInfo(pMessage);

    count_parts_in_message(pMessage);
    //GetBody( pMessage);
    //Analyze(pMessage);

    printf("[log]:\tunref the pMessage object.\n");
    g_object_unref(GMIME_OBJECT(pMessage));

    printf("[log]:\tshutdown for gmime\n");
    printf("local charset %s\n", g_mime_charset_locale_name());
    g_mime_shutdown();

    return 0;

}
