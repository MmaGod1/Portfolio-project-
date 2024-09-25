#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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


#define WALL_TYPES 4     // Number of different wall types

SDL_Texture *wallTextures[WALL_TYPES];
SDL_Texture* floorTexture = NULL; // Single floor texture



// Maze map (1 = wall, 0 = empty space)
int maze_map[MAP_WIDTH][MAP_HEIGHT] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},                       
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 2, 3, 4, 0, 1, 0, 2, 2, 3, 1, 4, 0, 2, 0, 1, 3, 1, 4, 1, 0, 1},
        {1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 0, 0, 2, 2, 2, 2, 2, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 3, 3, 3, 3, 0, 2, 2, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 4, 4, 4, 4, 4, 0, 1, 0, 1, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 4, 4, 4, 4, 3, 3, 3, 2, 2, 2, 4, 3, 2, 3, 4, 3, 2, 2, 3, 4, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 4, 4, 4, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 4, 3, 2, 3, 4, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 3, 2, 1, 2, 3, 4, 3, 2, 0, 4, 0, 2, 3, 4, 3, 2, 3, 1, 2, 3, 4, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 0, 3, 3, 3, 3, 2, 2, 2, 3, 1, 1, 0, 1, 4, 1, 1, 1},
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




int loadTextures() {
    // Load wall textures
    wallTextures[0] = IMG_LoadTexture(renderer, "./wall1.jpg");
    wallTextures[1] = IMG_LoadTexture(renderer, "./wall2.jpg");
    wallTextures[2] = IMG_LoadTexture(renderer, "./wall3.jpg");
    wallTextures[3] = IMG_LoadTexture(renderer, "./wall4.jpg");

    // Load a single floor texture
    floorTexture = IMG_LoadTexture(renderer, "./floor3.1.jpg");

    // Check for wall texture loading errors
    for (int i = 0; i < 4; ++i) {
        if (!wallTextures[i]) {
            printf("Error loading wall texture %d\n", i);
            return -1; // Error loading wall textures
        }
    }

    // Check for floor texture loading error
    if (!floorTexture) {
        printf("Error loading floor texture\n");
        return -1; // Error loading floor texture
    }

    return 0; // Success
}



float castRay(float playerX, float playerY, float rayAngle, int *mapXHit, int *mapYHit, int *sideHit) {
    // Ray direction
    float rayDirX = cos(rayAngle);
    float rayDirY = sin(rayAngle);

    // Which box of the map we're in
    int mapX = (int)playerX;
    int mapY = (int)playerY;

    // Length of ray from one x or y side to next x or y side
    float deltaDistX = fabs(1 / rayDirX);
    float deltaDistY = fabs(1 / rayDirY);

    // What direction to step in (+1 or -1)
    int stepX, stepY;
    float sideDistX, sideDistY;

    // Calculate step and initial sideDist
    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (playerX - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1.0 - playerX) * deltaDistX;
    }
    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (playerY - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1.0 - playerY) * deltaDistY;
    }

    // Perform DDA
    bool hit = false;
    int side; // was the wall hit on the X or Y side?
    
    // (Other raycasting variables like rayDirX, rayDirY, deltaDistX, etc.)

    // Perform DDA (Digital Differential Analysis)
    while (!hit) {
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
        }

        if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT && maze_map[mapX][mapY] > 0) {
            hit = true;
            *mapXHit = mapX;
            *mapYHit = mapY;
            *sideHit = side;
        }
    }

    // Calculate distance to the wall
    float perpWallDist = (side == 0) ?
        (mapX - playerX + (1 - stepX) / 2) / rayDirX :
        (mapY - playerY + (1 - stepY) / 2) / rayDirY;

    return perpWallDist;
}



void drawSky() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);  // Light blue for sky
    SDL_Rect skyRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &skyRect);
}

void drawFloor() {
    SDL_Rect floorRect = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderCopy(renderer, floorTexture, NULL, &floorRect);  // Always use floorTextures[0]
}


void drawMiniMap(Player *player, bool showMap) {
    if (!showMap) return;

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
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Wall color (e.g., red)
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Empty space color (e.g., white)
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Blue color for line of sight
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        float rayAngle = player->angle - (FOV / 2) + (FOV * i / SCREEN_WIDTH);
        float endX = mapPlayerX + cos(rayAngle) * (mapWidth / 4);  // Adjust length for rays (shorter)
        float endY = mapPlayerY + sin(rayAngle) * (mapHeight / 4); // Adjust length for rays (shorter)
        SDL_RenderDrawLine(renderer, mapPlayerX, mapPlayerY, endX, endY);
    }
}





void render(Player *player) {
    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Sky blue
    SDL_RenderClear(renderer);
    
    // Draw the mini-map first, if showing
    if (showMap) {
        drawMiniMap(player, showMap);
    }

    int mapXHit, mapYHit, sideHit;

    // Loop over each vertical screen line
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float rayAngle = player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH);
        float distance = castRay(player->x, player->y, rayAngle, &mapXHit, &mapYHit, &sideHit);

        // Correct fisheye effect
        float correctedDistance = distance * cos(rayAngle - player->angle);
        int wallHeight = (int)(SCREEN_HEIGHT / correctedDistance);
        int wallTop = (SCREEN_HEIGHT / 2) - (wallHeight / 2);
        int wallBottom = (SCREEN_HEIGHT / 2) + (wallHeight / 2);

        // Ensure within screen bounds
        if (wallTop < 0) wallTop = 0;
        if (wallBottom >= SCREEN_HEIGHT) wallBottom = SCREEN_HEIGHT - 1;

        // Determine wall texture based on map hit
        int wallType = maze_map[mapXHit][mapYHit] - 1; // Assuming 1-based index in maze_map
        if (wallType < 0 || wallType >= WALL_TYPES) {
            wallType = 0; // Default to the first texture if out of bounds
        }
        SDL_Texture *currentTexture = wallTextures[wallType];

        // Calculate texture X coordinate
        float wallHitX = (sideHit == 0) ? player->y + correctedDistance * sin(rayAngle) : player->x + correctedDistance * cos(rayAngle);
        wallHitX -= floor(wallHitX); // Normalize
        int texX = (int)(wallHitX * 64); // Assuming texture width of 64
        if (sideHit == 0 && rayAngle > M_PI) texX = 64 - texX;
        if (sideHit == 1 && (rayAngle < M_PI / 2 || rayAngle > 3 * M_PI / 2)) texX = 64 - texX;

        // Render wall slice
        SDL_Rect srcRect = { texX, 0, 1, 64 }; // Texture coordinates
        SDL_Rect destRect = { x, wallTop, 1, wallHeight }; // Screen coordinates
        SDL_RenderCopy(renderer, currentTexture, &srcRect, &destRect);
    }

    // Draw the floor after the walls
    drawFloor();

    // Present the final render
    SDL_RenderPresent(renderer);
}


void updatePlayerPosition(Player *player, const Uint8 *keyState) {
    float speed = 0.1f; // Adjust as necessary
    if (keyState[SDL_SCANCODE_W]) {
        float newX = player->x + cos(player->angle) * speed;
        float newY = player->y + sin(player->angle) * speed;
        // Check for wall collision
        if (maze_map[(int)newX][(int)newY] == 0) {
            player->x = newX;
            player->y = newY;
        }
    }
    if (keyState[SDL_SCANCODE_S]) {
        float newX = player->x - cos(player->angle) * speed;
        float newY = player->y - sin(player->angle) * speed;
        // Check for wall collision
        if (maze_map[(int)newX][(int)newY] == 0) {
            player->x = newX;
            player->y = newY;
        }
    }
    if (keyState[SDL_SCANCODE_A]) {
        player->angle -= 0.1f; // Turn left
    }
    if (keyState[SDL_SCANCODE_D]) {
        player->angle += 0.1f; // Turn right
    }
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
                case SDLK_l:  // leave game when 'l' is pressed
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
                                        if (maze_map[(int)newX][(int)player->y] == 0) {              
                                               player->x = newX;      
                                        }          
                                        if (maze_map[(int)player->x][(int)newY] == 0) {             
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
    
    window = SDL_CreateWindow("Go-Maze", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

    // Load textures (replace manual texture loading with loadTextures function)
    if (loadTextures() != 0) {
        fprintf(stderr, "Failed to load textures! SDL_image Error: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Game loop
    bool running = true;
    while (running) {
            handleInput(&player, &running, maze_map); // Ensure handleInput is called

        const Uint8 *keyState = SDL_GetKeyboardState(NULL); // Get the current state of the keyboard
        updatePlayerPosition(&player, keyState); // Update player position based on input
        render(&player); // Render the scene

        SDL_Delay(16);  // Cap the frame rate to ~60 FPS
    }

    // Clean up and quit SDL
    for (int i = 0; i < WALL_TYPES; i++) {
        SDL_DestroyTexture(wallTextures[i]);
    }
    SDL_DestroyTexture(floorTexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
