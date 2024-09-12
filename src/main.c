#include <SDL2/SDL.h>
#include "init.h"
#include "player.h"

int main(void) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    
    /* Initialize SDL */
    if (init(&window, &renderer) != 0) {
        return 1;
    }

    /* Main loop */
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        
        /* Draw the scene */
        draw(renderer, player);

        /* Delay to control frame rate */
        SDL_Delay(16);
    }

    /* Clean up */
    cleanup(window, renderer);
    return 0;
}
