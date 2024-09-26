#ifndef RAYCASTING_H
#define RAYCASTING_H

#include <SDL2/SDL.h>
#include <math.h>
#include <SDL2/SDL_image.h>
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



typedef struct {
    float x, y;         // Player position
    float angle;        // Player angle (rotation)
    float moveSpeed;
    float rotSpeed;
} Player;

typedef struct {
    SDL_Texture *texture;
    int width, height;
} Texture;


SDL_Window *window;
SDL_Renderer *renderer;

Texture wallTextures[4];
Texture floorTexture;


Texture loadTexture(const char *filename);
float castRay(float playerX, float playerY, float rayAngle);
void drawSky();
void drawMiniMap(Player *player, bool showMap);
void drawFloor(Player *player);
float getWallHitCoordinates(float playerX, float playerY, float rayAngle, int *mapX, int *mapY);
void renderWalls(Player *player);
void render(Player *player);
void handleInput(Player *player, bool *running, int maze_map[MAP_WIDTH][MAP_HEIGHT]);
int loadMap(const char *filename, int maze_map[MAP_WIDTH][MAP_HEIGHT]);

#endif 
