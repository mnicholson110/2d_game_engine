#include "../include/game.h"

int main(void) {
    GameState state = init();

    while (state.run) {
        process_input(&state);
        update(&state);
        render(&state);
    }

    cleanup(&state);
    return 0;
}

void process_input(GameState *state) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            state->run = false;
            return;
        } else if (event.type == SDL_EVENT_KEY_DOWN) {
            switch (event.key.scancode) {
            case SDL_SCANCODE_ESCAPE:
                state->run = false;
                return;
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                if (state->player.y > 0)
                    state->player.y -= TILE_SIDE;
                return;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                if (state->player.x > 0)
                    state->player.x -= TILE_SIDE;
                return;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                if (state->player.y < ((GRID_Y - 1) * TILE_SIDE))
                    state->player.y += TILE_SIDE;
                return;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                if (state->player.x < ((GRID_X - 1) * TILE_SIDE))
                    state->player.x += TILE_SIDE;
                return;
            default:
                return;
            }
        }
    }
}

void update(GameState *state) {
}

void render(GameState *state) {
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);
    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
    // draw the background (ALL SCENES)
    for (int r = 0; r < GRID_Y; r++) {
        for (int c = 0; c < GRID_X; c++) {
            SDL_RenderRect(state->renderer, &(SDL_FRect){c * TILE_SIDE, r * TILE_SIDE, TILE_SIDE, TILE_SIDE});
        }
    }

    // draw the player-
    SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(state->renderer,
                       &(SDL_FRect){
                           state->player.x + .1 * TILE_SIDE,
                           state->player.y + .1 * TILE_SIDE,
                           .8 * TILE_SIDE,
                           .8 * TILE_SIDE});

    SDL_RenderPresent(state->renderer);

    // Frame limiting
    uint64_t elapsed_time = SDL_GetTicksNS() - state->last_frame_time;
    if (elapsed_time < NS_PER_FRAME)
        SDL_DelayNS(NS_PER_FRAME - elapsed_time);
    state->last_frame_time = SDL_GetTicksNS();
    state->frame_count++;
    state->frame_count %= 60;
}

// SDL_Texture *create_text_texture(SDL_Renderer *renderer, TTF_Font *font, char *text) {
//     SDL_Color color = {0, 0, 0, 255};
//     SDL_Surface *surface = TTF_RenderText_Blended(font, text, (size_t)NULL, color);
//     if (!surface) {
//         fprintf(stderr, "Failed to render text: %s\n", SDL_GetError());
//         fprintf(stderr, "%s\n", text);
//         return NULL;
//     }
//     SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
//     SDL_DestroySurface(surface);
//     return texture;
// }

GameState init() {
    srand((unsigned int)time(NULL));
    GameState state = {0};

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //    if (!TTF_Init()) {
    //        fprintf(stderr, "Failed to initialize SDL_ttf: %s\n", SDL_GetError());
    //        SDL_Quit();
    //        exit(EXIT_FAILURE);
    //    }

    //    state.font = TTF_OpenFont("assets/font.ttf", FONT_SIZE);
    //    if (!state.font) {
    //        fprintf(stderr, "Failed to load font: %s\n", SDL_GetError());
    //        exit(EXIT_FAILURE);
    //    }

    state.window = SDL_CreateWindow("2D Starting Point", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN);
    state.renderer = SDL_CreateRenderer(state.window, NULL);
    SDL_SetRenderLogicalPresentation(state.renderer, WIDTH, HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    for (int x = 0; x < GRID_X; x++) {
        for (int y = 0; y < GRID_Y; y++) {
            state.grid[x][y] = (SDL_FRect){y * TILE_SIDE, x * TILE_SIDE, TILE_SIDE, TILE_SIDE};
        }
    }

    state.run = true;
    state.last_frame_time = SDL_GetTicksNS();
    state.player.x = 0;
    state.player.y = 0;
    state.player.direction = RIGHT;
    // this should set all the different directions.
    // TODO: create an Animation struct? to use for input actions?
    state.player.texture[0] = NULL;

    return state;
}

void cleanup(GameState *state) {
    //    TTF_CloseFont(state->font);
    SDL_DestroyRenderer(state->renderer);
    SDL_DestroyWindow(state->window);
    //    TTF_Quit();
    SDL_Quit();
}

void reset(GameState *state) {
    state->run = true;
    state->last_frame_time = SDL_GetTicksNS();
}
