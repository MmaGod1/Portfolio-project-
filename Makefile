CC=gcc
CFLAGS=-c -Wall -std=c99
LDFLAGS=-lSDL2

SOURCES=main.c map.c camera.c raycasting.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=raycasting

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
    $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
    $(CC) $(CFLAGS) $< -o $@

clean:
    rm -f *.o $(EXECUTABLE)
