#include "inits.h"
#include "all.h"
#include "EnableSwitch.h"
#include "assert.h"

char* workspace;

GMimeMessage* AllInits(int argc, char **argv)
{
    workspace=argv[2];
    assert(workspace!=NULL);

    g_mime_init(0);
    if(argc<2)
    {
        printf("[log]:\terror open file to parser. hello <file>\n");
        return 0;
    }

#if __DEBUG
    printf("[log]:\thello, gmime! please\n");
#endif
    FILE* fp = fopen(argv[1],"rb");
    /*create a new stream...*/
    GMimeStream* pStream = g_mime_stream_file_new(fp);
#if __DEBUG
    printf("[log]:\t[main] g_mime_stream_file_new success!\n");
#endif
    /*set the filter
    *创建一个CRLF过滤器，过滤器介绍见前述。一般而言，MTA之间的通信每行都是以\n结尾的，但因为我们要处理以CRLF结尾的邮件体，所以必须附加一个CRLF过滤器来处理\r\n.
    */
    GMimeFilter* pCrlfFilter = g_mime_filter_crlf_new (FALSE,TRUE);

#if __DEBUG
    printf("[log]:\t[main] new crlf filter success!\n");
#endif

    /*根据初始文件流和过滤器创建一个具备过滤功能的流对象*/
    GMimeStream* pFilterStream = g_mime_stream_filter_new (pStream);
#if __DEBUG
    printf("[log]:\t[main] create filter stream with file stream success!\n");
    printf("[log]:\tunref the stream object.\n");
#endif
    //g_mime_stream_unref(pStream);
    g_object_unref (pStream);
    g_mime_stream_filter_add (GMIME_STREAM_FILTER (pFilterStream), pCrlfFilter);
    g_object_unref (pCrlfFilter);

#if __DEBUG
    printf("[log]:\t[main] add crlf filter to decode success!\n");
#endif
    /*
    *创建一个MIME解析器，分析器可对一封邮件进行解析，生成消息对象。在消息对象对邮件消息进行结构化的存储。通过消息对象可以遍历邮件各部分内容
    */
    GMimeParser* pParser = g_mime_parser_new();

    if(!pParser)
    {
        printf("[log]:\terror new parser.\n");
    }
#if __DEBUG
    printf("[log]:\tnew parser success!\n");
#endif
    /*
    *根据输入流对象对MIME解析器进行初始化
    */
    g_mime_parser_init_with_stream(pParser,pFilterStream);
    /*
    *利用MIME解析器生成消息对象
    */
    GMimeMessage* pMessage = g_mime_parser_construct_message(pParser);

    if(!pMessage)
    {
        printf("[log]:\terror construct the message!\n");
        return 0;
    }
#if __DEBUG
    printf("[log]:\tconstruct message with filter stream success!\n");
    printf("[log]:\tunref the filter stream.\n");
#endif
    //g_mime_stream_unref(pFilterStream);
    g_object_unref (pFilterStream);
#if __DEBUG
    printf("[log]:\tunref the parser object.\n");
#endif
    g_object_unref(pParser);
    return pMessage;
}


void AllFree(GMimeMessage *pMessage)
{
#if __DEBUG	
    printf("[log]:\tunref the pMessage object.\n");
#endif    
    g_object_unref(GMIME_OBJECT(pMessage));
#if __DEBUG
    printf("[log]:\tshutdown for gmime\n");
#endif    
    g_mime_shutdown();
}
