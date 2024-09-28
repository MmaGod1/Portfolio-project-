#include "raycasting.h"

void draw_map(GameStats *gameStats, bool showMap)
{
    if (!showMap)
        return;

    int mapStartX = 10;    
    int mapStartY = 10;
    int tileSize = TILE_SIZE; 

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
                SDL_SetRenderDrawColor(gameStats->renderer, 255, 0, 0, 255); 
            } else {
                SDL_SetRenderDrawColor(gameStats->renderer, 255, 255, 255, 255); 
            }

            SDL_RenderFillRect(gameStats->renderer, &rect);
        }
    }
}

void draw_player_and_los(GameStats *gameStats, Player *player)
{
    int mapStartX = 10;    
    int mapStartY = 10;  
    int mapWidth = 160;  
    int mapHeight = 120;  

    float mapPlayerX = mapStartX + (player->x * mapWidth / MAP_WIDTH);
    float mapPlayerY = mapStartY + (player->y * mapHeight / MAP_HEIGHT);
    SDL_SetRenderDrawColor(gameStats->renderer, 0, 255, 0, 255);  
    SDL_Rect playerRect = { (int)mapPlayerX - 2, (int)mapPlayerY - 2, 4, 4 };  
    SDL_RenderFillRect(gameStats->renderer, &playerRect);

    SDL_SetRenderDrawColor(gameStats->renderer, 0, 0, 255, 255);  
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        float rayAngle = player->angle - (FOV / 2) + (FOV * i / SCREEN_WIDTH);
        float endX = mapPlayerX + cos(rayAngle) * (mapWidth / 4);
        float endY = mapPlayerY + sin(rayAngle) * (mapHeight / 4);
        SDL_RenderDrawLine(gameStats->renderer, mapPlayerX, mapPlayerY, endX, endY);
    }
}

void draw_mini_map(GameStats *gameStats, Player *player, bool showMap)
{
    draw_map(gameStats, showMap);
    draw_player_and_los(gameStats, player);
}
