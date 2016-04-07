TARGET=jpegparse.so

DEBGU=-g

CC=gcc
CPP=g++

LINK=-lexif
#WARNING= -Wall
WARNING= -Werror

CFILE3=main.c

all: $(TARGET)


$(TARGET):$(CFILE3)
	$(CC) $(DEBGU) $(WARNING) -fPIC -shared $(CFILE3) -o $(TARGET) $(LINK)

run:
	./$(TARGET) Jellyfish.jpg 

clean:
	-rm $(TARGET)  ; clear
