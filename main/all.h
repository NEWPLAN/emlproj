/*************************************************************************
	> File Name: all.h
	> Author: 
	> Mail: 
	> Created Time: 2016��02��17�� ������ 13ʱ40��52��
 ************************************************************************/

#ifndef EML_ALL_H
#define EML_ALL_H

/*���gmime�Ľӿ�ͷ�ļ�*/
#include "../gmime/src/all.h"

/*�ִʿ�ӿ�*/
#include "../spliter/allhead.h"

/*���������򣬹ؼ�����ӿ�*/
#include "../regex/src/all.h"

/*��Ӹ����ӿ�*/
#include "../appendix/all.h"

/*��ӷ�������ӿ�*/
#include "../antivirus/spam/antispam.h"

/*��ӷ�������ӿ�*/
#include "../antivirus/virus/antivirus.h"

/*����eml�ļ�*/
int ParseEML(char* filename);

/*URL����*/
int ParseURL(char* filename);

/*�ؼ��ֹ���*/
int ParseKeyChs(char* filename);
/*�ؼ����ദ��*/
int ParseKeyClass(char* filename);

/*��������*/
int ParseAppendix(char* filedirname);

/*����������*/
int AntiSpams(char* filename);

/*����������*/
int AntiVirus(char* filepath);


#endif
