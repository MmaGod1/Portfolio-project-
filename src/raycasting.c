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
        float rayDirX = cos(player.angle) + cameraX * sin(player.angle); // Adjust based on player's angle
        float rayDirY = sin(player.angle) - cameraX * cos(player.angle);

        /* Ray start position */
        float rayPosX = player.x;
        float rayPosY = player.y;

        /* Raycasting algorithm */
        float stepSize = 0.1f; // This could be adjusted
        float rayLength = 0.0f;
        int hit = 0;

        while (rayPosX >= 0 && rayPosX < MAP_WIDTH && rayPosY >= 0 && rayPosY < MAP_HEIGHT) {
            // Check for wall hit
            if (map[(int)rayPosY][(int)rayPosX] == 1) {
                hit = 1;
                break;
            }

            rayPosX += rayDirX * stepSize;
            rayPosY += rayDirY * stepSize;
            rayLength += stepSize;
        }

        // Draw the wall based on ray length
        if (hit) {
            int wallHeight = height / rayLength; // Simple wall height calculation
            int wallTop = (height / 2) - (wallHeight / 2);
            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Gray for walls
            SDL_Rect wallRect = {x, wallTop, 1, wallHeight};
            SDL_RenderFillRect(renderer, &wallRect);
        }
    }

    /* Present the rendered frame */
    SDL_RenderPresent(renderer);
}
