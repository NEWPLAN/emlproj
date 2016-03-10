INCLUDE= -D_LARGEFILE64_SOURCE -pthread -I/opt/gmime/include/gmime-2.6\
		 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
LIBS= -L/opt/gmime/lib -lgmime-2.6 -lgio-2.0 -lgobject-2.0 -lglib-2.0
CC=gcc
TARGET=gmimelibs.so
DEBUG=-g
WERROR= -Wall

HEADS= inits.h all.h heads.h messageB.h EnableSwitch.h basicInfo.h messageParser.h
OBJS= main.o inits.o heads.o messageB.o basicInfo.o messageParser.o

all: $(TARGET)

%.o : %.c
	$(CC) -fPIC $(DEBUG) $(WERROR) $(INCLUDE) -c $< -o $@  $(HEDAS)
$(TARGET):$(OBJS)
	$(CC) -fPIC -shared  $(OBJS) -o $(TARGET) $(LIBS)
run:
	@echo 'test for basic email analyse!';
	./$(TARGET) testFile/22222.eml 
clean:
	-rm $(OBJS) ;
	-rm $(TARGET) ;
	clear
