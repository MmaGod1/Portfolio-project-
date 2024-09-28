#include "raycasting.h"


float get_wall_hit_coordinates(float playerX, float playerY, float rayAngle, int *mapX, int *mapY)
{
    float rayDirX = cos(rayAngle);
    float rayDirY = sin(rayAngle);

    *mapX = (int)playerX;
    *mapY = (int)playerY;

    float sideDistX, sideDistY, wallX;
    float deltaDistX = fabs(1 / rayDirX);
    float deltaDistY = fabs(1 / rayDirY);

    int stepX, stepY, side, hit = 0;

    /* Determine step direction and initial side distances */
    if (rayDirX < 0)
    {
        stepX = -1;
        sideDistX = (playerX - *mapX) * deltaDistX;
    }
    else
    {
        stepX = 1;
        sideDistX = (*mapX + 1.0 - playerX) * deltaDistX;
    }

    if (rayDirY < 0)
    {
        stepY = -1;
        sideDistY = (playerY - *mapY) * deltaDistY;
    }
    else
    {
        stepY = 1;
        sideDistY = (*mapY + 1.0 - playerY) * deltaDistY;
    }

    /* Perform Digital Differential Analyzer */
    while (hit == 0)
    {
        if (sideDistX < sideDistY)
        {
            sideDistX += deltaDistX;
            *mapX += stepX;
            side = 0;  /* Hit a vertical wall */
        }
        else
        {
            sideDistY += deltaDistY;
            *mapY += stepY;
            side = 1;  /* Hit a horizontal wall */
        }

        /* Check for wall hit */
        if (*mapX >= 0 && *mapX < MAP_WIDTH && *mapY >= 0 && *mapY < MAP_HEIGHT && maze_map[*mapX][*mapY] == 1)
            hit = 1;
    }

    /* Calculate the exact position of where the wall was hit */
    if (side == 0)
        wallX = playerY + ((sideDistX - deltaDistX) * rayDirY);
    else
        wallX = playerX + ((sideDistY - deltaDistY) * rayDirX);
  wallX -= floor(wallX);  /* Keep only the fractional part */

    return (wallX);  /* Return the hit coordinate on the wall */
}
