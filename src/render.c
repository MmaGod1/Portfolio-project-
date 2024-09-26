#include "raycasting.h"

  void renderWalls(Player *player) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float rayAngle = player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH);
        float distance = castRay(player->x, player->y, rayAngle);
        
        if (distance > 10.0) distance = 10.0;

        // Correct the distance to avoid fisheye effect
        float correctedDistance = distance * cos(rayAngle - player->angle);

        // Calculate the wall height
        int wallHeight = (int)(SCREEN_HEIGHT / correctedDistance);
        int wallTop = (SCREEN_HEIGHT / 2) - (wallHeight / 2);
        int wallBottom = (SCREEN_HEIGHT / 2) + (wallHeight / 2);

        // Ensure values are within screen bounds
        if (wallTop < 0) wallTop = 0;
        if (wallBottom >= SCREEN_HEIGHT) wallBottom = SCREEN_HEIGHT - 1;

        // Determine which wall texture to use based on map hit
        int mapX, mapY; // Ray hit coordinates
        float wallX = getWallHitCoordinates(player->x, player->y, rayAngle, &mapX, &mapY);
        int wallTextureIndex = maze_map[mapX][mapY] - 1;

        // Texture coordinates
        int texX = (int)(wallX * wallTextures[wallTextureIndex].width) % wallTextures[wallTextureIndex].width;

        SDL_Rect srcRect = { texX, 0, 1, wallTextures[wallTextureIndex].height };
        SDL_Rect dstRect = { x, wallTop, 1, wallHeight };

        // Render wall slice
        SDL_RenderCopy(renderer, wallTextures[wallTextureIndex].texture, &srcRect, &dstRect);
    }
}


void render(Player *player) {
    SDL_RenderClear(renderer);

    // Draw the sky and floor
    drawSky();
    drawFloor(player);

    // Cast rays and render walls
    renderWalls(player);

    // Optionally draw the mini-map
    if (showMap) {
        drawMiniMap(player, showMap);
    }

    SDL_RenderPresent(renderer);
}
