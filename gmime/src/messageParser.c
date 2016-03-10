#include "messageParser.h"
#include <string.h>
#include <stdlib.h>
void AnalyseMessageParse(GMimePart * mime_part)
{
    int ichar=0,iencode =0;

    if( NULL==mime_part )
    {
        printf("empty part\n");
        return ;
    }

    GMimeContentEncoding ReEncoding = g_mime_part_get_content_encoding(mime_part);

    char* szCharset = (char*)g_mime_object_get_content_type_parameter((GMimeObject*)mime_part,"charset");
    printf("charset %s\n",szCharset);/*编码字体*/

    {
        char* szCharset = (char*)g_mime_object_get_content_type_parameter((GMimeObject*)mime_part,"name");
        printf("name %s\n",szCharset);
        GMimeContentType* pContentType = g_mime_object_get_content_type((GMimeObject*)mime_part) ;
        printf("The content type is: (%s/%s)\n",pContentType->type,pContentType->subtype);
        const char *  ppp= g_mime_object_get_disposition((GMimeObject*)mime_part);
        if(!ppp)
        {
            goto exits;
        }
        const char * qqq=g_mime_object_get_content_disposition_parameter((GMimeObject*)mime_part,"filename");
        printf("get disposition\t%s\n%s\n",ppp,qqq);
        if(qqq)
        {
            char filepath[50]={0};
            FILE *ps=NULL;
            memset(filepath,0,sizeof(filepath));
            strcpy(filepath,"testfile/");
            strcat(filepath,qqq);
            ps=fopen(filepath,"w+b");
            if(!ps)
            {
                printf("error in writing file \n");
                return;
            }
            printf("\n=======write to file================\n" );
            GMimeStream *stream = g_mime_stream_file_new (ps);
            GMimeDataWrapper * content=g_mime_part_get_content_object(mime_part);
            g_mime_data_wrapper_write_to_stream(content,stream);
            fclose(ps);
            printf("finish writing\n");
           // getchar();
            return ;
        }
    }

exits:{}
    /*decode for text/plain or html*/
    GMimeDataWrapper *dataWrap = g_mime_part_get_content_object(mime_part);
    if(!dataWrap)
        printf("error in dataWrap\n" );

    GMimeStream * gmime_stream= g_mime_data_wrapper_get_stream(dataWrap);
    //encoding转码
    GMimeFilter * pAttFilter =  g_mime_filter_basic_new(ReEncoding,FALSE);
    GMimeStream* pFilterStream =  g_mime_stream_filter_new(gmime_stream);

    iencode = g_mime_stream_filter_add (GMIME_STREAM_FILTER (pFilterStream), pAttFilter);

    /*create a filter convert the charset into local type*/
    GMimeFilter * secondPtr =  g_mime_filter_charset_new(szCharset,g_mime_charset_locale_name());
    ichar = g_mime_stream_filter_add (GMIME_STREAM_FILTER (pFilterStream), secondPtr);

    /*convert stream into stdout*/
    FILE* fileptr=fopen("temps/test.txt","ab");
    GMimeStream *stream;
    if(fileptr)
        stream = g_mime_stream_file_new (fileptr);
    else
        stream = g_mime_stream_file_new (stdout);
    g_mime_stream_write_to_stream(pFilterStream,stream);
    fclose(fileptr);
#if 0
    /*get data from stream load into char* */
    char pp[10000]= {0};
    int tt=g_mime_stream_read(pFilterStream,pp,10000);

    printf("%d\t%s\n",tt,pp);
#endif

    g_object_unref ( pAttFilter );
    g_object_unref ( secondPtr );


    g_mime_stream_filter_remove((GMimeStreamFilter*)pFilterStream,ichar);
    g_mime_stream_filter_remove((GMimeStreamFilter*)pFilterStream,iencode);
    g_object_unref (pFilterStream);
    //g_object_unref (gmime_stream);
    //g_object_unref (stream);

    //if(NULL==szCharset)
    {
        printf("\n====>>>>====>>>>====>>>>Finish decoding this part<<<<====<<<<====<<<<====\n");
        //getchar();   /*for test */
        //return ;
    }
    return ;
}
