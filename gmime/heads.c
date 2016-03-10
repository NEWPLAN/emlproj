#include "heads.h"

void GetHeader(GMimeMessage* pMessage)
{
    GMimeHeaderList *list=NULL;
    GMimeHeaderIter iter;
    const char * name,*value;

    if(NULL==(list=g_mime_object_get_header_list(GMIME_OBJECT(pMessage))))
        printf("[log]:\tfailed to get header list\n");
    if(!g_mime_header_list_get_iter(list,&iter))
        printf("[log]:\tfailed to get header list iter!\n");

#if ENABLE_HEADERS_LOG
    printf("\n[log]\t Output Headers =====>>>>>\n\n");
#endif

    for(;;)
    {
        name=g_mime_header_iter_get_name(&iter);
        value=g_mime_header_iter_get_value(&iter);
        printf("%s:\t %s\n", name, value);
        if (!g_mime_header_iter_next(&iter))
        {
            break;/* code */
        }
    }
}
