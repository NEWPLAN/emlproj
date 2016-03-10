TARGET=unzips.so
DEBUG=-g
all:
	gcc $(DEBUG) -fPIC -shared  main.c -o $(TARGET) -lpthread


.PHONY:clean

clean:
	-rm $(TARGET) -r appendix ;clear;

run:
	./$(TARGET) MyProject.zip
