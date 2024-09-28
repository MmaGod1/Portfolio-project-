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

typedef struct {
    SDL_Texture *texture;
    int width, height;
} Texture;

typedef struct {
    float x, y;         /* Player position */
    float angle;        /* Player angle (rotation) */
    float moveSpeed;
    float rotSpeed;
} Player;


typedef struct GameStats 
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    Texture wallTextures[4];
    Texture floorTexture;
    int showMap;
    int maze_map[MAP_WIDTH][MAP_HEIGHT];
} GameStats;


Texture load_texture(const char *filename);
float cast_ray(float playerX, float playerY, float rayAngle);
void draw_sky(void);
void draw_mini_map(Player *player, bool showMap);
float get_wall_hit_coordinates(float playerX, float playerY, float rayAngle, int *mapX, int *mapY);
void draw_floor(Player *player);
void render_walls(Player *player);
void render(Player *player);
void handle_input(Player *player, bool *running, int maze_map[MAP_WIDTH][MAP_HEIGHT]);
int load_map(const char *filename, int maze_map[MAP_WIDTH][MAP_HEIGHT]);



#endif
