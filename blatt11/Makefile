CFLAGS:=-Wall -Wextra -Werror=all -g
LDFLAGS:=/usr/lib/libowfat.a
SOURCES:=main.c hash.c
OBJECTS:=$(SOURCES:.c=.o)
EXECUTABLE:=blatt11

all: $(EXECUTABLE)

run: all
	./$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	gcc $(LDFLAGS) -o $@ $(OBJECTS)

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)
