TARGET=dp.exe

GMIMELibs=gmimelibs.so
NLPIRLibs=libNLPIR.so
REGEXLibs=libregex.so
SPLITERLibs=spliter.so
APPENDIXLibs=appendix.so
JPEGLibs=jpegparse.so
PDFLibs=pdfpharse.so
SPAMLibs=libantispam.so
VIRUSLibs=libantivirus.so
SQLDBLibs=sqlDB.so
DLPLibs=dlp.so

MP3Libs=MP3libs.so
MP4Libs=MP4libs.so
OfficeLibs=OfficeLibs.so

#WARINING=-Wall
WARINING=-Werror

FPIC=-fPIC

#SHARELIBS=-shared

LINKS= -lssl -lcrypto -lpthread

Heads	+=	emailhead.h gateway.h getline.h getline_ssl.h log.h parsefile.h proxy.h proxy_imap.h proxy_pop3.h proxy_smtp.h util.h

Cfiles	+=	emailbody.c	gateway.c getline.c getline_ssl.c log.c parsefile.c proxy.c proxy_imap.c proxy_pop3.c proxy_smtp.c util.c

ALLLIBS	+=	emailbody.o	gateway.o getline.o getline_ssl.o log.o parsefile.o proxy.o proxy_imap.o proxy_pop3.o proxy_smtp.o util.o

CC		=	gcc
DEBUGS	=	-g



	
DEBUG=-g
ADDATION=testfile/appendix 
ADDATIONFILE=temps/* testfile/* 
ADDATIONFOLDE+=running*

DISABLEWEAR=-fno-stack-protector


CC=gcc
#WALL=-Werror 

WALL=-Wall

all:$(TARGET)
	@echo 'make done';

	
$(TARGET): $(ALLLIBS) $(DLPLibs)  $(GMIMELibs) $(NLPIRLibs) $(REGEXLibs) $(SPLITERLibs)\
	$(APPENDIXLibs)  $(JPEGLibs) $(PDFLibs) $(MP3Libs) $(MP4Libs) $(OfficeLibs) \
	$(SPAMLibs) $(VIRUSLibs) 
	gcc $(ALLLIBS) -o $(TARGET) ./$(DLPLibs)  ./$(GMIMELibs) ./$(NLPIRLibs) \
	./$(REGEXLibs) ./$(SPLITERLibs)	./$(APPENDIXLibs) \
	./$(JPEGLibs) ./$(PDFLibs) ./$(SPAMLibs) ./$(VIRUSLibs) ./$(MP3Libs) ./$(MP4Libs) ./$(OfficeLibs)  $(LINKS)



%.o:%.c $(CFILES) $(ALLHEADS)
	$(CC) -fPIC $(DEBUG) $(WALL) -c $(INCLUDE) $< -o $@


debug:$(TARGET)
	gdb --args $(TARGET) ../main/testdata/6666.eml

$(DLPLibs):  $(GMIMELibs) $(NLPIRLibs) $(REGEXLibs) $(SPLITERLibs)\
	$(APPENDIXLibs) $(JPEGLibs) $(PDFLibs)\
	$(SPAMLibs) $(VIRUSLibs)  $(ALLLIBS) $(LIBS)
	cd ../sql2; make $(DLPLibs);
	cp ../sql2/$(DLPLibs) ./

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
	./$(TARGET)

.PHONY:clean
clean:
	-rm $(TARGET) $(OBJS);
	-rm $(TARGET) $(LIBS) $(DLPLibs);
	-rm $(TARGET) $(ALLLIBS);
	-rm -r $(ADDATION);
	-rm $(ADDATIONFILE);
	-rm -rf $(ADDATIONFOLDE);
	-rm $(GMIMELibs) $(REGEXLibs) $(SPLITERLibs)\
		$(APPENDIXLibs) $(JPEGLibs) $(MP3Libs) $(MP4Libs) $(OfficeLibs) \
		$(PDFLibs) $(SPAMLibs) $(VIRUSLibs) ;
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
	cd ../sql2;		make clean;
	clear;
	@echo 'all clean done!'

