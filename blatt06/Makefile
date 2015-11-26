
run: blatt06
	./blatt06

debug: blatt06
	gdb ./blatt06 -ex run

all: blatt06

blatt06: blatt06.c
	gcc $< -o $@ -std=c89 -lm -Wall -Wextra -Werror -g

clean:
	rm -rf ./blatt06
