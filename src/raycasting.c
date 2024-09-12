#include <SDL2/SDL.h>
#include "init.h"

/* Draws walls and ground using raycasting */
void draw(SDL_Renderer *renderer) {
    /* Define the map (example map size 10x10) */
    int map[10][10] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 1, 0, 1, 1, 1, 0, 0, 1},
        {1, 0, 1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    /* Define the size of each cell */
    const int cellWidth = 80;
    const int cellHeight = 60;

    /* Clear screen */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  /* Set color to black for background */
    SDL_RenderClear(renderer);

    /* Draw walls */
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  /* Set color to red for walls */
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            if (map[y][x] == 1) {
                SDL_Rect rect = {x * cellWidth, y * cellHeight, cellWidth, cellHeight};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    /* Draw ground/ceiling */
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  /* Set color to green for ground/ceiling */
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            if (map[y][x] == 0) {
                SDL_Rect rect = {x * cellWidth, y * cellHeight, cellWidth, cellHeight};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    /* Present the renderer */
    SDL_RenderPresent(renderer);
}
