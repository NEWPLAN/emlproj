MP3Libs=MP3libs.so
MP4Libs=MP4libs.so
TESTTAR=test.exe
CXX=g++
CCC=gcc
SHARED_LIBFLAGS= -shared
COMPFLAGS= -fPIC
MP3Flags= -ltag -liconv
MP4Flags= -lmediainfo -lzen -D_UNICODE
OBJS=  mp4parse.o mp3parse.o main.o
Cfile= mp4parse.c mp3parse.c main.c
Cheads= mp4parse.h mp3parse.h
DEBUG=-g
TFile=temps

all:$(MP3Libs) $(MP4Libs)
	@echo 'make done';

$(MP3Libs):mp3parse.o
	$(CXX) $(SHARED_LIBFLAGS) mp3parse.o -o $(MP3Libs) $(MP3Flags)
	
$(MP4Libs):mp4parse.o
	$(CXX) $(SHARED_LIBFLAGS) mp4parse.o -o $(MP4Libs) $(MP4Flags)
	
%.o:%.cpp $(Cheads)
	$(CXX) $(DEBUG) -c $(COMPFLAGS) $< -o $@ -D_UNICODE

run:main.o $(MP3Libs) $(MP4Libs)
	mkdir $(TFile);
	$(CCC) $(DEBUG) main.o -o $(TESTTAR) ./$(MP3Libs) ./$(MP4Libs);
	./$(TESTTAR)

.PHONY:clean
clean:
	-rm $(MP3Libs) $(MP4Libs) $(OBJS) *.txt $(TESTTAR);
	-rm -rf $(TFile);
	clear;
	@echo 'clean done';
	
