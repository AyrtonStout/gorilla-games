
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <thread>
#include <map>
#include "game_cursor.h"
#include "direction.h"
#include "game_grid.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace std;
// This is some hacky dev-environment nonsense
#ifndef __EMSCRIPTEN__
const string pathPrefix = "../"; // NOLINT(cert-err58-cpp)
#endif
#ifdef __EMSCRIPTEN__
const string pathPrefix = "./";
#endif

const int scaling = 3;

struct context
{
    SDL_Renderer *renderer;
    map<BlockType, SDL_Texture*> block_textures;

    GameCursor game_cursor;
    GameGrid game_grid;
    bool loop_forever = true;
};

void load_textures(context *ctx, const string& file_path) {
    for (int i = 0; i < BlockType::COUNT; i++) {
        auto block_type = (BlockType) i;
        auto map_entry = Block::block_to_file_name.find(block_type);
        auto file_name = map_entry->second;

        SDL_Surface *image = IMG_Load((file_path + file_name).c_str());
        if (!image) {
            printf("IMG_Load: %s\n", IMG_GetError());
            continue;
        }
        auto texture = SDL_CreateTextureFromSurface(ctx->renderer, image);
        ctx->block_textures.emplace(block_type, texture);

        SDL_FreeSurface (image);
    }
}

void process_keypress(SDL_Event event, GameCursor *cursor) {
    switch (event.key.keysym.sym)
    {
        case SDLK_UP:
            cursor->move(UP); break;
        case SDLK_DOWN:
            cursor->move(DOWN); break;
        case SDLK_LEFT:
            cursor->move(LEFT); break;
        case SDLK_RIGHT:
            cursor->move(RIGHT); break;
        default:
            break;
    }
}

void process_input(context *ctx) {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                process_keypress(event, &ctx->game_cursor); break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    ctx->loop_forever = false;
                }
        }
    }
}

void loop_fn(void* arg) {
    auto *ctx = (context*) arg;

    process_input(ctx);

    SDL_RenderClear(ctx->renderer);

    for (const auto& block_row : ctx->game_grid.blocks) {
        for (auto block : block_row) {
            SDL_Rect rect = {
                    .x = block.get_grid_x() * Block::BLOCK_SIZE * scaling,
                    .y = block.get_grid_y() * Block::BLOCK_SIZE * scaling,
                    .w = Block::BLOCK_SIZE * scaling,
                    .h = Block::BLOCK_SIZE * scaling
            };

            SDL_RenderCopy(ctx->renderer, ctx->block_textures[block.get_block_type()], nullptr, &rect);
        }
    }
    SDL_RenderPresent(ctx->renderer);

#ifdef __EMSCRIPTEN__
    if (!ctx->loop_forever) {
        emscripten_cancel_main_loop();
    }
#endif
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window;

    struct context ctx;

    SDL_CreateWindowAndRenderer(500, 600, 0, &window, &ctx.renderer);

    // Set up a white background
    SDL_SetRenderDrawColor(ctx.renderer, 255, 255, 255, 255);
    SDL_RenderClear(ctx.renderer);

    ctx.game_cursor = GameCursor();

    load_textures(&ctx, pathPrefix + "assets/images/");

    // Show what is in the renderer
    SDL_RenderPresent(ctx.renderer);

#ifdef __EMSCRIPTEN__
    // If using emscripten, have our main loop be browser friendly
  emscripten_set_main_loop_arg(loop_fn, &ctx, -1, 1);
  return 0;
#endif

#ifndef __EMSCRIPTEN__
    // Otherwise, do a janky normal loop
    while (ctx.loop_forever) {
        loop_fn(&ctx);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
#endif

    return 0;
}

