#include "log.h"

#define __LOG__

void do_log(int level, const char *fmt, ...)
{
    char puffer[4096];
    va_list az;
    va_start(az, fmt);
    vsnprintf(puffer, 4096, fmt, az);
#ifdef __LOG__11
    printf("%s\n", puffer);
#endif    
    va_end(az);
}
