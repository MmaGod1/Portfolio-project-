#include <SDL2/SDL.h>
#include <math.h>
#include "player.h"
#include "constants.h"

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

/**
 * draw - Draws the scene using raycasting based on the player's position.
 * @renderer: The SDL renderer.
 * @player: The player's position and angle.
 */
void draw(SDL_Renderer *renderer, Player player) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set background to black
    SDL_RenderClear(renderer);

    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float cameraX = 2 * x / (float)SCREEN_WIDTH - 1;
        float rayDirX = player.dirX + player.planeX * cameraX;
        float rayDirY = player.dirY + player.planeY * cameraX;

        int mapX = (int)player.posX;
        int mapY = (int)player.posY;

        float sideDistX;
        float sideDistY;
        float deltaDistX = fabs(rayDirX) > 0.0001 ? fabs(1 / rayDirX) : 1.0;
        float deltaDistY = fabs(rayDirY) > 0.0001 ? fabs(1 / rayDirY) : 1.0;
        float perpWallDist;

        int stepX, stepY;
        int hit = 0;
        int side;

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (player.posX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - player.posX) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (player.posY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - player.posY) * deltaDistY;
        }

        while (hit == 0) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (mapX >= MAP_WIDTH || mapX < 0 || mapY >= MAP_HEIGHT || mapY < 0) {
                hit = 1; // Exit if out of bounds
            } else if (map[mapX][mapY] > 0) {
                hit = 1;
            }
        }

        if (side == 0) {
            perpWallDist = (mapX - player.posX + (1 - stepX) / 2.0) / rayDirX;
        } else {
            perpWallDist = (mapY - player.posY + (1 - stepY) / 2.0) / rayDirY;
        }

        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

        // Debug prints
        printf("x: %d, drawStart: %d, drawEnd: %d, perpWallDist: %f, side: %d\n", x, drawStart, drawEnd, perpWallDist, side);

        // Set color based on side
        if (side == 0) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for vertical walls
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for horizontal walls
        }

        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }

    SDL_RenderPresent(renderer);
}
