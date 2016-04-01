#include "basicInfo.h"
#include "DataStructure.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
char* workspace;
GmimeDataPtr GetBasicInfo(GMimeMessage * pMessage)
{
    char emailpath[1024]={0};
    sprintf(emailpath,"%s/temps/test.email.txt",workspace);
    FILE * fptr=fopen(emailpath,"ab");
    if(fptr)
    {
        fprintf(fptr, "sender : %s\n",g_mime_message_get_sender(pMessage));/*get sender*/
        fprintf(fptr,"Reply-to:%s\n",g_mime_message_get_reply_to (pMessage));/*get reply to*/
        fprintf(fptr,"To:%s\n", internet_address_list_to_string(g_mime_message_get_recipients(pMessage,GMIME_RECIPIENT_TYPE_TO),FALSE));/*get the to receiver*/
        fprintf(fptr,"subject:%s\n",g_mime_message_get_subject (pMessage));/*get subject*/
        fprintf(fptr,"message ID:%s\n",g_mime_message_get_message_id(pMessage));/*get message ID*/
        fprintf(fptr,"date:%s\n",g_mime_message_get_date_as_string(pMessage) );/*get date of e-mail*/
        fclose(fptr);
    }
    printf("sender:%s\n",g_mime_message_get_sender(pMessage));/*get sender*/
    printf("Reply-to:%s\n",g_mime_message_get_reply_to (pMessage));/*get reply to*/
    //printf("all recipients:%s\n", internet_address_list_to_string(g_mime_message_get_all_recipients(pMessage),TRUE));/*all receiver*/
    printf("To:%s\n", internet_address_list_to_string(g_mime_message_get_recipients(pMessage,GMIME_RECIPIENT_TYPE_TO),FALSE));/*get the to receiver*/
    printf("subject:%s\n",g_mime_message_get_subject (pMessage));/*get subject*/
    printf("message ID:%s\n",g_mime_message_get_message_id(pMessage));/*get message ID*/
    printf("date:%s\n",g_mime_message_get_date_as_string(pMessage) );/*get date of e-mail*/
    {
        /*write to memory*/
        const char *temps=NULL;
        GmimeDataPtr A=NULL;
        A=(GmimeDataPtr)malloc(sizeof(GmimeData));
        assert(A!=NULL);
        memset(A,0,sizeof(GmimeData));

        temps=g_mime_message_get_sender(pMessage);/*get sender*/
        if(temps)
        {
            int temNu=0;
            temNu=strlen(temps);
            A->from=(char*)malloc(temNu+2);
            memset(A->from,0,temNu+2);
            memcpy(A->from,temps,temNu+2);
            temps=NULL;
        }

        temps=g_mime_message_get_reply_to(pMessage);/*replay to*/
        if(temps)
        {
            int temNu=0;
            temNu=strlen(temps);
            A->replayto=(char*)malloc(temNu+2);
            memset(A->replayto,0,temNu+2);
            memcpy(A->replayto,temps,temNu+2);
            temps=NULL;
        }

        temps=internet_address_list_to_string(g_mime_message_get_recipients(pMessage,GMIME_RECIPIENT_TYPE_TO),FALSE);/*To*/
        if(temps)
        {
            int temNu=0;
            temNu=strlen(temps);
            A->to=(char*)malloc(temNu+2);
            memset(A->to,0,temNu+2);
            memcpy(A->to,temps,temNu+2);
            temps=NULL;
        }

        temps=g_mime_message_get_message_id(pMessage);/*MESSAGE ID*/
        if(temps)
        {
            int temNu=0;
            temNu=strlen(temps);
            A->messageID=(char*)malloc(temNu+2);
            memset(A->messageID,0,temNu+2);
            memcpy(A->messageID,temps,temNu+2);
            temps=NULL;
        }

        temps=g_mime_message_get_subject (pMessage);/*subject*/
        if(temps)
        {
            int temNu=0;
            temNu=strlen(temps);
            A->subjects=(char*)malloc(temNu+2);
            memset(A->subjects,0,temNu+2);
            memcpy(A->subjects,temps,temNu+2);
            temps=NULL;
        }
		return A;
    }

    /* code */
}
