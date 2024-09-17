#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>

// Player attributes
float playerX = 1.5f, playerY = 1.5f;  // Starting position
float playerAngle = 0.0f;  // Camera facing angle
float moveSpeed = 0.05f;   // Movement speed
float rotateSpeed = 0.05f; // Rotation speed

// Maze dimensions
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

int maze_map[MAP_WIDTH][MAP_HEIGHT] = {
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
    bool collisionX = false, collisionY = false;
    
    if (key == SDLK_w) {
        // Move forward
        newX = playerX + dirX * moveSpeed;
        newY = playerY + dirY * moveSpeed;
        
        if (!is_wall(newX, playerY)) playerX = newX; // Check movement in X direction
        else collisionX = true;
        if (!is_wall(playerX, newY)) playerY = newY; // Check movement in Y direction
        else collisionY = true;
        
        // Slide along the wall if necessary
        if (collisionX && !collisionY) {
            newX = playerX + perpX * moveSpeed;
            newY = playerY + perpY * moveSpeed;
            if (!is_wall(newX, playerY)) playerX = newX;
            if (!is_wall(playerX, newY)) playerY = newY;
        } else if (collisionY && !collisionX) {
            newX = playerX + dirX * moveSpeed;
            newY = playerY + dirY * moveSpeed;
            if (!is_wall(playerX, newY)) playerY = newY;
            if (!is_wall(newX, playerY)) playerX = newX;
        }
    }
    
    if (key == SDLK_s) {
        // Move backward
        newX = playerX - dirX * moveSpeed;
        newY = playerY - dirY * moveSpeed;
        
        if (!is_wall(newX, playerY)) playerX = newX;
        else collisionX = true;
        if (!is_wall(playerX, newY)) playerY = newY;
        else collisionY = true;
        
        // Slide along the wall if necessary
        if (collisionX && !collisionY) {
            newX = playerX - perpX * moveSpeed;
            newY = playerY - perpY * moveSpeed;
            if (!is_wall(newX, playerY)) playerX = newX;
            if (!is_wall(playerX, newY)) playerY = newY;
        } else if (collisionY && !collisionX) {
            newX = playerX - dirX * moveSpeed;
            newY = playerY - dirY * moveSpeed;
            if (!is_wall(playerX, newY)) playerY = newY;
            if (!is_wall(newX, playerY)) playerX = newX;
        }
    }
    
    if (key == SDLK_a) {
        // Strafe left
        newX = playerX + perpX * moveSpeed;
        newY = playerY + perpY * moveSpeed;
        
        if (!is_wall(newX, playerY)) playerX = newX;
        else collisionX = true;
        if (!is_wall(playerX, newY)) playerY = newY;
        else collisionY = true;
        
        // Slide along the wall if necessary
        if (collisionX && !collisionY) {
            newX = playerX + dirX * moveSpeed;
            newY = playerY + dirY * moveSpeed;
            if (!is_wall(newX, playerY)) playerX = newX;
            if (!is_wall(playerX, newY)) playerY = newY;
        } else if (collisionY && !collisionX) {
            newX = playerX + perpX * moveSpeed;
            newY = playerY + perpY * moveSpeed;
            if (!is_wall(playerX, newY)) playerY = newY;
            if (!is_wall(newX, playerY)) playerX = newX;
        }
    }
    
    if (key == SDLK_d) {
        // Strafe right
        newX = playerX - perpX * moveSpeed;
        newY = playerY - perpY * moveSpeed;
        
        if (!is_wall(newX, playerY)) playerX = newX;
        else collisionX = true;
        if (!is_wall(playerX, newY)) playerY = newY;
        else collisionY = true;
        
        // Slide along the wall if necessary
        if (collisionX && !collisionY) {
            newX = playerX - dirX * moveSpeed;
            newY = playerY - dirY * moveSpeed;
            if (!is_wall(newX, playerY)) playerX = newX;
            if (!is_wall(playerX, newY)) playerY = newY;
        } else if (collisionY && !collisionX) {
            newX = playerX - perpX * moveSpeed;
            newY = playerY - perpY * moveSpeed;
            if (!is_wall(playerX, newY)) playerY = newY;
            if (!is_wall(newX, playerY)) playerX = newX;
        }
    }
}

void handle_events(SDL_Event *event) {
    if (event->type == SDL_QUIT) {
        exit(0);
    }
    
    if (event->type == SDL_KEYDOWN) {
        handle_movement(event->key.keysym.sym);
    }
}

// Your main rendering function
void render() {
    // Your rendering code here
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Raycasting Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_Event event;
    bool running = true;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            handle_events(&event);
        }
        
        render();
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Cap at ~60 FPS
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
