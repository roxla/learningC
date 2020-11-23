CC=gcc

C_SOURCE=$(wildcard src/*.c , main/*.c)
TARGET=bin/main

$(TARGET):$(C_SOURCE)
	$(CC) $^ -o $@ -I ./include -lpthread 

.PHONY:clean

clean:
	$(RM) $(TARGET)