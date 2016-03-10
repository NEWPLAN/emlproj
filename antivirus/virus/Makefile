INCLUDE= 
LIBS= -lclamav
CC=gcc
DLibs=libantivirus.so
TARGET=test.exe
DEBUG=-g
FPIC=-fPIC
SHAREDLIBS=-shared
WERROR= -Wall

HEADS= antivirus.h
OBJS=  antivirus.o 

all: $(TARGET)

$(TARGET):test.o $(DLibs)
	$(CC) test.o ./$(DLibs) -o $(TARGET) 

%.o : %.c
	$(CC)  $(DEBUG) $(FPIC) $(WERROR) $(INCLUDE) -c $< -o $@  $(HEDAS)
$(DLibs):$(OBJS)
	$(CC) $(SHAREDLIBS)  $(OBJS) -o $(DLibs) $(LIBS)
run:
	@echo 'test for basic email analyse!';
	./$(TARGET) 
clean:
	-rm $(OBJS) ;
	-rm $(DLibs);
	-rm test.o ;
	-rm $(TARGET) ;
	clear
