/*************************************************************************
	> File Name: all.h
	> Author: 
	> Mail: 
	> Created Time: 2016年02月17日 星期三 13时40分52秒
 ************************************************************************/

#ifndef EML_ALL_H
#define EML_ALL_H

/*添加gmime的接口头文件*/
#include "../gmime/src/all.h"

/*分词库接口*/
#include "../spliter/allhead.h"

/*添加正则规则，关键字类接口*/
#include "../regex/src/all.h"

/*添加附件接口*/
#include "../appendix/all.h"

/*添加反垃圾库接口*/
#include "../antivirus/spam/antispam.h"

/*添加反病毒库接口*/
#include "../antivirus/virus/antivirus.h"

/*解析eml文件*/
int ParseEML(char* filename);

/*URL处理*/
int ParseURL(char* filename);

/*关键字过滤*/
int ParseKeyChs(char* filename);
/*关键字类处理*/
int ParseKeyClass(char* filename);

/*附件处理*/
int ParseAppendix(char* filedirname);

/*反病毒处理*/
int AntiSpams(char* filename);

/*反垃圾处理*/
int AntiVirus(char* filepath);


#endif
