#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

// Screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Player attributes
float playerX = 1.5f, playerY = 1.5f;  // Starting position
float playerAngle = 0.0f;  // Camera facing angle
float moveSpeed = 0.05f;   // Movement speed
float rotateSpeed = 0.05f; // Rotation speed

// Maze dimensions
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

int maze_map[MAP_WIDTH][MAP_HEIGHT] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},                       
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};
// Check collision function
bool is_wall(float x, float y) {
    int mapX = (int)x;
    int mapY = (int)y;
    
    // Check if the new position is inside the map and if it's a wall
    if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT) {
        return maze_map[mapX][mapY] == 1;
    }
    return true; // If out of bounds, treat as a wall
}

// Handle player movement
void handle_movement(SDL_Keycode key) {
    float dirX = cos(playerAngle);
    float dirY = sin(playerAngle);
    float perpX = -dirY;  // Perpendicular direction (left)
    float perpY = dirX;   // Perpendicular direction (up)
    
    float newX, newY;
    
    if (key == SDLK_w) {
        // Move forward
        newX = playerX + dirX * moveSpeed;
        newY = playerY + dirY * moveSpeed;
        
        if (!is_wall(newX, playerY)) playerX = newX;
        if (!is_wall(playerX, newY)) playerY = newY;
    }
    
    if (key == SDLK_s) {
        // Move backward
        newX = playerX - dirX * moveSpeed;
        newY = playerY - dirY * moveSpeed;
        
        if (!is_wall(newX, playerY)) playerX = newX;
        if (!is_wall(playerX, newY)) playerY = newY;
    }
    
    if (key == SDLK_a) {
        // Strafe left
        newX = playerX + perpX * moveSpeed;
        newY = playerY + perpY * moveSpeed;
        
        if (!is_wall(newX, playerY)) playerX = newX;
        if (!is_wall(playerX, newY)) playerY = newY;
    }
    
    if (key == SDLK_d) {
        // Strafe right
        newX = playerX - perpX * moveSpeed;
        newY = playerY - perpY * moveSpeed;
        
        if (!is_wall(newX, playerY)) playerX = newX;
        if (!is_wall(playerX, newY)) playerY = newY;
    }
}

// Handle rotation
void handle_rotation(SDL_Event *event) {
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_LEFT) {
            playerAngle -= rotateSpeed;
        } else if (event->key.keysym.sym == SDLK_RIGHT) {
            playerAngle += rotateSpeed;
        }
    }
}

// Your main rendering function
void render(SDL_Renderer *renderer) {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);
    
    // Draw player position (for debugging)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red player position
    SDL_RenderDrawPoint(renderer, (int)(playerX * 40), (int)(playerY * 40)); // Scale for visibility
    
    // Draw maze (for debugging)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White walls
    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            if (maze_map[x][y] == 1) {
                SDL_Rect rect = { x * 40, y * 40, 40, 40 }; // Scale for visibility
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Raycasting Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_Event event;
    bool running = true;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            handle_movement(event.key.keysym.sym);
            handle_rotation(&event);
        }
        
        render(renderer);
        SDL_Delay(16); // Cap at ~60 FPS
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
