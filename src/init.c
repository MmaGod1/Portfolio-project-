#include <SDL2/SDL.h>
#include "init.h"

/* Initializes SDL and creates a window and renderer */
int init(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return -1;
    }
    *window = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (!*window) {
        SDL_Quit();
        return -1;
    }
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return -1;
    }
    return 0;
}

/* Cleans up SDL resources */
void cleanup(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
