TARGET=test

#ADDITIONS=-lmysqlclient -I/usr/include/mysql -L/usr/lib/mysql

INCLUDE +=	-I/usr/include/mysql

LINKS	+=	-lmysqlclient -L/usr/lib/mysql

Heads	+=		   dboperate.h strategywords.h behaviorLog.h strategyclass.h keywords.h keyclass.h \
				   protocol.h spamlist.h statistic_DLP_keywords.h statistic_DLP_url.h syslog.h \
				   statistic_eml_info_basic.h statistic_keyclass_basic.h statistic_spam.h url_list.h \
				   statistic_virus.h user_info.h virus_list.h

Cfiles	+=	main.c dboperate.c strategywords.c behaviorLog.c strategyclass.c keywords.c keyclass.c \
					protocol.c spamlist.c statistic_DLP_keywords.c statistic_DLP_url.c syslog.c \
					statistic_eml_info_basic.c statistic_keyclass_basic.c statistic_spam.c url_list.c \
					statistic_virus.c user_info.c virus_list.c

OBJS	+=	main.o dboperate.o strategywords.o behaviorLog.o strategyclass.o keywords.o keyclass.o \
					protocol.o spamlist.o statistic_DLP_keywords.o statistic_DLP_url.o syslog.o \
					statistic_eml_info_basic.o statistic_keyclass_basic.o statistic_spam.o url_list.o \
					statistic_virus.o user_info.o virus_list.o

CC		=	gcc
DEBUGS	=	-g

all:$(OBJS)
	$(CC) $(DEBUGS) $(OBJS) -o $(TARGET) $(LINKS)

%.o:%.c $(Heads)
	$(CC) $(DEBUGS) -c $(INCLUDE)  $< -o $@ 

run:
	./$(TARGET)

del:
	./$(TARGET) 222
.PHONY:
	clean
clean:
	-rm $(TARGET) $(OBJS);
	clear
