CFLAGS := -std=gnu11 -Wall -c -g
LDFLAGS :=
SOURCES=main.c heap.c traverse.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=blatt10

all: $(EXECUTABLE)

run: all
	./$(EXECUTABLE)

main.o: main.c heap.h traverse.h
	gcc $(CFLAGS) $< -o $@

heap.o: heap.c heap.h
	gcc $(CFLAGS) $< -o $@

traverse.o: traverse.c traverse.h
	gcc $(CFLAGS) $< -o $@

blatt10: $(OBJECTS)
	gcc $(LDFLAGS) -o $@ $(OBJECTS)

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)
