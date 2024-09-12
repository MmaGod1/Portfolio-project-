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
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float cameraX = 2 * x / (float)SCREEN_WIDTH - 1;  // X-coordinate in camera space

        /* Calculate ray position and direction */
        float rayDirX = player.dirX + player.planeX * cameraX;
        float rayDirY = player.dirY + player.planeY * cameraX;

        /* Which box of the map we're in */
        int mapX = (int)player.posX;
        int mapY = (int)player.posY;

        /* Length of ray from current position to next x or y-side */
        float sideDistX;
        float sideDistY;

        /* Length of ray from one x or y-side to next x or y-side */
        float deltaDistX = fabs(1 / rayDirX);
        float deltaDistY = fabs(1 / rayDirY);
        float perpWallDist;

        /* What direction to step in x or y-direction (either +1 or -1) */
        int stepX;
        int stepY;
        int hit = 0;  // Was there a wall hit?
        int side;     // Was a NS or EW wall hit?

        /* Calculate step and initial sideDist */
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

        /* Perform DDA to find wall */
        while (hit == 0) {
            // Jump to next map square
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            // Check if ray has hit a wall
            if (worldMap[mapX][mapY] > 0) hit = 1;
        }

        // Calculate distance projected on camera direction
        if (side == 0) perpWallDist = (mapX - player.posX + (1 - stepX) / 2) / rayDirX;
        else           perpWallDist = (mapY - player.posY + (1 - stepY) / 2) / rayDirY;

        // Calculate height of line to draw on screen
        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

        // Calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

        // Choose wall color
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Default wall color

        // Draw the vertical stripe
        SDL_RenderDrawLine(renderer, posX, drawStart, posX, drawEnd);
    }

    // Present the renderer
    SDL_RenderPresent(renderer);
}
