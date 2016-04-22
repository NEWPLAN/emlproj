#include "log.h"

void do_log(int level, const char *fmt, ...)
{
    char puffer[4096];
    va_list az;
    va_start(az, fmt);
    vsnprintf(puffer, 4096, fmt, az);
    printf("%s\n", puffer);
    va_end(az);
}
