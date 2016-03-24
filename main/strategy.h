/*************************************************************************
	> File Name: strategy.h
	> Author: 
	> Mail: 
	> Created Time: 2016年03月24日 星期四 13时07分50秒
 ************************************************************************/

#ifndef GMIME_STRATEGY_H
#define GMIME_STRATEGY_H

/*定义关键字类型*/
#define STA_VIRUS 0x80
#define STA_SPAM  0x40
#define STA_WORDS 0x20
#define STA_CLASS 0x10

#define STA_SRC   0x08
#define STA_TER   0x04

int strategy_init(void);

#endif
