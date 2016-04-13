TARGET=test.exe
dyLIBS=uncompress.so


CFILE=main.c getsubstr.c uncompress.c
CHEADS=all.h getsubstr.h uncompress.h
COBJS=		 getsubstr.o uncompress.o
testOBJS=main.o
CC=gcc
DEBUG=-g

all:$(TARGET)
	@echo 'test for this makefile'
run:
	./$(TARGET) testfile/final.tar.bz2

$(TARGET):$(testOBJS) $(dyLIBS)
	$(CC)  $(testOBJS) -o $(TARGET) ./$(dyLIBS)
 
$(dyLIBS):$(COBJS)
	$(CC) -shared $(COBJS) -o $(dyLIBS)
	
%.o:%.c $(HEAD)
	$(CC) -fPIC -Wall $(DEBUG) -c $< -o $@

.PHONY:clean

clean:
	-rm $(COBJS) $(TARGET) $(dyLIBS);
	-rm -rf temp;
	@echo 'clean done!';
	clear
