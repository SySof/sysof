
run: blatt07
	./blatt07

debug: blatt07
	gdb ./blatt07 -ex run

all: blatt07 minimax

blatt07: blatt07.c
	gcc $< -o $@ -std=c89 -pedantic-errors -lm -Wall -Wextra -Werror -g

minimax: minimax.cc
	g++ $< -o $@ -std=c++11 -lm -Wall -Wextra -Werror -O3

clean:
	rm -rf ./blatt07 ./minimax

