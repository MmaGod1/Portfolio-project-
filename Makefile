CC=gcc
CFLAGS=-c -Wall -std=c99
LDFLAGS=-lSDL2

SOURCES=main.c map.c camera.c raycasting.c
OBJECTS=main.o map.o camera.o raycasting.o
EXECUTABLE=raycasting

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

main.o: main.c
	$(CC) $(CFLAGS) main.c -o main.o

map.o: map.c
	$(CC) $(CFLAGS) map.c -o map.o

camera.o: camera.c
	$(CC) $(CFLAGS) camera.c -o camera.o

raycasting.o: raycasting.c
	$(CC) $(CFLAGS) raycasting.c -o raycasting.o

clean:
	rm -f *.o $(EXECUTABLE)
