#include "raycasting.h"

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




float cast_ray(GameStats *gameStats, float playerX, float playerY,
               float rayAngle, float screenWidth, float hFOV) // Change the parameter name
{
    int stepX, stepY, mapX, mapY;
    float sideDistX, sideDistY, deltaDistX, deltaDistY;

    // Initialize the ray for the first ray
    initialize_ray(playerX, playerY, rayAngle, &mapX, &mapY,
                   &stepX, &stepY, &sideDistX, &sideDistY,
                   &deltaDistX, &deltaDistY);

    // Calculate the projection plane width
    float projection_plane_width = 2.0f * tan(hFOV * M_PI / 360.0f); // Use the updated parameter name

    float totalDistance = 0.0f; // This will store the distance of the closest wall hit

    // Loop over each pixel (or ray) on the screen
    for (int i = 0; i < screenWidth; i++) {
        // Calculate the normalized offset for the current ray
        float progress = (i / screenWidth) - 0.5f; // Normalize to [-0.5, 0.5]

        // Adjust the ray direction based on the projection
        float adjustedRayAngle = rayAngle + (progress * projection_plane_width);

        // Re-initialize ray parameters based on the adjusted angle
        initialize_ray(playerX, playerY, adjustedRayAngle, &mapX, &mapY,
                       &stepX, &stepY, &sideDistX, &sideDistY,
                       &deltaDistX, &deltaDistY);

        // Perform ray casting to find the wall distance
        float wallDistance = perform_ray_cast(gameStats, &mapX, &mapY, stepX, stepY,
                                               sideDistX, sideDistY, deltaDistX,
                                               deltaDistY, playerX, playerY, adjustedRayAngle);
        
        // Store the total distance for the closest wall hit (if needed)
        if (i == 0 || wallDistance < totalDistance) {
            totalDistance = wallDistance; // Update if it's the closest hit
        }
    }

    return totalDistance; // Return the distance of the closest wall hit
}
