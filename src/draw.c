#include "raycasting.h"

/**
 * draw_sky - Draws the sky portion of the game screen.
 *
 * @gameStats: Pointer to the GameStats struct containing the renderer.
 *
 * Fills the upper half of the screen with a sky-blue color to represent the sky.
 */
void draw_sky(GameStats *gameStats)
{
    SDL_SetRenderDrawColor(gameStats->renderer, 135, 206, 235, 255);
    SDL_Rect skyRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(gameStats->renderer, &skyRect);
}


/**
 * draw_floor - Draws the textured floor based on player's position and view angle.
 *
 * @gameStats: Pointer to the GameStats struct containing the renderer and floor texture.
 * @player: Pointer to the Player struct containing player's position and view angle.
 *
 * Renders the floor using perspective-correct distances and textures.
 */
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
