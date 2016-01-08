LIB :=		`pkg-config --cflags --libs gdk-pixbuf-2.0`
CFLAGS :=	-std=gnu11 -Wall -c

main: main.c 
	gcc $(CFLAGS) main.c $(LIB)
	gcc $(CFLAGS) output.c $(LIB)
	gcc $(CFLAGS) iterator.c $(LIB)
	gcc $(CFLAGS) colorize.c $(LIB)
	gcc -o fractal colorize.o main.o output.o iterator.o $(LIB) -lm

clean:
	rm -rf ./fractal
	rm -f *.o
