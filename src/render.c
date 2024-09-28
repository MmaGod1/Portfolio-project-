#include "raycasting.h"
#include <math.h>
#include <stdbool.h>

#define EPSILON 0.1f // Small value to avoid division by zero
#define MAX_DISTANCE 1000.0f // Define MAX_DISTANCE

float normalize_angle(float angle) {
    while (angle < 0) angle += 2 * M_PI;
    while (angle >= 2 * M_PI) angle -= 2 * M_PI;
    return angle;
}

void calculate_wall_dimensions(float distance, Player *player, float rayAngle, int *wallHeight, int *wallTop, int *wallBottom) {
    rayAngle = normalize_angle(rayAngle);
    float correctedDistance = distance * cos(rayAngle - player->angle);

    // Avoid division by zero or extremely small distances
    correctedDistance = fmax(correctedDistance, EPSILON);

    *wallHeight = (int)(SCREEN_HEIGHT / correctedDistance);
    *wallTop = (SCREEN_HEIGHT / 2) - (*wallHeight / 2);
    *wallBottom = (SCREEN_HEIGHT / 2) + (*wallHeight / 2);

    // Ensure values are within screen bounds
    *wallTop = fmax(*wallTop, 0);
    *wallBottom = fmin(*wallBottom, SCREEN_HEIGHT - 1);
}

void render_single_wall(GameStats *gameStats, int x, int wallHeight, int wallTop, float wallX, int wallTextureIndex, float lightIntensity) {
    SDL_Rect srcRect, dstRect;
    int texWidth = gameStats->wallTextures[wallTextureIndex].width;  
    int texHeight = gameStats->wallTextures[wallTextureIndex].height; 
    int texX = (int)(wallX * texWidth) % texWidth;

    srcRect.x = texX;          
    srcRect.y = 0;             
    srcRect.w = 1;             
    srcRect.h = texHeight;     

    dstRect.x = x;             
    dstRect.y = wallTop;       
    dstRect.w = 1;             
    dstRect.h = wallHeight;    

    // Adjust texture transparency based on light intensity
    SDL_SetTextureAlphaMod(gameStats->wallTextures[wallTextureIndex].texture, (Uint8)(255 * lightIntensity));
    SDL_RenderCopy(gameStats->renderer, gameStats->wallTextures[wallTextureIndex].texture, &srcRect, &dstRect);
}

void render_walls(GameStats *gameStats, Player *player)
{
    int x, mapX, mapY, wallHeight, wallTop, wallBottom;
    int wallTextureIndex;
    float rayAngle, distance, wallX;

    for (x = 0; x < SCREEN_WIDTH; x++)
    {
        // Normalize ray angle based on player angle and FOV
        rayAngle = normalize_angle(player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH));

        // Cast the ray to get the distance to the wall
        distance = cast_ray(player->x, player->y, rayAngle);

        // Calculate wall dimensions based on distance
        calculate_wall_dimensions(distance, player, rayAngle, &wallHeight, &wallTop, &wallBottom);

        // Get the wall hit coordinate and texture index
        wallX = get_wall_hit_coordinates(player->x, player->y, rayAngle, &mapX, &mapY); // Fixed

        wallTextureIndex = maze_map[mapX][mapY] - 1;

        // Render the wall slice with the calculated dimensions and texture coordinates
        render_single_wall(gameStats, x, wallHeight, wallTop, wallX, wallTextureIndex);
    }
}

void render(Player *player, bool showMap, SDL_Renderer *renderer, GameStats *gameStats) {
    SDL_RenderClear(renderer);
    draw_sky(renderer);
    draw_floor(renderer, player, gameStats->floorTexture);
    render_walls(gameStats, player);

    if (showMap) {
        draw_mini_map(renderer, player, showMap);
    }

    SDL_RenderPresent(renderer);
}
