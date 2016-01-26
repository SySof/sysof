CFLAGS:=-Wall -Wextra -g
LDFLAGS:=-lowfat
SOURCES:=main.c
OBJECTS:=$(SOURCES:.c=.o)
EXECUTABLE:=blatt12

all: $(EXECUTABLE)

run: all
	./$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	gcc -o $@ $(OBJECTS) $(LDFLAGS)

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)
