CC = clang
CFLAGS = -lm -g -Wall -Werror -fsanitize=address

engine:
	$(CC) engine.c geometry.c vector.c -o engine $(CFLAGS)
	./engine

engine_debug:
	$(CC) vector_test.c geometry.c vector.c -o vec_test $(CFLAGS)
	./vec_test

clean:
	rm engine vec_test

.PHONY: engine engine_debug clean
