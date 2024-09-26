#!include "raycasting.h"

void drawSky() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);  // Light blue for sky
    SDL_Rect skyRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &skyRect);
}


void drawFloor(Player *player) {
    for (int y = SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT; y++) {
        float floorDist = SCREEN_HEIGHT / (2.0f * y - SCREEN_HEIGHT); // Perspective correct distance

        for (int x = 0; x < SCREEN_WIDTH; x++) {
            float rayAngle = player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH);

            // Compute floor X and Y coordinates
            float floorX = player->x + cos(rayAngle) * floorDist;
            float floorY = player->y + sin(rayAngle) * floorDist;

            // Determine the texture coordinates
            int texX = (int)(floorX * floorTexture.width) % floorTexture.width;
            int texY = (int)(floorY * floorTexture.height) % floorTexture.height;

            // Ensure texture coordinates are valid
            if (texX < 0) texX = 0;
            if (texY < 0) texY = 0;

            // Render the floor pixel
            SDL_Rect srcRect = { texX, texY, 1, 1 };
            SDL_Rect dstRect = { x, y, 1, 1 };
            SDL_RenderCopy(renderer, floorTexture.texture, &srcRect, &dstRect);
        }
    }
}

void drawMiniMap(Player *player, bool showMap) {
    if (!showMap) return;

    int mapStartX = 10;    // Position the map in the top-left corner
    int mapStartY = 10;
    int mapWidth = 160;    // Width of the map on screen (adjust as needed)
    int mapHeight = 120;   // Height of the map on screen (adjust as needed)
    int tileSize = TILE_SIZE; // Size of each tile (you should define TILE_SIZE)

    // Draw the map tiles
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            SDL_Rect rect;
            rect.x = mapStartX + x * tileSize;  // Position of the tile
            rect.y = mapStartY + y * tileSize;  // Position of the tile
            rect.w = tileSize;                  // Width of the tile
            rect.h = tileSize;                  // Height of the tile

            if (maze_map[x][y] == 1) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Wall color (e.g., red)
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Empty space color (e.g., white)
            }

            SDL_RenderFillRect(renderer, &rect);
        }
    }

    // Draw the player's position on the map
    float mapPlayerX = mapStartX + (player->x * mapWidth / MAP_WIDTH);
    float mapPlayerY = mapStartY + (player->y * mapHeight / MAP_HEIGHT);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green color for player's position
    SDL_Rect playerRect = { (int)mapPlayerX - 2, (int)mapPlayerY - 2, 4, 4 };  // Small rectangle to represent the player
    SDL_RenderFillRect(renderer, &playerRect);

    // Draw the player's line of sight on the map
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Blue color for line of sight
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        float rayAngle = player->angle - (FOV / 2) + (FOV * i / SCREEN_WIDTH);
        float endX = mapPlayerX + cos(rayAngle) * (mapWidth / 4);  // Adjust length for rays (shorter)
        float endY = mapPlayerY + sin(rayAngle) * (mapHeight / 4); // Adjust length for rays (shorter)
        SDL_RenderDrawLine(renderer, mapPlayerX, mapPlayerY, endX, endY);
    }
}
