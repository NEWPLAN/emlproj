#ifndef _LOG_H
#define _LOG_H
#include <stdio.h>
#include <stdarg.h>

#define LOG_EMERG 1
#define LOG_NOTICE 2
#define LOG_DEBUG 3
#define LOG_WARNING 4
#define LOG_CRIT 5
#define LOG_ALERT 6

extern void do_log(int level, const char *fmt, ...);


#endif
