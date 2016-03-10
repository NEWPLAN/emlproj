TARGET=jpegparse.so

DEBGU=-g

CC=gcc
CPP=g++

LINK=-lexif


CFILE3=main.c

all: $(TARGET)


$(TARGET):$(CFILE3)
	$(CC) $(DEBGU) -fPIC -shared $(CFILE3) -o $(TARGET) $(LINK)

run:
	./$(TARGET) Jellyfish.jpg 

clean:
	-rm $(TARGET)  ; clear
