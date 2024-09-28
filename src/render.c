#include "raycasting.h"

void render_walls(Player *player)
{
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        float rayAngle = player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH);
        float distance = cast_ray(player->x, player->y, rayAngle);
        int mapX, mapY;

        if (distance < 0.1) distance = 0.1;

        /* Correct the distance to avoid fisheye effect */
        float correctedDistance = distance * cos(rayAngle - player->angle);

        int wallHeight = (int)(SCREEN_HEIGHT / correctedDistance);
        int wallTop = (SCREEN_HEIGHT / 2) - (wallHeight / 2);
        int wallBottom = (SCREEN_HEIGHT / 2) + (wallHeight / 2);
      
        if (wallTop < 0)
          wallTop = 0;
        if (wallBottom >= SCREEN_HEIGHT)
          wallBottom = SCREEN_HEIGHT - 1;

        /* Determine which wall texture to use based on map hit */
        float wallX = get_wall_hit_coordinates(player->x, player->y, rayAngle, &mapX, &mapY);
        int wallTextureIndex = maze_map[mapX][mapY] - 1;

        int texX = (int)(wallX * wallTextures[wallTextureIndex].width) % wallTextures[wallTextureIndex].width;

        SDL_Rect srcRect = { texX, 0, 1, wallTextures[wallTextureIndex].height };
        SDL_Rect dstRect = { x, wallTop, 1, wallHeight };

        /* Render wall slice */
        SDL_RenderCopy(renderer, wallTextures[wallTextureIndex].texture, &srcRect, &dstRect);
    }
}



void render(Player *player)
{
    SDL_RenderClear(renderer);

    draw_sky();
    draw_floor(player);

    render_walls(player);

    if (showMap)
        draw_mini_map(player, showMap);

    SDL_RenderPresent(renderer);
}
