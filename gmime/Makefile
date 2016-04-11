INCLUDE=`pkg-config --cflags gmime-2.6`
LIBS= `pkg-config --libs gmime-2.6`
CC=gcc
TARGET=gmimelibs.so
DISABLEWEAR=-fno-stack-protector
DEBUG=-g
#WERROR= -Wall
WERROR= -Werror

HEADS= inits.h all.h heads.h messageB.h EnableSwitch.h basicInfo.h messageParser.h DataStructure.h
OBJS= main.o inits.o heads.o messageB.o basicInfo.o messageParser.o 

all: $(TARGET)

%.o : %.c
	$(CC) $(DISABLEWEAR) -fPIC $(DEBUG) $(WERROR) $(INCLUDE) -c $< -o $@  $(HEDAS)
$(TARGET):$(OBJS)
	$(CC) $(DISABLEWEAR) -fPIC -shared  $(OBJS) -o $(TARGET) $(LIBS)
run:
	@echo 'test for basic email analyse!';
	./$(TARGET) testFile/22222.eml 
clean:
	-rm $(OBJS) ;
	-rm $(TARGET) ;
	clear
