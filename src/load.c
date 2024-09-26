#include "raycasting.h"



Texture loadTexture(const char *filename) {
    SDL_Surface *surface = IMG_Load(filename);
    if (!surface) {
        fprintf(stderr, "Error loading texture: %s\n", IMG_GetError());
        return (Texture){NULL, 0, 0};
    }
    Texture texture;
    texture.texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    texture.width = surface->w;
    texture.height = surface->h;
    return texture;
}


int loadMap(const char *filename, int maze_map[MAP_WIDTH][MAP_HEIGHT]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return -1;
    }

    int ch;  // Declare the variable 'ch' here

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            ch = fgetc(file);
            while (ch == '\r' || ch == '\n') {  // Skip newline and carriage return characters
                ch = fgetc(file);
            }

            if (ch == EOF) {
                fprintf(stderr, "Error reading file %s: unexpected EOF\n", filename);
                fclose(file);
                return -1;
            }

            if (ch == '#') {  // Wall character
                maze_map[x][y] = 1;
            } else if (ch == '.') {  // Empty space character
                maze_map[x][y] = 0;
            } else {
                fprintf(stderr, "Invalid character '%c' in map file at [%d,%d]\n", ch, x, y);
                fclose(file);
                return -1;
            }
        }

        // Skip over remaining characters (newline or carriage return)
        while ((ch = fgetc(file)) == '\r' || ch == '\n');
        if (ch != EOF) {
            ungetc(ch, file);  // Put the last character back if not EOF
        }
    }

    fclose(file);
    return 0;
}
