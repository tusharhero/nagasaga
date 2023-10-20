test : main.c
	gcc -g3 -fsanitize=address,undefined -Wall -lSDL2 -lm main.c
	./a.out
build : main.c
	gcc main.c -lSDL2 -lm
run : a.out
	./a.out
