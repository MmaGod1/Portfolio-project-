#include "raycasting.h"

void render_walls(GameStats *gameStats, Player *player)
{
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        float rayAngle = player->angle - (FOV / 2) + 
                         (FOV * x / SCREEN_WIDTH);
        float distance = cast_ray(gameStats, player->x, player->y, 
                                  rayAngle);
        int mapX, mapY;

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

        float wallX = get_wall_hit_coordinates(player->x, player->y, 
                                                rayAngle, &mapX, 
                                                &mapY);
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
}

void render(GameStats *gameStats, Player *player)
{
    SDL_RenderClear(gameStats->renderer);

    draw_sky();
    draw_floor(player);

    render_walls(gameStats, player);

    if (gameStats->showMap)
    {
        draw_mini_map(player, gameStats->showMap);
    }

    SDL_RenderPresent(gameStats->renderer);
}
