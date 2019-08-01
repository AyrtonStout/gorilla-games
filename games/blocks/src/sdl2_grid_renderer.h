#ifndef HELLO_SDL2_GRID_RENDERER_H
#define HELLO_SDL2_GRID_RENDERER_H


#include <SDL_render.h>
#include "block.h"
#include "game_state.h"
#include "sdl_game_textures.h"

class Sdl2GridRenderer {
public:
    static const int GRID_WIDTH = Block::BLOCK_SIZE * GameGrid::GAME_WIDTH;

    Sdl2GridRenderer(sdl_game_textures *textures, GameGrid *game_grid, int num_players, int player);
    void render(SDL_Renderer *renderer, int start_x, int scaling);
private:
    sdl_game_textures *textures;
    GameGrid *game_grid;
    int num_players;
    int player;

    const int BACKGROUND_HEIGHT = 224;
    const int BACKGROUND_GAME_HEIGHT_OFFSET = 23;

    int get_draw_point_for_block_height(int coordinate, int scaling);
};


#endif //HELLO_SDL2_GRID_RENDERER_H
