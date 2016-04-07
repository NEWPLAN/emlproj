ADDFile=Test_result.txt
TARGET1=ACmachine
Cfile1=ACmachine.cpp
TARGET2=test

TARGET3=spliter.so

LinkLibs=./libNLPIR.so
Comparas=-m64  -O3 -DOS_LINUX
CPP=g++
STD=-std=c++11
DEBUG=-g

objs=main.o ACmachine.o cDemoOK.o hashs.o loadstr.o test.o
inputs=main.cpp ACmachine.cpp cDemoOK.cpp hashs.cpp loadstr.cpp test.cpp
heads=allhead.h  ACmachine.h cDemoOK.h NLPIR.h hashs.h loadstr.h 



#WARNING=-Wall
WARNING= -Werror

all:$(TARGET3)

$(TARGET2):$(objs)
	$(CPP)  $(objs) -o $(TARGET2) $(LinkLibs) -lpthread

$(TARGET3):$(objs)
	$(CPP) -shared $(objs) -o $(TARGET3) $(LinkLibs) -lpthread

run:
	./$(TARGET3)

%.o:%.cpp $(heads)
	$(CPP) $(WARNING) -fPIC $(DEBUG) $(Comparas) $(STD)  -c $< -o $@

.PHONY:clean
clean:
	rm $(TARGET3) $(TARGET2) $(TARGET1) $(objs) ; clear
