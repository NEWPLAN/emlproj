#include "hashs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "allhead.h"

typedef struct Node
{
    char *str;
    struct Node * next;
} ChNode,*ChNodePtr;

typedef struct Pname
{
    ChNodePtr index;
} HaType;

#define MAXLEN 10007

static ChNodePtr NodeCreate(void)
{
    ChNodePtr pp=(ChNodePtr)malloc(sizeof(ChNode));
    memset(pp,0,sizeof(ChNode));
    return pp;
}

static HaType * HashInit(void)
{
    HaType * Ptrs=(HaType *)malloc(MAXLEN*sizeof(HaType));
    if(!Ptrs)
        return NULL;
    memset(Ptrs,0,MAXLEN*sizeof(HaType));
    return Ptrs;
}

static int HashGetValue(char* str)
{
    int addr=0;
    unsigned char* p=(unsigned char*)str;
    for(addr=0; *p; p++)
    {
        addr=257*addr+*p;
        addr%=MAXLEN;
    }
    return addr;
}

static void AddStr(HaType * Hd,int index,char* str)
{
    HaType * Paddr=&(Hd[index]);
    if(!(Paddr->index))
    {
        Paddr->index=NodeCreate();
        Paddr->index->str=str;
        return ;
    }
    ChNodePtr pp=Paddr->index;
    ChNodePtr par;
    while (pp)
    {
        if(!strcmp(pp->str,str))
            return;
        else
        {
            par=pp;
            pp=pp->next;
        }
    }
    par->next=NodeCreate();
    par->next->str=str;
    return;
}

static HaType * HashCreate(SpiPtr pattern,int numpatt)
{
    HaType * Hd=HashInit();
    for(int i=0; i<numpatt; i++)
    {
        int val=HashGetValue(pattern[i]);
        AddStr(Hd,val,pattern[i]);
    }
    return Hd;
}

int HashMach(char* objs, SpiPtr pattern,int numpatt)
{
	int matchres=0;
    HaType * Hed=HashCreate(pattern,numpatt);
    int Nums=strlen(objs);
    char *p=(char*)malloc(Nums);
    char *Links=p;
    if(!p)
    {
        printf("error in malloc\n");
        return matchres;
    }
    strcpy(p,objs);
    char *before=p;
#ifdef __DEBUG
    printf("location\tpattern\n");
#endif
    while(*p)
    {
        if(*p==' ')
        {
            *p=0;
            int val=HashGetValue(before);
            ChNodePtr nptr=Hed[val].index;
            while(nptr)
            {
                if(!strcmp(nptr->str,before))
                {
#ifdef __DEBUG                	
                    printf("%-8d\t%s\n",(int)(before-Links),before);
#endif                    
                    matchres++;
//                    break;
                }
                nptr=nptr->next;
            }
            *p=' ';
            before=p+1;
        }
        p++;
    }
    return matchres;
//    free(Links);
}
