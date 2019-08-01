//
// Created by mobius on 6/25/19.
//

#ifndef HELLO_SDL_GAME_TEXTURES_H
#define HELLO_SDL_GAME_TEXTURES_H

#include <SDL_render.h>
#include "block.h"

struct sdl_game_textures {
    map<BlockType, SDL_Texture*> block_textures;
    map<BlockType, SDL_Texture*> block_flash_textures;
    map<BlockType, SDL_Texture*> block_face_textures;
    map<int, SDL_Texture*> combo_textures;
    map<int, SDL_Texture*> chain_textures;
    SDL_Texture *combo_unknown_texture;
    SDL_Texture *chain_unknown_texture;
    SDL_Texture *cursor_texture;
    SDL_Texture *background_texture;
    SDL_Texture *background_texture_2player;
    SDL_Texture *black_tint_texture;
};

#endif //HELLO_SDL_GAME_TEXTURES_H
