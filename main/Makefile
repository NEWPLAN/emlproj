TARGET=eml.exe
DEBUG=-g
ADDATION=testfile/appendix 
ADDATIONFILE=temps/* testfile/*
ABSPATH=`pwd`

GMIMELibs=gmimelibs.so
NLPIRLibs=libNLPIR.so
REGEXLibs=regex.so
SPLITERLibs=spliter.so
APPENDIXLibs=appendix.so
UNZIPSLibs=unzips.so
JPEGLibs=jpegparse.so
PDFLibs=pdfpharse.so
SPAMLibs=libantispam.so
VIRUSLibs=libantivirus.so

all:$(TARGET)

$(TARGET):$(GMIMELibs) $(NLPIRLibs) $(REGEXLibs) $(SPLITERLibs)\
	$(APPENDIXLibs) $(UNZIPSLibs) $(JPEGLibs) $(PDFLibs)\
	$(SPAMLibs) $(VIRUSLibs)
	gcc $(DEBUG) main.c -o $(TARGET) ./$(GMIMELibs)\
		./$(NLPIRLibs) ./$(REGEXLibs) ./$(SPLITERLibs)\
		./$(APPENDIXLibs) ./$(UNZIPSLibs) ./$(JPEGLibs)\
		./$(PDFLibs) ./$(SPAMLibs) ./$(VIRUSLibs) -ldl

$(GMIMELibs):
	cd ../gmime; 		make;
	cp ../gmime/$(GMIMELibs) 		./

$(NLPIRLibs):

$(REGEXLibs):
	cd ../regex; 		make;
	cp ../regex/$(REGEXLibs) 		./

$(SPLITERLibs):
	cd ../spliter; 			make;
	cp ../spliter/$(SPLITERLibs) 		./

$(APPENDIXLibs):
	cd ../appendix; 		make;
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

test:
	./$(TARGET) testdata/66666.eml

.PHONY:clean

clean:
	-rm $(TARGET);
	-rm -r $(ADDATION);
	-rm $(ADDATIONFILE);
	-rm $(GMIMELibs) $(REGEXLibs) $(SPLITERLibs)\
		$(APPENDIXLibs) $(UNZIPSLibs) $(JPEGLibs)\
		$(PDFLibs) $(SPAMLibs) $(VIRUSLibs);
	cd ../gmime; 		make clean;
	cd ../regex; 		make clean;
	cd ../spliter;	 		make clean;
	cd ../appendix;	 		make clean;
	cd ../appendix/zips; 	make clean;
	cd ../appendix/pdf;  	make clean;
	cd ../appendix/jpeg; 	make clean;
	cd ../antivirus/spam; 	make clean;
	cd ../antivirus/virus; 	make clean;
	clear;
	@echo 'all clean done!'
