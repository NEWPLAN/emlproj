/*************************************************************************
	> File Name: DataStructure.h
	> Author: 
	> Mail: 
	> Created Time: 2016年03月23日 星期三 14时15分33秒
 ************************************************************************/

#ifndef GMIME_DATASTRUCTURE_H
#define GMIME_DATASTRUCTURE_H

typedef struct GMIMEDATAS
{
    char* subjects;
    char* from;
    char* to;
    char* replayto;
    char* messageID;
    int   nu;/*抄送的个数*/
}GmimeData,*GmimeDataPtr;


#endif
