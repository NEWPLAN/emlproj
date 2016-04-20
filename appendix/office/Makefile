target=OfficeLibs.so

WARNING= -Wall

DEBUG=-g
CC=gcc

CFiles=office_extract.cxx
Cheads=office_extract.h
CObjs=office_extract.o

CXX=g++
COMPFLAGS=-I. -I/home/host1/libreoffice5.0_sdk/LINUXexample.out/inc -I/home/host1/libreoffice5.0_sdk/LINUXexample.out/inc/examples -I/opt/libreoffice5.0/sdk/include -I/home/host1/libreoffice5.0_sdk/LINUXexample.out/inc/Test -DUNX -DGCC -DLINUX -DCPPU_ENV=gcc3 -DHAVE_GCC_VISIBILITY_FEATURE
MYLIKS= -L"/home/host1/libreoffice5.0_sdk/LINUXexample.out/lib" -L"/opt/libreoffice5.0/sdk/lib" -L"/opt/libreoffice5.0/sdk/../program" -luno_cppuhelpergcc3 -luno_cppu -luno_salhelpergcc3 -luno_sal

all:$(target)
	@echo 'make done';
	
	
$(target):$(CObjs)
	$(CXX) -shared $(CObjs) -o $(target) $(MYLIKS)

%.o:%.cxx $(Cheads)
	$(CXX) $(DEBUG)  $(WARNING) -fPIC $(COMPFLAGS) -c $< -o $@

run:
	

.PHONY:clean
clean:
	-rm $(target);
	-rm $(CObjs);
	-rm *.txt;
	clear;
	@echo "clean done!"
	
BC:
	g++ -fPIC -shared office_extract.cxx -I. -I/home/host1/libreoffice5.0_sdk/LINUXexample.out/inc -I/home/host1/libreoffice5.0_sdk/LINUXexample.out/inc/examples -I/opt/libreoffice5.0/sdk/include -I/home/host1/libreoffice5.0_sdk/LINUXexample.out/inc/Test -DUNX -DGCC -DLINUX -DCPPU_ENV=gcc3 -DHAVE_GCC_VISIBILITY_FEATURE -L"/home/host1/libreoffice5.0_sdk/LINUXexample.out/lib" -L"/opt/libreoffice5.0/sdk/lib" -L"/opt/libreoffice5.0/sdk/../program" -o office_extract.so -luno_cppuhelpergcc3 -luno_cppu -luno_salhelpergcc3 -luno_sal
