#ifndef __OFFICE_EXTRACT_H
#define __OFFICE_EXTRACT_H

#ifdef __cplusplus
extern "C"
{
#endif
//成功返回0，失败返回-1，均有相关提示信息
int docmain(int argc, char* argv[]);

int xlsmain(int argc, char* argv[]);

int pptmain(int argc, char* argv[]);

#ifdef __cplusplus
}
#endif

#endif
