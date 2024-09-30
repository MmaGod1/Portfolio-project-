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
                int *side, GameStats *gameStats, float rayAngle)
{
    int hit = 0;
    //float perpWallDist = 0.0f;

    // Raycasting loop to find wall hit
    while (hit == 0)
    {
        if (*sideDistX < *sideDistY)  // Use dereferencing because sideDistX and sideDistY are pointers
        {
            *sideDistX += deltaDistX;
            *mapX += stepX;  // Ensure mapX is a pointer
            *side = 0;       // Indicates a hit on the X side
            perpWallDist = (*sideDistX - deltaDistX) / cos(rayAngle);  // Fish-eye correction
        }
        else
        {
            *sideDistY += deltaDistY;
            *mapY += stepY;  // Ensure mapY is a pointer
            *side = 1;       // Indicates a hit on the Y side
            perpWallDist = (*sideDistY - deltaDistY) / cos(rayAngle);  // Fish-eye correction
        }

        // Ensure gameStats is passed correctly
        if (*mapX >= 0 && *mapX < MAP_WIDTH && *mapY >= 0 &&
            *mapY < MAP_HEIGHT && gameStats->maze_map[*mapX][*mapY] == 1)
        {
            hit = 1;
        }
    }

    return hit;
}


float calculate_wall_x(float playerX, float playerY, int side,
                       float sideDistX, float sideDistY,
                       float deltaDistX, float deltaDistY,
                       float rayDirX, float rayDirY)
{
    float wallX;

    if (side == 0)
    {
        // Wall hit on X-axis
        wallX = playerY + (sideDistX - deltaDistX) * rayDirY;
    }
    else
    {
        // Wall hit on Y-axis
        wallX = playerX + (sideDistY - deltaDistY) * rayDirX;
    }

    // We only need the fractional part of the wall hit position for texture mapping
    wallX -= floor(wallX);

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

    dda_raycast_step(mapX, mapY, &sideDistX, &sideDistY, deltaDistX, deltaDistY, 
                 stepX, stepY, side, gameStats, rayAngle);

    wallX = calculate_wall_x(playerX, playerY, side, sideDistX, sideDistY,
                             deltaDistX, deltaDistY, rayDirX, rayDirY);

    return (wallX);
}
