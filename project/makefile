TARGET = ./bin/project

C_MAIN = $(wildcard main/*.c)
C_FUN = $(wildcard src/*.c)

INCLUDE_PATH = -I ./include
LIB_PTHREAD = -lpthread

$(TARGET):$(C_FUN) $(C_MAIN)
	$(CC) $^ -o $@ $(INCLUDE_PATH) -Wall $(LIB_PTHREAD)

run:
	$(TARGET)

clear:
	$(RM) $(TARGET)