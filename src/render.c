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
void render_walls(GameStats *gameStats, Player *player)
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
void render_wall_segment(GameStats *gameStats, Player *player, 
                         float rayAngle, int x, 
                         int wallTop, int wallHeight)
{
    float perpWallDist;

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
