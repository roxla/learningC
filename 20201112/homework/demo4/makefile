

bin/main:main/main.c src/init_pool.c src/add_task.c src/add_threads.c src/rmmove_threads.c src/destroy_pool.c src/thread_routine.c
	gcc $^ -o $@ -I ./include -lpthread 

clean:
	rm bin/main