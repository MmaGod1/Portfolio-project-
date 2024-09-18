#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>

// Constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define FOV (M_PI / 3) // 60 degrees field of view
#define WALL_CHAR '#'
#define EMPTY_CHAR '.'
#define MAP_DISPLAY_WIDTH 200
#define MAP_DISPLAY_HEIGHT 150
#define TILE_SIZE (MAP_DISPLAY_WIDTH / MAP_WIDTH)
#define MINIMAP_SCALE_FACTOR 0.25

// Global variable to toggle map display
int showMap = 1;  // 1 to show map, 0 to hide map

// SDL variables
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *wallTexture;
SDL_Texture *floorTexture;

// Struct for player details
typedef struct {
    float x, y;         // Player position
    float angle;        // Player angle (rotation)
    float moveSpeed;
    float rotSpeed;
} Player;

// Maze map (1 = wall, 0 = empty space)
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




// Function to load textures
SDL_Texture* loadTexture(const char *path) {
    SDL_Texture *newTexture = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
    } else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

// Initialize SDL, window, and load textures
int init(const char* wallTexturePath, const char* floorTexturePath) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 0;
    }

    if (!IMG_Init(IMG_INIT_PNG)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return 0;
    }

    window = SDL_CreateWindow("Raycasting Maze", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return 0;
    }

    wallTexture = loadTexture(wallTexturePath);
    floorTexture = loadTexture(floorTexturePath);
    if (wallTexture == NULL || floorTexture == NULL) {
        return 0;
    }

    return 1;
}

// Close and clean up SDL
void closeSDL() {
    SDL_DestroyTexture(wallTexture);
    SDL_DestroyTexture(floorTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

// Render the minimap
void renderMap(int map[MAP_WIDTH][MAP_HEIGHT]) {
    if (showMap) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                SDL_Rect tileRect = {x * TILE_SIZE * MINIMAP_SCALE_FACTOR, y * TILE_SIZE * MINIMAP_SCALE_FACTOR, TILE_SIZE * MINIMAP_SCALE_FACTOR, TILE_SIZE * MINIMAP_SCALE_FACTOR};
                if (map[y][x] == 1) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White for walls
                } else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black for empty spaces
                }
                SDL_RenderFillRect(renderer, &tileRect);
            }
        }
    }
}

// Handle player input
void handleInput(Player *player, int map[MAP_WIDTH][MAP_HEIGHT], bool *quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            *quit = true;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    player->x += player->moveSpeed * cos(player->angle);
                    player->y += player->moveSpeed * sin(player->angle);
                    break;
                case SDLK_DOWN:
                    player->x -= player->moveSpeed * cos(player->angle);
                    player->y -= player->moveSpeed * sin(player->angle);
                    break;
                case SDLK_LEFT:
                    player->angle -= player->rotSpeed;
                    break;
                case SDLK_RIGHT:
                    player->angle += player->rotSpeed;
                    break;
                case SDLK_m:  // Toggle minimap
                    showMap = !showMap;
                    break;
            }
        }
    }
}

// Render the game scene
void renderScene(Player *player) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Set background color to black
    SDL_RenderClear(renderer);

    // Render floor
    SDL_Rect floorRect = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderCopy(renderer, floorTexture, NULL, &floorRect);

    // Render walls (pseudo-raycasting)
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        SDL_Rect wallRect = {x, SCREEN_HEIGHT / 4, 1, SCREEN_HEIGHT / 2}; // Placeholder for each ray
        SDL_RenderCopy(renderer, wallTexture, NULL, &wallRect);
    }
}

int main(int argc, char *argv[]) {
    const char* wallTexturePath = "path_to_wall_texture.png";
    const char* floorTexturePath = "path_to_floor_texture.png";

    if (!init(wallTexturePath, floorTexturePath)) {
        printf("Failed to initialize!\n");
        return 1;
    }

    Player player = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, 4.0f, 0.05f};
    int map[MAP_WIDTH][MAP_HEIGHT];  // Placeholder for your map

    bool quit = false;

    while (!quit) {
        handleInput(&player, map, &quit);
        renderScene(&player);
        renderMap(map);
        SDL_RenderPresent(renderer);
    }

    closeSDL();
    return 0;
}
