#!include "raycasting.h"


float castRay(float playerX, float playerY, float rayAngle) {
    // Ray direction
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
            side = 0;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
        }

        // Check if ray has hit a wall
        if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT && maze_map[mapX][mapY] == 1) {
            hit = true;
        }
    }

    // Calculate the distance to the point of impact
    float perpWallDist;
    if (side == 0)
        perpWallDist = (mapX - playerX + (1 - stepX) / 2) / rayDirX;
    else
        perpWallDist = (mapY - playerY + (1 - stepY) / 2) / rayDirY;

    return perpWallDist;
}


float getWallHitCoordinates(float playerX, float playerY, float rayAngle, int *mapX, int *mapY) {
    // DDA (Digital Differential Analyzer) algorithm for raycasting
    float rayDirX = cos(rayAngle);
    float rayDirY = sin(rayAngle);

    // Map grid positions of the player
    *mapX = (int)playerX;
    *mapY = (int)playerY;

    // Length of ray to the next side in X and Y
    float sideDistX, sideDistY;

    // Length of the ray to the next X or Y side (constant)
    float deltaDistX = fabs(1 / rayDirX);
    float deltaDistY = fabs(1 / rayDirY);

    int stepX, stepY;
    int hit = 0;  // Did we hit a wall?
    int side;     // Was it a vertical or horizontal wall hit?

    // Calculate step and initial sideDist
    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (playerX - *mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (*mapX + 1.0 - playerX) * deltaDistX;
    }
    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (playerY - *mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (*mapY + 1.0 - playerY) * deltaDistY;
    }

    // Perform DDA (ray casting)
    while (hit == 0) {
        // Jump to next map square, either in X-direction or Y-direction
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            *mapX += stepX;
            side = 0;
        } else {
            sideDistY += deltaDistY;
            *mapY += stepY;
            side = 1;
        }

        // Check if the ray has hit a wall (1 represents a wall in the maze_map)
        if (maze_map[*mapX][*mapY] > 0) hit = 1;
    }

    // Calculate exact position of where the wall was hit
    float wallX;  // Where the wall was hit (X-coordinate)
    if (side == 0) {
        wallX = playerY + ((sideDistX - deltaDistX) * rayDirY);
    } else {
        wallX = playerX + ((sideDistY - deltaDistY) * rayDirX);
    }
    wallX -= floor(wallX);  // Keep only the fractional part

    return wallX;  // Return the hit coordinate on the wall
}
