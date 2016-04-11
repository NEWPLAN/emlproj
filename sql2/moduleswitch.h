/*************************************************************************
	> File Name: moduleswitch.h
	> Author: 
	> Mail: 
	> Created Time: 2016年04月11日 星期一 09时30分45秒
 ************************************************************************/

#ifndef EMAIL_MODULESWITCH_H
#define EMAIL_MODULESWITCH_H
	#define VIRUS_SWITCH 	(1<<0)
	#define SPAM_SWITCH 	(1<<1)
	#define URL_SWITCH 		(1<<2)
	#define KEYWORDS_SWITCH (1<<3)
	#define KEYCLASS_SWITCH (1<<4)
int loadmodule(void);
#endif
