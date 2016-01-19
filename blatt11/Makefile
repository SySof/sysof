CFLAGS:=-Wall -Wextra -Werror=all -g
LDFLAGS:=-lowfat
SOURCES:=main.c hash.c
OBJECTS:=$(SOURCES:.c=.o)
EXECUTABLE:=blatt11

all: $(EXECUTABLE)

run: all
	./$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	gcc $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)
