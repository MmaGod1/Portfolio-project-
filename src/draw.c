#include "raycasting.h"

void drawSky()
{
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_Rect skyRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &skyRect);
}
