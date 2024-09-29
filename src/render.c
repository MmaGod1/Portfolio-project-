#include "raycasting.h"

    

void render_walls(GameStats *gameStats, Player *player)
{
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        // Calculate ray angle based on player angle and field of view
        float rayAngle = player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH);
        
        // Cast the ray to find the distance to the wall
        float distance = cast_ray(gameStats, player->x, player->y, rayAngle);
        
        // Prevent division by zero
        if (distance < 0.1)
        {
            distance = 0.1;
        }

        // Calculate the corrected distance
        float correctedDistance = distance * cos(rayAngle - player->angle);
        
        // Calculate the height of the wall segment
        int wallHeight = (int)(SCREEN_HEIGHT / correctedDistance);
        int wallTop = (SCREEN_HEIGHT / 2) - (wallHeight / 2);
        int wallBottom = (SCREEN_HEIGHT / 2) + (wallHeight / 2);
        
        // Clamp the drawing range
        if (wallTop < 0) wallTop = 0;
        if (wallBottom >= SCREEN_HEIGHT) wallBottom = SCREEN_HEIGHT - 1;

        // Render the wall segment
        render_wall_segment(gameStats, player, rayAngle, x, wallTop, wallHeight);
    }
}



/*void render_wall_segment(GameStats *gameStats, Player *player, 
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
}
*/
void render_wall_segment(GameStats *gameStats, Player *player, 
                         float rayAngle, int x, 
                         int wallTop, int wallHeight)
{
    float wallX;
    int mapX, mapY;

    // Get wall hit coordinates
    wallX = get_wall_hit_coordinates(gameStats, player->x, player->y, rayAngle, &mapX, &mapY);
    int wallTextureIndex = gameStats->maze_map[mapX][mapY] - 1;

    // Calculate texture coordinate
    int texX = (int)(wallX * gameStats->wallTextures[wallTextureIndex].width) % 
               gameStats->wallTextures[wallTextureIndex].width;

    SDL_Rect srcRect = { texX, 0, 1, gameStats->wallTextures[wallTextureIndex].height };
    SDL_Rect dstRect = { x, wallTop, 1, wallHeight };

    // Render the wall segment
    SDL_RenderCopy(gameStats->renderer, 
                   gameStats->wallTextures[wallTextureIndex].texture, 
                   &srcRect, &dstRect);
}


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
