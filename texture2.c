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
    float distance = INFINITY;

    // Normalize the angle
    while (rayAngle < 0) rayAngle += 2 * M_PI;
    while (rayAngle >= 2 * M_PI) rayAngle -= 2 * M_PI;

    printf("Casting ray at angle: %f\n", rayAngle); // Debug print

    // Horizontal raycasting
    float xStep = cos(rayAngle) > 0 ? 1.0 : -1.0;
    float yStep = sin(rayAngle) > 0 ? 1.0 : -1.0;
    float tanAngle = tan(rayAngle);
    float horizontalDist = INFINITY;
    
    float yIntercept = floor(rayY) + (sin(rayAngle) > 0 ? 1.0 : 0.0);
    float xIntercept = rayX + (yIntercept - rayY) * tanAngle;

    while (xIntercept >= 0 && xIntercept < MAP_WIDTH && yIntercept >= 0 && yIntercept < MAP_HEIGHT) {
        if (maze_map[(int)xIntercept][(int)yIntercept] == 1) {
            horizontalDist = sqrt(pow(rayX - xIntercept, 2) + pow(rayY - yIntercept, 2));
            printf("Horizontal hit at: (%f, %f) Distance: %f\n", xIntercept, yIntercept, horizontalDist); // Debug print
            break;
        }
        yIntercept += yStep;
        xIntercept += tanAngle * xStep;
    }

    // Vertical raycasting
    float verticalDist = INFINITY;
    float xIntercept2 = floor(rayX) + (cos(rayAngle) > 0 ? 1.0 : 0.0);
    float yIntercept2 = rayY + (xIntercept2 - rayX) / tanAngle;

    while (xIntercept2 >= 0 && xIntercept2 < MAP_WIDTH && yIntercept2 >= 0 && yIntercept2 < MAP_HEIGHT) {
        if (maze_map[(int)xIntercept2][(int)yIntercept2] == 1) {
            verticalDist = sqrt(pow(rayX - xIntercept2, 2) + pow(rayY - yIntercept2, 2));
            printf("Vertical hit at: (%f, %f) Distance: %f\n", xIntercept2, yIntercept2, verticalDist); // Debug print
            break;
        }
        xIntercept2 += xStep;
        yIntercept2 += (1.0 / tanAngle) * yStep;
    }

    // Return the shortest distance
    float finalDist = fmin(horizontalDist, verticalDist);
    printf("Final Distance: %f\n", finalDist); // Debug print
    return finalDist;
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

        // Cap the distance for better visuals
        if (distance > 10.0) distance = 10.0;

        // Correct distance for wall height calculation
        float correctedDistance = distance * cos(rayAngle - player->angle);

        // Calculate wall height and position
        int wallHeight = (int)(SCREEN_HEIGHT / correctedDistance);
        int wallTop = (SCREEN_HEIGHT / 2) - (wallHeight / 2);
        int wallBottom = (SCREEN_HEIGHT / 2) + (wallHeight / 2);

        printf("Wall at x=%d: Height=%d, Top=%d, Bottom=%d\n", x, wallHeight, wallTop, wallBottom); // Debug print

        // Ensure wall heights are non-negative
        if (wallHeight < 0) wallHeight = 0;
        if (wallTop < 0) wallTop = 0;
        if (wallBottom >= SCREEN_HEIGHT) wallBottom = SCREEN_HEIGHT - 1;

        // Draw the vertical line representing the wall slice
        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);  // Light gray
        SDL_RenderDrawLine(renderer, x, wallTop, x, wallBottom);
    }

    // Draw the map if enabled
    if (showMap) {
        // Map drawing logic
        // ...

        // Print player map position for debugging
        float mapPlayerX = mapStartX + (player->x * mapWidth / MAP_WIDTH);
        float mapPlayerY = mapStartY + (player->y * mapHeight / MAP_HEIGHT);
        printf("Player on map at: (%f, %f)\n", mapPlayerX, mapPlayerY);
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
                case SDLK_s:  // Move backward
                {
                    float direction = (event.key.keysym.sym == SDLK_w) ? 1.0 : -1.0;
                    float newX = player->x + cos(moveAngle) * moveStep * direction;
                    float newY = player->y + sin(moveAngle) * moveStep * direction;

                    // Check if the new position is within bounds
                    if (newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT) {
                        // Check for collisions
                        if (maze_map[(int)newX][(int)player->y] == 0) {
                            player->x = newX;
                        }
                        if (maze_map[(int)player->x][(int)newY] == 0) {
                            player->y = newY;
                        }
                    }
                    printf("Moved player to: (%f, %f)\n", player->x, player->y); // Debug print
                }
                    break;
                case SDLK_a:  // Strafe left
                case SDLK_d:  // Strafe right
                {
                    float strafeAngle = (event.key.keysym.sym == SDLK_a) ? player->angle - M_PI / 2 : player->angle + M_PI / 2;
                    float newX = player->x + cos(strafeAngle) * moveStep;
                    float newY = player->y + sin(strafeAngle) * moveStep;

                    // Check if the new position is within bounds
                    if (newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT) {
                        // Check for collisions
                        if (maze_map[(int)newX][(int)player->y] == 0) {
                            player->x = newX;
                        }
                        if (maze_map[(int)player->x][(int)newY] == 0) {
                            player->y = newY;
                        }
                    }
                    printf("Strafed player to: (%f, %f)\n", player->x, player->y); // Debug print
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

    int ch;
    int x = 0, y = 0;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\r' || ch == '\n') {
            // Skip line breaks
            if (x > 0) {
                y++;
                x = 0;
            }
        } else if (ch >= '0' && ch <= '9') {
            // Convert character to integer and store in maze_map
            maze_map[x][y] = ch - '0';
            x++;
            if (x >= MAP_WIDTH) {
                fprintf(stderr, "Warning: Line exceeds map width\n");
                break;
            }
        } else {
            fprintf(stderr, "Warning: Unexpected character '%c' in map file\n", ch);
        }
    }

    if (y >= MAP_HEIGHT) {
        fprintf(stderr, "Warning: Map file exceeds map height\n");
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
