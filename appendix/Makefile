TARGET=appendix.so

JPEGLibs=jpegparse.so
PDFLibs=pdfpharse.so
#UNZIPSLibs=unzips.so
MP3Libs=MP3libs.so
MP4Libs=MP4libs.so
OfficeLibs=OfficeLibs.so

#LinkLibs=./jpegparse.so ./pdfpharse.so ./unzips.so
INPUTFILE=main.c getsubstr.c
HEAD=all.h getsubstr.h
OBJS=main.o getsubstr.o
CC=gcc
DEBUG=-g
WARNING= -Werror
#WARNING= -Wall

all:$(TARGET)
	@echo 'test for this makefile'
run:
	./$(TARGET) 1234.ppt.jpeg.rmvb.mp4

$(TARGET):$(OBJS) $(JPEGLibs) $(PDFLibs)  $(MP3Libs) $(MP4Libs) $(OfficeLibs)
	$(CC) -shared $(OBJS) -o $(TARGET) ./$(JPEGLibs) ./$(PDFLibs)  ./$(OfficeLibs) ./$(MP3Libs) ./$(MP4Libs)
 
%.o:%.c $(HEAD)
	$(CC) -fPIC $(WARNING) $(DEBUG) -c $< -o $@

$(JPEGLibs):
	cd jpeg; make;
	cp jpeg/$(JPEGLibs) ./


$(PDFLibs):
	cd pdf; make;
	cp pdf/$(PDFLibs) ./

$(MP3Libs):
	cd media; make $(MP3Libs);
	cp media/$(MP3Libs) ./
	
$(MP4Libs):
	cd media; make $(MP4Libs);
	cp media/$(MP4Libs) ./
	
$(OfficeLibs):
	cd office; make $(OfficeLibs);
	cp office/$(OfficeLibs) ./
	
.PHONY:clean

clean:
	-rm $(OBJS) $(TARGET);
	-rm $(PDFLibs) $(JPEGLibs) $(MP3Libs) $(MP4Libs) $(OfficeLibs);
	@echo 'clean done!';
	clear
