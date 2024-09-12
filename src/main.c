#include <SDL2/SDL.h>
#include "map.h"
#include "camera.h"
#include "raycasting.h"

int main() {
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Set the drawing color to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Clear the screen
    SDL_RenderClear(renderer);

    // Initialize the camera
    float camera_x = 10.0f;
    float camera_y = 10.0f;
    float camera_angle = 0.0f;

    // Main loop
    int running = 1;
    while (running) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Cast rays and draw walls
        cast_ray(camera_x, camera_y, camera_angle);

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
