/*************************************************************************
	> File Name: all.h
	> Author:
	> Mail:
	> Created Time: 2016Äê02ÔÂ17ÈÕ ÐÇÆÚÈý 13Ê±40·Ö52Ãë
 ************************************************************************/

#ifndef EML_ALL_H
#define EML_ALL_H

/*load gmime libs*/
#include "../gmime/all.h"

/*load spliter words module*/
#include "../spliter/allhead.h"

/*load regex module*/
#include "../regex/all.h"

/*load appendix parsing module*/
#include "../appendix/all.h"

/*load antispam module*/
#include "../antivirus/spam/antispam.h"

/*load anti virus module*/
#include "../antivirus/virus/antivirus.h"

/*Load db from Database*/
//#include "../mysql/all.h"

#include "kmp.h"

/*Parsing email function */
//int ParseEML(char* filename);
int ParseEML(char* filename,GmimeDataPtr* rtevalPtr);

/*parsing URL module*/
int ParseURL(char* filename);

/*parsing appendix module*/
int ParseAppendix(char* filedirname);

#endif
