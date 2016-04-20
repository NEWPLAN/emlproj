MP3Libs=MP3libs.so
MP4Libs=MP4libs.so
CXX=g++
CCC=gcc
SHARED_LIBFLAGS= -shared
COMPFLAGS= -fPIC
MP3Flags= -ltag -liconv
MP4Flags= -lmediainfo -lzen -D_UNICODE
OBJS=  mp4parse.o mp3parse.o
Cfile= mp4parse.c mp3parse.c
Cheads= mp4parse.h mp3parse.h

all:$(MP3Libs) $(MP4Libs)
	@echo 'make done';

$(MP3Libs):mp3parse.o
	$(CXX) $(SHARED_LIBFLAGS) mp3parse.o -o $(MP3Libs) $(MP3Flags)
	
$(MP4Libs):mp4parse.o
	$(CXX) $(SHARED_LIBFLAGS) mp4parse.o -o $(MP4Libs) $(MP4Flags)
	
%.o:%.cpp $(Cheads)
	$(CXX) -c $(COMPFLAGS) $< -o $@ -D_UNICODE

.PHONY:clean
clean:
	-rm $(MP3Libs) $(MP4Libs) $(OBJS);
	clear;
	@echo 'clean done';
	
