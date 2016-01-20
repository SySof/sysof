CFLAGS:=-Wall -Wextra -g
LDFLAGS:=-lowfat
SOURCES:=main.c hash.c scan.c
OBJECTS:=$(SOURCES:.c=.o)
EXECUTABLE:=blatt11

all: $(EXECUTABLE)

run: all
	./$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	gcc -o $@ $(OBJECTS) $(LDFLAGS)

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)
