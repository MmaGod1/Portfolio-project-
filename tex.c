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
#define TEX_WIDTH 64
#define TEX_HEIGHT 64
SDL_Texture* wallTextures[4];
SDL_Texture* floorTexture;
Uint32 buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
SDL_Window *window;
SDL_Renderer *renderer;



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
    while (!hit) {
        // Jump to next map square, either in x-direction or y-direction
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
        }

        // Check if ray has hit a wall
        if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT && maze_map[mapX][mapY] == 1) {
            hit = true;
        }
    }

    // Calculate the distance to the point of impact
    float perpWallDist;
    if (side == 0)
        perpWallDist = (mapX - playerX + (1 - stepX) / 2) / rayDirX;
    else
        perpWallDist = (mapY - playerY + (1 - stepY) / 2) / rayDirY;

    return perpWallDist;
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



void render(Player *player, SDL_Texture *floorTexture, SDL_Texture *wallTextures[]) {
    SDL_RenderClear(renderer);

    // Draw the floor texture
    SDL_Rect floorRect = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderCopy(renderer, floorTexture, NULL, &floorRect);

    // Cast rays across the screen
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float rayAngle = player->angle - (FOV / 2) + (FOV * x / SCREEN_WIDTH);
        float distance = castRay(player->x, player->y, rayAngle);

        // Cap the distance for better visuals
        if (distance > 10.0) distance = 10.0;

        // Correct distance for wall height calculation
        float correctedDistance = distance * cos(rayAngle - player->angle);

        // Calculate wall height and position
        int wallHeight = (int)(SCREEN_HEIGHT / correctedDistance);
        int wallTop = (SCREEN_HEIGHT / 2) - (wallHeight / 2);
        int wallBottom = (SCREEN_HEIGHT / 2) + (wallHeight / 2);

        // Ensure wall heights are non-negative
        if (wallHeight < 0) wallHeight = 0;
        if (wallTop < 0) wallTop = 0;
        if (wallBottom >= SCREEN_HEIGHT) wallBottom = SCREEN_HEIGHT - 1;

        // Choose a wall texture based on the ray's angle or some other criteria
        int textureIndex = (int)(rayAngle * 4 / (2 * M_PI)); // Simplified example
        textureIndex = (textureIndex + 4) % 4; // Ensure index is in range
        SDL_Rect wallRect = {x, wallTop, 1, wallHeight};
        SDL_RenderCopy(renderer, wallTextures[textureIndex], NULL, &wallRect);
    }

    // Draw the map if enabled
    if (showMap) {
        drawMiniMap(player, showMap);
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


void loadTextures(SDL_Renderer* renderer) {
    wallTextures[0] = IMG_LoadTexture(renderer, "./wall1.jpg");
    wallTextures[1] = IMG_LoadTexture(renderer, "./wall2.jpg");
    wallTextures[2] = IMG_LoadTexture(renderer, "./wall3.jpg");
    wallTextures[3] = IMG_LoadTexture(renderer, "./wall4.jpg");
    floorTexture = IMG_LoadTexture(renderer, "./floor3.1.jpg");
}


void renderFrame(SDL_Renderer* renderer, double posX, double posY, double dirX, double dirY, double planeX, double planeY) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        // Calculate ray position and direction
        double cameraX = 2 * x / (double)SCREEN_WIDTH - 1; // x-coordinate in camera space
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        int mapXHit, mapYHit, sideHit;
        float perpWallDist = castRay(posX, posY, atan2(rayDirY, rayDirX), &mapXHit, &mapYHit, &sideHit);

        // Calculate height of line to draw on screen
        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

        // Calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

        // Texturing calculations
        int texNum = maze_map[mapXHit][mapYHit] - 1; // 1 subtracted so that texture 0 can be used!
        double wallX;
        if (sideHit == 0) wallX = posY + perpWallDist * rayDirY; // X-axis hit
        else wallX = posX + perpWallDist * rayDirX; // Y-axis hit
        wallX -= floor(wallX);

        // X coordinate on the texture
        int texX = (int)(wallX * (double)TEX_WIDTH);
        if (sideHit == 0 && rayDirX > 0) texX = TEX_WIDTH - texX - 1;
        if (sideHit == 1 && rayDirY < 0) texX = TEX_WIDTH - texX - 1;

        // How much to increase the texture coordinate per screen pixel
        double step = 1.0 * TEX_HEIGHT / lineHeight;
        double texPos = (drawStart - SCREEN_HEIGHT / 2 + lineHeight / 2) * step;

        for (int y = drawStart; y < drawEnd; y++) {
            int texY = (int)texPos & (TEX_HEIGHT - 1);
            texPos += step;

            // Get pixel color from texture
            SDL_Rect srcRect = { texX, texY, 1, 1 };
            SDL_Rect dstRect = { x, y, 1, 1 };
            SDL_RenderCopy(renderer, wallTextures[texNum], &srcRect, &dstRect);
        }
    }

    // Optional: render the floor (simple flat shading for demonstration)
    SDL_Rect floorRect = { 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 };
    SDL_RenderCopy(renderer, floorTexture, NULL, &floorRect);
}


int main(int argc, char* argv[]) {
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Maze Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // Load textures
    loadTextures(renderer);

    // Player's initial position and direction
    double posX = 22.0, posY = 11.5; // start position
    double dirX = -1.0, dirY = 0.0; // initial direction
    double planeX = 0.0, planeY = 0.66; // camera plane

    // Main loop
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        renderFrame(renderer, posX, posY, dirX, dirY, planeX, planeY);
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Approx 60 FPS
    }

    // Clean up
    for (int i = 0; i < 4; i++) {
        SDL_DestroyTexture(wallTextures[i]);
    }
    SDL_DestroyTexture(floorTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
