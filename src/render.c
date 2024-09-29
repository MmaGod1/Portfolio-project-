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
    // Forward direction (normalized vector)
    float forward_x = cos(player->angle);
    float forward_y = sin(player->angle);

    // Width of the projection plane (how far the edges are from the center)
    float projection_plane_width = 2 * tan(FOV / 2);

    // Right direction (perpendicular to forward direction)
    float right_x = -forward_y * projection_plane_width;
    float right_y = forward_x * projection_plane_width;

    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        // Step from -0.5 to +0.5 in equal increments across the screen width
        float progress = ((float)x / (SCREEN_WIDTH - 1)) - 0.5f;

        // Calculate the ray direction by adding the appropriate amount of the right vector
        float rayDir_x = forward_x + progress * right_x;
        float rayDir_y = forward_y + progress * right_y;

        // Normalize the ray direction
        float ray_length = sqrt(rayDir_x * rayDir_x + rayDir_y * rayDir_y);
        rayDir_x /= ray_length;
        rayDir_y /= ray_length;

        // Calculate the ray angle from the ray direction
        float rayAngle = atan2(rayDir_y, rayDir_x); // Get the angle in radians

        // Cast the ray and get the distance to the wall
        float distance = cast_ray(gameStats, player->x, player->y, rayAngle); // Call with rayAngle

        if (distance < 0.1)
        {
            distance = 0.1; // Prevent division by zero or too small values
        }

        // Calculate the perceived wall height based on the corrected distance
        int wallHeight = (int)(SCREEN_HEIGHT / distance);

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

        // Render the wall segment using the correct arguments
        render_wall_segment(gameStats, player, rayAngle, x, wallTop, wallHeight); // Call with rayAngle
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
