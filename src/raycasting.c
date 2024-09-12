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
        float rayDirX = player.angle + cameraX; // Adjust based on player's angle
        float rayDirY = player.y; // This should be used for vertical direction adjustment

        // This is a placeholder for actual raycasting and collision detection logic
        // You should implement a proper raycasting algorithm here

        // Example code to visualize ray direction
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for rays
        SDL_RenderDrawLine(renderer, player.x, player.y, player.x + rayDirX * 50, player.y + rayDirY * 50); // Draw ray direction line
    }

    /* Present the rendered frame */
    SDL_RenderPresent(renderer);
}
