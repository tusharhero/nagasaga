test : main.c
	zig cc -Wall main.c -g -lSDL2 -lm
	./a.out
build : main.c
	zig cc main.c -g -lSDL2 -lm
run : a.out
	./a.out
