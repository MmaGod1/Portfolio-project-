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

extern int maze_map[MAP_WIDTH][MAP_HEIGHT];


extern Texture wallTextures[4];
extern Texture floorTexture;
extern int maze_map[MAP_WIDTH][MAP_HEIGHT];
extern int showMap;                                               extern SDL_Window *window;
extern SDL_Renderer *renderer;


/* Function prototypes */
Texture load_texture(const char *filename);
void calculate_step_and_side_dist(float rayDirX, float rayDirY,
	float playerX, float playerY, int *mapX, int *mapY,
	int *stepX, int *stepY, float *sideDistX, float *sideDistY);
void perform_DDA(int stepX, int stepY, float *sideDistX,
                  float *sideDistY, int *mapX, int *mapY,
                  int *hit, int *side, float rayAngle);
float cast_ray(float playerX, float playerY, float rayAngle);
void draw_sky(void);
void draw_mini_map(Player *player, bool showMap);
void draw_map_tiles(int mapStartX, int mapStartY, int tileSize);
void draw_floor(Player *player);
float get_wall_hit_coordinates(float playerX, float playerY, float rayAngle, int *mapX, int *mapY);
void calculate_wall_dimensions(int distance, Player *player, 
    int *wallHeight, int *wallTop, int *wallBottom);

void render_walls(GameStats *gameStats, Player *player);
void render_walls(Player *player);
void render_walls(Player *player);
void render(Player *player);
void process_rotation(Player *player, int direction);
void process_movement(Player *player, int maze_map[MAP_WIDTH][MAP_HEIGHT], float moveStep, float moveAngle);
void process_quit_event(SDL_Event event, bool *running);
void process_keydown_event(SDL_Event event, Player *player, bool *running, int maze_map[MAP_WIDTH][MAP_HEIGHT]);
void handle_input(Player *player, bool *running, int maze_map[MAP_WIDTH][MAP_HEIGHT]);
int read_next_char(FILE *file);
int process_char(int ch, int x, int y, int maze_map[MAP_WIDTH][MAP_HEIGHT]);
int load_map(const char *filename, int maze_map[MAP_WIDTH][MAP_HEIGHT]);
int load_resources(char *mapfile);
int initialize_sdl(void);
void initialize_player(Player *player);
void cleanup(void);

#endif /* RAYCASTING_H */
