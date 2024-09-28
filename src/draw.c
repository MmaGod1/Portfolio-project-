#include "raycasting.h"

void draw_sky(GameStats *gameStats)
{
    SDL_SetRenderDrawColor(gameStats->renderer, 135, 206, 235, 255);
    SDL_Rect skyRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(gameStats->renderer, &skyRect);
}

void draw_floor(GameStats *gameStats, Player *player)
{
    for (int y = SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT; y++)
    {
        float floorDist = SCREEN_HEIGHT / (2.0f * y - SCREEN_HEIGHT); /* Perspective correct distance */

        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            float rayAngle = player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH);
            float floorX = player->x + cos(rayAngle) * floorDist;
            float floorY = player->y + sin(rayAngle) * floorDist;

            int texX = (int)(floorX * gameStats->floorTexture.width) % 
                        gameStats->floorTexture.width;
            int texY = (int)(floorY * gameStats->floorTexture.height) % 
                        gameStats->floorTexture.height;
            if (texX < 0)
                texX = 0;
            if (texY < 0)
                texY = 0;

            /* Render the floor pixel */
            SDL_Rect srcRect = { texX, texY, 1, 1 };
            SDL_Rect dstRect = { x, y, 1, 1 };
            SDL_RenderCopy(gameStats->renderer, gameStats->floorTexture.texture, 
                           &srcRect, &dstRect);
        }
    }
}
