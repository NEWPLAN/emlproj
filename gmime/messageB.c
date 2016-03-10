#include "messageB.h"
#include "EnableSwitch.h"

void GetMessageBody(GMimeMessage* pMessage)
{
    GMimeObject *ps =g_mime_message_get_body(pMessage);
    printf("\n\n\nbody:%s\n",  g_mime_object_to_string(ps));
}

static void
count_foreach_callback (GMimeObject *parent, GMimeObject *part, gpointer user_data)
{
    int *count =(int *) user_data;
    (*count)++;

    /* 'part' points to the current part node that
     * g_mime_message_foreach() is iterating over */

    /* find out what class 'part' is... */
    if (GMIME_IS_MESSAGE_PART (part))
    {
        /* message/rfc822 or message/news */
        GMimeMessage *message;

        /* g_mime_message_foreach() won't descend into
                   child message parts, so if we want to count any
                   subparts of this child message, we'll have to call
                   g_mime_message_foreach() again here. */
        printf("\n===============descend into subparts=================\n");
        message = g_mime_message_part_get_message ((GMimeMessagePart *) part);
        g_mime_message_foreach (message, count_foreach_callback, count);
    }
    else if (GMIME_IS_MESSAGE_PARTIAL (part))
    {
        /* message/partial */

        /* this is an incomplete message part, probably a
                   large message that the sender has broken into
                   smaller parts and is sending us bit by bit. we
                   could save some info about it so that we could
                   piece this back together again once we get all the
                   parts? */
        printf("\n===============partial=================\n");
    }
    else if (GMIME_IS_MULTIPART (part))
    {
        /* multipart/mixed, multipart/alternative,
         * multipart/related, multipart/signed,
         * multipart/encrypted, etc... */

        /* we'll get to finding out if this is a
         * signed/encrypted multipart later... */
        printf("\n===============multipart=================\n");
    }
    else if (GMIME_IS_PART (part))
    {
        /* a normal leaf part, could be text/plain or
         * image/jpeg etc */
        printf("\n===============normal leaf part=================\n");
        printf("====>>>>====>>>>====>>>>Decode start here<<<<====<<<<====<<<<====\n");
        {
#if 0
            printf("\ndecode this normal leaf part================\n" );
            //GMimeStream *stream = g_mime_stream_file_new (stdout);
            //GMimeDataWrapper * content=g_mime_part_get_content_object(part);
            //g_mime_data_wrapper_write_to_stream(content,stream);
#endif
        }
        AnalyseMessageParse((GMimePart*)part);
    }
    else
    {
        printf("\n===============descend not reached=================\n");
        g_assert_not_reached ();
    }
}

void count_parts_in_message (GMimeMessage *message)
{
    int count = 0;

    /* count the number of parts (recursively) in the message
     * including the container multiparts */
    g_mime_message_foreach (message, count_foreach_callback, &count);
    printf ("There are %d parts in the message\n", count);
}
