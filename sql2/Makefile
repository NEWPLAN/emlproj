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

ALLLIBS+=main.o kmp.o 
ALLCFILE+=main.c kmp.c 
ALLHEADS+=all.h mainall.h


INCLUDE +=	-I/usr/include/mysql
LINKS	+=	-lmysqlclient -L/usr/lib/mysql
TARGET=test
CC=gcc
WALL=
LIBS=keywordtype.o keywordcheck.o main.o spamcheck.o urlcheck.o virus.o assist.o dboperate.o kmp.o
CFILES=keywordtype.c keywordcheck.c main.c spamcheck.c urlcheck.c virus.c assist.c dboperate.c kmp.c
HEADS=all.h assist.h dboperate.h kmp.h mainall.h

all:$(LIBS) $(GMIMELibs) $(NLPIRLibs) $(REGEXLibs) $(SPLITERLibs)\
	$(APPENDIXLibs) $(UNZIPSLibs) $(JPEGLibs) $(PDFLibs)\
	$(SPAMLibs) $(VIRUSLibs)  $(ALLLIBS)
	$(CC) $(LIBS) -o $(TARGET) ./$(GMIMELibs)\
		./$(NLPIRLibs) ./$(REGEXLibs) ./$(SPLITERLibs)\
		./$(APPENDIXLibs) ./$(UNZIPSLibs) ./$(JPEGLibs)\
		./$(PDFLibs) ./$(SPAMLibs) ./$(VIRUSLibs) \
		 $(LINKS) -ldl
%.o:%.c $(CFILES)
	$(CC) $(DEBUG) $(WALL) -c $(INCLUDE) $< -o $@

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
	-rm $(TARGET) $(LIBS);
	-rm $(TARGET) $(ALLLIBS);
	-rm -r $(ADDATION);
	-rm $(ADDATIONFILE);
	-rm $(GMIMELibs) $(REGEXLibs) $(SPLITERLibs)\
		$(APPENDIXLibs) $(UNZIPSLibs) $(JPEGLibs)\
		$(PDFLibs) $(SPAMLibs) $(VIRUSLibs) ;
	clear;
