#include "raycasting.h"


/**
 * draw_map - Renders the maze map on the screen if the map is enabled.
 *
 * @gameStats: Pointer to the GameStats struct containing the maze map and renderer.
 * @showMap: Boolean flag to indicate whether the map should be drawn.
 *
 * Draws a simple 2D top-down view of the maze map using different colors for walls.
 */
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

            if (gameStats->maze_map[x][y] == 1)
            {
                SDL_SetRenderDrawColor(gameStats->renderer, 255, 0, 0, 255); 
            }
            else
            {
                SDL_SetRenderDrawColor(gameStats->renderer, 255, 255, 255, 255); 
            }

            SDL_RenderFillRect(gameStats->renderer, &rect);
        }
    }
}



/**
 * draw_player_and_los - Draws the player and their line of sight on the mini-map.
 *
 * @gameStats: Pointer to the GameStats struct containing the renderer.
 * @player: Pointer to the Player struct containing player position and angle.
 *
 * Renders the player's position and the field of view (lines of sight) on the mini-map.
 */
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
    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
        float rayAngle = player->angle - (FOV / 2) + (FOV * i / SCREEN_WIDTH);
        float endX = mapPlayerX + cos(rayAngle) * (mapWidth / 4);
        float endY = mapPlayerY + sin(rayAngle) * (mapHeight / 4);
        SDL_RenderDrawLine(gameStats->renderer, mapPlayerX, mapPlayerY, endX, endY);
    }
}


/**
 * draw_mini_map - Draws the mini-map including the maze map and player's position.
 *
 * @gameStats: Pointer to the GameStats struct containing the maze map and renderer.
 * @player: Pointer to the Player struct containing player information.
 * @showMap: Boolean flag to indicate whether the map should be drawn.
 *
 * Combines map rendering and player position/LOS to draw the complete mini-map.
 */
void draw_mini_map(GameStats *gameStats, Player *player, bool showMap)
{
    draw_map(gameStats, showMap);
    draw_player_and_los(gameStats, player);
}
