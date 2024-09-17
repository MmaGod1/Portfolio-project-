#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "SDL2/SDL.h"

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
    int mapX, mapY;
    double rayAngle;
    double rayDirX, rayDirY;
    double distToWall;
    int wallHeight;
    SDL_Color wallColor;

    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Sky color
    SDL_RenderClear(renderer);

    for (int i = 0; i < SCREEN_WIDTH; i++) {
        rayAngle = (player.angle - FOV / 2.0) + (FOV / SCREEN_WIDTH) * i;
        rayDirX = cos(rayAngle);
        rayDirY = sin(rayAngle);

        distToWall = 0;
        bool hit = false;
        int wallSide = 0; // 0 = North/South, 1 = East/West

        while (!hit && distToWall < 20) {  // Arbitrary max distance
            distToWall += 0.1;
            mapX = (int)(player.x + rayDirX * distToWall);
            mapY = (int)(player.y + rayDirY * distToWall);

            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
                hit = true;
                distToWall = 20; // Set distance to max
            } else if (maze_map[mapY][mapX] == 1) {
                hit = true;

                // Determine wall orientation
                if (fabs(rayDirY) > fabs(rayDirX)) {
                    wallSide = 0; // North/South
                } else {
                    wallSide = 1; // East/West
                }
            }
        }

        double wallDist = distToWall * cos(player.angle - rayAngle); // Correct for player angle
        wallHeight = (int)(SCREEN_HEIGHT / wallDist);

        // Set wall color based on orientation
        if (wallSide == 0) {
            wallColor = (SDL_Color){169, 169, 169, 255}; // Gray for North/South walls
        } else {
            wallColor = (SDL_Color){192, 192, 192, 255}; // Light gray for East/West walls
        }

        // Draw wall
        SDL_SetRenderDrawColor(renderer, wallColor.r, wallColor.g, wallColor.b, wallColor.a);
        SDL_RenderDrawLine(renderer, i, (SCREEN_HEIGHT / 2) - (wallHeight / 2), i, (SCREEN_HEIGHT / 2) + (wallHeight / 2));
    }

    // Draw the floor (Green color)
    SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255); // Green color for floor
    SDL_Rect floorRect = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &floorRect);

    SDL_RenderPresent(renderer);
}

void draw_map(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for map lines
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (maze_map[y][x] == 1) {
                SDL_Rect rect = {x * (SCREEN_WIDTH / MAP_WIDTH), y * (SCREEN_HEIGHT / MAP_HEIGHT), SCREEN_WIDTH / MAP_WIDTH, SCREEN_HEIGHT / MAP_HEIGHT};
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Wall color (black)
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
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
        draw_map(); // Draw the map on top of the maze
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
