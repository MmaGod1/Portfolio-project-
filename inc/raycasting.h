#ifndef RAYCASTING_H
#define RAYCASTING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
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


/* Structure to represent the player */
typedef struct {
	float x, y;           /* Player position */
	float angle;          /* Player angle (rotation) */
	float moveSpeed;
	float rotSpeed;
} Player;

/* Structure to represent a texture */
typedef struct {
	SDL_Texture *texture;
	int width, height;
} Texture;

typedef struct {
    Texture wallTextures[4];
    Texture floorTexture;
    SDL_Renderer *renderer;
} GameStats;
extern int maze_map[MAP_WIDTH][MAP_HEIGHT];

/* Function prototypes */
float normalize_angle(float angle);
Texture load_texture(SDL_Renderer *renderer, const char *file);

void calculate_wall_dimensions(float distance, Player *player,
float rayAngle, int *wallHeight, int *wallTop, int *wallBottom);

void perform_DDA(int stepX, int stepY, float *sideDistX,
                  float *sideDistY, int *mapX, int *mapY,
                  int *hit, int *side);
float cast_ray(float playerX, float playerY, float rayAngle);
void draw_sky(SDL_Renderer *renderer);
void draw_mini_map(SDL_Renderer *renderer, Player *player, bool showMap);
void draw_map_tiles(SDL_Renderer *renderer, int mapStartX, int mapStartY, int tileSize);
void draw_floor(SDL_Renderer *renderer, Player *player, Texture floorTexture);
float get_wall_hit_coordinates(float playerX, float playerY, float rayAngle, int *mapX, int *mapY);

void render_walls(GameStats *gameStats, Player *player);

void render_single_wall(GameStats *gameStats,
int x, int wallHeight, int wallTop,
float wallX, int wallTextureIndex);

void render(Player *player, bool showMap, SDL_Renderer *renderer, GameStats *gameStats);
void process_rotation(Player *player, int direction);
void process_movement(Player *player, int maze_map[MAP_WIDTH][MAP_HEIGHT], float moveStep, float moveAngle);
void process_quit_event(SDL_Event event, bool *running);
void process_keydown_event(
	SDL_Event event,
	Player *player,
	bool *running,
	int maze_map[MAP_WIDTH][MAP_HEIGHT],
	bool showMap
);
int read_next_char(FILE *file);
int process_char(int ch, int x, int y, int maze_map[MAP_WIDTH][MAP_HEIGHT]);
int load_map(const char *filename, int maze_map[MAP_WIDTH][MAP_HEIGHT]);
int load_resources(GameStats *gameStats, char *mapfile);
int initialize_sdl(SDL_Window **window, SDL_Renderer **renderer);
void initialize_player(Player *player);
void cleanup(SDL_Renderer *renderer, SDL_Window *window, GameStats *gameStats);

#endif /* RAYCASTING_H */
