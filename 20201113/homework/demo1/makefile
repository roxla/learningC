CC=gcc

C_SOURCE=main/main.c src/init_pool.c src/add_task.c src/add_threads.c src/rmmove_threads.c src/destroy_pool.c src/thread_routine.c
TARGET=bin/main

$(TARGET):$(C_SOURCE)
	$(CC) $^ -o $@ -I ./include -lpthread 

clean:
	$(RM) $(TARGET)