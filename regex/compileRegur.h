#ifndef __COMPILEREGULR_H__
#define __COMPILEREGULR_H__

#define OVECCOUNT 30    /* should be a multiple of 3 */
#define EBUFLEN 128
#define BUFLEN 1024

int compliemain(char* pattern , char* src);

typedef struct URTLLIST
{
	char * data;
	struct  URTLLIST * next;
}urltype, *urlptr;

urlptr rteStr(char* pattern , char* src);
#endif
