CC=gcc
CFLAGS=-c -Wall -std=c99
LDFLAGS=-lSDL2

SOURCES=main.c map.c camera.c raycasting.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=raycasting

all: $(SOURCES) $(EXECUTABLE)
\t$(EXECUTABLE): $(OBJECTS)
\t$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
\t$(CC) $(CFLAGS) $< -o $@

clean:
\trm -f *.o $(EXECUTABLE)
