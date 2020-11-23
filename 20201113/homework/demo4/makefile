CC=gcc

C_SOURCE=$(wildcard src/*.c)
C_MAIN=$(wildcard main/*.c)
TARGET=bin/main
INCLUDE_PATH=-I ./include
LIB_PATH=-L ./lib -lggy -lpthread 
LIB_NAME=lib/libggy.a

OBJ=$(patsubst %.c,%.o,$(C_SOURCE))

%.o:%.c
	$(CC) $^ -o $@ -c $(INCLUDE_PATH)

$(LIB_NAME):$(OBJ)
	ar rcs $@ $^

bin/main:$(C_MAIN)
	$(CC) $^ -o $@ $(INCLUDE_PATH) $(LIB_PATH)

.PHONY:clean

clean:
	$(RM) $(TARGET)
	$(RM) src/*.o
	$(RM) $(LIB_NAME)