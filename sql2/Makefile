# this is for sql test files
DEBUG=-g
ADDATION=testfile/appendix 
ADDATIONFILE=temps/* testfile/* 
ADDATIONFOLDE+=running*
ABSPATH=`pwd`
DISABLEWEAR=-fno-stack-protector

GMIMELibs=gmimelibs.so
NLPIRLibs=libNLPIR.so
REGEXLibs=libregex.so
SPLITERLibs=spliter.so
APPENDIXLibs=appendix.so
UNZIPSLibs=unzips.so
JPEGLibs=jpegparse.so
PDFLibs=pdfpharse.so
SPAMLibs=libantispam.so
VIRUSLibs=libantivirus.so
SQLDBLibs=sqlDB.so
DLPLibs=dlp.so

ALLLIBS+=main.o kmp.o 
ALLCFILE+=main.c kmp.c 
ALLHEADS+=all.h mainall.h kmp.h


INCLUDE +=	-I/usr/include/mysql
LINKS	+=	-lmysqlclient -L/usr/lib/mysql
TARGET=test
CC=gcc
WALL=-Werror 

#WALL=-Wall
LIBS=keywordtype.o keywordcheck.o main.o spamcheck.o urlcheck.o virus.o assist.o dboperate.o kmp.o moduleswitch.o statistic.o
CFILES=keywordtype.c keywordcheck.c main.c spamcheck.c urlcheck.c virus.c assist.c dboperate.c kmp.c moduleswitch.c statistic.c
HEADS=all.h assist.h dboperate.h kmp.h mainall.h moduleswitch.h statistic.h

all:$(TARGET)
	@echo 'make done';
	
$(TARGET):$(LIBS) $(GMIMELibs) $(NLPIRLibs) $(REGEXLibs) $(SPLITERLibs)\
	$(APPENDIXLibs) $(UNZIPSLibs) $(JPEGLibs) $(PDFLibs)\
	$(SPAMLibs) $(VIRUSLibs)  $(ALLLIBS)
	$(CC) $(LIBS) -o $(TARGET) ./$(GMIMELibs)\
		./$(NLPIRLibs) ./$(REGEXLibs) ./$(SPLITERLibs)\
		./$(APPENDIXLibs) ./$(UNZIPSLibs) ./$(JPEGLibs)\
		./$(PDFLibs) ./$(SPAMLibs) ./$(VIRUSLibs) \
		 $(LINKS) -ldl



%.o:%.c $(CFILES) $(ALLHEADS)
	$(CC) -fPIC $(DEBUG) $(WALL) -c $(INCLUDE) $< -o $@


debug:$(TARGET)
	gdb --args $(TARGET) ../main/testdata/6666.eml

$(DLPLibs):  $(GMIMELibs) $(NLPIRLibs) $(REGEXLibs) $(SPLITERLibs)\
	$(APPENDIXLibs) $(UNZIPSLibs) $(JPEGLibs) $(PDFLibs)\
	$(SPAMLibs) $(VIRUSLibs)  $(ALLLIBS) $(LIBS)
	gcc -shared $(LIBS) -o $(DLPLibs) $(LINKS) -ldl  ./$(GMIMELibs)\
		./$(NLPIRLibs) ./$(REGEXLibs) ./$(SPLITERLibs)\
		./$(APPENDIXLibs) ./$(UNZIPSLibs) ./$(JPEGLibs)\
		./$(PDFLibs) ./$(SPAMLibs) ./$(VIRUSLibs)

$(GMIMELibs):
	cd ../gmime;  make;
	cp ../gmime/$(GMIMELibs) ./

$(NLPIRLibs):

$(REGEXLibs):
	cd ../regex; 		make;
	cp ../regex/$(REGEXLibs)	 	./

$(SPLITERLibs):
	cd ../spliter; 		make;
	cp ../spliter/$(SPLITERLibs) 		./

$(APPENDIXLibs):
	cd ../appendix; 	make;
	cp ../appendix/$(APPENDIXLibs) 		./
$(UNZIPSLibs):
	cd ../appendix/zips; 	make;
	cp ../appendix/zips/$(UNZIPSLibs) 	./
$(JPEGLibs):
	cd ../appendix/jpeg; 	make;
	cp ../appendix/jpeg/$(JPEGLibs) 	./
$(PDFLibs):
	cd ../appendix/pdf; 	make;
	cp ../appendix/pdf/$(PDFLibs) 		./

$(SPAMLibs):
	cd ../antivirus/spam; 	make $(SPAMLibs);
	cp ../antivirus/spam/$(SPAMLibs) 	./

$(VIRUSLibs):
	cd ../antivirus/virus; 	make $(VIRUSLibs);
	cp ../antivirus/virus/$(VIRUSLibs) 	./

run:
	./$(TARGET) ./../main/testdata/6666.eml

.PHONY:clean
clean:
	-rm $(TARGET) $(LIBS) $(DLPLibs);
	-rm $(TARGET) $(ALLLIBS);
	-rm -r $(ADDATION);
	-rm $(ADDATIONFILE);
	-rm -rf $(ADDATIONFOLDE);
	-rm $(GMIMELibs) $(REGEXLibs) $(SPLITERLibs)\
		$(APPENDIXLibs) $(UNZIPSLibs) $(JPEGLibs)\
		$(PDFLibs) $(SPAMLibs) $(VIRUSLibs) ;
	cd ../gmime; 		make clean;
	cd ../regex; 		make clean;
	cd ../spliter;	 	make clean;
	cd ../appendix;	 	make clean;
	cd ../appendix/zips; 	make clean;
	cd ../appendix/pdf;  	make clean;
	cd ../appendix/jpeg; 	make clean;
	cd ../antivirus/spam; 	make clean;
	cd ../antivirus/virus; 	make clean;
	clear;
	@echo 'all clean done!'
