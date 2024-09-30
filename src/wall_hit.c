#include "raycasting.h"

void init_ray_direction(float *rayDirX, float *rayDirY, float rayAngle)
{
    *rayDirX = cos(rayAngle);
    *rayDirY = sin(rayAngle);
}

void init_map_coordinates(float playerX, float playerY, int *mapX, int *mapY)
{
    *mapX = (int)playerX;
    *mapY = (int)playerY;
}

void calculate_step_and_initial_side_dist(float playerX, float playerY,
                                          int *mapX, int *mapY,
                                          float rayDirX, float rayDirY,
                                          int *stepX, int *stepY,
                                          float *sideDistX, float *sideDistY,
                                          float deltaDistX, float deltaDistY)
{
    if (rayDirX < 0)
    {
        *stepX = -1;
        *sideDistX = (playerX - *mapX) * deltaDistX;
    }
    else
    {
        *stepX = 1;
        *sideDistX = (*mapX + 1.0 - playerX) * deltaDistX;
    }

    if (rayDirY < 0)
    {
        *stepY = -1;
        *sideDistY = (playerY - *mapY) * deltaDistY;
    }
    else
    {
        *stepY = 1;
        *sideDistY = (*mapY + 1.0 - playerY) * deltaDistY;
    }
}

int dda_raycast_step(int *mapX, int *mapY, float *sideDistX, float *sideDistY,
                float deltaDistX, float deltaDistY, int stepX, int stepY,
                int *side, GameStats *gameStats)
{
    int hit = 0;

    while (hit == 0)
    {
        if (*sideDistX < *sideDistY)
        {
            *sideDistX += deltaDistX;
            *mapX += stepX;
            *side = 0;
        }
        else
        {
            *sideDistY += deltaDistY;
            *mapY += stepY;
            *side = 1;
        }

        if (*mapX >= 0 && *mapX < MAP_WIDTH && *mapY >= 0 &&
            *mapY < MAP_HEIGHT && gameStats->maze_map[*mapX][*mapY] == 1)
        {
            hit = 1;
        }
    }

    return hit;
}




float calculate_wall_x(float playerX, float playerY, int side,
                       float rayDirX, float rayDirY,
                       int mapX, int mapY, int stepX, int stepY)
{
    // Step 1: Calculate the perpendicular wall distance (fish-eye correction)
    float perpWallDist;
    if (side == 0)
        perpWallDist = (mapX - playerX + (1 - stepX) / 2) / rayDirX;
    else
        perpWallDist = (mapY - playerY + (1 - stepY) / 2) / rayDirY;

    // Step 2: Calculate the exact hit position (wallX) for texture mapping
    float wallX;
    if (side == 0)
        wallX = playerY + perpWallDist * rayDirY;  // Use perpendicular distance here
    else
        wallX = playerX + perpWallDist * rayDirX;  // Use perpendicular distance here

    // Get the fractional part of wallX (for texture mapping)
    wallX -= floor(wallX);

    // Use perpWallDist for correct wall scaling, and wallX for texture mapping
    return wallX;
}






float get_wall_hit_coordinates(GameStats *gameStats, float playerX,
                               float playerY, float rayAngle,
                               int *mapX, int *mapY)
{
    float rayDirX, rayDirY, sideDistX, sideDistY, wallX;
    int stepX, stepY, side;

    init_ray_direction(&rayDirX, &rayDirY, rayAngle);
    init_map_coordinates(playerX, playerY, mapX, mapY);

    float deltaDistX = fabs(1 / rayDirX);
    float deltaDistY = fabs(1 / rayDirY);

    calculate_step_and_initial_side_dist(playerX, playerY, mapX, mapY,
                                         rayDirX, rayDirY,
                                         &stepX, &stepY,
                                         &sideDistX, &sideDistY,
                                         deltaDistX, deltaDistY);

    dda_raycast_step(mapX, mapY, &sideDistX, &sideDistY,
                deltaDistX, deltaDistY, stepX, stepY,
                &side, gameStats);

    // Call calculate_wall_x with the updated parameter list
    wallX = calculate_wall_x(playerX, playerY, side, rayDirX, rayDirY,
                             *mapX, *mapY, stepX, stepY);

    return wallX;
}
