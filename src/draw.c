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

void draw_mini_map(GameStats *gameStats, Player *player, bool showMap)
{
    if (!showMap)
        return;

    int mapStartX = 10;    /* Position the map in the top-left corner */
    int mapStartY = 10;
    int mapWidth = 160;    /* Width of the map on screen */
    int mapHeight = 120;   /* Height of the map on screen */
    int tileSize = TILE_SIZE; /* Size of each tile */

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            SDL_Rect rect;
            rect.x = mapStartX + x * tileSize;
            rect.y = mapStartY + y * tileSize;
            rect.w = tileSize;
            rect.h = tileSize;

            if (gameStats->maze_map[x][y] == 1) {
                SDL_SetRenderDrawColor(gameStats->renderer, 255, 0, 0, 255); /* Wall color */
            } else {
                SDL_SetRenderDrawColor(gameStats->renderer, 255, 255, 255, 255); /* Empty space */
            }

            SDL_RenderFillRect(gameStats->renderer, &rect);
        }
    }

    /* Draw the player's position on the map */
    float mapPlayerX = mapStartX + (player->x * mapWidth / MAP_WIDTH);
    float mapPlayerY = mapStartY + (player->y * mapHeight / MAP_HEIGHT);
    SDL_SetRenderDrawColor(gameStats->renderer, 0, 255, 0, 255);  /* Green color */
    SDL_Rect playerRect = { (int)mapPlayerX - 2, (int)mapPlayerY - 2, 4, 4 };  
    SDL_RenderFillRect(gameStats->renderer, &playerRect);

    /* Draw the player's line of sight on the map */
    SDL_SetRenderDrawColor(gameStats->renderer, 0, 0, 255, 255);  /* Blue color */
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        float rayAngle = player->angle - (FOV / 2) + (FOV * i / SCREEN_WIDTH);
        float endX = mapPlayerX + cos(rayAngle) * (mapWidth / 4);
        float endY = mapPlayerY + sin(rayAngle) * (mapHeight / 4);
        SDL_RenderDrawLine(gameStats->renderer, mapPlayerX, mapPlayerY, endX, endY);
    }
}
