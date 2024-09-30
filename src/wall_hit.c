 #include "raycasting.h"


/**
 * init_ray_direction - Initializes the ray direction based on the angle.
 *
 * @rayDirX: Pointer to store the X component of the ray direction.
 * @rayDirY: Pointer to store the Y component of the ray direction.
 * @rayAngle: The angle of the ray in radians.
 *
 * Calculates the ray direction using cosine and sine of the given angle.
 */
void init_ray_direction(float *rayDirX, float *rayDirY, float rayAngle)
{
    *rayDirX = cos(rayAngle);
    *rayDirY = sin(rayAngle);
}


/**
 * init_map_coordinates - Initializes the map coordinates based on player position.
 *
 * @playerX: The player's X position in the world.
 * @playerY: The player's Y position in the world.
 * @mapX: Pointer to store the corresponding map X coordinate.
 * @mapY: Pointer to store the corresponding map Y coordinate.
 *
 * Converts the player's floating-point position to integer map coordinates.
 */
void init_map_coordinates(float playerX, float playerY, int *mapX, int *mapY)
{
    *mapX = (int)playerX;
    *mapY = (int)playerY;
}


/**
 * calculate_step_and_initial_side_dist - Determines the step direction 
 * and initial side distance for raycasting.
 *
 * @playerX: The player's X position in the world.
 * @playerY: The player's Y position in the world.
 * @mapX: Pointer to the current map X coordinate.
 * @mapY: Pointer to the current map Y coordinate.
 * @rayDirX: The X component of the ray direction.
 * @rayDirY: The Y component of the ray direction.
 * @stepX: Pointer to store the step direction in the X direction.
 * @stepY: Pointer to store the step direction in the Y direction.
 * @sideDistX: Pointer to store the initial side distance in the X direction.
 * @sideDistY: Pointer to store the initial side distance in the Y direction.
 * @deltaDistX: The distance to move in the X direction per step.
 * @deltaDistY: The distance to move in the Y direction per step.
 *
 * Initializes step directions and calculates initial distances for the raycast.
 */
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


/**
 * dda_raycast_step - Performs DDA (Digital Differential Analyzer) steps 
 * to find the next wall hit in the raycasting algorithm.
 *
 * @mapX: Pointer to the current map X coordinate.
 * @mapY: Pointer to the current map Y coordinate.
 * @sideDistX: Pointer to the current side distance in the X direction.
 * @sideDistY: Pointer to the current side distance in the Y direction.
 * @deltaDistX: The distance to move in the X direction per step.
 * @deltaDistY: The distance to move in the Y direction per step.
 * @stepX: The step direction in the X direction.
 * @stepY: The step direction in the Y direction.
 * @side: Pointer to store the side hit (0 for vertical, 1 for horizontal).
 * @gameStats: Pointer to the GameStats struct containing maze data.
 *
 * Continues stepping until a wall is hit, updating the map coordinates 
 * and side distances accordingly.
 */
int dda_raycast_step(int *mapX, int *mapY, float *sideDistX, float *sideDistY,
                float deltaDistX, float deltaDistY, int stepX, int stepY,
                int *side, GameStats *gameStats, float rayAngle)
{
    int hit = 0;
    float perpWallDist = 0.0f;

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




/**
 * calculate_wall_x - Calculates the precise wall hit coordinates for texture mapping.
 *
 * @playerX: The player's X position in the world.
 * @playerY: The player's Y position in the world.
 * @side: The side of the wall hit (0 for vertical, 1 for horizontal).
 * @sideDistX: The distance to the next wall in the X direction.
 * @sideDistY: The distance to the next wall in the Y direction.
 * @deltaDistX: The distance to move in the X direction per step.
 * @deltaDistY: The distance to move in the Y direction per step.
 * @rayDirX: The X component of the ray direction.
 * @rayDirY: The Y component of the ray direction.
 *
 * Computes the wall hit coordinates relative to the texture coordinates.
 */
/*float calculate_wall_x(float playerX, float playerY, int side,
                       float sideDistX, float sideDistY,
                       float deltaDistX, float deltaDistY,
                       float rayDirX, float rayDirY)
{
    float wallX;

    if (side == 0)
    {
        wallX = playerY + ((sideDistX - deltaDistX) * rayDirY);
    }
    else
    {
        wallX = playerX + ((sideDistY - deltaDistY) * rayDirX);
    }

    wallX -= floor(wallX);

    return (wallX);*/

    int hit = 0;
    float perpWallDist;  // Perpendicular distance to eliminate fish-eye

    while (hit == 0)
    {
        if (*sideDistX < *sideDistY)
        {
            *sideDistX += deltaDistX;
            *mapX += stepX;
            *side = 0;

            perpWallDist = (*sideDistX - deltaDistX) / cos(rayAngle);  // Fish-eye correction
        }
        else
        {
            *sideDistY += deltaDistY;
            *mapY += stepY;
            *side = 1;

            perpWallDist = (*sideDistY - deltaDistY) / cos(rayAngle);  // Fish-eye correction
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





/**
 * get_wall_hit_coordinates - Determines the wall hit coordinates during raycasting.
 *
 * @gameStats: Pointer to the GameStats struct containing maze data.
 * @playerX: The player's X position in the world.
 * @playerY: The player's Y position in the world.
 * @rayAngle: The angle of the ray in radians.
 * @mapX: Pointer to store the map X coordinate of the wall hit.
 * @mapY: Pointer to store the map Y coordinate of the wall hit.
 *
 * Initializes ray parameters, calculates step direction, performs DDA steps,
 * and calculates wall hit coordinates for rendering.
 */
/*float get_wall_hit_coordinates(GameStats *gameStats, float playerX,
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

    wallX = calculate_wall_x(playerX, playerY, side, sideDistX, sideDistY,
                             deltaDistX, deltaDistY, rayDirX, rayDirY);

    return (wallX);
}
*/



float get_wall_hit_coordinates(GameStats *gameStats, float playerX,
                               float playerY, float rayAngle,
                               int *mapX, int *mapY)
{
    float rayDirX, rayDirY, sideDistX, sideDistY, wallX;
    int stepX, stepY, side;

    // Initialize ray direction based on rayAngle
    init_ray_direction(&rayDirX, &rayDirY, rayAngle);
    
    // Initialize map coordinates
    init_map_coordinates(playerX, playerY, mapX, mapY);

    // Calculate delta distances for raycasting
    float deltaDistX = fabs(1 / rayDirX);
    float deltaDistY = fabs(1 / rayDirY);

    // Calculate the steps and initial side distances for DDA
    calculate_step_and_initial_side_dist(playerX, playerY, mapX, mapY,
                                         rayDirX, rayDirY,
                                         &stepX, &stepY,
                                         &sideDistX, &sideDistY,
                                         deltaDistX, deltaDistY);

    // Perform DDA raycasting step to find wall hit
    dda_raycast_step(mapX, mapY, &sideDistX, &sideDistY,
                     deltaDistX, deltaDistY, stepX, stepY,
                     &side, gameStats);

    // Calculate the exact wall hit coordinate for texture mapping
    wallX = calculate_wall_x(playerX, playerY, side, sideDistX, sideDistY,
                             deltaDistX, deltaDistY, rayDirX, rayDirY);

    return wallX;  // Return the texture mapping coordinate
}
