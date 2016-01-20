CFLAGS:=-Wall -Wextra -g -fsanitize=address
LDFLAGS:=-lasan -lowfat
SOURCES:=main.c hash.c scan.c
OBJECTS:=$(SOURCES:.c=.o)
EXECUTABLE:=blatt11

all: $(EXECUTABLE)

run: all
	./$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	gcc $(LDFLAGS) -o $@ $(OBJECTS)

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)
