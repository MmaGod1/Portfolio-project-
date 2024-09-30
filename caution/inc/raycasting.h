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
#define FOV (M_PI / 3) /* 60 degrees field of view */
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


int initialize_sdl(GameStats *gameStats);
int initialize_game(GameStats *gameStats, Player *player, char *mapFile);

void render_walls(GameStats *gameStats, Player *player);
void render_wall_segment(GameStats *gameStats, Player *player, 
                         float rayAngle, int x, 
                         int wallTop, int wallHeight);
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
int load_game_textures(GameStats *gameStats);


void process_movement(Player *player, SDL_Keycode key, float *newX, float *newY);
void update_player_angle(Player *player, SDL_Keycode key);
bool check_new_position(GameStats *gameStats, float newX, float newY);
void handle_input(Player *player, bool *running, GameStats *gameStats);






void initialize_ray(float playerX, float playerY, float rayAngle,
                    int *mapX, int *mapY, int *stepX, int *stepY,
                    float *sideDistX, float *sideDistY,
                    float *deltaDistX, float *deltaDistY);

float perform_ray_cast(GameStats *gameStats, int *mapX, int *mapY,
                       int stepX, int stepY, float sideDistX,
                       float sideDistY, float deltaDistX, float deltaDistY,
                       float playerX, float playerY, float rayAngle);

float cast_ray(GameStats *gameStats, float playerX, float playerY, 
                float rayAngle);





void init_ray_direction(float *rayDirX, float *rayDirY, float rayAngle);
void init_map_coordinates(float playerX, float playerY, int *mapX, int *mapY);
void calculate_step_and_initial_side_dist(float playerX, float playerY,
                                          int *mapX, int *mapY,
                                          float rayDirX, float rayDirY,
                                          int *stepX, int *stepY,
                                          float *sideDistX, float *sideDistY,
                                          float deltaDistX, float deltaDistY);
int dda_raycast_step(int *mapX, int *mapY, float *sideDistX, float *sideDistY,
                float deltaDistX, float deltaDistY, int stepX, int stepY,
                int *side, GameStats *gameStats);
float get_wall_hit_coordinates(GameStats *gameStats, float playerX,
                               float playerY, float rayAngle,
                               int *mapX, int *mapY);

float get_wall_hit_coordinates(GameStats *gameStats, float playerX, 
                               float playerY, float rayAngle, 
                               int *mapX, int *mapY);



void clean_up(GameStats *gameStats);

#endif
