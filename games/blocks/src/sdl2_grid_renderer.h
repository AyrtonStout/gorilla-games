#ifndef HELLO_SDL2_GRID_RENDERER_H
#define HELLO_SDL2_GRID_RENDERER_H


#include <SDL_render.h>
#include "block.h"
#include "game_state.h"
#include "sdl_game_textures.h"

class Sdl2GridRenderer {
public:
    Sdl2GridRenderer(sdl_game_textures *textures, GameGrid *game_grid);
    void render(SDL_Renderer *renderer);
private:
    sdl_game_textures *textures;
    GameGrid *game_grid;

    const int SCALING = 3;
    const int BACKGROUND_WIDTH = 256;
    const int BACKGROUND_HEIGHT = 224;
    const int BACKGROUND_GAME_WIDTH_OFFSET = 80;
    const int BACKGROUND_GAME_HEIGHT_OFFSET = 23;

    int get_draw_point_for_block_height(int coordinate);
};


#endif //HELLO_SDL2_GRID_RENDERER_H
