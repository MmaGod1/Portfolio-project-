#ifndef INIT_H
#define INIT_H

#include <SDL2/SDL.h>

/* Initializes SDL and creates a window and renderer */
int init(SDL_Window **window, SDL_Renderer **renderer);

/* Cleans up SDL resources */
void cleanup(SDL_Window *window, SDL_Renderer *renderer);

#endif /* INIT_H */
