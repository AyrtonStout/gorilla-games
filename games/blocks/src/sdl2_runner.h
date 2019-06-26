#ifndef HELLO_SDL2_RUNNER_H
#define HELLO_SDL2_RUNNER_H


#include <SDL_render.h>
#include <SDL_events.h>
#include "block.h"
#include "game_state.h"

class Sdl2Runner {
public:
    explicit Sdl2Runner(GameGrid *game_grid);
    bool update();
private:
    SDL_Renderer *renderer;
    map<BlockType, SDL_Texture*> block_textures;
    map<BlockType, SDL_Texture*> block_flash_textures;
    map<BlockType, SDL_Texture*> block_face_textures;
    map<int, SDL_Texture*> combo_textures;
    map<int, SDL_Texture*> chain_textures;
    SDL_Texture *combo_unknown_texture;
    SDL_Texture *chain_unknown_texture;
    SDL_Texture *cursor_texture;
    SDL_Texture *background_texture;
    SDL_Texture *black_tint_texture;
    GameGrid *game_grid;
    bool running = true;

    const int SCALING = 3;
    const int BACKGROUND_WIDTH = 256;
    const int BACKGROUND_HEIGHT = 224;
    const int BACKGROUND_GAME_WIDTH_OFFSET = 80;
    const int BACKGROUND_GAME_HEIGHT_OFFSET = 23;

    void load_textures();
    void process_input();
    void process_keypress(SDL_Event event);
    int get_draw_point_for_block_height(int coordinate);
};


#endif //HELLO_SDL2_RUNNER_H
