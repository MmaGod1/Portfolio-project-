CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -Iinc
LDFLAGS = `sdl2-config --cflags --libs`
TARGET = my_project
OBJS = src/init.o src/main.o src/raycasting.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

src/init.o: src/init.c inc/init.h
	$(CC) $(CFLAGS) -c src/init.c -o src/init.o

src/main.o: src/main.c inc/init.h inc/player.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/raycasting.o: src/raycasting.c inc/player.h
	$(CC) $(CFLAGS) -c src/raycasting.c -o src/raycasting.o

clean:
	rm -f $(OBJS) $(TARGET)
