#include "raycasting.h"

void draw_sky()
{
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_Rect skyRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &skyRect);
}



void draw_mini_map(Player *player, bool showMap)
{
    if (!showMap) return;

    int mapStartX = 10;    /* Position the map in the top-left corner */
    int mapStartY = 10;
    int mapWidth = 160;    /* Width of the map on screen (adjust as needed) */
    int mapHeight = 120;   /* Height of the map on screen (adjust as needed) */
    int tileSize = TILE_SIZE; /* Size of each tile (you should define TILE_SIZE) */

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            SDL_Rect rect;
            rect.x = mapStartX + x * tileSize;
            rect.y = mapStartY + y * tileSize;
            rect.w = tileSize;
            rect.h = tileSize;

            if (maze_map[x][y] == 1) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); /* Wall color (e.g., red) */
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); /* Empty space color (e.g., white) */
            }

            SDL_RenderFillRect(renderer, &rect);
        }
    }
    /* Draw the player's position on the map */
    float mapPlayerX = mapStartX + (player->x * mapWidth / MAP_WIDTH);
    float mapPlayerY = mapStartY + (player->y * mapHeight / MAP_HEIGHT);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  /* Green color for player's position */
    SDL_Rect playerRect = { (int)mapPlayerX - 2, (int)mapPlayerY - 2, 4, 4 };  /* Small rectangle to represent the player*/
    SDL_RenderFillRect(renderer, &playerRect);

    /* Draw the player's line of sight on the map */
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  /* Blue color for line of sight */
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        float rayAngle = player->angle - (FOV / 2) + (FOV * i / SCREEN_WIDTH);
        float endX = mapPlayerX + cos(rayAngle) * (mapWidth / 4);
        float endY = mapPlayerY + sin(rayAngle) * (mapHeight / 4);
        SDL_RenderDrawLine(renderer, mapPlayerX, mapPlayerY, endX, endY);
    }
}
