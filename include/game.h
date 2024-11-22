#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TARGET_FPS 60
#define NS_PER_FRAME (1. / TARGET_FPS) * 1000000000
#define WIDTH 800
#define HEIGHT 600
#define TILE_SIDE 50
#define GRID_X (WIDTH / TILE_SIDE)
#define GRID_Y (HEIGHT / TILE_SIDE)

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct {
    float x, y, x_vel, y_vel;
    Direction direction;
    SDL_Texture *texture[4];
} Player;

typedef struct {
    bool run;
    uint64_t last_frame_time;
    uint8_t frame_count;
    SDL_FRect grid[GRID_X][GRID_Y];
    SDL_Renderer *renderer;
    SDL_Window *window;
    Player player;
} GameState;

GameState init(void);
void reset(GameState *state);
void cleanup(GameState *state);
void process_input(GameState *state);
void update(GameState *state);
void render(GameState *state);
void draw_background(GameState *state);
