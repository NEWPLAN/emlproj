#include "basicInfo.h"

void GetBasicInfo(GMimeMessage * pMessage)
{
    FILE * fptr=fopen("temps/test.txt","ab");
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
    /* code */
}
