CC=gcc

C_SOURCE=$(wildcard src/*.c)
C_MAIN=$(wildcard main/*.c)
TARGET=bin/main
INCLUDE_PATH=-I ./include
LIB_PATH=-L ./lib -lmy -lpthread -lm
LIB_NAME=lib/libmy.so

OBJ=$(patsubst %.c,%.o,$(C_SOURCE))

%.o:%.c
	$(CC) $^ -o $@ -c $(INCLUDE_PATH) -fPIC

$(LIB_NAME):$(OBJ)
	$(CC) -fPIC -shared -o $@ $^

bin/main:$(C_MAIN)
	$(CC) $^ -o $@ $(INCLUDE_PATH) $(LIB_PATH) -Wall

.PHONY:clean

clean:
	$(RM) $(TARGET)
	$(RM) src/*.o
	$(RM) $(LIB_NAME)