target=pdfpharse.so

#WARNING= -Wall
WARNING= -Werror

DEBUG=-g
Links=-lpthread
CC=gcc
inputFile=main.c
OBJS=main.o
all:$(target)

$(target):$(OBJS)
	$(CC) -shared $(OBJS) -o $(target) $(Links)

%.o:%.c
	$(CC) $(DEBUG)  $(WARNING) -fPIC -c $< -o $@

run:
	./$(target) linux.pdf -

.PHONY:clean
clean:
	-rm $(target);
	-rm $(OBJS);
	-rm *.txt;
	clear;
	@echo "clean done!"
