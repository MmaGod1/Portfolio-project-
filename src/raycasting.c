#include <SDL2/SDL.h>
#include <math.h>
#include "player.h"

/* Define map dimensions */
#define MAP_WIDTH 10
#define MAP_HEIGHT 10

/* Define the map (1 = wall, 0 = floor) */
int map[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

/* Function to draw the scene */
void draw(SDL_Renderer *renderer, Player player) {
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    /* Clear screen with sky blue for the ceiling */
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Sky blue
    SDL_RenderClear(renderer);

    /* Draw the floor */
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for the floor
    SDL_Rect floor_rect = {0, height / 2, width, height / 2};
    SDL_RenderFillRect(renderer, &floor_rect);

    /* Raycasting loop */
    for (int x = 0; x < width; x++) {
        /* Calculate ray angle */
        float cameraX = 2 * x / (float)width - 1; // Coordinate in camera space
        float rayDirX = player.angle + player.x + cameraX;
        float rayDirY = player.y;
        
        /* Raycasting */
        // This should be more detailed, involving collision detection and rendering
        // Basic example: vertical line based on map value
        int mapX = (int)player.x;
        int mapY = (int)player.y;

        // Assuming simple vertical wall detection
        if (map[mapY][mapX] == 1) {
            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Gray for walls
            SDL_RenderDrawLine(renderer, x, height / 2, x, height / 2 - 50); // Draw wall slice
        }
    }

    /* Present the rendered frame */
    SDL_RenderPresent(renderer);
}
