TARGET=libregex.so
TEST=test.exe
CC=gcc
CPP=g++
LINKS=-lpcre
objs=url.o mobile.o idCard.o email.o main.o ipnum.o compileRegur.o bankCard.o loadstr.o
Cfile=url.c mobile.c idCard.c email.c main.c ipnum.c compileRegur.c bankCard.c loadstr.c
HeadF=  all.h url.h mobile.h idCard.h email.h ipnum.h compileRegur.h bankCard.h loadstr.h
#WALL=-Wall
WALL=-Werror
DEBUG=-g


all:$(TARGET)


%.o:%.c $(HeadF)
	$(CC) $(DEBUG) $(WALL) -fPIC -c $< -o $@

$(TARGET):$(objs)
	$(CC) -shared $(objs) -o $(TARGET) $(LINKS)

run:  $(TARGET) $(TEST)
	./$(TEST)
	
$(TEST):test.o $(HeadF)
	$(CC)  test.o -o $(TEST) ./$(TARGET)
	
	
.PHONY:clean
clean:
	-rm $(TARGET) $(objs) $(TEST) test.o;
	clear;
	@echo "clean done"
