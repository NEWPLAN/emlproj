#ifndef __OFFICE_EXTRACT_H
#define __OFFICE_EXTRACT_H

#ifdef __cplusplus
extern "C"
{
#endif
//成功返回0，失败返回-1，均有相关提示信息

int docmain(char* src, char* workspace,char *curpath);

int xlsmain(char* src, char* workspace,char *curpath);

int pptmain(char* src, char* workspace,char *curpath);

#ifdef __cplusplus
}
#endif

#endif
