#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>


SDL_Window *window;
SDL_Renderer *renderer;
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
// Global variable to toggle map display
int showMap = 1;  // 1 to show map, 0 to hide map
SDL_Texture *textureWall;
#define TEXTURE_WIDTH 64   // Set this to the width of your texture
#define TEXTURE_HEIGHT 64  // Set this to the height of your texture
SDL_Texture *wallTexture;
SDL_Texture *floorTexture;


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

typedef struct {
    float x, y;         // Player position
    float angle;        // Player angle (rotation)
    float moveSpeed;
    float rotSpeed;
} Player;



int loadTextures(SDL_Renderer *renderer)
{
    // Load wall texture
    SDL_Surface *surface = IMG_Load("./wall.jpg");
    if (!surface) {
        printf("Error loading wall texture: %s\n", IMG_GetError());
        return -1;
    }
    wallTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!wallTexture) {
        printf("Error creating wall texture: %s\n", SDL_GetError());
        return -1;
    }

    // Load floor texture
    surface = IMG_Load("./floor.jpg");
    if (!surface) {
        printf("Error loading floor texture: %s\n", IMG_GetError());
        return -1;
    }
    floorTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!floorTexture) {
        printf("Error creating floor texture: %s\n", SDL_GetError());
        return -1;
    }

    return 0;
}

float castRay(float playerX, float playerY, float rayAngle) {
    float rayX = playerX;
    float rayY = playerY;
    float stepSize = 0.05;
    float distance = 0.0;

    while (true) {
        rayX += cos(rayAngle) * stepSize;
        rayY += sin(rayAngle) * stepSize;
        distance += stepSize;

        int mapX = (int)rayX;
        int mapY = (int)rayY;

        if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
            return distance;  // Ray went out of bounds
        }

        if (maze_map[mapX][mapY] == 1) {  // Wall hit
            return distance;
        }
    }
}

void drawSky() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);  // Light blue for sky
    SDL_Rect skyRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &skyRect);
}

void drawFloor() {
    SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);  // Green for floor
    SDL_Rect floorRect = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &floorRect);
}


void render(Player *player) {
    SDL_RenderClear(renderer);

    // Draw the sky and floor
    drawSky();
    drawFloor();

    // Cast rays across the screen
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float rayAngle = player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH);
        float distance = castRay(player->x, player->y, rayAngle);

        // Debug output
        if (distance > 10.0) distance = 10.0;  // Cap the distance for better visuals

        int wallHeight = (int)(SCREEN_HEIGHT / distance);
        int wallTop = (SCREEN_HEIGHT / 2) - (wallHeight / 2);
        int wallBottom = (SCREEN_HEIGHT / 2) + (wallHeight / 2);

        // Calculate texture coordinates and width for wall slice
        SDL_Rect srcRect;
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = TEXTURE_WIDTH; // Width of the texture
        srcRect.h = TEXTURE_HEIGHT; // Height of the texture

        SDL_Rect destRect;
        destRect.x = x;
        destRect.y = wallTop;
        destRect.w = 1; // Width of the slice (1 pixel wide)
        destRect.h = wallHeight;

        // Select the appropriate texture based on the ray's distance
        SDL_Texture* textureToUse = textureWall; // Choose the right texture here

        // Draw the texture slice representing the wall
        SDL_RenderCopy(renderer, textureToUse, &srcRect, &destRect);
    }

    // Draw the map if enabled
    if (showMap) {
        int mapStartX = 10;    // Position the map in the top-left corner
        int mapStartY = 10;
        int mapWidth = 160;    // Width of the map on screen (adjust as needed)
        int mapHeight = 120;   // Height of the map on screen (adjust as needed)
        int tileSize = TILE_SIZE; // Size of each tile (you should define TILE_SIZE)

        // Draw the map tiles
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                SDL_Rect rect;
                rect.x = mapStartX + x * tileSize;  // Position of the tile
                rect.y = mapStartY + y * tileSize;  // Position of the tile
                rect.w = tileSize;                  // Width of the tile
                rect.h = tileSize;                  // Height of the tile

                if (maze_map[x][y] == 1) {
                    // Wall color (e.g., red)
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                } else {
                    // Empty space color (e.g., white)
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                }

                SDL_RenderFillRect(renderer, &rect);
            }
        }

        // Draw the player's position on the map
        float mapPlayerX = mapStartX + (player->x * mapWidth / MAP_WIDTH);
        float mapPlayerY = mapStartY + (player->y * mapHeight / MAP_HEIGHT);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green color for player's position
        SDL_Rect playerRect = { (int)mapPlayerX - 2, (int)mapPlayerY - 2, 4, 4 };  // Small rectangle to represent the player
        SDL_RenderFillRect(renderer, &playerRect);

        // Draw the player's line of sight on the map
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green color for line of sight
        for (int i = 0; i < SCREEN_WIDTH; i++) {
            float rayAngle = player->angle - (FOV / 2) + (FOV * i / SCREEN_WIDTH);
            float endX = mapPlayerX + cos(rayAngle) * (mapWidth / 2);  // Adjust length as needed
            float endY = mapPlayerY + sin(rayAngle) * (mapHeight / 2); // Adjust length as needed
            SDL_RenderDrawLine(renderer, mapPlayerX, mapPlayerY, endX, endY);
        }
    }

    SDL_RenderPresent(renderer);
}



void handleInput(Player *player, bool *running, int maze_map[MAP_WIDTH][MAP_HEIGHT]) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *running = false;
        }

        if (event.type == SDL_KEYDOWN) {
            float moveStep = player->moveSpeed;
            float moveAngle = player->angle;

            switch (event.key.keysym.sym) {
                case SDLK_q:  // Quit when 'q' is pressed
                    *running = false;
                    break;
                case SDLK_m:
                        showMap = !showMap;
                        break;
                case SDLK_LEFT:  // Rotate left
                    player->angle -= player->rotSpeed;
                    if (player->angle < 0) player->angle += 2 * M_PI;
                    break;
                case SDLK_RIGHT:  // Rotate right
                    player->angle += player->rotSpeed;
                    if (player->angle > 2 * M_PI) player->angle -= 2 * M_PI;
                    break;
                case SDLK_w:  // Move forward
                    {
                        float newX = player->x + cos(moveAngle) * moveStep;
                        float newY = player->y + sin(moveAngle) * moveStep;

                        // Ensure the new position is within bounds
                        if (newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT) {
                            // Check for collisions with walls
                            if (maze_map[(int)newX][(int)player->y] == 0 && maze_map[(int)newX][(int)player->y] != 1) {
                                player->x = newX;
                            }
                            if (maze_map[(int)player->x][(int)newY] == 0 && maze_map[(int)player->x][(int)newY] != 1) {
                                player->y = newY;
                            }
                        }
                    }
                    break;
                case SDLK_s:  // Move backward
                    {
                        float newX = player->x - cos(moveAngle) * moveStep;
                        float newY = player->y - sin(moveAngle) * moveStep;

                        // Ensure the new position is within bounds
                        if (newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT) {
                            // Check for collisions with walls
                            if (maze_map[(int)newX][(int)player->y] == 0 && maze_map[(int)newX][(int)player->y] != 1) {
                                player->x = newX;
                            }
                            if (maze_map[(int)player->x][(int)newY] == 0 && maze_map[(int)player->x][(int)newY] != 1) {
                                player->y = newY;
                            }
                        }
                    }
                    break;
                case SDLK_a:  // Strafe left
                    {
                        float strafeAngle = player->angle - M_PI / 2;
                        float newX = player->x + cos(strafeAngle) * moveStep;
                        float newY = player->y + sin(strafeAngle) * moveStep;

                        // Ensure the new position is within bounds
                        if (newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT) {
                            // Check for collisions with walls
                            if (maze_map[(int)newX][(int)player->y] == 0 && maze_map[(int)newX][(int)player->y] != 1) {
                                player->x = newX;
                            }
                            if (maze_map[(int)player->x][(int)newY] == 0 && maze_map[(int)player->x][(int)newY] != 1) {
                                player->y = newY;
                            }
                        }
                    }
                    break;
                case SDLK_d:  // Strafe right
                    {
                        float strafeAngle = player->angle + M_PI / 2;
                        float newX = player->x + cos(strafeAngle) * moveStep;
                        float newY = player->y + sin(strafeAngle) * moveStep;

                        // Ensure the new position is within bounds
                        if (newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT) {
                            // Check for collisions with walls
                            if (maze_map[(int)newX][(int)player->y] == 0 && maze_map[(int)newX][(int)player->y] != 1) {
                                player->x = newX;
                            }
                            if (maze_map[(int)player->x][(int)newY] == 0 && maze_map[(int)player->x][(int)newY] != 1) {
                                player->y = newY;
                            }
                        }
                    }
                    break;
            }
        }
    }
}


int loadMap(const char *filename, int maze_map[MAP_WIDTH][MAP_HEIGHT]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return -1;
    }

    int ch;  // Declare the variable 'ch' here

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            ch = fgetc(file);
            while (ch == '\r' || ch == '\n') {  // Skip newline and carriage return characters
                ch = fgetc(file);
            }

            if (ch == EOF) {
                fprintf(stderr, "Error reading file %s: unexpected EOF\n", filename);
                fclose(file);
                return -1;
            }

            if (ch == '#') {  // Wall character
                maze_map[x][y] = 1;
            } else if (ch == '.') {  // Empty space character
                maze_map[x][y] = 0;
            } else {
                fprintf(stderr, "Invalid character '%c' in map file at [%d,%d]\n", ch, x, y);
                fclose(file);
                return -1;
            }
        }

        // Skip over remaining characters (newline or carriage return)
        while ((ch = fgetc(file)) == '\r' || ch == '\n');
        if (ch != EOF) {
            ungetc(ch, file);  // Put the last character back if not EOF
        }
    }

    fclose(file);
    return 0;
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <mapfile>\n", argv[0]);
        return 1;
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (!IMG_Init(IMG_INIT_PNG))
    {
        printf("SDL_image could not initialize! IMG_Error: %s\n", IMG_GetError());
        return 1;
    }

  
    window = SDL_CreateWindow("Raycasting Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

     // Load textures
    if (loadTextures(renderer) < 0) {
        return 1;
    }

    // Initialize player
    Player player = { .x = 2.0, .y = 2.0, .angle = 0.0, .moveSpeed = 0.05, .rotSpeed = 0.05 };

    // Load the map
    if (loadMap(argv[1], maze_map) != 0) {
        fprintf(stderr, "Failed to load map\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Game loop
    bool running = true;
    while (running) {
        handleInput(&player, &running, maze_map);  // Handle player input
        render(&player);                 // Render the scene

        SDL_Delay(16);  // Cap the frame rate to ~60 FPS
    }

    // Clean up and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
