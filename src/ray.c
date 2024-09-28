#include "raycasting.h"

float cast_ray(float playerX, float playerY, float rayAngle)
{
/* Ray direction */
float rayDirX = cos(rayAngle);
    float rayDirY = sin(rayAngle);

    // Which box of the map we're in
    int mapX = (int)playerX;
    int mapY = (int)playerY;

    // Length of ray from one x or y side to next x or y side
    float deltaDistX = fabs(1 / rayDirX);
    float deltaDistY = fabs(1 / rayDirY);

    // What direction to step in (+1 or -1)
    int stepX, stepY;
    float sideDistX, sideDistY;

    // Calculate step and initial sideDist
    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (playerX - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1.0 - playerX) * deltaDistX;
    }
    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (playerY - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1.0 - playerY) * deltaDistY;
    }

    // Perform DDA
    bool hit = false;
    int side; // was the wall hit on the X or Y side?
    while (!hit) {
        // Jump to next map square, either in x-direction or y-direction
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0; // X-side hit
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1; // Y-side hit
        }

        // Check if ray has hit a wall
        if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT && maze_map[mapX][mapY] == 1) {
            hit = true;
        }
}
