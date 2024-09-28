#include "raycasting.h"

/**
 * calculate_step_and_side_dist - Calculates the step direction and
 * initial side distances for DDA.
 *
 * @rayDirX: The X direction of the ray.
 * @rayDirY: The Y direction of the ray.
 * @playerX: The player's X position.
 * @playerY: The player's Y position.
 * @mapX: Pointer to the current map's X coordinate.
 * @mapY: Pointer to the current map's Y coordinate.
 * @stepX: Pointer to store the step in the X direction.
 * @stepY: Pointer to store the step in the Y direction.
 * @sideDistX: Pointer to store the initial distance to the next X side.
 * @sideDistY: Pointer to store the initial distance to the next Y side.
 */
void calculate_step_and_side_dist(float rayDirX, float rayDirY, float playerX, float playerY, int *mapX, int *mapY, int *stepX, int *stepY, float *sideDistX, float *sideDistY) 
{
    float deltaDistX = fabs(1 / rayDirX);
    float deltaDistY = fabs(1 / rayDirY);

    if (rayDirX < 0) {
        *stepX = -1;
        *sideDistX = (playerX - *mapX) * deltaDistX;
    } else {
        *stepX = 1;
        *sideDistX = (*mapX + 1.0 - playerX) * deltaDistX;
    }

    if (rayDirY < 0) {
        *stepY = -1;
        *sideDistY = (playerY - *mapY) * deltaDistY;
    } else {
        *stepY = 1;
        *sideDistY = (*mapY + 1.0 - playerY) * deltaDistY;
    }
}

/**
 * perform_DDA - Performs the Digital Differential Analyzer (DDA) algorithm
 * to find the next wall hit by the ray.
 *
 * @stepX: The step in the X direction.
 * @stepY: The step in the Y direction.
 * @sideDistX: Pointer to the distance to the next X side.
 * @sideDistY: Pointer to the distance to the next Y side.
 * @mapX: Pointer to the current X coordinate in the map.
 * @mapY: Pointer to the current Y coordinate in the map.
 * @hit: Pointer to an integer indicating if a wall has been hit.
 * @side: Pointer to an integer indicating which side was hit (0 for X, 1 for Y).
 */
void perform_DDA(int stepX, int stepY, float *sideDistX, float *sideDistY, int *mapX, int *mapY, int *hit, int *side) 
{
    while (*hit == 0) {
        if (*sideDistX < *sideDistY) {
            *sideDistX += fabs(1 / cos(stepX));
            *mapX += stepX;
            *side = 0;  // Hit on the X side
        } else {
            *sideDistY += fabs(1 / sin(stepY));
            *mapY += stepY;
            *side = 1;  // Hit on the Y side
        }

        if (maze_map[*mapX][*mapY] == 1) {
            *hit = 1;  // Wall hit
        }
    }
}

/**
 * cast_ray - Casts a ray from the player's position to determine
 * the distance to the nearest wall.
 *
 * @playerX: The X-coordinate of the player's position.
 * @playerY: The Y-coordinate of the player's position.
 * @rayAngle: The angle at which the ray is cast.
 *
 * Return: The perpendicular distance from the player to the wall
 * hit by the ray.
 */
float cast_ray(float playerX, float playerY, float rayAngle) 
{
    float rayDirX = cos(rayAngle);
    float rayDirY = sin(rayAngle);
    int mapX = (int)playerX;
    int mapY = (int)playerY;
    float sideDistX, sideDistY;
    int stepX, stepY, hit = 0, side;
    float perpWallDist;

    calculate_step_and_side_dist(rayDirX, rayDirY, playerX, playerY, &mapX, &mapY, &stepX, &stepY, &sideDistX, &sideDistY);
    perform_DDA(stepX, stepY, &sideDistX, &sideDistY, &mapX, &mapY, &hit, &side);

    if (side == 0) {
        perpWallDist = (mapX - playerX + (1 - stepX) / 2) / rayDirX;
    } else {
        perpWallDist = (mapY - playerY + (1 - stepY) / 2) / rayDirY;
    }

    return perpWallDist;
}

/**
 * get_wall_hit_coordinates - Determines the exact fractional X-coordinate
 * of the wall hit by the ray from the player's position.
 *
 * @playerX: The X-coordinate of the player's position.
 * @playerY: The Y-coordinate of the player's position.
 * @rayAngle: The angle at which the ray is cast.
 * @mapX: A pointer to an integer that will be set to the X-coordinate of the hit wall in the map.
 * @mapY: A pointer to an integer that will be set to the Y-coordinate of the hit wall in the map.
 *
 * Return: The fractional X-coordinate of the wall hit relative to its width.
 */
float get_wall_hit_coordinates(float playerX, float playerY, float rayAngle, int *mapX, int *mapY) 
{
    float rayDirX = cos(rayAngle);
    float rayDirY = sin(rayAngle);
    float sideDistX, sideDistY, wallX;
    int stepX, stepY, hit = 0, side;

    *mapX = (int)playerX;
    *mapY = (int)playerY;

    calculate_step_and_side_dist(rayDirX, rayDirY, playerX, playerY, mapX, mapY, &stepX, &stepY, &sideDistX, &sideDistY);
    perform_DDA(stepX, stepY, &sideDistX, &sideDistY, mapX, mapY, &hit, &side);

    if (side == 0) {
        // Wall hit on the X side: calculate based on Y position
        wallX = playerY + ((mapX - playerX + (1 - stepX) / 2) / rayDirX) * rayDirY;
    } else {
        // Wall hit on the Y side: calculate based on X position
        wallX = playerX + ((mapY - playerY + (1 - stepY) / 2) / rayDirY) * rayDirX;
    }
    wallX -= floor(wallX);  // Get the fractional part of the hit position

    return wallX;
}
