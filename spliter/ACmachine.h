#ifndef _AC_MACHINE__
#define _AC_MACHINE__
typedef char (*SpiPtr)[255];
int ACStart(char* objerct, SpiPtr pattern, int numpatt);
#endif
