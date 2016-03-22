/*************************************************************************
	> File Name: all.h
	> Author: 
	> Mail: 
	> Created Time: 2016Äê02ÔÂ17ÈÕ ÐÇÆÚÈý 13Ê±40·Ö52Ãë
 ************************************************************************/

#ifndef EML_ALL_H
#define EML_ALL_H

/*ÌíŒÓgmimeµÄœÓ¿ÚÍ·ÎÄŒþ*/
#include "../gmime/all.h"

/*·ÖŽÊ¿âœÓ¿Ú*/
#include "../spliter/allhead.h"

/*ÌíŒÓÕýÔò¹æÔò£¬¹ØŒü×ÖÀàœÓ¿Ú*/
#include "../regex/all.h"

/*ÌíŒÓžœŒþœÓ¿Ú*/
#include "../appendix/all.h"

/*ÌíŒÓ·ŽÀ¬»ø¿âœÓ¿Ú*/
#include "../antivirus/spam/antispam.h"

/*ÌíŒÓ·Ž²¡¶Ÿ¿âœÓ¿Ú*/
#include "../antivirus/virus/antivirus.h"

/*Load db from Database*/
#include "../mysql/all.h"

/*œâÎöemlÎÄŒþ*/
int ParseEML(char* filename);

/*URLŽŠÀí*/
int ParseURL(char* filename);

/*¹ØŒü×Ö¹ýÂË*/
int ParseKeyChs(char* filename);
/*¹ØŒü×ÖÀàŽŠÀí*/
int ParseKeyClass(char* filename);

/*žœŒþŽŠÀí*/
int ParseAppendix(char* filedirname);

/*·Ž²¡¶ŸŽŠÀí*/
int AntiSpams(char* filename);

/*·ŽÀ¬»øŽŠÀí*/
int AntiVirus(char* filepath);

/*加载数据库信息*/


#endif
