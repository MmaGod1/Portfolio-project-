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

/* Function to draw the player */

void draw(SDL_Renderer *renderer, Player player) {
    /* Clear the screen with sky blue */
    SDL_SetRenderDrawColor(renderer, 135, 206, 250, 255); // Sky blue
    SDL_RenderClear(renderer);

    /* Draw the floor and ceiling */
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for floor
    SDL_Rect floorRect = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &floorRect);

    SDL_SetRenderDrawColor(renderer, 135, 206, 250, 255); // Sky blue for ceiling
    SDL_Rect ceilingRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &ceilingRect);

    /* Raycasting logic */
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float cameraX = 2 * x / (float)SCREEN_WIDTH - 1;
        float rayDirX = player.angle + cameraX; // Adjusted ray direction
        float rayDirY = player.y; // Adjusted ray direction

        /* Raycasting calculations */
        // For simplicity, we'll use placeholder values for now
        int wallHeight = SCREEN_HEIGHT / 2; // Example height

        /* Determine wall color based on orientation */
        SDL_Color wallColor;
        if (rayDirX > rayDirY) { // Example condition for direction
            wallColor = (SDL_Color){128, 128, 128, 255}; // Gray for wall facing EAST/WEST
        } else {
            wallColor = (SDL_Color){255, 0, 0, 255}; // Red for wall facing NORTH/SOUTH
        }

        /* Draw the wall */
        SDL_SetRenderDrawColor(renderer, wallColor.r, wallColor.g, wallColor.b, wallColor.a);
        SDL_Rect wallRect = {x, SCREEN_HEIGHT / 2 - wallHeight / 2, 1, wallHeight};
        SDL_RenderFillRect(renderer, &wallRect);
    }

    /* Present the rendered frame */
    SDL_RenderPresent(renderer);
}
