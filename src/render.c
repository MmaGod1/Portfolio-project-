f#include "raycasting.h"


/**
 * render_walls - Renders the walls of the maze based on raycasting.
 *
 * @gameStats: Pointer to the GameStats struct containing rendering data.
 * @player: Pointer to the Player struct containing player position and angle.
 *
 * Loops through each screen column, casts rays to detect wall distances,
 * calculates the wall height, and calls render_wall_segment to draw each
 * wall segment.
 */
/*void render_walls(GameStats *gameStats, Player *player)
{
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        float rayAngle = player->angle - (FOV / 2) + 
                         (FOV * x / SCREEN_WIDTH);
        float distance = cast_ray(gameStats, player->x, player->y, 
                                  rayAngle);
        
        if (distance < 0.1)
        {
            distance = 0.1;
        }

        float correctedDistance = distance * 
                                  cos(rayAngle - player->angle);
        
        int wallHeight = (int)(SCREEN_HEIGHT / correctedDistance);
        int wallTop = (SCREEN_HEIGHT / 2) - (wallHeight / 2);
        int wallBottom = (SCREEN_HEIGHT / 2) + (wallHeight / 2);
        
        if (wallTop < 0)
        {
            wallTop = 0;
        }
        if (wallBottom >= SCREEN_HEIGHT)
        {
            wallBottom = SCREEN_HEIGHT - 1;
        }

        render_wall_segment(gameStats, player, rayAngle, 
                            x, wallTop, wallHeight);
    }
}*/
int get_ray_side(GameStats *gameStats, float posX, float posY, float rayAngle, int *mapX, int *mapY, float *perpWallDist)
{
    // Current player position
    *mapX = (int)posX;
    *mapY = (int)posY;

    // Direction of the ray
    float rayDirX = cos(rayAngle);
    float rayDirY = sin(rayAngle);

    // Length of ray from one x or y-side to next x or y-side
    float deltaDistX = fabs(1 / rayDirX);
    float deltaDistY = fabs(1 / rayDirY);

    // Initialize step and initial sideDist
    int stepX, stepY;
    float sideDistX, sideDistY;

    // Calculate step direction and initial sideDist
    if (rayDirX < 0)
    {
        stepX = -1;
        sideDistX = (posX - *mapX) * deltaDistX;
    }
    else
    {
        stepX = 1;
        sideDistX = (*mapX + 1.0 - posX) * deltaDistX;
    }

    if (rayDirY < 0)
    {
        stepY = -1;
        sideDistY = (posY - *mapY) * deltaDistY;
    }
    else
    {
        stepY = 1;
        sideDistY = (*mapY + 1.0 - posY) * deltaDistY;
    }

    // Perform DDA (Digital Differential Analysis)
    int hit = 0;  // Was there a wall hit?
    int side;     // Was the wall hit on the X side (0) or Y side (1)?

    while (hit == 0)
    {
        // Jump to the next map square, either in x-direction or y-direction
        if (sideDistX < sideDistY)
        {
            sideDistX += deltaDistX;
            *mapX += stepX;
            side = 0;  // Hit on X side
        }
        else
        {
            sideDistY += deltaDistY;
            *mapY += stepY;
            side = 1;  // Hit on Y side
        }

        // Check if the ray has hit a wall
        if (gameStats->maze_map[*mapX][*mapY] > 0)
        {
            hit = 1;
        }
    }

    // Calculate the perpendicular wall distance
    if (side == 0)
    {
        *perpWallDist = (sideDistX - deltaDistX);
    }
    else
    {
        *perpWallDist = (sideDistY - deltaDistY);
    }

    // Return the side that was hit
    return side;
}





/**
 * render_wall_segment - Renders a segment of a wall based on the ray hit.
 *
 * @gameStats: Pointer to the GameStats struct containing rendering data.
 * @player: Pointer to the Player struct containing player position.
 * @rayAngle: The angle of the ray hitting the wall.
 * @x: The current x-coordinate on the screen for rendering.
 * @wallTop: The top pixel coordinate for the wall segment.
 * @wallHeight: The height of the wall segment to be rendered.
 *
 * Calculates the texture coordinates and renders the specified wall segment
 * using the appropriate wall texture from the maze map.
 */
void render_walls(GameStats *gameStats, Player *player)
{
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        float rayAngle = player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH);
        
        // Map coordinates and perpendicular wall distance
        int mapX, mapY;
        float perpWallDist;

        // Get the side hit (X or Y) and the perpendicular wall distance
        int side = get_ray_side(gameStats, player->x, player->y, rayAngle, &mapX, &mapY, &perpWallDist);
        
        // Use perpWallDist for wall height or texture scaling
        if (perpWallDist < 0.1)
        {
            perpWallDist = 0.1;
        }
        
        int wallHeight = (int)(SCREEN_HEIGHT / perpWallDist);
        int wallTop = (SCREEN_HEIGHT / 2) - (wallHeight / 2);
        int wallBottom = (SCREEN_HEIGHT / 2) + (wallHeight / 2);

        if (wallTop < 0)
        {
            wallTop = 0;
        }
        if (wallBottom >= SCREEN_HEIGHT)
        {
            wallBottom = SCREEN_HEIGHT - 1;
        }

        // Optionally use 'side' to change texture rendering based on hit side
        render_wall_segment(gameStats, player, rayAngle, x, wallTop, wallHeight, side);
    }
}


/**
 * render - Main render function to draw the entire scene.
 *
 * @player: Pointer to the Player struct containing player position.
 * @gameStats: Pointer to the GameStats struct containing rendering data.
 *
 * Clears the rendering buffer, draws the sky and floor, calls render_walls
 * to draw walls, optionally draws the mini map, and presents the rendered
 * frame to the screen.
 */
void render(Player *player, GameStats *gameStats)
{
    SDL_RenderClear(gameStats->renderer);

    draw_sky(gameStats);
    draw_floor(gameStats, player);

    // Call the refactored render_walls function
    render_walls(gameStats, player);

    if (gameStats->showMap)
    {
        draw_mini_map(gameStats, player, gameStats->showMap);
    }

    SDL_RenderPresent(gameStats->renderer);
}
