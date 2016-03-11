TARGET=test

#ADDITIONS=-lmysqlclient -I/usr/include/mysql -L/usr/lib/mysql

INCLUDE +=	-I/usr/include/mysql
LINKS	+=	-lmysqlclient -L/usr/lib/mysql
Heads	+=	HYmysql.h
Cfiles	+=	main.c
OBJS	+=	main.o
CC		=	gcc
DEBUGS	=	-g
all:$(OBJS)
	$(CC) $(DEBUGS) $(OBJS) -o $(TARGET) $(LINKS)

%.o:%.c $(Heads)
	$(CC) $(DEBUGS) -c $(INCLUDE)  $< -o $@ 
run:
	./$(TARGET)

.PHONY:
	clean
clean:
	-rm $(TARGET) $(OBJS);
	clear
