#ifndef HELLO_SDL2_RUNNER_H
#define HELLO_SDL2_RUNNER_H


#include <SDL_render.h>
#include <SDL_events.h>
#include "block.h"
#include "game_state.h"

class Sdl2Runner {
public:
    Sdl2Runner(GameState *game_state);
    void run();
    void update();
private:
    SDL_Renderer *renderer;
    map<BlockType, SDL_Texture*> block_textures;
    SDL_Texture *cursor_texture;
    GameState *game_state;
    bool running = true;

    const int SCALING = 3;

    void load_textures();
    void process_input();
    void process_keypress(SDL_Event event);
};


#endif //HELLO_SDL2_RUNNER_H
