#include "raycasting.h".

/**
 * calculate_step_and_side_dist - Calculate the step direction and
 * initial side distances.
 *
 * @rayDirX: The X direction of the ray.
 * @rayDirY: The Y direction of the ray.
 * @playerX: The player's X position.
 * @playerY: The player's Y position.
 * @mapX: Pointer to the map's X coordinate.
 * @mapY: Pointer to the map's Y coordinate.
 * @stepX: Pointer to store the step in the X direction.
 * @stepY: Pointer to store the step in the Y direction.
 * @sideDistX: Pointer to store the initial distance to the next X side.
 * @sideDistY: Pointer to store the initial distance to the next Y side.
 */
void calculate_step_and_side_dist(float rayDirX, float rayDirY,
                                   float playerX, float playerY,
                                   int *mapX, int *mapY,
                                   int *stepX, int *stepY,
                                   float *sideDistX, float *sideDistY)
{
    if (rayDirX < 0)
    {
        *stepX = -1;
        *sideDistX = (playerX - *mapX) * fabs(1 / rayDirX);
    }
    else
    {
        *stepX = 1;
        *sideDistX = (*mapX + 1.0 - playerX) * fabs(1 / rayDirX);
    }

    if (rayDirY < 0)
    {
        *stepY = -1;
        *sideDistY = (playerY - *mapY) * fabs(1 / rayDirY);
    }
    else
    {
        *stepY = 1;
        *sideDistY = (*mapY + 1.0 - playerY) * fabs(1 / rayDirY);
    }
}

/**
 * perform_DDA - Performs the Digital Differential Analyzer (DDA) algorithm
 *                to find the next wall hit by the ray.
 *
 * @stepX: The step in the x-direction.
 * @stepY: The step in the y-direction.
 * @sideDistX: Pointer to the distance to the next x-side.
 * @sideDistY: Pointer to the distance to the next y-side.
 * @mapX: Pointer to the current x-coordinate in the map.
 * @mapY: Pointer to the current y-coordinate in the map.
 * @hit: Pointer to an integer indicating if a wall has been hit.
 * @side: Pointer to an integer indicating which side was hit (0 for x, 1 for y).
 * @rayAngle: The angle of the ray being cast.
 */
void perform_DDA(int stepX, int stepY, float *sideDistX,
                  float *sideDistY, int *mapX, int *mapY,
                  int *hit, int *side, float rayAngle)
{
    float rayDirX = cos(rayAngle); /* Calculate ray direction in X */
    float rayDirY = sin(rayAngle); /* Calculate ray direction in Y */

    while (*hit == 0)
    {
        if (*sideDistX < *sideDistY)
        {
            *sideDistX += fabs(1 / rayDirX);
            *mapX += stepX;
            *side = 0;
        }
        else
        {
            *sideDistY += fabs(1 / rayDirY);
            *mapY += stepY;
            *side = 1;
        }

        if (maze_map[*mapX][*mapY] == 1)
            *hit = 1;
    }
}

/**
 * get_wall_hit_coordinates - Determines the coordinates of the wall hit
 *                            by a ray from the player's position.
 *
 * @playerX: The X-coordinate of the player's position.
 * @playerY: The Y-coordinate of the player's position.
 * @rayAngle: The angle at which the ray is cast.
 * @mapX: A pointer to an integer set to the X map coordinate of the wall hit.
 * @mapY: A pointer to an integer set to the Y map coordinate of the wall hit.
 *
 * Return: The fractional X-coordinate of the wall hit relative to its width.
 */
float get_wall_hit_coordinates(float playerX,
                                float playerY, float rayAngle,
                                int *mapX, int *mapY)
{
    float rayDirX = cos(rayAngle);
    float rayDirY = sin(rayAngle);
    float sideDistX, sideDistY, wallX;
    int stepX, stepY;
    int hit = 0;
    int side;

    *mapX = (int)playerX;
    *mapY = (int)playerY;
    calculate_step_and_side_dist(rayDirX, rayDirY, playerX, playerY,
                                  mapX, mapY, &stepX, &stepY,
                                  &sideDistX, &sideDistY);

    perform_DDA(stepX, stepY, &sideDistX, &sideDistY, mapX, mapY,
                 &hit, &side, rayAngle); // Added rayAngle here

    if (side == 0)
    {
        wallX = playerY + ((sideDistX - fabs(1 / rayDirX)) * rayDirY);
    }
    else
    {
        wallX = playerX + ((sideDistY - fabs(1 / rayDirY)) * rayDirX);
    }
    wallX -= floor(wallX);

    return (wallX);
}
