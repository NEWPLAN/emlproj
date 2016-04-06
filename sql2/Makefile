# this is for sql test files

INCLUDE +=	-I/usr/include/mysql
LINKS	+=	-lmysqlclient -L/usr/lib/mysql
TARGET=test
CC=gcc
WALL=-Wall
LIBS=keywordtype.o keywordcheck.o main.o spamcheck.o urlcheck.o virus.o assist.o dboperate.o
CFILES=keywordtype.c keywordcheck.c main.c spamcheck.c urlcheck.c virus.c assist.c dboperate.c
HEADS=all.h assist.h dboperate.h

all:$(LIBS)
	$(CC) $(LIBS) -o $(TARGET) $(LINKS)
%.o:%.c $(CFILES)
	$(CC) $(WALL) -c $(INCLUDE) $< -o $@
.PHONY:clean
clean:
	-rm $(TARGET) $(LIBS);
	clear;
