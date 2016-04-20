TARGET=appendix.so

JPEGLibs=jpegparse.so
PDFLibs=pdfpharse.so
UNZIPSLibs=unzips.so

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

$(TARGET):$(OBJS) $(JPEGLibs) $(PDFLibs) $(UNZIPSLibs) 
	$(CC) -shared $(OBJS) -o $(TARGET) ./$(JPEGLibs) ./$(PDFLibs) ./$(UNZIPSLibs) 
 
%.o:%.c $(HEAD)
	$(CC) -fPIC $(WARNING) $(DEBUG) -c $< -o $@

$(JPEGLibs):
	cd jpeg; make;
	cp jpeg/$(JPEGLibs) ./


$(PDFLibs):
	cd pdf; make;
	cp pdf/$(PDFLibs) ./

$(UNZIPSLibs):
	cd zips; make;
	cp zips/$(UNZIPSLibs) ./


.PHONY:clean

clean:
	-rm $(OBJS) $(TARGET);
	-rm $(PDFLibs) $(JPEGLibs) $(UNZIPSLibs);
	@echo 'clean done!';
	clear
