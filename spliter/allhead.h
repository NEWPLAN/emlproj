#ifndef __ALL_HEAD__
#define __ALL_HEAD__
#include "ACmachine.h"
#include "cDemoOK.h"
#include "hashs.h"

#define __DEBUG
#ifdef  __DEBUG
#undef  __DEBUG
#endif

#define EML__SYSTEMS__

#ifdef EML__SYSTEMS__
#ifdef __cplusplus
extern "C"
{
#endif
int SpliterMain(int argc, char* argv[]);
int SpliterExit(void);
int SpliterInit(void);
#ifdef __cplusplus
}
#endif
#endif
#endif
