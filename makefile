test : main.c
	gcc main.c -lSDL2 -lm
	./a.out
build : main.c
	gcc main.c -lSDL2 -lm
run : a.out
	./a.out
