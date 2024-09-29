#include "raycasting.h"

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
}

*/

void render_walls(GameStats *gameStats, Player *player)
{
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        // Calculate the angle of the ray for this column (current angle from the player's view)
        float rayAngle = player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH);
        
        // Cast the ray and get the raw distance to the wall (along the ray's path)
        float distance = cast_ray(gameStats, player->x, player->y, rayAngle);
        
        if (distance < 0.1)
        {
            distance = 0.1; // Prevent division by zero or too small values
        }

        // Correct the fisheye effect by dividing by cos(rayAngle - player->angle)
        float correctedDistance = distance / cos(rayAngle - player->angle);

        // Ensure corrected distance is not zero or negative
        if (correctedDistance <= 0)
        {
            correctedDistance = 0.1; // Set to a minimum value
        }

        // Use the actual object height (wall height) for scaling
        float objectHeight = 1.0f;  // Height of the wall or object in the game world

        // Calculate the perceived wall height using the corrected distance
        int wallHeight = (int)(SCREEN_HEIGHT * objectHeight / correctedDistance);

        // Calculate where the top and bottom of the wall should be drawn
        int wallTop = (SCREEN_HEIGHT / 2) - (wallHeight / 2);
        int wallBottom = (SCREEN_HEIGHT / 2) + (wallHeight / 2);
        
        // Clamp the drawing range to avoid rendering outside the screen
        if (wallTop < 0)
        {
            wallTop = 0;
        }
        if (wallBottom >= SCREEN_HEIGHT)
        {
            wallBottom = SCREEN_HEIGHT - 1;
        }

        // Render the wall segment
        render_wall_segment(gameStats, player, rayAngle, x, wallTop, wallHeight);
    }
}






void render_wall_segment(GameStats *gameStats, Player *player, 
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
