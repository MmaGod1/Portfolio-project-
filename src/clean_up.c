#include "raycasting.h"


/**
 * clean_up - Frees textures, renderer, window, and quits SDL.
 *
 * @gameStats: Pointer to the GameStats struct containing the SDL resources.
 * 
 * Frees any dynamically allocated textures in the game, destroys the
 * renderer and window, and shuts down SDL to release all resources.
 */
void clean_up(GameStats *gameStats)
{
    for (int i = 0; i < 4; i++)
    {
        if (gameStats->wallTextures[i].texture)
        {
            SDL_DestroyTexture(gameStats->wallTextures[i].texture);
            gameStats->wallTextures[i].texture = NULL;
        }
    }

    if (gameStats->floorTexture.texture)
    {
        SDL_DestroyTexture(gameStats->floorTexture.texture);
        gameStats->floorTexture.texture = NULL;
    }

    SDL_DestroyRenderer(gameStats->renderer);
    SDL_DestroyWindow(gameStats->window);
    SDL_Quit();
}
