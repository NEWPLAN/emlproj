ccccc=DocToText - Converts DOC, XLS, XLSB, PPT, RTF, ODF (ODT, ODS, ODP),OOXML (DOCX, XLSX, PPTX), iWork (PAGES, NUMBERS, KEYNOTE),           ODFXML (FODP, FODS, FODT), PDF, EML and HTML documents to plain text.           Extracts metadata and annotations.

OfficeLibs=OfficeLibs.so
TESTRTE=test.exe

WARNING= -Werror

DEBUG=-g
CC=gcc

CFiles=officeparser.cpp
Cheads=officeparser.h 
CObjs=officeparser.o 

TFolder=temps

CXX=g++
COMPFLAGS=-I/usr/lib/doctotext/ -L/usr/lib/doctotext/
MYLIKS= -L/usr/lib/doctotext/ -ldoctotext

all:$(OfficeLibs)
	@echo 'make done';
	
	
$(OfficeLibs):$(CObjs)
	$(CXX) -shared $(CObjs) -o $(OfficeLibs) $(MYLIKS)

%.o:%.cpp $(Cheads)
	$(CXX) $(DEBUG)  $(WARNING) -fPIC $(COMPFLAGS) -c $< -o $@

run: $(OfficeLibs)
	mkdir $(TFolder);
	$(CC) main.c -o $(TESTRTE) ./$(OfficeLibs)
	./$(TESTRTE)

.PHONY:clean
clean:
	-rm $(OfficeLibs) *.txt $(TESTRTE);
	-rm $(CObjs);
	-rm *.txt;
	-rm -rf $(TFolder);
	clear;
	@echo "clean done!"
	
