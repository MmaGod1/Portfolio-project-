#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>

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

SDL_Window *window;
SDL_Renderer *renderer;

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

    // Draw the map if enabled
    if (showMap) {
        // Map parameters
        int mapStartX = 0;
        int mapStartY = 0;
        int mapWidth = MAP_DISPLAY_WIDTH;  // Define map width (e.g., 160)
        int mapHeight = MAP_DISPLAY_HEIGHT; // Define map height (e.g., 120)

        // Draw the map tiles
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                SDL_Rect rect;
                rect.x = mapStartX + x * TILE_SIZE;  // Position of the tile
                rect.y = mapStartY + y * TILE_SIZE;  // Position of the tile
                rect.w = TILE_SIZE;                  // Width of the tile
                rect.h = TILE_SIZE;                  // Height of the tile

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

        // Draw a border around the map for clarity
        SDL_Rect border;
        border.x = mapStartX;
        border.y = mapStartY;
        border.w = mapWidth;
        border.h = mapHeight;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black border
        SDL_RenderDrawRect(renderer, &border);

        // Draw the player’s line of sight on the map
        // Example: Draw a line from the player's position to the map's boundary
        float mapPlayerX = mapStartX + player->x * TILE_SIZE / MAP_WIDTH; // Scale player’s x to map size
        float mapPlayerY = mapStartY + player->y * TILE_SIZE / MAP_HEIGHT; // Scale player’s y to map size

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green color for player’s line of sight
        for (int i = 0; i < SCREEN_WIDTH; i++) {
            float rayAngle = player->angle - (FOV / 2) + (FOV * i / SCREEN_WIDTH);
            float endX = mapPlayerX + cos(rayAngle) * 20;  // Adjust length as needed
            float endY = mapPlayerY + sin(rayAngle) * 20;  // Adjust length as needed
            SDL_RenderDrawLine(renderer, mapPlayerX, mapPlayerY, endX, endY);
        }
    }

    // Cast rays across the screen
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float rayAngle = player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH);
        float distance = castRay(player->x, player->y, rayAngle);

        // Debug output
        if (distance > 10.0) distance = 10.0;  // Cap the distance for better visuals

        int wallHeight = (int)(SCREEN_HEIGHT / distance);
        int wallTop = (SCREEN_HEIGHT / 2) - (wallHeight / 2);
        int wallBottom = (SCREEN_HEIGHT / 2) + (wallHeight / 2);

        // Draw the vertical line representing the wall slice
        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);  // Light gray
        SDL_RenderDrawLine(renderer, x, wallTop, x, wallBottom);
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
                case SDLK_m:
                            showMap = !showMap;
                            break;
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
