#include "raycasting.h"


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
/**void render_walls(GameStats *gameStats, Player *player)
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
}**/
void render_walls(GameStats *gameStats, Player *player)
{
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        // Calculate the ray angle based on player's view and FOV
        float rayAngle = player->angle - (FOV / 2.0f) + 
                         (FOV * (x / (float)SCREEN_WIDTH));

        // Cast the ray and get the distance to the nearest wall
        float distance = cast_ray(gameStats, player->x, player->y, rayAngle);

        // Avoid division by near-zero distances (if too close)
        if (distance < 0.1)
        {
            distance = 0.1;
        }

        // Calculate the wall height based on the raw distance (no cosine correction)
        int wallHeight = (int)(SCREEN_HEIGHT / distance);
        int wallTop = (SCREEN_HEIGHT / 2) - (wallHeight / 2);
        int wallBottom = (SCREEN_HEIGHT / 2) + (wallHeight / 2);

        // Ensure the wall's top and bottom are within the screen bounds
        if (wallTop < 0)
        {
            wallTop = 0;
        }
        if (wallBottom >= SCREEN_HEIGHT)
        {
            wallBottom = SCREEN_HEIGHT - 1;
        }

        // Render the wall segment for this particular x-column
        render_wall_segment(gameStats, player, rayAngle, x, wallTop, wallHeight);
    }
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
/**void render_wall_segment(GameStats *gameStats, Player *player, 
                         float rayAngle, int x, 
                         int wallTop, int wallHeight)
{
    int mapX, mapY;
    float wallX = get_wall_hit_coordinates(gameStats, player->x, 
                                            player->y, rayAngle, 
                                            &mapX, &mapY);
    int wallTextureIndex = gameStats->maze_map[mapX][mapY] - 1;

    int texX = (int)(wallX * gameStats->wallTextures[wallTextureIndex].width) % 
               gameStats->wallTextures[wallTextureIndex].width;

    SDL_Rect srcRect = { texX, 0, 1, 
                         gameStats->wallTextures[wallTextureIndex].height };
    SDL_Rect dstRect = { x, wallTop, 1, wallHeight };

    SDL_RenderCopy(gameStats->renderer, 
                   gameStats->wallTextures[wallTextureIndex].texture, 
                   &srcRect, &dstRect);
} **/

void render_wall_segment(GameStats *gameStats, Player *player, 
                         float rayAngle, int x, 
                         int wallTop, int wallHeight)
{
    int mapX, mapY;
    float perpWallDist;
    // Get the exact wall hit coordinates
    //float wallX = get_wall_hit_coordinates(gameStats, player->x, player->y, rayAngle, &mapX, &mapY);
    float wallX = get_wall_hit_coordinates(gameStats, player->x, player->y, rayAngle, &mapX, &mapY, &perpWallDist);
    // Determine the texture index from the maze map
    int wallTextureIndex = gameStats->maze_map[mapX][mapY] - 1;

    // Compute texture X coordinate based on the wall hit point
    int texX = (int)(wallX * gameStats->wallTextures[wallTextureIndex].width) % 
               gameStats->wallTextures[wallTextureIndex].width;

    // Define the source and destination rectangles for rendering the wall slice
    SDL_Rect srcRect = { texX, 0, 1, gameStats->wallTextures[wallTextureIndex].height };
    SDL_Rect dstRect = { x, wallTop, 1, wallHeight };

    // Render the wall segment slice to the screen
    SDL_RenderCopy(gameStats->renderer, 
                   gameStats->wallTextures[wallTextureIndex].texture, 
                   &srcRect, &dstRect);
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
