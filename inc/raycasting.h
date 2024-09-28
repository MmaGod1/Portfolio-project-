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





void render(Player *player, GameStats *gameStats);
void draw_sky(GameStats *gameStats);
void draw_floor(GameStats *gameStats, Player *player);



void draw_map(GameStats *gameStats, bool showMap);
void draw_player_and_los(GameStats *gameStats, Player *player);
void draw_mini_map(GameStats *gameStats, Player *player, bool showMap);








int open_map_file(const char *filename, FILE **file);
int read_character(FILE *file, int *ch);
int process_character(GameStats *gameStats, int ch, int x, int y);
int load_map(GameStats *gameStats, const char *filename);



Texture load_texture(GameStats *gameStats, const char *filename);



void process_movement(Player *player, SDL_Keycode key, float *newX, float *newY);
void update_player_angle(Player *player, SDL_Keycode key);
bool check_new_position(GameStats *gameStats, float newX, float newY);

void handle_input(Player *player, bool *running, GameStats *gameStats);








void initialize_ray(float rayAngle, float *rayDirX, float *rayDirY, 
                    int *stepX, int *stepY, float playerX, float playerY,
                    int *mapX, int *mapY, float *sideDistX, 
                    float *sideDistY, float *deltaDistX, 
                    float *deltaDistY);

float calculate_distance(int stepX, int stepY, int *mapX, int *mapY, 
                         float playerX, float playerY, 
                         float rayDirX, float rayDirY, 
                         GameStats *gameStats);

float cast_ray(GameStats *gameStats, float playerX, float playerY, 
                float rayAngle);



float get_wall_hit_coordinates(GameStats *gameStats, float playerX, 
                               float playerY, float rayAngle, 
                               int *mapX, int *mapY);

void render_walls(GameStats *gameStats, Player *player);


#endif
