TARGET=libregex.so
CC=gcc
CPP=g++
LINKS=-lpcre
objs=url.o mobile.o idCard.o email.o main.o ipnum.o compileRegur.o bankCard.o loadstr.o
Cfile=url.c mobile.c idCard.c email.c main.c ipnum.c compileRegur.c bankCard.c loadstr.c
HeadF=  all.h url.h mobile.h idCard.h email.h ipnum.h compileRegur.h bankCard.h loadstr.h
WALL=-Wall


all:$(TARGET)


%.o:%.c $(HeadF)
	$(CC) $(WALL) -fPIC -c $< -o $@

$(TARGET):$(objs)
	$(CC) -shared $(objs) -o $(TARGET) $(LINKS)

run:
	./$(TARGET)

.PHONY:clean
clean:
	-rm $(TARGET) $(objs);
	clear;
	@echo "clean done"
