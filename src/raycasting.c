#include <SDL2/SDL.h>
#include <math.h>
#include "init.h"

/* Define constants for the maze and display */
#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define CELL_SIZE 80
#define FOV 60  // Field of view in degrees
#define VIEW_DIST 1000 // Maximum view distance


void draw(SDL_Renderer *renderer, Player player) {
    /* Define the map (example map size 10x10) */
    int map[MAP_HEIGHT][MAP_WIDTH] = {
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

    /* Clear screen */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  /* Black background */
    SDL_RenderClear(renderer);

    /* Cast rays to draw walls */
    for (int x = 0; x < 640; x++) {  // Assuming screen width is 640
        // Calculate the angle of the ray
        float rayAngle = (player.angle - FOV / 2.0f) + ((float)x / 640.0f) * FOV;

        // Cast the ray
        float rayX = player.x;
        float rayY = player.y;
        float stepSize = 0.1f;
        float distanceToWall = 0.0f;
        int hit = 0;

        while (distanceToWall < VIEW_DIST) {
            rayX += cosf(rayAngle) * stepSize;
            rayY += sinf(rayAngle) * stepSize;
            distanceToWall += stepSize;

            int mapX = (int)(rayX / CELL_SIZE);
            int mapY = (int)(rayY / CELL_SIZE);

            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
                hit = 1;  // Ray has gone out of bounds
                break;
            }

            if (map[mapY][mapX] == 1) {
                hit = 1;  // Wall hit
                break;
            }
        }

        // Draw the wall slice
        int wallHeight = (int)(CELL_SIZE / distanceToWall * 200);  // Scale factor for height
        SDL_Rect rect = {x, 300 - wallHeight / 2, 1, wallHeight};  // Assuming screen height is 600

        if (hit) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White color for walls
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black color if no wall hit
        }

        SDL_RenderFillRect(renderer, &rect);
    }

    /* Present the renderer */
    SDL_RenderPresent(renderer);
}
