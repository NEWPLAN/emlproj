#ifndef __MESSAGEB_H__TEST__
#define  __MESSAGEB_H__TEST__
#include <gmime/gmime.h>
#include <stdio.h>
#include "messageParser.h"
void GetMessageBody(GMimeMessage* pMessage);
void count_parts_in_message(GMimeMessage *message);
#endif
