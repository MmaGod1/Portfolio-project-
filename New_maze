#include <stdio.h>
#include "SDL2/SDL.h"
#include <math.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define FOV 60  // Field of view

typedef struct {
    double x;
    double y;
    double angle; // Player's view angle
} Player;

int maze_map[MAP_WIDTH][MAP_HEIGHT] = {
    // Your maze map data
};

SDL_Window *window = NULL;
SDL_Renderer *renderer;

int initialize(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Initialization failed! %s\n", SDL_GetError());
        return 1;
    }
    window = SDL_CreateWindow("Go-Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        printf("Window creation failed! %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer creation failed! %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    return 0;
}

void draw_maze(Player player) {
    int x, y;
    int mapX, mapY;
    double rayAngle;
    double rayDirX, rayDirY;
    double wallDist;
    int wallHeight;
    int color;

    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Sky color
    SDL_RenderClear(renderer);

    for (int i = 0; i < SCREEN_WIDTH; i++) {
        rayAngle = (player.angle - FOV / 2.0) + (FOV / SCREEN_WIDTH) * i;
        rayDirX = cos(rayAngle);
        rayDirY = sin(rayAngle);

        double distToWall = 0;
        bool hit = false;

        while (!hit && distToWall < 20) {  // Arbitrary max distance
            distToWall += 0.1;
            mapX = (int)(player.x + rayDirX * distToWall);
            mapY = (int)(player.y + rayDirY * distToWall);

            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
                hit = true;
                distToWall = 20; // Set distance to max
            } else if (maze_map[mapY][mapX] == 1) {
                hit = true;
            }
        }

        wallDist = distToWall * cos(player.angle - rayAngle); // Correct for player angle
        wallHeight = (int)(SCREEN_HEIGHT / wallDist);

        color = (wallDist < 5) ? 255 : 127; // Change color based on distance

        SDL_SetRenderDrawColor(renderer, color, color, color, 255);
        SDL_RenderDrawLine(renderer, i, (SCREEN_HEIGHT / 2) - (wallHeight / 2), i, (SCREEN_HEIGHT / 2) + (wallHeight / 2));
    }

    SDL_RenderPresent(renderer);
}

void handle_input(Player *player, bool *running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *running = false;
        }
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_q:
                    *running = false;
                    break;
                case SDLK_LEFT:
                    player->angle -= 0.05;
                    break;
                case SDLK_RIGHT:
                    player->angle += 0.05;
                    break;
            }
        }
    }
}

int main(void) {
    Player player = { 12, 12, M_PI / 4 }; // Start in the middle, facing right
    bool running = true;

    if (initialize() != 0) {
        return 1;
    }

    while (running) {
        handle_input(&player, &running);
        draw_maze(player);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
