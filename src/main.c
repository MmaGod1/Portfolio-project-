#include <SDL2/SDL.h>
#include <math.h>  // For sin and cos
#include "init.h"
#include "player.h"
#include "constants.h"

/* Rotation speed in radians */
#define ROTATION_SPEED 0.05

void rotatePlayerLeft(Player *player, float rotateSpeed) {
    float oldDirX = player->dirX;
    player->dirX = player->dirX * cos(-rotateSpeed) - player->dirY * sin(-rotateSpeed);
    player->dirY = oldDirX * sin(-rotateSpeed) + player->dirY * cos(-rotateSpeed);
    
    float oldPlaneX = player->planeX;
    player->planeX = player->planeX * cos(-rotateSpeed) - player->planeY * sin(-rotateSpeed);
    player->planeY = oldPlaneX * sin(-rotateSpeed) + player->planeY * cos(-rotateSpeed);
}

void rotatePlayerRight(Player *player, float rotateSpeed) {
    float oldDirX = player->dirX;
    player->dirX = player->dirX * cos(rotateSpeed) - player->dirY * sin(rotateSpeed);
    player->dirY = oldDirX * sin(rotateSpeed) + player->dirY * cos(rotateSpeed);
    
    float oldPlaneX = player->planeX;
    player->planeX = player->planeX * cos(rotateSpeed) - player->planeY * sin(rotateSpeed);
    player->planeY = oldPlaneX * sin(rotateSpeed) + player->planeY * cos(rotateSpeed);
}

void handle_input(Player *player) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    
    if (state[SDL_SCANCODE_LEFT]) {
        rotatePlayerLeft(player, ROTATION_SPEED);  // Rotate left
    }

    if (state[SDL_SCANCODE_RIGHT]) {
        rotatePlayerRight(player, ROTATION_SPEED);  // Rotate right
    }
}

int main(void)
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    Player player = { .posX = 2, .posY = 2, .dirX = -1, .dirY = 0, .planeX = 0, .planeY = 0.66 }; // Initial player position and direction

    if (init_sdl(&window, &renderer) != 0)
    {
        return (1);
    }

    while (1)
    {
        handle_input(&player);
        draw(renderer, player);
        SDL_Delay(16); /* Limit frame rate to ~60 FPS */
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return (0);
}
