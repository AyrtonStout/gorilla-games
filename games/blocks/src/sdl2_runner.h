#ifndef HELLO_SDL2_RUNNER_H
#define HELLO_SDL2_RUNNER_H


#include <SDL_render.h>
#include <SDL_events.h>
#include "block.h"
#include "game_state.h"
#include "sdl2_grid_renderer.h"
#include "sdl_game_textures.h"

class Sdl2Runner {
public:
    explicit Sdl2Runner(GameState *game_state);
    bool update();
private:
    SDL_Renderer *renderer;
    sdl_game_textures textures;

    Sdl2GridRenderer *p1_renderer;
    Sdl2GridRenderer *p2_renderer;

    GameState *game_state;
    bool running = true;

    const int SCALING = 3;
    const int FULL_GAME_WIDTH = 256;
    const int FULL_GAME_HEIGHT = 224;
    const int MULTIPLAYER_GRID_SPACING = 40;

    void load_textures();
    void process_input();
    void process_keypress(SDL_Event event);
};


#endif //HELLO_SDL2_RUNNER_H
