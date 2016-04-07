TARGET=sqlDB.so

#WARINING=-Wall
WARINING=-Werror

FPIC=-fPIC

SHARELIBS=-shared

#ADDITIONS=-lmysqlclient -I/usr/include/mysql -L/usr/lib/mysql

INCLUDE +=	-I/usr/include/mysql

LINKS	+=	-lmysqlclient -L/usr/lib/mysql

Heads	+=	all.h  dboperate.h DLP_list_keywords.h behavior_log.h DLP_list_keywordsclass.h keyword_list.h keywordclass_list.h \
				   Protocol_switch.h spam_list.h Statistic_DLP_keywords.h Statistic_DLP_URL.h Syslog.h \
				   Statistic_Emlinfo_basic.h Statistic_keywordclass_Basic.h Statistic_Spam_Basic.h URL_list.h \
				   Statistic_Virus_Basic.h user_info.h virus_list.h Statistic_Protocol.h

Cfiles	+=	main.c dboperate.c DLP_list_keywords.c behavior_log.c DLP_list_keywordsclass.c keyword_list.c keywordclass_list.c \
					Protocol_switch.c spam_list.c Statistic_DLP_keywords.c Statistic_DLP_URL.c Syslog.c \
					Statistic_Emlinfo_basic.c Statistic_keywordclass_Basic.c Statistic_Spam_Basic.c URL_list.c \
					Statistic_Virus_Basic.c user_info.c virus_list.c Statistic_Protocol.c

OBJS	+=	main.o dboperate.o DLP_list_keywords.o behavior_log.o DLP_list_keywordsclass.o keyword_list.o keywordclass_list.o \
					Protocol_switch.o spam_list.o Statistic_DLP_keywords.o Statistic_DLP_URL.o Syslog.o \
					Statistic_Emlinfo_basic.o Statistic_keywordclass_Basic.o Statistic_Spam_Basic.o URL_list.o \
					Statistic_Virus_Basic.o user_info.o virus_list.o Statistic_Protocol.o

CC		=	gcc
DEBUGS	=	-g

all:$(OBJS)
	$(CC) $(SHARELIBS) $(WARINING) $(OBJS) -o $(TARGET) $(LINKS)

%.o:%.c $(Heads)
	$(CC) $(FPIC)  $(WARINING) $(DEBUGS) -c $(INCLUDE)  $< -o $@

run:
	./$(TARGET)

del:
	./$(TARGET) 222
.PHONY:
	clean
clean:
	-rm $(TARGET) $(OBJS);
	clear
