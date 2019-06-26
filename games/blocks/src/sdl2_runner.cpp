#include <SDL_image.h>
#include <thread>
#include <iostream>
#include "sdl2_runner.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// This is some hacky dev-environment nonsense
#ifndef __EMSCRIPTEN__
const string pathPrefix = "../"; // NOLINT(cert-err58-cpp)
#else
const string pathPrefix = "./";
#endif

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"

#pragma clang diagnostic ignored "-Wunknown-pragmas"
Sdl2Runner::Sdl2Runner(GameState *game_state) {
    this->game_state = game_state;

    this->p1_renderer = new Sdl2GridRenderer(&this->textures, &game_state->game_grid);
    this->p2_renderer = nullptr;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window;

    SDL_CreateWindowAndRenderer(FULL_GAME_WIDTH * SCALING, FULL_GAME_HEIGHT * SCALING, 0, &window, &renderer);

    // Set up a white background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    load_textures();

    // Show what is in the renderer
    SDL_RenderPresent(renderer);
}


void Sdl2Runner::load_textures() {
    const string file_path = pathPrefix + "assets/images/";

    // Cursor
    SDL_Surface *cursor_image = IMG_Load((file_path + "cursor.png").c_str());
    if (!cursor_image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return;
    }

    textures.cursor_texture = SDL_CreateTextureFromSurface(renderer, cursor_image);
    SDL_FreeSurface(cursor_image);


    // Background
    SDL_Surface *background_image = IMG_Load((file_path + "game-background.png").c_str());
    if (!background_image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return;
    }

    textures.background_texture = SDL_CreateTextureFromSurface(renderer, background_image);
    SDL_FreeSurface(background_image);


    // Black tint
    SDL_Surface *black_tint_image = IMG_Load((file_path + "black-tint.png").c_str());
    if (!black_tint_image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return;
    }

    textures.black_tint_texture = SDL_CreateTextureFromSurface(renderer, black_tint_image);
    SDL_FreeSurface(black_tint_image);


    // Normal block images
    for (int i = 0; i < BlockType::COUNT; i++) {
        auto block_type = (BlockType) i;
        auto map_entry = Block::block_to_file_name.find(block_type);
        auto file_name = map_entry->second + ".png";

        SDL_Surface *image = IMG_Load((file_path + file_name).c_str());
        if (!image) {
            printf("IMG_Load: %s\n", IMG_GetError());
            continue;
        }
        auto texture = SDL_CreateTextureFromSurface(renderer, image);
        textures.block_textures.emplace(block_type, texture);

        SDL_FreeSurface(image);
    }


    // Flashing block images
    for (int i = 0; i < BlockType::COUNT; i++) {
        auto block_type = (BlockType) i;
        auto map_entry = Block::block_to_file_name.find(block_type);
        auto file_name = map_entry->second + "-flash.png";

        SDL_Surface *image = IMG_Load((file_path + file_name).c_str());
        if (!image) {
            printf("IMG_Load: %s\n", IMG_GetError());
            continue;
        }
        auto texture = SDL_CreateTextureFromSurface(renderer, image);
        textures.block_flash_textures.emplace(block_type, texture);

        SDL_FreeSurface(image);
    }


    // Face block images
    for (int i = 0; i < BlockType::COUNT; i++) {
        auto block_type = (BlockType) i;
        auto map_entry = Block::block_to_file_name.find(block_type);
        auto file_name = map_entry->second + "-face.png";

        SDL_Surface *image = IMG_Load((file_path + file_name).c_str());
        if (!image) {
            printf("IMG_Load: %s\n", IMG_GetError());
            continue;
        }
        auto texture = SDL_CreateTextureFromSurface(renderer, image);
        textures.block_face_textures.emplace(block_type, texture);

        SDL_FreeSurface(image);
    }


    // Combo images
    for (int i = 4; i <= GameGrid::MAX_COMBO; i++) {
        SDL_Surface *image = IMG_Load((file_path + "combo/combo-" + to_string(i) + ".png").c_str());
        if (!image) {
            printf("IMG_Load: %s\n", IMG_GetError());
            continue;
        }
        auto texture = SDL_CreateTextureFromSurface(renderer, image);
        textures.combo_textures.emplace(i, texture);

        SDL_FreeSurface(image);
    }

    SDL_Surface *combo_unknown_image = IMG_Load((file_path + "combo/combo-unknown.png").c_str());
    if (!combo_unknown_image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return;
    }

    textures.combo_unknown_texture = SDL_CreateTextureFromSurface(renderer, combo_unknown_image);
    SDL_FreeSurface(combo_unknown_image);


    // Chain images
    for (int i = 2; i <= GameGrid::MAX_COMBO; i++) {
        SDL_Surface *image = IMG_Load((file_path + "chain/chain-" + to_string(i) + ".png").c_str());
        if (!image) {
            printf("IMG_Load: %s\n", IMG_GetError());
            continue;
        }
        auto texture = SDL_CreateTextureFromSurface(renderer, image);
        textures.chain_textures.emplace(i, texture);

        SDL_FreeSurface(image);
    }

    SDL_Surface *chain_unknown_image = IMG_Load((file_path + "chain/chain-unknown.png").c_str());
    if (!chain_unknown_image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return;
    }

    textures.chain_unknown_texture = SDL_CreateTextureFromSurface(renderer, chain_unknown_image);
    SDL_FreeSurface(chain_unknown_image);
}

void Sdl2Runner::process_keypress(SDL_Event event) {
    auto cursor = &game_state->game_grid.game_cursor;
    auto game_grid = &game_state->game_grid;

    switch (event.key.keysym.sym)
    {
        case SDLK_UP:
            game_grid->move_cursor(Direction::UP); break;
        case SDLK_DOWN:
            game_grid->move_cursor(Direction::DOWN); break;
        case SDLK_LEFT:
            game_grid->move_cursor(Direction::LEFT); break;
        case SDLK_RIGHT:
            game_grid->move_cursor(Direction::RIGHT); break;
        case SDLK_z:
            game_grid->swap_panels(cursor->x, cursor->y); break;
        case SDLK_x:
            game_grid->stack_raise_requested = true;
        default:
            break;
    }
}

void Sdl2Runner::process_input() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                process_keypress(event);
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_x:
                        game_state->game_grid.stack_raise_requested = false;
                        break;
                }
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    running = false;
                }
                break;
        }
    }
}

bool Sdl2Runner::update() {
    process_input();

    SDL_RenderClear(renderer);

    p1_renderer->render(renderer);

    SDL_RenderPresent(renderer);

    return running;
}

