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
JPEGLibs=jpegparse.so
PDFLibs=pdfpharse.so
SPAMLibs=libantispam.so
VIRUSLibs=libantivirus.so
DLPLibs=dlp.so
MP3Libs=MP3libs.so
MP4Libs=MP4libs.so
OfficeLibs=OfficeLibs.so

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
	$(APPENDIXLibs) $(JPEGLibs) $(PDFLibs)\
	$(SPAMLibs) $(VIRUSLibs)  $(ALLLIBS) $(MP3Libs) $(MP4Libs) $(OfficeLibs)
	$(CC) $(LIBS) -o $(TARGET) ./$(GMIMELibs)\
		./$(NLPIRLibs) ./$(REGEXLibs) ./$(SPLITERLibs)\
		./$(APPENDIXLibs)  ./$(JPEGLibs)\
		./$(PDFLibs) ./$(SPAMLibs) ./$(VIRUSLibs) ./$(OfficeLibs) ./$(MP3Libs) ./$(MP4Libs) \
		 $(LINKS) -ldl



%.o:%.c $(CFILES) $(ALLHEADS)
	$(CC) -fPIC $(DEBUG) $(WALL) -c $(INCLUDE) $< -o $@


debug:$(TARGET)
	gdb --args $(TARGET) testdata/6677.eml

$(DLPLibs):  $(GMIMELibs) $(NLPIRLibs) $(REGEXLibs) $(SPLITERLibs)\
	$(APPENDIXLibs)  $(JPEGLibs) $(PDFLibs)\
	$(SPAMLibs) $(VIRUSLibs)  $(ALLLIBS) $(LIBS) $(MP3Libs) $(MP4Libs) $(OfficeLibs)
	gcc -shared $(LIBS) -o $(DLPLibs) $(LINKS) -ldl  ./$(GMIMELibs)\
		./$(NLPIRLibs) ./$(REGEXLibs) ./$(SPLITERLibs)\
		./$(APPENDIXLibs)  ./$(JPEGLibs) ./$(OfficeLibs) ./$(MP3Libs) ./$(MP4Libs) \
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

$(MP3Libs):
	cd ../appendix/media; make $(MP3Libs);
	cp ../appendix/media/$(MP3Libs) ./
	
$(MP4Libs):
	cd ../appendix/media; make $(MP4Libs);
	cp ../appendix/media/$(MP4Libs) ./
	
$(OfficeLibs):
	cd ../appendix/office; make $(OfficeLibs);
	cp ../appendix/office/$(OfficeLibs) ./


run:
	./$(TARGET) testdata/6677.eml

.PHONY:clean
clean:
	-rm $(TARGET) $(LIBS) $(DLPLibs);
	-rm $(TARGET) $(ALLLIBS);
	-rm -r $(ADDATION);
	-rm $(ADDATIONFILE);
	-rm -rf $(ADDATIONFOLDE);
	-rm $(GMIMELibs) $(REGEXLibs) $(SPLITERLibs)\
		$(APPENDIXLibs)  $(JPEGLibs)\
		$(PDFLibs) $(SPAMLibs) $(VIRUSLibs) \
		$(MP3Libs) $(MP4Libs) $(OfficeLibs);
	cd ../gmime; 		make clean;
	cd ../regex; 		make clean;
	cd ../spliter;	 	make clean;
	cd ../appendix;	 	make clean;
	cd ../appendix/pdf;  	make clean;
	cd ../appendix/jpeg; 	make clean;
	cd ../antivirus/spam; 	make clean;
	cd ../antivirus/virus; 	make clean;
	cd ../appendix/media;  	make clean;
	cd ../appendix/office; 	make clean;
	clear;
	@echo 'all clean done!'
