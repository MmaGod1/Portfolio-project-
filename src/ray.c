#include "raycasting.h"


/**
 * initialize_ray - Initializes the parameters needed for raycasting.
 *
 * @playerX: The player's current x-coordinate.
 * @playerY: The player's current y-coordinate.
 * @rayAngle: The angle of the ray being cast.
 * @mapX: Pointer to store the x-coordinate of the map cell.
 * @mapY: Pointer to store the y-coordinate of the map cell.
 * @stepX: Pointer to store the step direction in x.
 * @stepY: Pointer to store the step direction in y.
 * @sideDistX: Pointer to store the distance to the next vertical grid line.
 * @sideDistY: Pointer to store the distance to the next horizontal grid line.
 * @deltaDistX: Pointer to store the distance to the next x grid line.
 * @deltaDistY: Pointer to store the distance to the next y grid line.
 *
 * Computes the ray direction, initializes distances, and determines steps
 * based on the player's position and the ray angle.
 */
void initialize_ray(float playerX, float playerY, float rayAngle,
                    int *mapX, int *mapY, int *stepX, int *stepY,
                    float *sideDistX, float *sideDistY,
                    float *deltaDistX, float *deltaDistY)
{
    float rayDirX = cos(rayAngle);
    float rayDirY = sin(rayAngle);

    *mapX = (int)playerX;
    *mapY = (int)playerY;

    *deltaDistX = fabs(1 / rayDirX);
    *deltaDistY = fabs(1 / rayDirY);

    if (rayDirX < 0)
    {
        *stepX = -1;
        *sideDistX = (playerX - *mapX) * (*deltaDistX);
    }
    else
    {
        *stepX = 1;
        *sideDistX = (*mapX + 1.0 - playerX) * (*deltaDistX);
    }

    if (rayDirY < 0)
    {
        *stepY = -1;
        *sideDistY = (playerY - *mapY) * (*deltaDistY);
    }
    else
    {
        *stepY = 1;
        *sideDistY = (*mapY + 1.0 - playerY) * (*deltaDistY);
    }
}


/**
 * perform_ray_cast - Casts a ray to detect collisions with the maze walls.
 *
 * @gameStats: Pointer to the GameStats struct containing the maze map.
 * @mapX: Pointer to the current x-coordinate in the maze.
 * @mapY: Pointer to the current y-coordinate in the maze.
 * @stepX: The step direction in x for ray traversal.
 * @stepY: The step direction in y for ray traversal.
 * @sideDistX: The current distance to the next vertical grid line.
 * @sideDistY: The current distance to the next horizontal grid line.
 * @deltaDistX: The distance to the next x grid line.
 * @deltaDistY: The distance to the next y grid line.
 * @playerX: The player's current x-coordinate.
 * @playerY: The player's current y-coordinate.
 * @rayAngle: The angle of the ray being cast.
 *
 * Iteratively checks for wall collisions and returns the perpendicular distance
 * to the wall hit.
 */
float perform_ray_cast(GameStats *gameStats, int *mapX, int *mapY,
                       int stepX, int stepY, float sideDistX,
                       float sideDistY, float deltaDistX, float deltaDistY,
                       float playerX, float playerY, float rayAngle)
{
    int side;
    bool hit = false;

    while (!hit)
    {
        if (sideDistX < sideDistY)
        {
            sideDistX += deltaDistX;
            *mapX += stepX;
            side = 0;
        }
        else
        {
            sideDistY += deltaDistY;
            *mapY += stepY;
            side = 1;
        }

        if (*mapX >= 0 && *mapX < MAP_WIDTH && *mapY >= 0 &&
            *mapY < MAP_HEIGHT && gameStats->maze_map[*mapX][*mapY] == 1)
        {
            hit = true;
        }
    }

    float perpWallDist;
    if (side == 0)
    {
        perpWallDist = (*mapX - playerX + (1 - stepX) / 2) / cos(rayAngle);
    }
    else
    {
        perpWallDist = (*mapY - playerY + (1 - stepY) / 2) / sin(rayAngle);
    }

    return (perpWallDist);
}


/**
 * cast_ray - Main function to cast a ray and determine the distance to the wall.
 *
 * @gameStats: Pointer to the GameStats struct containing the maze map.
 * @playerX: The player's current x-coordinate.
 * @playerY: The player's current y-coordinate.
 * @rayAngle: The angle of the ray being cast.
 *
 * Initializes ray parameters and calls perform_ray_cast to find the distance
 * to the nearest wall in the direction of the ray.
 *
 * Return: The distance to the wall hit by the ray.
 */
float cast_ray(GameStats *gameStats, float playerX, float playerY,
                float rayAngle)
{
    int stepX, stepY, mapX, mapY;
    float sideDistX, sideDistY, deltaDistX, deltaDistY;

    initialize_ray(playerX, playerY, rayAngle, &mapX, &mapY,
                   &stepX, &stepY, &sideDistX, &sideDistY,
                   &deltaDistX, &deltaDistY);

    return (perform_ray_cast(gameStats, &mapX, &mapY, stepX, stepY,
                             sideDistX, sideDistY, deltaDistX,
                             deltaDistY, playerX, playerY, rayAngle));
}

