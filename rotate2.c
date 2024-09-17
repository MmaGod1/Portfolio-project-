#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define FOV (M_PI / 3) // Field of View (60 degrees)

// Maze map (1 = wall, 0 = floor)
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

typedef struct {
    float x, y;
    float angle; // Player's viewing angle
    float rotationSpeed;
    float movementSpeed;
} Player;

SDL_Window *window;
SDL_Renderer *renderer;

void drawSky() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Sky blue
    SDL_Rect skyRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &skyRect);
}

void drawFloor() {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green floor
    SDL_Rect floorRect = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &floorRect);
}

// Function to cast rays and determine distance to walls
float castRay(float playerX, float playerY, float rayAngle) {
    float rayX = playerX;
    float rayY = playerY;
    float stepSize = 0.1; // Step size for ray
    float distanceToWall = 0;
    
    while (true) {
        rayX += cos(rayAngle) * stepSize;
        rayY += sin(rayAngle) * stepSize;
        distanceToWall += stepSize;
        
        int mapX = (int)rayX;
        int mapY = (int)rayY;
        
        if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
            return distanceToWall;
        }
        
        if (maze_map[mapX][mapY] == 1) {
            return distanceToWall;
        }
    }
}

void render(Player *player) {
    SDL_RenderClear(renderer);
    
    // Draw the sky and floor
    drawSky();
    drawFloor();
    
    // Loop through each vertical stripe (column) of the screen
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        // Calculate the current ray angle
        float rayAngle = player->angle - (FOV / 2) + (FOV / SCREEN_WIDTH) * x;
        float distanceToWall = castRay(player->x, player->y, rayAngle);
        
        // Calculate the height of the wall slice based on distance
        int wallHeight = (int)(SCREEN_HEIGHT / distanceToWall);
        int drawStart = (SCREEN_HEIGHT / 2) - (wallHeight / 2);
        int drawEnd = drawStart + wallHeight;
        
        // Set color based on wall orientation (you can tweak this for different directions)
        if (fabs(cos(rayAngle)) > fabs(sin(rayAngle))) {
            SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255); // Gray for vertical walls
        } else {
            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Darker gray for horizontal walls
        }
        
        // Draw the wall slice
        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }
    
    SDL_RenderPresent(renderer);
}

void handle_input(Player *player) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    // Handle left/right rotation
    if (state[SDL_SCANCODE_LEFT]) {
        player->angle -= player->rotationSpeed;
    }
    if (state[SDL_SCANCODE_RIGHT]) {
        player->angle += player->rotationSpeed;
    }
    
    // Handle forward/backward movement
    if (state[SDL_SCANCODE_UP]) {
        player->x += cos(player->angle) * player->movementSpeed;
        player->y += sin(player->angle) * player->movementSpeed;
    }
    if (state[SDL_SCANCODE_DOWN]) {
        player->x -= cos(player->angle) * player->movementSpeed;
        player->y -= sin(player->angle) * player->movementSpeed;
    }
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("3D Maze", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    // Initialize player data
    Player player = { .x = 12, .y = 12, .angle = 0, .rotationSpeed = 0.05, .movementSpeed = 0.1 };

    // Game loop
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        
        // Handle input and update player movement
        handle_input(&player);
        
        // Render the 3D maze
        render(&player);
        
        // Delay to control frame rate (~60 FPS)
        SDL_Delay(16);
    }
    
    // Clean up and quit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
