libs=libantispam.so
TARGET=test.exe
DEBUGS=-g 
CXX=gcc
OUTlib=test.o
Cfile=antispam.c basicControl.c handleconf.c handleWhiteBlackList.c test.c
objs=antispam.o basicControl.o handleconf.o handleWhiteBlackList.o
headers=antispam.h handleconf.h handleWhiteBlackList.h mystruct.h
WARNING=-Wall

all:$(libs) $(OUTlib) 
	$(CXX) $(OUTlib) -o $(TARGET) ./$(libs)

%.o:%.c $(headers)
	gcc $(WARNING) $(DEBUGS) -fPIC -c $< -o $@

$(libs):$(objs)
	$(CXX) -shared $(objs) -o $(libs)

.PHONY:clean

clean:
	-rm $(TARGET);
	-rm $(objs);
	-rm $(OUTlib);
	-rm $(libs);
	clear;
