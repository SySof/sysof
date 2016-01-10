CFLAGS :=	-std=gnu11 -Wall -c

main: main.c heap.c traverse.c heap.h
	gcc $(CFLAGS) heap.c  
	gcc $(CFLAGS) traverse.c 
	gcc $(CFLAGS) main.c
	gcc -o blatt10 heap.o main.o traverse.o 
clean:
	rm -rf ./blatt10
	rm -f *.o	
