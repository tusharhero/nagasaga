test : main.c
	# gcc -Wall main.c -g -lSDL2 -lm
	gcc -fsanitize=address,undefined main.c -g -lSDL2 -lm
	./a.out
build : main.c
	zig cc main.c -g -lSDL2 -lm
run : a.out
	./a.out
