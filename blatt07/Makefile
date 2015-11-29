
run: blatt07
	./blatt07

debug: blatt07
	gdb ./blatt07 -ex run

all: blatt07

blatt07: blatt07.c
	gcc $< -o $@ -std=c89 -pedantic-errors -lm -Wall -Wextra -Werror -g

clean:
	rm -rf ./blatt07

