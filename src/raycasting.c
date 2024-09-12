#include <SDL2/SDL.h>
#include <math.h>
#include "player.h"
#include "constants.h"

/* Define map dimensions */
#define MAP_WIDTH 10
#define MAP_HEIGHT 10

/* Define the map (1 = wall, 0 = floor) */
int map[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};


/**
 * draw - Draws the scene using raycasting based on the player's position.
 * @renderer: The SDL renderer.
 * @player: The player's position and angle.
 */
void draw(SDL_Renderer *renderer, Player player)
{
    int x;

    /* Clear the screen with sky blue */
    SDL_SetRenderDrawColor(renderer, 135, 206, 250, 255); /* Sky blue */
    SDL_RenderClear(renderer);

    /* Draw the floor and ceiling */
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); /* Green for floor */
    SDL_Rect floorRect = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &floorRect);

    SDL_SetRenderDrawColor(renderer, 135, 206, 250, 255); /* Sky blue for ceiling */
    SDL_Rect ceilingRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &ceilingRect);

    /* Raycasting logic */
    for (x = 0; x < SCREEN_WIDTH; x++)
    {
        float cameraX = 2 * x / (float)SCREEN_WIDTH - 1;
        float rayDirX = cos(player.angle) + cameraX * cos(player.angle + M_PI_2);
        float rayDirY = sin(player.angle) + cameraX * sin(player.angle + M_PI_2);

        /* Raycasting calculations */
        int wallHeight = SCREEN_HEIGHT / 2; /* Example height */

        /* Determine wall color based on orientation */
        SDL_Color wallColor;
        if (rayDirX > rayDirY)
        {
            wallColor = (SDL_Color){128, 128, 128, 255}; /* Gray for walls facing EAST/WEST */
        }
        else
        {
            wallColor = (SDL_Color){255, 0, 0, 255}; /* Red for walls facing NORTH/SOUTH */
        }

        /* Draw the wall */
        SDL_SetRenderDrawColor(renderer, wallColor.r, wallColor.g, wallColor.b, wallColor.a);
        SDL_Rect wallRect = {x, SCREEN_HEIGHT / 2 - wallHeight / 2, 1, wallHeight};
        SDL_RenderFillRect(renderer, &wallRect);
    }

    /* Present the rendered frame */
    SDL_RenderPresent(renderer);
}
